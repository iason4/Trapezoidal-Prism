//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1Β της OpenGL
//
//ΑΜ:4819                        Όνομα:ΙΑΣΩΝ ΤΣΑΤΣΗΣ

//*********************************

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <random>//

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

//************************
// Βοηθητικές συναρτήσεις

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

///****************************************************************
//  Εδω θα υλοποιήσετε την συνάρτηση της κάμερας
//****************************************************************

void camera_function(GLFWwindow* window,glm::vec3& rotation,glm::vec3& scale)
{
	//rotation upwards on x 
	if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
    		 rotation.x += 1.0f; 
	}
	//rotation downwards on x
	if (glfwGetKey(window, GLFW_KEY_X ) == GLFW_PRESS){
    		 rotation.x -= 1.0f;
	}
	//rotation clockwise on z
	if (glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS){
   		 rotation.z += 1.0f;
	}
	//rotation left turn on z
	if (glfwGetKey(window, GLFW_KEY_Z ) == GLFW_PRESS){
    		 rotation.z -= 1.0f;
	}
	//scale up
	if (glfwGetKey(window, GLFW_KEY_KP_ADD ) == GLFW_PRESS){
   		 scale += 0.1f;
	}
	//scale down
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT ) == GLFW_PRESS){
    		 scale -= 0.1f;
	}
}



//************************************
// Η LoadShaders είναι black box για σας
//************************************
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

//Height Function
void height(GLFWwindow* window,glm::vec3& Height)
{
	//height increase
	if (glfwGetKey(window, GLFW_KEY_U ) == GLFW_PRESS){
   		Height.z += 0.01f;	
	}	
	//height reduction
	if (glfwGetKey(window, GLFW_KEY_P ) == GLFW_PRESS){
    		Height.z -= 0.01f;
	}
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1000, 1000, u8"Εργασία 1Β – Τραπεζοειδές Πρίσμα", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the space key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	
	GLuint programID = LoadShaders("ProjBVertexShader.vertexshader", "ProjBFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	
	
	//Height,rotation,scale
	glm::vec3 rotation(0.0f);
	glm::vec3 scale(1.0f);
	glm::vec3 Height(1.0f);
	
	//********************
	// **Δείγμα κλήσης των συναρτήσεων για δημιουργία του MVP - είναι τυχαίες οι ρυθμίσεις και δεν ανταποκρίνονται στην άσκησή σας
	
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 20.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f)    
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	
	//height
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(2.0, 10.0);
 
	float h = dist(gen);
	
	static const GLfloat g_vertex_buffer_data[] = {
		-4.0f,-3.0f,(h/2.0f),//1
		-1.0f,3.0f, (h/2.0f),
		4.0f,-3.0f, (h/2.0f),
		-1.0f,3.0f,(h/2.0f),//2
		1.0f,3.0f, (h/2.0f),
		4.0f,-3.0f, (h/2.0f),
		-4.0f,-3.0f,0.0f,//3
		-4.0f,-3.0f,(h/2.0f),
		4.0f,-3.0f, (h/2.0f),
		-4.0f,-3.0f,0.0f,//4
		4.0f,-3.0f,0.0f,
		4.0f,-3.0f, (h/2.0f),
		-1.0f,3.0f,(h/2.0f),//5
		-1.0f,3.0f,0.0f,
		1.0f,3.0f, 0.0f,
		-1.0f,3.0f,(h/2.0f),//6
		1.0f,3.0f,0.0f,
		1.0f,3.0f, (h/2.0f),
		4.0f,-3.0f,(h/2.0f),//7
		1.0f,3.0f,0.0f,
		1.0f,3.0f, (h/2.0f),
		4.0f,-3.0f,(h/2.0f),//8
		4.0f,-3.0f,0.0f,
		1.0f,3.0f, 0.0f,
		-4.0f,-3.0f,(h/2.0f),//9
		-4.0f,-3.0f,0.0f,
		-1.0f,3.0f, (h/2.0f),
		-4.0f,-3.0f,0.0f,//10
		-1.0f,3.0f,0.0f,
		-1.0f,3.0f, (h/2.0f),
		-4.0f,-3.0f,-(h/2.0f),//11
		-1.0f,3.0f, -(h/2.0f),
		4.0f,-3.0f, -(h/2.0f),
		-1.0f,3.0f,-(h/2.0f),//12
		1.0f,3.0f, -(h/2.0f),
		4.0f,-3.0f, -(h/2.0f),
		-4.0f,-3.0f,0.0f,//13
		-4.0f,-3.0f,-(h/2.0f),
		4.0f,-3.0f, -(h/2.0f),
		-4.0f,-3.0f,0.0f,//14
		4.0f,-3.0f,0.0f,
		4.0f,-3.0f, -(h/2.0f),
		-1.0f,3.0f,-(h/2.0f),//15
		-1.0f,3.0f,0.0f,
		1.0f,3.0f, 0.0f,
		-1.0f,3.0f,-(h/2.0f),//16
		1.0f,3.0f,0.0f,
		1.0f,3.0f, -(h/2.0f),
		4.0f,-3.0f,-(h/2.0f),//17
		1.0f,3.0f,0.0f,
		1.0f,3.0f, -(h/2.0f),
		4.0f,-3.0f,-(h/2.0f),//18
		4.0f,-3.0f,0.0f,
		1.0f,3.0f, 0.0f,
		-4.0f,-3.0f,-(h/2.0f),//19
		-4.0f,-3.0f,0.0f,
		-1.0f,3.0f, -(h/2.0f),
		-4.0f,-3.0f,0.0f,//20
		-1.0f,3.0f,0.0f,
		-1.0f,3.0f, -(h/2.0f)
		
		
	};
	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.5f,  0.0f,  0.0f,//1 red
		0.5f,  0.0f,  0.0f,
		0.5f,  0.0f,  0.0f,
		0.5f,  0.0f,  0.0f,//2 red
		0.5f,  0.0f,  0.0f,
		0.5f,  0.0f,  0.0f,
		0.0f,  0.5f,  0.0f,//3 green
		0.0f,  0.5f,  0.0f,
		0.0f,  0.5f,  0.0f,
		0.0f,  0.5f,  0.0f,//4 green
		0.0f,  0.5f,  0.0f,
		0.0f,  0.5f,  0.0f,
		1.0f,  0.0f,  1.0f,//5 pink
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,//6 pink
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		0.5f,  0.0f, 1.0f,//7 purple
		0.5f,  0.0f, 1.0f,
		0.5f,  0.0f, 1.0f,
		0.5f,  0.0f, 1.0f,//8 purple
		0.5f,  0.0f, 1.0f,
		0.5f,  0.0f, 1.0f,
		0.0f,  0.5f,  0.5f,//9 green-blue
		0.0f,  0.5f,  0.5f,
		0.0f,  0.5f,  0.5f,
		0.0f,  0.5f,  0.5f,//10 green-blue
		0.0f,  0.5f,  0.5f,
		0.0f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.0f,//11 yellow
		0.5f,  0.5f,  0.0f,
		0.5f,  0.5f,  0.0f,
		0.5f,  0.5f,  0.0f,//12 yellow
		0.5f,  0.5f,  0.0f,
		0.5f,  0.5f,  0.0f,
		0.0f,  0.5f,  0.0f,//13 green
		0.0f,  0.5f,  0.0f,
		0.0f,  0.5f,  0.0f,
		0.0f,  0.5f,  0.0f,//14 green
		0.0f,  0.5f,  0.0f,
		0.0f,  0.5f,  0.0f,
		1.0f,  0.0f,  1.0f,//15 pink
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,//16 pink
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		0.5f,  0.0f, 1.0f,//17 purple
		0.5f,  0.0f, 1.0f,
		0.5f,  0.0f, 1.0f,
		0.5f,  0.0f, 1.0f,//18 purple
		0.5f,  0.0f, 1.0f,
		0.5f,  0.0f, 1.0f,
		0.0f,  0.5f,  0.5f,//19 green-blue
		0.0f,  0.5f,  0.5f,
		0.0f,  0.5f,  0.5f,
		0.0f,  0.5f,  0.5f,//20 green-blue
		0.0f,  0.5f,  0.5f,
		0.0f,  0.5f,  0.5f
		
	};



	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		
		
		//MVP
		//*************************************************
		 glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(10.0f, 50.0f, 0.0f), 
			glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f, 0.0f, 1.0f)  
		);
		View = glm::rotate(View,glm::radians(rotation.x),glm::vec3(1.0f,0.0f,0.0f));
		View = glm::rotate(View,glm::radians(rotation.z),glm::vec3(0.0f,0.0f,1.0f));
		View = glm::scale(View,scale);
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::scale(Model,Height);
		//adjust the height
		height(window,Height);
		//camera with keyboard
		camera_function(window,rotation,scale);
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
		// **************************************************/
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 20*3); // 3 indices starting at 0 -> 1 triangle
		

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		
	} // Check if the scape key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

