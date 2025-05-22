#include "PostprocessingEffect.h"

#include <GLAD/glad.h>
#include "../Utility/BindingPoints.h"

#include "../../Shaders/compute_shader.h"

namespace PPEffect
{

	std::size_t PostprocessingEffect::GetNumPasses()
	{
		return numPasses;
	}

	PostprocessingEffect::PostprocessingEffect(std::size_t _numPasses)
	{
		numPasses = _numPasses;
		ubos.resize(numPasses);
		glCreateBuffers(numPasses, ubos.data());
		//UBO storage can't be assigned in the constructor because it relies on the pure virtual GetDataSize() which doesn't have an implementation at this point in execution
		ubosInitialised = false;
	}
	PostprocessingEffect::~PostprocessingEffect()
	{
		glDeleteBuffers(numPasses, ubos.data());
		for (std::size_t i{ 0 }; i < shaders.size(); ++i)
		{
			delete shaders[i];
			delete data[i];
		}
	}

	void PostprocessingEffect::UpdateUBO(std::size_t index)
	{
		if (!ubosInitialised)
		{
			for (unsigned int ubo : ubos)
			{
				glNamedBufferStorage(ubo, GetDataSize(), nullptr, GL_DYNAMIC_STORAGE_BIT);
			}
			ubosInitialised = true;
		}
		glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::POSTPROCESS_EFFECT_DATA, ubos[index]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, GetDataSize(), data[index]);
	}

	void PostprocessingEffect::UpdateAllUBOs()
	{
		if (!ubosInitialised)
		{
			for (unsigned int ubo : ubos)
			{
				glNamedBufferStorage(ubo, GetDataSize(), nullptr, GL_DYNAMIC_STORAGE_BIT);
			}
			ubosInitialised = true;
		}
		for (std::size_t i{ 0 }; i<ubos.size(); ++i)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::POSTPROCESS_EFFECT_DATA, ubos[i]);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, GetDataSize(), data[i]);
		}
	}

	void PostprocessingEffect::PrepareState(std::size_t pass)
	{
		shaders[pass]->use();
		glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::POSTPROCESS_EFFECT_DATA, ubos[pass]);
	}
}