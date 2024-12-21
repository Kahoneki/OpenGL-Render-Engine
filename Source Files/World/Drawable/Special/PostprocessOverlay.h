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
    unsigned int colTex;
    GLuint64 colTexHandle;
    unsigned int depthStencilTex;
    GLuint64 depthStencilTexHandle;
    unsigned int fboTexturesBuffer;

    std::unordered_map<POSTPROCESSING_EFFECT, ComputeShader*> activeShaders; //All currently active postprocessing effects
    
    void Render(unsigned int outputFbo=0); //Render all active postprocessing effects onto `fbo` before rendering the final result to `outputFbo`

};

#endif