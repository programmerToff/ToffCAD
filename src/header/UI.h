#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <fstream>

namespace Settings
{
	
	struct Settings
	{
	private:
		void info(std::string info)
		{
			if (!loadingScreen)
			{
				std::cout << info << '\n';
				return;
			}
			ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - ImGui::GetTextLineHeight()));
			ImGui::Begin("Text Flag", nullptr,
				ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
			ImGui::Text(info.c_str());
			ImGui::End();
		}
		void load(std::ifstream& file)
		{
			info("loading background color");
			file.read(reinterpret_cast<char*>(&bgCol.x), 4);
			file.read(reinterpret_cast<char*>(&bgCol.y), 4);
			file.read(reinterpret_cast<char*>(&bgCol.z), 4);
		}
	public:
		bool loadingScreen;
		ImVec4 bgCol;
		Settings(const char* filepath)
		{
			std::ifstream tccFile(filepath, std::ios::binary);
			bool bits[8];
			char buffer;
			tccFile.read(reinterpret_cast<char*>(&buffer), 1);
			for (int i = 0; i < 8; ++i) {
				bits[i] = (buffer >> i) & 1;
			}
			loadingScreen = bits[0];

			if (loadingScreen)
			{
				if (!glfwInit()) {
					std::cerr << "Failed to initialize GLFW" << std::endl;
				}
				glfwWindowHint(GLFW_DECORATED, 0);
				glfwWindowHint(GLFW_RESIZABLE, 0);
				GLFWwindow* window = glfwCreateWindow(800, 600, "Loading Screen", NULL, NULL);
				if (!window) {
					std::cerr << "Failed to create GLFW window" << std::endl;
					glfwTerminate();
				}
				glfwMakeContextCurrent(window);
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				ImGui_ImplGlfw_InitForOpenGL(window, true);
				ImGui_ImplOpenGL3_Init("#version 330");
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				glfwSwapBuffers(window);
			}

			load(tccFile);
			if (loadingScreen)
			{
				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ImGui::DestroyContext();
				glfwTerminate();
			}
			tccFile.close();
		}
	};
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
	GLuint VAO;
	void init(GLFWwindow* window, GLuint vao)
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
		VAO = vao;
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