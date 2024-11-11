#ifndef MATERIAL_H
#define MATERIAL_H


#include <GLM/gtc/matrix_transform.hpp>
#include "GLAD/glad.h"



enum MaterialProperties
{
	MATERIAL_COLOUR_BIT  = (1 << 0), //0000 0000 0000 0000 0000 0000 0000 0001
	MATERIAL_TEXTURE_BIT = (1 << 1), //0000 0000 0000 0000 0000 0000 0000 0010
};

struct MaterialData
{
	glm::vec4 ambientColour;
	glm::vec4 diffuseColour;
	glm::vec4 specularColour;
	float specularPower;

	GLuint64 textureHandle;
	std::uint32_t activePropertiesBitfield; //uint in glsl is 32 bits
};


class Drawable;


/*To be attached to a Drawable object*/

class Material
{
	friend class Drawable;

public:
	Material();
	Material(std::uint16_t activePropertiesBitfield);
	~Material();

	void SetPropertyActive(MaterialProperties property, bool active);
	bool GetPropertyActive(MaterialProperties property);
	

	//MaterialData getters + setters//

	glm::vec4 getAmbientColour();
	void setAmbientColour(glm::vec4 colour);
	glm::vec4 getDiffuseColour();
	void setDiffuseColour(glm::vec4 colour);
	glm::vec4 getSpecularColour();
	void setSpecularColour(glm::vec4 colour);
	float getSpecularPower();
	void setSpecularPower(float power);

	GLuint64 getTextureHandle();
	void setTextureName(unsigned int textureName);
	void setTextureHandle(GLuint64 textureHandle);



private:
	[[no_discard]] const std::size_t GetMaxProperties();
	[[no_discard]] const GLsizeiptr GetPaddedSize();
	
	MaterialData materialData;

	Drawable* drawableParent;
};

#endif