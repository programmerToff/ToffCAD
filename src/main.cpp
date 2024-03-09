#include "body.h"
#include "stlToAddBody.h"
#include "WindowClass.h"
#include "shaderClass.h"

int main()
{
	WindowClass window(1000, 1000, "ToffCAD");
	while (!glfwWindowShouldClose(window.window))
	{
		processInput(window.window);
		glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
	window.Terminate();
}