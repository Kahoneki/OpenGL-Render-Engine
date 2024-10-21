#ifndef MATERIAL_H
#define MATERIAL_H


#include <GLM/gtc/matrix_transform.hpp>


/*To be attached to a Drawable object*/

enum MaterialProperties
{
	MATERIAL_COLOUR_BIT = (1 << 0), //0001
};

class Material
{
public:
	Material();
	Material(std::uint16_t activePropertiesBitfield);
	~Material();

	void SetPropertyActive(MaterialProperties property, bool active);
	bool GetPropertyActive(MaterialProperties property);
	glm::vec3 colour;

private:
	std::uint16_t activePropertiesBitfield;
};

#endif