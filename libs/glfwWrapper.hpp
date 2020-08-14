#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int g_SCR_WIDTH = 1600;
const unsigned int g_SCR_HEIGHT = 900;

struct GLFWContext
{
	unsigned int scrWidth;
	unsigned int scrHeight;
	GLFWframebuffersizefun framebufferSize;
	GLFWcursorposfun mousePos;
	GLFWscrollfun scroll;
};

GLFWwindow* glfwContextInit(GLFWContext context)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(context.scrWidth, context.scrHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, context.framebufferSize);
	glfwSetCursorPosCallback(window, context.mousePos);
	glfwSetScrollCallback(window, context.scroll);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}