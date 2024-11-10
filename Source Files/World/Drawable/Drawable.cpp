#include "Drawable.h"

#include "GLM/gtx/quaternion.hpp"
#include "../../Utility/BindingPoints.h"
#include "../../Application/Application.h"
#include "../../Application/Renderer.h"
#include "../../Shaders/shader.h"

#include <iostream>

Drawable::Drawable(const char* name, glm::vec3 topLeftFront, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) : SceneObject(name, parent, topLeftFront)
{
	//Construct model matrix
	model = glm::scale(glm::mat4(1.0f), scale);
	model = model * glm::toMat4(glm::quat(rotation));
	model = glm::translate(model, topLeftFront);

	glGenVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glCreateBuffers(1, &materialBuffer);
	material.parent = this;

	worldPos = topLeftFront;
}

void Drawable::Draw(Shader& shader)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::MATERIAL, materialBuffer);
}

Drawable::~Drawable() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &materialBuffer);
}

void Drawable::setPosition(glm::vec3 pos)
{
	glm::vec3 diff{ pos - getPosition() };
	model = glm::translate(model, diff);
	worldPos = pos;
}

void Drawable::UpdateMaterial()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::MATERIAL, materialBuffer);
	glBufferData(GL_UNIFORM_BUFFER, material.GetPaddedSize(), &material.materialData, GL_DYNAMIC_DRAW); //Dynamic draw as this function will be called everytime the material is modified
}