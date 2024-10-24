#include "Material.h"
#include "GLAD/glad.h"
#include "../World/Drawable/Drawable.h"
#include "../External/stb_image.h"
#include <iostream>

Material::Material()
{
	materialData.activePropertiesBitfield = 0;
	materialData.colour = glm::vec4(1.0f);
	glCreateTextures(GL_TEXTURE_2D, 1, &textureBuffer);
	stbi_set_flip_vertically_on_load(true);
}

Material::Material(std::uint16_t _activePropertiesBitfield)
{
	materialData.activePropertiesBitfield = _activePropertiesBitfield;
	materialData.colour = glm::vec4(1.0f);
	glCreateTextures(GL_TEXTURE_2D, 1, &textureBuffer);
	stbi_set_flip_vertically_on_load(true);
}

Material::~Material()
{
	glDeleteTextures(1, &textureBuffer);
}

void Material::SetPropertyActive(MaterialProperties property, bool active)
{
	if (active) { materialData.activePropertiesBitfield |= property; }
	else { materialData.activePropertiesBitfield &= ~property; }
}

bool Material::GetPropertyActive(MaterialProperties property)
{
	return materialData.activePropertiesBitfield & property;
}

const std::size_t Material::GetMaxProperties()
{
	return 8 * sizeof(materialData.activePropertiesBitfield);
}

const GLsizeiptr Material::GetPaddedSize()
{
	GLsizeiptr size{ 0 };
	
	size += sizeof(materialData.colour); //16 bytes

	size += sizeof(materialData.texture); //4 bytes
	
	size += sizeof(materialData.activePropertiesBitfield); //4 bytes
	
	size += 8; //8 bytes of padding for 16 byte alignment

	return size;
}

glm::vec4 Material::getColour()
{
	return materialData.colour;
}

void Material::setColour(glm::vec4 colour)
{
	materialData.colour = colour;
	parent->UpdateMaterial();
}

unsigned int Material::getTextureID()
{
	return materialData.texture;
}

void Material::setTexture(const char* filepath)
{
	char directory[255] { "Resource Files/" };
	int width, height, nrChannels;
	unsigned char* data{ stbi_load(strcat(directory, filepath), &width, &height, &nrChannels, 0) };

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
			std::cerr << "MATERIAL::SET_TEXTURE::FORMAT_NOT_SUPPORTED::NUMBER_CHANNELS=" << nrChannels;
		}

		glTextureStorage2D(textureBuffer, 1, internalFormat, width, height);
		glTextureSubImage2D(textureBuffer, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cerr << "MATERIAL::SET_TEXTURE::UNABLE_TO_READ_FILE::DIRECTORY=" << directory;
	}
}