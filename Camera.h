#pragma once
#include <glm\glm\glm.hpp>
#include<glm\glm\gtc\matrix_transform.hpp>

#include<glad\glad.h>

enum Camera_Movement {
	FORWARD,
	BACK,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.25f;
const float ZOOM = 45.0f;


class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float zoom;

	Camera(glm::vec3 pos = glm::vec3(0.0f,0.0f,0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),float yaw = YAW,float pitch= PITCH) :Front(glm::vec3(0.0f,0.0f,-1.0f)),MovementSpeed(SPEED),MouseSensitivity(SENSITIVITY),zoom(ZOOM)
	{
		Position = pos;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

	}
	//float based constructor
	Camera(float posX,float posY,float posZ,float upX,float upY,float upZ,float yaw,float pitch):Front(glm::vec3(0.0f,0.0f,-1.0f)),MovementSpeed(SPEED),MouseSensitivity(SENSITIVITY),zoom(ZOOM)
	{
		Position = glm::vec3(posX,posY,posZ);
		WorldUp = glm::vec3(upX,upY,upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	//get view matrix for transformation
	glm::mat4 get_view_matrix() {
		return glm::lookAt(Position,Position+Front,Up);
	}

	void ProcessKeyboard(Camera_Movement direction,float deltaTime){
		float velocity = MovementSpeed*deltaTime;
		if (direction == FORWARD) {
			Position += Front*velocity;
		}
		if (direction == BACK) {
			Position -= Front * velocity;
		}
		if (direction == RIGHT) {
			Position += Right * velocity;
		}
		if (direction == LEFT) {
			Position -= Right * velocity;
		}
	}

	void ProcessMouseMovement(float xoffset,float yoffset,GLboolean constrainPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		//Revise Euler Angles
		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch) {
			if (Pitch >= 89.0f) {
				Pitch = 89.0f;
			}
			if (Pitch <= -89.0f) {
				Pitch = -89.0f;
			}
		}

		updateCameraVectors();
	}

	void process_mouse_scroll(float yoffset) {
		if (zoom >= 1.0f && zoom<= 45.0f) {
			zoom -= yoffset;
		}
		if (zoom <= 1.0f) {
			zoom = 1.0f;
		}
		if (zoom >= 45.0f) {
			zoom = 45.0f;
		}
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw))*cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw))*cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		//Recalculate Right and Up Vector
		Right = glm::normalize(glm::cross(Front,WorldUp));
		Up = glm::normalize(glm::cross(Right,Front));
	}
};

