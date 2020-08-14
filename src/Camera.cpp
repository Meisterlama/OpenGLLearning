#include "Camera.hpp"

void Camera::MoveCamera(Camera_Movement direction, float deltaTime)
{
	float distance = _moveSpeed * deltaTime;
	switch (direction)
	{
	case FORWARD:
		_position += _front * distance;
		break;
	case BACKWARD:
		_position -= _front * distance;
		break;
	case LEFT:
		_position -= _right * distance;
		break;
	case RIGHT:
		_position += _right * distance;
		break;
	case W_FORWARD:
		_position += glm::vec3(0.0f, 0.0f, -1.0f) * distance;
		break;
	case W_BACKWARD:
		_position -= glm::vec3(0.0f, 0.0f, -1.0f) * distance;
		break;
	case W_LEFT:
		_position -= glm::vec3(1.0f, 0.0f, 0.0f) * distance;
		break;
	case W_RIGHT:
		_position += glm::vec3(1.0f, 0.0f, 0.0f) * distance;
		break;
	case W_UP:
		_position += glm::vec3(0.0f, 1.0f, 0.0f) * distance;
		break;
	case W_DOWN:
		_position -= glm::vec3(0.0f, 1.0f, 0.0f) * distance;
		break;
	case H_FORWARD:
		_position += glm::vec3(_front.x, 0.0f, _front.z) * distance;
		break;
	case H_BACKWARD:
		_position -= glm::vec3(_front.x, 0.0f, _front.z) * distance;
		break;
	case H_LEFT:
		_position -= glm::vec3(_right.x, 0.0f, _right.z) * distance;
		break;
	case H_RIGHT:
		_position += glm::vec3(_right.x, 0.0f, _right.z) * distance;
		break;
	default:
		break;
	}
}

void Camera::RotateCamera(float pitchOffset, float yawOffset, GLboolean constrainPitch)
{
	pitchOffset *= _mouseSensitivity;
	yawOffset *= _mouseSensitivity;

	_yaw += pitchOffset;
	_pitch += yawOffset;

	if (constrainPitch)
	{
		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::ZoomCamera(float offset)
{
	_zoom -= offset;
	if (_zoom < 1.0f)
		_zoom = 1.0f;
	if (_zoom > 45.0f)
		_zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);
	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}
