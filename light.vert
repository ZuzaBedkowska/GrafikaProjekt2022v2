#version 330 core

// Wektory wejsciowe
layout (location = 0) in vec3 position;

//import macierzy z funkcji glownej
uniform mat4 model; //macierz modelu
uniform mat4 camMatrix; //iloczyn macierzy widoku i projekcji

void main()
{
	//Okreslenie zrzutowanych pozycji (wspolrzednych) wszystkich wierzcholkow
	gl_Position = camMatrix * model * vec4(position, 1.0f);
}