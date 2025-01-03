#ifndef PLANE_H
#define PLANE_H

#include "PrimitiveModel.h"

class Plane : public PrimitiveModel
{
public:
    //Simple flat plane, includes texture coordinates, normals, and tangents
    Plane(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent = nullptr);

    std::pair<glm::vec3, glm::vec3> GetWorldAxes() const; //Returns a pair of axes in world space (horizontal then vertical)
};

#endif