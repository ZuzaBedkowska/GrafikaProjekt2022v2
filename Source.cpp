#include<iostream>
#include <vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

using namespace std;

const unsigned int width = 800;
const unsigned int height = 800;
constexpr auto PI = 3.14159265358979323846;

void makeSphere(int n, int& index, int& index2, int p, GLfloat vertices[], GLuint indices[], double x, double y, double z, double red, double green, double blue, double r)
{
	double lengthInv = 1.0f / r;
	int index1 = index;
	int stack = n; //stack - ilosc kwadratow na poludniku
	int sector = n; //sector - ilosc kwadratow na rownolezniku
	double sectorStep = 2.0 * PI / sector; //sector step - kat co ktory beda pojawiac sie nowe kwadraty na rownolezniku (360/n)
	double stackStep = PI / stack; //kat co ktory beda pojawiac sie nowe kwadraty na poludniku (180/n)
	double newX = 0.0, newY = 0.0, newZ = 0.0; //nowe wspolrzedne
	for (int i = 0; i <= stack; ++i) // dla kazdego kwadratu na poludniku
	{
		double stackAngle = PI / 2 - i * stackStep; //wyznacz kat o ktory jestesmy odchyleni od plaszczyzny xy (od 90 do -90)
		newZ = z + r * sinf(stackAngle); //wyznacz z ze wzoru
		for (int j = 0; j < sector; ++j) //dla kazdego kwadratu na rownolezniku
		{
			double sectorAngle = j * sectorStep; //wyznacz kat o ktory jestesmy odchyleni od plaszczyzny zy
			newX = x + r * cosf(stackAngle) * cosf(sectorAngle); //wyznacz x i y ze wzoru z uwzglednieniem polozenia srodka
			newY = y + r * cosf(stackAngle) * sinf(sectorAngle);
			vertices[index] = newX; //dodaj do tablicy kolejno x, y, z i parametry koloru
			index++;
			vertices[index] = newY;
			index++;
			vertices[index] = newZ;
			index++;
			vertices[index] = red;
			index++;
			vertices[index] = green;
			index++;
			vertices[index] = blue;
			index++;
			vertices[index] = newX * lengthInv;
			index++;
			vertices[index] = newY * lengthInv;
			index++;
			vertices[index] = newZ * lengthInv;
			index++;
		}
	}
	int k1 = 0, k2 = 0; //numery wierzcholkow do laczenia - k1 to przegladany wierzcholek a k2 to wierzcholek tuz pod nim
	int index3 = (index1) / 9;
	for (int i = 0; i < stack; ++i)
	{
		for (int j = 0; j < sector; ++j)
		{
			//gorne trojkaty
			if (i == 0)
			{
				k1 = j + index3;
				k2 = j + sector + index3;
				indices[index2] = k1;
				index2++;
				indices[index2] = k2;
				index2++;
				if (k1 % sector != sector - 1)
				{
					indices[index2] = k2 + 1;
					index2++;
				}
				else
				{
					indices[index2] = k2 + 1 - sector;
					index2++;
				}
			}
			//dolne trojkaty
			else if (i == stack - 1)
			{
				k1 = i * sector + j + index3;
				k2 = k1 + sector;
				indices[index2] = k1;
				index2++;
				indices[index2] = k2;
				index2++;
				if (k1 % sector != sector - 1)
				{
					indices[index2] = k1 + 1;
					index2++;
				}
				else
				{
					indices[index2] = k1 + 1 - sector;
					index2++;
				}

			}
			//reszta
			else
			{
				k1 = i * sector + j + index3;
				k2 = k1 + (sector);
				indices[index2] = k1;
				index2++;
				indices[index2] = k2;
				index2++;
				if (k1 % sector != sector - 1)
				{
					indices[index2] = k1 + 1;
					index2++;
					indices[index2] = k1 + 1;
					index2++;
					indices[index2] = k2 + 1;
					index2++;
				}
				else
				{
					indices[index2] = k1 + 1 - sector;
					index2++;
					indices[index2] = k1 + 1 - sector;
					index2++;
					indices[index2] = k2 + 1 - sector;
					index2++;
				}
				indices[index2] = k2;
				index2++;
			}

		}
	}
}

void makeOrbit(int n, int& index, int& index2, int p, GLfloat vertices[], GLuint indices[], double x, double y, double z, double red, double green, double blue, double r)
{
	double angle = 2 * PI / (n * 10); //kat miedzy ramionami trojkata
	double angle1 = 0.0;
	int index1 = index;
	for (int i = p * 2 * 9 * n * 10 + index; i < (p + 1) * 2 * 9 * n * 10 + index; ++i)
	{
		vertices[i] = x + r * cos(angle1); //pierwszy wierzcholek z f trygonometrycznych
		i++;
		vertices[i] = y + r * sin(angle1);
		i++;
		vertices[i] = z;
		i++;
		vertices[i] = red;
		i++;
		vertices[i] = green;
		i++;
		vertices[i] = blue;
		i++;
		vertices[i] = 0.0;
		i++;
		vertices[i] = 0.0;
		i++;
		vertices[i] = 0.0;
		i++;
		angle1 += angle;
		vertices[i] = x + r * cos(angle1); //2 wierzcholek tak samo jak 1, ale przesuniety o kat
		i++;
		vertices[i] = y + r * sin(angle1);
		i++;
		vertices[i] = z;
		i++;
		vertices[i] = red;
		i++;
		vertices[i] = green;
		i++;
		vertices[i] = blue;
		i++;
		vertices[i] = 0.0;
		i++;
		vertices[i] = 0.0;
		i++;
		vertices[i] = 0.0;
	}
	int index2_kopia = index2;
	for (int i = p*2*n*10; i < (p + 1)*2 * n * 10; ++i)
	{
		indices[i + index2] = i + index1/9; 
	}
}

void calculateCirclePosition(double& x, double& y, double x_s, double y_s, double r, double angle) //nowe wsp srodka okregu, wsp srodka orbity, odleglosc od 0.0, kat odchylenia od 0.0
{
	x = x_s + r * cos(angle);
	y = y_s + r * sin(angle);
}



int main()
{
	int n = 10; //do zrobienia "siatki kuli" - tyle kwadratow bedzie na kazdym rownolezniku i poludniku co daje n*n*2 trojkatow
	int p = 2 * 10 * n * 10;
	GLfloat vertices[18000 + 10 * 11 * 10 * 9]{}; //10 orbit po 500 punktow po 6 wsp, 10 planet po 10*11 punktow po 6 wsp
	GLuint indices[2000 + 10 * 10 * 9 * 3 * 2]{};
	vector <double> distances(10, 0.0);
	for (int i = 0; i < 9; ++i)
	{
		distances[i] = i * 0.1;
	}
	vector <double> x = distances;
	vector <double> y(10, 0.0);
	vector <double> z(10, 0.0);
	vector <double> angle1(10, 0.0);
	vector <double> angle2(10, 0.0);
	angle1[3] = 2 * PI / 10000; //obrot ziemi
	angle1[1] = angle1[3] / 0.24; //proporcjonalne przeszktalcenie czasu obrotu ziemi w programie na czas obrotu merkurego
	angle1[2] = angle1[3] / 0.615;
	angle1[4] = angle1[3] / 1.88;
	angle1[5] = angle1[3] / 11.86;
	angle1[6] = angle1[3] / 29.5;
	angle1[7] = angle1[3] / 84;
	angle1[8] = angle1[3] / 164;
	angle1[9] = angle1[3] * 12;
	double x_s = 0.0, y_s = 0.0, z_s = 0.0; //wspolrzedne srodka ukladu slonecznego
	// Initialize GLFW
	glfwInit();
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	GLFWwindow* window = glfwCreateWindow(width, height, "Opengl", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);



	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	//Utworzenie obiektu kamery
	Camera camera(width, height, glm::vec3(2.5f, 2.5f, 2.0f));

	float rotation1 = 0.0;
	float rotation2 = 0.0;
	double prevTime = glfwGetTime();
	// Main while loop

	while (!glfwWindowShouldClose(window))
	{
		int index1 = 0, index2 = 0;// indeksy ostatnich zapelnionych miejsc w vertices i indices
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		makeSphere(n, index1, index2, 0, vertices, indices, x[0], y[0], z[0], 1.0, 0.98, 0.2219, 0.08); //s, argumenty od y[0] do 0.08 to kolorki w formacie red, green, blue
		calculateCirclePosition(x[1], y[1], x_s, y_s, distances[1], angle2[1]);
		angle2[1] += angle1[1];
		makeSphere(n, index1, index2, 1, vertices, indices, x[1], y[1], z[1], 0.7412, 0.4314, 0.1176, 0.01); //m
		calculateCirclePosition(x[2], y[2], x_s, y_s, distances[2], angle2[2]);
		angle2[2] += angle1[2];
		makeSphere(n, index1, index2, 2, vertices, indices, x[2], y[2], z[2], 0.6235, 0.5412, 0.4196, 0.025); //v
		calculateCirclePosition(x[3], y[3], x_s, y_s, distances[3], angle2[3]);
		angle2[3] += angle1[3];
		makeSphere(n, index1, index2, 3, vertices, indices, x[3], y[3], z[3], 0.1451, 0.5725, 0.7725, 0.028); //e
		calculateCirclePosition(x[4], y[4], x_s, y_s, distances[4], angle2[4]);
		angle2[4] += angle1[4];
		makeSphere(n, index1, index2, 4, vertices, indices, x[4], y[4], z[4], 0.7569, 0.2392, 0.0588, 0.015); //m
		calculateCirclePosition(x[5], y[5], x_s, y_s, distances[5], angle2[5]);
		angle2[5] += angle1[5];
		makeSphere(n, index1, index2, 5, vertices, indices, x[5], y[5], z[5], 0.4941, 0.2863, 0.1176, 0.05); //j
		calculateCirclePosition(x[6], y[6], x_s, y_s, distances[6], angle2[6]);
		angle2[6] += angle1[6];
		makeSphere(n, index1, index2, 6, vertices, indices, x[6], y[6], z[6], 0.8509, 0.7333, 0.5608, 0.035); //s
		calculateCirclePosition(x[7], y[7], x_s, y_s, distances[7], angle2[7]);
		angle2[7] += angle1[7];
		makeSphere(n, index1, index2, 7, vertices, indices, x[7], y[7], z[7], 0.0353, 0.3216, 0.8706, 0.025); //u
		calculateCirclePosition(x[8], y[8], x_s, y_s, distances[8], angle2[8]);
		angle2[8] += angle1[8];
		makeSphere(n, index1, index2, 8, vertices, indices, x[8], y[8], z[8], 0.2706, 0.8941, 0.9922, 0.025); //n
		calculateCirclePosition(x[9], y[9], x[3], y[3], 0.05, angle2[9]);
		angle2[9] += angle1[9];
		makeSphere(n, index1, index2, 9, vertices, indices, x[9], y[9], z[9], 0.78, 0.78, 0.78, 0.01); //moon

		makeOrbit(n, index1, index2, 0, vertices, indices, x_s, y_s, z_s, 1, 1, 1, distances[1]); //m orbit
		makeOrbit(n, index1, index2, 1, vertices, indices, x_s, y_s, z_s, 1, 1, 1, distances[2]); //v orbit
		makeOrbit(n, index1, index2, 2, vertices, indices, x_s, y_s, z_s, 1, 1, 1, distances[3]); //e orbit
		makeOrbit(n, index1, index2, 3, vertices, indices, x_s, y_s, z_s, 1, 1, 1, distances[4]); //m orbit
		makeOrbit(n, index1, index2, 4, vertices, indices, x_s, y_s, z_s, 1, 1, 1, distances[5]); //j orbit
		makeOrbit(n, index1, index2, 5, vertices, indices, x_s, y_s, z_s, 1, 1, 1, distances[6]); //s orbit
		makeOrbit(n, index1, index2, 6, vertices, indices, x_s, y_s, z_s, 1, 1, 1, distances[7]); //u orbit
		makeOrbit(n, index1, index2, 7, vertices, indices, x_s, y_s, z_s, 1, 1, 1, distances[8]); //n orbit
		makeOrbit(n, index1, index2, 8, vertices, indices, x[6], y[6], z[6], 1, 1, 1, 0.045);  //s ring
		makeOrbit(n, index1, index2, 9, vertices, indices, x[3], y[3], z[3], 1, 1, 1, 0.05);  //moon orbit
		VAO VAO1;
		VAO1.Bind();

		// Generates Vertex Buffer Object and links it to vertices
		VBO VBO1(vertices, sizeof(vertices));
		// Generates Element Buffer Object and links it to indices
		EBO EBO1(indices, sizeof(indices));

		// Links VBO attributes such as coordinates and colors to VAO
		VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
		VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		// Unbind all to prevent accidentally modifying them
		VAO1.Unbind();
		VBO1.Unbind();
		EBO1.Unbind();

		// Gets ID of uniform called "scale"
		GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
		GLuint uniModel = glGetUniformLocation(shaderProgram.ID, "model");
		// Texture
		//Texture popCat("kotek.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		//popCat.texUnit(shaderProgram, "tex0", 0);

		camera.Matrix(45.0f, 1.0f, 10.0f, shaderProgram, "camMatrix");

		glEnable(GL_DEPTH_TEST);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 1.5f);

		camera.Matrix(45.0f, 1.0f, 10.0f, shaderProgram, "camMatrix");
		VAO1.Bind();
		glm::mat4 model = glm::mat4(1.0f);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			rotation1 += 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			rotation1 -= 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			rotation2 += 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			rotation2 -= 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			rotation1 = 0.0f;
			rotation2 = 0.0f;
		}
		double crntTime = glfwGetTime();
		model = glm::rotate(model, glm::radians(rotation1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotation2), glm::vec3(1.0f, 0.0f, 0.0f));
		
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 5400, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_LINES, 2000, GL_UNSIGNED_INT, (void*)(5400 * sizeof(GL_UNSIGNED_INT)));

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
		VAO1.Delete();
		VBO1.Delete();
		EBO1.Delete();
		}



	// Delete all the objects we've created
	//popCat.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}