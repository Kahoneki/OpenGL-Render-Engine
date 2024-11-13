#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 texCoord;
out vec3 viewNormal;
out vec3 viewTangent;
out vec3 viewBitangent;
out vec4 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{

	texCoord = aTexCoord;
	viewNormal = normalize(mat3(view * model) * aNormal);
	viewTangent = normalize(mat3(view * model) * aTangent);
	viewBitangent = cross(viewNormal, viewTangent);
	
	viewPos = view * model * vec4(aPos, 1.0f); //Vertex position in view-space
		
	gl_Position = projection * viewPos;
}