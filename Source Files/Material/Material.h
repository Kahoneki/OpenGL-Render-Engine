#ifndef MATERIAL_H
#define MATERIAL_H


#include <GLM/gtc/matrix_transform.hpp>
#include "GLAD/glad.h"



enum MaterialProperties
{
	MATERIAL_COLOUR_BIT = (1 << 0), //0001
};

struct MaterialData
{
	glm::vec4 colour;

	std::uint32_t activePropertiesBitfield; //uint in glsl is 32 bits
};


/*To be attached to a Drawable object*/

class Material
{
public:
	Material();
	Material(std::uint16_t activePropertiesBitfield);
	~Material();

	void SetPropertyActive(MaterialProperties property, bool active);
	bool GetPropertyActive(MaterialProperties property);
	
	[[no_discard]] const std::size_t GetMaxProperties();
	[[no_discard]] const GLsizeiptr GetPaddedSize();

	MaterialData materialData;
};

#endif