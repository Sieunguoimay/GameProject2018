#include "Platform.h"
#include"../../misc/Assistances.h"
#include"../../misc/Locator.h"
Platform::Platform(Texture * pTexture)
	:BodyBase(glm::vec2(0.0f))
	,m_pTexture(pTexture)
{
	m_specifier = ObjectType::GROUND;
}

Platform::~Platform()
{
	for (auto&a : m_polygons)
		delete a;
}

void Platform::Init()
{
	createBody(m_position);
}

//infrastructure is ready for creating physics body for platform
void Platform::createBody(const glm::vec2&pos)
{
	m_body = NULL;
	b2PolygonShape ps;
	for (auto&a : m_polygons) {
		assert(a->m_vertices.size() <= b2_maxPolygonVertices);

		std::vector<b2Vec2>vertices;
		for (int i = 0; i < a->m_vertices.size();i++) {
			vertices.emplace_back(a->m_vertices[i].x / M2P, a->m_vertices[i].y / M2P);
			//SDL_Log("--- %f %f", vertices[i].x, vertices[i].y);
		}
		ps.Set(&vertices[0], vertices.size());

		if(m_body==NULL)
			m_body = Locator::GetPhysicsFactory()->CreateBody(&ps, b2_staticBody, MaterialType::SOIL, b2Vec2(pos.x / M2P, pos.y / M2P));
		else 
			m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(MaterialType::SOIL));
	}
	m_body->SetUserData(this);

}
