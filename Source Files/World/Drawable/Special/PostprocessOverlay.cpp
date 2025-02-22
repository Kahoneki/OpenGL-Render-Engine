#include "PostprocessOverlay.h"
#include "../../../Application/Application.h"
#include "../../../Application/WindowManager.h"
#include "../../../Utility/BindingPoints.h"
#include "../../../../Shaders/compute_shader.h"
#include "../../../Postprocessing Effects/PostprocessingEffect.h"
#include <iostream>


PostprocessOverlay::PostprocessOverlay() : fbQuad(SHADER_PRESET::FBO_POSTPROCESS)
{

	//Store currently bound FBO for rebinding after this FBO's setup
	int drawFbo, readFbo;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFbo);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFbo);

	//Create FBO of screen size with RGBA8 colour texture and D32S8 depth-stencil texture
	const int SCRWIDTH{ Application::getInstance().windowManager.get()->SCRWIDTH };
	const int SCRHEIGHT{ Application::getInstance().windowManager.get()->SCRHEIGHT };

	glCreateFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glCreateTextures(GL_TEXTURE_2D, 1, &colTex);
	glTextureStorage2D(colTex, 1, GL_RGBA8, SCRWIDTH, SCRHEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colTex, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	colTexHandle = glGetTextureHandleARB(colTex);
	glMakeTextureHandleResidentARB(colTexHandle);
	fboTextureHandles.input.x = colTexHandle;

	glCreateTextures(GL_TEXTURE_2D, 1, &depthStencilTex);
	glTextureStorage2D(depthStencilTex, 1, GL_DEPTH32F_STENCIL8, SCRWIDTH, SCRHEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilTex, 0);
	depthStencilTexHandle = glGetTextureHandleARB(depthStencilTex);
	glMakeTextureHandleResidentARB(depthStencilTexHandle);
	fboTextureHandles.input.y = depthStencilTexHandle;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, readFbo);


	//Create intermediate colour buffer images for ping-pong buffering between multiple shader passes
	glCreateTextures(GL_TEXTURE_2D, 1, &intermediateCol1Image);
	glTextureStorage2D(intermediateCol1Image, 1, GL_RGBA8, SCRWIDTH, SCRHEIGHT);
	glBindTexture(GL_TEXTURE_2D, intermediateCol1Image);

	glCreateTextures(GL_TEXTURE_2D, 1, &intermediateCol2Image);
	glTextureStorage2D(intermediateCol2Image, 1, GL_RGBA8, SCRWIDTH, SCRHEIGHT);
	

	//UBO setup
	glCreateBuffers(1, &fboTextureHandlesBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::FBO_TEXTURE_HANDLES, fboTextureHandlesBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(FBOTextureHandles), &fboTextureHandles, GL_STATIC_DRAW);


	//Join callbacks list for screen size changes
	Application::getInstance().windowManager.get()->JoinScreenSizeChangeCallbacksList([this](int scrwidth, int scrheight) { OnScreenSizeChange(scrwidth, scrheight); });
}

PostprocessOverlay::~PostprocessOverlay()
{
	glDeleteFramebuffers(1, &fbo);
	glMakeTextureHandleNonResidentARB(colTexHandle);
	glDeleteTextures(1, &colTex);
	glMakeTextureHandleNonResidentARB(depthStencilTexHandle);
	glDeleteTextures(1, &depthStencilTex);
	glDeleteTextures(1, &intermediateCol1Image);
	glDeleteTextures(1, &intermediateCol2Image);
}


//Run all passes of a single shader
void PostprocessOverlay::RunShader(PPEffect::PostprocessingEffect* ppe, bool& swap)
{
	int SCRWIDTH{ Application::getInstance().windowManager.get()->SCRWIDTH };
	int SCRHEIGHT{ Application::getInstance().windowManager.get()->SCRHEIGHT };

	std::size_t numPasses{ ppe->GetNumPasses() };
	
	for (std::size_t i{ 0 }; i < numPasses; ++i)
	{
		ppe->PrepareState(i);
		glDispatchCompute((SCRWIDTH + 31) / 32, (SCRHEIGHT + 31) / 32, 1);
		if (swap)
		{
			glBindImageTexture(0, intermediateCol2Image, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(1, intermediateCol1Image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
		}
		else
		{
			glBindImageTexture(0, intermediateCol1Image, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(1, intermediateCol2Image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
		}
		swap = !swap;
	}
}


void PostprocessOverlay::Render(unsigned int outputFbo)
{
	int SCRWIDTH{ Application::getInstance().windowManager.get()->SCRWIDTH };
	int SCRHEIGHT{ Application::getInstance().windowManager.get()->SCRHEIGHT };

	//Copy data from colTex to intermediateCol1Image
	//Bind intermediateCol1Image to image unit 0
	//Bind intermediateCol2Image to image unit 1
	//Initialise output tracker to track which image has the most recent output (initially 1)
	//For all postprocessing effects in activeShaders:
	//	Run all shader passes, swapping intermediateColImage 1 and 2 between passes
	//Draw framebuffer quad

	glCopyImageSubData(colTex, GL_TEXTURE_2D, 0, 0, 0, 0, intermediateCol1Image, GL_TEXTURE_2D, 0, 0, 0, 0, SCRWIDTH, SCRHEIGHT, 1);
	glBindImageTexture(0, intermediateCol1Image, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glBindImageTexture(1, intermediateCol2Image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	bool swap{ true };
	for (PPEffect::PostprocessingEffect* ppe : activeEffects)
	{
		RunShader(ppe, swap);
	}
	if (swap)
	{
		glBindImageTexture(0, intermediateCol2Image, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(1, intermediateCol1Image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	}
	else
	{
		glBindImageTexture(0, intermediateCol1Image, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(1, intermediateCol2Image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, outputFbo);
	glStencilMask(0xFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);
	fbQuad.Draw();
}

void PostprocessOverlay::OnScreenSizeChange(int scrwidth, int scrheight)
{
	//Recreate textures with new size
	glMakeTextureHandleNonResidentARB(colTexHandle);
	glDeleteTextures(1, &colTex);
	glMakeTextureHandleNonResidentARB(depthStencilTexHandle);
	glDeleteTextures(1, &depthStencilTex);
	glDeleteTextures(1, &intermediateCol1Image);
	glDeleteTextures(1, &intermediateCol2Image);

	//Store currently bound FBO for rebinding after
	int drawFbo, readFbo;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFbo);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFbo);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glCreateTextures(GL_TEXTURE_2D, 1, &colTex);
	glTextureStorage2D(colTex, 1, GL_RGBA8, scrwidth, scrheight);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colTex, 0);
	colTexHandle = glGetTextureHandleARB(colTex);
	glMakeTextureHandleResidentARB(colTexHandle);
	fboTextureHandles.input.x = colTexHandle;

	glCreateTextures(GL_TEXTURE_2D, 1, &depthStencilTex);
	glTextureStorage2D(depthStencilTex, 1, GL_DEPTH32F_STENCIL8, scrwidth, scrheight);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilTex, 0);
	depthStencilTexHandle = glGetTextureHandleARB(depthStencilTex);
	glMakeTextureHandleResidentARB(depthStencilTexHandle);
	fboTextureHandles.input.y = depthStencilTexHandle;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, readFbo);


	//Create intermediate colour buffer images for ping-pong buffering between multiple shader passes
	glCreateTextures(GL_TEXTURE_2D, 1, &intermediateCol1Image);
	glTextureStorage2D(intermediateCol1Image, 1, GL_RGBA8, scrwidth, scrheight);

	glCreateTextures(GL_TEXTURE_2D, 1, &intermediateCol2Image);
	glTextureStorage2D(intermediateCol2Image, 1, GL_RGBA8, scrwidth, scrheight);
}


void PostprocessOverlay::AddPostprocessingEffect(PPEffect::PostprocessingEffect* effect)
{
	if (std::find(activeEffects.begin(), activeEffects.end(), effect) != activeEffects.end())
	{
		//Effect already in vector
		std::cerr << "Attempted to add post-processing effect to overlay that was already using the effect." << std::endl;
		return;
	}
	activeEffects.push_back(effect);
}

void PostprocessOverlay::RemovePostprocessingEffect(PPEffect::PostprocessingEffect* effect)
{
	std::vector<PPEffect::PostprocessingEffect*>::iterator it{ std::find(activeEffects.begin(), activeEffects.end(), effect) };
	if (it == activeEffects.end())
	{
		//Effect not in map
		std::cerr << "Attempted to remove post-processing effect from overlay that wasn't using the effect." << std::endl;
		return;
	}
	activeEffects.erase(it);
}