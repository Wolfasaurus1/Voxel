#pragma once

#include <glad/gl.h>
#include <vector>

#include "shader.h"

using namespace std;

class Line
{
public:
	Line()
	{
		std::vector<GLfloat> vertices = {
			-0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f, 
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
		};

		std::vector<GLuint> indices = {
			0, 1,
			1, 2,
			2, 3, 
			3, 0,

			4, 5, 
			5, 6, 
			6, 7,
			7, 4,

			0, 4, 
			1, 5, 
			2, 6,
			3, 7
		};


		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		// put vertex data in buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		// put indices in buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void Render(Shader &lineShader)
	{
		lineShader.use();
		glBindVertexArray(VAO);
		glLineWidth(10);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	}

private:
	unsigned int VAO, VBO, EBO;
};