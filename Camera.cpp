#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}


void Camera::ChangePosition(double x, double y)
{
	double radius = 2.0;
	double camX = sin(x) * cos(y) * radius;
	double camY = sin(y) * radius;
	double camZ = cos(x) * cos(y) * radius;
	glm::vec3 pomoc = glm::vec3(camX, camY, camZ);
	Position = pomoc;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	//inicjalizacja macierzy view i projection - stają się macierzami jednostkowymi
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//położenie kamery
	view = glm::lookAt(Position, Orientation, Up);

	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	//definicja nowej macierzy kamery
	cameraMatrix = projection * view;
}


//shader wierzchołków, zmienna uniform
void Camera::Matrix(Shader& shader, const char* uniform)
{

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

//Obsługa wejścia kamery
/*void Camera::Inputs(GLFWwindow* window) {
	//powiekszanie obrazu
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Position += speed * Orientation;
	}
	//pomniejszanie obrazu
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Position += speed * -Orientation;
	}
}
*/