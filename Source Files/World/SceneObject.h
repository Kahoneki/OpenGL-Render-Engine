#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "GLM/gtx/quaternion.hpp"
#include <vector>


class Scene;

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	inline Transform() : position(glm::vec3(0.0f)), scale(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)) {};
	inline Transform(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation) : position(_position), scale(_scale), rotation(_rotation) {};
	
	inline glm::mat4 GetMatrix() {
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat, position); // Translate first (global position)
		mat = mat * glm::toMat4(glm::quat(glm::radians(rotation))); // Apply local rotation
		mat = glm::scale(mat, scale); // Apply local scale
		return mat;
	};
};


class SceneObject
{

	friend class Scene;

public:
	SceneObject();
	SceneObject(const char* _name, SceneObject* _parent, Transform transform);
	virtual ~SceneObject();

	//Setters
	virtual void setPosition(glm::vec3 pos);
	virtual void setScale(glm::vec3 pos);
	virtual void setRotation(glm::vec3 pos);

	//Getters
	glm::vec3 getPosition() const;
	glm::vec3 getScale() const;
	glm::vec3 getRotation() const;

	glm::mat4 GetHeirarchicalModelMatrix();

	SceneObject* parent;
	std::vector<SceneObject*> children;

	const char* name;

protected:
	Transform transform;
	Scene* sceneParent;
};

#endif