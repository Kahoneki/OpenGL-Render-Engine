#include "ExternalModel.h"

#include "Mesh.h"
#include "../../../Shaders/shader.h"
#include "../../External/stb_image.h"

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>

#include <string>
#include <iostream>
#include "../../Application/Application.h"
#include "../../Application/AssetManager.h"



ExternalModel::ExternalModel(const std::string& path, Scene& _sceneParent, const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, SceneObject* parent)
	: Drawable(name, center, scale, rotation, parent),
	sceneParent(_sceneParent)
{
	std::string filepath{ "Resource Files/" };
	LoadModel(filepath + path);
}


void ExternalModel::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].instances = instances;
		meshes[i].Draw(shader);
	}
}


void ExternalModel::LoadModel(std::string const& path)
{
	//Import scene
	Assimp::Importer importer;
	unsigned int importFlags = aiProcess_Triangulate | aiProcess_FlipUVs;
	const aiScene* scene = importer.ReadFile(path, importFlags);

	//Check for null scene, null root node, or incomplete data
	if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}


void ExternalModel::ProcessNode(aiNode* node, const aiScene* scene)
{
	//Create new SceneObject for this node and update scene hierarchy
	assimpNodeToSceneObject[node] = new SceneObject(node->mName.C_Str(), nullptr, Transform());
	sceneParent.AddSceneObject(assimpNodeToSceneObject[node]);
	for (std::unordered_map<aiNode*, SceneObject*>::iterator it{ assimpNodeToSceneObject.begin() }; it != assimpNodeToSceneObject.end(); ++it)
	{
		if (it->first->mParent == node)
		{
			it->second->parent = assimpNodeToSceneObject[node];
			if (std::find(assimpNodeToSceneObject[node]->children.begin(), assimpNodeToSceneObject[node]->children.end(), it->second) == assimpNodeToSceneObject[node]->children.end())
			{
				assimpNodeToSceneObject[node]->children.push_back(it->second);
			}
		}
	}

	//Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
		sceneParent.AddDrawable(&meshes[meshes.size() - 1]);
		assimpNodeToSceneObject[node]->children.push_back(new SceneObject(mesh->mName.C_Str(), assimpNodeToSceneObject[node], Transform()));
	}

	//Recursively call this function for all the node's children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh ExternalModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;


	//----ADD VERTICES----//
	for (std::size_t i{ 0 }; i < mesh->mNumVertices; ++i) {
		//--Process vertex positions, normals, and texture coordinates--//
		vertices.push_back(static_cast<float>(mesh->mVertices[i].x));
		vertices.push_back(static_cast<float>(mesh->mVertices[i].y));
		vertices.push_back(static_cast<float>(mesh->mVertices[i].z));
		

		if (mesh->HasNormals()) {
			vertices.push_back(static_cast<float>(mesh->mNormals[i].x));
			vertices.push_back(static_cast<float>(mesh->mNormals[i].y));
			vertices.push_back(static_cast<float>(mesh->mNormals[i].z));
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}

		//Checking that the mesh conntains texture coordinates
		if (mesh->mTextureCoords[0]) {
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}


	//----ADD INDICES----//
	//Loop through all indices of all faces
	for (std::size_t i{ 0 }; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (std::size_t j{ 0 }; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}


	//----ADD TEXTURES----//
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//Diffuse maps
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "albedo");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		//Specular maps
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures, mesh->mName.C_Str(), glm::vec3(0), glm::vec3(1), glm::vec3(0), this);
}


std::vector<Texture> ExternalModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		//Checking if texture has already been loaded
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				//Use the already loaded texture
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			//texture.id = LoadTexture(str.C_Str(), this->directory);
			std::string filename = std::string(directory);
			filename = directory + '/' + filename;
			texture.id = Application::getInstance().assetManager.get()->addTexture(filename.c_str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}