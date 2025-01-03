#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 texCoord;
out vec3 worldNormal;
out vec3 worldTangent;
out vec3 worldBitangent;
out vec3 transform;
out vec3 cameraPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	texCoord = aTexCoord;
	worldNormal = normalize(mat3(model) * aNormal);
	worldTangent = normalize(mat3(model) * aTangent);
	worldBitangent = cross(worldNormal, worldTangent);
	
	transform = vec4(model * vec4(aPos, 1.0f)).xyz; //Vertex position in world-space
	cameraPos = inverse(view)[3].xyz;
		
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}