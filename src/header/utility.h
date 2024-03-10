#pragma once
#include "body.h"

inline void stlToAddBody()
{
	bodies bodySTL;
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