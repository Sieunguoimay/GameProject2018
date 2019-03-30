#include "Camera2D.h"



Camera2D::Camera2D()
	:m_flag(true),m_scale(1.0f)
	,m_position(0.0f)
	,m_orthoMatrix(1.0f)
	,m_cameraMatrix(1.0f)
{
}


void Camera2D::Init(const glm::vec2 & size, const glm::vec2 & physicalSize)
{
	m_size = size;
	m_physicalSize = physicalSize;
	m_orthoMatrix = glm::ortho(-size.x / 2.0f, size.x /2.0f, -size.y/2.0f, size.y/2.0f);
	m_AABB = glm::vec4((m_position - 0.5f*m_size), m_size);
}

void Camera2D::Update(float deltaTime) {
	if (m_flag) {
		m_AABB = glm::vec4((m_position - 0.5f*m_size), m_size);

		glm::vec3 translate(-m_position.x, -m_position.y, 0.0f);
		m_cameraMatrix = glm::translate(m_orthoMatrix, translate);
		glm::vec3 scale(m_scale, m_scale, 1.0f);
		m_cameraMatrix = glm::scale(m_cameraMatrix, scale);
		m_flag = false;
	}
}
