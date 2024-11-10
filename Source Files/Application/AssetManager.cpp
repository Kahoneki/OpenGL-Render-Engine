#include "AssetManager.h"
#include "../External/stb_image.h"
#include <iostream>

AssetManager::AssetManager(std::size_t _maxTextures)
{
	maxTextures = _maxTextures;

	glCreateBuffers(1, &textureHandleBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, textureHandleBuffer);
	glBufferStorage(GL_UNIFORM_BUFFER, maxTextures * sizeof(GLuint64), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	textureHandleBufferMap = (GLuint64*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, maxTextures * sizeof(GLuint64), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
}

AssetManager::~AssetManager()
{
	for (std::size_t i{ 0 }; i < textureBuffers.size(); ++i) {
		glMakeTextureHandleNonResidentARB(textureHandles[textureBuffers[i]]);
		textureBuffers.erase(textureBuffers.begin() + i);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, textureHandleBuffer);
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	glDeleteBuffers(1, &textureHandleBuffer);
}


unsigned int AssetManager::addTexture(const char* filepath)
{
	char directory[255]{ "Resource Files/" };
	strcat_s(directory, sizeof(directory), filepath);
	int width, height, nrChannels;
	unsigned char* data{ stbi_load(directory, &width, &height, &nrChannels, 0) };

	if (data) {
		GLenum format;
		GLenum internalFormat;
		switch (nrChannels) {
		case 1:
			format = GL_RED;
			internalFormat = GL_R8;
			break;
		case 2:
			format = GL_RG;
			internalFormat = GL_RG8;
			break;
		case 3:
			format = GL_RGB;
			internalFormat = GL_RGB8;
			break;
		case 4:
			format = GL_RGBA;
			internalFormat = GL_RGBA8;
			break;
		default:
			std::cerr << "ASSET_MANAGER::ADD_TEXTURE::FORMAT_NOT_SUPPORTED::NUMBER_CHANNELS=" << nrChannels;
		}

		if (textureBuffers.size() < maxTextures) {
			textureBuffers.push_back(0);
			std::size_t i{ textureBuffers.size() - 1 };
			glCreateTextures(GL_TEXTURE_2D, 1, &textureBuffers[i]);
			glTextureStorage2D(textureBuffers[i], 1, internalFormat, width, height);
			glTextureSubImage2D(textureBuffers[i], 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
			textureHandles[textureBuffers[i]] = glGetTextureHandleARB(textureBuffers[i]);
			textureHandleBufferMap[i] = textureHandles[textureBuffers[i]];
			glMakeTextureHandleResidentARB(textureHandles[textureBuffers[i]]);
			return textureBuffers[i];
		}
		else {
			//Max textures reached
			std::cerr << "ASSET_MANAGER::ADD_TEXTURE::MAX_TEXTURES_EXCEEDED::MAX_TEXTURES=" << maxTextures;
			return -1;
		}

	}
	else {
		std::cerr << "ASSET_MANAGER::ADD_TEXTURE::UNABLE_TO_READ_FILE::DIRECTORY=" << directory;
		return -1;
	}
	return -1;
}

void AssetManager::removeTexture(unsigned int textureName)
{
	glDeleteTextures(1, &textureName);
	for (std::size_t i{ 0 }; i < textureBuffers.size(); ++i) {
		if (textureBuffers[i] == textureName) {
			textureBuffers.erase(textureBuffers.begin() + i);
			glMakeTextureHandleNonResidentARB(textureHandles[textureName]);
			return;
		}
	}

	//Texture not found
	std::cerr << "ASSET_MANAGER::REMOVE_TEXTURE::TEXTURE_NAME_NOT_FOUND::TEXTURE_NAME=" << textureName;
}


GLuint64 AssetManager::getTextureHandle(unsigned int textureName)
{
	return textureHandles[textureName];
}
