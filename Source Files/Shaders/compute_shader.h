#include "shader.h"

class ComputeShader : public Shader
{
public:
	ComputeShader(const char* shaderFilepath);
	ComputeShader(SHADER_PRESET preset, POSTPROCESSING_EFFECT effect);
};