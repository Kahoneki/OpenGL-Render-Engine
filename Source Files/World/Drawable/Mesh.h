#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <GLM/glm.hpp>

#include "../../../Shaders/shader.h"
#include "Drawable.h"

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh : public Drawable {

public:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<float> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures, std::string name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent = nullptr);
	void Draw(Shader& shader) override;

private:
	void SetupMesh();

	std::string persistentName;
};

#endif