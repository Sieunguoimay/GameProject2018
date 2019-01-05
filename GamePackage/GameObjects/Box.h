#pragma once
#include<Box2D/Box2D.h>
#include<glm\glm.hpp>
#include"../2D/Renderer.h"
#include"../2D/Texture.h"
#include"Entity.h"
class Box:public Entity
{
	//physics component
	b2Body*m_body;
	b2Fixture*m_fixture;
	b2World *m_pWorld;

	//graphics component
	glm::vec2 m_dimension;
	Texture*m_pTexture;

	glm::vec2 m_position;


public:
	Box(b2World*world, const glm::vec2& pos, const glm::vec2& dimension,Texture*texture,bool fixedRotation =false);
	~Box();
	void Draw();
	void Update(float deltaTime);

	b2Body*GetBody()const { return m_body; }
	b2Fixture*GetFixture()const { return m_fixture; }
	const glm::vec2& GetPosition() { return m_position; }
	const glm::vec2& GetDimension()const { return m_dimension; }
};

