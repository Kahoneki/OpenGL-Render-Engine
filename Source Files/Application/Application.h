#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include <GLAD/glad.h>

class InputManager;
class WindowManager;
class TimeManager;
class SceneManager;
class AssetManager;
class Renderer;
class GLFWwindow;

class Application
{
	friend class InputManager;
	friend class WindowManager;
	friend class TimeManager;
	friend class SceneManager;
	friend class AssetManager;
	friend class Renderer;

public:

	static Application& getInstance();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	~Application();
	void RunFrame();

	bool applicationRunning;

	std::shared_ptr<InputManager> inputManager;
	std::shared_ptr<WindowManager> windowManager;
	std::shared_ptr<TimeManager> timeManager;
	std::shared_ptr<SceneManager> sceneManager;
	std::shared_ptr<AssetManager> assetManager;
	std::shared_ptr<Renderer> renderer;

private:
	Application();

	void InitialiseGLAD();
	void InitialiseImGui(GLFWwindow* window);

	bool editorMode;
};

#endif