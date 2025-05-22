#include "Material.h"
#include "GLAD/glad.h"
#include "../Application/Application.h"
#include "../Application/AssetManager.h"
#include "../World/Drawable/Drawable.h"
#include "stb_image.h"
#include <iostream>
#include <cstring>
#include "../World/Drawable/ExternalModel.h"

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

	if (ExternalModel* e{ dynamic_cast<ExternalModel*>(drawableParent) })
	{
		for (Mesh* m : e->meshes)
		{
			m->material.SetPropertyActive(property, active);
		}
	}
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

	if (ExternalModel* e{ dynamic_cast<ExternalModel*>(drawableParent) })
	{
		for (Mesh* m : e->meshes)
		{
			m->material.setAmbientColour(colour);
		}
	}
}

glm::vec4 Material::getDiffuseColour()
{
	return materialData.diffuseColour;
}

void Material::setDiffuseColour(glm::vec4 colour)
{
	materialData.diffuseColour = colour;
	drawableParent->UpdateMaterial();

	if (ExternalModel * e{ dynamic_cast<ExternalModel*>(drawableParent) })
	{
		for (Mesh* m : e->meshes)
		{
			m->material.setDiffuseColour(colour);
		}
	}
}

glm::vec4 Material::getSpecularColour()
{
	return materialData.specularColour;
}

void Material::setSpecularColour(glm::vec4 colour)
{
	materialData.specularColour = colour;
	drawableParent->UpdateMaterial();

	if (ExternalModel* e{ dynamic_cast<ExternalModel*>(drawableParent) })
	{
		for (Mesh* m : e->meshes)
		{
			m->material.setSpecularColour(colour);
		}
	}
}

float Material::getSpecularPower()
{
	return materialData.specularPower;
}

void Material::setSpecularPower(float power)
{
	materialData.specularPower = power;
	drawableParent->UpdateMaterial();

	if (ExternalModel* e{ dynamic_cast<ExternalModel*>(drawableParent) })
	{
		for (Mesh* m : e->meshes)
		{
			m->material.setSpecularPower(power);
		}
	}
}

float Material::getRimPower()
{
	return materialData.rimPower;
}

void Material::setRimPower(float power)
{
	materialData.rimPower = power;
	drawableParent->UpdateMaterial();

	if (ExternalModel * e{ dynamic_cast<ExternalModel*>(drawableParent) })
	{
		for (Mesh* m : e->meshes)
		{
			m->material.setRimPower(power);
		}
	}
}

const std::size_t Material::GetMaxProperties()
{
	return 8 * sizeof(materialData.activePropertiesBitfield);
}

GLuint64 Material::getAlbedoTextureHandle()
{
	return materialData.packedAlbedoNormal.x;
}

GLuint64 Material::getNormalTextureHandle()
{
	return materialData.packedAlbedoNormal.y;
}

GLuint64 Material::getSpecularTextureHandle()
{
	return materialData.packedSpecularNA.x;
}

unsigned int Material::getAlbedoTextureName()
{
	return Application::getInstance().assetManager.get()->getTextureName(getAlbedoTextureHandle());
}

unsigned int Material::getNormalTextureName()
{
	return Application::getInstance().assetManager.get()->getTextureName(getNormalTextureHandle());
}

unsigned int Material::getSpecularTextureName()
{
	return Application::getInstance().assetManager.get()->getTextureName(getSpecularTextureHandle());
}

void Material::setAlbedoTextureName(unsigned int textureName)
{
	setAlbedoTextureHandle(Application::getInstance().assetManager.get()->getTextureHandle(textureName));
}

void Material::setNormalTextureName(unsigned int textureName)
{
	setNormalTextureHandle(Application::getInstance().assetManager.get()->getTextureHandle(textureName));
}

void Material::setSpecularTextureName(unsigned int textureName)
{
	setSpecularTextureHandle(Application::getInstance().assetManager.get()->getTextureHandle(textureName));
}

void Material::setAlbedoTextureHandle(GLuint64 textureHandle)
{
	materialData.packedAlbedoNormal.x = textureHandle;
	drawableParent->UpdateMaterial();
}

void Material::setNormalTextureHandle(GLuint64 textureHandle)
{
	materialData.packedAlbedoNormal.y = textureHandle;
	drawableParent->UpdateMaterial();
}

void Material::setSpecularTextureHandle(GLuint64 textureHandle)
{
	materialData.packedSpecularNA.x = textureHandle;
	drawableParent->UpdateMaterial();
}
