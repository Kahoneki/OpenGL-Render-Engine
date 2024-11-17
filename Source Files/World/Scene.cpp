#include "Scene.h"
#include <GLAD/glad.h>
#include "LightSource.h"
#include "../Utility/BindingPoints.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
#include "../Camera/camera.h"

Scene::Scene()
{
	lightSources = std::vector<LightSource*>();
	renderSources = std::vector<RenderSource*>();
	drawables = std::vector<Drawable*>();
	sceneObjects = std::vector<SceneObject*>();
	glCreateBuffers(1, &lightSourcesBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::LIGHT_SOURCES, lightSourcesBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * 512, nullptr, GL_DYNAMIC_DRAW);
	automaticLightUpdates = true;
	//Purposefully leave activeRenderSource uninitialised
}

Scene::Scene(std::vector<LightSource*> _lightSources, std::vector<RenderSource*> _renderSources, std::vector<Drawable*> _drawables)
{
	lightSources = _lightSources;
	renderSources = _renderSources;
	drawables = _drawables;
	sceneObjects = std::vector<SceneObject*>();

	for (LightSource* ls : lightSources) {
		sceneObjects.push_back(ls);
	}
	for (RenderSource* rs : renderSources) {
		sceneObjects.push_back(rs);
	}
	for (Drawable* d : drawables) {
		sceneObjects.push_back(d);
	}

	automaticLightUpdates = true;
	lightsChanged = true;

	//Purposefully leave activeRenderSource uninitialised
	
	glCreateBuffers(1, &lightSourcesBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::LIGHT_SOURCES, lightSourcesBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * 512, nullptr, GL_DYNAMIC_DRAW);
	for (LightSource* ls : lightSources) {
		ls->sceneParent = this;
		drawables.push_back(&ls->cube);
	}

}

Scene::~Scene()
{
	glDeleteBuffers(1, &lightSourcesBuffer);
}

void Scene::SetAutomaticLightUpdates(bool val)
{
	automaticLightUpdates = val;
}

bool Scene::GetAutomaticLightUpdates()
{
	return automaticLightUpdates;
}

std::size_t Scene::AddLightSource(LightSource* lightSource)
{
	lightSources.push_back(lightSource);
	lightSource->sceneParent = this;
	drawables.push_back(&lightSource->cube);
	sceneObjects.push_back(lightSource);
	sceneObjects.push_back(&lightSource->cube);
	lightsChanged = true;

	return lightSources.size();
}

void Scene::RemoveLightSource(std::size_t index)
{
	for (std::size_t i{ 0 }; i < sceneObjects.size(); ++i) {
		if (dynamic_cast<LightSource*>(sceneObjects[i])) {
			if (dynamic_cast<LightSource*>(sceneObjects[i]) == lightSources[index]) {
				sceneObjects.erase(sceneObjects.begin() + i);
			}
		}
	}
	lightSources.erase(lightSources.begin() + index);

	lightsChanged = true;
}

void Scene::UpdateLightSources()
{
	std::vector<Light> lightData;
	for (LightSource* ls : lightSources) {
		lightData.push_back(ls->light);
		ls->cube.material.setDiffuseColour(ls->getDiffuseColour());
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT::LIGHT_SOURCES, lightSourcesBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Light) * lightSources.size(), lightData.data()); //Dynamic draw as this function will be called everytime a light source is added, removed, or modified

	lightsChanged = false;
}

std::size_t Scene::AddRenderSource(RenderSource* renderSource)
{
	renderSources.push_back(renderSource);
	sceneObjects.push_back(renderSource);
	return renderSources.size();
}

void Scene::RemoveRenderSource(std::size_t index)
{
	for (std::size_t i{ 0 }; i < sceneObjects.size(); ++i) {
		if (dynamic_cast<RenderSource*>(sceneObjects[i])) {
			if (dynamic_cast<RenderSource*>(sceneObjects[i]) == renderSources[index]) {
				sceneObjects.erase(sceneObjects.begin() + i);
			}
		}
	}
	renderSources.erase(renderSources.begin() + index);
}

void Scene::SetActiveRenderSource(std::size_t index)
{
	activeRenderSource = index;
}

RenderSource* Scene::GetActiveRenderSource()
{
	return (renderSources.size() == 0) ? nullptr : renderSources[activeRenderSource];
}

std::size_t Scene::AddDrawable(Drawable* drawable)
{
	drawables.push_back(drawable);
	sceneObjects.push_back(drawable);
	return drawables.size();
}

void Scene::RemoveDrawable(std::size_t index)
{
	for (std::size_t i{ 0 }; i < sceneObjects.size(); ++i) {
		if (dynamic_cast<Drawable*>(sceneObjects[i])) {
			if (dynamic_cast<Drawable*>(sceneObjects[i]) == drawables[index]) {
				sceneObjects.erase(sceneObjects.begin() + i);
			}
		}
	}
	drawables.erase(drawables.begin() + index);
}
