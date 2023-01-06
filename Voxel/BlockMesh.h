#pragma once

#include <vector>
#include <glad/gl.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

std::vector<float> blockVertices{
	//positions			  //normals
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	// Front face
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	// Left face
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
	// Right face
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
	 // Bottom face      
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 // Top face
	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



struct BlockData {
	glm::vec3 translation;
	glm::vec3 color;
};


class BlockMesh {
public:

	BlockMesh(std::vector<std::vector<std::vector<int>>>& blocks) {
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

		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, blocks.size());
	}

private:
	//prepares the mesh for rendering
	void SetupMesh() {
		// buffer for instance data
		unsigned int instanceVBO;
		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(BlockData) * blocks.size(), &blocks[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		unsigned int VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * blockVertices.size(), blockVertices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// instance attribute
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

		//the following attributes come from the instanceVBO

		//translation
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);

		//color
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// tell OpenGL this is an instanced vertex attribute.
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);

	}

	void addBlockMesh(float x, float y, float z) {
		blocks.push_back({ glm::vec3(x, y, z), glm::vec3(0.0f, (y) / 50.f, 1.f - (y / 52.0f)) });
	}

	std::vector <BlockData> blocks;
	unsigned int VAO;
};



/*

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
			//positions
			-0.5f + x, -0.5f + y, -0.5f + z,  		  0.0f,  0.0f, -1.0f,
			 0.5f + x, -0.5f + y, -0.5f + z,  		  0.0f,  0.0f, -1.0f,
			 0.5f + x,  0.5f + y, -0.5f + z,  		  0.0f,  0.0f, -1.0f,
			 0.5f + x,  0.5f + y, -0.5f + z,  		  0.0f,  0.0f, -1.0f,
			-0.5f + x,  0.5f + y, -0.5f + z,  		  0.0f,  0.0f, -1.0f,
			-0.5f + x, -0.5f + y, -0.5f + z,  		  0.0f,  0.0f, -1.0f,
			// Fr + xont fac + ye	    + z
			-0.5f + x, -0.5f + y,  0.5f + z,  		  0.0f,  0.0f,  1.0f,
			 0.5f + x,  0.5f + y,  0.5f + z,  		  0.0f,  0.0f,  1.0f,
			 0.5f + x, -0.5f + y,  0.5f + z,  		  0.0f,  0.0f,  1.0f,
			 0.5f + x,  0.5f + y,  0.5f + z,  		  0.0f,  0.0f,  1.0f,
			-0.5f + x, -0.5f + y,  0.5f + z,  		  0.0f,  0.0f,  1.0f,
			-0.5f + x,  0.5f + y,  0.5f + z,  		  0.0f,  0.0f,  1.0f,
			// Le + xft face + y	    + z
			-0.5f + x,  0.5f + y,  0.5f + z,  		 -1.0f,  0.0f,  0.0f,
			-0.5f + x, -0.5f + y, -0.5f + z,  		 -1.0f,  0.0f,  0.0f,
			-0.5f + x,  0.5f + y, -0.5f + z,  		 -1.0f,  0.0f,  0.0f,
			-0.5f + x, -0.5f + y, -0.5f + z,  		 -1.0f,  0.0f,  0.0f,
			-0.5f + x,  0.5f + y,  0.5f + z,  		 -1.0f,  0.0f,  0.0f,
			-0.5f + x, -0.5f + y,  0.5f + z,  		 -1.0f,  0.0f,  0.0f,
			// Ri + xght fac + ye	    + z
			 0.5f + x,  0.5f + y,  0.5f + z,  		  1.0f,  0.0f,  0.0f,
			 0.5f + x,  0.5f + y, -0.5f + z,  		  1.0f,  0.0f,  0.0f,
			 0.5f + x, -0.5f + y, -0.5f + z,  		  1.0f,  0.0f,  0.0f,
			 0.5f + x, -0.5f + y, -0.5f + z,  		  1.0f,  0.0f,  0.0f,
			 0.5f + x, -0.5f + y,  0.5f + z,  		  1.0f,  0.0f,  0.0f,
			 0.5f + x,  0.5f + y,  0.5f + z,  		  1.0f,  0.0f,  0.0f,
			 // B + xottom f + yace     + z
			-0.5f + x, -0.5f + y, -0.5f + z, 		  0.0f, -1.0f,  0.0f,
			 0.5f + x, -0.5f + y,  0.5f + z, 		  0.0f, -1.0f,  0.0f,
			 0.5f + x, -0.5f + y, -0.5f + z, 		  0.0f, -1.0f,  0.0f,
			 0.5f + x, -0.5f + y,  0.5f + z, 		  0.0f, -1.0f,  0.0f,
			-0.5f + x, -0.5f + y, -0.5f + z, 		  0.0f, -1.0f,  0.0f,
			-0.5f + x, -0.5f + y,  0.5f + z, 		  0.0f, -1.0f,  0.0f,
			 // T + xop face + y	    + z
			-0.5f + x,  0.5f + y, -0.5f + z, 		  0.0f,  1.0f,  0.0f,
			 0.5f + x,  0.5f + y, -0.5f + z, 		  0.0f,  1.0f,  0.0f,
			 0.5f + x,  0.5f + y,  0.5f + z, 		  0.0f,  1.0f,  0.0f,
			 0.5f + x,  0.5f + y,  0.5f + z, 		  0.0f,  1.0f,  0.0f,
			-0.5f + x,  0.5f + y,  0.5f + z, 		  0.0f,  1.0f,  0.0f,
			-0.5f + x,  0.5f + y, -0.5f + z, 		  0.0f,  1.0f,  0.0f
		};
		vertices.insert(vertices.end(), blockVertices.begin(), blockVertices.end());
	}

	std::vector<float> vertices;
	unsigned int VAO;
};

*/

