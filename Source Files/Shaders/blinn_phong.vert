#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 worldPos;
out vec2 texCoord;
out vec3 worldNormal;
out vec3 cameraPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

mat3 getNormalMatrix(mat4 worldMatrix) {
    mat3 normalMatrix = transpose(inverse(mat3(worldMatrix)));
    return normalMatrix;
}

void main()
{
	worldPos = vec4(model * vec4(aPos, 1.0f)).xyz;
	texCoord = aTexCoord;
	mat3 normalMatrix = getNormalMatrix(model);
    worldNormal = normalize(normalMatrix * aNormal);
	cameraPos = inverse(view)[3].xyz;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}