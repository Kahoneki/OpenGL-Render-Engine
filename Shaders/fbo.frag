#version 460 core

in vec2 texCoords;
out vec4 FragColour;

void main()
{
	gl_FragDepth = 1.0f;
	FragColour = vec4(0.0f);
}