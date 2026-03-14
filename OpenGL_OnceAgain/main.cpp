#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "Shader.h"


const GLint width = 800, Height = 600;
GLuint VAO, VBO,EBO;

bool dir = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncreament = 0.005f;



std::string vertexShader = "shader.vert";
std::string fragmentShader = "shader.frag";

void createTriangle()
{
	unsigned int indices[] = {
		0,2,3,
		2,1,3,
		1,0,3,
		0,1,2
	};


	GLfloat vertices[] = {
		 -0.5f, -0.5f,0.0f, // left vertex of the the front face of triangle : 0***
		 0.0f,-0.5f,0.5f,  // the vertex behind the front face Z vertex : 1***
		0.5f,-0.5f,0.0f, // Right vertex basically  : 2***
		0.0f,0.5f,0.0f // top vertex of the Pyramid : 3***
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//EBO inside The VAO^^^
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
	//^^^VBO inside the VAO^^^
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//Layout (location = 0) ^^^
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int main()
{
	
	if (!glfwInit())
	{
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Basically doesnt allow the usage of old ,removed or planned to get removed features to get used
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	GLFWwindow* mainWindow = glfwCreateWindow(width, Height, "OpenGlWindow", NULL, NULL);
	if (!mainWindow)
	{
		printf("Creating the window has failed!");
		glfwTerminate();
		return 1;
	}

	//Setting Buffer size
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	
	//Let GLEW know which window to draw things into by using CONTEXTS
	glfwMakeContextCurrent(mainWindow);

	//Allow the usage of experimental features in GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Glew init failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}
	//Setting the viewport where the rendering will actually happen
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Testing and enabling the depth buffer

	glEnable(GL_DEPTH_TEST);

	//The while loop of the window(It keeps running untill your filthy ass clicks on the exit button)

	GLfloat defaultOffset = 0;
	GLfloat AnotheroneOffset = 1;

	createTriangle();

	GLfloat aspectRATIO = (GLfloat)800 / (GLfloat)600;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRATIO, 0.1f, 100.0f);

	
	Shaders shader(vertexShader, fragmentShader);

	while (!glfwWindowShouldClose(mainWindow))
	{
		//Handling user Input which in this case is nothing (yet).

		glfwPollEvents();

		if (dir) {
			triOffset += triIncreament;
		}
		else {
			triOffset -= triIncreament;
		}
		if (abs(triOffset )>=triMaxoffset) {
			dir = !dir;
		}

		//clear the window
		glClearColor(0.0f, 0.0f,0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader.GetShaderProgram());
		glBindVertexArray(VAO);

		//creating a model matrix that can do 3 different things - translation - rotation - scaling -
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));

		//calling functions that set and create unifrom variables from our Shader.h folder which contains our Shaders class
		shader.setUNIFORMmat4("projection", projection);
		shader.setUNIFORMmat4("model", model);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		
		glUseProgram(0);
		


		glfwSwapBuffers(mainWindow);
	}
	return 0;
}