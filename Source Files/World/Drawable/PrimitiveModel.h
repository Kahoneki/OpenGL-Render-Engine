#ifndef PRIMITIVEMODEL_H
#define PRIMITIVEMODEL_H
#include "Drawable.h"

#include "GLM/gtc/matrix_transform.hpp"
#include <vector>

class Shader;

/*Virtual class - Must be created as derived class (Cube, Plane, or Sphere)*/

class PrimitiveModel : public virtual Drawable
{
public:
	PrimitiveModel(const char* name, glm::vec3 topLeftFront, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent);
	void Draw(Shader& shader) override;
	virtual ~PrimitiveModel() = 0;
};

#endif