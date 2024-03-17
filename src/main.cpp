#include "body.h"
#include "utility.h"


int mainLoop();
void errorCallback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}
void framebuffer_size_callback(GLFWwindow* window, int x, int y)
{
	glViewport(0, 0, x, y);
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

int main()
{
	return mainLoop();
}

namespace ImGui
{
	enum Pos
	{
		Middle,
		Left,
		Right,
		Up,
		DownLeft,
		DownRight
	};
	enum Modes
	{
		viewport,
		settings,
		add,
		menu,
		properties,
		history,
		elements,
		options
	};
	int upHeight;
	int downHeight;
	int leftWidth;
	int rightWidth;
	int winHeight;
	int winWidth;
	int downleftWidth;
	Modes winModes[6];
	const char* modes[] = {
		"viewport",
		"settings",
		"add",
		"menu",
		"properties",
		"history",
		"elements",
		"options"
	};
	int selectedModeUp;
	int selectedModeMiddle;
	int selectedModeDownRight;
	int selectedModeLeft;
	int selectedModeRight;
	int selectedModeDownLeft;
	void init(GLFWwindow* window)
	{
		glfwGetWindowSize(window, &winWidth, &winHeight);
		upHeight = 200;
		downHeight = 200;
		leftWidth = 200;
		rightWidth = 200;
		downleftWidth = 200;
		winModes[Middle] = viewport;
		winModes[Up] = menu;
		winModes[DownLeft] = add;
		winModes[DownRight] = options;
		winModes[Left] = elements;
		winModes[Right] = properties;
		selectedModeMiddle = viewport;
		selectedModeUp = menu;
		selectedModeDownLeft = add;
		selectedModeDownRight = options;
		selectedModeLeft = elements;
		selectedModeRight = properties;
	}
	void update(GLFWwindow* window)
	{
		glfwGetWindowSize(window, &winWidth, &winHeight);
	}
	void combo(Pos mode, int& selectedMode)
	{
		if (ImGui::Combo(" ", &selectedMode, modes, IM_ARRAYSIZE(modes)))
		{
			switch (selectedMode)
			{
			case viewport:
				winModes[mode] = viewport;
				break;
			case settings:
				winModes[mode] = settings;
				break;
			case add:
				winModes[mode] = add;
				break;
			case menu:
				winModes[mode] = menu;
				break;
			case options:
				winModes[mode] = options;
				break;
			case properties:
				winModes[mode] = properties;
				break;
			case history:
				winModes[mode] = history;
				break;
			case elements:
				winModes[mode] = elements;
				break;
			}
		}
	}

	void ImViewport()
	{
		return;
	}
	void ImSettings()
	{
		return;
	}
	void ImAdd()
	{
		return;
	}
	void ImMenu()
	{
		return;
	}
	void ImProperties()
	{
		return;
	}
	void ImHistory()
	{
		return;
	}
	void ImElements()
	{
		return;
	}
	void ImOptions()
	{
		return;
	}

	void middle()
	{
		if(winModes[Middle] == viewport)
		{
			ImGui::SetNextWindowBgAlpha(0.0f);
		}
		ImGui::SetNextWindowPos(ImVec2(leftWidth, upHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(winWidth - leftWidth - rightWidth, winHeight - upHeight - downHeight), ImGuiCond_Always);
		ImGui::Begin("Middle", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		combo(Middle, selectedModeMiddle);
		switch (winModes[Middle])
		{
		case viewport:
			ImViewport();
			break;
		case settings:
			ImSettings();
			break;
		case add:
			ImAdd();
			break;
		case menu:
			ImMenu();
			break;
		case properties:
			ImProperties();
			break;
		case history:
			ImHistory();
			break;
		case elements:
			ImElements();
			break;
		case options:
			ImOptions();
			break;
		}

		ImGui::End();
	}
	void left()
	{
		if (winModes[Left] == viewport)
		{
			ImGui::SetNextWindowBgAlpha(0.0f);
		}
		ImGui::SetNextWindowPos(ImVec2(0, upHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(leftWidth, winHeight - upHeight - downHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, winHeight - upHeight - winHeight/2.5), ImVec2(winWidth / 2.5, winHeight - upHeight - 10));
		ImGui::Begin("left", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		downHeight = winHeight - upHeight - ImGui::GetWindowSize().y;
		leftWidth = ImGui::GetWindowSize().x;

		combo(Left, selectedModeLeft);
		switch (winModes[Left])
		{
		case viewport:
			ImViewport();
			break;
		case settings:
			ImSettings();
			break;
		case add:
			ImAdd();
			break;
		case menu:
			ImMenu();
			break;
		case properties:
			ImProperties();
			break;
		case history:
			ImHistory();
			break;
		case elements:
			ImElements();
			break;
		case options:
			ImOptions();
			break;
		}

		ImGui::End();
	}
	void right()
	{
		if (winModes[Right] == viewport)
		{
			ImGui::SetNextWindowBgAlpha(0.0f);
		}
		ImGui::SetNextWindowPos(ImVec2(winWidth - rightWidth, upHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(rightWidth, winHeight - upHeight - downHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, winHeight - upHeight - downHeight), ImVec2(winWidth / 2.5, winHeight - upHeight - downHeight));
		ImGui::Begin("right", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		rightWidth = ImGui::GetWindowSize().x;

		combo(Right, selectedModeRight);
		switch (winModes[Right])
		{
		case viewport:
			ImViewport();
			break;
		case settings:
			ImSettings();
			break;
		case add:
			ImAdd();
			break;
		case menu:
			ImMenu();
			break;
		case properties:
			ImProperties();
			break;
		case history:
			ImHistory();
			break;
		case elements:
			ImElements();
			break;
		case options:
			ImOptions();
			break;
		}

		ImGui::End();
	}
	void up()
	{
		if (winModes[Up] == viewport)
		{ 
			ImGui::SetNextWindowBgAlpha(0.0f); 
		}
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSizeConstraints(ImVec2(winWidth + 10, 0), ImVec2(winWidth + 10, winHeight / 2.5));
		ImGui::SetNextWindowSize(ImVec2(winWidth + 10, upHeight), ImGuiCond_Always);
		ImGui::Begin("Up", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		upHeight = ImGui::GetWindowSize().y;

		combo(Up, selectedModeUp);
		switch (winModes[Up])
		{
		case viewport:
			ImViewport();
			break;
		case settings:
			ImSettings();
			break;
		case add:
			ImAdd();
			break;
		case menu:
			ImMenu();
			break;
		case properties:
			ImProperties();
			break;
		case history:
			ImHistory();
			break;
		case elements:
			ImElements();
			break;
		case options:
			ImOptions();
			break;
		}

		ImGui::End();
	}
	void down()
	{
		if (winModes[DownLeft] == viewport)
		{
			ImGui::SetNextWindowBgAlpha(0.0f);
		}
		ImGui::SetNextWindowPos(ImVec2(0, winHeight - downHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(downleftWidth, downHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(winWidth, winHeight / 2.5));
		ImGui::Begin("downLeft", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		downHeight = ImGui::GetWindowSize().y;

		combo(DownLeft, selectedModeDownLeft);
		switch (winModes[DownLeft])
		{
		case viewport:
			ImViewport();
			break;
		case settings:
			ImSettings();
			break;
		case add:
			ImAdd();
			break;
		case menu:
			ImMenu();
			break;
		case properties:
			ImProperties();
			break;
		case history:
			ImHistory();
			break;
		case elements:
			ImElements();
			break;
		case options:
			ImOptions();
			break;
		}

		ImGui::End();
		if (winModes[DownRight] == viewport)
		{
			ImGui::SetNextWindowBgAlpha(0.0f);
		}
		ImGui::SetNextWindowPos(ImVec2(downleftWidth, winHeight - downHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(winWidth + 10 - downleftWidth, downHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(winWidth, winHeight / 2.5));
		ImGui::Begin("downRight", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		downHeight = ImGui::GetWindowSize().y;
		downleftWidth = winWidth - (ImGui::GetWindowSize().x - 10);

		combo(DownRight, selectedModeDownRight);
		switch (winModes[DownRight])
		{
		case viewport:
			ImViewport();
			break;
		case settings:
			ImSettings();
			break;
		case add:
			ImAdd();
			break;
		case menu:
			ImMenu();
			break;
		case properties:
			ImProperties();
			break;
		case history:
			ImHistory();
			break;
		case elements:
			ImElements();
			break;
		case options:
			ImOptions();
			break;
		}

		ImGui::End();
	}
}

int mainLoop()
{
	bodies Bodies(0, 0);
	
	if (!glfwInit())
	{
		std::cout << "glfw init failed";
	}
	glfwSetErrorCallback(errorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "ToffCAD", NULL, NULL);
	if (!window)
	{
		std::cout << "Window creation failed";
	}
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
	ImGui_ImplOpenGL3_Init("#version 460");
	ImGui::StyleColorsDark();
	ImGui::init(window);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame(); 

		ImGui::update(window);
		ImGui::middle();
		ImGui::right();
		ImGui::down();
		ImGui::left();
		ImGui::up();

		ImGui::Render();
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