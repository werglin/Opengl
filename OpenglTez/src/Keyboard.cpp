#include"Keyboard.hpp"

// gets the last glfw key and key macros are integer so this defines how many key in keyboard defined in glfw
// start all the keys as 0 - false
bool Keyboard::s_keys[GLFW_KEY_LAST] = { 0 };
bool Keyboard::s_keysChanged[GLFW_KEY_LAST] = { 0 };

// key state callback
void Keyboard::s_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_RELEASE)
	{
		if (!s_keys[key])
		{
			s_keys[key] = true;
		}
	}
	else
	{
		s_keys[key] = false;
	}
	s_keysChanged[key] = action != GLFW_REPEAT;

}

/*
* accessors
*/
bool Keyboard::s_Key(int key) {
	return s_keys[key];
}
bool Keyboard::s_KeyChanged(int key) {
	bool ret = s_keysChanged[key];
	s_keysChanged[key] = false;
	return ret;
}
bool Keyboard::s_KeyDown(int key) {
	return s_keys[key] && s_KeyChanged(key);
}
bool Keyboard::s_KeyUp(int key) {
	return !s_keys[key] && s_KeyChanged(key);
}