#include "Material.h"

Material::Material()
{
	activePropertiesBitfield = 0;
}

Material::Material(std::uint16_t _activePropertiesBitfield)
{
	activePropertiesBitfield = _activePropertiesBitfield;
	colour = glm::vec3(1.0f);
}

Material::~Material()
{
}

void Material::SetPropertyActive(MaterialProperties property, bool active)
{
	if (active) { activePropertiesBitfield |= property; }
	else { activePropertiesBitfield &= ~property; }
}

bool Material::GetPropertyActive(MaterialProperties property)
{
	return activePropertiesBitfield & property;
}