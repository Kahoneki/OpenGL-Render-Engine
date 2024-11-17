#include "Plane.h"

Plane::Plane(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) :
	SceneObject(name, parent, center),
	Drawable(name, center, scale, rotation, parent),
	PrimitiveModel(name, center, scale, rotation, parent)
{

	vertices = {
		//Position             //Texture     //Normal              //Tangent
		// Front face: (z = 0.0f instead of 1.0f to ensure origin center at 0,0,0 rather than 0,0,1)
		-1.0f, -1.0f,  0.0f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,  // Bottom-left
		-1.0f,  1.0f,  0.0f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,  // Top-left
		 1.0f,  1.0f,  0.0f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,  // Top-right
		-1.0f, -1.0f,  0.0f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,  // Bottom-left
		 1.0f,  1.0f,  0.0f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,  // Top-right
		 1.0f, -1.0f,  0.0f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,  // Bottom-right
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glBindVertexArray(0);

	drawMode = GL_TRIANGLES;
	indexed = false;
}