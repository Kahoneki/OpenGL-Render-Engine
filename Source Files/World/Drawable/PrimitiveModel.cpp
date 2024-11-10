#include "PrimitiveModel.h"

#include "../../Shaders/shader.h"

#include <GLAD/glad.h>

PrimitiveModel::~PrimitiveModel() {}

void PrimitiveModel::Draw(Shader& shader)
{
	Drawable::Draw(shader);
	glBindVertexArray(VAO);
	shader.setMat4("model", model);

	if (indexed) {
		glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(drawMode, 0, vertices.size());
	}
}