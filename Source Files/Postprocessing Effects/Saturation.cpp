#include "Saturation.h"

#include "../World/Drawable/Special/PostprocessOverlay.h"
#include "../Shaders/compute_shader.h"

namespace PPEffect
{
	Saturation::Saturation() : PostprocessingEffect(1)
	{
		data.push_back(new SaturationData());
		reinterpret_cast<SaturationData*>(data[0])->factor = 1.0f;
		UpdateUBO(0);

		shaders.push_back(new ComputeShader(SHADER_PRESET::POSTPROCESS, POSTPROCESSING_EFFECT::SATURATION));
	}
	
	Saturation::~Saturation()
	{
	}

	void Saturation::SetFactor(float val)
	{
		reinterpret_cast<SaturationData*>(data[0])->factor = val;
		UpdateUBO(0);
	}
	
	float Saturation::GetFactor()
	{
		return reinterpret_cast<SaturationData*>(data[0])->factor;
	}

	::size_t Saturation::GetDataSize() const
	{
		return sizeof(SaturationData);
	}
}