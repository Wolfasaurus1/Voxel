#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Application.h"
#include "Player.h"
#include "shader.h"
#include "World.h"
#include "GUI.h"


class Game : public Application
{
public:
	void Init() 
	{		
		// set matrices
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians<float>(70.0f), (float)3840.0f / (float)2160.0f, 0.1f, 250.0f);

		// prepare chunk shader
		shader = new Shader("vertex.vs", "fragment.fs");
		shader->use();
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);	

		// other relavant game objects
		player = new Player;
		world = new ChunkManager;
		gui = new GUI;
	}

	void Update(float dt) 
	{		
		player->Update(dt);

		// generate world
		world->GenerateChunksAroundPosition(player->position);

		// mesh world
		world->MeshChunksAroundPosition(player->position);
	}

	void Render() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render world
		shader->use();
		shader->setVec3("lightPos", player->position);

		shader->setMat4("view", player->GetRightViewMatrix());
		world->RenderChunks(player->position, *shader);

		gui->Render();
	}

	void ProcessMouseInput(double xposIn, double yposIn, double dt) 
	{
		player->ProcessMouseMovement(xposIn, yposIn, dt);
	}

	void ProcessMouseButton(int button, int action) 
	{
		if (action == GLFW_PRESS)
			world->PlaceBlock(WATER, player->position);


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
		// 
		gui->HandleMouseScroll(-yoffset);
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

	ChunkManager* world;

	GUI* gui;
};


// ideas
// basic fluid mechanics: 
// pretty basic rules
// look at adjacent blocks and attempt to spread downward, essentially
// the world will need to tick
// cause now we'll have stuff going on that's independent of player input


// portals and teleportation could be a fun concept to experiment with. 
// what would the implementation look like?
// we teleport when we


// check block below us
// if it's a jump block, we're thrown into the air


// other kinds of blocks that we can interact with