#include "LightSource.h"

LightSource::LightSource() : SceneObject("New Light Source", nullptr, glm::vec3(0.0f))
{
	light = Light(glm::vec3(0.0f), glm::vec3(1.0f), 1.0f);
}

LightSource::LightSource(const char* name, glm::vec3 _position, glm::vec3 _colour, float _intensity, SceneObject* parent) : SceneObject(name, parent, _position)
{
	light = Light(_position, _colour, _intensity);
}

LightSource::~LightSource()
{
}

void LightSource::setPosition(glm::vec3 pos)
{
	worldPos = pos;
	light.position = pos;
}

glm::vec3 LightSource::getColour()
{
	return light.colour;
}

float LightSource::getIntensity()
{
	return light.intensity;
}

void LightSource::setColour(glm::vec3 val)
{
	light.colour = val;
}

void LightSource::setIntensity(float val)
{
	light.intensity = val;
}
