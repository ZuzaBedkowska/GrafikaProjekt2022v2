//Fragment Shader source code
#version 330 core
in vec3 Color;
//in vec2 myTexture;

out vec4 outColor;

//uniform sampler2D tex0;

void main()
{
   outColor = vec4(Color, 1.0f);
}