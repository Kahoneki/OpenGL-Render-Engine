#ifndef POSTPROCESSING_EFFECT_H
#define POSTPROCESSING_EFFECT_H


class ComputeShader;

namespace PPEffect
{

	//For polymorphism - should be inherited by all PostprocessingEffect classes
	class PostprocessingEffect
	{
	public:
		void PrepareState(); //Binds shader and UBO

	protected:
		PostprocessingEffect();
		virtual ~PostprocessingEffect();

		ComputeShader* shader;

		void* data;
		unsigned int ubo; //UBO of PPEffectData to be sent to the compute shader
		bool uboInitialised;
		void UpdateUBO();

		virtual ::size_t GetDataSize() const = 0;
	};
}

#endif