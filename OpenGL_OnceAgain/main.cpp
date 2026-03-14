#include <stdio.h>
#include <GL/glew.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

const GLint width = 800, Height = 600;
GLuint VAO, VBO,EBO, shader,uniformModel,uniformProjection;

bool dir = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncreament = 0.005f;

static const char* vShader = R"(		
#version 330									
											
layout (location = 0) in vec3 pos;				
												
uniform mat4 model;			
uniform mat4 projection;
					
out vec3 vCol;												
void main()										
{												
	gl_Position = projection * model * vec4(pos.x,pos.y,pos.z,1.0f);	
	vCol = (clamp(pos, 0.0,1.0));
}
)";


static const char* fShader = R"(		
#version 330									
												
out vec4 color;									
in vec3 vCol;			
void main()										
{												
	color =  vec4(vCol,1.0f);
}												
)";

void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar elog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
		printf("error while compiling the %d shader :'%s'\n",shaderType, elog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

void compileShaders()
{
	shader = glCreateProgram();

	if (!shader) {
		printf("Failed to creathe the shader program!\n");
		return;
	}

	addShader(shader, vShader, GL_VERTEX_SHADER);
	addShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar elog[1024] = { 0 };

	//Linking the shader program and checking for linking
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		printf("error while linking program :'%s'\n", elog);
		return;
	}
	//Validating the shader program and checking for validating
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		printf("error while validating program :'%s'\n", elog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjection = glGetUniformLocation(shader, "projection");
}



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
	//createTriangle(AnotheroneOffset);
	compileShaders();
	float angle = 0.0f;

	GLfloat aspectRATIO = (GLfloat)800 / (GLfloat)600;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRATIO, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(mainWindow))
	{
		//Handling user Input which in this case is me
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
		angle += 0.3f;
		if (angle >= 360) angle -= 0.3f;
		//clear the window
		glClearColor(0.0f, 0.0f,0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 model2 = glm::mat4(1.0f);
		//model2 = glm::rotate(model2, glm::radians(45.0f),glm::vec3(0.0f,1.0f,0.0f));



		//glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformModel2, 1, GL_FALSE, glm::value_ptr(model2));

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
		


		glfwSwapBuffers(mainWindow);
	}
	return 0;
}