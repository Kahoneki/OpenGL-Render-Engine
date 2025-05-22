#include "Vignette.h"

#include "../World/Drawable/Special/PostprocessOverlay.h"
#include "../../Shaders/compute_shader.h"

namespace PPEffect
{
	Vignette::Vignette() : PostprocessingEffect(1)
	{
		data.push_back(new VignetteData());
		reinterpret_cast<VignetteData*>(data[0])->strength = 1.0f;
		UpdateUBO(0);

		shaders.push_back(new ComputeShader(SHADER_PRESET::POSTPROCESS, POSTPROCESSING_EFFECT::VIGNETTE));
	}
	
	Vignette::~Vignette()
	{
	}

	void Vignette::SetStrength(float val)
	{
		reinterpret_cast<VignetteData*>(data[0])->strength = val;
		UpdateUBO(0);
	}
	
	float Vignette::GetStrength()
	{
		return reinterpret_cast<VignetteData*>(data[0])->strength;
	}

	std::size_t Vignette::GetDataSize() const
	{
		return sizeof(VignetteData);
	}
}