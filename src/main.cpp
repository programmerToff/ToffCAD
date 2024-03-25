#include "body.h"
#include "utility.h"
#include "UI.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

int mainLoop(bodies b, Settings::Settings Settings);
void errorCallback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
bool loadIcon(GLFWwindow* window) {
	int width, height, channels;
	unsigned char* pixels = stbi_load("mainIcon.png", &width, &height, &channels, 4);

	if (!pixels) {
		return false;
	}

	GLFWimage image;
	image.width = width;
	image.height = height;
	image.pixels = pixels;

	glfwSetWindowIcon(window, 1, &image);

	stbi_image_free(pixels);
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	bodies b(0, 0);
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	std::string filePath(lpCmdLine);
	if (filePath.length() > 5)
	{
		std::string extension = filePath.substr(filePath.length() - 5, 4);
		if (extension == ".tcc")
		{
			std::ifstream inputFile("paths.txt");
			std::string line;
			std::getline(inputFile, line);
			std::ofstream outputFile("paths.txt");
			outputFile << filePath.substr(1, filePath.length()-2) << "\n";
			while (std::getline(inputFile, line))
			{
				outputFile << line << "\n";
			}
			inputFile.close();
			outputFile.close();
		}
		else if (extension == "tcad")
		{
			b.openTCAD(filePath.substr(1, filePath.length() - 2));
		}
		else if (extension == ".stl")
		{
			b.readSTL(filePath.substr(1, filePath.length() - 2));
			b.optimizeVertexCount(0);
		}
	}

	createDefaultTCC();
	std::ifstream paths("paths.txt");
	std::string line;
	std::getline(paths, line);
	char* charPtr = new char[line.length() + 1];
	std::copy(line.begin(), line.end(), charPtr);
	charPtr[line.length()] = '\0';
	Settings::Settings Settings(charPtr);
	paths.close();

	return mainLoop(b, Settings);
}

int mainLoop(bodies b, Settings::Settings Settings)
{
	bodies Bodies = b;
	
	if (!glfwInit())
	{
		std::cout << "glfw init failed";
	}
	glfwSetErrorCallback(errorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "ToffCAD", NULL, NULL);
	if (!window)
	{
		std::cout << "Window creation failed";
	}
	loadIcon(window);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	gladLoadGL();
	std::string vertexCode = get_file_contents("vert.glsl");
	std::string fragmentCode = get_file_contents("frag.glsl");
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	unsigned int VBO, EBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bodies.vertices), Bodies.vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Bodies.indices), Bodies.indices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiStyle& style = ImGui::GetStyle();
	UI::init(window, VAO, &Settings);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(Settings.bgCol.x, Settings.bgCol.y, Settings.bgCol.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame(); 

		UI::update(window);
		UI::middle();
		UI::right();
		UI::down();
		UI::left();
		UI::up();

		ImGui::Render();

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, b.indsSize, GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}