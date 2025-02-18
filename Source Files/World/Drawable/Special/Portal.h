#ifndef PORTAL_H
#define PORTAL_H

#include "../PrimitiveModel.h"
#include "../Cube24.h"
#include "../Plane.h"
#include "FramebufferQuad.h"

class Portal : public PrimitiveModel
{
public:
    //Portal implemented by making use of stencil buffer masking
    Portal(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent = nullptr);

    //After both portals have been created, they can be linked by calling this function on each portal passing in the other portal
    void SetOtherPortal(Portal* portal);

    void Draw(Shader& shader) override;

    Cube24 portal;
    Plane renderRegion;

    unsigned int stencilValue; //Value used when rendering this portal - will be unique among portals in the same scene

private:
    Portal* otherPortal;
    static unsigned int numPortalsInScene;

    FramebufferQuad fbQuad;

    void CheckForCameraCollision();
    static bool cameraCanTeleport;
    static float teleportCooldown;
    static float teleportCooldownTimer;
};

#endif