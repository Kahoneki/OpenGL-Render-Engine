#ifndef PLAYERCAMERA_H
#define PLAYERCAMERA_H

#include "camera.h"

#include <glad/glad.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <vector>


//Possible camera movement options
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//Default PlayerCamera values
const float MOVEMENT_SPEED = 50.0f;
const float SENSITIVITY = 0.05f;
const float FOV = 90.0f;

//Abstract player camera class that processes inputs and calculates corresponding euler angles, vectors, and matrices for use with OpenGL
class PlayerCamera : public Camera
{
public:
	float MovementSpeed;
	float MouseSensitivity;
	float Fov;


	//----CONSTRUCTORS----//

	//Vector Constructor
	PlayerCamera(const char* name, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	//Scalar Constructor
	PlayerCamera(const char* name, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);


	//----METHODS----//

	//Processes keyboard input in the form of camera defined ENUM
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);


	//Processes mouse input
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);


	//Processes scroll wheel input
	void ProcessMouseScroll(float yOffset);
};

#endif