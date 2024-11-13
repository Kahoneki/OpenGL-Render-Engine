#ifndef VEC2UI64_H
#define VEC2UI64_H

#include <GLAD/glad.h>

struct vec2ui64
{
public:
	GLuint64 x;
	GLuint64 y;

	vec2ui64() : x(0), y(0) {}
	vec2ui64(GLuint64 a, GLuint64 b) : x(a), y(b) {}
};

#endif