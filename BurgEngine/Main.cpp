#include "Common.h"
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// SOIL
#include <SOIL/SOIL.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// assimp

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// Other
#include <stdio.h>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Model.h"



void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void UpdateCamera();
const GLuint WIDTH = 800, HEIGHT = 600;

bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat aspect = DEFAULT_FOV;
GLfloat farPlane = 100.0f;
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;

Camera MainCamera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// =======================================================
	//                   INITIALIZE
	// =======================================================
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGl Program", nullptr, nullptr);
	if (window == nullptr)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW\n");
		return false;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, Key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	//// Build and compile our shader program
	//Shader lightingShader("../Data/Shaders/vertexShaderLighting_nano.vs",
	//	"../Data/Shaders/fragmentShaderLighting_nano.frag", 2);

	// Build and compile our shader program
	Shader lightingShader("../Data/Shaders/vertexShaderLighting_nano.vs",
		"../Data/Shaders/fragmentShaderLighting_nano.frag", 2);

	Shader lampShader("../Data/Shaders/vertexShaderLighting_Lamp.vs",
		"../Data/Shaders/fragmentShaderLighting_Lamp.frag", 0);

	// set the application to capture the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glm::vec3 pointLightPositions[] =
	{
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(-2.0f, 0.0f, -2.0f),
	};



	DirLight dirlight(lightingShader, vect3(-0.2f, -1.0f, -0.3f), vect3(255, 255, 255), 0.5f);
	PointLight pointLight1(lightingShader, pointLightPositions[0], vect3(255, 255, 255), 0.5f);
	PointLight pointLight2(lightingShader, pointLightPositions[1], vect3(255, 255, 255), 0.5f);
	//PointLight pointLight3(lightingShader, pointLightPositions[2], vect3(255, 255, 255), 1.0f);
	//PointLight pointLight4(lightingShader, pointLightPositions[3], vect3(255, 255, 255), 1.0f);
	SpotLight spotLight(lightingShader, MainCamera.Position, MainCamera.Front, vect3(255, 255, 255), 1.0f);

	GLfloat time = glfwGetTime();
	Model nanosuit("../Data/Models/Box/OutPutModel.txt");
	time = glfwGetTime() - time;
	printf("Time to load: %f", time);

	nanosuit.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	
	// =======================================================
	//                  MAIN UPDATE LOOP
	// =======================================================
	//float count = 0;
	//GLfloat time = 0;

	while (!glfwWindowShouldClose(window))
	{

		// Update deltaTime
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// FPS check
		//if (time >= 1.0f)
		//{
		//	printf("FPS: %f", count);
		//	time = 0;
		//	count = 0;
		//}

		// check and call events
		glfwPollEvents();
		UpdateCamera();

		// Rendering commands here
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw triangle
		lightingShader.Use();

		GLint viewPosLoc = lightingShader.GetUniformLocation("viewPos");
		glUniform3f(viewPosLoc, MainCamera.Position.x, MainCamera.Position.y, MainCamera.Position.z);

		// Directional light
		dirlight.Update();

		// Point lights		
		pointLight1.Update(0);
		pointLight2.Update(1);

		// SpotLight
		spotLight.SetPosDir(MainCamera.Position, MainCamera.Front);
		spotLight.Update();

		// Camera/View transformation
		glm::mat4 view;
		view = MainCamera.GetViewMatrix();

		// set the projection matrix (perspective)
		glm::mat4 projectionMatrix;

		// FOV, aspect ratio, near plane, far plane
		projectionMatrix = glm::perspective(glm::radians(MainCamera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, farPlane);

		// specify matrices
		GLint modelLoc = lightingShader.GetUniformLocation("modelMat");

		GLint viewLoc = lightingShader.GetUniformLocation("viewMat");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		GLint projLoc = lightingShader.GetUniformLocation("projectionMat");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));		

		//glm::mat4 model;
		//model = glm::translate(model, glm::vec3(0, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model += glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

		nanosuit.Draw(lightingShader);

		
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	// =======================================================
	//                     CLEANUP
	// =======================================================
	
	glfwTerminate();
	//_CrtDumpMemoryLeaks();
	return 0;
}

void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{

		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	MainCamera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	MainCamera.ProcessMouseScroll(yoffset);
}

void UpdateCamera()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		MainCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		MainCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		MainCamera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		MainCamera.ProcessKeyboard(RIGHT, deltaTime);
}