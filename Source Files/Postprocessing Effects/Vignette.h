#ifndef VIGNETTE_H
#define VIGNETTE_H

#include "PostprocessingEffect.h"
#include <cstddef>

namespace PPEffect
{
	struct VignetteData
	{
		float strength;
	};

	class Vignette : public PostprocessingEffect
	{
	public:
		Vignette();
		~Vignette();
		
		void SetStrength(float val);
		float GetStrength();

		std::size_t GetDataSize() const override;
	};
}

#endif