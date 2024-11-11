#include "PlayerCamera.h"
#include <iostream>


PlayerCamera::PlayerCamera(const char* name, SceneObject* parent, glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Camera(name, parent, position, up, yaw, pitch)
{

	MovementSpeed = MOVEMENT_SPEED;
	MouseSensitivity = SENSITIVITY;
	Fov = FOV;

	updateCameraVectors();
}

//Scalar Constructor
PlayerCamera::PlayerCamera(const char* name, SceneObject* parent, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Camera(name, parent, posX, posY, posZ, upX, upY, upZ, yaw, pitch)
{
	MovementSpeed = MOVEMENT_SPEED;
	MouseSensitivity = SENSITIVITY;
	Fov = FOV;

	updateCameraVectors();
}


//Processes keyboard input in the form of camera defined ENUM
void PlayerCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		setPosition(Position + Front * velocity);
	if (direction == BACKWARD)
		setPosition(Position - Front * velocity);
	if (direction == LEFT)
		setPosition(Position - Right * velocity);
	if (direction == RIGHT)
		setPosition(Position + Right * velocity);
}


//Processes mouse input
void PlayerCamera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	//Constraints
	if (constrainPitch) {
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	//Update Front, Right, and Up vectors using the updated euler angles
	updateCameraVectors();
}


//Processes scroll wheel input
void PlayerCamera::ProcessMouseScroll(float yOffset)
{
	Fov -= (float)yOffset;
	if (Fov < 1.0f)
		Fov = 1.0f;
	if (Fov > 100.0f)
		Fov = 100.0f;
}