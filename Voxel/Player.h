#pragma once


#include "camera.h"

class Player
{
public:
	Player() {
		camera = new Camera;
	}

	void Update(double dt)
	{
		camera->update();
	}

	void ProcessKeyEvent(int key, int action, double dt)
	{
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
			camera->ProcessKeyboard(FORWARD, dt);
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
			camera->ProcessKeyboard(BACKWARD, dt);
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
			camera->ProcessKeyboard(LEFT, dt);
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
			camera->ProcessKeyboard(RIGHT, dt);

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			camera->ProcessKeyboard(UP, dt);
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
			camera->ProcessKeyboard(DOWN, dt);
	}

	void ProcessMouseMovement(double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera->ProcessMouseMovement(xoffset, yoffset);
	}


	Camera* camera;

private:
	// for keeping track of mouse movements
	bool firstMouse = true;
	float lastX = 3840.0f / 2.0f;
	float lastY = 2160.0f / 2.0f;
};