#pragma once
#include "Application.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "PerlinNoise.hpp"
#include "world.h"
#include "camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/quaternion.hpp>

#include "GUI.h"



class Game : public Application
{
public:
	void Init() 
	{
		myShader = new Shader("vertex.vs", "fragment.fs");
		camera = new Camera;

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				chunks.push_back(new Chunk(i * 256, j * 256, perlin));
			}
		}

		gui = new GUI();

		myShader->use();
		//projection matrix
		glm::mat4 projection = glm::perspective(glm::radians<float>(70.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
		myShader->setMat4("projection", projection);
	}

	/*
	glm::vec3 intersection(const glm::vec3 origin, const glm::vec3 ray, const glm::vec3 boxMin, const glm::vec3 boxMax) 
	{
		float tmin = 0.0, tmax = INFINITY;
		int minIndex = -1;

		for (int i = 0; i < 3; ++i) {
			float t1 = (boxMin[i] - origin[i]) / ray[i];
			float t2 = (boxMax[i] - origin[i]) / ray[i];

			if (t1 > t2) {
				std::swap(t1, t2);
			}

			if (t1 > tmin) {
				tmin = t1;
				minIndex = i;
			}
			tmax = glm::min(t2, tmax);
		}

		if (tmin < tmax) {
			glm::vec3 normal(0.0f);
			normal[minIndex] = (ray[minIndex] > 0.0f) ? -1.0f : 1.0f;
			return normal;
		}
		else {
			return glm::vec3(0.0f);
		}
	}


	void rayTrace() {
		glm::vec3 origin = -camera->position;
		glm::vec3 ray = glm::vec3(0.0f, 0.0f, -1.0f) * camera->orientation;
		glm::vec3 voxel = glm::round(origin);
		glm::vec3 step = glm::sign(ray);

		glm::vec3 tMax = glm::vec3(voxel + (step / 2.0f) - origin) / ray;
		glm::vec3 tDelta = step / ray;

		std::vector<glm::vec3> positions;

		while(!chunk->IsActive(voxel.x, voxel.y, voxel.z)) {
			if (tMax.x < tMax.y) {
				if (tMax.x < tMax.z) {
					voxel.x += step.x;
					tMax.x += tDelta.x;
				}
				else {
					voxel.z += step.z;
					tMax.z += tDelta.z;
				}
			}
			else {
				if (tMax.y < tMax.z) {
					voxel.y += step.y;
					tMax.y += tDelta.y;
				}
				else {
					voxel.z += step.z;
					tMax.z += tDelta.z;
				}
			}
			positions.push_back({ voxel.x, voxel.y, voxel.z });
		}
		positions.pop_back();
		chunk->PlaceBlocks(positions);
	}*/


	void Update(float dt) 
	{
		/*
		float newvy = vy + ay * dt;
		float newy = camera->targetPosition.y + (vy * dt) + (ay * dt * dt / 2.0f);

		glm::vec3 pos = glm::round(camera->targetPosition);
		if (newy >= -chunk->elevations[-pos.x][-pos.z] - 1.5f) {
			newvy = -newvy;
		}

		camera->targetPosition.y = newy;
		vy = newvy;
		*/
		camera->update();
	}

	void Render() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader->use();

		//view matrix
		glm::mat4 view = camera->GetViewMatrix();
		myShader->setMat4("view", view);

		for (auto chunk : chunks)
			chunk->Render(*myShader);

		gui->Render();
	}

	//
	void ProcessMouseInput(double xposIn, double yposIn) 
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

	void ProcessMouseButton(int button, int action) 
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !blockPlaced)
		{
			blockPlaced = true;
			//rayTrace();
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
			blockPlaced = false;
	}

	void ProcessInput(float dt) 
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->ProcessKeyboard(FORWARD, dt);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->ProcessKeyboard(BACKWARD, dt);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->ProcessKeyboard(LEFT, dt);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->ProcessKeyboard(RIGHT, dt);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera->ProcessKeyboard(UP, dt);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera->ProcessKeyboard(DOWN, dt);
	}

	void ProcessScroll(double xoffset, double yoffset)
	{
		gui->HandleMouseScroll(yoffset);
	}

private:
	float vy = 0.0f;
	float ay = 40.f;
	bool blockPlaced = false;
	Shader* myShader;
	std::vector<Chunk*> chunks;
	const siv::PerlinNoise::seed_type seed = 55u;
	const siv::PerlinNoise perlin{ seed };
	Camera* camera;
	bool firstMouse = true;
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	GUI* gui;
};