#pragma once

#include <iostream>

//GL
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

//STL
#include <vector>

//other headers
#include "shader.h"


class Mesh
{
public:
	Mesh(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
	{
		numTriangles = indices.size();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		// put vertex data in buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

		// put indices in buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// color attribute
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	// update the vertices and indices of the mesh
	void Update(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
	{
		numTriangles = indices.size();

		glBindVertexArray(VAO); // might not need this

		// put vertex data in buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

		// put indices in buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

		glBindVertexArray(0); // might not need this
	}

	void Draw(int x, int y, int z, Shader& shader)
	{
		shader.use();
		glBindVertexArray(VAO);

		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		//shader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, 0);
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

private:
	unsigned int VAO, VBO, EBO;
	int numTriangles;
};


class ChunkRenderer
{
public:

	// allows us to keep track of when a chunk changes
	void OnChunkChanged()
	{

	}

	// remesh any chunks that have changed and update the VBO
	void Update()
	{

	}

	void Render()
	{
		// loop through and render all the chunk meshes
	}

private:
	std::vector<int> chunksChanged; // maybe make this a queue so that we update them in the order that they were changed
};