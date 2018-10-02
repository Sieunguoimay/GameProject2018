#pragma once
#include"glm\glm.hpp"
#include<vector>
#include"Shaders.h"
class Light {
	enum {
		SUN,LIGHT_BULB
	};
	int m_type; 
	int m_id;

	glm::vec3 m_color;
	glm::vec3 m_pos;

	float m_constant;
	float m_linear;
	float m_quadratic;

	std::vector<Shaders*>m_shaders;
public:
	Light();
	~Light();
	bool Init(int id,int type,glm::vec3 pos, glm::vec3 color,float constant,float linear, float quadratic);
	
	void SendDataToShaders(Shaders*shader,int id);

	void AddShader(Shaders*shader) { m_shaders.push_back(shader); }
	void SetPos(glm::vec3 pos) { this->m_pos = pos; }
	void SetColor(glm::vec3 color) { this->m_color = color; }
	const glm::vec3&GetColor() { return this->m_color; }
	const glm::vec3&GetPos() { return this->m_pos; }

	const float&GetConstant(){ return this->m_constant; }
	const float&GetLinear(){ return this->m_linear; }
	const float&GetQuadratic(){ return this->m_quadratic; }

	const int&GetType() { return this->m_type; }
};
