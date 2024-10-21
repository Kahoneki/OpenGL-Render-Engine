#include "Drawable.h"

#include "GLM/gtx/quaternion.hpp"

Drawable::Drawable(const char* name, glm::vec3 topLeftFront, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) : SceneObject(name, parent, topLeftFront)
{
	//Construct model matrix
	model = glm::scale(glm::mat4(1.0f), scale);
	model = model * glm::toMat4(glm::quat(rotation));
	model = glm::translate(model, topLeftFront);

	glGenVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	worldPos = topLeftFront;
}

Drawable::~Drawable() {}

void Drawable::setPosition(glm::vec3 pos)
{
	worldPos = pos;
	glm::vec3 diff{ pos - getPosition() };
	model = glm::translate(model, diff);
}