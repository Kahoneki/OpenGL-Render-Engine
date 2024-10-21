#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "RenderSource.h"

#include <vector>


//Default camera values
const float YAW = -90.0f; //Rotated 90 degrees to ensure camera looks along +z axis
const float PITCH = 0.0f;
const float NEARPLANEDISTANCE = 0.1f;
const float FARPLANEDISTANCE = 100.0f;


//Abstract camera class for use with OpenGL
class Camera : public RenderSource
{
public:
	//----ATTRIBUTES----//
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	
	float Yaw;
	float Pitch;

	float nearPlaneDistance;
	float farPlaneDistance;

	
	//----CONSTRUCTORS----//
	
	//Vector Constructor
	Camera(const char* name, glm::vec3 position=glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up=glm::vec3(0.0f, 1.0f, 0.0f), float yaw=YAW, float pitch=PITCH, float nearPlaneDist=NEARPLANEDISTANCE, float farPlaneDist=FARPLANEDISTANCE);
	//Scalar Constructor
	Camera(const char* name, float posX=0.0f, float posY=0.0f, float posZ=0.0f, float upX=0.0f, float upY=1.0f, float upZ=0.0f, float yaw=YAW, float pitch=PITCH, float nearPlaneDist = NEARPLANEDISTANCE, float farPlaneDist = FARPLANEDISTANCE);

	virtual ~Camera();


	//----METHODS----//

	void setPosition(glm::vec3 pos) override;

	//Returns the view matrix calculated using euler angles and the LookAt matrix
	glm::mat4 GetViewMatrix() const;

protected:
	//Calculates the front vector from the camera's updated euler angles
	void updateCameraVectors();
};


#endif