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
	addTriangle(Cube.inds, 0, 1, 2);
	addTriangle(Cube.inds, 4, 5, 6);
	addTriangle(Cube.inds, 4, 5, 7);
	addTriangle(Cube.inds, 1, 3, 7);
	addTriangle(Cube.inds, 1, 3, 5);
	addTriangle(Cube.inds, 0, 2, 4);
	addTriangle(Cube.inds, 0, 2, 6);
	addTriangle(Cube.inds, 0, 1, 4);
	addTriangle(Cube.inds, 0, 1, 5);
	addTriangle(Cube.inds, 2, 3, 6);
	addTriangle(Cube.inds, 2, 3, 7);
	BodyList.first.push_back(Cube);
	BodyList.second.push_back("Cube");
}

void bodies::addPyramid()
{
}

void bodies::addSphere()
{
}

void bodies::generateSTL(int BodyIndex)
{
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
		file.write(reinterpret_cast<char*>(&BodyList.second[i]), 40);
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
	std::ifstream file(openExplorerDialog().c_str(), std::ios::binary);

	uint16_t bodyCount;
	file.read(reinterpret_cast<char*>(&bodyCount), 2);
	for (int i = 0; i < bodyCount; i++)
	{ 
		BodyList.second.push_back(""); 
		BodyList.first.push_back(NULL); 
	}

	uint32_t vertexCount;
	uint32_t indiceCount;
	Body body(0);


	for (short i = 0; i < bodyCount; i++)
	{
		file.read(reinterpret_cast<char*>(&BodyList.second[i]), 40);

		file.read(reinterpret_cast<char*>(&vertexCount), 4);
		for (int j = 0; j < vertexCount * 6; j++)
		{
			body.verts.push_back(0.0f);
		}

		for (uint32_t j = 0; j < vertexCount * 6; j++)
		{
			file.read(reinterpret_cast<char*>(&body.verts[j]), 4);
		}

		file.read(reinterpret_cast<char*>(&indiceCount), 4);
		for (int j = 0; j < indiceCount * 3; j++)
		{
			body.inds.push_back(0);
		}

		for (uint32_t j = 0; j < indiceCount * 3; j++)
		{
			file.read(reinterpret_cast<char*>(&body.inds[j]), 4);
		}
		BodyList.first[i] = 0;
		body.inds.clear();
		body.verts.clear();
	}
	file.close();
}
