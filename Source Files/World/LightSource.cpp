#include "LightSource.h"
#include "../World/Scene.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>

LightSource::LightSource() : SceneObject("New Light Source", nullptr, Transform(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f))), cube("New Light Source Box", glm::vec3(0.0f), glm::vec3(0.2f), glm::vec3(0.0f), this)
{
	light.position = glm::vec4(0.0f);
	light.ambientColour = glm::vec4(1.0f);
	light.diffuseColour = glm::vec4(1.0f);
	light.specularColour = glm::vec4(1.0f);
	light.intensityPack = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

	cube.UseOnlyDiffuse(true);
	cube.material.setDiffuseColour(light.diffuseColour);
}

LightSource::LightSource(const char* name, glm::vec3 _position, glm::vec4 ambientColour, glm::vec4 diffuseColour, glm::vec4 specularColour, float intensity, SceneObject* parent) : SceneObject(name, parent, Transform(_position, glm::vec3(1.0f), glm::vec3(0.0f))), cube("Light Source Box", glm::vec3(0.0f), glm::vec3(0.2f), glm::vec3(0.0f), this)
{
	light.position = glm::vec4(_position, 0.0f);
	light.ambientColour = ambientColour;
	light.diffuseColour = diffuseColour;
	light.specularColour = specularColour;
	light.intensityPack = glm::vec4(intensity, 0.0f, 0.0f, 0.0f);

	cube.UseOnlyDiffuse(true);
	cube.material.setDiffuseColour(light.diffuseColour);
}

LightSource::~LightSource()
{
}


void LightSource::setPosition(glm::vec3 pos)
{
	SceneObject::setPosition(pos);
	light.position = glm::vec4(pos, 0.0f);
	sceneParent->lightsChanged = true;
	//cube.setPosition(pos);
}

glm::vec4 LightSource::getAmbientColour()
{
	return light.ambientColour;
}

void LightSource::setAmbientColour(glm::vec4 colour)
{
	light.ambientColour = colour;
	sceneParent->lightsChanged = true;
}

glm::vec4 LightSource::getDiffuseColour()
{
	return light.diffuseColour;
}

void LightSource::setDiffuseColour(glm::vec4 colour)
{
	light.diffuseColour = colour;
	sceneParent->lightsChanged = true;
}

glm::vec4 LightSource::getSpecularColour()
{
	return light.specularColour;
}

void LightSource::setSpecularColour(glm::vec4 colour)
{
	light.specularColour = colour;
	sceneParent->lightsChanged = true;
}

float LightSource::getIntensity()
{
	return light.intensityPack.x;
}

void LightSource::setIntensity(float intensity)
{
	light.intensityPack.x = intensity;
	sceneParent->lightsChanged = true;
}

const GLsizeiptr LightSource::GetPaddedSize()
{
	GLsizeiptr size{ 0 };

	size += sizeof(light.position); //16 bytes

	size += sizeof(light.ambientColour); //16 bytes

	size += sizeof(light.diffuseColour); //16 bytes

	size += sizeof(light.specularColour); //16 bytes

	size += sizeof(light.intensityPack); //16 bytes
	//size += 12; //12 padding bytes

	//Padding bytes for 16 byte alignment
	if (size % 16 != 0) {
		size += 16 - (size % 16);
	}

	return size;
}