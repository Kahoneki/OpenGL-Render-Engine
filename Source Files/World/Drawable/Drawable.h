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
	Drawable(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent);
	virtual void Draw(Shader& shader);
	virtual ~Drawable() = 0;

	unsigned int getRenderOrder();
	void setRenderOrder(unsigned int val);

	Material material;

	bool useOnlyDiffuse; //When using a shader with lighting calculations, this toggle, if true, will skip the calculations and use a diffuse term instead

protected:
	unsigned int materialBuffer;
	void UpdateMaterial();

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	GLenum drawMode;
	bool indexed;

	unsigned int renderOrder; //Used to sort Scene::drawables vector (lower value = earlier render). Not unique. Default value = 0

private:
	glm::vec3 eulerRotation; //Only used for display purposes
};

#endif

