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

	inline void printInfo()
	{
		int i = 0;
		for (int j = 0; j < BodyList.second.size(); j++)
		{
			std::cout << BodyList.second[j] << ":\n";
			std::cout << "\nVertices:\n";
			for (auto& itr : BodyList.first[j].verts)
			{
				std::cout << itr << " ";
				if (i < 5) { i++; }
				else { i = 0; std::cout << '\n'; }
			}
			std::cout << "\nIndices:\n";
			i = 0;
			for (auto& itr : BodyList.first[j].inds)
			{
				std::cout << itr << " ";
				if (i < 2) { i++; }
				else { i = 0; std::cout << '\n'; }
			}
		}
	}
};

std::string saveFileDialog();
std::string openExplorerDialog();