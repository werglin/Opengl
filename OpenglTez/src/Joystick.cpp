#include"Joystick.hpp"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

JoyStick::JoyStick(int i){
	_id = GetId(i);
	Update();
}

void JoyStick::Update(){
	_present = glfwJoystickPresent(_id);
	if (_present)
	{
		this->c_pName = glfwGetJoystickName(_id);
		this->c_pAxes = glfwGetJoystickAxes(_id, &(this->_axesCount));
		this->c_pButtons = glfwGetJoystickButtons(_id, &(this->_buttonCount));
	}
}
float JoyStick::AxesState(int axis){
	if (_present)
	{
		return c_pAxes[axis];
	}
	return 0.0f;
}
unsigned char JoyStick::ButtonState(int button){
	return _present ? c_pButtons[button] : GLFW_RELEASE;
}

int JoyStick::GetAxesCount(){
	return _axesCount;
}
int JoyStick::GetButtonCount(){
	return _buttonCount;
}

bool JoyStick::IsPresent(){
	return _present;
}
const char* JoyStick::GetName(){
	return c_pName;
}

int JoyStick::GetId(int i){
	return GLFW_JOYSTICK_1 + i;
}