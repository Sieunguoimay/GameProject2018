#include "VertexObject.h"
#include"../../misc/Assistances.h"
#include"../../misc/Locator.h"
#include"Platform.h"

VertexObject::VertexObject():m_pPlatform(NULL){}

VertexObject::VertexObject(Platform * platform, const glm::ivec2& vertexIndex /*= glm::ivec2(0, 0)*/)
	:m_pPlatform(platform), m_vertexIndex(vertexIndex)
{
	if (platform == NULL)return;
	m_AABB = CalculateAABB();
}

void VertexObject::Attach(AABBEntity * entity)
{
	for (Node<AABBEntity*>*it = m_attachedEntities.first(); it != m_attachedEntities.tail; it=it->next) 
		if (it->data == entity) return;

	m_attachedEntities.push_back(NULL); 
	m_attachedEntities.last()->data = (AABBEntity*)entity->AddPointer((void**)&m_attachedEntities.last()->data);
}

void VertexObject::Dettach(AABBEntity * entity)
{
	for (Node<AABBEntity*>*it = m_attachedEntities.first(); it != m_attachedEntities.tail; it = it->next)
		if (it->data == entity)
			it = m_attachedEntities.erase(it);
}



void VertexObject::Draw()
{
	AABBEntity::Draw();


	glm::vec2 pos = glm::vec2((m_AABB.x + m_AABB.z) / 2.0f, (m_AABB.y + m_AABB.w) / 2.0f);
	for (Node<AABBEntity*>*it = m_attachedEntities.first(); it != m_attachedEntities.tail; it = it->next){
		if (it->data == NULL) {
			it = m_attachedEntities.erase(it);
			continue;
		}
		AABBEntity*a = it->data;
		glm::vec2 pos2 = glm::vec2((a->GetAABB().x + a->GetAABB().z) / 2.0f, (a->GetAABB().y + a->GetAABB().w) / 2.0f);
		Locator::GetPrimitiveRenderer()->DrawLine(pos, pos2,glm::vec4(0,0,1,1));
	}
}

Spawner * VertexObject::Spawn(InfoPacket * info)
{
	VertexObjectData*data = (VertexObjectData*)info->GetData();
	Spawner*a=(new VertexObject(data->m_pPlatform, data->m_vertexIndex))->SetId(info->GetId());
	delete info;
	return a;
}

glm::vec4 VertexObject::CalculateAABB()
{

	glm::vec2 platform_pos(m_pPlatform->GetAABB().x, m_pPlatform->GetAABB().y);
	const b2Vec2& pos = m_pPlatform->m_polygons[m_vertexIndex.x][m_vertexIndex.y];
	
	return glm::vec4(
		M2P*pos.x - 20 + platform_pos.x,
		M2P*pos.y - 20 + platform_pos.y,
		M2P*pos.x + 20 + platform_pos.x,
		M2P*pos.y + 20 + platform_pos.y);/*unchanged */
};