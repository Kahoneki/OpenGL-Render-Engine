#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Application.h"
#include "SceneManager.h"
#include "WindowManager.h"
#include "TimeManager.h"

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

	for (bool& d : dropdownStatuses) { d = false; }

	clearColour = glm::vec4(0.0f);

	//Purposefully leave activeShader uninitialised.
}

Renderer::Renderer(Shader* shader)
{
	shaders.push_back(shader);
	lightSourceShader = new Shader(SHADER_PRESET::LIGHT_SOURCE);

	for (bool& d : dropdownStatuses) { d = false; }

	clearColour = glm::vec4(0.0f);

	//Purposefully leave activeShader uninitialised.
}

Renderer::Renderer(std::vector<Shader*> _shaders)
{
	shaders = _shaders;
	lightSourceShader = new Shader(SHADER_PRESET::LIGHT_SOURCE);

	for (bool& d : dropdownStatuses) { d = false; }

	clearColour = glm::vec4(0.0f);

	//Purposefully leave activeShader uninitialised.
}

Renderer::~Renderer()
{

}


void Renderer::Render(Scene* scene) {

	RenderSource* rs{ app->sceneManager->GetActiveScene()->GetActiveRenderSource() };
	if (rs->postprocessOverlay.activeShaders.size() > 0) {
		//There are postprocessing effects active, bind the offscreen framebuffer for deferred rendering
		glBindFramebuffer(GL_FRAMEBUFFER, rs->postprocessOverlay.fbo);
	}

	glStencilMask(0xFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);
	shaders[activeShader]->use();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Set view and projection matrices
	Camera* c{ dynamic_cast<Camera*>(rs) };
	if (c) {
		//shaders[activeShader]->setMat4("view", dynamic_cast<const Camera*>(rs)->GetViewMatrix());
		PlayerCamera* pc{ dynamic_cast<PlayerCamera*>(rs) };
		if (pc) {
			shaders[activeShader]->setMat4("projection", glm::perspective(glm::radians(pc->Fov), static_cast<float>(app->windowManager->SCRWIDTH) / static_cast<float>(app->windowManager->SCRWIDTH), pc->nearPlaneDistance, pc->farPlaneDistance));
		}
		else {
			shaders[activeShader]->setMat4("projection", glm::perspective(glm::radians(90.0f), static_cast<float>(app->windowManager->SCRWIDTH) / static_cast<float>(app->windowManager->SCRWIDTH), c->nearPlaneDistance, c->farPlaneDistance));
		}
	}

	//Attach scene's light source buffer to the context's UBO
	if (app->sceneManager.get()->GetActiveScene()->lightsChanged && app->sceneManager.get()->GetActiveScene()->automaticLightUpdates) {
		app->sceneManager.get()->GetActiveScene()->UpdateLightSources();
	}
	glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::LIGHT_SOURCES, app->sceneManager->GetActiveScene()->lightSourcesBuffer);
	shaders[activeShader]->setInt("numLights", app->sceneManager->GetActiveScene()->lightSources.size());

	//Loop through all drawables in the active scene and call draw function - if drawable.renderOrder changes between items, place a memory barrier to maintain render order
	if (app->sceneManager->GetActiveScene()->drawables.size() > 0) {
		unsigned int previousRenderOrder{ app->sceneManager->GetActiveScene()->drawables[0]->getRenderOrder() };
		for (Drawable* drawable : app->sceneManager->GetActiveScene()->drawables) {
			if (drawable->getRenderOrder() != previousRenderOrder) {
				glMemoryBarrier(GL_ALL_BARRIER_BITS);
				previousRenderOrder = drawable->getRenderOrder();
			}
			drawable->Draw(*shaders[activeShader]);
		}
	}


	if (rs->postprocessOverlay.activeShaders.size() > 0) {
		//There are postprocessing effects active, rebind the default framebuffer and render them
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);
		rs->postprocessOverlay.Render();
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

void Renderer::SetClearColour(glm::vec4 colour)
{
	clearColour = colour;
	glClearColor(colour.r, colour.g, colour.b, colour.a);
}

glm::vec4 Renderer::GetClearColour()
{
	return clearColour;
}


void DrawHeirarchy(SceneObject* s)
{
	if (ImGui::TreeNode(s->name)) {

		//SceneObject can either be a LightSource, RenderSource, or Drawawble - handle each case separately

		if (dynamic_cast<LightSource*>(s)) {
			LightSource* ls{ dynamic_cast<LightSource*>(s) };
			
			if (ImGui::TreeNode("Transform")) {

				glm::vec3 pos{ ls->getPosition() };
				if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) {
					ls->setPosition(pos);
				}
				glm::vec3 scale{ ls->cube.getScale() };
				if (ImGui::DragFloat3("Scale", &scale.x, 0.1f)) {
					ls->cube.setScale(scale);
				}
				glm::vec3 rotation{ ls->cube.getRotation() };
				if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) {
					ls->cube.setRotation(rotation);
				}


				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Colour")) {

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

				ImGui::TreePop();
			}

			float intensity{ ls->getIntensity() };
			if (ImGui::DragFloat("Intensity", &intensity, 0.1f)) {
				ls->setIntensity(intensity);
			}
		}


		if (dynamic_cast<RenderSource*>(s)) {
			RenderSource* rs{ dynamic_cast<RenderSource*>(s) };

			if (dynamic_cast<Camera*>(rs)) {

				Camera* c{ dynamic_cast<Camera*>(rs) };
				if (ImGui::TreeNode("Transform")) {

					glm::vec3 pos{ c->getPosition() };
					if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) {
						c->setPosition(pos);
					}
					glm::vec3 rotation{ c->getRotation() };
					if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) {
						c->setRotation(rotation);
						c->updateCameraVectors();
					}

					ImGui::TreePop();
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

			if (ImGui::TreeNode("Transform")) {

				glm::vec3 pos{ d->getPosition() };
				if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) {
					d->setPosition(pos);
				}
				glm::vec3 scale{ d->getScale() };
				if (ImGui::DragFloat3("Scale", &scale.x, 0.1f)) {
					d->setScale(scale);
				}
				glm::vec3 rotation{ d->getRotation() };
				if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) {
					d->setRotation(rotation);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Colour")) {

					glm::vec3 amb{ d->material.getAmbientColour() };
					if (ImGui::ColorPicker3("Ambient", &amb.x)) {
						d->material.setAmbientColour(glm::vec4(amb, 0.0f));
					}
					glm::vec3 dif{ d->material.getDiffuseColour() };
					if (ImGui::ColorPicker3("Diffuse", &dif.x)) {
						d->material.setDiffuseColour(glm::vec4(dif, 0.0f));
					}
					glm::vec3 spec{ d->material.getSpecularColour() };
					if (ImGui::ColorPicker3("Specular", &spec.x)) {
						d->material.setSpecularColour(glm::vec4(spec, 0.0f));
					}
					float specPow{ d->material.getSpecularPower() };
					if (ImGui::SliderFloat("Specular Power", &specPow, 0.1f, 256.0f)) {
						d->material.setSpecularPower(specPow);
					}
					float rimPow{ d->material.getRimPower() };
					if (ImGui::SliderFloat("Rim Power", &rimPow, 0.0f, 5.0f)) {
						d->material.setRimPower(rimPow);
					}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Properties")) {
			
				bool colour = d->material.GetPropertyActive(MATERIAL_COLOUR_BIT);
				if (ImGui::Checkbox("Colour", &colour)) {
					d->material.SetPropertyActive(MATERIAL_COLOUR_BIT, colour);
				}

				bool albedo = d->material.GetPropertyActive(MATERIAL_ALBEDO_TEXTURE_BIT);
				if (ImGui::Checkbox("Albedo Texture", &albedo)) {
					d->material.SetPropertyActive(MATERIAL_ALBEDO_TEXTURE_BIT, albedo);
				}

				bool normal = d->material.GetPropertyActive(MATERIAL_NORMAL_TEXTURE_BIT);
				if (ImGui::Checkbox("Normal Texture", &normal)) {
					d->material.SetPropertyActive(MATERIAL_NORMAL_TEXTURE_BIT, normal);
				}

				ImGui::TreePop();
			}
		}

		for (SceneObject* child : s->children) {
			DrawHeirarchy(child);
		}

		ImGui::TreePop();
	}
}


void Renderer::RenderImGui()
{
	if (app->editorMode) {
		ImGui::Begin("Editor");

		ImGui::Text(std::string(std::string("FPS: ") + std::to_string(1.0 / app->timeManager.get()->dt)).c_str());

		if (ImGui::Checkbox("Fullscreen", &app->windowManager.get()->fullscreen)) {
			app->windowManager.get()->UpdateFullscreen();
		}

		bool automaticLightUpdates{ app->sceneManager->GetActiveScene()->GetAutomaticLightUpdates() };
		if (ImGui::Checkbox("Automatic Light Updates", &automaticLightUpdates)) {
			app->sceneManager->GetActiveScene()->SetAutomaticLightUpdates(automaticLightUpdates);
		}
		if (!automaticLightUpdates) {
			if (ImGui::Button("Update Light Sources") && app->sceneManager.get()->GetActiveScene()->lightsChanged) {
				app->sceneManager->GetActiveScene()->UpdateLightSources();
			}
		}

		if (ImGui::TreeNode("Background Colour")) {
			float colour[3] = { clearColour.r, clearColour.g, clearColour.b };
			if (ImGui::ColorPicker3("", colour)) {
				SetClearColour(glm::vec4(colour[0], colour[1], colour[2], 1.0f));
			}
			ImGui::TreePop();
		}

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