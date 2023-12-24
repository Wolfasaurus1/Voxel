#pragma once
#include <unordered_map>

#include <glm/glm.hpp>
#include <FastNoise/FastNoise.h>

#include "Chunk.h"

#include <glm/gtc/integer.hpp>

class ChunkManager
{
public:
	ChunkManager() 
	{
		fnPerlin = FastNoise::New<FastNoise::Perlin>();
	}

	void GenerateChunksAroundPosition(glm::vec3 playerPosition)
	{
		glm::ivec3 chunkKey = glm::floor(playerPosition / 16.f); // where is the current chunk? 

		for (int x = chunkKey.x - generationDistance; x <= chunkKey.x + generationDistance; x++) 
		{
			for (int y = chunkKey.y - generationDistance; y <= chunkKey.y + generationDistance; y++)
			{
				for (int z = chunkKey.z - generationDistance; z <= chunkKey.z + generationDistance; z++) 
				{
					// create and generate the chunk
					if (!chunks[ivec3(x, y, z)])  // has the chunk object been created?
					{
						chunks[ivec3(x, y, z)] = new Chunk(16 * x, 16 * y, 16 * z); // if not let's create it 
						GenerateChunk3D(chunks[ivec3(x, y, z)]); // now generate the block data in the chunk
					}
				}
			}
		}
	}

	// modify the chunk according to noise function
	void GenerateChunk(Chunk* chunk)
	{
		std::vector<float> noiseOutput(16 * 16);

		// Generate a 16 x 16 area of noise
		fnPerlin->GenUniformGrid2D(noiseOutput.data(), chunk->globalPosition.x, chunk->globalPosition.z, 16, 16, 0.03f, 1337);
		
		int index = 0;

		for (int z = 0; z < 16; z++)
		{
			for (int x = 0; x < 16; x++)
			{
				int y = ((noiseOutput[index++] + 1) / 2.0f) * 15;
				chunk->SetBlock(x, y, z, GRASS);
			}
		}
	}

	void GenerateChunk3D(Chunk* chunk)
	{
		std::vector<float> noiseOutput(16 * 16 * 16);

		// Generate a 16 x 16 area of noise
		fnPerlin->GenUniformGrid3D(noiseOutput.data(), chunk->globalPosition.x, chunk->globalPosition.y, chunk->globalPosition.z, 16, 16, 16, 0.03f, 1337);

		int index = 0;

		for (int z = 0; z < 16; z++)
		{
			for (int y = 0; y < 16; y++) 
			{
				for (int x = 0; x < 16; x++)
				{
					float val = ((noiseOutput[index++] + 1) / 2.0f);

					if (val < 0.4)
						chunk->SetBlock(x, y, z, GRASS);
				}
			}
		}
	}


	void MeshChunksAroundPosition(glm::vec3 playerPosition)
	{
		glm::ivec3 chunkKey = glm::floor(playerPosition / 16.f);

		for (int x = chunkKey.x - meshingDistance; x <= chunkKey.x + meshingDistance; x++)
		{
			for (int y = chunkKey.y - meshingDistance; y <= chunkKey.y + meshingDistance; y++)
			{
				for (int z = chunkKey.z - meshingDistance; z <= chunkKey.z + meshingDistance; z++)
				{
					if (chunks[ivec3(x, y, z)] && chunks[ivec3(x, y, z)]->meshIsOutdated())
					{
						chunks[ivec3(x, y, z)]->UpdateMesh();
					}
				}
			}
		}
	}

	void RenderChunks(glm::vec3 playerPosition, Shader &chunkShader)
	{
		glm::ivec3 chunkKey = glm::floor(playerPosition / 16.f);

		for (int x = chunkKey.x - meshingDistance; x <= chunkKey.x + meshingDistance; x++)
		{
			for (int y = chunkKey.y - meshingDistance; y <= chunkKey.y + meshingDistance; y++)
			{
				for (int z = chunkKey.z - meshingDistance; z <= chunkKey.z + meshingDistance; z++)
				{
					chunks[ivec3(x, y, z)]->Render(chunkShader);
				}
			}
		}
	}

	int floorDivide(int value, int divisor)
	{
		if (value >= 0)
			return value / divisor;
		else
			return (value - divisor + 1) / divisor;
	}

	void PlaceBlock(BlockType blockType, ivec3 position)
	{
		// First, get which chunk it is in
		ivec3 chunkKey;
		chunkKey.x = floorDivide(position.x, 16);
		chunkKey.y = floorDivide(position.y, 16);
		chunkKey.z = floorDivide(position.z, 16);

		ivec3 relativePosition;
		relativePosition.x = position.x - (chunkKey.x * 16);
		relativePosition.y = position.y - (chunkKey.y * 16);
		relativePosition.z = position.z - (chunkKey.z * 16);

		chunks[chunkKey]->SetBlock(relativePosition.x, relativePosition.y, relativePosition.z, blockType);
		chunks[chunkKey]->UpdateMesh();
	}

	Chunk* GetChunk(ivec3& coords)
	{
		return chunks[coords];
	}
	
private:

	// meshing will need to be less, need to know info from surrounding chunks during meshing
	int generationDistance = 5;
	int meshingDistance = 5;

	std::unordered_map<ivec3, Chunk*> chunks;

	// terrain generation should probably be handled in a whole separate class
	FastNoise::SmartNode<FastNoise::Perlin> fnPerlin;
};