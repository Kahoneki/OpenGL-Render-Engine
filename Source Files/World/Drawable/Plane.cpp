#include "Plane.h"
#include <GLM/vec4.hpp>

Plane::Plane(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) :
	SceneObject(name, parent, Transform(center, scale, rotation)),
	Drawable(name, center, scale, rotation, parent),
	PrimitiveModel(name, center, scale, rotation, parent)
{

	vertices = {
		//Position             //Texture     //Normal              //Tangent
		// Front face: (z = 0.0f)
		-0.5f, -0.5f,  0.0f,   0.0f, 0.0f,   0.0f,  0.0f,  0.5f,   0.5f,  0.0f,  0.0f,  // Bottom-left
		-0.5f,  0.5f,  0.0f,   0.0f, 0.5f,   0.0f,  0.0f,  0.5f,   0.5f,  0.0f,  0.0f,  // Top-left
		 0.5f,  0.5f,  0.0f,   0.5f, 0.5f,   0.0f,  0.0f,  0.5f,   0.5f,  0.0f,  0.0f,  // Top-right
		-0.5f, -0.5f,  0.0f,   0.0f, 0.0f,   0.0f,  0.0f,  0.5f,   0.5f,  0.0f,  0.0f,  // Bottom-left
		 0.5f,  0.5f,  0.0f,   0.5f, 0.5f,   0.0f,  0.0f,  0.5f,   0.5f,  0.0f,  0.0f,  // Top-right
		 0.5f, -0.5f,  0.0f,   0.5f, 0.0f,   0.0f,  0.0f,  0.5f,   0.5f,  0.0f,  0.0f,  // Bottom-right
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


std::pair<glm::vec3, glm::vec3> Plane::GetWorldAxes() const
{
	//Get local axes
	glm::vec3 uLocal{ vertices[5] - vertices[0] }; //Bottom left to bottom right
	glm::vec3 vLocal{ vertices[1] - vertices[0] }; //Bottom left to top left

	//Convert to homogeneous
	glm::vec4 uLocalHomogeneous{ glm::vec4(uLocal, 1.0f) };
	glm::vec4 vLocalHomogeneous{ glm::vec4(vLocal, 1.0f) };

	//Convert to world space
	glm::vec4 uWorld{ GetHeirarchicalModelMatrix() * uLocalHomogeneous };
	glm::vec4 vWorld{ GetHeirarchicalModelMatrix() * vLocalHomogeneous };
	
	return std::make_pair<glm::vec3, glm::vec3>(glm::vec3(uWorld.x, uWorld.y, uWorld.z), glm::vec3(vWorld.x, vWorld.y, vWorld.z));
}