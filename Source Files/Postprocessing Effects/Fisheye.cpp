#include "Fisheye.h"

#include "../World/Drawable/Special/PostprocessOverlay.h"
#include "../Shaders/compute_shader.h"

namespace PPEffect
{
	Fisheye::Fisheye() : PostprocessingEffect(1)
	{
		data.push_back(new FisheyeData());
		reinterpret_cast<FisheyeData*>(data[0])->strength = 1.0f;
		UpdateUBO(0);

		shaders.push_back(new ComputeShader(SHADER_PRESET::POSTPROCESS, POSTPROCESSING_EFFECT::FISHEYE));
	}
	
	Fisheye::~Fisheye()
	{
	}

	void Fisheye::SetStrength(float val)
	{
		reinterpret_cast<FisheyeData*>(data[0])->strength = val;
		UpdateUBO(0);
	}
	
	float Fisheye::GetStrength()
	{
		return reinterpret_cast<FisheyeData*>(data[0])->strength;
	}

	std::size_t Fisheye::GetDataSize() const
	{
		return sizeof(FisheyeData);
	}
}