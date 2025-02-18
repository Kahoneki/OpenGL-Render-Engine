#ifndef POSTPROCESSOVERLAY_H
#define POSTPROCESSOVERLAY_H

#include "../PrimitiveModel.h"
#include "FramebufferQuad.h"
#include <unordered_map>


class ComputeShader;

enum POSTPROCESSING_EFFECT
{
    NONE,
    SATURATION,
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

    void AddPostprocessingEffect(POSTPROCESSING_EFFECT effect);
    void RemovePostprocessingEffect(POSTPROCESSING_EFFECT effect);

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
    
    std::unordered_map<POSTPROCESSING_EFFECT, ComputeShader*> activeShaders; //All currently active postprocessing effects
    
    void Render(unsigned int outputFbo=0); //Render all active postprocessing effects onto `fbo` before rendering the final result to `outputFbo`

};

#endif