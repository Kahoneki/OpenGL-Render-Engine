#include "camera.h"
#include <iostream>
#include "camera.h"


//Vector Constructor
Camera::Camera(const char* name, glm::vec3 position, glm::vec3 up, float yaw, float pitch, float nearPlaneDist, float farPlaneDist)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;

	Front = glm::vec3(0.0f, 0.0f, -1.0f);

	nearPlaneDistance = nearPlaneDist;
	farPlaneDistance = farPlaneDist;

	updateCameraVectors();
}

//Scalar Constructor
Camera::Camera(const char* name, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float nearPlaneDist, float farPlaneDist)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;

	Front = glm::vec3(0.0f, 0.0f, -1.0f);

	nearPlaneDistance = nearPlaneDist;
	farPlaneDistance = farPlaneDist;

	updateCameraVectors();
}

Camera::~Camera()
{
}


void Camera::setPosition(glm::vec3 pos)
{
	worldPos = pos;
}

//Returns the view matrix calculated using euler angles and the LookAt matrix
glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Front, Up); //lookAt() automatically normalises direction argument (Position + Front)
}


//Calculates the front vector from the camera's updated euler angles
void Camera::updateCameraVectors()
{
	//Calculate the new front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	//Calculate the new Right and Up vectors
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}