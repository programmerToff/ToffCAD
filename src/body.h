#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <shlobj.h>
#include <string>
#include <fstream>
#include <vector>

const float PI = 3.14159265358979323846f;

struct Body
{
	std::vector<float> verts;
	std::vector<GLuint> inds;
	float* vertices;
	GLuint* indices;

	inline Body(int null)
	{
		verts = { (float)null };
		inds = { (GLuint)null };
		vertices = &verts[0];
		indices = &inds[0];
		verts.pop_back();
		inds.pop_back();
	}
};


class bodies
{
public:
	std::pair<std::vector<Body>, std::vector<std::string>> BodyList;

	void addCube();
	void addPyramid();
	void addSphere();

	void saveTCAD();
	void openTCAD();
	void generateSTL(int BodyIndex);
};

