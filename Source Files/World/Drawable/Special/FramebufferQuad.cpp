#include "FramebufferQuad.h"

FramebufferQuad::FramebufferQuad() : framebufferShader(Shader(SHADER_PRESET::FBO))
{
	float quadVertices[] = {
		// positions            // texture coords
		 1.0f,  1.0f, 0.0f,     1.0f, 1.0f, // top right
		 1.0f,  -1.0f, 0.0f,    1.0f, 0.0f, // bottom right
		 -1.0f,  -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
		 -1.0f,  1.0f, 0.0f,    0.0f, 1.0f  // top left 
	};
	unsigned int quadIndices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

FramebufferQuad::~FramebufferQuad()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void FramebufferQuad::Draw()
{
	//Store current shader id to restore after drawing
	int id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	framebufferShader.use();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	//Restore state
	glUseProgram(id);
}
