#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

in vec2 texCoords;
out vec4 FragColour;

struct MaterialData
{
	uint64_t albedoTextureHandle;
	uint64_t normalTextureHandle;
};

layout (binding=1, std140) uniform MaterialBlock
{
	MaterialData matData;
} material;