// Vertex Shader source code
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
//layout (location = 2) in vec2 inTexture;

out vec3 Color;
//out vec2 myTexture;

uniform float scale;
uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    gl_Position = model * camMatrix * vec4(position.x*scale, position.y*scale, position.z*scale, 1.0);
	Color = color;
	//myTexture = inTexture;
}