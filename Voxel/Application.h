#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

struct KeyboardEvent 
{
	int key;
	int action;
};


struct CursorMovementEvent 
{
	double xpos, ypos;
};


class Application
{
public:
	Application();
	void OnCursorPosEvent(double xpos, double ypos);
	void mouse_button_callback(int button, int action);
	void ScrollCallBack(double xoffset, double yoffset);
	void Run();

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual void ProcessInput(float dt) = 0;
	virtual void ProcessMouseInput(double xposIn, double yposIn) = 0;
	virtual void ProcessMouseButton(int button, int action) = 0;
	virtual void ProcessScroll(double xoffset, double yoffset) = 0;

protected:
	GLFWwindow* window;
	int SCR_WIDTH = 3840, SCR_HEIGHT = 2160;
};