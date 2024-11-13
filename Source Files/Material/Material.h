#ifndef MATERIAL_H
#define MATERIAL_H


#include <GLM/gtc/matrix_transform.hpp>
#include <GLAD/glad.h>
#include "../Utility/Vec2ui64.h"



enum MaterialProperties
{
	MATERIAL_COLOUR_BIT         = (1 << 0), //0000 0000 0000 0000 0000 0000 0000 0001
	MATERIAL_ALBEDO_TEXTURE_BIT = (1 << 1), //0000 0000 0000 0000 0000 0000 0000 0010
	MATERIAL_NORMAL_TEXTURE_BIT = (1 << 2), //0000 0000 0000 0000 0000 0000 0000 0100
};

struct MaterialData
{
	glm::vec4 ambientColour;
	glm::vec4 diffuseColour;
	glm::vec4 specularColour;
	float specularPower;
	float rimPower;

	glm::u64vec2 textureHandles; //Albedo + Normal
	std::uint32_t activePropertiesBitfield; //uint in glsl is 32 bits

	MaterialData() {}
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
	float getRimPower();
	void setRimPower(float power);

	GLuint64 getAlbedoTextureHandle();
	GLuint64 getNormalTextureHandle();
	unsigned int getAlbedoTextureName();
	unsigned int getNormalTextureName();
	void setAlbedoTextureHandle(GLuint64 textureHandle);
	void setNormalTextureHandle(GLuint64 textureHandle);
	void setAlbedoTextureName(unsigned int textureName);
	void setNormalTextureName(unsigned int textureName);



private:
	[[no_discard]] const std::size_t GetMaxProperties();
	[[no_discard]] const GLsizeiptr GetPaddedSize(); //For std140 layout padding requirements
	
	MaterialData materialData;

	Drawable* drawableParent; //The drawable that this material is attached to
};

#endif