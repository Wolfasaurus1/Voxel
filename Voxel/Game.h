#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Application.h"
#include "Player.h"
#include "shader.h"
#include "Renderer.h"
#include "Chunk.h"

#include <unordered_set>
#include <queue>


class Game : public Application
{
public:
	void Init() 
	{		
		// prepare shaders
		shader = new Shader("vertex.vs", "fragment.fs");
		player = new Player;

		// set matrices
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians<float>(70.0f), (float)3840.0f / (float)2160.0f, 0.1f, 2000.0f);
		
		shader->use();
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
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
		glm::vec3 voxel = glm::round(origin) + ray * 20.f;
		glm::vec3 step = glm::sign(ray);

		glm::vec3 tMax = glm::vec3(voxel + (step / 2.0f) - origin) / ray;
		glm::vec3 tDelta = step / ray;

		int i = 0;
		while(i < 400) {
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


	// BREAK THIS UP? 
	// updates chunks, meshes according to what blocks are placed (ONLY IN CHUNKS ALREADY LOADED)
	void PlaceBlocks(vector<vec3>& positions, BlockType type)
	{		
		unordered_set<ivec3> meshLocationsToUpdate;

		// update the chunk(s) with the blocks
		for (const vec3& position : positions) 
		{
			ivec3 relativePosition = mod(glm::round(position), vec3(16));
			ivec3 chunkLocation = floor(round(position) / 16.f);

			if (chunks[chunkLocation] && chunks[chunkLocation]->GetBlock(relativePosition.x, relativePosition.y, relativePosition.z) == EMPTY) // if the chunk is loaded
			{
				chunks[chunkLocation]->SetBlock(relativePosition.x, relativePosition.y, relativePosition.z, type);
				meshLocationsToUpdate.insert(chunkLocation); // keep track of what meshes we need to update
			}
		}

		// update the meshes with the new blocks
		for (const ivec3& iter: meshLocationsToUpdate)
		{
			std::vector<GLfloat> vertices;
			std::vector<GLuint> indices;
			MeshChunk(chunks[iter], glm::vec3(16 * iter.x, 16 * iter.y, 16 * iter.z), vertices, indices);

			if (meshes[iter])
				meshes[iter]->Update(vertices, indices);
			else
				meshes[iter] = new Mesh(vertices, indices);
		}
	}


	void Update(float dt) 
	{		
		player->Update(dt);

		glm::ivec3 pos = glm::floor(player->position / 16.f);

		// generate nearby chunks
		for (int x = pos.x - distance; x <= pos.x + distance; x++) {
			for (int y = pos.y - distance; y <= pos.y + distance; y++) {
				for (int z = pos.z - distance; z <= pos.z + distance; z++) {
					if (!chunks[ivec3(x, y, z)]) {
						chunks[ivec3(x, y, z)] = new Chunk(16 * x, 16 * y, 16 * z, perlin);

						std::vector<GLfloat> vertices;
						std::vector<GLuint> indices;
						MeshChunk(chunks[ivec3(x, y, z)], glm::vec3(16 * x, 16 * y, 16 * z), vertices, indices);
						meshes[ivec3(x, y, z)] = new Mesh(vertices, indices);
					}
				}
			}
		}

		if (mouseButtonPressed[GLFW_MOUSE_BUTTON_LEFT]) {
			vector<vec3> positions;
			rayTrace(positions);
			PlaceBlocks(positions, DIRT);
		}

		if (mouseButtonPressed[GLFW_MOUSE_BUTTON_RIGHT]) {
			vector<vec3> positions;
			rayTrace(positions);
			PlaceBlocks(positions, WATER);
		}
	}

	void Render() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->use();
		shader->setMat4("view", player->GetViewMatrix());
		shader->setVec3("lightPos", player->position);

		glm::ivec3 pos = glm::floor(player->position / 16.f);

		// render nearby chunks
		for (int x = pos.x - distance; x <= pos.x + distance; x++) 
			for (int y = pos.y - distance; y <= pos.y + distance; y++) 
				for (int z = pos.z - distance; z <= pos.z + distance; z++) 
					if (meshes[ivec3(x, y, z)])
						meshes[ivec3(x, y, z)]->Draw(0, 0, 0, *shader);
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
	const int distance = 7;

	std::unordered_map<ivec3, Chunk*> chunks;
	std::unordered_map<ivec3, Mesh*> meshes;

	Shader* shader;
	Player* player;

	// for generating terrain
	const siv::PerlinNoise::seed_type seed = 1233445u;
	const siv::PerlinNoise perlin{ seed };

	std::unordered_map<int, bool> mouseButtonPressed;
};


// the world state is passed to the graphics engine. 
// the grapchis engine handles the meshes. 
// it can handle their updates and everything.. 


// in update, what do we really want to do
// all we want to do is add blocks to the world. shouldn't be that complicated
// the world will implement how exactly that happens

// we should not be updating the world and meshes in the same place
// these things should be done in separate places

// this is mixing game logic and render logic and other logic.. 

// we could have a worldrenderer to be more specific !! 


// world editing
// how to highlight a region? 
// this is a graphical thing since we're not actually changing the state of the world

// we create a SelectionRenderer! 
// for different types of selection