#include <iostream>
#include "Source Files/Application/Application.h"
#include "Source Files/Application/Renderer.h"
#include "Source Files/Application/SceneManager.h"
#include "Source Files/Application/AssetManager.h"
#include "Source Files/Camera/PlayerCamera.h"
#include "Source Files/World/Drawable/Cube24.h"
#include "Source Files/World/Scene.h"
#include "Source Files/Shaders/shader.h"

int main()
{
	Application& app{ Application::getInstance() };

	//Define renderer
	Shader shader{ SHADER_PRESET::STATIC_MATERIAL };
	app.renderer->AddShader(&shader);
	app.renderer->SetActiveShader(0);

	//Define scene
	PlayerCamera cam{ "cam" };
	
	Cube24 cube{ "cube", glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(1.0f), glm::vec3(0.0f) };
	cube.material.setColour(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	cube.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	unsigned int kevinTextureName{ app.assetManager.get()->addTexture("kevin.png") };
	cube.material.setTextureName(kevinTextureName);
	cube.material.SetPropertyActive(MATERIAL_TEXTURE_BIT, true);
	cube.setPosition(cube.getPosition() + glm::vec3(-2, 0, 0));

	Cube24 cube2{ "cube", glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(1.0f), glm::vec3(0.0f) };
	cube2.material.setColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	cube2.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	unsigned int garfieldTextureName{ app.assetManager.get()->addTexture("garfield.png") };
	cube2.material.setTextureName(garfieldTextureName);
	cube2.material.SetPropertyActive(MATERIAL_TEXTURE_BIT, true);
	cube2.setPosition(cube2.getPosition() + glm::vec3(2, 0, 0));

	Scene scene;
	scene.AddRenderSource(&cam);
	scene.SetActiveRenderSource(0);
	scene.AddDrawable(&cube);
	scene.AddDrawable(&cube2);

	app.sceneManager->AddScene(&scene);
	app.sceneManager->SetActiveScene(0);

	while (app.applicationRunning) {
		app.RunFrame();
	}
}