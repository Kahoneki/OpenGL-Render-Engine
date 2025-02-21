#ifndef SATURATION_H
#define SATURATION_H

#include "PostprocessingEffect.h"

namespace PPEffect
{
	struct SaturationData
	{
		float factor;
	};

	class Saturation : public PostprocessingEffect
	{
	public:
		Saturation();
		~Saturation();
		
		void SetFactor(float val);
		float GetFactor();

		::size_t GetDataSize() const override;
	};
}

#endif