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
		uint64_t specularTextureHandle;
		uint64_t NATextureHandle;
	#else
		uint albedoTextureHandleLow;
		uint albedoTextureHandleHigh;
		uint normalTextureHandleLow;
		uint normalTextureHandleHigh;
		uint specularTextureHandleLow;
		uint specularTextureHandleHigh;
		uint NATextureHandleLow;
		uint NATextureHandleHigh;
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
			FragColour = texture(sampler2D(uvec2(material.matData.albedoTextureHandleLow, material.matData.albedoTextureHandleHigh)), texCoord);
		#endif
	}
	if ((material.matData.activePropertiesBitfield & 1u) != 0u) {
		FragColour *= material.matData.diffuseColour;
	}
}