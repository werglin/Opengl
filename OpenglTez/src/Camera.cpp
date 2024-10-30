#include"Camera.hpp"

#include<glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position)
	: _camPosition(position),
	_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	_yaw(-90.0f),
	_pitch(0.0f),
	_speed(2.5f),
	_zoom(45.0f),
	_camFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	UpdateCameraVectors();
}

void Camera::UpdateCameraDirection(double dx, double dy){
	_yaw += dx;
	_pitch += dy;
	if (_pitch > 89.0f)
	{
		_pitch = 89.0f;
	}
	else if (_pitch < -89.0f)
	{
		_pitch = -89.0f;
	}
	UpdateCameraVectors();
}
void Camera::UpdateCameraPos(CameraDirection dir, double dt){
	float velo = float(dt) * _speed;
	switch (dir)
	{
	case CameraDirection::NONE:
		break;
	case CameraDirection::FORWARD:
		_camPosition += _camFront * velo;
		break;
	case CameraDirection::BACKWARD:
		_camPosition -= _camFront * velo;
		break;
	case CameraDirection::LEFT:
		_camPosition -= _camRight * velo;
		break;
	case CameraDirection::RIGHT:
		_camPosition += _camRight * velo;
		break;
	case CameraDirection::UP:
		_camPosition += _camUp * velo;
		break;
	case CameraDirection::DOWN:
		_camPosition -= _camUp * velo;
		break;
	default:
		break;
	}
}
void Camera::UpdateCameraZoom(double dy){
	if (_zoom >= 1.0f && _zoom <= 45.0f)
	{
		// reverse scroll
		_zoom -= dy;
	}
	else if (_zoom < 1.0f)
	{
		_zoom = 1.0f;
	}
	else
	{
		_zoom = 45.0f;
	}
}


glm::mat4 Camera::GetViewMatrix(){
	return glm::lookAt(_camPosition, _camPosition + _camFront, _camUp);
}

// private
void Camera::UpdateCameraVectors(){
	glm::vec3 dir;
	dir.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	dir.y = sin(glm::radians(_pitch));
	dir.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_camFront = glm::normalize(dir);
	_camRight = glm::normalize(glm::cross(_camFront, _worldUp));
	_camUp = glm::normalize(glm::cross(_camRight, _camFront));
}