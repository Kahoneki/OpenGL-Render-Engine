#include "Ray.h"
#include "../../World/Drawable/Plane.h"

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
    glm::vec3 p{ plane.getPosition() };
    std::pair<glm::vec3, glm::vec3> axes{ plane.GetWorldAxes() };
    glm::vec3 a{ axes.first };
    glm::vec3 b{ axes.second };

    glm::vec3 o{ origin };
    glm::vec3 d{ endpoint - origin };

    //Equations of ray and plane can be equated
    //r = p + ua + vb
    //r = o + td
    //->p + ua + vb = o + td

    //Convert to parametric form
    //p.x + ua.x + vb.x = o.x + td.x
    //p.y + ua.y + vb.y = o.y + td.y
    //p.z + ua.z + vb.z = o.z + td.z

    //Organise into form suitable for system of equations
    //ua.x + vb.x - td.x = o.x - p.x
    //ua.y + vb.y - td.y = o.y - p.y
    //ua.z + vb.z - td.z = o.z - p.z

    //Create coefficient matrix
    Eigen::Matrix3d A;
    A << a.x, b.x, -d.x,
         a.y, b.y, -d.y,
         a.z, b.z, -d.z;

    //Create right hand vector
    Eigen::Vector3d B;
    B << o.x - p.x,
         o.y - p.y,
         o.z - p.z;

    //Solve system of equations
    Eigen::VectorXd x = A.colPivHouseholderQr().solve(B);

    //Verify that solution is real
    Eigen::VectorXd B_calculated = A * x;
    double tolerance = 1e-9;
    if ((B - B_calculated).norm() > tolerance)
    {
        //Solution does not exist
        return false;
    }
    //Solution exists, intersection only occurs if solution in range u,v,t [0,1]
    return (x[0] >= 0 && x[0] <= 1 && x[1] >= 0 && x[1] <= 1 && x[2] >= 0 && x[2] <= 1);
}
