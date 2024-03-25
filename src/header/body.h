#pragma once
#pragma warning( disable: 6031 )
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <shlobj.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "add.h"

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

inline std::string saveFileDialog() {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	IFileSaveDialog* pFileSave;

	if (SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave)))) {
		if (SUCCEEDED(pFileSave->Show(NULL))) {
			IShellItem* pItem;
			if (SUCCEEDED(pFileSave->GetResult(&pItem))) {
				PWSTR pszFilePath;
				if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
					std::wstring wsFilePath(pszFilePath);
					CoTaskMemFree(pszFilePath);
					pItem->Release();
					pFileSave->Release();
					CoUninitialize();
					std::string filePath(wsFilePath.begin(), wsFilePath.end());
					return filePath;
				}
				pItem->Release();
			}
		}
		pFileSave->Release();
	}
	CoUninitialize();

	return "";
}
inline void getNormal(float normal[3], float p1[3], float p2[3], float p3[3])
{
	float v1[3] = { p2[0] - p1[0],p2[1] - p1[1],p2[2] - p1[2] };
	float v2[3] = { p3[0] - p1[0],p3[1] - p1[1],p3[2] - p1[2] };
	normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
	normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
	normal[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return;
}
inline std::string openExplorerDialog() {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	IFileOpenDialog* pFileOpen;

	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)))) {
		if (SUCCEEDED(pFileOpen->Show(NULL))) {
			IShellItem* pItem;
			if (SUCCEEDED(pFileOpen->GetResult(&pItem))) {
				PWSTR pszFilePath;
				if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
					int bufferSize = WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, nullptr, 0, nullptr, nullptr);
					std::string filePath(bufferSize, '\0');
					WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, &filePath[0], bufferSize, nullptr, nullptr);

					CoTaskMemFree(pszFilePath);
					pItem->Release();
					pFileOpen->Release();
					CoUninitialize();
					return filePath;
				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}
	CoUninitialize();

	return "";
}


class bodies
{
public:
	std::pair<std::vector<Body>, std::vector<std::string>> BodyList;
	float* vertices;
	GLuint* indices;
	GLuint vertSize;
	GLuint indsSize;

	inline bodies(float i, GLuint g)
	{
		vertices = &i;
		indices = &g;
		vertSize = 0;
		indsSize = 0;
	}

	inline void addCube()
	{
		BodyList.first.push_back(genCube());
		BodyList.second.push_back("Cube");
	}
	inline void addPyramid()
	{
		BodyList.first.push_back(genPyramid());
		BodyList.second.push_back("Pyramid");
	}
	inline void addSphere()
	{
		BodyList.first.push_back(genSphere());
		BodyList.second.push_back("Sphere");
	}
	inline void addSSM()
	{
		BodyList.first.push_back(genSSM());
		BodyList.second.push_back("Vacuum Monster");
	}
	inline void addTriangle()
	{
		BodyList.first.push_back(genTriangle());
		BodyList.second.push_back("Triangle");
	}

	inline void saveTCAD(std::string filepath)
	{
		std::ofstream file(filepath, std::ios::binary);

		int i = 0;
		uint16_t bodyCount = BodyList.first.size();
		uint32_t vertexCount;
		uint32_t indicesCount;

		file.write(reinterpret_cast<char*>(&bodyCount), 2);

		for (Body& body : BodyList.first)
		{
			BodyList.second[i].resize(40, '\0');
			file.write(BodyList.second[i].c_str(), 40);
			vertexCount = body.verts.size() / 6;
			indicesCount = body.inds.size() / 3;
			file.write(reinterpret_cast<char*>(&vertexCount), 4);
			for (unsigned int j = 0; j < vertexCount * 6; j++)
			{
				file.write(reinterpret_cast<char*>(&body.verts[j]), 4);
			}
			file.write(reinterpret_cast<char*>(&indicesCount), 4);
			for (unsigned int j = 0; j < indicesCount * 3; j++)
			{
				file.write(reinterpret_cast<char*>(&body.inds[j]), 4);
			}
			i++;
		}

		file.close();
	}
	inline void openTCAD(std::string filepath)
	{
		BodyList.first.clear();
		BodyList.second.clear();

		Body nullBody;
		float vertex;
		GLuint indice;

		std::ifstream file(filepath, std::ios::binary);
		if (!file.is_open()) {
			std::cerr << "Error opening file: " << filepath << std::endl;
			return;
		}

		uint16_t bodyCount;
		file.read(reinterpret_cast<char*>(&bodyCount), sizeof(uint16_t));

		for (short currentBodyId = 0; currentBodyId < bodyCount; ++currentBodyId)
		{
			char name[41];
			file.read(name, 40);
			name[40] = '\0';
			std::string bodyName(name);

			BodyList.first.push_back(nullBody);
			BodyList.second.push_back(bodyName);

			uint32_t vertexCount;
			file.read(reinterpret_cast<char*>(&vertexCount), sizeof(uint32_t));

			for (uint32_t i = 0; i < vertexCount * 6; ++i)
			{
				file.read(reinterpret_cast<char*>(&vertex), 4);
				BodyList.first[currentBodyId].verts.push_back(vertex);
			}

			uint32_t indiceCount;
			file.read(reinterpret_cast<char*>(&indiceCount), sizeof(uint32_t));

			for (uint32_t i = 0; i < indiceCount * 3; ++i)
			{
				file.read(reinterpret_cast<char*>(&indice), sizeof(GLuint));
				BodyList.first[currentBodyId].inds.push_back(indice);
			}
		}

		file.close();
	}
	inline void generateSTL(int bodyIndex, std::string filepath)
	{
		const char* header = "ToffCAD generated this STL-script. https://github.com/programmerToff/ToffCAD   ";
		uint16_t attribByteCount = 0;
		uint32_t triangleCount = BodyList.first[bodyIndex].inds.size() / 3;

		std::ofstream file(filepath, std::ios::binary);

		file.write(header, 80);
		file.write(reinterpret_cast<char*>(&triangleCount), 4);

		float normal[3];
		float p1[3];
		float p2[3];
		float p3[3];

		for (int i = 0; i < triangleCount; i++)
		{
			p1[0] = BodyList.first[bodyIndex].verts[BodyList.first[bodyIndex].inds[i * 3 + 0] * 6 + 0];
			p1[1] = BodyList.first[bodyIndex].verts[BodyList.first[bodyIndex].inds[i * 3 + 0] * 6 + 1];
			p1[2] = BodyList.first[bodyIndex].verts[BodyList.first[bodyIndex].inds[i * 3 + 0] * 6 + 2];

			p2[0] = BodyList.first[bodyIndex].verts[BodyList.first[bodyIndex].inds[i * 3 + 1] * 6 + 0];
			p2[1] = BodyList.first[bodyIndex].verts[BodyList.first[bodyIndex].inds[i * 3 + 1] * 6 + 1];
			p2[2] = BodyList.first[bodyIndex].verts[BodyList.first[bodyIndex].inds[i * 3 + 1] * 6 + 2];

			p3[0] = BodyList.first[bodyIndex].verts[BodyList.first[bodyIndex].inds[i * 3 + 2] * 6 + 0];
			p3[1] = BodyList.first[bodyIndex].verts[BodyList.first[bodyIndex].inds[i * 3 + 2] * 6 + 1];
			p3[2] = BodyList.first[bodyIndex].verts[BodyList.first[bodyIndex].inds[i * 3 + 2] * 6 + 2];

			getNormal(normal, p1, p2, p3);

			file.write(reinterpret_cast<char*>(&normal), 12);
			file.write(reinterpret_cast<char*>(&p1), 12);
			file.write(reinterpret_cast<char*>(&p2), 12);
			file.write(reinterpret_cast<char*>(&p3), 12);
			file.write(reinterpret_cast<char*>(&attribByteCount), 2);
		}
		file.close();
	}
	inline void readSTL(std::string filepath)
	{
		std::ifstream file(filepath, std::ios::binary);

		Body body;
		float coord;

		file.seekg(80, std::ios::beg);

		uint32_t triangleCount = 0;
		file.read(reinterpret_cast<char*>(&triangleCount), 4);
		for (int i = 0; i < triangleCount; i++)
		{
			file.seekg(12, std::ios::cur);
			file.read(reinterpret_cast<char*>(&coord), 4);
			body.verts.push_back(coord);
			file.read(reinterpret_cast<char*>(&coord), 4);
			body.verts.push_back(coord);
			file.read(reinterpret_cast<char*>(&coord), 4);
			body.verts.push_back(coord);
			body.verts.push_back(0.0f);
			body.verts.push_back(0.4f);
			body.verts.push_back(0.0f);
			file.read(reinterpret_cast<char*>(&coord), 4);
			body.verts.push_back(coord);
			file.read(reinterpret_cast<char*>(&coord), 4);
			body.verts.push_back(coord);
			file.read(reinterpret_cast<char*>(&coord), 4);
			body.verts.push_back(coord);
			body.verts.push_back(0.0f);
			body.verts.push_back(0.4f);
			body.verts.push_back(0.0f);
			file.read(reinterpret_cast<char*>(&coord), 4);
			body.verts.push_back(coord);
			file.read(reinterpret_cast<char*>(&coord), 4);
			body.verts.push_back(coord);
			file.read(reinterpret_cast<char*>(&coord), 4);
			body.verts.push_back(coord);
			body.verts.push_back(0.0f);
			body.verts.push_back(0.4f);
			body.verts.push_back(0.0f);
			file.seekg(2, std::ios::cur);
		}
		for (int i = 0; i < triangleCount * 3; i++)
		{
			body.inds.push_back(i);
		}
		BodyList.first.push_back(body);
		BodyList.second.push_back("ImportedSTL");
		file.close();
	}

	inline void printFinalInfo()
	{
		std::cout << "Vertices: \n";
		float* verts = vertices;
		for (int i = 0; i < vertSize; i += 6)
		{
			std::cout << *verts << ", ";
			verts++;
			std::cout << *verts << ", ";
			verts++;
			std::cout << *verts << ", ";
			verts++;
			std::cout << *verts << ", ";
			verts++;
			std::cout << *verts << ", ";
			verts++;
			std::cout << *verts << "\n";
			verts++;
		}

		std::cout << "Indices: \n";
		GLuint* inds = indices;
		for (int i = 0; i < indsSize; i+=3)
		{
			std::cout << *inds << ", ";
			inds++;
			std::cout << *inds << ", ";
			inds++;
			std::cout << *inds << "\n";
			inds++;
		}
	}
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

	inline void optimizeVertexCount(int index)
	{
		Body& body = BodyList.first[index];
		struct Vertex
		{
			float x, y, z, r, g, b;
		};
		std::vector<Vertex> uniqueVertices;
		std::map<Vertex, GLuint,
			bool(*)(const Vertex&, const Vertex&)> vertexMap(
				[](const Vertex& lhs, const Vertex& rhs) -> bool {
					if (lhs.x != rhs.x) return lhs.x < rhs.x;
					if (lhs.y != rhs.y) return lhs.y < rhs.y;
					if (lhs.z != rhs.z) return lhs.z < rhs.z;
					if (lhs.r != rhs.r) return lhs.r < rhs.r;
					if (lhs.g != rhs.g) return lhs.g < rhs.g;
					return lhs.b < rhs.b;
				});

		for (GLuint i = 0; i < body.inds.size(); ++i) {
			GLuint index = body.inds[i];
			Vertex vertex{
				body.verts[index * 6], body.verts[index * 6 + 1], body.verts[index * 6 + 2],
				body.verts[index * 6 + 3], body.verts[index * 6 + 4], body.verts[index * 6 + 5]
			};

			auto it = vertexMap.find(vertex);
			if (it == vertexMap.end()) {
				GLuint newIndex = static_cast<GLuint>(uniqueVertices.size());
				vertexMap[vertex] = newIndex;
				body.inds[i] = newIndex;
				uniqueVertices.push_back(vertex);
			}
			else {
				body.inds[i] = it->second;
			}
		}

		// Copy unique vertices back to the original vector
		body.verts.clear();
		for (const auto& vertex : uniqueVertices) {
			body.verts.push_back(vertex.x);
			body.verts.push_back(vertex.y);
			body.verts.push_back(vertex.z);
			body.verts.push_back(vertex.r);
			body.verts.push_back(vertex.g);
			body.verts.push_back(vertex.b);
		}
	}

	inline void update()
	{
		if (BodyList.first.size() == 0) { return; }

		size_t vertS = 0;
		size_t indS = 0;
		for (int i = 0; i < BodyList.first.size(); i++)
		{
			vertS += BodyList.first[i].verts.size();
			indS += BodyList.first[i].inds.size();
		}

		vertices = new float[vertS];
		indices = new GLuint[indS];
		vertSize = vertS;
		indsSize = indS;

		size_t offset = 0;
		for (const auto& Body : BodyList.first) {
			std::transform(Body.inds.begin(), Body.inds.end(), indices + offset, [offset](GLuint v) { return v + offset; });
			offset += Body.inds.size();
		}

		for (const auto& Body : BodyList.first)
		{
			std::copy(Body.verts.begin(), Body.verts.end(), vertices);
		}
		return;
	}
};