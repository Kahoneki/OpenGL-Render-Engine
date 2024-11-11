#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Application.h"
#include "SceneManager.h"
#include "WindowManager.h"

#include "../Utility/BindingPoints.h"

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
	lightSourceShader = new Shader(SHADER_PRESET::LIGHT_SOURCE);

	//Purposefully leave activeShader uninitialised.
}

Renderer::Renderer(Shader* shader)
{
	shaders.push_back(shader);
	lightSourceShader = new Shader(SHADER_PRESET::LIGHT_SOURCE);

	//Purposefully leave activeShader uninitialised.
}

Renderer::Renderer(std::vector<Shader*> _shaders)
{
	shaders = _shaders;
	lightSourceShader = new Shader(SHADER_PRESET::LIGHT_SOURCE);

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
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::LIGHT_SOURCES, app->sceneManager->GetActiveScene()->lightSourcesBuffer);
	shaders[activeShader]->setInt("numLights", app->sceneManager->GetActiveScene()->lightSources.size());

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


void DrawHeirarchy(SceneObject* s)
{
	if (ImGui::TreeNode(s->name)) {

		//SceneObject can either be a LightSource, RenderSource, or Drawawble - handle each case separately

		if (dynamic_cast<LightSource*>(s)) {
			LightSource* ls{ dynamic_cast<LightSource*>(s) };
			
			glm::vec3 pos{ ls->getPosition() };
			if (ImGui::DragFloat3("Position", &pos.x)) {
				ls->setPosition(pos);
			}
			glm::vec3 scale{ ls->cube.getScale() };
			if (ImGui::DragFloat3("Scale", &scale.x)) {
				ls->cube.setScale(scale);
			}
			glm::vec3 rotation{ ls->cube.getRotation() };
			if (ImGui::DragFloat3("Rotation", &rotation.x)) {
				ls->cube.setRotation(rotation);
			}

			glm::vec3 amb{ ls->getAmbientColour() };
			if (ImGui::ColorPicker3("Ambient Colour", &amb.x)) {
				ls->setAmbientColour(glm::vec4(amb, 0.0f));
			}
			glm::vec3 dif{ ls->getDiffuseColour() };
			if (ImGui::ColorPicker3("Diffuse Colour", &dif.x)) {
				ls->setDiffuseColour(glm::vec4(dif, 0.0f));
			}
			glm::vec3 spec{ ls->getSpecularColour() };
			if (ImGui::ColorPicker3("Specular Colour", &spec.x)) {
				ls->setSpecularColour(glm::vec4(spec, 0.0f));
			}
		}


		if (dynamic_cast<RenderSource*>(s)) {
			RenderSource* rs{ dynamic_cast<RenderSource*>(s) };

			if (dynamic_cast<Camera*>(rs)) {
				Camera* c{ dynamic_cast<Camera*>(rs) };
				glm::vec3 pos{ c->getPosition() };
				if (ImGui::DragFloat3("Position", &pos.x)) {
					c->setPosition(pos);
				}
				glm::vec2 rotation{ c->Pitch, c->Yaw};
				if (ImGui::DragFloat2("Rotation", &rotation.x)) {
					c->Pitch = rotation.x;
					c->Yaw = rotation.y;
					c->updateCameraVectors();
				}
				ImGui::DragFloat("Near Plane Distance", &c->nearPlaneDistance);
				ImGui::DragFloat("Far Plane Distance", &c->farPlaneDistance);
			
				if (dynamic_cast<PlayerCamera*>(c)) {
					PlayerCamera* pc{ dynamic_cast<PlayerCamera*>(c) };
					ImGui::SliderFloat("FOV", &pc->Fov, 1.0f, 140.0f);
				}
			}
		}


		if (dynamic_cast<Drawable*>(s)) {
			Drawable* d{ dynamic_cast<Drawable*>(s) };

			glm::vec3 pos{ d->getPosition() };
			if (ImGui::DragFloat3("Position", &pos.x)) {
				d->setPosition(pos);
			}
			glm::vec3 scale{ d->getScale() };
			if (ImGui::DragFloat3("Scale", &scale.x)) {
				d->setScale(scale);
			}
			glm::vec3 rotation{ d->getRotation() };
			if (ImGui::DragFloat3("Rotation", &rotation.x)) {
				d->setRotation(rotation);
			}

			glm::vec3 amb{ d->material.getAmbientColour() };
			if (ImGui::ColorPicker3("Ambient Colour", &amb.x)) {
				d->material.setAmbientColour(glm::vec4(amb, 0.0f));
			}
			glm::vec3 dif{ d->material.getDiffuseColour() };
			if (ImGui::ColorPicker3("Diffuse Colour", &dif.x)) {
				d->material.setDiffuseColour(glm::vec4(dif, 0.0f));
			}
			glm::vec3 spec{ d->material.getSpecularColour() };
			if (ImGui::ColorPicker3("Specular Colour", &spec.x)) {
				d->material.setSpecularColour(glm::vec4(spec, 0.0f));
			}
			float specPow{ d->material.getSpecularPower() };
			if (ImGui::SliderFloat("Specular Power", &specPow, 0.0f, 256.0f)) {
				d->material.setSpecularPower(specPow);
			}
		}

		if (s->child != nullptr) {
			DrawHeirarchy(s->child);
		}

		ImGui::TreePop();
	}
}


void Renderer::RenderImGui() const
{
	if (app->editorMode) {
		ImGui::Begin("Editor");

		for (SceneObject* s : app->sceneManager->GetActiveScene()->sceneObjects) {
			if (s->parent == nullptr) {
				//Drawable is either top of a heirarchy or not part of a heirarchy
				DrawHeirarchy(s);
			}
		}

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

