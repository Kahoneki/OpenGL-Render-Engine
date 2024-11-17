#include "RandomFunctions.h"
#include <random>
#include <functional>

glm::vec3 randomVec3ExcludeByDistance(float min, float max, float excludeMin, float excludeMax)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);

    glm::vec3 value;
    do {
        value = glm::vec3(dist(gen), dist(gen), dist(gen));
    } while (glm::length(value) >= excludeMin && glm::length(value) <= excludeMax);

    return value;
}

glm::vec3 randomVec3(float min, float max, float excludeMin, float excludeMax)
{
    return glm::vec3(
            randomFloat(min, max, excludeMin, excludeMax),
            randomFloat(min, max, excludeMin, excludeMax),
            randomFloat(min, max, excludeMin, excludeMax)
        );
}

float randomFloat(float min, float max, float excludeMin, float excludeMax)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);

    float value;
    do {
        value = dist(gen);
    } while (value >= excludeMin && value <= excludeMax);
    return value;
}