#include "PrimitiveModel.h"

#include "../../Shaders/shader.h"

#include <GLAD/glad.h>
#include <iostream>

PrimitiveModel::~PrimitiveModel() {}

PrimitiveModel::PrimitiveModel(const char* name, glm::vec3 topLeftFront, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) : Drawable(name, topLeftFront, scale, rotation, parent)
{
}

void PrimitiveModel::Draw(Shader& shader)
{
	Drawable::Draw(shader);
	glBindVertexArray(VAO);

	if (indexed) {
		glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(drawMode, 0, vertices.size());
	}
}