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
	materialData.ambientColour = glm::vec4(0.0f);
	materialData.diffuseColour = glm::vec4(0.0f);
	materialData.specularColour = glm::vec4(0.0f);
	stbi_set_flip_vertically_on_load(true);
}

Material::Material(std::uint16_t _activePropertiesBitfield)
{
	materialData.activePropertiesBitfield = _activePropertiesBitfield;
	materialData.ambientColour = glm::vec4(0.0f);
	materialData.diffuseColour = glm::vec4(0.0f);
	materialData.specularColour = glm::vec4(0.0f);
	stbi_set_flip_vertically_on_load(true);
}

Material::~Material()
{
}

void Material::SetPropertyActive(MaterialProperties property, bool active)
{
	if (active) { materialData.activePropertiesBitfield |= property; }
	else { materialData.activePropertiesBitfield &= ~property; }
	drawableParent->UpdateMaterial();
}

bool Material::GetPropertyActive(MaterialProperties property)
{
	return materialData.activePropertiesBitfield & property;
}

glm::vec4 Material::getAmbientColour()
{
	return materialData.ambientColour;
}

void Material::setAmbientColour(glm::vec4 colour)
{
	materialData.ambientColour = colour;
	drawableParent->UpdateMaterial();
}

glm::vec4 Material::getDiffuseColour()
{
	return materialData.diffuseColour;
}

void Material::setDiffuseColour(glm::vec4 colour)
{
	materialData.diffuseColour = colour;
	drawableParent->UpdateMaterial();
}

glm::vec4 Material::getSpecularColour()
{
	return materialData.specularColour;
}

void Material::setSpecularColour(glm::vec4 colour)
{
	materialData.specularColour = colour;
	drawableParent->UpdateMaterial();
}

float Material::getSpecularPower()
{
	return materialData.specularPower;
}

void Material::setSpecularPower(float power)
{
	materialData.specularPower = power;
	drawableParent->UpdateMaterial();
}

float Material::getRimPower()
{
	return materialData.rimPower;
}

void Material::setRimPower(float power)
{
	materialData.rimPower = power;
	drawableParent->UpdateMaterial();
}

const std::size_t Material::GetMaxProperties()
{
	return 8 * sizeof(materialData.activePropertiesBitfield);
}

const GLsizeiptr Material::GetPaddedSize()
{
	GLsizeiptr size{ 0 };
	
	size += sizeof(materialData.ambientColour); //16 bytes

	size += sizeof(materialData.diffuseColour); //16 bytes

	size += sizeof(materialData.specularColour); //16 bytes

	size += sizeof(materialData.specularPower); //4 bytes
	size += 12;

	size += sizeof(materialData.rimPower); //4 bytes
	size += 12;

	size += sizeof(materialData.textureHandle); //8 bytes
	size += 8;

	size += sizeof(materialData.activePropertiesBitfield); //4 bytes
	size += 12;

	//Padding bytes for 16 byte alignment
	if (size % 16 != 0) {
		size += 16 - (size % 16);
	}

	return size;
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
	drawableParent->UpdateMaterial();
}
