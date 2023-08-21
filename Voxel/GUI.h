#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "shader.h"


class GUI
{
public:
	GUI() {
		//init the shader
		shader = new Shader("gui.vs", "gui.fs");
		shader->use();

		// init orthographic projection matrix
		glm::mat4 projection = glm::ortho(0.0f, 3840.0f, 0.0f, 2160.0f, -0.5f, 0.5f);

		// set the related uniform
		shader->setMat4("projection", projection);

		float quadVertices[] = {
			0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,

			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, quadVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void Render() {
		shader->use();

		// bind the render state
		glBindVertexArray(VAO);

		for (int i = 0; i < 10; i++)
		{
			float offset = 0.0f;
			if (i == selectedBlock)
				offset = 30.0f;

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(3840.0f / 2.0f + (i * 200) - (200 * 5 - 100), 200.0f + offset, 0.0f));
			model = glm::scale(model, glm::vec3(120.0f, 120.0f, 0.0f));

			shader->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	void HandleMouseScroll(double yoffset) {
		int newSelection = selectedBlock + (int)yoffset;
		if (newSelection >= 0 && newSelection < 10)
			selectedBlock = newSelection;
	}

	int GetSelectedBlock() {
		return 0;
	}

private:
	Shader* shader;
	unsigned int VAO;
	int selectedBlock = 0;
};