#define STB_IMAGE_IMPLEMENTATION

#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cmath>
#include "Shader.h"
#include <stb_image.h>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include<glm\glm\gtc\type_ptr.hpp>

int texwidth, texheight;
unsigned char* load_texture(const char* filename)
{
	int  nrChannels;
	unsigned char *imgdata = stbi_load(filename, &texwidth, &texheight, &nrChannels, 0);
	return imgdata;
}

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

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f  ,  0.0f   , 0.0f),
		glm::vec3(2.0f  ,  5.0f   , -15.0f),
		glm::vec3(-1.5f ,  -2.2f  , -2.5f),
		glm::vec3(-3.8f ,  -2.0f  , -12.3f),
		glm::vec3(2.4f  ,  -0.4f  , -3.5f),
		glm::vec3(-1.7f ,  3.0f   , -7.5f),
		glm::vec3(1.3f  ,  -2.0f  , -2.5f),
		glm::vec3(1.5f  ,   2.0f  ,  -2.5f),
		glm::vec3(1.5f  ,   0.2f  ,  -1.5f),
		glm::vec3(-1.3f ,   1.0f   , -1.5f),


	};

	
	
	unsigned int VBO, VAO,TexID;
	glGenTextures(1,&TexID);
	glBindTexture(GL_TEXTURE_2D,TexID);
	//set texture wrapping options on the currently bound texture object
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	char *data = (char*)load_texture("textures/container.jpg");

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	ourshader.use();
	//always use shader first before passing uniforms
	ourshader.setInt("ourTexture",0);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind vertex object
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);


	//create generic vertex attribute  data
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	//send textures
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//create 3d model
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model , (float)glfwGetTime() , glm::vec3(0.5f,1.0f,0.0f));
		view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
		projection = glm::perspective(glm::radians(45.0f) , (float)scr_width/(float)scr_height , 0.1f,100.0f);

		ourshader.use();

		//send uniform model and view coords
		unsigned int modelLoc = glGetUniformLocation(ourshader.ID,"model");
		unsigned int viewLoc = glGetUniformLocation(ourshader.ID,"view");
		//three ways to pass args
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc , 1, GL_FALSE, &view[0][0]);
		ourshader.setMat4("projection",projection);

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		for (unsigned int i = 0;i < 10;i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model,cubePositions[i]);
			float angle = 20.0f*i;
			model = glm::rotate(model, (float)glfwGetTime(),glm::vec3(1.0f,0.3f,0.5f));
			ourshader.setMat4("model",model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1,&VBO);

	glfwTerminate();
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