#pragma once
#include "body.h"

inline void stlToAddBody()
{
	bodies bodySTL;
	bodySTL.readSTL();
	std::cout << "Body body(0);\n";
	for (int i = 0; i < bodySTL.BodyList.first[0].verts.size() / 6; i += 6)
	{
		std::cout << "addVertex( body.verts," << bodySTL.BodyList.first[0].verts[i] << "," << bodySTL.BodyList.first[0].verts[i + 1] << "," << bodySTL.BodyList.first[0].verts[i + 2] << " );\n";
	}
	for (int i = 0; i < bodySTL.BodyList.first[0].inds.size() / 3; i += 3)
	{
		std::cout << "addTriangle(body.inds," << bodySTL.BodyList.first[0].inds[i] << "," << bodySTL.BodyList.first[0].inds[i + 1] << "," << bodySTL.BodyList.first[0].inds[i + 2] << " );\n";
	}
	std::cout << "BodyList.first.push_back(body);\nBodyList.second.push_back(\"Body\");";
}
