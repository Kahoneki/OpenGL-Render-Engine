#include "Material.h"
#include "GLAD/glad.h"
#include "../Application/Application.h"
#include "../Application/AssetManager.h"
#include "../World/Drawable/Drawable.h"
#include "../External/stb_image.h"
#include <iostream>
#include <cstring>

Material::Material()
{
	materialData.activePropertiesBitfield = 0;
	materialData.colour = glm::vec4(1.0f);
	stbi_set_flip_vertically_on_load(true);
}

Material::Material(std::uint16_t _activePropertiesBitfield)
{
	materialData.activePropertiesBitfield = _activePropertiesBitfield;
	materialData.colour = glm::vec4(1.0f);
	stbi_set_flip_vertically_on_load(true);
}

Material::~Material()
{
}

void Material::SetPropertyActive(MaterialProperties property, bool active)
{
	if (active) { materialData.activePropertiesBitfield |= property; }
	else { materialData.activePropertiesBitfield &= ~property; }
	parent->UpdateMaterial();
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

	size += sizeof(materialData.textureHandle); //8 bytes
	
	size += sizeof(materialData.activePropertiesBitfield); //4 bytes
	
	size += 4; //4 bytes of padding for 16 byte alignment

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

GLuint64 Material::getTextureHandle()
{
	return materialData.textureHandle;
}

void Material::setTextureName(unsigned int textureName)
{
	setTextureHandle(Application::getInstance().assetManager.get()->getTextureHandle(textureName));
}

void Material::setTextureHandle(GLuint64 textureHandle)
{
	materialData.textureHandle = textureHandle;
	parent->UpdateMaterial();
}
