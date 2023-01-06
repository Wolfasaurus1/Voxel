#pragma once

#include <algorithm>
#include "PerlinNoise.hpp"
#include <glm/ext/matrix_transform.hpp>

#include "shader.h"
#include "BlockMesh.h"
#include "Chunk.h"
#include "camera.h"


class World {
public:
	World() 
	{
		chunks.resize(2, std::vector<Chunk*>(2));
		
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				chunks[i][j] = new Chunk(256 * i, 256 * j, perlin);
			}
		}

		worldShader = new Shader("vertex.vs", "fragment.fs");
		worldShader->use();

		glm::mat4 projection = glm::perspective(glm::radians<float>(70.0f), (float)3840.0f / (float)2160.0f, 0.1f, 2000.0f);
		worldShader->setMat4("projection", projection);
	}

	glm::vec2 GetChunkIndices(int x, int z) 
	{
		return glm::vec2((int)glm::floor(x / chunkSize), (int)glm::floor(z / chunkSize));
	}

	glm::vec2 GetChunkCoords(int x, int z) 
	{
		return glm::vec2(x % chunkSize, z % chunkSize);
	}

	int GetElevation(int x, int z) 
	{
		glm::vec2 chunkIndices = GetChunkIndices(x, z);
		glm::vec2 chunkCoords = GetChunkCoords(x, z);
		return chunks[chunkIndices[0]][chunkIndices[1]]->GetElevation(chunkCoords[0], chunkCoords[1]);
	}

	void PlaceBlock(glm::vec3& position) 
	{
		//glm::vec2 chunkIndices = GetChunkIndices(x, z);
		//glm::vec2 chunkCoords = GetChunkCoords(x, z);
		//chunks[chunkIndices[0]][chunkIndices[1]]->PlaceBlock()
	}

	void Render(Camera* camera) 
	{
		worldShader->use();

		//view matrix
		glm::mat4 view = camera->GetViewMatrix();
		worldShader->setMat4("view", view);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				chunks[i][j]->Render(*worldShader);
			}
		}
	}

	void Update(double dt) {}

private:
	Shader* worldShader;
	std::vector <std::vector<Chunk*>> chunks;
	int chunkSize = 30;
	const siv::PerlinNoise::seed_type seed = 1233445u;
	const siv::PerlinNoise perlin{ seed };
};