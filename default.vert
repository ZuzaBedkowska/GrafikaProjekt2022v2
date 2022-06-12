// Vertex Shader source code
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

out vec3 Color;
out vec3 Normal;
out vec3 currentPosition;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	currentPosition = vec3(model * vec4 (position, 1.0f));
    gl_Position = camMatrix * vec4(currentPosition, 1.0);
	Color = color;
	Normal = normal;
}