#include "../Shaders/shader.h"

#include "GLM/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>

#include <iostream>
#include "Windows.h"
#include <string>

Shader::Shader(const char* vertexFilepath, const char* fragmentFilepath)
{
    //1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //Ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //Open files
        char cwd[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, cwd);
        vShaderFile.open(std::string(cwd).append("/Source Files/Shaders/").append(vertexFilepath));
        fShaderFile.open(std::string(cwd).append("/Source Files/Shaders/").append(fragmentFilepath));
        std::stringstream vShaderStream, fShaderStream;
        //Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //Close file handlers
        vShaderFile.close();
        fShaderFile.close();
        //Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


    //2. Compile and link shaders
    unsigned int vert{ glCreateShader(GL_VERTEX_SHADER) };
    unsigned int frag{ glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(vert, 1, &vShaderCode, NULL);
    glShaderSource(frag, 1, &fShaderCode, NULL);
    glCompileShader(vert);
    {
        int success;
        char infoLog[512];
        glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vert, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
            glDeleteShader(vert);
            exit(-1);
        }
    }
    glCompileShader(frag);
    {
        int success;
        char infoLog[512];
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(frag, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
            glDeleteShader(frag);
            exit(-1);
        }
    }

    id = glCreateProgram();
    glAttachShader(id, vert);
    glAttachShader(id, frag);
    glLinkProgram(id);

    //Deallocate vert and frag resources
    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::Shader(SHADER_PRESET preset)
{
    const char* vertexFilepath;
    const char* fragmentFilepath;

    switch (preset)
    {
    case SHADER_PRESET::DEFAULT:
    {
        vertexFilepath = "default.vert";
        fragmentFilepath = "default.frag";
        break;
    }
    case SHADER_PRESET::STATIC_COLOUR:
    {
        vertexFilepath = "static_colour.vert";
        fragmentFilepath = "static_colour.frag";
        break;
    }
    case SHADER_PRESET::STATIC_MATERIAL:
    {
        vertexFilepath = "static_material.vert";
        fragmentFilepath = "static_material.frag";
        break;
    }
    case SHADER_PRESET::BLINN_PHONG:
    {
        vertexFilepath = "blinn_phong.vert";
        fragmentFilepath = "blinn_phong.frag";
        break;
    }
    case SHADER_PRESET::LIGHT_SOURCE:
    {
        vertexFilepath = "light_source.vert";
        fragmentFilepath = "light_source.frag";
        break;
    }
    case SHADER_PRESET::FBO:
    {
        vertexFilepath = "fbo.vert";
        fragmentFilepath = "fbo.frag";
        break;
    }
    default:
    {
        std::cerr << "Shader preset not found: " << preset << std::endl;
        vertexFilepath = "";
        fragmentFilepath = "";
        break;
    }
    }

    *this = Shader(vertexFilepath, fragmentFilepath);
}

Shader::Shader() {}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::use() const
{
    glUseProgram(id);
}

void Shader::setBool(const char* name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name), static_cast<int>(value));
}

void Shader::setBoolArray(const char* name, const std::vector<bool>& values) const
{
    //Convert std::vector<bool> to std::vector<int>
    std::vector<int> intValues;
    for (const bool& val : values) {
        intValues.push_back(static_cast<int>(val));
    }

    glUniform1iv(glGetUniformLocation(id, name), values.size(), intValues.data());
}

void Shader::setInt(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::setFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::setFloatArray(const char* name, const std::vector<float>& values) const
{
    glUniform1fv(glGetUniformLocation(id, name), values.size(), values.data());
}

void Shader::setMat4(const char* name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec2(const char* name, const glm::vec2& value) const
{
    glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}

void Shader::setVec2Array(const char* name, const std::vector<glm::vec2>& values) const
{
    //Convert std::vector<glm::vec2> to std::vector<GLfloat>
    //std::memcpy(floatValues.data(), values.data(), sizeof(values));
    std::vector<GLfloat> floatValues;
    for (const glm::vec2& val : values) {
        floatValues.push_back(val.x);
        floatValues.push_back(val.y);
    }

    glUniform2fv(glGetUniformLocation(id, name), values.size(), floatValues.data());
}

void Shader::setVec3(const char* name, const glm::vec3& value) const
{
    glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}
