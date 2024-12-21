#include "PostprocessOverlay.h"
#include "../../../Application/Application.h"
#include "../../../Application/WindowManager.h"
#include "../../../Utility/BindingPoints.h"
#include "../../../Shaders/compute_shader.h"
#include <iostream>


PostprocessOverlay::PostprocessOverlay()
{

	//FBO setup
	int SCRWIDTH{ Application::getInstance().windowManager.get()->SCRWIDTH };
	int SCRHEIGHT{ Application::getInstance().windowManager.get()->SCRHEIGHT };

	glCreateFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glCreateTextures(GL_TEXTURE_2D, 1, &colTex);
	glTextureStorage2D(colTex, 1, GL_RGBA8, SCRWIDTH, SCRHEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colTex, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	colTexHandle = glGetTextureHandleARB(colTex);
	glMakeTextureHandleResidentARB(colTexHandle);

	glCreateTextures(GL_TEXTURE_2D, 1, &depthStencilTex);
	glTextureStorage2D(depthStencilTex, 1, GL_DEPTH32F_STENCIL8, SCRWIDTH, SCRHEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilTex, 0);
	depthStencilTexHandle = glGetTextureHandleARB(depthStencilTex);
	glMakeTextureHandleResidentARB(depthStencilTexHandle);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	//SSBO setup
	glCreateBuffers(1, &fboTexturesBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::FBO_TEXTURES, fboTexturesBuffer);
}

PostprocessOverlay::~PostprocessOverlay()
{
	glDeleteFramebuffers(1, &fbo);
	glMakeTextureHandleNonResidentARB(colTexHandle);
	glDeleteTextures(1, &colTex);
	glMakeTextureHandleNonResidentARB(depthStencilTexHandle);
	glDeleteTextures(1, &depthStencilTex);
}


void PostprocessOverlay::Render(unsigned int outputFbo)
{

}


void PostprocessOverlay::AddPostprocessingEffect(POSTPROCESSING_EFFECT effect)
{
	if (activeShaders.count(effect) != 0) {
		//Effect already in map
		std::cerr << "Attempted to add post-processing effect to overlay that was already using the effect. Effect: " << effect << std::endl;
		return;
	}

	activeShaders[effect] = new ComputeShader(SHADER_PRESET::POSTPROCESS, effect);
}

void PostprocessOverlay::RemovePostprocessingEffect(POSTPROCESSING_EFFECT effect)
{
	if (!activeShaders.erase(effect)) {
		//Effect not in map
		std::cerr << "Attempted to remove post-processing effect from overlay that wasn't using the effect. Effect: " << effect << '\n';
	}
}