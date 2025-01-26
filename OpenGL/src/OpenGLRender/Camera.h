#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Core/Timestep.h"

enum class CameraMoveDirection
{
	FORWARD = 0, BACKWARD, LEFT, RIGHT, UP, DOWN
};

const float c_Yaw        = -90.0f;//偏航角
const float c_Pitch      = 0.0f;//俯仰角
const float c_Speed      = 0.015f;
const float c_Sensitivty = 0.03f;//灵敏度
const float c_Zoom       = 45.0f;

class Camera
{
public:
	Camera(const glm::vec3& position = glm::vec3(0.0f,0.0f,0.0f), const glm::vec3& up = glm::vec3(0.0f,1.0f,0.0f),
		float yaw = c_Yaw,float pitch = c_Pitch);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	const glm::mat4 GetViewMatrix() const;

	void CameraInput(CameraMoveDirection direction, Timestep ts);
	void MouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void MouseScroll(float yoffset);
private:
	void UpdateCameraVectors();

public://照相机属性
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	//欧拉角
	float m_Yaw;
	float m_Pitch;
	float m_Roll;
	//照相机选项
	float m_MoveSpeed;
	float m_MouseSensitivity;
	float m_Zoom;
};
