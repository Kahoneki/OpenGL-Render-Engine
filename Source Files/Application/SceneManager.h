#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>

class Application;
class Scene;

class SceneManager
{
public:
	SceneManager(Application* _app);
	SceneManager(Application* _app, const Scene* scene);


	std::size_t AddScene(Scene* scene); //Returns index
	void RemoveScene(std::size_t index);
	void SetActiveScene(std::size_t index);
	Scene* GetActiveScene();

private:
	Application* app;

	std::vector<Scene*> scenes;
	std::size_t activeScene; //Index
};

#endif