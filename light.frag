#version 330 core

out vec4 outColor;

uniform vec4 lightColor;

void main()
{
	outColor = lightColor;
}