#ifndef EXTERNALMODEL_H
#define EXTERNALMODEL_H

#include <assimp/scene.h>

#include <vector>

#include "../../../Shaders/shader.h"
#include "Mesh.h"
#include "../Scene.h"
#include <unordered_map>

class ExternalModel : public Drawable
{

	friend class Material;
	friend class Drawable;

public:

	ExternalModel(const std::string& path, Scene& _sceneParent, const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, bool _reverseWinding=false, SceneObject* parent=nullptr);
	~ExternalModel();
	void Draw(Shader& shader) override;

private:
	//Model data
	std::vector<Mesh*> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	Scene& sceneParent;

	std::unordered_map<aiNode*, SceneObject*> assimpNodeToSceneObject;

	bool reverseWinding;

	void LoadModel(std::string const& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, aiNode* nodeParent);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif