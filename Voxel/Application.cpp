#include "Application.h"

#include <iostream>
#include <functional>
#include <Windows.h>

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

	window = glfwCreateWindow(mode->width, mode->height, "Voxel Engine", primary, NULL);

	glfwSetWindowUserPointer(this->window, this);

	/*
	glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Application* dataPtr = static_cast <Application*> (glfwGetWindowUserPointer(window));
		dataPtr->OnKeyEvent(key, action);
	});*/

	glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double xpos, double ypos)
	{
		Application* dataPtr = static_cast <Application*> (glfwGetWindowUserPointer(window));
		dataPtr->OnCursorPosEvent(xpos, ypos);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) 
	{
		Application* dataPtr = static_cast <Application*> (glfwGetWindowUserPointer(window));
		dataPtr->mouse_button_callback(button, action);
	});
	
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
}



void Application::OnCursorPosEvent(double xpos, double ypos)
{
	this->ProcessMouseInput(xpos, ypos);
}


void Application::mouse_button_callback(int button, int action) 
{
	this->ProcessMouseButton(button, action);
}


void Application::Run() 
{
	Init();

	float lastTime = glfwGetTime();
	glfwSwapInterval(0);
	
	glClearColor(0.62, 0.871, 0.969, 1.0f);

	while (!glfwWindowShouldClose(window))
	{	
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents();
		ProcessInput(deltaTime);
		Update(deltaTime);
		Render();
		glfwSwapBuffers(window);
	}
}