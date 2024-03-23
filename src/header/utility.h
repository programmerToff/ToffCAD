#pragma once
#include "body.h"

inline void stlToAddBody()
{
	bodies bodySTL(0, 0);
	bodySTL.readSTL();
	bodySTL.optimizeVertexCount(0);
	std::ofstream output(saveFileDialog());
	output << "Body body(0);\n";
	for (int i = 0; i < bodySTL.BodyList.first[0].verts.size(); i += 6)
	{
		output << "addVertex( body.verts," << bodySTL.BodyList.first[0].verts[i] << "," << bodySTL.BodyList.first[0].verts[i + 1] << "," << bodySTL.BodyList.first[0].verts[i + 2] << " );\n";
	}
	for (int i = 0; i < bodySTL.BodyList.first[0].inds.size(); i += 3)
	{
		output << "addTriangle(body.inds," << bodySTL.BodyList.first[0].inds[i] << "," << bodySTL.BodyList.first[0].inds[i + 1] << "," << bodySTL.BodyList.first[0].inds[i + 2] << " );\n";
	}
	output << "return body;";
}

inline void createDefaultTCC()
{
	std::ofstream file("default.tcc", std::ios::binary);
	bool loadingScreen = false;
	float bgColor[3] = { 0.08,0.51,0.83 };

	char buffer;
	if (loadingScreen) {
		buffer = 255;
	}
	else
	{
		buffer = 0;
	}
	file.write(reinterpret_cast<char*>(&buffer), 1);
	file.write(reinterpret_cast<char*>(&bgColor), 12);






	file.close();
}