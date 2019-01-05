#include "Box.h"
#include"../misc/Locator.h"


Box::Box(b2World * world, const glm::vec2 & pos, const glm::vec2 & dimension,Texture*texture, bool fixedRotation)
	:m_pWorld(world),m_dimension(dimension),m_pTexture(texture)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pos.x, pos.y);//Example: if x is 100 then position.x is 1.0f
	bd.fixedRotation = fixedRotation;
	m_body = m_pWorld->CreateBody(&bd);

	b2PolygonShape ps;
	ps.SetAsBox(dimension.x/200.0f, dimension.y/200.0f);

	b2FixtureDef fd;
	fd.shape = &ps;
	fd.density = 2.0f;
	fd.restitution = 0.0f;
	fd.friction = 0.5f;
	m_fixture = m_body->CreateFixture(&fd);
}

Box::~Box()
{
	m_pWorld->DestroyBody(m_body);
}
void Box::Update(float deltaTime) {
	m_position = glm::vec2(
		(m_body->GetPosition().x*100.0f),
		(m_body->GetPosition().y*100.0f));
}
void Box::Draw()
{
	//Color color; color.r = 255, color.g = 255, color.b = 255, color.a = 255;
	//glm::vec4 destRect = glm::vec4(
	//	m_position.x - m_dimension.x / 2.0f,
	//	m_position.y - m_dimension.y / 2.0f,m_dimension);
	//Locator::GetTextureRenderer()->Draw(destRect,glm::vec4(0,0,1,1),
	//	m_pTexture->GetId(),0.0f, 
	//	color, m_body->GetAngle());

	//Locator::GetPrimitiveRenderer()->DrawBox(destRect,m_body->GetAngle());
}
