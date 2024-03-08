#include "body.h"



int main()
{
	bodies bodies;
	bodies.addPyramid();
	PropertiesTriangle p = bodies.getTriangleProperties(0, 0);
	bodies.printPropertiesTriangle(p);
}