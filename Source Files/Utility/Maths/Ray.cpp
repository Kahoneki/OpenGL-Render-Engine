#include "Ray.h"
#include "../../World/Drawable/Plane.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>

//Disable deprecation warning in EIGEN/Dense
#pragma warning(disable : 4996)
#include "EIGEN/Dense"

Ray::Ray(glm::vec3 _origin, glm::vec3 _endpoint)
{
    origin = _origin;
    endpoint = _endpoint;
}

bool Ray::IntersectsWithPlane(const Plane& plane)
{
    //Standard plane equation: Point a is on infinite plane with known point p and normal n if (a-p).n = 0
    glm::vec3 p{ plane.getPosition() };
    std::pair<glm::vec3, glm::vec3> axes{ plane.GetWorldAxes() };
    glm::vec3 n{ glm::normalize(glm::cross(axes.first, axes.second)) };

    //Standard ray equation: R(t) = o+td {0 <= t <= 1 for finite rays}
    glm::vec3 o{ origin };
    glm::vec3 d{ endpoint - origin };

    //Check edge case where ray and plane are parallel - avoid division by 0 error in next step
    double denom{ glm::dot(d, n) };
    if (std::abs(denom) < 1e-6)
    {
        //Ray is parallel to the plane (or very close to it)
        return false;
    }

    //Substitute R(t) into plane equation
    //Plane eqn: (a-p).n = 0
    //Ray eqn: R(t) = o+td
    //=> (o+td-p).n = 0
    //=> (o-p).n + (td).n = 0
    //=> (o-p).n + t(d.n) = 0
    //=> t = ((p-o).n) / (d.n)
    double t{ glm::dot((p-o), n) / denom };
    
    //Check that 0 <= t <= 1
    if (t < 0 || t > 1)
    {
        //The infinite extension of the finite ray intersects with the plane, but not this finite ray
        return false;
    }

    //The finite ray intersects with the infinite plane!
    //Now check if the finite ray intersects with the finite plane
    
    //Convert intersection point to plane's local coordinates
    glm::vec3 intersectionPoint{ o + glm::vec3(t) * d };
    glm::mat4 inverseModelMatrix{ glm::inverse(plane.GetHeirarchicalModelMatrix()) };
    glm::vec3 localIntersectionPoint{ glm::vec3(inverseModelMatrix * glm::vec4(intersectionPoint, 1.0f)) };

    //Check if localIntersectionPoint lies within the plane's vertex position bounds (-0.5f to 0.5f on both axes)
    //If it does, the finite ray intersects with the finite plane
    return (std::abs(localIntersectionPoint.x) <= 0.5f && std::abs(localIntersectionPoint.y) <= 0.5f);
}
