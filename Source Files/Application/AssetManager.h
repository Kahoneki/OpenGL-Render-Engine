#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <vector>
#include <map>
#include "GLAD/glad.h"

class Application;

/*Asset Manager class - holds all references to textures*/

class AssetManager
{
public:
	AssetManager(std::size_t _maxTextures=256);
	~AssetManager(); //AssetManager is responsible for cleaning up textures

	unsigned int addTexture(const char* filepath); //Creates texture, makes it resident, and returns texture name
	void removeTextureByName(unsigned int textureName);
	void removeTextureByHandle(GLuint64 textureHandle);

	GLuint64 getTextureHandle(unsigned int textureName);
	unsigned int getTextureName(GLuint64 textureHandle);

private:
	std::size_t maxTextures;

	std::vector<unsigned int> textureBuffers;

	std::map<unsigned int, GLuint64> textureHandles; //Map from texture name to texture handle
	unsigned int textureHandleBuffer;
	GLuint64* textureHandleBufferMap; //Persistent and coherent
};

#endif