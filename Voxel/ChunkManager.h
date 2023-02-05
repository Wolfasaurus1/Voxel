#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <unordered_map>
#include <glm/glm.hpp>

#include "Chunk.h"


using namespace glm;

class ChunkManager
{
public:
	ChunkManager(int distance = 2): distance(distance) 
	{
		chunks[ivec3(0, 0, 0)] = new Chunk(0, 0, 0, perlin);
	}
	Chunk* GetChunk(ivec3 pos) {	return chunks[pos];		}
	
	void GenerateAroundPosition(glm::ivec3 pos)
	{
		for (int x = pos.x - distance; x <= pos.x + distance; x++) {
			for (int y = pos.y - distance; y <= pos.y + distance; y++) {
				for (int z = pos.z - distance; z <= pos.z + distance; z++) {
					if (!GetChunk(ivec3(x, y, z))) {
						chunks[ivec3(x, y, z)] = new Chunk(16 * x, 16 * y, 16 * z, perlin);
					}
				}
			}
		}
	}

private:
	int distance;
	std::unordered_map<glm::ivec3, Chunk*> chunks;

	// for generating terrain
	const siv::PerlinNoise::seed_type seed = 1233445u;
	const siv::PerlinNoise perlin{ seed };
};