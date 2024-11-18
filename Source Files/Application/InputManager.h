#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Application.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "GLFW/glfw3.h"
#include "../Camera/PlayerCamera.h"
#include "../World/Scene.h"
#include <iostream>

class Application;
class GLFWwindow;

class InputManager
{
public:
	InputManager(Application* _app);

	//TODO: define this in InputManager.cpp and fix linker error
	inline void ProcessInputs(GLFWwindow* window, float dt) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (!app->editorMode && dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessKeyboard(FORWARD, app->timeManager->dt);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessKeyboard(BACKWARD, app->timeManager->dt);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessKeyboard(LEFT, app->timeManager->dt);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessKeyboard(RIGHT, app->timeManager->dt);
		}
		if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !f3Pressed) {
			f3Pressed = true;
			app->editorMode = !app->editorMode;
			glfwSetInputMode(window, GLFW_CURSOR, app->editorMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		}
		if (glfwGetKey(window, GLFW_KEY_F3) != GLFW_PRESS && f3Pressed)
			f3Pressed = false;
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			flip = false;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			flip = true;
	}
	void ProcessMouse(double xposIn, double yposIn);
	void ProcessScroll(double yOffset);

	bool flip;
	bool firstMouse;

private:
	Application* app;

	float lastX;
	float lastY;
	bool f3Pressed;
};

#endif