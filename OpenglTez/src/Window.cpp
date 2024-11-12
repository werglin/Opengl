#include "Window.hpp"

#include"Keyboard.hpp"
#include"Mouse.hpp"

#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw.h"
#include"imgui/imgui_impl_opengl3.h"

unsigned int Window::SRC_WIDTH = 800;
unsigned int Window::SRC_HEIGHT = 600;

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Window::SRC_WIDTH = width;
	Window::SRC_HEIGHT = height;
}

Window::Window()
	:window(nullptr)
{
}

bool Window::Init()
{
	this->window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (this->window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(this->window);
	return true;
}
void Window::SetParameters()
{
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);

	glfwSetFramebufferSizeCallback(window, Window::FramebufferSizeCallback);

	glfwSetKeyCallback(window, Keyboard::s_KeyCallback);

	glfwSetCursorPosCallback(window, Mouse::s_CursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::s_MouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::s_MouseWheelCallback);

	SetCursor(true);

	// glEnable(GL_DEBUG_OUTPUT);
	// glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);
	// V Sync Level setter
	glfwSwapInterval(0);
	
}

void Window::SetCursor(bool enabled)
{
	_cursorEnabled = enabled;
	if (enabled)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}

void Window::Update()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::NewFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::SetShouldClose(bool shouldClose)
{
	glfwSetWindowShouldClose(window, shouldClose);
}
