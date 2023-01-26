#pragma once

#include <utility>
#include <algorithm>
#include "PerlinNoise.hpp"
#include <glm/ext/matrix_transform.hpp>

#include "shader.h"
#include "BlockMesh.h"
#include "Chunk.h"
#include "Player.h"


class World {
public:
	World() 
	{	
		// prepare shaders and projection matrix for rendering the world
		worldShader = new Shader("vertex.vs", "fragment.fs");
		worldShader->use();
		glm::mat4 projection = glm::perspective(glm::radians<float>(70.0f), (float)3840.0f / (float)2160.0f, 0.1f, 2000.0f);
		worldShader->setMat4("projection", projection);
	}

	glm::ivec2 GetChunkIndices(int x, int z) {return glm::ivec2((int)glm::floor(x / chunkSize), (int)glm::floor(z / chunkSize));}
	glm::ivec2 GetChunkCoords(int x, int z) {return glm::ivec2(x % chunkSize, z % chunkSize);}

	void Render(Player& player) 
	{
		worldShader->use();

		// view matrix
		glm::mat4 view = player.GetViewMatrix();
		worldShader->setMat4("view", view);

		// generate and render chunk
		glm::ivec2 chunkIndices = GetChunkIndices(player.position.x, player.position.z);

		if (!chunks[chunkIndices]) chunks[chunkIndices] = new Chunk(256*chunkIndices[0], 256 * chunkIndices[1], perlin);

		chunks[chunkIndices]->Render(*worldShader);
	}

	void Update(double dt) {}

	~World() 
	{
		delete worldShader;
	}

private:
	Shader* worldShader;
	int chunkSize = 256;
	const siv::PerlinNoise::seed_type seed = 1233445u;
	const siv::PerlinNoise perlin{ seed };
	std::unordered_map<glm::ivec2, Chunk*> chunks;
};