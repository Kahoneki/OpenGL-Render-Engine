#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

in vec3 worldPos;
in vec2 texCoord;
in vec3 worldNormal;
in vec3 cameraPos;

out vec4 FragColour;

uniform int numLights;
uniform bool isEmissive;


struct LightData
{
	vec4 position;
	
	vec4 ambientColour;
	vec4 diffuseColour;
	vec4 specularColour;
};

layout (binding=0, std140) uniform LightBlock
{
	LightData[256] lights;
} lightSources;


struct MaterialData
{
	vec4 ambientColour;
	vec4 diffuseColour;
	vec4 specularColour;
	float specularPower;

	uint64_t textureHandle;
	uint activePropertiesBitfield;
};

layout (binding=1, std140) uniform MaterialBlock
{
	MaterialData matData;
} material;


void main()
{


	if (isEmissive) {
		FragColour = material.matData.diffuseColour;
		return;
	}


	//Calculate lighting
	vec3 worldPosToCamera = normalize(cameraPos - worldPos);
	for (int i=0; i<numLights; ++i) {
		vec3 worldPosToLightPos = normalize(lightSources.lights[i].position.xyz - worldPos.xyz);
		vec3 lightDirReflectedAroundNormal = reflect(-worldPosToLightPos, worldNormal);
		
		vec4 ambient = material.matData.ambientColour * lightSources.lights[i].ambientColour;
		vec4 diffuse = max(dot(worldNormal, worldPosToLightPos), 0.0f) * material.matData.diffuseColour * lightSources.lights[i].diffuseColour;
		vec4 specular = pow(max(dot(lightDirReflectedAroundNormal, worldPosToCamera), 0.0f), material.matData.specularPower) * material.matData.specularColour * lightSources.lights[i].specularColour;
	
		FragColour += ambient + diffuse + specular;
	}

	if ((material.matData.activePropertiesBitfield & 2u) != 0u) {
		FragColour *= texture(sampler2D(material.matData.textureHandle), texCoord);
	}
}