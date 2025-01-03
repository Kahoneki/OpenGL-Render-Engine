#include "shader.h"

enum POSTPROCESSING_EFFECT;

class ComputeShader : public Shader
{
public:
	ComputeShader(const char* shaderFilepath);
	ComputeShader(SHADER_PRESET preset, POSTPROCESSING_EFFECT effect);

private:
	void ComputeShaderConstructor(const char* shaderFilepath);
};