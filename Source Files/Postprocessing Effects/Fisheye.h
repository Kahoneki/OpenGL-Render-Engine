#ifndef FISHEYE_H
#define FISHEYE_H

#include "PostprocessingEffect.h"
#include <cstddef>

namespace PPEffect
{
	struct FisheyeData
	{
		float strength;
	};

	class Fisheye : public PostprocessingEffect
	{
	public:
		Fisheye();
		~Fisheye();
		
		void SetStrength(float val);
		float GetStrength();

		std::size_t GetDataSize() const override;
	};
}

#endif