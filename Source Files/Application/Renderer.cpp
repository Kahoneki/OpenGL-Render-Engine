#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Application.h"
#include "SceneManager.h"
#include "WindowManager.h"

#include "../World/Scene.h"
#include "../World/Drawable/Drawable.h"
#include "../Camera/RenderSource.h"
#include "../Shaders/shader.h"
#include "../World/LightSource.h"
#include "../Camera/camera.h"
#include "../Camera/PlayerCamera.h"
#include <iostream>

#include <GLAD/glad.h>
#include "GLFW/glfw3.h"

Renderer::Renderer(Application* _app)
{
	app = _app;

	shaders = std::vector<Shader*>();

	//Purposefully leave activeShader uninitialised.
}

Renderer::Renderer(Shader* shader)
{
	shaders.push_back(shader);

	//Purposefully leave activeShader uninitialised.
}

Renderer::Renderer(std::vector<Shader*> _shaders)
{
	shaders = _shaders;

	//Purposefully leave activeShader uninitialised.
}

Renderer::~Renderer()
{

}


void Renderer::Render(Scene* scene) const {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaders[activeShader]->use();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Set view and projection matrices
	RenderSource* rs{ app->sceneManager->GetActiveScene()->GetActiveRenderSource() };
	Camera* c{ dynamic_cast<Camera*>(rs) };
	if (c) {
		shaders[activeShader]->setMat4("view", dynamic_cast<const Camera*>(rs)->GetViewMatrix());
		PlayerCamera* pc{ dynamic_cast<PlayerCamera*>(rs) };
		if (pc) {
			shaders[activeShader]->setMat4("projection", glm::perspective(glm::radians(pc->Fov), static_cast<float>(app->windowManager->SCRWIDTH) / static_cast<float>(app->windowManager->SCRWIDTH), pc->nearPlaneDistance, pc->farPlaneDistance));
		}
		else {
			shaders[activeShader]->setMat4("projection", glm::perspective(glm::radians(90.0f), static_cast<float>(app->windowManager->SCRWIDTH) / static_cast<float>(app->windowManager->SCRWIDTH), c->nearPlaneDistance, c->farPlaneDistance));
		}
	}

	//Attach scene's light source buffer to the context's UBO
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, app->sceneManager->GetActiveScene()->lightSourcesBuffer);
	

	//Loop through all drawables in the active scene and call draw function
	for (Drawable* drawable : app->sceneManager->GetActiveScene()->drawables) {
		drawable->Draw(*shaders[activeShader]);
	}

	RenderImGui();
	glfwSwapBuffers(app->windowManager->GetWindow());
	glfwPollEvents();
}



std::size_t Renderer::AddShader(Shader* shader)
{
	shaders.push_back(shader);
	return shaders.size();
}

void Renderer::RemoveShader(std::size_t index)
{
	shaders.erase(shaders.begin() + index);
}

void Renderer::SetActiveShader(std::size_t index)
{
	activeShader = index;
}

Shader* Renderer::GetActiveShader()
{
	return shaders[activeShader];
}

Shader* Renderer::GetShader(std::size_t index)
{
	return shaders[index];
}



void Renderer::RenderImGui() const
{
	if (app->editorMode) {
		ImGui::Begin("Template");
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

