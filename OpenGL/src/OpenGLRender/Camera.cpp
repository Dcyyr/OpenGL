#include "Camera.h"


Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
	:m_Front(glm::vec3(0.0f,0.0f,-1.0f)),m_MoveSpeed(c_Speed),m_MouseSensitivity(c_Sensitivty),m_Zoom(c_Zoom)
{
	m_Position = position;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	:m_Front(glm::vec3(0.0f,0.0f,-1.0f)),m_MoveSpeed(c_Speed), m_MouseSensitivity(c_Sensitivty), m_Zoom(c_Zoom)
{
	m_Position = glm::vec3(posX, posY, posZ);
	m_WorldUp = glm::vec3(upX, upY, upZ);
	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateCameraVectors();
}

const glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::CameraInput(CameraMoveDirection direction, float ts)
{
	float Speed = m_MoveSpeed * ts;
	if (direction == CameraMoveDirection::FORWARD)
		m_Position += Speed * m_Front;
	if (direction == CameraMoveDirection::BACKWARD)
		m_Position -= Speed * m_Front;
	if (direction == CameraMoveDirection::LEFT)
		m_Position -= Speed * m_Right;
	if (direction == CameraMoveDirection::RIGHT)
		m_Position += Speed * m_Right;
}

void Camera::MouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw   += xoffset;//ƽ��ת
	m_Pitch += yoffset;//����ת

	constrainPitch = true;//���Pitch����90��ʱ�ӽǻᷢ����ת

	if (constrainPitch)
	{
		if (m_Pitch > 89)
			m_Pitch = 89;
		if (m_Pitch < -89)
			m_Pitch = -89;
	}

	UpdateCameraVectors();
}

void Camera::MouseScroll(float yoffset)
{
	m_Zoom -= yoffset;

	if (m_Zoom < 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom > 60.0f)//45.0f��Ĭ�ϵ���Ұֵ
		m_Zoom = 60.0f;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = glm::cos(glm::radians(m_Yaw) * glm::cos(glm::radians(m_Pitch)));
	front.y = glm::sin(glm::radians(m_Pitch));
	front.z = glm::sin(glm::radians(m_Yaw) * glm::cos(glm::radians(m_Pitch)));

	m_Front = glm::normalize(front);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	//��ʸ�����й�һ��������ΪԽ���ϻ����¿���ʸ���ĳ��Ⱦ�Խ�ӽ� 0���Ӷ������ƶ��ٶȼ�����

}
