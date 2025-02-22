#ifndef SATURATION_H
#define SATURATION_H

#include "PostprocessingEffect.h"
#include <cstddef>

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

		std::size_t GetDataSize() const override;
	};
}

#endif