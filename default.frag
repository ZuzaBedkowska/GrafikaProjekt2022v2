//Fragment Shader source code
#version 330 core

// wektory wyjsciowe
out vec4 outColor;

// Wektory wejsciowe
in vec3 Color; //skladowe kolorow
//in vec2 myTexture;
in vec3 Normal; // normalna
in vec3 currentPosition; // aktualna pozycja


//uniform sampler2D tex0;
uniform vec4 lightColor; // kolor oswietlenia
uniform vec3 lightPos; // Pozycja oswietlenia
uniform vec3 camPos; // pozycja kamery

vec4 pointLight()
{
	vec3 lightVec = lightPos - currentPosition;
	float dist = length(lightVec);
	float a = 0.5;
	float b = 0.54;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

//ambient lighting
	float ambient = 0.6f;
//diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 1.05f;
	vec3 viewDirection = normalize(camPos - currentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 1024);
	float specular = specAmount * specularLight;

	return vec4(Color, 1.0) * lightColor * (diffuse * inten + ambient + specular);
}

void main()
{
	// outputs final color
	outColor = pointLight();
}
