#include "PostprocessingEffect.h"

#include <GLAD/glad.h>
#include "../Utility/BindingPoints.h"

#include "../Shaders/compute_shader.h"

namespace PPEffect
{

	PostprocessingEffect::PostprocessingEffect()
	{
		glCreateBuffers(1, &ubo);
		//UBO storage can't be assigned in the constructor because it relies on the pure virtual GetDataSize() which doesn't have an implementation at this point in execution
		uboInitialised = false;
		shader = nullptr;
		data = nullptr;
	}
	PostprocessingEffect::~PostprocessingEffect()
	{
		glDeleteBuffers(1, &ubo);
		delete shader;
		delete data;
	}

	void PostprocessingEffect::UpdateUBO()
	{
		if (!uboInitialised)
		{
			glNamedBufferStorage(ubo, GetDataSize(), nullptr, GL_DYNAMIC_STORAGE_BIT);
			uboInitialised = true;
		}
		glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::POSTPROCESS_EFFECT_DATA, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, GetDataSize(), data);
	}

	void PostprocessingEffect::PrepareState()
	{
		shader->use();
		glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::POSTPROCESS_EFFECT_DATA, ubo);
	}
}