#pragma once
#include<glm\glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
class Camera2D
{
	bool m_flag;
	float m_scale;
	glm::vec2 m_position;
	glm::mat4 m_orthoMatrix;
	glm::mat4 m_cameraMatrix;

public:
	Camera2D();
	~Camera2D();
	void Init(int windowWidth, int windowHeight);
	void Update(float deltaTime);

	//getters and setters
	void SetPos(glm::vec2 pos) { this->m_position = pos; m_flag = true; }
	void SetScale(float scale) { this->m_scale = scale; m_flag = true; }
	glm::vec2 GetPosition() { return this->m_position; }
	glm::mat4&GetMatrix() { return this->m_cameraMatrix; }
	glm::vec2 CameraToWorld(glm::vec2 point) { return point + m_position; }
};

