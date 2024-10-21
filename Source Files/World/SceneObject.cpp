#include "SceneObject.h"

SceneObject::SceneObject()
{
	name = "Unkown SceneObject";
	worldPos = glm::vec3(0.0f);
	parent = nullptr;
}

SceneObject::SceneObject(const char* _name, SceneObject* _parent, glm::vec3 _worldPos)
{
	name = _name;
	parent = _parent;
	worldPos = _worldPos;
}

SceneObject::~SceneObject() {}

void SceneObject::setPosition(glm::vec3 pos)
{
	worldPos = pos;
}

glm::vec3 SceneObject::getPosition() const
{
	return worldPos;
}
