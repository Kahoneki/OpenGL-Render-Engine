#ifndef SCENE_H
#define SCENE_H

#include "SceneObject.h"

#include <vector>

class LightSource;
class RenderSource;
class Drawable;

/*Scene class - holds all the `SceneObject`s*/

class Scene
{

	friend class Renderer;

public:
	Scene();
	Scene(std::vector<LightSource*> _lightSources, std::vector<RenderSource*> _renderSources, std::vector<Drawable*> _drawables);
	~Scene();

	std::size_t AddLightSource(LightSource* lightSource); //Returns index
	void RemoveLightSource(std::size_t index);
	void UpdateLightSources(); //Must be called after LightSource::setPosition(), LightSource::setColour() or LightSource::setIntensity()

	std::size_t AddRenderSource(RenderSource* renderSource); //Returns index
	void RemoveRenderSource(std::size_t index);
	void SetActiveRenderSource(std::size_t index);
	RenderSource* GetActiveRenderSource();

	std::size_t AddDrawable(Drawable* drawable); //Returns index
	void RemoveDrawable(std::size_t index);


private:
	std::vector<LightSource*> lightSources; //Can't be pointers as they need to be passed to the GPU
	unsigned int lightSourcesBuffer;

	std::vector<RenderSource*> renderSources; //E.g.: cameras
	std::size_t activeRenderSource;

	std::vector<Drawable*> drawables;

	std::vector<SceneObject*> sceneObjects; //Contains all light sources, render sources, and drawables
};

#endif