#ifndef RAY_H
#define RAY_H


#include <glm/vec3.hpp>

class Plane;

//Takes form a+tb
struct Ray
{
    glm::vec3 origin;
    glm::vec3 endpoint;

    Ray(glm::vec3 _origin, glm::vec3 _endpoint);

    bool IntersectsWithPlane(const Plane& plane, float startRayBuffer=0, float endRayBuffer=0);
};


#endif