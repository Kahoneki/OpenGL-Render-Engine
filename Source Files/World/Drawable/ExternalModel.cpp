#include "ExternalModel.h"

#include "Mesh.h"
#include "../../../Shaders/shader.h"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <iostream>
#include "../../Application/Application.h"
#include "../../Application/AssetManager.h"



ExternalModel::ExternalModel(const std::string& path, Scene& _sceneParent, const char* name, glm::vec3 center, glm::vec3 scale, glm::vec3 rotation, bool _reverseWinding, SceneObject* parent)
	: SceneObject(name, parent, Transform(center, scale, rotation)),
	  Drawable(name, center, scale, rotation, parent),
	  sceneParent(_sceneParent)
{
	reverseWinding = _reverseWinding;
	LoadModel(path);
}

ExternalModel::~ExternalModel()
{
	for (Mesh* m : meshes)
	{
		delete m;
	}
}


void ExternalModel::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->instances = instances;
		meshes[i]->Draw(shader);
	}
}


void ExternalModel::LoadModel(std::string const& path)
{
	//Import scene
	Assimp::Importer importer;
	unsigned int importFlags = aiProcess_Triangulate;
	if (reverseWinding) { importFlags |= aiProcess_FlipWindingOrder; }
	const aiScene* scene = importer.ReadFile(std::string("Resource Files/") + path, importFlags);

	//Check for null scene, null root node, or incomplete data
	if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}


glm::vec3 AIVec3ToGLMVec3(aiVector3D vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}


void ExternalModel::ProcessNode(aiNode* node, const aiScene* scene)
{
	//Create new SceneObject for this node and update scene hierarchy
	Transform transform;
	aiVector3D scaling;
	aiVector3D rotation;
	aiVector3D translation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	transform.scale = AIVec3ToGLMVec3(scaling);
	transform.rotation = AIVec3ToGLMVec3(rotation);
	transform.position = AIVec3ToGLMVec3(translation);

	assimpNodeToSceneObject[node] = new SceneObject(node->mName.C_Str(), node->mParent ? assimpNodeToSceneObject[node->mParent] : this, transform);
	sceneParent.AddSceneObject(assimpNodeToSceneObject[node]);

	//Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, node));
	}

	//Recursively call this function for all the node's children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* ExternalModel::ProcessMesh(aiMesh* mesh, const aiScene* scene, aiNode* nodeParent)
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
	return new Mesh(vertices, indices, textures, mesh->mName.C_Str(), glm::vec3(0), glm::vec3(1), glm::vec3(0), assimpNodeToSceneObject[nodeParent]);
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
			std::string filename{ directory + '/' + std::string(str.C_Str()) };
			texture.id = Application::getInstance().assetManager.get()->addTexture(filename.c_str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}