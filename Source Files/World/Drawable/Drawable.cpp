#include "Drawable.h"

#include <glm/gtx/quaternion.hpp>
#include "../../Utility/BindingPoints.h"
#include "../../Application/Application.h"
#include "../../Application/Renderer.h"
#include "../../../Shaders/shader.h"
#include "../Scene.h"
#include "ExternalModel.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <climits>

Drawable::Drawable(std::string name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) : SceneObject(name, parent, Transform(center, scale, rotation))
{
	glGenVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glCreateBuffers(1, &materialBuffer);
	material.drawableParent = this;
	UpdateMaterial();

	useOnlyDiffuse = false;

	eulerRotation = rotation;

	renderOrder = 0;

	instances = 1;
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

void Drawable::UseOnlyDiffuse(bool val)
{
	useOnlyDiffuse = val;
	
	if (ExternalModel* e{ dynamic_cast<ExternalModel*>(this) })
	{
		for (Mesh* m : e->meshes)
		{
			m->UseOnlyDiffuse(val);
		}
	}
}


void Drawable::UpdateMaterial()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::MATERIAL, materialBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), &material.materialData, GL_DYNAMIC_DRAW); //Dynamic draw as this function will be called everytime the material is modified
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}