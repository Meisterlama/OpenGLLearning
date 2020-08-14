#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Light.hpp"

ImGuiIO &setupImGui(GLFWwindow *window, const char *GLSLVersion);
void ImGuiNewFrame();
void ImGuiDestroy();
void mainMenuBar(int framerate);
void DrawLightEditor(std::vector<Light> &lights);
void editLightUI(Light &light);
