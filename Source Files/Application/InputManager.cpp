#include "InputManager.h"
#include "Application.h"
#include "SceneManager.h"
#include "TimeManager.h"
//#include "GLFW/glfw3.h"
#include "../Camera/PlayerCamera.h"
#include "../World/Scene.h"

InputManager::InputManager(Application* _app)
	: app(_app), firstMouse(true), lastX(0.0f), lastY(0.0f), f3Pressed(false), flip(false)
{
}

//void InputManager::ProcessInputs(GLFWwindow* window, float dt)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//
//	if (!app->editorMode && dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())) {
//		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//			dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessKeyboard(FORWARD, app->timeManager->dt);
//		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//			dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessKeyboard(BACKWARD, app->timeManager->dt);
//		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//			dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessKeyboard(LEFT, app->timeManager->dt);
//		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//			dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessKeyboard(RIGHT, app->timeManager->dt);
//	}
//	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !f3Pressed) {
//		f3Pressed = true;
//		app->editorMode = !app->editorMode;
//		glfwSetInputMode(window, GLFW_CURSOR, app->editorMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
//	}
//	if (glfwGetKey(window, GLFW_KEY_F3) != GLFW_PRESS && f3Pressed)
//		f3Pressed = false;
//}

void InputManager::ProcessMouse(double xposIn, double yposIn)
{
	if (app->editorMode) {
		firstMouse = true;
	}

	if (!app->editorMode && dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())) {
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; //Reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessMouseMovement(xoffset, yoffset);
	}
}

void InputManager::ProcessScroll(double yOffset)
{
	if (!app->editorMode && dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())) {
		dynamic_cast<PlayerCamera*>(app->sceneManager->GetActiveScene()->GetActiveRenderSource())->ProcessMouseScroll(static_cast<float>(yOffset));
	}
}