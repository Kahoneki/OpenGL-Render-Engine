#include "compute_shader.h"

#include <fstream>
#include <sstream>

#include <iostream>
#include "Windows.h"
#include <string>

#include "../Source Files/World/Drawable/Special/PostprocessOverlay.h"

void ComputeShader::ComputeShaderConstructor(const char* shaderFilepath)
{
    std::string code;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        char cwd[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, cwd);
        shaderFile.open(std::string(cwd).append("/Shaders/").append(shaderFilepath));
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* shaderCode = code.c_str();


    unsigned int shader{ glCreateShader(GL_COMPUTE_SHADER) };
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    {
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
            glDeleteShader(shader);
            exit(-1);
        }
    }

    id = glCreateProgram();
    glAttachShader(id, shader);
    glLinkProgram(id);

    //Deallocate vert and frag resources
    glDeleteShader(shader);
}


ComputeShader::ComputeShader(const char* shaderFilepath)
{
    ComputeShaderConstructor(shaderFilepath);
}

ComputeShader::ComputeShader(SHADER_PRESET preset, POSTPROCESSING_EFFECT effect)
{
    const char* shaderFilepath;

    switch (preset)
    {
    case SHADER_PRESET::POSTPROCESS:
    {
        switch (effect)
        {
        case POSTPROCESSING_EFFECT::NONE:
        {
            std::cerr << "Postprocessing effect set to none." << std::endl;
            shaderFilepath = "";
            break;
        }
        case POSTPROCESSING_EFFECT::SATURATION:
        {
            shaderFilepath = "Postprocess/saturation.comp";
            break;
        }
        default:
            std::cerr << "Postprocessing effect not found: " << effect << std::endl;
            shaderFilepath = "";
            break;
        }
        break;
    }

    default:
    {
        std::cerr << "Shader preset not found: " << preset << std::endl;
        shaderFilepath = "";
        break;
    }
    }

   ComputeShaderConstructor(shaderFilepath);
}
