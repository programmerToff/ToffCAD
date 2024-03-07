#include "body.h"

std::string saveFileDialog() {
	// Use Windows API to open Save As dialog
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	IFileSaveDialog* pFileSave;

	// Create the FileSaveDialog object
	if (SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave)))) {
		// Show the dialog
		if (SUCCEEDED(pFileSave->Show(NULL))) {
			// Get the result
			IShellItem* pItem;
			if (SUCCEEDED(pFileSave->GetResult(&pItem))) {
				PWSTR pszFilePath;
				// Get the file path
				if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
					std::wstring wsFilePath(pszFilePath);
					CoTaskMemFree(pszFilePath);
					pItem->Release();
					pFileSave->Release();
					CoUninitialize();

					// Convert wide string to narrow string
					std::string filePath(wsFilePath.begin(), wsFilePath.end());
					return filePath;
				}
				pItem->Release();
			}
		}
		pFileSave->Release();
	}
	CoUninitialize();

	return "";  // Return an empty string if the user cancels the dialog or an error occurs
}

void getNormal(float normal[3], float p1[3], float p2[3], float p3[3])
{
	float v1[3] = { p2[0] - p1[0],p2[1] - p1[1],p2[2] - p1[2] };
	float v2[3] = { p3[0] - p1[0],p3[1] - p1[1],p3[2] - p1[2] };
	normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
	normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
	normal[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return;
}

void addVertex(std::vector<float>& v, float x, float y, float z)
{
	v.push_back(x);
	v.push_back(y);
	v.push_back(z);
	v.push_back(0.0f);
	v.push_back(0.4f);
	v.push_back(0.0f);
}

void addTriangle(std::vector<GLuint>& v, GLuint p1, GLuint p2, GLuint p3)
{
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
}

std::string openExplorerDialog() {
	// Use Windows API to open Explorer dialog
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	IFileOpenDialog* pFileOpen;

	// Create the FileOpenDialog object
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)))) {
		// Show the dialog
		if (SUCCEEDED(pFileOpen->Show(NULL))) {
			// Get the result
			IShellItem* pItem;
			if (SUCCEEDED(pFileOpen->GetResult(&pItem))) {
				PWSTR pszFilePath;
				// Get the file path
				if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
					// Convert wide string to narrow string
					int bufferSize = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, nullptr, 0, nullptr, nullptr);
					std::string filePath(bufferSize, '\0');
					WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &filePath[0], bufferSize, nullptr, nullptr);

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

void bodies::addCube()
{
	Body Cube(0);
	addVertex(Cube.verts, 1.0f, 0.0f, 1.0f);
	addVertex(Cube.verts, 1.0f, 0.0f, 0.0f);
	addVertex(Cube.verts, 0.0f, 0.0f, 1.0f);
	addVertex(Cube.verts, 0.0f, 0.0f, 0.0f);
	addVertex(Cube.verts, 1.0f, 1.0f, 1.0f);
	addVertex(Cube.verts, 1.0f, 1.0f, 0.0f);
	addVertex(Cube.verts, 0.0f, 1.0f, 1.0f);
	addVertex(Cube.verts, 0.0f, 1.0f, 0.0f);
	addTriangle(Cube.inds, 0, 1, 3);
	addTriangle(Cube.inds, 0, 2, 3);
	addTriangle(Cube.inds, 4, 5, 7);
	addTriangle(Cube.inds, 4, 6, 7);
	addTriangle(Cube.inds, 1, 3, 7);
	addTriangle(Cube.inds, 1, 5, 7);
	addTriangle(Cube.inds, 0, 2, 6);
	addTriangle(Cube.inds, 0, 4, 6);
	addTriangle(Cube.inds, 3, 6, 7);
	addTriangle(Cube.inds, 2, 3, 6);
	addTriangle(Cube.inds, 0, 1, 5);
	addTriangle(Cube.inds, 0, 4, 5);
	BodyList.first.push_back(Cube);
	BodyList.second.push_back("Cube");
}

void bodies::addPyramid()
{
	Body pyramid(0);
	addVertex(pyramid.verts, 0, 0, 0);
	addVertex(pyramid.verts, 1, 1, 0);
	addVertex(pyramid.verts, 0, 1, 0);
	addVertex(pyramid.verts, 1, 0, 0);
	addVertex(pyramid.verts, 0.5, 0.5, 1);
	addTriangle(pyramid.inds, 0, 1, 2);
	addTriangle(pyramid.inds, 0, 1, 3);
	addTriangle(pyramid.inds, 0, 2, 4);
	addTriangle(pyramid.inds, 0, 3, 4);
	addTriangle(pyramid.inds, 1, 2, 4);
	addTriangle(pyramid.inds, 1, 3, 4);
	BodyList.first.push_back(pyramid);
	BodyList.second.push_back("Pyramid");
}

void bodies::addSphere()
{
	Body body(0);
	


	BodyList.first.push_back(body);
	BodyList.second.push_back("Sphere");
}

void bodies::saveTCAD()
{
	std::ofstream file(saveFileDialog().c_str(), std::ios::binary);

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
		for (unsigned int j = 0; j < vertexCount * 6; j ++)
		{
			file.write(reinterpret_cast<char*>(&body.verts[j]), 4);
		}
		file.write(reinterpret_cast<char*>(&indicesCount), 4);
		for (unsigned int j = 0; j < indicesCount * 3; j ++)
		{
			file.write(reinterpret_cast<char*>(&body.inds[j]), 4);
		}
		i++;
	}

	file.close();
}

void bodies::openTCAD()
{
	BodyList.first.clear();
	BodyList.second.clear();

	std::string filepath = openExplorerDialog();
	Body nullBody(0);
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

		for (uint32_t i = 0; i < vertexCount*6; ++i)
		{
			file.read(reinterpret_cast<char*>(&vertex), 4);
			BodyList.first[currentBodyId].verts.push_back(vertex);
		}

		uint32_t indiceCount;
		file.read(reinterpret_cast<char*>(&indiceCount), sizeof(uint32_t));

		for (uint32_t i = 0; i < indiceCount*3; ++i)
		{
			file.read(reinterpret_cast<char*>(&indice), sizeof(GLuint));
			BodyList.first[currentBodyId].inds.push_back(indice);
		}
	}

	file.close();
}

void bodies::generateSTL(int bodyIndex)
{
	const char* header = "ToffCAD generated this STL-script. https://github.com/programmerToff/ToffCAD   ";
	uint16_t attribByteCount = 0;
	uint32_t triangleCount = BodyList.first[bodyIndex].inds.size() / 3;

	std::ofstream file(saveFileDialog(), std::ios::binary);

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

void bodies::readSTL()
{
	std::ifstream file(openExplorerDialog(), std::ios::binary);
	Body body(0);
	float coord;

	file.seekg(80, std::ios::beg);

	uint32_t triangleCount;
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

void optimizeBodyData(Body& body)
{
	
}