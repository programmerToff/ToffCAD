#include "body.h"



int main()
{
	bodies bodies;
	bodies.addCube();
	bodies.addCube();
	bodies.saveTCAD();
	bodies.openTCAD();
	bodies.printInfo();
}