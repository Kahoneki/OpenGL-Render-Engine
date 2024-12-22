#include "WindowManager.h"
#include "Application.h"
#include "InputManager.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_iconify_callback(GLFWwindow* window, int iconified);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

WindowManager::WindowManager(Application* _app)
{
	app = _app;

	SCRWIDTH = 1280;
	SCRHEIGHT = 720;
	fullscreen = false;
	minimised = false;

	window = InitialiseGLFW();
	glfwMakeContextCurrent(window);
}

void WindowManager::UpdateFullscreen()
{
	if (fullscreen) {
		//Change to fullscreen mode

		//Store current window size and position
		glfwGetWindowPos(window, &windowedXPos, &windowedYPos);
		glfwGetWindowSize(window, &SCRWIDTH, &SCRHEIGHT);

		//Get primary monitor and video mode
		GLFWmonitor* monitor{ glfwGetPrimaryMonitor() };
		const GLFWvidmode* mode{ glfwGetVideoMode(monitor) };

		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		glfwMakeContextCurrent(window);
	}
	else {
		//Change to windowed mode
		glfwSetWindowMonitor(window, NULL, windowedXPos, windowedYPos, SCRWIDTH, SCRHEIGHT, 0);
	}
}

//GLFWwindow* WindowManager::GetWindow()
//{
//	return window;
//}


GLFWwindow* WindowManager::InitialiseGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	GLFWwindow* window{ glfwCreateWindow(SCRWIDTH, SCRHEIGHT, "Neki", NULL, NULL)};
	if (window == NULL) {
		std::cerr << "Failed to initialise GLFW." << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);
	glfwSetWindowIconifyCallback(window, &framebuffer_iconify_callback);
	glfwSetCursorPosCallback(window, &mouse_callback);
	glfwSetScrollCallback(window, &scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}

void WindowManager::framebuffer_iconify_callback_impl(GLFWwindow* window, int iconified)
{
	minimised = iconified;
	if (!minimised)
	{
		glfwFocusWindow(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void WindowManager::framebuffer_size_callback_impl(GLFWwindow* window, int width, int height)
{
	// if (width == 0 || height == 0) {
	// 	minimised = true;
	// 	return;
	// }
	// else {
	// 	minimised = false;
	// }
	glViewport(0, 0, width, height);
	SCRWIDTH = width;
	SCRHEIGHT = height;
}

void WindowManager::mouse_callback_impl(GLFWwindow* window, double xposIn, double yposIn) const
{
	app->inputManager->ProcessMouse(xposIn, yposIn);
}

void WindowManager::scroll_callback_impl(GLFWwindow* window, double xOffset, double yOffset) const
{
	app->inputManager->ProcessScroll(yOffset);
}

bool WindowManager::getMinimised()
{
	return minimised;
}


void framebuffer_iconify_callback(GLFWwindow* window, int iconified)
{
	WindowManager* wm = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	wm->framebuffer_iconify_callback_impl(window, iconified);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WindowManager* wm = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	wm->framebuffer_size_callback_impl(window, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	WindowManager* wm = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	wm->mouse_callback_impl(window, xposIn, yposIn);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	WindowManager* wm = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	wm->scroll_callback_impl(window, xOffset, yOffset);
}