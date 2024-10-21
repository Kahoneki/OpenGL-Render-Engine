#include "Scene.h"
#include <GLAD/glad.h>
#include "LightSource.h"

Scene::Scene()
{
	lightSources = std::vector<LightSource>();
	renderSources = std::vector<RenderSource*>();
	drawables = std::vector<Drawable*>();
	//Purposefully leave activeRenderSource uninitialised
}

Scene::Scene(std::vector<LightSource> _lightSources, std::vector<RenderSource*> _renderSources, std::vector<Drawable*> _drawables)
{
	lightSources = _lightSources;
	renderSources = _renderSources;
	drawables = _drawables;

	//Purposefully leave activeRenderSource uninitialised
	
	glCreateBuffers(1, &lightSourcesBuffer);
	if (lightSources.size() > 0) {
		UpdateLightSources();
	}
}

Scene::~Scene()
{
	glDeleteBuffers(1, &lightSourcesBuffer);
}

std::size_t Scene::AddLightSource(LightSource lightSource)
{
	lightSources.push_back(lightSource);
	UpdateLightSources();
	return lightSources.size();
}

void Scene::RemoveLightSource(std::size_t index)
{
	lightSources.erase(lightSources.begin() + index);
	UpdateLightSources();
}

void Scene::UpdateLightSources()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightSourcesBuffer);
	glBufferData(GL_UNIFORM_BUFFER, lightSources.size() * sizeof(lightSources[0]), lightSources.data(), GL_DYNAMIC_DRAW); //Dynamic draw as this function will be called everytime a light source is added, removed, or modified
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::size_t Scene::AddRenderSource(RenderSource* renderSource)
{
	renderSources.push_back(renderSource);
	return renderSources.size();
}

void Scene::RemoveRenderSource(std::size_t index)
{
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
	return drawables.size();
}

void Scene::RemoveDrawable(std::size_t index)
{
	drawables.erase(drawables.begin() + index);
}
