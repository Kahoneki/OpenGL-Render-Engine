#version 450 core

out vec4 FragColour;


struct Light
{
	vec3 position;
	vec3 colour;
	float intensity;
};

layout (binding=0, std430) buffer Lights
{
	Light lights[];
}


int main() {

}