#pragma once

#include <PerlinNoise.hpp>
#include "shader.h"
#include "BlockMesh.h"


class Chunk {
public:
	Chunk(int globalX, int globalZ, const siv::PerlinNoise& perlin, int height = 50, int size = 256) {
		this->globalX = globalX;
		this->globalZ = globalZ;
		this->height = height;
		this->size = size;

		blocks.resize(size, std::vector<std::vector<int>>(height, std::vector<int>(size, 0)));

		elevations.resize(size, std::vector<int>(size, 0));
		for (int x = 0; x < size; x++) {
			for (int z = 0; z < size; z++) {
				float y = (int)(perlin.octave2D_01(((globalX + x) * 0.008), ((globalZ + z) * 0.008), 4) * height);
				elevations[x][z] = y;
				blocks[x][y][z] = 1;
			}
		}

		mesh = new BlockMesh(blocks);
	}


	bool CoordsValid(int x, int y, int z) {
		return x >= 0 && x < size&& z >= 0 && z < size&& y >= 0 && y < height;
	}


	bool IsActive(int x, int y, int z) {
		if (CoordsValid(x, y, z))
			return blocks[x][y][z];

		return false;
	}


	void Render(Shader& myShader) {
		mesh->Render(globalX, 0, globalZ, myShader);
	}


	void PlaceBlock(int x, int y, int z) {
		if (CoordsValid(x, y, z)) {
			blocks[x][y][z] = 1;

			delete mesh;
			mesh = new BlockMesh(blocks);
		}
	}


	void PlaceBlocks(std::vector<glm::vec3> positions)
	{
		for (const glm::vec3& position : positions)
		{
			float x = position.x;
			float y = position.y;
			float z = position.z;

			if (CoordsValid(x, y, z)) {
				blocks[x][y][z] = 1;
			}
		}
		delete mesh;
		mesh = new BlockMesh(blocks);
	}


	void DestroyBlock(int x, int y, int z) {
		if (CoordsValid(x, y, z)) {
			blocks[x][y][z] = 0;
			//rebuild mesh
			delete mesh;
			mesh = new BlockMesh(blocks);
		}
	}


	void Update(double dt) {

	}


	int GetElevation(int x, int z) {
		if (x >= 0 && x < size && z >= 0 && z < size)
			return elevations[x][z];
		else
			return 0;
	}


	~Chunk() {
		delete mesh;
	}


private:
	std::vector<std::vector<std::vector<int>>> blocks;
	std::vector<std::vector<int>> elevations;
	BlockMesh* mesh;
	int globalX, globalZ;
	int height;
	int size;
};

