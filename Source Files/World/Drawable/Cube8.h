#ifndef CUBE8_H
#define CUBE8_H

#include "PrimitiveModel.h"

class Cube8 : public PrimitiveModel
{
public:
    //Simple cube with 8 vertices. For face-dependent vertex attributes, use Cube24
    Cube8(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent = nullptr);
};

#endif