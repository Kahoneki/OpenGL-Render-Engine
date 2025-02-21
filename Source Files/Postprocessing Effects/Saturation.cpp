#include "Saturation.h"

#include "../World/Drawable/Special/PostprocessOverlay.h"
#include "../Shaders/compute_shader.h"

namespace PPEffect
{
	Saturation::Saturation()
	{
		data = new SaturationData();
		reinterpret_cast<SaturationData*>(data)->factor = 1.0f;
		UpdateUBO();

		shader = new ComputeShader(SHADER_PRESET::POSTPROCESS, POSTPROCESSING_EFFECT::SATURATION);
	}
	
	Saturation::~Saturation()
	{
	}

	void Saturation::SetFactor(float val)
	{
		reinterpret_cast<SaturationData*>(data)->factor = val;
		UpdateUBO();
	}
	
	float Saturation::GetFactor()
	{
		return reinterpret_cast<SaturationData*>(data)->factor;
	}

	::size_t Saturation::GetDataSize() const
	{
		return sizeof(SaturationData);
	}
}