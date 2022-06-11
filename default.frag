//Fragment Shader source code
#version 330 core

// Wektory wejsciowe
in vec3 Color; //skladowe kolorow
//in vec2 myTexture;
in vec3 Normal; // normalna
in vec3 currentPosition; // aktualna pozycja

// wektory wyjsciowe
out vec4 outColor;

//uniform sampler2D tex0;
uniform vec4 lightColor; // kolor oswietlenia
uniform vec3 lightPos; // Pozycja oswietlenia
uniform vec3 camPos; // pozycja kamery


void main()
{
	//Natezenie osweitlenia
	//Swiatlo otoczenai
	float ambient = 0.2f;
	// Odbicie rozproszone
	float diffLight = 1.0f;
	// Odbicie zwierciadlane
	float specularLight = 0.5f;

//Oswietlenie rozproszone
	vec3 fragNormal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currentPosition);
	float diffuse =  diffLight * max(dot(fragNormal, lightDirection), 0.0f);

//Oswietlenie zwierciadlane
	vec3 viewDirection = normalize(camPos - currentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, fragNormal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 128);
	float specular = specAmount * specularLight;



   outColor = vec4(Color, 1.0f) * lightColor * (diffuse + ambient + specular);
}
