#define STB_IMAGE_IMPLEMENTATION

#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cmath>
#include "Shader.h"
#include"Camera.h"
#include <stb_image.h>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include<glm\glm\gtc\type_ptr.hpp>

std::string vertexShader;
std::string fragmentShader;

//handler functions
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window,int width,int height);

void mouse_callback(GLFWwindow* window,double xpos, double ypos);
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset);

const unsigned int scr_width = 800;
const unsigned int scr_height = 600;

bool firstMouse = true;
//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;

//lightPos
glm::vec3 lightPos(1.2f,1.0f,2.0f);
float deltaTime = 0.0f;
float last_frame = 0.0f;

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
	glfwSetCursorPosCallback(window,mouse_callback);
	glfwSetScrollCallback(window,scroll_callback);

	//check if glad initiated
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialized GLAD" << std::endl;
		return - 1;
	}
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	//Shader ourshader("shaders/shader.vs","shaders/fragment.vs");
	Shader materialShader("shaders/material.vs","shaders/material.fs");
	Shader lampShader("shaders/lamp.vs","shaders/lamp.fs");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
	//send textures
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	unsigned int LightVAO;
	glGenVertexArrays(1,&LightVAO);
	glBindVertexArray(LightVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - last_frame;
		last_frame = currentFrame;

		processInput(window);

		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		materialShader.use();
		materialShader.setVec3("lit.position",lightPos);
		materialShader.setVec3("viewPos",camera.Position);

		//light properties
		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime()*0.9f);
		lightColor.y = sin(glfwGetTime()*0.3f);
		lightColor.z = sin(glfwGetTime()*1.5f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.8);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.4);
		materialShader.setVec3("lit.ambient",ambientColor);
		materialShader.setVec3("lit.diffuse",diffuseColor);
		materialShader.setVec3("lit.specular",1.0f,1.0f,1.0f);

		//material properties
		materialShader.setVec3("mater.ambient",1.0f,0.5f,0.31f);
		materialShader.setVec3("mater.diffuse", 1.0f, 0.5f, 0.31f);
		materialShader.setVec3("mater.specular", 0.5f, 0.5f, 0.5f);
		materialShader.setFloat("mater.shininess",32.0f);



		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),(float)scr_width/(float)scr_height,0.1f,100.0f);
		materialShader.setMat4("projection",projection);
		//set view matric
		glm::mat4 view = camera.get_view_matrix();
		materialShader.setMat4("view",view);

		glm::mat4 model = glm::mat4(1.0f);
		lampShader.setMat4("model",model);

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		for (unsigned int i = 0;i < 10;i++) {
			
			model = glm::translate(model,cubePositions[i]);
			
			materialShader.setMat4("model",model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lampShader.use();
		lampShader.setMat4("projection",projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model,lightPos);
		model = glm::scale(model,glm::vec3(0.2f));
		lampShader.setMat4("model", model);
		glBindVertexArray(LightVAO);
		glDrawArrays(GL_TRIANGLES,0,36);

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
	float cameraSpeed=0.10f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window,true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD,deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACK,deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT,deltaTime);
	}
}

//mouse  callback
void mouse_callback(GLFWwindow *window,double xpos,double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;//IMP: reversed cause y coords vary from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset,yoffset);
}
//scroll callback
void scroll_callback(GLFWwindow *window,double xoffset,double yoffset) {
	camera.process_mouse_scroll(yoffset);
}