#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <vector>
#include <functional>

class Application;
class GLFWwindow;

class WindowManager
{

	friend class Renderer;

public:
	WindowManager(Application* _app);

	//TODO: define this in WindowManager.cpp and fix linker error
	inline GLFWwindow* GetWindow() { return window; }

	int SCRWIDTH;
	int SCRHEIGHT;

	void framebuffer_iconify_callback_impl(GLFWwindow* window, int iconified);
	void framebuffer_size_callback_impl(GLFWwindow* window, int width, int height);
	void mouse_callback_impl(GLFWwindow* window, double xposIn, double yposIn) const;
	void scroll_callback_impl(GLFWwindow* window, double xOffset, double yOffset) const;

	bool getMinimised();

	void JoinScreenSizeChangeCallbacksList(std::function<void(int scrwidth, int scrheight)> callback);
	
private:
	Application* app;

	GLFWwindow* window;

	[[nodiscard]] GLFWwindow* InitialiseGLFW();
	
	void UpdateFullscreen();
	bool fullscreen;
	//For restoring state after returning from fullscreen
	int windowedXPos;
	int windowedYPos;
	int windowedSCRWIDTH;
	int windowedSCRHEIGHT;

	bool minimised;

	//Functions to be called by WindowManager when the screen size changes
	//This is for things that rely on the screen size - i.e.: PostprocessOverlay
	std::vector<std::function<void(int scrwidth, int scrheight)>> screenSizeChangeCallbacks;
	void NotifyScreenSizeChangeListeners();
};

#endif