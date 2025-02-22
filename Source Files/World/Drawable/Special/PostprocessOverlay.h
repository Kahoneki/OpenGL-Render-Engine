#ifndef POSTPROCESSOVERLAY_H
#define POSTPROCESSOVERLAY_H

#include "../PrimitiveModel.h"
#include "FramebufferQuad.h"
#include <vector>
#include "../../../Postprocessing Effects/PostprocessingEffect.h"


class ComputeShader;

enum POSTPROCESSING_EFFECT
{
    NONE,
    SATURATION,
    VIGNETTE,
    FISHEYE,
};

struct FBOTextureHandles
{
    glm::u64vec2 input; //Tightly packed colTexHandle and depthStencilTexHandle
};

class PostprocessOverlay
{

    friend class Renderer;

public:
    PostprocessOverlay();
    ~PostprocessOverlay();

    void AddPostprocessingEffect(PPEffect::PostprocessingEffect* effect);
    void RemovePostprocessingEffect(PPEffect::PostprocessingEffect* effect);

private:
    FramebufferQuad fbQuad;
    unsigned int fbo;
    
    //Unchanging between passes - always holds the initial scene data prior to any postprocessing effects
    unsigned int colTex;
    GLuint64 colTexHandle;
    unsigned int depthStencilTex;
    GLuint64 depthStencilTexHandle;

    unsigned int fboTextureHandlesBuffer; //Buffer storing fboTextureHandles
    FBOTextureHandles fboTextureHandles;
    
    //Used for ping-pong buffering
    //intermediateCol1Image will match colTex on first pass
    unsigned int intermediateCol1Image;
    unsigned int intermediateCol2Image;
    
    std::vector<PPEffect::PostprocessingEffect*> activeEffects;
    
    //For internal use only - use Render() instead
    void RunShader(PPEffect::PostprocessingEffect* ppe, bool& swap);

    void Render(unsigned int outputFbo=0); //Render all active postprocessing effects onto `fbo` before rendering the final result to `outputFbo`

    void OnScreenSizeChange(int scrwidth, int scrheight); //Called by WindowManager as callback

};

#endif