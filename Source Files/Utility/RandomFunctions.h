#ifndef RANDOMFUNCTIONS_H
#define RANDOMFUNCTIONS_H

#include <glm/glm.hpp>


glm::vec3 randomVec3ExcludeByDistance(float min = -1.0f, float max = 1.0f, float excludeMin = 0.0f, float excludeMax = 0.0f);
glm::vec3 randomVec3(float min = -1.0f, float max = 1.0f, float excludeMin=0.0f, float excludeMax=0.0f);
float randomFloat(float min = -1.0f, float max = 1.0f, float excludeMin = 0.0f, float excludeMax = 0.0f);

#endif