#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	W_FORWARD,
	W_BACKWARD,
	W_LEFT,
	W_RIGHT,
	W_UP,
	W_DOWN,
	H_FORWARD,
	H_BACKWARD,
	H_LEFT,
	H_RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;

	// Euler angles
	float _yaw;
	float _pitch;

	float _moveSpeed;
	float _mouseSensitivity;
	float _zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
	{
		_position = position;
		_front = glm::vec3(0.0f, 0.0f, -1.0f);
		_worldUp = up;
		_yaw = yaw;
		_pitch = pitch;
		_moveSpeed = SPEED;
		_mouseSensitivity = SENSITIVITY;
		_zoom = ZOOM;
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix() { return glm::lookAt(_position, _position + _front, _up); }

	void MoveCamera(Camera_Movement direction, float deltaTime);

	void RotateCamera(float pitchOffset, float yawOffset, GLboolean constrainPitch = true);

	void ZoomCamera(float offset);

private:
	void UpdateCameraVectors();
};
