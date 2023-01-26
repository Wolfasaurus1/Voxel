#pragma once

#include <unordered_map>
#include <PerlinNoise.hpp>
#include "shader.h"
#include "BlockMesh.h"




class Chunk {
public:
	Chunk(int globalX, int globalZ, const siv::PerlinNoise& perlin, int height = 50, int isize = 256) {
		this->globalPos = glm::ivec3(globalX, 0, globalZ);
		this->size = glm::ivec3(isize, height, isize);


		std::vector<std::pair<glm::ivec3, bool>> duh(256 * 256);
		glm::ivec3 pos;
		for (int x = 0; x < isize; x++) {
			for (int z = 0; z < isize; z++) {
				int y = (int)(perlin.octave2D_01(((globalX + x) * 0.008), ((globalZ + z) * 0.008), 4) * height);
				pos.x = x;
				pos.y = y;
				pos.z = z;

				duh.push_back({ pos, 1 });
			}
		}
		blocks.insert(duh.begin(), duh.end());
		mesh = new BlockMesh(blocks);
	}


	bool CoordsValid(int x, int y, int z) {
		return x >= 0 && x < size.x && z >= 0 && z < size.z && y >= 0 && y < size.y;
	}


	bool IsActive(int x, int y, int z) {
		if (CoordsValid(x, y, z))
			return blocks[glm::ivec3(x, y, z)];

		return false;
	}

	void Render(Shader& myShader) {
		mesh->Render(globalPos.x, globalPos.y, globalPos.z, myShader);
	}

	void Update(double dt) {

	}

	~Chunk() {
		delete mesh;
	}


private:
	std::unordered_map<glm::ivec3, bool> blocks;
	BlockMesh* mesh;
	glm::ivec3 globalPos, size;
};