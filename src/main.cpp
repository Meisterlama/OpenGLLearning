#define STB_IMAGE_IMPLEMENTATION

#include <filesystem>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Model.hpp"
#include "Light.hpp"
#include "Shader.hpp"
#include "stb_image.hpp"
#include "glfwWrapper.hpp"
#include "UI.hpp"
#include "Options.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int scrWidth = 1600;
unsigned int scrHeight = 900;
const char *glslVersion = "#version 460 core";
bool ShowLightEditor = false;
const char *indexes[MAXLIGHTS] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
int selectedIndex = 0;
bool ShowMiscEditor = true;

GLFWContext context{
	scrWidth,  // screen width
	scrHeight, // screen height
	framebuffer_size_callback,
	mouse_callback,
	scroll_callback,
};

// Init camera
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = scrWidth / 2.0f;
float lastY = scrHeight / 2.0f;
bool firstMouse = true;
bool cursorDisabled = true;
bool togglewindow = true;

// Init timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	GLFWwindow *window = glfwContextInit(context);

	ImGuiIO &io = setupImGui(window, glslVersion);

	// Load opengGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 0;
	}

	stbi_set_flip_vertically_on_load(true); // Flip the textures on load

	Shader ourShader("shaders/shader");
	Shader LightShader("shaders/lightShader");

	Model backpack("model/backpack/backpack.obj");

	std::vector<Light> lights;

	Light lightCube("model/cube/cube.obj",
					glm::vec3(3.0f, 0.0f, 5.0f),
					glm::vec3(0.9f),
					LIGHT_POSITIONAL);
	lights.push_back(lightCube);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		// Calculate timing
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGuiNewFrame();

		// input
		processInput(window);

		// view/projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(cam._zoom),
												static_cast<float>(scrWidth) / static_cast<float>(scrHeight), 0.1f,
												100.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		ourShader.use();
		ourShader.set("projection", projection);
		ourShader.set("view", cam.GetViewMatrix());
		ourShader.set("model", model);
		ourShader.set("material.shininess", 128.0f);
		if (lights.size() > 0)
		{
			for (int i = 0; i < lights.size(); i++)
			{
				lights[i].setUniforms(ourShader, i);
			}
		}
		ourShader.set("nbLights", (int)lights.size());
		ourShader.set("viewPos", cam._position);
		backpack.Draw(ourShader);

		LightShader.use();
		if (lights.size() > 0)
		{
			for (int i = 0; i < lights.size(); i++)
			{
				if (lights[i].isActive && !(lights[i].isDirectional || lights[i].isSpotlight))
				{
					glm::mat4 lightModel = glm::translate(model, glm::vec3(lights[i].position));
					lightModel = glm::scale(lightModel, glm::vec3(0.5f));
					LightShader.set("projection", projection);
					LightShader.set("view", cam.GetViewMatrix());
					LightShader.set("model", lightModel);
					LightShader.set("lightColor", lights[i].color);
					lights[i].Draw(LightShader);
				}
			}
		}
		mainMenuBar(1 / deltaTime);

		if (ShowMiscEditor)
		{
			ImGui::Begin("Test window");
			ImGui::DragFloat3("Cam position", &cam._position[0], 0.1f);
			ImGui::Text("%f,%f,%f", cam._front[0], cam._front[1], cam._front[2]);
			if (ImGui::Button("Reload Shaders"))
				ourShader.reload();
			ImGui::End();
		}

		if (ShowLightEditor)
		{
			DrawLightEditor(lights);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGuiDestroy();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	if (cursorDisabled)
		cam.RotateCamera(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	cam.ZoomCamera(yoffset);
}

bool consoleKeyPressed = false;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.MoveCamera(H_FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.MoveCamera(H_BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.MoveCamera(H_LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.MoveCamera(H_RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cam.MoveCamera(W_UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cam.MoveCamera(W_DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		consoleKeyPressed = true;
	if (consoleKeyPressed && glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
	{
		consoleKeyPressed = false;
		if (cursorDisabled)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cursorDisabled = false;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cursorDisabled = true;
		}
	}
}
