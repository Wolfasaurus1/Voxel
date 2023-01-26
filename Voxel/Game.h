#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Application.h"
#include "World.h"
#include "GUI.h"
#include "Player.h"



class Game : public Application
{
public:
	void Init() 
	{
		world = new World();
		gui = new GUI();
		player = new Player();
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
		player->Update(dt);
		world->Update(dt);
	}

	void Render() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		world->Render(*player);
		gui->Render();
	}

	void ProcessMouseInput(double xposIn, double yposIn) 
	{
		player->ProcessMouseMovement(xposIn, yposIn);
	}

	void ProcessMouseButton(int button, int action) {}

	void ProcessKeyEvent(int key, int action, double dt)
	{		
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		player->ProcessKeyEvent(key, action, dt);
	}

	void ProcessScroll(double xoffset, double yoffset)
	{
		gui->HandleMouseScroll(-yoffset);
	}

	~Game()
	{
		delete world;
		delete gui;
		delete player;
	}

private:
	GUI* gui;
	World* world;
	Player* player;
};