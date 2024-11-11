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
	float rimPower;

	uint64_t textureHandle;
	uint activePropertiesBitfield;
};

layout (binding=1, std140) uniform MaterialBlock
{
	MaterialData matData;
} material;


vec4 calculateRimContribution(vec3 normal, vec3 view, vec4 lightColour) {
	//Rim lighting is highest when view vector is orthogonal to normal
	//Just use specular colour and strength multiplied by some constant - might change this later
	
	float rimFactor = 1.0f - dot(normal, view);

	//Transform to range 0,1
	rimFactor = smoothstep(0.0f, 1.0f, rimFactor);

	//Raise to rim exponent
	rimFactor = pow(rimFactor, material.matData.rimPower);
	
	return rimFactor * lightColour;
}


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
		vec4 rim = calculateRimContribution(worldNormal, worldPosToCamera, lightSources.lights[i].diffuseColour);

		FragColour += ambient + diffuse + specular + rim;
	}

	if ((material.matData.activePropertiesBitfield & 2u) != 0u) {
		FragColour *= texture(sampler2D(material.matData.textureHandle), texCoord);
	}
}