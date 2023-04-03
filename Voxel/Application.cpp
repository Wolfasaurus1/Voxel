#include "Application.h"

#include <iostream>
#include <functional>
#include <Windows.h>
#include <chrono>

Application::Application() 
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* primary = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(primary);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(3840, 2160, "Voxel Engine", primary, NULL);

	glfwSetWindowUserPointer(this->window, this);

	glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Application* dataPtr = static_cast <Application*> (glfwGetWindowUserPointer(window));
		dataPtr->ProcessKeyEvent(key, action, dataPtr->deltaTime);
	});

	glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double xpos, double ypos)
	{
		Application* dataPtr = static_cast <Application*> (glfwGetWindowUserPointer(window));
		dataPtr->ProcessMouseInput(xpos, ypos);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) 
	{
		Application* dataPtr = static_cast <Application*> (glfwGetWindowUserPointer(window));
		dataPtr->ProcessMouseButton(button, action);
	});

	glfwSetScrollCallback(this->window, [](GLFWwindow* window, double xoffset, double yoffset)
	{
		Application* dataPtr = static_cast <Application*> (glfwGetWindowUserPointer(window));
		dataPtr->ProcessScroll(xoffset, yoffset);
	});
	
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width / 2, height);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::Run() 
{
	Init();

	double lastTime = glfwGetTime();
	glfwSwapInterval(1);
	
	glClearColor(0.62, 0.871, 0.969, 1.0f);

	while (!glfwWindowShouldClose(window))
	{	
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//std::cout << 1.0f / deltaTime << std::endl;

		glfwPollEvents();
		Update(deltaTime);
		Render();
		glfwSwapBuffers(window);
		glFinish();
	}
}