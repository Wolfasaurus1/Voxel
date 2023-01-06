#pragma once

#include <algorithm>
#include "PerlinNoise.hpp"
#include "shader.h"
#include <glm/ext/matrix_transform.hpp>

#include "BlockMesh.h"

#include "Chunk.h"



class World {
public:
	World() {
		chunks.resize(2, std::vector<Chunk*>(2));
		
	}

	glm::vec2 GetChunkIndices(int x, int z) {
		return glm::vec2((int)glm::floor(x / chunkSize), (int)glm::floor(z / chunkSize));
	}

	glm::vec2 GetChunkCoords(int x, int z) {
		return glm::vec2(x % chunkSize, z % chunkSize);
	}

	int GetElevation(int x, int z) {
		glm::vec2 chunkIndices = GetChunkIndices(x, z);
		glm::vec2 chunkCoords = GetChunkCoords(x, z);
		return chunks[chunkIndices[0]][chunkIndices[1]]->GetElevation(chunkCoords[0], chunkCoords[1]);
	}

	void PlaceBlock(glm::vec3& position) {
		//glm::vec2 chunkIndices = GetChunkIndices(x, z);
		//glm::vec2 chunkCoords = GetChunkCoords(x, z);
		//chunks[chunkIndices[0]][chunkIndices[1]]->PlaceBlock()
	}

	void Render() {
		for (auto& row : chunks)
			for (auto& chunk : row)
				chunk->Render(*worldShader);
	}

	void Update(double dt) {
		for (auto& row : chunks)
			for (auto& chunk : row)
				chunk->Update(dt);
	}

private:
	Shader* worldShader;
	std::vector <std::vector<Chunk*>> chunks;
	int chunkSize = 30;
	const siv::PerlinNoise::seed_type seed = 1233445u;
	const siv::PerlinNoise perlin{ seed };
};