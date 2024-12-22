#version 460 core
#extension GL_ARB_bindless_texture : require

#ifdef GL_ARB_gpu_shader_int64
	#extension GL_ARB_gpu_shader_int64 : enable
#endif

in vec2 texCoord;
out vec4 FragColour;

struct MaterialData
{
	vec4 ambientColour;
	vec4 diffuseColour;
	vec4 specularColour;
	float specularPower;
	float rimPower;

	#ifdef GL_ARB_gpu_shader_int64
		uint64_t albedoTextureHandle;
		uint64_t normalTextureHandle;
	#else
		uint albedoTextureHandleLow;
		uint albedoTextureHandleHigh;
		uint normalTextureHandleLow;
		uint normalTextureHandleHigh;
	#endif

	uint activePropertiesBitfield;
};

layout (binding=1, std140) uniform MaterialBlock
{
	MaterialData matData;
} material;


void main()
{
	FragColour = vec4(1.0f);
	if ((material.matData.activePropertiesBitfield & 2u) != 0u) {
		#ifdef GL_ARB_gpu_shader_int64
			FragColour = texture(sampler2D(material.matData.albedoTextureHandle), texCoord);
		#else
	}
	if ((material.matData.activePropertiesBitfield & 1u) != 0u) {
		FragColour *= material.matData.diffuseColour;
	}
}