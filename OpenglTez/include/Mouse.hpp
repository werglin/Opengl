#pragma once

#ifndef MOUSE_hpp
#define MOUSE_hpp

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Mouse {
public:
	// key state callback
	static void s_CursorPosCallback(GLFWwindow* window, double x, double y);
	static void s_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void s_MouseWheelCallback(GLFWwindow* window, double dx, double dy);

	static double s_GetMouseX();
	static double s_GetMouseY();

	static double s_GetMouseDX();
	static double s_GetMouseDY();
	
	static double s_GetScrollDY();
	static double s_GetScrollDX();
	
	static bool s_Button(int button);
	static bool s_ButtonChanged(int button);
	static bool s_ButtonUp(int button);
	static bool s_ButtonDown(int button);

private:

	static double s_x;
	static double s_y;

	static double s_lastX;
	static double s_lastY;
	
	static double s_dx;
	static double s_dy;

	static double s_scrollDx;
	static double s_scrollDy;

	static bool s_firstMouse;

	static bool s_buttons[];
	static bool s_buttonsChanged[];
};

#endif // !MOUSE_hpp