#pragma once
#include"SDL2\SDL.h"
#include"glm\glm.hpp"
#include"../Controls/InputHolder.h"
#include<vector>
#include"Shaders.h"
class CameraController;
class Camera{
	glm::vec3 m_pos;
	glm::vec3 m_front;

	glm::vec3 m_worldUp;

	glm::vec3 m_up;
	glm::vec3 m_right;

	glm::mat4 m_view;

	glm::vec3 m_vel;
	float m_speed;
	float m_yaw;
	float m_pitch;

	bool m_trigger;

	friend class CameraController;
	CameraController*m_controller;
	void CalculateFront();
public:
	Camera();
	~Camera();
	void Init(glm::vec3 pos, glm::vec3 front );
	void Update(float deltaTime);
	void SendMatrix(std::vector<Shaders*>&shaders);

	//getters and setters
	glm::vec3&GetPos() { return this->m_pos; }
	glm::vec3&GetFront() { return this->m_front; }
	glm::vec3&GetUp() { return this->m_up; }
	glm::vec3&GetRight() { return this->m_right; }
	glm::mat4&GetViewMatrix() { return this->m_view; }
	glm::vec3&GetVel() { return this->m_vel; }
	float GetSpeed() { return m_speed; }

	void SetPos(glm::vec3 pos) { this->m_pos = pos; }
	void SetFront(glm::vec3 front);
	void SetWorldUp(glm::vec3 world_up) { this->m_worldUp = world_up; }
	void SetSpeed(float speed) { this->m_speed = speed; }
	void SetVel(glm::vec3 vel) { this->m_vel = vel; }
	void SetVel(float x, float y, float z) { this->m_vel = glm::vec3(x,y,z); }

	void SetController(CameraController*controller) { this->m_controller = controller; }
	CameraController*GetController() { return this->m_controller; }
};



class CameraController{
	Camera*m_pCamera;
	void processMouse(float x, float y);
	bool m_firstMouse;
	float m_lastX;
	float m_lastY;

	glm::vec2 m_target;
public:
	CameraController(Camera*camera);
	~CameraController();
	void Update(float deltaTime);
	void SetTarget(float x, float y);
};
