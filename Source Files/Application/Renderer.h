#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class Application;
class Shader;
class Scene;
class SceneObject;

/*Main class - holds Scene, and Shader references, call Renderer::Render() to render the active scene with the active rendersource and active shader*/

/*TODO: Look into how to make renderer unable to delete scenes and shaders passed in. Keyword: std::unique_ptr*/

class Renderer
{
public:
	Renderer(Application* _app);
	Renderer(Shader* shader);
	Renderer(std::vector<Shader*> _shaders);
	~Renderer(); //Renderer is not responsible for clearing up memory allocated to scenes and shaders

	void Render(Scene* scene);
	
	std::size_t AddShader(Shader* shader); //Returns index
	void RemoveShader(std::size_t index);
	void SetActiveShader(std::size_t index);
	Shader* GetActiveShader();
	Shader* GetShader(std::size_t index);

	void SetClearColour(glm::vec4 colour);
	glm::vec4 GetClearColour();

private:
	Application* app;

	std::vector<Shader*> shaders;
	std::size_t activeShader; //Index

	Shader* lightSourceShader;

	void RenderImGui();
	void DrawHeirarchy(SceneObject* s);

	bool dropdownStatuses[1]; //ImGui dropdowns

	glm::vec4 clearColour;
};

#endif