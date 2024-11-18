#include "Drawable.h"

#include "GLM/gtx/quaternion.hpp"
#include "../../Utility/BindingPoints.h"
#include "../../Application/Application.h"
#include "../../Application/Renderer.h"
#include "../../Shaders/shader.h"
#include "../Scene.h"

#include <iostream>
#include <GLM/gtx/string_cast.hpp>
#include <climits>

Drawable::Drawable(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) : SceneObject(name, parent, Transform(center, scale, rotation))
{
	glGenVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glCreateBuffers(1, &materialBuffer);
	material.drawableParent = this;
	UpdateMaterial();

	eulerRotation = rotation;

	renderOrder = 0;
}

void Drawable::Draw(Shader& shader)
{
	UpdateMaterial();
	shader.setMat4("model", GetHeirarchicalModelMatrix());
	shader.setBool("useOnlyDiffuse", useOnlyDiffuse);
}

Drawable::~Drawable() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &materialBuffer);
}


unsigned int Drawable::getRenderOrder()
{
	return renderOrder;
}

void Drawable::setRenderOrder(unsigned int val)
{
	renderOrder = val;
	if (sceneParent) {
		sceneParent->ReorderDrawable(this);
	}
}


void Drawable::UpdateMaterial()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::MATERIAL, materialBuffer);
	glBufferData(GL_UNIFORM_BUFFER, material.GetPaddedSize(), &material.materialData, GL_DYNAMIC_DRAW); //Dynamic draw as this function will be called everytime the material is modified
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}