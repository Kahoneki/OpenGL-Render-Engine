#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "SceneObject.h"

#include "GLM/glm.hpp"

struct Light
{
	glm::vec3 position;
	glm::vec3 colour;
	float intensity;
};

class LightSource : SceneObject
{
public:
	LightSource();
	LightSource(const char* name, glm::vec3 _position, glm::vec3 _colour, float _intensity, SceneObject* parent);
	~LightSource();

	void setPosition(glm::vec3 pos) override;

	glm::vec3 getColour();
	float getIntensity();

	void setColour(glm::vec3 val);
	void setIntensity(float val);

private:
	Light light;
};
#endif