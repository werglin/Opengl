#include "Mouse.hpp"

double Mouse::s_x= 0;
double Mouse::s_y= 0;

double Mouse::s_lastX= 0;
double Mouse::s_lastY= 0;

double Mouse::s_dx= 0;
double Mouse::s_dy= 0;

double Mouse::s_scrollDx= 0;
double Mouse::s_scrollDy= 0;

bool Mouse::s_firstMouse= 0;

bool Mouse::s_buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::s_buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };

// key state callback
void Mouse::s_CursorPosCallback(GLFWwindow* window, double x, double y){
	s_x = x;
	s_y = y;

	if (s_firstMouse)
	{
		s_lastX = s_x;
		s_lastY = s_y;
		s_firstMouse = false;
	}
	s_dx = s_x - s_lastX;
	s_dy = s_lastY - s_y; // inverted


	s_lastX = s_x;
	s_lastY = s_y;
}
void Mouse::s_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	if (action != GLFW_RELEASE)
	{
		if (!s_buttons[button])
		{
			s_buttons[button] = true;
		}
	}
	else
	{
		s_buttons[button] = false;
	}
	s_buttonsChanged[button] = action != GLFW_REPEAT;
}
void Mouse::s_MouseWheelCallback(GLFWwindow* window, double dx, double dy){
	s_scrollDx = dx;
	s_scrollDy = dy;
}

double Mouse::s_GetMouseX(){
	return s_x;
}
double Mouse::s_GetMouseY(){
	return s_y;
}
double Mouse::s_GetMouseDX(){
	double ret = s_dx;
	s_dx = 0;
	return ret;
}
double Mouse::s_GetMouseDY(){
	double ret = s_dy;
	s_dy = 0;
	return ret;
}
double Mouse::s_GetScrollDY(){
	double ret = s_scrollDy;
	s_scrollDy = 0;
	return ret;
}
double Mouse::s_GetScrollDX(){
	double ret = s_scrollDx;
	s_scrollDx = 0;
	return ret;
}

bool Mouse::s_Button(int button){
	return s_buttons[button];
}
bool Mouse::s_ButtonChanged(int button){
	bool ret = s_buttonsChanged[button];
	s_buttonsChanged[button] = false;
	return ret;
}
bool Mouse::s_ButtonUp(int button){
	return !s_buttons[button] && s_ButtonChanged(button);
}
bool Mouse::s_ButtonDown(int button){
	return s_buttons[button] && s_ButtonChanged(button);
}