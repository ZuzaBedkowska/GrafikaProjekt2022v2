// Vertex Shader source code
#version 330 core
layout (location = 0) in vec3 position; // Wspolrzedne wierzcholkow
layout (location = 1) in vec3 color; //skladowe kolorow
//layout (location = 2) in vec2 inTexture; // Zaczepienie tekstury
layout (location = 3) in vec3 normal; //Normalne - niekoniecznie unormwoane

out vec3 Color;
//out vec2 myTexture;
out vec3  Normal; // Normalna --> fragment shader
out vec3 currentPosition; // Aktualna pozycja --> fragment shader

//import macierzy z funkcji głównej
uniform float scale;
uniform mat4 model;
uniform mat4 camMatrix; // iloczyn macierzy widoku i projekcji

void main()
{
	// okreslenie biezacej pozycji
	currentPosition = vec3(model * vec4 (position.x*scale, position.y*scale, position.z*scale, 1.0f));
    //Okreslenie rzutowanych pozycji wszystkich wierzcholkow
	gl_Position = model * camMatrix * vec4(position.x*scale, position.y*scale, position.z*scale, 1.0);
	//Przepisanie kolorow z danych wierzcholkowych do wektora "Color"
	Color = color;
	Normal = normal
}
