#pragma once
#include <glad/glad.h>
class Buffers
{
public:
	inline Buffers(float* vertices, GLuint* indices)
	{
		unsigned int VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	inline void use()
	{
		glBindVertexArray(VAO);
	}
	inline void terminate()
	{
		glDeleteVertexArrays(1, &VAO);
	}
	GLuint VAO = 0;
};

