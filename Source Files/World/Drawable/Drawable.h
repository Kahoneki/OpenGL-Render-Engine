#ifndef MODEL_H
#define MODEL_H

#include "../SceneObject.h"
#include "../../Material/Material.h"

#include "GLM/gtc/matrix_transform.hpp"
#include "GLAD/glad.h"

#include <vector>

class Shader;


/*Virtual Class - Must be created as derived class (Mesh or PrimitiveModel)*/

class Drawable : public virtual SceneObject
{
	friend class Material;

public:
	Drawable(const char* name, glm::vec3 topLeftFront, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent);
	virtual void Draw(Shader& shader) = 0;
	virtual ~Drawable() = 0;
	void setPosition(glm::vec3 pos) override;
	Material material;

protected:
	glm::mat4 model;
	unsigned int materialBuffer;
	void UpdateMaterial();

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	GLenum drawMode;
};

#endif

