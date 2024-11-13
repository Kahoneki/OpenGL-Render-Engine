#include <iostream>
#include "Source Files/Application/Application.h"
#include "Source Files/Application/Renderer.h"
#include "Source Files/Application/SceneManager.h"
#include "Source Files/Application/AssetManager.h"
#include "Source Files/Camera/PlayerCamera.h"
#include "Source Files/World/Drawable/Cube24.h"
#include "Source Files/World/Scene.h"
#include "Source Files/World/LightSource.h"
#include "Source Files/Shaders/shader.h"

int main()
{
	Application& app{ Application::getInstance() };

	//Define renderer
	Shader shader{ SHADER_PRESET::BLINN_PHONG };
	app.renderer->AddShader(&shader);
	app.renderer->SetActiveShader(0);

	//Define scene
	PlayerCamera cam{ "Camera", nullptr };
	
	Cube24 cube{ "orange cube", glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f) };
	cube.material.setAmbientColour(glm::vec4(0.2f, 0.2f, 0.1f, 1.0f)); // Ambient: warm subtle light
	cube.material.setDiffuseColour(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)); // Diffuse: warm orange color
	cube.material.setSpecularColour(glm::vec4(0.8f, 0.6f, 0.3f, 1.0f)); // Specular: slightly muted warm shine
	cube.material.setSpecularPower(64.0f); // Specular strength
	cube.material.setRimPower(64.0f);
	cube.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);

	unsigned int kevinTextureName{ app.assetManager.get()->addTexture("kevin.png") };
	cube.material.setAlbedoTextureName(kevinTextureName);
	cube.material.SetPropertyActive(MATERIAL_ALBEDO_TEXTURE_BIT, true);

	// Position set to the right and slightly raised
	cube.setPosition(cube.getPosition() + glm::vec3(2.0f, 1.0f, 0.0f));


	Cube24 cube2{ "Brick Cube", glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f) };
	//cube2.material.setAmbientColour(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f)); // Ambient: cool subtle light
	//cube2.material.setDiffuseColour(glm::vec4(0.2f, 0.3f, 1.0f, 1.0f)); // Diffuse: cool blue
	//cube2.material.setSpecularColour(glm::vec4(0.5f, 0.6f, 1.0f, 1.0f)); // Specular: blue, shiny
	cube2.material.setAmbientColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	cube2.material.setDiffuseColour(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
	cube2.material.setSpecularColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	cube2.material.setSpecularPower(128.0f); // Specular strength
	cube2.material.setRimPower(128.0f);
	cube2.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);

	unsigned int brickAlbedoName{ app.assetManager.get()->addTexture("brickwall.jpg") };
	unsigned int brickNormalName{ app.assetManager.get()->addTexture("brickwall_normal.jpg") };
	cube2.material.setAlbedoTextureName(brickAlbedoName);
	cube2.material.setNormalTextureName(brickNormalName);
	cube2.material.SetPropertyActive(MATERIAL_ALBEDO_TEXTURE_BIT, true);
	cube2.material.SetPropertyActive(MATERIAL_NORMAL_TEXTURE_BIT, true);

	// Position set to the left and slightly raised
	cube2.setPosition(cube2.getPosition() + glm::vec3(-2.0f, 1.0f, 0.0f));


	Scene scene;
	scene.AddRenderSource(&cam);
	scene.SetActiveRenderSource(0);
	//scene.AddDrawable(&cube);
	scene.AddDrawable(&cube2);

	// Warm yellow light
	LightSource light{
		"yellow light",
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec4(0.2f, 0.2f, 0.1f, 1.0f),  // Ambient: subtle warm
		glm::vec4(1.0f, 0.9f, 0.6f, 1.0f),  // Diffuse: warm yellow
		glm::vec4(1.0f, 0.9f, 0.7f, 1.0f),   // Specular: slightly cooler than diffuse
		nullptr
	};

	light.cube.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);

	// Cool blue/purple light
	LightSource light2{
		"White Light",
		glm::vec3(-3.0f, 3.0f, -3.0f),
		glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
		glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		//glm::vec4(0.1f, 0.1f, 0.2f, 1.0f),  // Ambient: subtle cool
		//glm::vec4(0.6f, 0.7f, 1.0f, 1.0f),  // Diffuse: cool blue
		//glm::vec4(0.7f, 0.8f, 1.0f, 1.0f),   // Specular: slightly brighter than diffuse
		nullptr
	};

	light2.cube.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);

	//scene.AddLightSource(&light);
	scene.AddLightSource(&light2);

	app.sceneManager->AddScene(&scene);
	app.sceneManager->SetActiveScene(0);

	while (app.applicationRunning) {
		app.RunFrame();
	}
}