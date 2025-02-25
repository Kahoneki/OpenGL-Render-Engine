#include "Mesh.h"

#include <string>
#include <vector>

#include <GLM/glm.hpp>

#include "../../../Shaders/shader.h"
#include <iostream>


Mesh::Mesh(std::vector<float> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures, std::string name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent)
	: SceneObject(name, parent, Transform(center, scale, rotation)),
	  Drawable(name, center, scale, rotation, parent)
{
	vertices = _vertices;
	indices = _indices;
	textures = _textures;

	std::cout << "name in mesh constructor is " << name << '\n';

	SetupMesh();
}


void Mesh::SetupMesh()
{
	//VAO, VBO, EBO setup
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture coordinates
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);


	//Setup material
	for (Texture& t : textures)
	{
		if (t.type == "albedo")
		{
			material.setAlbedoTextureHandle(t.id);
			material.SetPropertyActive(MATERIAL_ALBEDO_TEXTURE_BIT, true);
			continue;
		}
		else if (t.type == "specular")
		{
			material.setSpecularTextureHandle(t.id);
			material.SetPropertyActive(MATERIAL_SPECULAR_TEXTURE_BIT, true);
			continue;
		}

		if (material.GetPropertyActive(MATERIAL_ALBEDO_TEXTURE_BIT) && material.GetPropertyActive(MATERIAL_SPECULAR_TEXTURE_BIT))
		{
			//Both textures found
			break;
		}
	}
}


void Mesh::Draw(Shader& shader)
{
	//Draw mesh
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instances);
	glBindVertexArray(0);
}