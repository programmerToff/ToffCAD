#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <fstream>
#include <Windows.h>
#include <cstring>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int x, int y)
{
	glViewport(0, 0, x, y);
}

enum Pos
{
	Middle,
	Up,
	DownRight,
	DownLeft,
	Right,
	Left,
	posCount
};
enum Modes : uint8_t
{
	viewport,
	settings,
	add,
	menu,
	properties,
	history,
	elements,
	options,
	modesCount
};

namespace Settings
{
	struct Settings
	{
	private:
		void info(std::string info, GLFWwindow* window)
		{
			if (!loadingScreen)
			{
				std::cout << info << '\n';
				return;
			}
			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Text Flag");
			ImGui::Text(info.c_str());
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glClearColor(1.0f, 1.0f, 0.9f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(window);
		}
		void load(std::ifstream& file, GLFWwindow* window)
		{
			info("loading background color", window);
			file.read(reinterpret_cast<char*>(&bgCol.x), 4);
			file.read(reinterpret_cast<char*>(&bgCol.y), 4);
			file.read(reinterpret_cast<char*>(&bgCol.z), 4);
			info("Loading default modes", window);
			char modes[posCount];
			file.read(reinterpret_cast<char*>(&modes), posCount);
			int i = 0;
			for (auto& mode : modes)
			{
				defaultModes[i] = Modes(mode);
				i++;
			}
		}
		void resetLoad(std::ifstream& file)
		{
			file.read(reinterpret_cast<char*>(&bgCol.x), 4);
			file.read(reinterpret_cast<char*>(&bgCol.y), 4);
			file.read(reinterpret_cast<char*>(&bgCol.z), 4);
			file.read(reinterpret_cast<char*>(&defaultModes), posCount);
		}
	public:
		bool loadingScreen;
		ImVec4 bgCol;
		char* settingsPath;
		Modes defaultModes[posCount];

		Settings(char* filepath)
		{
			settingsPath = filepath;
			std::ifstream tccFile(filepath, std::ios::binary);
			if (!tccFile.good())
			{
				settingsPath = (char*)"default.tcc";
				tccFile.close();
				tccFile.open(settingsPath, std::ios::binary);
				std::ifstream inputFile("paths.txt");
				std::string line;
				std::getline(inputFile, line);
				std::ofstream outputFile("paths.txt");
				outputFile << "default.tcc" << "\n";
				while (std::getline(inputFile, line))
				{
					outputFile << line << "\n";
				}
				inputFile.close();
				outputFile.close();
			}
			bool bits[8];
			tccFile.read(reinterpret_cast<char*>(&bits), 1);
			loadingScreen = bits[0];

			if (loadingScreen)
			{
				if (!glfwInit()) 
				{
					std::cerr << "Failed to initialize GLFW" << std::endl;
				}
				glfwWindowHint(GLFW_DECORATED, 0);
				glfwWindowHint(GLFW_RESIZABLE, 0);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				GLFWwindow* window = glfwCreateWindow(800, 600, "Loading Screen", NULL, NULL);
				if (!window) {
					std::cerr << "Failed to create GLFW window" << std::endl;
					glfwTerminate();
				}
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				if (!monitor) {
					std::cerr << "Failed to get primary monitor" << std::endl;
				}
				int monitorWidth, monitorHeight;
				glfwGetMonitorWorkarea(monitor, NULL, NULL, &monitorWidth, &monitorHeight);
				int width, height;
				glfwGetWindowSize(window, &width, &height);
				int xPos = (monitorWidth - width) / 2;
				int yPos = (monitorHeight - height) / 2;
				glfwSetWindowPos(window, xPos, yPos);
				glfwMakeContextCurrent(window);
				glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
				gladLoadGL();
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				ImGui_ImplGlfw_InitForOpenGL(window, true);
				ImGui_ImplOpenGL3_Init("#version 330");
				load(tccFile, window);
				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ImGui::DestroyContext();
				glfwTerminate();
				return;
			}
			if (!glfwInit()) {
				std::cerr << "Failed to initialize GLFW" << std::endl;
			}
			glfwWindowHint(GLFW_DECORATED, 0);
			glfwWindowHint(GLFW_RESIZABLE, 0);
			GLFWwindow* window = glfwCreateWindow(0, 0, " ", NULL, NULL);
			load(tccFile, window);
			glfwTerminate();

			tccFile.close();
		}
		void reset()
		{
			char* filepath = settingsPath;
			std::ifstream tccFile(filepath, std::ios::binary);
			bool bits[8];
			char buffer;
			tccFile.read(reinterpret_cast<char*>(&buffer), 1);
			for (int i = 0; i < 8; ++i) {
				bits[i] = (buffer >> (7 - i)) & 1;
			}

			loadingScreen = bits[0];
			resetLoad(tccFile);

			tccFile.close();
		}
		Settings& operator=(const Settings& other) {
			if (this != &other) 
			{
				loadingScreen = other.loadingScreen;
				bgCol = other.bgCol;
				settingsPath = other.settingsPath;
			}
			return *this;
		}
	};
	void saveSettings(const char* filepath, Settings set)
	{
		std::ofstream file(filepath, std::ios::binary);
		char c;
		if (set.loadingScreen) {
			c |= (1 << 0);
		}
		else {
			c &= ~(1 << 0);
		}
		file.write(reinterpret_cast<char*>(&c), 1);
		file.write(reinterpret_cast<char*>(&set.bgCol), 12);
		file.write(reinterpret_cast<char*>(&set.defaultModes), posCount);


		file.close();
	}
}

namespace Menu
{
	std::string filepath;
	char* message;
}

namespace UI
{
	int upHeight;
	int downHeight;
	int leftWidth;
	int rightWidth;
	int winHeight;
	int winWidth;
	int downleftWidth;
	Modes winModes[posCount];
	const char* modes[modesCount] = {
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
	int defaultModesCur[posCount];
	GLuint VAO;
	Settings::Settings* Settings;
	bodies* b;
	void init(GLFWwindow* window, GLuint vao, Settings::Settings* initSettings, bodies* bp)
	{
		Settings = initSettings;
		glfwGetWindowSize(window, &winWidth, &winHeight);
		upHeight = 200;
		downHeight = 200;
		leftWidth = 200;
		rightWidth = 200;
		downleftWidth = 200;
		winModes[Middle] = Settings->defaultModes[Middle];
		winModes[Up] = Settings->defaultModes[Up];
		winModes[DownLeft] = Settings->defaultModes[DownLeft];
		winModes[DownRight] = Settings->defaultModes[DownRight];
		winModes[Left] = Settings->defaultModes[Left];
		winModes[Right] = Settings->defaultModes[Right];
		selectedModeMiddle = Settings->defaultModes[Middle];
		selectedModeUp = Settings->defaultModes[Up];
		selectedModeDownLeft = Settings->defaultModes[DownLeft];
		selectedModeDownRight = Settings->defaultModes[DownRight];
		selectedModeLeft = Settings->defaultModes[Left];
		selectedModeRight = Settings->defaultModes[Right];
		defaultModesCur[Middle] = winModes[Middle];
		defaultModesCur[Up] = winModes[Up];
		defaultModesCur[DownRight] = winModes[DownRight];
		defaultModesCur[DownLeft] = winModes[DownLeft];
		defaultModesCur[Right] = winModes[Right];
		defaultModesCur[Left] = winModes[Left];
		Menu::message = (char*)"";
		VAO = vao;
		b = bp;
	}
	void update(GLFWwindow* window)
	{
		glfwGetWindowSize(window, &winWidth, &winHeight);
	}

	void ImViewport()
	{
		return;
	}
	void ImSettings()
	{
		ImGui::Text("TC-Config Source:  ");
		ImGui::SameLine();
		ImGui::Text(Settings->settingsPath);
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			std::string path = openExplorerDialog();
			Settings->settingsPath = new char[path.length() + 1];
			std::copy(path.begin(), path.end(), Settings->settingsPath);
			Settings->settingsPath[path.length()] = '\0';
			Settings->reset();
			std::ifstream inputFile("paths.txt");
			std::string line;
			std::getline(inputFile, line);
			std::ofstream outputFile("paths.txt");
			outputFile << path << "\n";
			while (std::getline(inputFile, line))
			{
				outputFile << line << "\n";
			}
			inputFile.close();
			outputFile.close();
			winModes[Middle] = Settings->defaultModes[Middle];
			winModes[Up] = Settings->defaultModes[Up];
			winModes[DownLeft] = Settings->defaultModes[DownLeft];
			winModes[DownRight] = Settings->defaultModes[DownRight];
			winModes[Left] = Settings->defaultModes[Left];
			winModes[Right] = Settings->defaultModes[Right];
			selectedModeMiddle = Settings->defaultModes[Middle];
			selectedModeUp = Settings->defaultModes[Up];
			selectedModeDownLeft = Settings->defaultModes[DownLeft];
			selectedModeDownRight = Settings->defaultModes[DownRight];
			selectedModeLeft = Settings->defaultModes[Left];
			selectedModeRight = Settings->defaultModes[Right];
			defaultModesCur[Middle] = winModes[Middle];
			defaultModesCur[Up] = winModes[Up];
			defaultModesCur[DownRight] = winModes[DownRight];
			defaultModesCur[DownLeft] = winModes[DownLeft];
			defaultModesCur[Right] = winModes[Right];
			defaultModesCur[Left] = winModes[Left];
		}
		ImGui::SameLine();
		if (ImGui::Button("default"))
		{
			Settings->settingsPath = (char*)"default.tcc";
			Settings->reset();
			std::ifstream inputFile("paths.txt");
			std::string line;
			std::getline(inputFile, line);
			std::ofstream outputFile("paths.txt");
			outputFile << "default.tcc" << "\n";
			while (std::getline(inputFile, line))
			{
				outputFile << line << "\n";
			}
			inputFile.close();
			outputFile.close();
			winModes[Middle] = Settings->defaultModes[Middle];
			winModes[Up] = Settings->defaultModes[Up];
			winModes[DownLeft] = Settings->defaultModes[DownLeft];
			winModes[DownRight] = Settings->defaultModes[DownRight];
			winModes[Left] = Settings->defaultModes[Left];
			winModes[Right] = Settings->defaultModes[Right];
			selectedModeMiddle = Settings->defaultModes[Middle];
			selectedModeUp = Settings->defaultModes[Up];
			selectedModeDownLeft = Settings->defaultModes[DownLeft];
			selectedModeDownRight = Settings->defaultModes[DownRight];
			selectedModeLeft = Settings->defaultModes[Left];
			selectedModeRight = Settings->defaultModes[Right];
			defaultModesCur[Middle] = winModes[Middle];
			defaultModesCur[Up] = winModes[Up];
			defaultModesCur[DownRight] = winModes[DownRight];
			defaultModesCur[DownLeft] = winModes[DownLeft];
			defaultModesCur[Right] = winModes[Right];
			defaultModesCur[Left] = winModes[Left];
		}
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Loading Screen", &Settings->loadingScreen);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::ColorEdit4("Background Color", (float*)&Settings->bgCol);
		ImGui::SetNextItemWidth(125);
		if (ImGui::Combo("Default Middle", &defaultModesCur[Middle], modes, IM_ARRAYSIZE(modes)))
		{
			switch (defaultModesCur[Middle])
			{
			case viewport:
				defaultModesCur[Middle] = viewport;
				break;
			case settings:
				defaultModesCur[Middle] = settings;
				break;
			case add:
				defaultModesCur[Middle] = add;
				break;
			case menu:
				defaultModesCur[Middle] = menu;
				break;
			case options:
				defaultModesCur[Middle] = options;
				break;
			case properties:
				defaultModesCur[Middle] = properties;
				break;
			case history:
				defaultModesCur[Middle] = history;
				break;
			case elements:
				defaultModesCur[Middle] = elements;
				break;
			}
		}
		ImGui::SetNextItemWidth(125);
		if (ImGui::Combo("Default Up", &defaultModesCur[Up], modes, IM_ARRAYSIZE(modes)))
		{
			switch (defaultModesCur[Up])
			{
			case viewport:
				defaultModesCur[Up] = viewport;
				break;
			case settings:
				defaultModesCur[Up] = settings;
				break;
			case add:
				defaultModesCur[Up] = add;
				break;
			case menu:
				defaultModesCur[Up] = menu;
				break;
			case options:
				defaultModesCur[Up] = options;
				break;
			case properties:
				defaultModesCur[Up] = properties;
				break;
			case history:
				defaultModesCur[Up] = history;
				break;
			case elements:
				defaultModesCur[Up] = elements;
				break;
			}
		}
		ImGui::SetNextItemWidth(125);
		if (ImGui::Combo("Default DownRight", &defaultModesCur[DownRight], modes, IM_ARRAYSIZE(modes)))
		{
			switch (defaultModesCur[DownRight])
			{
			case viewport:
				defaultModesCur[DownRight] = viewport;
				break;
			case settings:
				defaultModesCur[DownRight] = settings;
				break;
			case add:
				defaultModesCur[DownRight] = add;
				break;
			case menu:
				defaultModesCur[DownRight] = menu;
				break;
			case options:
				defaultModesCur[DownRight] = options;
				break;
			case properties:
				defaultModesCur[DownRight] = properties;
				break;
			case history:
				defaultModesCur[DownRight] = history;
				break;
			case elements:
				defaultModesCur[DownRight] = elements;
				break;
			}
		}
		ImGui::SetNextItemWidth(125);
		if (ImGui::Combo("Default DownLeft", &defaultModesCur[DownLeft], modes, IM_ARRAYSIZE(modes)))
		{
			switch (defaultModesCur[DownLeft])
			{
			case viewport:
				defaultModesCur[DownLeft] = viewport;
				break;
			case settings:
				defaultModesCur[DownLeft] = settings;
				break;
			case add:
				defaultModesCur[DownLeft] = add;
				break;
			case menu:
				defaultModesCur[DownLeft] = menu;
				break;
			case options:
				defaultModesCur[DownLeft] = options;
				break;
			case properties:
				defaultModesCur[DownLeft] = properties;
				break;
			case history:
				defaultModesCur[DownLeft] = history;
				break;
			case elements:
				defaultModesCur[DownLeft] = elements;
				break;
			}
		}
		ImGui::SetNextItemWidth(125);
		if (ImGui::Combo("Default Right", &defaultModesCur[Right], modes, IM_ARRAYSIZE(modes)))
		{
			switch (defaultModesCur[Right])
			{
			case viewport:
				defaultModesCur[Right] = viewport;
				break;
			case settings:
				defaultModesCur[Right] = settings;
				break;
			case add:
				defaultModesCur[Right] = add;
				break;
			case menu:
				defaultModesCur[Right] = menu;
				break;
			case options:
				defaultModesCur[Right] = options;
				break;
			case properties:
				defaultModesCur[Right] = properties;
				break;
			case history:
				defaultModesCur[Right] = history;
				break;
			case elements:
				defaultModesCur[Right] = elements;
				break;
			}
		}
		ImGui::SetNextItemWidth(125);
		if (ImGui::Combo("Default Left", &defaultModesCur[Left], modes, IM_ARRAYSIZE(modes)))
		{
			switch (defaultModesCur[Left])
			{
			case viewport:
				defaultModesCur[Left] = viewport;
				break;
			case settings:
				defaultModesCur[Left] = settings;
				break;
			case add:
				defaultModesCur[Left] = add;
				break;
			case menu:
				defaultModesCur[Left] = menu;
				break;
			case options:
				defaultModesCur[Left] = options;
				break;
			case properties:
				defaultModesCur[Left] = properties;
				break;
			case history:
				defaultModesCur[Left] = history;
				break;
			case elements:
				defaultModesCur[Left] = elements;
				break;
			}
		}
		if (ImGui::Button("save Configuration..."))
		{
			int i = 0;
			for (auto& M : defaultModesCur)
			{
				Settings->defaultModes[i] = Modes(M);
				i++;
			}
			Settings::saveSettings(Settings->settingsPath, *Settings);
		}
		if (ImGui::Button("save Configuration as..."))
		{
			std::string path = saveFileDialog();
			Settings->settingsPath = new char[path.length() + 1];
			std::copy(path.begin(), path.end(), Settings->settingsPath);
			Settings->settingsPath[path.length()] = '\0';
			int i = 0;
			for (auto& M : defaultModesCur)
			{
				Settings->defaultModes[i] = Modes(M);
				i++;
			}
			Settings::saveSettings(path.c_str(), *Settings);
			Settings->reset();
			std::ifstream inputFile("paths.txt");
			std::string line;
			std::getline(inputFile, line);
			std::ofstream outputFile("paths.txt");
			outputFile << path << "\n";
			while (std::getline(inputFile, line))
			{
				outputFile << line << "\n";
			}
			inputFile.close();
			outputFile.close();
		}
	}
	void ImAdd()
	{
		return;
	}
	void ImMenu()
	{
		if (ImGui::Button("new TCAD", ImVec2(95, 20)))
		{
			b->BodyList.first.clear();
			b->BodyList.second.clear();
			Menu::filepath = "";
		}

		ImGui::SameLine();
		if (ImGui::Button("save TCAD", ImVec2(95, 20)))
			if (Menu::filepath != "")
				b->saveTCAD(Menu::filepath);
			else
				Menu::message = (char*)"no filepath given";

		ImGui::SameLine();
		if (ImGui::Button("open STL", ImVec2(95, 20)))
			b->readSTL(openExplorerDialog());

		if (ImGui::Button("open TCAD", ImVec2(95, 20)))
			b->openTCAD(openExplorerDialog());
		ImGui::SameLine();
		if (ImGui::Button("save TCAD as", ImVec2(95, 20)))
		{
			Menu::filepath = saveFileDialog();
			b->saveTCAD(Menu::filepath);
		}

		ImGui::SameLine();
		if (ImGui::Button("save STL", ImVec2(95, 20)))
			b->generateSTL(0, saveFileDialog());

		for(int i = 0;i<5;i++)
			ImGui::Spacing();
		
		ImGui::Text(Menu::message);
		ImGui::SameLine();
		if(Menu::message != "")
			if (ImGui::Button("OK"))
				Menu::message = (char*)"";
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
		switch (winModes[mode])
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
	}

	void middle()
	{
		if (winModes[Middle] == viewport)
		{
			ImGui::SetNextWindowBgAlpha(0.0f);
		}
		ImGui::SetNextWindowPos(ImVec2(leftWidth, upHeight), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(winWidth - leftWidth - rightWidth, winHeight - upHeight - downHeight), ImGuiCond_Always);
		ImGui::Begin("Middle", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		combo(Middle, selectedModeMiddle);

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
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, winHeight - upHeight - winHeight / 2.5), ImVec2(winWidth / 2.5, winHeight - upHeight - 10));
		ImGui::Begin("left", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		downHeight = winHeight - upHeight - ImGui::GetWindowSize().y;
		leftWidth = ImGui::GetWindowSize().x;

		combo(Left, selectedModeLeft);
		
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
		
		ImGui::End();
	}
}