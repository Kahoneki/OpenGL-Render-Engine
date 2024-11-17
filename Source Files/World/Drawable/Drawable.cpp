#include "Drawable.h"

#include "GLM/gtx/quaternion.hpp"
#include "../../Utility/BindingPoints.h"
#include "../../Application/Application.h"
#include "../../Application/Renderer.h"
#include "../../Shaders/shader.h"

#include <iostream>
#include <GLM/gtx/string_cast.hpp>

Drawable::Drawable(const char* name, glm::vec3 topLeftFront, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) : SceneObject(name, parent, topLeftFront)
{
	//Construct model matrix
	model = glm::mat4(1.0f);
	model = model * glm::toMat4(glm::quat(rotation));
	model = glm::scale(model, scale);
	model = glm::translate(model, topLeftFront / scale);

	glGenVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glCreateBuffers(1, &materialBuffer);
	material.drawableParent = this;
	UpdateMaterial();

	worldPos = topLeftFront;
}

void Drawable::Draw(Shader& shader)
{
	UpdateMaterial();
	shader.setMat4("model", model);
	shader.setBool("isEmissive", isEmissive);
}

Drawable::~Drawable() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &materialBuffer);
}


void Drawable::setPosition(glm::vec3 pos)
{
	glm::vec3 currentPos = getPosition();
	glm::vec3 diff = pos - currentPos;
	model = glm::translate(model, diff / getScale());
	worldPos = pos;
}

glm::vec3 Drawable::getScale() {
	glm::vec3 scale;
	scale.x = glm::length(glm::vec3(model[0])); // First column
	scale.y = glm::length(glm::vec3(model[1])); // Second column
	scale.z = glm::length(glm::vec3(model[2])); // Third column
	return scale;
}

void Drawable::setScale(glm::vec3 scale)
{
	glm::vec3 currentScale = getScale();
	glm::vec3 scaleFactor{
		scale.x / currentScale.x,
		scale.y / currentScale.y,
		scale.z / currentScale.z
	};
	model = glm::scale(model, scaleFactor);
}

glm::vec3 Drawable::getRotation()
{
	glm::vec3 rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(model)));
	return rotation;
}

void Drawable::setRotation(glm::vec3 rotation)
{
	glm::vec3 currentRotation = getRotation();
	glm::vec3 diff = rotation - currentRotation;
	glm::mat4 rotationMatrix = glm::toMat4(glm::quat(glm::radians(diff)));
	model = model * rotationMatrix;
}


void Drawable::UpdateMaterial()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::MATERIAL, materialBuffer);
	glBufferData(GL_UNIFORM_BUFFER, material.GetPaddedSize(), &material.materialData, GL_DYNAMIC_DRAW); //Dynamic draw as this function will be called everytime the material is modified
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}