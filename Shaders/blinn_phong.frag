#version 460 core
#extension GL_ARB_bindless_texture : require
#ifdef GL_ARB_gpu_shader_int64
	#extension GL_ARB_gpu_shader_int64 : enable
#endif

in vec2 texCoord;
in vec3 worldNormal;
in vec3 worldTangent;
in vec3 worldBitangent;
in vec3 transform;
in vec3 cameraPos;

out vec4 FragColour;

uniform int numLights;
uniform bool useOnlyDiffuse;


struct LightData {
    vec4 position;
    vec4 ambientColour;
    vec4 diffuseColour;
    vec4 specularColour;
    vec4 intensityPack;  // intensity in x component
};

layout (binding=0, std140) uniform LightBlock
{
	LightData[800] lights;
} lightSources;


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



vec4 calculateRimContribution(vec3 normal, vec3 view, vec4 lightColour) {
	//Rim lighting is highest when view vector is orthogonal to normal
	//Just use specular colour and strength multiplied by some constant - might change this later
	
	float rimFactor = 1.0f - dot(normal, view);

	//Transform to range 0,1
	rimFactor = smoothstep(0.0f, 1.0f, rimFactor);

	//Raise to rim exponent
	rimFactor = pow(rimFactor, 5.001f-material.matData.rimPower); //5.001f-power rather than just power because a higher rim power should result in more rim lighting (rimFactor is <= 1) (max rim power is 5)
	
	return rimFactor * lightColour;
}


//Branch logic:
//
//FragColour starts at 0
//If colour bit:
//	If diffuse only:
//		FragColour = diffuse;
//		return;
//	If normal texture: set normal
//	Else: use interpolated normal from vert shader
//	Lighting calculations
//	
//	If albedo texture: multiply by albedo
//
//Else:
//	If albedo texture: FragColour = albedo
//
//Note: useOnlyDiffuse and the active properties bitfield are per-draw-call, they do not vary per-invocation. This means branch divergence on a warp is not an issue.

void main()
{
	FragColour = vec4(0.0f);

	//Colour bit
	if ((material.matData.activePropertiesBitfield & 1u) != 0u) {
		if (useOnlyDiffuse) {
			FragColour = material.matData.diffuseColour;
			return;
		}

		mat3 TBN = mat3(worldTangent, worldBitangent, worldNormal);

		//Normal texture bit - will determine whether to use view space or tangent space
		vec3 lightingNormal; //Normal used in lighting calculations
		vec3 transformToCamera = normalize(cameraPos - transform);
		if ((material.matData.activePropertiesBitfield & 4u) != 0u) {
			#ifdef GL_ARB_gpu_shader_int64
				lightingNormal = normalize(texture(sampler2D(material.matData.normalTextureHandle), texCoord).rgb * 2.0f - vec3(1.0f));
			#else
				lightingNormal = normalize(texture(sampler2D(uvec2(material.matData.normalTextureHandleLow, material.matData.normalTextureHandleHigh)), texCoord).rgb * 2.0f - vec3(1.0f));
			#endif
			transformToCamera = normalize(transpose(TBN) * transformToCamera); //Go from world space to tangent space - TBN is orthogonal (vectors are perpendicular and normalised) so transpose = inverse. Transpose is a cheaper operation than inverse.
		}
		else {
			lightingNormal = normalize(worldNormal);
		}

		//Calculate lighting
		for (int i=0; i<numLights; ++i) {
			LightData light = lightSources.lights[i];

			if (light.intensityPack.x == 0) {
				continue;
			}

			vec3 lightDir = normalize(light.position.xyz - transform); //Vector from fragment position to light

			//Normal texture bit
			if ((material.matData.activePropertiesBitfield & 4u) != 0u) {
				lightDir = normalize(transpose(TBN) * lightDir);
			}
			vec3 lightDirReflectedAroundNormal = reflect(-lightDir, lightingNormal);


			//Setting ambient, diffuse, specular, and rim terms
			vec4 ambient = material.matData.ambientColour * light.ambientColour;
			vec4 diffuse = max(dot(lightingNormal, lightDir), 0.0f) * material.matData.diffuseColour * light.diffuseColour;
			//Specular texture bit
			vec3 specularColour;
			if ((material.matData.activePropertiesBitfield & 8u) != 0u) {
			#ifdef GL_ARB_gpu_shader_int64
				specularColour = texture(sampler2D(material.matData.specularTextureHandle), texCoord).rgb;
			#else
				specularColour = texture(sampler2D(uvec2(material.matData.specularTextureHandleLow, material.matData.specularTextureHandleHigh)), texCoord).rgb;
			#endif
			}
			else {
				specularColour = material.matData.specularColour.rgb;
			}
			vec4 specular = pow(max(dot(lightDirReflectedAroundNormal, transformToCamera), 0.0f), material.matData.specularPower) * material.matData.specularColour * light.specularColour;
			vec4 rim = calculateRimContribution(lightingNormal, transformToCamera, light.diffuseColour);



			//Attenuate distance based on inverse square law
			float lengthFromLightToPoint = length(light.position.xyz - transform);
			float lengthFromPointToCamera = length(transform - cameraPos);
			float totalDistanceTravelled = lengthFromLightToPoint + lengthFromPointToCamera;
			vec4 distanceAttenuatedColour = (ambient + diffuse + specular + rim) / (lengthFromLightToPoint * lengthFromLightToPoint);

			FragColour += distanceAttenuatedColour * light.intensityPack.x;
		}

		//Albedo texture bit
		if ((material.matData.activePropertiesBitfield & 2u) != 0u) {
			#ifdef GL_ARB_gpu_shader_int64
				FragColour *= texture(sampler2D(material.matData.albedoTextureHandle), texCoord);
			#else
				FragColour *= texture(sampler2D(uvec2(material.matData.albedoTextureHandleLow, material.matData.albedoTextureHandleHigh)), texCoord);
			#endif
		}
	}

	else {
		//Albedo texture bit
		if ((material.matData.activePropertiesBitfield & 2u) != 0u) {
			#ifdef GL_ARB_gpu_shader_int64
				FragColour = texture(sampler2D(material.matData.albedoTextureHandle), texCoord);
			#else
				FragColour = texture(sampler2D(uvec2(material.matData.albedoTextureHandleLow, material.matData.albedoTextureHandleHigh)), texCoord);
			#endif
		}
	}
}