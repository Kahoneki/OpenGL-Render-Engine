#include "Cube24.h"

Cube24::Cube24(const char* name, glm::vec3 topLeftFront, glm::vec3 scale, glm::vec3 rotation, glm::vec3 colour, SceneObject* parent) :
    SceneObject(name, parent, topLeftFront),
    Drawable(name, topLeftFront, scale, rotation, parent),
    PrimitiveModel(name, topLeftFront, scale, rotation, parent)
{

    vertices = {
        //Position             //Texture     //Normal
        
        // Front face: (z = 1.0f)
        -1.0f, -1.0f,  1.0f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,  // Bottom-left
        -1.0f,  1.0f,  1.0f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f,  // Top-left
         1.0f,  1.0f,  1.0f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,  // Top-right
        -1.0f, -1.0f,  1.0f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,  // Bottom-left
         1.0f,  1.0f,  1.0f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,  // Top-right
         1.0f, -1.0f,  1.0f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f,  // Bottom-right

        // Back face: (z = -1.0f)
         1.0f, -1.0f, -1.0f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,  // Bottom-right
         1.0f,  1.0f, -1.0f,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f,  // Top-right
        -1.0f,  1.0f, -1.0f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,  // Top-left
         1.0f, -1.0f, -1.0f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,  // Bottom-right
        -1.0f,  1.0f, -1.0f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,  // Top-left
        -1.0f, -1.0f, -1.0f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f,  // Bottom-left

        // Left face: (x = -1.0f)
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,  // Bottom-back
        -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,  // Top-back
        -1.0f,  1.0f,  1.0f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,  // Top-front
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,  // Bottom-back
        -1.0f,  1.0f,  1.0f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,  // Top-front
        -1.0f, -1.0f,  1.0f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,  // Bottom-front

        // Right face: (x = 1.0f)
         1.0f, -1.0f,  1.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,  // Bottom-front
         1.0f,  1.0f,  1.0f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,  // Top-front
         1.0f,  1.0f, -1.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,  // Top-back
         1.0f, -1.0f,  1.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,  // Bottom-front
         1.0f,  1.0f, -1.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,  // Top-back
         1.0f, -1.0f, -1.0f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,  // Bottom-back

        // Bottom face: (y = -1.0f)
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f,  // Back-left
        -1.0f, -1.0f,  1.0f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,  // Front-left
         1.0f, -1.0f,  1.0f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f,  // Front-right
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f,  // Back-left
         1.0f, -1.0f,  1.0f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f,  // Front-right
         1.0f, -1.0f, -1.0f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,  // Back-right

        // Top face: (y = 1.0f)
        -1.0f,  1.0f,  1.0f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f,  // Front-left
        -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f,  // Back-left
         1.0f,  1.0f, -1.0f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f,  // Back-right
        -1.0f,  1.0f,  1.0f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f,  // Front-left
         1.0f,  1.0f, -1.0f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f,  // Back-right
         1.0f,  1.0f,  1.0f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f   // Front-right
    };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glBindVertexArray(0);

	drawMode = GL_TRIANGLES;
	indexed = false;
}