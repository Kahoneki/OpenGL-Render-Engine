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
public:
	Drawable(const char* name, glm::vec3 topLeftFront, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent);
	virtual void Draw(Shader& shader) = 0;
	void setPosition(glm::vec3 pos) override;
	virtual ~Drawable() = 0;

protected:
	glm::mat4 model;
	Material material;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	GLenum drawMode;
};

#endif

