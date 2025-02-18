#ifndef FRAMEBUFFERQUAD_H
#define FRAMEBUFFERQUAD_H

#include <GLAD/glad.h>
#include "../../../../Shaders/shader.h"

class FramebufferQuad
{
public:
    //Full screen-space quad with texture coordinates
    FramebufferQuad();
    ~FramebufferQuad();

    void Draw();

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Shader framebufferShader;
};

#endif