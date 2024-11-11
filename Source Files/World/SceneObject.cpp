#include "SceneObject.h"
#include <iostream>

SceneObject::SceneObject()
{
	std::cout << "unknown\n";
	name = "Unkown SceneObject";
	worldPos = glm::vec3(0.0f);
	parent = nullptr;
	child = nullptr;
}

SceneObject::SceneObject(const char* _name, SceneObject* _parent, glm::vec3 _worldPos)
{
	name = _name;
	parent = _parent;
	worldPos = _worldPos;

	parent = _parent;
	if (parent != nullptr) {
		parent->child = this;
	}
	child = nullptr;
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
