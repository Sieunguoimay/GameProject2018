#include "Camera2D.h"



Camera2D::Camera2D()
	:m_flag(true),m_scale(1.0f)
	,m_position(0.0f)
	,m_orthoMatrix(1.0f)
	,m_cameraMatrix(1.0f)
	, m_pTarget(NULL)
{
}


Camera2D::~Camera2D()
{
}

void Camera2D::Init(int windowWidth, int windowHeight) {
	//camera position is at the center of screen
	//not at the corner
	m_size.x = (float)windowWidth;
	m_size.y = (float)windowHeight;
	m_orthoMatrix = glm::ortho(-(float)windowWidth / 2.0f, (float)windowWidth/2.0f, -(float)windowHeight/2.0f, (float)windowHeight/2.0f);
}
#include"SDL2\SDL.h"
void Camera2D::Update(float deltaTime) {
	if (m_pTarget != NULL) {
		//glm::vec2 vel = 0.5f*(*m_pTarget - m_position);
		//if (vel.x + vel.y > 10.0f)
			SetPos(*m_pTarget);
	}

	if (m_flag) {

		glm::vec3 translate(-m_position.x, -m_position.y, 0.0f);
		m_cameraMatrix = glm::translate(m_orthoMatrix, translate);
		glm::vec3 scale(m_scale, m_scale, 1.0f);
		m_cameraMatrix = glm::scale(m_cameraMatrix, scale);
		m_flag = false;
	}
}
