#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>


class Application
{
public:
	Application();

	void Run();

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual void ProcessMouseInput(double xposIn, double yposIn) = 0;
	virtual void ProcessMouseButton(int button, int action) = 0;
	virtual void ProcessScroll(double xoffset, double yoffset) = 0;
	virtual void ProcessKeyEvent(int key, int action, double deltaTime) = 0;

	double deltaTime = 0.0f;
protected:
	GLFWwindow* window;
	int SCR_WIDTH = 3840, SCR_HEIGHT = 2160;
};