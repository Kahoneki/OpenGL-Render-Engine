#include "Cube8.h"

Cube8::Cube8(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) :
	SceneObject(name, parent, Transform(center, scale, rotation)),
	Drawable(name, center, scale, rotation, parent),
	PrimitiveModel(name, center, scale, rotation, parent)
{

	vertices =
	{
		//   Coordinates
		-1.0f, -1.0f,  1.0f,//        7--------6
		 1.0f, -1.0f,  1.0f,//       /|       /|
		 1.0f, -1.0f, -1.0f,//      4--------5 |
		-1.0f, -1.0f, -1.0f,//      | |      | |
		-1.0f,  1.0f,  1.0f,//      | 3------|-2
		 1.0f,  1.0f,  1.0f,//      |/       |/
		 1.0f,  1.0f, -1.0f,//      0--------1
		-1.0f,  1.0f, -1.0f
	};

	indices =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	drawMode = GL_TRIANGLES;
	indexed = true;
}