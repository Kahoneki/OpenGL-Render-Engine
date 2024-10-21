#include "TimeManager.h"
#include "Application.h"
#include "GLFW/glfw3.h"

TimeManager::TimeManager(Application* _app)
{
	app = _app;
}

void TimeManager::NewFrame()
{
	double currentTime{ glfwGetTime() };
	dt = currentTime - lastTime;
	lastTime = currentTime;
}