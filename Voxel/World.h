#pragma once
#include <unordered_map>

#include <glm/glm.hpp>
#include <FastNoise/FastNoise.h>

#include "Chunk.h"


class ChunkObserver
{
public:
	virtual void OnChunkChanged(glm::ivec3& dirtyChunkCoords) = 0;
};


class World
{
public:
	World() 
	{
		fnPerlin = FastNoise::New<FastNoise::Perlin>();
	}

	void GenerateChunksAroundPosition(glm::vec3 playerPosition)
	{
		glm::ivec3 chunkKey = glm::floor(playerPosition / 16.f); // where is the current chunk? 

		for (int x = chunkKey.x - generationDistance; x <= chunkKey.x + generationDistance; x++) 
		{
			for (int y = 0; y <=  0; y++) 
			{
				for (int z = chunkKey.z - generationDistance; z <= chunkKey.z + generationDistance; z++) 
				{
					if (!chunks[ivec3(x, y, z)])  // has the chunk object been created?
					{
						chunks[ivec3(x, y, z)] = new Chunk(16 * x, 16 * y, 16 * z); // if not let's create it 
						GenerateChunk(chunks[ivec3(x, y, z)]); // now generate the block data in the chunk
						chunks[ivec3(x, y, z)]->state = GENERATED;
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

	void MeshChunksAroundPosition(glm::vec3 playerPosition)
	{
		glm::ivec3 chunkKey = glm::floor(playerPosition / 16.f);

		for (int x = chunkKey.x - meshingDistance; x <= chunkKey.x + meshingDistance; x++)
		{
			for (int y =  0; y <= 0; y++)
			{
				for (int z = chunkKey.z - meshingDistance; z <= chunkKey.z + meshingDistance; z++)
				{
					if (chunks[ivec3(x, y, z)] && chunks[ivec3(x, y, z)]->state != MESHED)
					{
						chunks[ivec3(x, y, z)]->UpdateMesh();
						chunks[ivec3(x, y, z)]->state = MESHED;
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
			for (int y = 0; y <= 0; y++)
			{
				for (int z = chunkKey.z - meshingDistance; z <= chunkKey.z + meshingDistance; z++)
				{
					chunks[ivec3(x, y, z)]->Render(chunkShader);
				}
			}
		}
	}

	Chunk* GetChunk(ivec3& coords)
	{
		return chunks[coords];
	}

	void addChunkObserver(ChunkObserver* observer) 
	{
		observers.push_back(observer);
	}

	void removeChunkObserver(ChunkObserver* observer) 
	{
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}
	
private:
	void notifyObservers(ivec3& coords) 
	{
		for (ChunkObserver* observer : observers) {
			observer->OnChunkChanged(coords);
		}
	}

	// meshing will need to be less, need to know info from surrounding chunks during meshing
	int generationDistance = 16;
	int meshingDistance = 16;

	std::unordered_map<ivec3, Chunk*> chunks;

	// terrain generation should probably be handled in a whole separate class
	FastNoise::SmartNode<FastNoise::Perlin> fnPerlin;

	std::vector<ChunkObserver*> observers;
};