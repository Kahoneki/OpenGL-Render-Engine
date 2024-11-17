#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "SceneObject.h"
#include "Drawable/Cube8.h"

#include "GLM/glm.hpp"
#include <GLAD/glad.h>

class Scene;

struct Light {
	glm::vec4 position;         // 16 bytes
	glm::vec4 ambientColour;    // 16 bytes
	glm::vec4 diffuseColour;    // 16 bytes
	glm::vec4 specularColour;   // 16 bytes
	glm::vec4 intensityPack;    // 16 bytes (intensity in x, unused yzw)
};

class LightSource : public SceneObject
{

	friend class Scene;

public:
	LightSource();
	LightSource(const char* name, glm::vec3 _position, glm::vec4 ambientColour, glm::vec4 diffuseColour, glm::vec4 specularColour, float intensity, SceneObject* parent);
	~LightSource();

	void setPosition(glm::vec3 pos) override;

	glm::vec4 getAmbientColour();
	void setAmbientColour(glm::vec4 colour);
	glm::vec4 getDiffuseColour();
	void setDiffuseColour(glm::vec4 colour);
	glm::vec4 getSpecularColour();
	void setSpecularColour(glm::vec4 colour);

	float getIntensity();
	void setIntensity(float intensity);

	Cube8 cube;

private:

	[[no_discard]] const GLsizeiptr GetPaddedSize();

	Scene* sceneParent;
	Light light;

};
#endif