#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cmath>
#include "Shader.h"

std::string vertexShader;
std::string fragmentShader;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window,int width,int height);
void read_shaders(const char* filevertex,const char* filefragment);
const unsigned int scr_width = 800;
const unsigned int scr_height = 600;

void read_shaders(const char* filevertex, const char* filefragment) {
	std::ifstream vertexfile(filevertex);
	std::ifstream fragmentfile(filefragment);

	if (vertexfile.fail()) {
		std::cout << "Failed to load vertx shader \n" << std::endl;
	}
	if (fragmentfile.fail()) {
		std::cout << "Failed to load fragment shader \n" << std::endl;
	}

	std::string linevertex;
	std::string linefrag;
	while (std::getline(vertexfile, linevertex)) {
		vertexShader += linevertex + "\n";
	}
	while (std::getline(fragmentfile, linefrag)) {
		fragmentShader += linefrag + "\n";
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(scr_width,scr_height,"Triangle",NULL,NULL);
	if (window == NULL) {
		std::cout << "Failed to Create Window"<<std::endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

	//check if glad initiated
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialized GLAD" << std::endl;
		return - 1;
	}

	Shader ourshader("shaders/shader.vs","shaders/fragment.vs");
/*
	//create shaders
	read_shaders("shaders/shader.vs","shaders/fragment.vs");
	const char* vertshader = vertexShader.c_str();
	const char* fragshader = fragmentShader.c_str();
	int success;
	char infolog[512];
	int vertexshadID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshadID,1,&vertshader,NULL);
	glCompileShader(vertexshadID);
	glGetShaderiv(vertexshadID,GL_COMPILE_STATUS,&success);
	if (!success) {
		glGetShaderInfoLog(vertexshadID,512,NULL,infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED \n" << infolog << std::endl;
	}
	int fragmentshadID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshadID, 1, &fragshader, NULL);
	glCompileShader(fragmentshadID);
	glGetShaderiv(fragmentshadID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentshadID, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED \n" << infolog << std::endl;
	}

	//create a program to link shaders to
	int shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID,vertexshadID);
	glAttachShader(shaderProgramID, fragmentshadID);
	glLinkProgram(shaderProgramID);
	glGetProgramiv(shaderProgramID,GL_LINK_STATUS,&success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID,512,NULL,infolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED \n"<<infolog << std::endl;
	}
	glDeleteShader(vertexshadID);
	glDeleteShader(fragmentshadID);
	/*
	float vertices[] = {
	-0.5f , -0.5f , 0.0f,
	0.5f , -0.5f , 0.0f,
	0.0f , 0.5f , 0.0f
	};*/
	
	/*
	float vertices[] = {
		-0.5f , -0.5f , 0.0f , 1.0f , 0.0f , 0.0f,
		0.5f , -0.5f , 0.0f , 0.0f , 1.0f , 0.0f, 
		0.0f , 0.5f , 0.0f , 0.0f , 0.0f , 1.0f
	};
	*/

	float vertices[] = {
		//positions				//colors				//texture coords
		-0.5f ,-0.5f , 0.0f ,	 1.0f , 0.0f , 0.0f ,		1.0f , 1.0f,
		 0.5f ,-0.5f , 0.0f ,	 0.0f , 1.0f , 0.0f ,	    1.0f , 0.0f,
		 0.0f , 0.5f , 0.0f ,	 0.0f , 0.0f , 1.0f ,		0.0f , 0.0f,
		-0.5f , 0.5f , 0.0f ,	 1.0f , 1.0f , 0.0f ,		0.0f , 1.0f 
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//bind vertex object
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	//create generic vertex attribute  data
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	//create attrivute pointer for colour data with offset of 12
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture loading
	


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourshader.use();

		//float timeVal = glfwGetTime();
		//float sinvalue = sin(timeVal)+0.2;
		//float cosvalue = cos(timeVal)-0.2;
		//int vertexColorLocation = glGetUniformLocation(shaderProgramID,"ourColor");
		//glUniform4f(vertexColorLocation,sinvalue,0.0f,cosvalue,1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width, int height) {
	glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window,true);
	}
}