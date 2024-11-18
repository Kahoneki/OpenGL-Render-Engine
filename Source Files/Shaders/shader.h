#ifndef SHADER_H
#define SHADER_H

#include "GLAD/glad.h"
#include <GLM/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

enum SHADER_PRESET
{
	DEFAULT,
	STATIC_COLOUR,
	STATIC_MATERIAL,
	BLINN_PHONG,
	LIGHT_SOURCE,
	FBO,
};


class Shader
{
public:
	Shader(const char* vertexFilepath, const char* fragmentFilepath);
	Shader(SHADER_PRESET preset);
	~Shader();

	unsigned int id;
	void use() const;
	void setBool(const char* name, bool value) const;
	void setBoolArray(const char* name, const std::vector<bool>& values) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setFloatArray(const char* name, const std::vector<float>& values) const;
	void setMat4(const char* name, const glm::mat4& value) const;
	void setVec2(const char* name, const glm::vec2& value) const;
	void setVec2Array(const char* name, const std::vector<glm::vec2>& values) const;
	void setVec3(const char* name, const glm::vec3& value) const;
};

#endif