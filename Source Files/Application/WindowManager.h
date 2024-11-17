#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

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

	void framebuffer_size_callback_impl(GLFWwindow* window, int width, int height);
	void mouse_callback_impl(GLFWwindow* window, double xposIn, double yposIn);
	void scroll_callback_impl(GLFWwindow* window, double xOffset, double yOffset);

private:
	Application* app;

	GLFWwindow* window;

	[[nodiscard]] GLFWwindow* InitialiseGLFW();
	void UpdateFullscreen();
	bool fullscreen;
	int windowedXPos;
	int windowedYPos;
};

#endif