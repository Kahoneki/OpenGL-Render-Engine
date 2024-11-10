#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "Application.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include "InputManager.h"
#include "WindowManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "AssetManager.h"
#include "Renderer.h"
#include "../Shaders/shader.h"

#include <iostream>


Application& Application::getInstance()
{
	static Application instance;
	return instance;
}

Application::Application()
{
	applicationRunning = true;

	//GLFW needs to be initialised before GLAD
	windowManager = std::make_shared<WindowManager>(this);

	InitialiseGLAD();
	InitialiseImGui(windowManager->GetWindow());

	inputManager = std::make_shared<InputManager>(this);
	timeManager = std::make_shared<TimeManager>(this);
	sceneManager = std::make_shared<SceneManager>(this);
	assetManager = std::make_shared<AssetManager>();
	renderer = std::make_shared<Renderer>(this);
}

Application::~Application() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void Application::RunFrame()
{
	timeManager->NewFrame();
	inputManager->ProcessInputs(windowManager->GetWindow(), timeManager->dt);
	renderer->Render(sceneManager->GetActiveScene());
	
	applicationRunning = !glfwWindowShouldClose(windowManager->GetWindow());
}


void Application::InitialiseGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialise GLAD function loader." << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glViewport(0, 0, windowManager->SCRWIDTH, windowManager->SCRHEIGHT);
	glClearColor(0.8f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

void Application::InitialiseImGui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io{ ImGui::GetIO() };
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}