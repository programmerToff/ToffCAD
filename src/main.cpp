#include "body.h"
#include "stlToAddBody.h"


int main()
{
	bodies bodies;
	bodies.addSSM();
	bodies.printPropertiesBody(bodies.getBodyProperties(0));
}