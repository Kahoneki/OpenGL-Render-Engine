#include "SceneObject.h"
#include <GLM/gtx/string_cast.hpp>
#include <iostream>

SceneObject::SceneObject()
{
	name = "Unkown SceneObject";
	transform = Transform();
	parent = nullptr;
	children = std::vector<SceneObject*>();
}

SceneObject::SceneObject(const char* _name, SceneObject* _parent, Transform _transform)
{
	name = _name;
	parent = _parent;
	transform = _transform;

	parent = _parent;
	if (parent != nullptr) {
		parent->children.push_back(this);
	}
	children = std::vector<SceneObject*>();
}

SceneObject::~SceneObject() {}

void SceneObject::setPosition(glm::vec3 pos)
{
	transform.position = pos;
}

void SceneObject::setScale(glm::vec3 scale)
{
	transform.scale = scale;
}

void SceneObject::setRotation(glm::vec3 rotation)
{
	transform.rotation = rotation;
}

glm::vec3 SceneObject::getPosition() const
{
	return transform.position;
}

glm::vec3 SceneObject::getScale() const
{
	return transform.scale;
}

glm::vec3 SceneObject::getRotation() const
{
	return transform.rotation;
}

glm::mat4 SceneObject::GetHeirarchicalModelMatrix() const
{
	return parent == nullptr ? transform.GetMatrix() : parent->GetHeirarchicalModelMatrix() * transform.GetMatrix();
}