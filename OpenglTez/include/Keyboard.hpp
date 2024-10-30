#pragma once

#ifndef KEYBOARD_hpp
#define KEYBOARD_hpp

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Keyboard {
public:
	// key state callback
	static void s_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/*
	* accessors
	*/
	static bool s_Key(int key);
	static bool s_KeyChanged(int key);
	static bool s_KeyDown(int key);
	static bool s_KeyUp(int key);

private:
	static bool s_keys[];
	static bool s_keysChanged[];
};

#endif // !KEYBOARD_hpp
