#pragma once

#include <algorithm>
#include "PerlinNoise.hpp"
#include "shader.h"
#include <glm/ext/matrix_transform.hpp>



class BlockMesh {
public:

	BlockMesh(std::vector<std::vector<std::vector<int>>> blocks) {
		//build the mesh
		for (int x = 0; x < blocks.size(); x++) {
			for (int y = 0; y < blocks[x].size(); y++) {
				for (int z = 0; z < blocks[x][y].size(); z++) {
					if (blocks[x][y][z]) {
						addBlockMesh(x, y, z);
					}
				}
			}
		}
		SetupMesh();
	}
	
	void Render(int x, int y, int z, Shader& myShader) {
		myShader.use();

		glBindVertexArray(VAO);
	
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		myShader.setMat4("model", model);
		myShader.setVec3("color", glm::vec3(19.0f / 255.0f, 133.0f / 255.0f, 16.0f / 255.0f));

		glDrawArrays(GL_TRIANGLES, 0, (int)(vertices.size() / 4));
	}

private:
	//prepares the mesh for rendering
	void SetupMesh() {
		unsigned int VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void addBlockMesh(float x, float y, float z) {
		std::vector<float> blockVertices{
			-0.5f + x, -0.5f + y, -0.5f + z,  0.0f,  0.0f, -1.0f,
			 0.5f + x, -0.5f + y, -0.5f + z,  0.0f,  0.0f, -1.0f,
			 0.5f + x,  0.5f + y, -0.5f + z,  0.0f,  0.0f, -1.0f,
			 0.5f + x,  0.5f + y, -0.5f + z,  0.0f,  0.0f, -1.0f,
			-0.5f + x,  0.5f + y, -0.5f + z,  0.0f,  0.0f, -1.0f,
			-0.5f + x, -0.5f + y, -0.5f + z,  0.0f,  0.0f, -1.0f,
											 
			-0.5f + x, -0.5f + y,  0.5f + z,  0.0f,  0.0f,  1.0f,
			 0.5f + x, -0.5f + y,  0.5f + z,  0.0f,  0.0f,  1.0f,
			 0.5f + x,  0.5f + y,  0.5f + z,  0.0f,  0.0f,  1.0f,
			 0.5f + x,  0.5f + y,  0.5f + z,  0.0f,  0.0f,  1.0f,
			-0.5f + x,  0.5f + y,  0.5f + z,  0.0f,  0.0f,  1.0f,
			-0.5f + x, -0.5f + y,  0.5f + z,  0.0f,  0.0f,  1.0f,
											 
			-0.5f + x,  0.5f + y,  0.5f + z, -1.0f,  0.0f,  0.0f,
			-0.5f + x,  0.5f + y, -0.5f + z, -1.0f,  0.0f,  0.0f,
			-0.5f + x, -0.5f + y, -0.5f + z, -1.0f,  0.0f,  0.0f,
			-0.5f + x, -0.5f + y, -0.5f + z, -1.0f,  0.0f,  0.0f,
			-0.5f + x, -0.5f + y,  0.5f + z, -1.0f,  0.0f,  0.0f,
			-0.5f + x,  0.5f + y,  0.5f + z, -1.0f,  0.0f,  0.0f,
											 
			 0.5f + x,  0.5f + y,  0.5f + z,  1.0f,  0.0f,  0.0f,
			 0.5f + x,  0.5f + y, -0.5f + z,  1.0f,  0.0f,  0.0f,
			 0.5f + x, -0.5f + y, -0.5f + z,  1.0f,  0.0f,  0.0f,
			 0.5f + x, -0.5f + y, -0.5f + z,  1.0f,  0.0f,  0.0f,
			 0.5f + x, -0.5f + y,  0.5f + z,  1.0f,  0.0f,  0.0f,
			 0.5f + x,  0.5f + y,  0.5f + z,  1.0f,  0.0f,  0.0f,
											 
			-0.5f + x, -0.5f + y, -0.5f + z,  0.0f, -1.0f,  0.0f,
			 0.5f + x, -0.5f + y, -0.5f + z,  0.0f, -1.0f,  0.0f,
			 0.5f + x, -0.5f + y,  0.5f + z,  0.0f, -1.0f,  0.0f,
			 0.5f + x, -0.5f + y,  0.5f + z,  0.0f, -1.0f,  0.0f,
			-0.5f + x, -0.5f + y,  0.5f + z,  0.0f, -1.0f,  0.0f,
			-0.5f + x, -0.5f + y, -0.5f + z,  0.0f, -1.0f,  0.0f,
											 
			-0.5f + x,  0.5f + y, -0.5f + z,  0.0f,  1.0f,  0.0f,
			 0.5f + x,  0.5f + y, -0.5f + z,  0.0f,  1.0f,  0.0f,
			 0.5f + x,  0.5f + y,  0.5f + z,  0.0f,  1.0f,  0.0f,
			 0.5f + x,  0.5f + y,  0.5f + z,  0.0f,  1.0f,  0.0f,
			-0.5f + x,  0.5f + y,  0.5f + z,  0.0f,  1.0f,  0.0f,
			-0.5f + x,  0.5f + y, -0.5f + z,  0.0f,  1.0f,  0.0f
		};

		vertices.insert(vertices.end(), blockVertices.begin(), blockVertices.end());
	}

	std::vector<float> vertices;
	unsigned int VAO;
};




class Chunk {
public:
	Chunk(int globalX, int globalZ, const siv::PerlinNoise& perlin) {
		this->globalX = globalX;
		this->globalZ = globalZ;

		//GENERATE THE TERRAIN AND STORE IN BLOCKS vector

		//init the grid with 0s
		blocks.resize(size, std::vector<std::vector<int>>(100, std::vector<int>(size, 0)));

		//get elevations
		elevations.resize(size, std::vector<int>(size, 0));
		for (int x = 0; x < size; x++) {
			for (int z = 0; z < size; z++) {
				float y = (int)(perlin.octave2D_01(((globalX + x) * 0.005), ((globalZ + z) * 0.005), 4) * 39);
				elevations[x][z] = y;
				blocks[x][y][z] = 1; //update the grid
			}
		}

		//create the mesh to be rendered
		mesh = new BlockMesh(blocks);
	}

	//returns whether block is there or not
	bool IsActive(int x, int y, int z) {
		if (x >= 0 && x < size && z >= 0 && z < size && y >= 0 && y < 100)
			return blocks[x][y][z];

		return false;
	}

	void Render(Shader& myShader) {
		mesh->Render(globalX, 0, globalZ, myShader);
	}

	void PlaceBlock(int x, int y, int z) {
		if (x >= 0 && x < size && z >= 0 && z < size && y >= 0 && y < 100) {
			blocks[x][y][z] = 1;

			delete mesh;
			mesh = new BlockMesh(blocks);
		}
	}

	void PlaceBlocks(std::vector<glm::vec3> positions) 
	{
		for (const glm::vec3 &position : positions) 
		{
			float x = position.x;
			float y = position.y;
			float z = position.z;

			if (x >= 0 && x < size && z >= 0 && z < size && y >= 0 && y < 100) {
				blocks[x][y][z] = 1;
			}
		}
		delete mesh;
		mesh = new BlockMesh(blocks);
	}

	void DestroyBlock(int x, int y, int z) {
		//if a block is there, destroy it
		//then rebuild the mesh and send to GPU
		if (x >= 0 && x < size && z >= 0 && z < size && y >= 0 && y < 40) {
			blocks[x][y][z] = 0;
			//rebuild mesh
			delete mesh;
			mesh = new BlockMesh(blocks);
		}
	}

	void Update(double dt) {
		//in the future, there will be ongoing processes that will require updates
	}

	~Chunk() {
		delete mesh;
	}


	std::vector<std::vector<int>> elevations;
private:
	std::vector<std::vector<std::vector<int>>> blocks;
	BlockMesh* mesh;
	int globalX, globalZ;
	int size = 200;
};



