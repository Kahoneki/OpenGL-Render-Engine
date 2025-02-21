#ifndef POSTPROCESSING_EFFECT_H
#define POSTPROCESSING_EFFECT_H


#include <vector>
#include <cstddef>

class ComputeShader;

namespace PPEffect
{

	//For polymorphism - should be inherited by all PostprocessingEffect classes
	class PostprocessingEffect
	{
	public:
		void PrepareState(std::size_t pass); //Binds shader and UBO
		std::size_t GetNumPasses();

	protected:
		PostprocessingEffect(std::size_t _numPasses);
		virtual ~PostprocessingEffect();

		//parallel vectors - each pass in a singular post-processing effect gets one compute shader, one data struct, and one ubo
		std::vector<ComputeShader*> shaders;
		std::vector<void*> data;
		std::vector<unsigned int> ubos; //UBOs of PPEffectData to be sent to the compute shader
		bool ubosInitialised;
		void UpdateUBO(std::size_t index);
		void UpdateAllUBOs();

		virtual std::size_t GetDataSize() const = 0;

	private:
		std::size_t numPasses;
	};
}

#endif