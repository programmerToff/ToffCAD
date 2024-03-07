#include "body.h"



int main()
{
	bodies bodies;
	bodies.addSphere(1.0f, 20, 20);
	bodies.generateSTL(0);
}