#pragma once
#include<glm\glm.hpp>
#include<glm/gtc/matrix_transform.hpp>


class Camera2D
{
	bool m_flag;
	float m_scale;
	glm::vec2 m_size;
	glm::vec2 m_position;
	glm::mat4 m_orthoMatrix;
	glm::mat4 m_cameraMatrix;

	glm::vec2 m_physicalSize;
	glm::vec4 m_AABB;
public:
	Camera2D();
	~Camera2D(){}

	void Init(const glm::vec2& size, const glm::vec2& physicalSize);
	void Update(float deltaTime);

	//getters and setters
	inline void SetPos(glm::vec2 pos) { this->m_position=pos; m_flag = true; }
	inline void SetScale(float scale) { this->m_scale = scale; m_flag = true; }

	inline const glm::vec2& GetPosition() const { return m_position; }
	inline glm::mat4& GetMatrix() { return this->m_cameraMatrix; }
	inline const glm::vec2& CameraToWorld(glm::vec2 point) { return point + m_position; }
	inline const glm::vec2& GetSize() { return m_size; }
	inline const glm::vec2& GetPhysicalSize() { return m_physicalSize; }

	inline glm::vec4* GetAABB() { return &m_AABB; }
};

