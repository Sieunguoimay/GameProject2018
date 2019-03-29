#include "Platform.h"
#include"../../misc/Assistances.h"
#include"../../misc/Locator.h"
#include"../ObjectPool.h"
Platform::Platform()
	: NoAnimationBodyEntity(NULL, ObjectType::NULL_TYPE, glm::vec4())
	,InfoPacket(ObjectId::OID_PLATFORM, NULL)
{
}
Platform::Platform(Texture*pTexture, const glm::vec4& AABB)
	:NoAnimationBodyEntity(new NoAnimationSkin(pTexture),ObjectType::NULL_TYPE,AABB)
	,InfoPacket(ObjectId::OID_PLATFORM,NULL)
	, m_scaleBuffer(1.0f,1.0f)
{
	m_originalDimension = glm::vec2(m_AABB.z - m_AABB.x, m_AABB.w - m_AABB.y);
}

Platform::~Platform()
{
}

Spawner * Platform::Spawn(InfoPacket * info)
{
	return ((Platform*)info)->SetId(info->GetId());
}

//override entire NoAnimationBodyEntity::Update()
void Platform::Update(float deltaTime)
{
	//super
	AABBEntity::Update(deltaTime);
	if (AABBEntity::IsSelected())return;

	b2Vec2 pos = M2P*m_body->GetPosition();
	m_skinBase->SetPos(glm::vec2(pos.x, pos.y)+m_skin->GetSize()/2.0f);
	m_skinBase->SetAngle(m_body->GetAngle());
}

//infrastructure is ready for creating physics body for platform
void Platform::SetupBody(bool atRunTime /*= false*/)
{
	glm::vec2 pos = glm::vec2(m_AABB.x,m_AABB.y);
	glm::vec2 dimension = glm::vec2(m_AABB.z - m_AABB.x, m_AABB.w - m_AABB.y);
	m_scaleBuffer = dimension / m_originalDimension;
	
	m_body = NULL;
	for (auto&a : m_polygons) {
		assert(a.size() <= b2_maxPolygonVertices);

		b2PolygonShape ps;
		if (atRunTime) {
			std::vector<b2Vec2>vertices;
			for (auto&v : a)
				vertices.push_back(b2Vec2(v.x*m_scaleBuffer.x, v.y*m_scaleBuffer.y));
			ps.Set(&vertices[0], vertices.size());
		}
		else 
			ps.Set(&a[0], a.size());
		
		

		if(m_body==NULL)
			m_body = Locator::GetPhysicsFactory()->CreateBody(&ps, b2_staticBody, MaterialType::SOIL, b2Vec2(pos.x / M2P, pos.y / M2P));
		else 
			m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(MaterialType::SOIL));
		
	}
	m_body->SetUserData(this);
}
