#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Application.h"
#include "Player.h"
#include "shader.h"
#include "Line.h"
#include "World.h"


class Game : public Application
{
public:
	void Init() 
	{		
		// set matrices
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians<float>(70.0f), (float)3840.0f / (float)2160.0f, 0.1f, 2000.0f);

		// prepare line shader
		lineShader = new Shader("line.vs", "line.fs");
		lineShader->use();
		lineShader->setMat4("projection", projection);
		lineShader->setMat4("view", view);

		// prepare chunk shader
		shader = new Shader("vertex.vs", "fragment.fs");
		shader->use();
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);	

		// other relavant game objects
		player = new Player;
		line = new Line;
		chunkManager = new ChunkManager;
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
	*/

	// where will this function be used? 
	// for block placement, world editing

	// world editing will probably need its own class
	// because it will have state.. 
	void rayTrace(vector<vec3>& positions) {
		glm::vec3 origin = player->position;
		glm::vec3 ray = glm::vec3(0.0f, 0.0f, -1.0f) * player->orientation;
		glm::vec3 voxel = glm::round(origin);
		glm::vec3 step = glm::sign(ray);

		glm::vec3 tMax = glm::vec3(voxel + (step / 2.0f) - origin) / ray;
		glm::vec3 tDelta = step / ray;

		int i = 0;
		while(i < 15) {
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
			i += 1;
			positions.push_back(voxel);
		}
	}



	void Update(float dt) 
	{		
		player->Update(dt);

		// generate world
		chunkManager->GenerateChunksAroundPosition(player->position);

		//mesh world
		chunkManager->MeshChunksAroundPosition(player->position);
	}

	void Render() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render world
		shader->use();
		shader->setMat4("view", player->GetViewMatrix());
		shader->setVec3("lightPos", player->position);

		chunkManager->RenderChunks(player->position, *shader);
		

		//// render ray tracing
		//lineShader->use();
		//lineShader->setMat4("view", player->GetViewMatrix());

		//vector<vec3> positions;
		//rayTrace(positions);

		//for (const vec3& position : positions) {
		//	lineShader->setMat4("model", glm::translate(glm::mat4(1.0f), position));
		//	line->Render(*lineShader);
		//}
	}

	void ProcessMouseInput(double xposIn, double yposIn) 
	{
		player->ProcessMouseMovement(xposIn, yposIn);
	}

	void ProcessMouseButton(int button, int action) {
		if (action == GLFW_PRESS)
			mouseButtonPressed[button] = true;
		if (action == GLFW_RELEASE)
			mouseButtonPressed[button] = false;
	}

	void ProcessKeyEvent(int key, int action, double dt)
	{		
		player->ProcessKeyEvent(key, action, dt);

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void ProcessScroll(double xoffset, double yoffset)
	{
		
	}

	~Game()
	{
		delete shader;
		delete player;
	}

private:
	Shader* shader, *lineShader;
	Player* player;

	std::unordered_map<int, bool> mouseButtonPressed;

	Line* line;

	ChunkManager* chunkManager;
};