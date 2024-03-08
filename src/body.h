#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <shlobj.h>
#include <string>
#include <fstream>
#include <vector>
#include <set>

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

struct PropertiesBody
{
	uint32_t triangleCount;
	uint32_t verticesCount;
	float center[3];
};

struct PropertiesTriangle
{
	float p1[6];
	float p2[6];
	float p3[6];
	float center[3];
};


class bodies
{
public:
	std::pair<std::vector<Body>, std::vector<std::string>> BodyList;

	void addCube();
	void addPyramid();
	void addSphere();
	void test();

	void saveTCAD();
	void openTCAD();
	void generateSTL(int bodyIndex);
	void readSTL();

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
	inline void printPropertiesBody(PropertiesBody p)
	{
		std::cout << "Vertices: " << p.verticesCount << " Triangles: " << p.triangleCount << " Center: " << p.center[0] << " " << p.center[1] << " " << p.center[2];

	}
	inline void printPropertiesTriangle(PropertiesTriangle p)
	{
		std::cout << "P1( " << p.p1[0] << "," << p.p1[1] << "," << p.p1[2] << " )" << "\nP2( " << p.p2[0] << "," << p.p2[1] << "," << p.p2[2] << " )" << "\nP3( " << p.p3[0] << "," << p.p3[1] << "," << p.p3[2] << " )" << "\nCenter( " << p.center[0] << "," << p.center[1] << "," << p.center[2] << " )";
	}
	inline PropertiesBody getBodyProperties(int index)
	{
		PropertiesBody out;
		out.triangleCount = BodyList.first[index].inds.size() / 3;
		out.verticesCount = BodyList.first[index].verts.size() / 6;
		float center[3] = { 0,0,0 };
		for (int i = 0; i < out.verticesCount * 6; i += 6)
		{
			center[0] += BodyList.first[index].verts[i + 0];
			center[1] += BodyList.first[index].verts[i + 1];
			center[2] += BodyList.first[index].verts[i + 2];
		}
		center[0] /= out.verticesCount;
		center[1] /= out.verticesCount;
		center[2] /= out.verticesCount;
		out.center[0] = center[0];
		out.center[1] = center[1];
		out.center[2] = center[2];
		return out;
	}
	inline PropertiesTriangle getTriangleProperties(int indexBody, int indexTriangle)
	{
		PropertiesTriangle out;

		float p1[3];
		float p2[3];
		float p3[3];
		float center[3];

		p1[0] = BodyList.first[indexBody].verts[6 * BodyList.first[indexBody].inds[indexTriangle * 3 + 0] + 0];
		p1[1] = BodyList.first[indexBody].verts[6 * BodyList.first[indexBody].inds[indexTriangle * 3 + 0] + 1];
		p1[2] = BodyList.first[indexBody].verts[6 * BodyList.first[indexBody].inds[indexTriangle * 3 + 0] + 2];

		p2[0] = BodyList.first[indexBody].verts[6 * BodyList.first[indexBody].inds[indexTriangle * 3 + 1] + 0];
		p2[1] = BodyList.first[indexBody].verts[6 * BodyList.first[indexBody].inds[indexTriangle * 3 + 1] + 1];
		p2[2] = BodyList.first[indexBody].verts[6 * BodyList.first[indexBody].inds[indexTriangle * 3 + 1] + 2];

		p3[0] = BodyList.first[indexBody].verts[6 * BodyList.first[indexBody].inds[indexTriangle * 3 + 2] + 0];
		p3[1] = BodyList.first[indexBody].verts[6 * BodyList.first[indexBody].inds[indexTriangle * 3 + 2] + 1];
		p3[2] = BodyList.first[indexBody].verts[6 * BodyList.first[indexBody].inds[indexTriangle * 3 + 2] + 2];

		center[0] = (p1[0] + p2[0] + p3[0]) / 3;
		center[1] = (p1[1] + p2[1] + p3[1]) / 3;
		center[2] = (p1[2] + p2[2] + p3[2]) / 3;

		out.p1[0] = p1[0];
		out.p1[1] = p1[1];
		out.p1[2] = p1[2];

		out.p2[0] = p2[0];
		out.p2[1] = p2[1];
		out.p2[2] = p2[2];

		out.p3[0] = p3[0];
		out.p3[1] = p3[1];
		out.p3[2] = p3[2];

		out.center[0] = center[0];
		out.center[1] = center[1];
		out.center[2] = center[2];

		return out;
	}
	
};

std::string saveFileDialog();
std::string openExplorerDialog();