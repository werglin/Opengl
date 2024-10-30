#pragma once
#ifndef CAMERA_hpp
#define CAMERA_hpp

#include<glm/glm.hpp>

enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public:
	glm::vec3 _camPosition;

	glm::vec3 _camFront;
	glm::vec3 _camUp;
	glm::vec3 _camRight;

	glm::vec3 _worldUp;

	float _yaw; // x-axis
	float _pitch;
	float _speed;
	float _zoom;

	Camera(glm::vec3 position);
	
	void UpdateCameraDirection(double dx, double dy);
	void UpdateCameraPos(CameraDirection dir, double dt);
	void UpdateCameraZoom(double dy);

	glm::mat4 GetViewMatrix();
private:

	void UpdateCameraVectors();

};

#endif // !CAMERA_hpp
