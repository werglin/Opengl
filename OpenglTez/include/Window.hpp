#pragma once

#ifndef WINDOW_hpp
#define WINDOW_hpp

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Window {
public:
	static unsigned int SRC_WIDTH;
	static unsigned int SRC_HEIGHT;

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	Window();

	bool Init();

	void SetParameters();
	void SetCursor(bool enabled);

	// main loop
	void Update();
	void NewFrame();

	bool ShouldClose();
	void SetShouldClose(bool shouldClose);

	bool _cursorEnabled = true;

	GLFWwindow* window;

};

#endif // !WINDOW_hpp
