#include"Camera.h"
#include"glm\gtc\matrix_transform.hpp"
#include"ResourceManager.h"
void Camera::CalculateFront()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	front.y = sin(glm::radians(m_pitch));
	front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_front = glm::normalize(front);

}
Camera::Camera()
	:m_pos(0.0,0.0,0.0),
	m_vel(0.0f,0.0f,0.0f),
	m_worldUp(0.0,1.0,0.0),

	m_front(0.0,0.0,-1.0),
	m_up(0.0,1.0,0.0),
	m_right(1.0,0.0,0.0),

	m_view(1.0f),
	m_speed(0.5f),
	m_pitch(0.0f),
	m_yaw(0.0f)
{
	m_controller = new CameraController(this);
	m_trigger = true;
}
Camera::~Camera() {
	delete m_controller;
	m_controller = NULL;
}

void Camera::Init(glm::vec3 pos, glm::vec3 front)
{
	this->m_pos = pos;
	this->m_front = front;
	m_right = glm::normalize(glm::cross(m_worldUp, m_front));
	m_up = glm::cross(m_front, m_right);
	m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
}

void Camera::Update(float deltaTime) {
	m_controller->Update(deltaTime);
	if (m_trigger) {
		m_pos += deltaTime*m_vel;
		m_right = glm::normalize(glm::cross(m_worldUp, m_front));
		m_up = glm::cross(m_front, m_right);
		m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
		SendMatrix(*s_resourceManager.GetAllShaders());
		m_trigger = false;
	}
}

void Camera::SendMatrix(std::vector<Shaders*>& shaders)
{
	for (auto&shader : shaders) {
		shader->UseProgram();
		glUniformMatrix4fv(shader->u_view, 1, GL_FALSE, &(m_view[0][0]));
		glUniform1f(shader->u_camZ, m_pos.z);
	}
}

void Camera::SetFront(glm::vec3 front) {
	this->m_front = front;
	this->m_pitch = asin(front.y);
	this->m_yaw = atan2(front.x, front.z);
}







#include"../misc/GameAccessor.h"

CameraController::CameraController(Camera * camera)
	:m_pCamera(camera)
{
	m_firstMouse = true;
}

CameraController::~CameraController()
{
	m_pCamera = NULL;
}

void CameraController::Update(float deltaTime)
{
	m_pCamera->m_pos.x = m_target.x;
	m_pCamera->m_pos.y = m_target.y;
	m_pCamera->m_trigger = true;
}

void CameraController::SetTarget(float x, float y)
{
	m_target.x = x*s_gameAccessor.PixelToOpenGL();
	m_target.y = y*s_gameAccessor.PixelToOpenGL();
}

void CameraController::processMouse(float xpos, float ypos)
{
	if (m_firstMouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}

	float xoffset = m_lastX - xpos;
	float yoffset = ypos - m_lastY;
	m_lastX = xpos;
	m_lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_pCamera->m_yaw += xoffset;
	m_pCamera->m_pitch += yoffset;

	if (m_pCamera->m_pitch > 89.0f)
		m_pCamera->m_pitch = 89.0f;
	if (m_pCamera->m_pitch < -89.0f)
		m_pCamera->m_pitch = -89.0f;
	m_pCamera->CalculateFront();
}
