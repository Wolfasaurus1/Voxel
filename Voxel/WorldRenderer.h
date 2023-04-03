#pragma once

#include "Mesh.h"
#include "World.h"


class WorldRenderer : public ChunkObserver
{
public:
	// allows us to keep track of when a chunk changes
	void OnChunkChanged(glm::ivec3& dirtyChunkCoords)
	{
		dirtyChunks.push(dirtyChunkCoords);
	}

	// remesh any chunks that have changed and update the VBO
	void Update(World& world)
	{
		while (dirtyChunks.empty())
		{
			glm::ivec3 dirtyChunkCoords = dirtyChunks.front();
			dirtyChunks.pop();

			// mesh the chunk
			vector<GLfloat> vertices;
			vector<GLuint> indices;
			// typically the minimum sizes needed with current terrain generation
			vertices.reserve(10000);
			indices.reserve(1000);

			Chunk* dirtyChunk = world.GetChunk(dirtyChunkCoords);
			MeshChunk(dirtyChunk, dirtyChunk->globalPosition, vertices, indices);

			if (chunkMeshes[dirtyChunkCoords]) {
				chunkMeshes[dirtyChunkCoords]->Update(vertices, indices);
			}
			else {
				chunkMeshes[dirtyChunkCoords] = new Mesh(vertices, indices);
			}
		}
	}

	// perhaps not. we can set the uniform from elsewhere
	void RenderChunks(glm::vec3& playerPosition, Shader& chunkShader)
	{
		glm::ivec3 chunkKey = glm::floor(playerPosition / 16.f);

		for (int x = chunkKey.x - renderDistance; x <= chunkKey.x + renderDistance; x++)
		{
			for (int y = 0; y <= 0; y++)
			{
				for (int z = chunkKey.z - renderDistance; z <= chunkKey.z + renderDistance; z++)
				{
					chunkMeshes[glm::ivec3(x, y, z)]->Draw(0, 0, 0, chunkShader);
				}
			}
		}
	}

private:
	std::queue<glm::ivec3> dirtyChunks;
	std::unordered_map<glm::ivec3, Mesh*> chunkMeshes;
	int renderDistance = 16;
};