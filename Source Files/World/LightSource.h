#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "SceneObject.h"
#include "Drawable/Cube24.h"

#include "GLM/glm.hpp"
#include <GLAD/glad.h>

class Scene;

struct Light
{
	glm::vec4 position;
	
	glm::vec4 ambientColour;
	glm::vec4 diffuseColour;
	glm::vec4 specularColour;
};

class LightSource : public SceneObject
{

	friend class Scene;

public:
	LightSource();
	LightSource(const char* name, glm::vec3 _position, glm::vec4 ambientColour, glm::vec4 diffuseColour, glm::vec4 specularColour, SceneObject* parent);
	~LightSource();

	void setPosition(glm::vec3 pos) override;

	glm::vec4 getAmbientColour();
	void setAmbientColour(glm::vec4 colour);
	glm::vec4 getDiffuseColour();
	void setDiffuseColour(glm::vec4 colour);
	glm::vec4 getSpecularColour();
	void setSpecularColour(glm::vec4 colour);

	Cube24 cube;

private:

	[[no_discard]] const GLsizeiptr GetPaddedSize();

	Scene* sceneParent;
	Light light;

};
#endif