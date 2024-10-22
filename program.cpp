#include <iostream>
#include "Source Files/Application/Application.h"
#include "Source Files/Application/Renderer.h"
#include "Source Files/Application/SceneManager.h"
#include "Source Files/Camera/PlayerCamera.h"
#include "Source Files/World/Drawable/Cube8.h"
#include "Source Files/World/Scene.h"
#include "Source Files/Shaders/shader.h"


int main()
{
	Application app;

	//Define scene
	PlayerCamera cam{ "cam" };
	Cube8 cube{ "cube", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f) };
	cube.material.materialData.colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	cube.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	cube.UpdateMaterial();
	cube.setPosition(cube.getPosition() + glm::vec3(0, 0, -5));
	Scene scene;
	scene.AddRenderSource(&cam);
	scene.SetActiveRenderSource(0);
	scene.AddDrawable(&cube);

	//Define renderer
	Shader shader{ SHADER_PRESET::STATIC_MATERIAL };
	app.renderer->AddShader(&shader);
	app.renderer->SetActiveShader(0);
	app.sceneManager->AddScene(&scene);
	app.sceneManager->SetActiveScene(0);

	while (app.applicationRunning) {
		app.RunFrame();
	}
}