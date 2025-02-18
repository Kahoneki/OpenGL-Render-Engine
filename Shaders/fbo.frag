#version 460 core

in vec2 texCoords;
out vec4 FragColour;

layout (binding=1, rgba8) readonly uniform image2D img;

void main()
{
	ivec2 P = ivec2(texCoords * imageSize(img));

	gl_FragDepth = 1.0f;
	FragColour = vec4(imageLoad(img, P));
	FragColour = vec4(1.0, 0.0, 0.0, 1.0);
}