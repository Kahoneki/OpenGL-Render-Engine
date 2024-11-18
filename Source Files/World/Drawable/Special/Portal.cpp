#include "Portal.h"
#include "../../../Shaders/shader.h"
#include "../../../Application/Application.h"
#include "../../../Application/Renderer.h"
#include "../../../Application/SceneManager.h"
#include "../../../Application/InputManager.h"
#include "../../../Application/WindowManager.h"
#include "../../Scene.h"
#include "../../../Camera/Camera.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>

unsigned int Portal::numPortalsInScene = 0;
bool Portal::cameraCanTeleport = true;

Portal::Portal(const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent) :
    SceneObject(name, parent, Transform(center, scale, rotation)),
    Drawable(name, center, scale, rotation, parent),
    PrimitiveModel(name, center, scale, rotation, parent),
    portal("Container", glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), this),
    renderRegion("Render Region", glm::vec3(0.0f, 0.0f, 1.01f), glm::vec3(1.0f), glm::vec3(0.0f), this)
{
    renderOrder = UINT_MAX - Portal::numPortalsInScene;
    ++Portal::numPortalsInScene;
    stencilValue = Portal::numPortalsInScene;

    renderRegion.useOnlyDiffuse = true;

    renderRegion.material.setDiffuseColour(Application::getInstance().renderer.get()->GetClearColour());
    renderRegion.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
}

void Portal::SetOtherPortal(Portal* portal)
{
    otherPortal = portal;
}

void Portal::Draw(Shader& shader)
{

    //Stencil buffer mask rendering algorithm:
    //-Stencil buffer is cleared to 0 (in Renderer::Render())
    //-Stencil writes are disabled
    //-Portal outline is drawn
    //-Stencil writes are enabled
    //-Render region is drawn with flat shading (useOnlyDiffuse=true where diffuse is the clear colour), writing stencilValue into the stencil buffer
    //-Stencil writes are disabled
    //-Active render source's transform relative to this portal is used to find transform relative to otherPortal to create a new view matrix
    //-Depth buffer is cleared in portal region (where )
    //-Stencil func is set to only render where there is a value of stencilValue in the stencil buffer
    //-All drawables in scene with a render order < portal's render order are re-rendered using new view matrix

    //--FIRST PASS--//
    //Draw outline
    portal.Draw(shader);
    //Draw render region, writing stencilValue into the stencil buffer
    renderRegion.material.setDiffuseColour(Application::getInstance().renderer.get()->GetClearColour());
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, stencilValue, 0xFF);
    renderRegion.Draw(shader);
    glStencilMask(0x00);


    //--SECOND PASS--//
    //Calculate new view matrix and attach to shader
    
    glm::mat4 activeRenderSourceViewMatrix{ dynamic_cast<Camera*>(sceneParent->GetActiveRenderSource())->GetViewMatrix() };
    glm::mat4 renderSourceTransformRelativeToThisPortal{ glm::inverse(GetHeirarchicalModelMatrix()) * glm::inverse(activeRenderSourceViewMatrix) };
    glm::vec3 rotationBefore{ otherPortal->getRotation() };
    otherPortal->setRotation(rotationBefore + glm::vec3(-2 * rotationBefore.x, 180.0f, 0.0f));
    glm::mat4 view{ glm::inverse(otherPortal->GetHeirarchicalModelMatrix() * renderSourceTransformRelativeToThisPortal) };
    otherPortal->setRotation(rotationBefore);

    if (Application::getInstance().inputManager.get()->flip) {
        shader.setMat4("view", activeRenderSourceViewMatrix);
    }
    else {
        shader.setMat4("view", view);
    }
    

    //Only render where there is a value of stencilValue in the buffer
    glStencilFunc(GL_EQUAL, stencilValue, 0xFF);
    
    //Clear the depth buffer within the portal region
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthFunc(GL_ALWAYS);
    fbQuad.Draw();
    shader.use();
    glDepthFunc(GL_LESS);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


    //Re-render all drawables that have been rendered this frame besides portals (those with a render order < any portals')
    unsigned int previousRenderOrder{ Application::getInstance().sceneManager->GetActiveScene()->drawables[0]->getRenderOrder() };
    for (Drawable* drawable : Application::getInstance().sceneManager->GetActiveScene()->drawables) {
        if (drawable->getRenderOrder() >= UINT_MAX - numPortalsInScene - 1) {
            break;
        }
        if (drawable->getRenderOrder() != previousRenderOrder) {
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            previousRenderOrder = drawable->getRenderOrder();
        }
        drawable->Draw(shader);
    }

    if (Application::getInstance().inputManager.get()->flip) {
        shader.setMat4("view", view);
    }
    else {
        shader.setMat4("view", activeRenderSourceViewMatrix);
    }
    glStencilFunc(GL_ALWAYS, 0, 0xFF);

    CheckForCameraCollision();
}


bool pointInsideModelMatrix(glm::vec3 p, glm::mat4 m)
{
    //Transform point into model matrix's space and see if it lies within unit cube
    float epsilon{ 0.1f };
    glm::vec3 transformedPoint{ glm::inverse(m) * glm::vec4(p, 1.0f) };
    return (transformedPoint.x >= (-1.0f - epsilon) && transformedPoint.x <= (1.0f + epsilon) &&
        transformedPoint.y >= (-1.0f - epsilon) && transformedPoint.y <= (1.0f + epsilon) &&
        transformedPoint.z >= (-1.0f - epsilon) && transformedPoint.z <= (1.0f + epsilon));
}

void Portal::CheckForCameraCollision()
{

    Camera* camera{ dynamic_cast<Camera*>(sceneParent->GetActiveRenderSource()) };
    bool inPortal{ pointInsideModelMatrix(camera->getPosition(), portal.GetHeirarchicalModelMatrix()) };
    bool inOtherPortal{ pointInsideModelMatrix(camera->getPosition(), otherPortal->GetHeirarchicalModelMatrix()) };
    
    if (!inPortal && !inOtherPortal) {
        //Use high-precision collision check with camera's inbetween-frame interpolated positions
        for (const glm::vec3& pos : camera->interpolatedPositionsBetweenFrames) {
            if (pointInsideModelMatrix(pos, portal.GetHeirarchicalModelMatrix())) {
                inPortal = true;
                break;
            }
            else if (pointInsideModelMatrix(pos, otherPortal->GetHeirarchicalModelMatrix())) {
                inOtherPortal = true;
                break;
            }
        }
    }

    if (!inPortal && !inOtherPortal) {
        Portal::cameraCanTeleport = true;
    }

    if (!Portal::cameraCanTeleport) {
        return;
    }

    glm::vec3 portalPos{ portal.GetHeirarchicalModelMatrix()[3] };
    glm::vec3 renderRegionPos{ renderRegion.GetHeirarchicalModelMatrix()[3] };
    glm::vec3 portalNorm{ glm::normalize(renderRegionPos - portalPos) };

    glm::vec3 otherPortalPos{ otherPortal->GetHeirarchicalModelMatrix()[3] };
    glm::vec3 otherRenderRegionPos{ otherPortal->renderRegion.GetHeirarchicalModelMatrix()[3] };
    glm::vec3 otherPortalNorm{ glm::normalize(otherRenderRegionPos - otherPortalPos) };

    glm::vec3 portalToCam{ camera->getPosition() - renderRegionPos };

    if ((glm::dot(portalNorm, portalToCam) < 0) && inPortal) {

        //Camera has moved to other side of portal, teleport it to other portal
        glm::mat4 cameraRelativeToPortal{ glm::inverse(renderRegion.GetHeirarchicalModelMatrix()) * glm::inverse(camera->GetViewMatrix()) };
        glm::mat4 newTransform{ otherPortal->renderRegion.GetHeirarchicalModelMatrix() * cameraRelativeToPortal };
        camera->setPosition(newTransform[3]);

        // Calculate rotation axis and angle using dot product
        double dotProduct{ glm::dot(portalNorm, otherPortalNorm) };
        if (dotProduct == -1) {
            //Vectors are opposite, no rotation needed
        }
        else  {
            if (dotProduct == 1) {
                //Vectors are parallel, nudge otherPortalNorm very slightly to avoid NaN cross product
                otherPortalNorm.x += 0.00000001;
            }
            glm::vec3 rotationAxis = glm::normalize(glm::cross(portalNorm, -otherPortalNorm));
            float rotationAngle = acos(glm::dot(portalNorm, -otherPortalNorm));

            // Create rotation matrix
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAngle, rotationAxis);

            glm::vec3 front{ rotationMatrix * glm::vec4(camera->Front, 0.0f) };

            float pitch{ glm::degrees(asin(front.y)) };
            float yaw{ glm::degrees(atan2(front.z, front.x)) };
            camera->setRotation(glm::vec3(yaw, pitch, 0.0f));
        }

        camera->setPosition(camera->getPosition() + camera->Front * portal.getScale());

        Portal::cameraCanTeleport = false;
    }
}