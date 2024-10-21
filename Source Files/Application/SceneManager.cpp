#include "SceneManager.h"
#include "Application.h"
#include "../World/Scene.h"

SceneManager::SceneManager(Application* _app)
{
	app = _app;

	scenes = std::vector<Scene*>();

	activeScene = -1;
}

Scene* SceneManager::GetActiveScene()
{
	return (scenes.size() == 0) ? nullptr : scenes[activeScene];
}


std::size_t SceneManager::AddScene(Scene* scene)
{
	scenes.push_back(scene);
	return scenes.size();
}

void SceneManager::RemoveScene(std::size_t index)
{
	scenes.erase(scenes.begin() + index);
}

void SceneManager::SetActiveScene(std::size_t index)
{
	activeScene = index;
}