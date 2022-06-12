#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ShaderClass.h"


class Camera
{
public:
	//Pozycja kamery
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 4.0f);
	//Punkt na który kamera jest skierowana
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	//Ustawienie kamery w kierunku do góry
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	//Macierz kamery
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	//zmienne pprzechowujące szrokość i wysokość okna
	int width;
	int height;


	//Konstruktor kamery - nadaje wartości poczatkowe polom klasy
	Camera(int width, int height, glm::vec3 position);

	void ChangePosition(double x, double y);
	//Aktualizuje macierz kamery w Vertex Shaderze
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	//Eksportuje macierz kamery do wybranego shadera
	void Matrix(Shader& shader, const char* uniform);

	//obsługa wejścia kamery
	//void Inputs(GLFWwindow* window);
};

#endif