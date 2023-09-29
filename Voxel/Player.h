#pragma once

#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#include <unordered_map>

#include "Chunk.h"

class Player
{
public:
	Player() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		velocity = glm::vec3(0.0, 0.0, 0.0);
		acceleration = glm::vec3(0.0, -80.0, 0.0);
		orientation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
	}

	void Update(float dt)
	{
		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f) * orientation;
		float speed = 30.0f;

		if (keysPressed[GLFW_KEY_W])
			position += glm::normalize(cameraDirection * glm::vec3(1.0f, 0.0f, 1.0f)) * dt * speed;
		if (keysPressed[GLFW_KEY_S])
			position -= glm::normalize(cameraDirection * glm::vec3(1.0f, 0.0f, 1.0f)) * dt * speed;
		if (keysPressed[GLFW_KEY_A])
			position += glm::normalize(glm::vec3(cameraDirection.z, 0, -cameraDirection.x)) * dt * speed;
		if (keysPressed[GLFW_KEY_D])
			position -= glm::normalize(glm::vec3(cameraDirection.z, 0, -cameraDirection.x)) * dt * speed;

		if (keysPressed[GLFW_KEY_SPACE])
			position.y += dt * speed;
		if (keysPressed[GLFW_KEY_LEFT_SHIFT])
			position.y -= dt * speed;

		if (keysPressed[GLFW_KEY_UP])
			separation += 0.025f;
		if (keysPressed[GLFW_KEY_DOWN])
			separation -= 0.025f;
	}

	// key presses result in changes in position! 
	void ProcessKeyEvent(int key, int action, double dt)
	{
		if (action == GLFW_PRESS)
			keysPressed[key] = true;
		if (action == GLFW_RELEASE)
			keysPressed[key] = false;
	}

	// mouse movement changes the orientation of the player
	void ProcessMouseMovement(double xposIn, double yposIn, float dt)
	{
		double xpos = static_cast<double>(xposIn);
		double ypos = static_cast<double>(yposIn);
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		double xoffset = xpos - lastX;
		double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		float pitch = -yoffset * 0.12f;
		float yaw = xoffset * 0.12f;

		glm::quat pitchQuat = glm::angleAxis(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat yawQuat = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));

		orientation = glm::normalize(pitchQuat * dt) * orientation * glm::normalize(yawQuat * dt);
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::mat4_cast(orientation) * glm::translate(glm::mat4(1.0f), -position);
	}

	glm::mat4 GetLeftViewMatrix()
	{
		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f) * orientation;
		return glm::mat4_cast(orientation) * glm::translate(glm::mat4(1.0f), -glm::vec3(position.x, position.y, position.z) - glm::normalize(glm::vec3(cameraDirection.z, 0, -cameraDirection.x)) * separation);
	}

	glm::mat4 GetRightViewMatrix()
	{
		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f) * orientation;
		return glm::mat4_cast(orientation) * glm::translate(glm::mat4(1.0f), -glm::vec3(position.x, position.y, position.z) + glm::normalize(glm::vec3(cameraDirection.z, 0, -cameraDirection.x)) * separation);
	}

	glm::vec3 position, velocity, acceleration;
	glm::quat orientation;
private:
	std::unordered_map<int, bool> keysPressed;

	bool firstMouse = true;
	float lastX = 3840.0f / 2.0f;
	float lastY = 2160.0f / 2.0f;

	float separation = 0.0f;
};