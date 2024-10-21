#include "PrimitiveModel.h"

#include "../../Shaders/shader.h"

#include <GLAD/glad.h>

PrimitiveModel::~PrimitiveModel() {}

void PrimitiveModel::Draw(Shader& shader)
{
	glBindVertexArray(VAO);
	shader.setMat4("model", model);
	glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
}