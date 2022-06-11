#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::ChangePosition(double x, double y, double z)
{
	double radius = 4.0;
	double camX = sin(x) * radius;
	double camY = sin(y) * radius;
	double camZ = cos(x)*cos(y) * radius;
	glm::vec3 pomoc = glm::vec3(camX, camY, camZ);
	Position = pomoc;
}



void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::lookAt(Position, Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection*view));
}
