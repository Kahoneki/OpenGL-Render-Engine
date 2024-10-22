#include "Material.h"
#include "GLAD/glad.h"

Material::Material()
{
	materialData.activePropertiesBitfield = 0;
	materialData.colour = glm::vec4(1.0f);
}

Material::Material(std::uint16_t _activePropertiesBitfield)
{
	materialData.activePropertiesBitfield = _activePropertiesBitfield;
	materialData.colour = glm::vec4(1.0f);
}

Material::~Material()
{
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
	
	size += sizeof(materialData.activePropertiesBitfield); //4 bytes
	size += 12; //12 bytes of padding for 16 byte uint alignment

	return size;
}
