#pragma once
#include <string>
#include <fstream>
#include <cerrno>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class shaderClass
{
public:
	GLuint shaderProgram;
	shaderClass(std::string vertexShaderSource, std::string fragmentShaderSource);
	inline void Activate()
	{
		glUseProgram(shaderProgram);
	}
	inline void Delete()
	{
		glDeleteProgram(shaderProgram);
	}
};

std::string get_file_contents(const char* filename);