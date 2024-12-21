#ifndef FRAMEBUFFERQUAD_H
#define FRAMEBUFFERQUAD_H

#include <GLAD/glad.h>
#include "../../../Shaders/shader.h"

class FramebufferQuad
{
public:
    //Full screen-space quad with texture coordinates
    FramebufferQuad();
    ~FramebufferQuad();

    void Draw(); //Remember to rebind your previous shader after calling

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Shader& framebufferShader;
};

#endif