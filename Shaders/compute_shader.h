#include "shader.h"
#include "../Source Files/World/Drawable/Special/PostprocessOverlay.h"

class ComputeShader : public Shader
{
public:
	ComputeShader(const char* shaderFilepath);
	ComputeShader(SHADER_PRESET preset, POSTPROCESSING_EFFECT effect);
	
private:
	void ComputeShaderConstructor(const char* shaderFilepath);
};