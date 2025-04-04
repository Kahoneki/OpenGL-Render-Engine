#include <iostream>
#include "Source Files/Application/Application.h"
#include "Source Files/Application/Renderer.h"
#include "Source Files/Application/SceneManager.h"
#include "Source Files/Application/AssetManager.h"
#include "Source Files/Application/TimeManager.h"
#include "Source Files/Camera/PlayerCamera.h"
#include "Source Files/World/Drawable/Cube24.h"
#include "Source Files/World/Drawable/Plane.h"
#include "Source Files/World/Drawable/Special/Portal.h"
#include "Source Files/World/Scene.h"
#include "Source Files/World/LightSource.h"
#include "Shaders/shader.h"
#include "Source Files/Postprocessing Effects/PostprocessingEffects.h"
#include "Source Files/Utility/RandomFunctions.h"
#include "Source Files/World/Drawable/ExternalModel.h"
#include <GLM/gtx/string_cast.hpp>


int main()
{

	Application& app{ Application::getInstance() };

	//Define renderer
	Shader shader{ SHADER_PRESET::STATIC_MATERIAL };
	app.renderer->AddShader(&shader);
	app.renderer->SetActiveShader(0);
	app.renderer->SetClearColour(glm::vec4(0.8f, 1.0f, 1.0f, 1.0f));

	//Define scene
	Scene scene;

	PlayerCamera cam{ "Camera", nullptr };
	scene.AddRenderSource(&cam);
	scene.SetActiveRenderSource(0);

	Plane plane{"Brick Plane", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f), glm::vec3(-90.0f, 0.0f, 0.0f) };
	plane.material.setAmbientColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	plane.material.setDiffuseColour(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
	plane.material.setSpecularColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	plane.material.setSpecularPower(128.0f); // Specular strength
	plane.material.setRimPower(5.0f);
	plane.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	unsigned int brickAlbedoName{ app.assetManager.get()->addTexture("brickwall.jpg") };
	unsigned int brickNormalName{ app.assetManager.get()->addTexture("brickwall_normal.jpg") };
	plane.material.setAlbedoTextureName(brickAlbedoName);
	plane.material.setNormalTextureName(brickNormalName);
	plane.material.SetPropertyActive(MATERIAL_ALBEDO_TEXTURE_BIT, true);
	plane.material.SetPropertyActive(MATERIAL_NORMAL_TEXTURE_BIT, true);
	scene.AddDrawable(&plane);

	//Cube24 cube{ "Cube", glm::vec3(-3.5f, 1.0f, 4.5f), glm::vec3(1.0f), glm::vec3(0.0f) };
	//cube.material.setAmbientColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	//cube.material.setDiffuseColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//cube.material.setSpecularColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//cube.material.setSpecularPower(128.0f); // Specular strength
	//cube.material.setRimPower(5.0f);
	//cube.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	//cube.useOnlyDiffuse = true;
	//scene.AddDrawable(&cube);

	//Cube24 cube2{ "Cube2", glm::vec3(3.5f, 1.0f, -4.5f), glm::vec3(1.0f), glm::vec3(0.0f) };
	//cube2.material.setAmbientColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	//cube2.material.setDiffuseColour(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//cube2.material.setSpecularColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//cube2.material.setSpecularPower(128.0f); // Specular strength
	//cube2.material.setRimPower(5.0f);
	//cube2.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	//cube2.useOnlyDiffuse = true;
	//scene.AddDrawable(&cube2);

	Portal portal1{ "Portal 1", glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(1.25f, 3.0f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f) };
	portal1.portal.material.setAmbientColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	portal1.portal.material.setDiffuseColour(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
	portal1.portal.material.setSpecularColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	portal1.portal.material.setSpecularPower(128.0f); // Specular strength
	portal1.portal.material.setRimPower(5.0f);
	portal1.portal.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	scene.AddDrawable(&portal1);

	Portal portal2{ "Portal 2", glm::vec3(5.0f, 3.0f, 0.0f), glm::vec3(1.25f, 3.0f, 0.1f), glm::vec3(0.0f, 180.0f, 0.0f) };
	portal2.portal.material.setAmbientColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	portal2.portal.material.setDiffuseColour(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
	portal2.portal.material.setSpecularColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	portal2.portal.material.setSpecularPower(128.0f); // Specular strength
	portal2.portal.material.setRimPower(5.0f);
	portal2.portal.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	scene.AddDrawable(&portal2);

	portal1.SetOtherPortal(&portal2);
	portal2.SetOtherPortal(&portal1);


	//External models
	//ExternalModel grass{ "grass.obj", scene, "grass", glm::vec3(0), glm::vec3(1), glm::vec3(0) };
	//grass.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	//scene.AddDrawable(&grass);

	//ExternalModel iphone{ "iPhone/Iphone seceond version finished.obj", scene, "iphone", glm::vec3(0), glm::vec3(1), glm::vec3(0) };
	//iphone.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	//scene.AddDrawable(&iphone);

	//ExternalModel tree{ "Tree/Tree1.obj", scene, "tree", glm::vec3(0), glm::vec3(1), glm::vec3(0) };
	////tree.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	//scene.AddDrawable(&tree);

	//ExternalModel cube3{ "Cube/cube123.obj", scene, "cube3", glm::vec3(0), glm::vec3(1), glm::vec3(0), true };
	//cube3.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	//cube3.UseOnlyDiffuse(true);
	//scene.AddDrawable(&cube3);

	ExternalModel mario{ "mario/Mario.obj", scene, "mario", glm::vec3(0), glm::vec3(1), glm::vec3(0), true };
	//mario.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
	//mario.UseOnlyDiffuse(true);
	//mario.material.SetPropertyActive(MATERIAL_ALBEDO_TEXTURE_BIT, false);
	scene.AddDrawable(&mario);

	//Postprocess effects
	//PPEffect::Saturation satEffect1;
	//satEffect1.SetFactor(1.0f);
	//cam.postprocessOverlay.AddPostprocessingEffect(&satEffect1);
	//PPEffect::Saturation satEffect2;
	//satEffect2.SetFactor(1.0f);
	//cam.postprocessOverlay.AddPostprocessingEffect(&satEffect2);
	//PPEffect::Saturation satEffect3;
	//satEffect3.SetFactor(1.0f);
	//cam.postprocessOverlay.AddPostprocessingEffect(&satEffect3);
	//PPEffect::Saturation satEffect4;
	//satEffect4.SetFactor(1.0f);
	//cam.postprocessOverlay.AddPostprocessingEffect(&satEffect4);
	//PPEffect::Vignette vigEffect1;
	//vigEffect1.SetStrength(0.0f);
	//cam.postprocessOverlay.AddPostprocessingEffect(&vigEffect1);


	constexpr std::size_t NUM_LIGHTS{ 8 };
	LightSource* lights[NUM_LIGHTS];
	std::vector<std::string> lightNames;
	for (std::size_t i{ 0 }; i < NUM_LIGHTS; ++i) {
		lightNames.push_back(std::to_string(i));
	}
	for (int i{ 0 }; i < NUM_LIGHTS; ++i) {
		glm::vec3 specular{ randomVec3(0.0f, 1.0f) };
		lights[i] = new LightSource(
			lightNames[i].c_str(),
			randomVec3ExcludeByDistance(-18.0f, 18.0f, -15.5f, 15.5f),
			glm::vec4(specular * 0.1f, 1.0f),
			glm::vec4(specular * 0.3f, 1.0f),
			glm::vec4(specular, 1.0f),
			randomFloat((1.0f/NUM_LIGHTS) * 1000.0f, (1.0f/NUM_LIGHTS) * 2400.0f),
			nullptr
		);
		lights[i]->cube.material.SetPropertyActive(MATERIAL_COLOUR_BIT, true);
		scene.AddLightSource(lights[i]);
	}


	//Add scene to scene manager
	app.sceneManager->AddScene(&scene);
	app.sceneManager->SetActiveScene(0);
	app.sceneManager->GetActiveScene()->SetAutomaticLightUpdates(false);
	app.sceneManager->GetActiveScene()->UpdateLightSources();

	while (app.applicationRunning) {
		app.RunFrame();
		for (std::size_t i{ 0 }; i < NUM_LIGHTS; ++i) {
			lights[i]->setDiffuseColour(glm::vec4(randomVec3(0.0f, 1.0f) * 0.3f, 1.0f));
		}
	}
}