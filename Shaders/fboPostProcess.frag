#version 460 core

in vec2 texCoords;
out vec4 FragColour;

layout (binding=1, rgba8) readonly uniform image2D img;

void main()
{
	ivec2 P = ivec2(texCoords * imageSize(img));
	FragColour = vec4(imageLoad(img, P));
}