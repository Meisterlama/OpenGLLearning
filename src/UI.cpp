#include "UI.hpp"
#include "Options.hpp"

ImGuiIO &setupImGui(GLFWwindow *window, const char *GLSLVersion)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSLVersion);
	// Setup ImGui style
	ImGui::StyleColorsDark();

	return io;
}

void ImGuiNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiDestroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void mainMenuBar(int framerate)
{
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::MenuItem(std::to_string(framerate).c_str(), nullptr, nullptr);
		ImGui::MenuItem("Toggle Misc Window", nullptr, &ShowMiscEditor);
		ImGui::MenuItem("Toggle Light Editor", nullptr, &ShowLightEditor);
	}
	ImGui::EndMainMenuBar();
}

void DrawLightEditor(std::vector<Light> &lights)
{
	ImGui::Begin("Light Editor");
	if (ImGui::Button("Add Light"))
	{
		if (lights.size() < MAXLIGHTS)
		{
			lights.push_back(Light("model/cube/cube.obj",
								   glm::vec3(3.0f, 0.0f, 5.0f),
								   glm::vec3(0.9f),
								   LIGHT_POSITIONAL));
		}
	}
	if (ImGui::Button("Remove Selected Light"))
	{
		if (lights.size() > 0)
		{
			lights.erase(lights.begin() + selectedIndex);
			selectedIndex = 0;
		}
	}
	if (lights.size() > 0)
	{
		if (lights.size() > 1)
		{
			ImGui::ListBox("Lights", &selectedIndex, indexes, lights.size());
		}
		ImGui::Separator();
		editLightUI(lights[selectedIndex]);
	}
	ImGui::End();
}

void editLightUI(Light &light)
{
	ImGui::Checkbox("Active", &light.isActive);
	ImGui::Checkbox("Directional", &light.isDirectional);
	ImGui::Checkbox("Spotlight", &light.isSpotlight);
	ImGui::DragFloat3("Position", &light.position[0], 0.1f);
	ImGui::DragFloat3("Direction", &light.spotlightDirection[0], 0.1f);
	ImGui::DragFloat("Inner Cut Off", &light.cutOff, 0.001f, light.outerCutOff, 3.14);
	ImGui::DragFloat("Outer Cut Off", &light.outerCutOff, 0.001f, 0.0f, light.cutOff);
	ImGui::ColorEdit3("Light Color", &light.color[0]);
	ImGui::DragFloat("Ambient Factor", &light.ambientFactor, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Attenuation", &light.attenuation[0], 0.001f, 0.0f, 1.0f);
}
