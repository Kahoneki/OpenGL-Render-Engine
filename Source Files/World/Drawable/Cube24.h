#ifndef CUBE24_H
#define CUBE24_H

#include "PrimitiveModel.h"

class Cube24 : public PrimitiveModel
{
public:
    //Simple cube with 24 vertices, includes texture coordinates, normals, and tangents
    Cube24(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent = nullptr);
};

#endif