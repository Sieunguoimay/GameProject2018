#pragma once
#include"../../misc/list.h"
#include"../EntityHierachy/Entity.h"
class Platform;
struct VertexObjectData {
	Platform*m_pPlatform;
	glm::ivec2 m_vertexIndex;
};

class VertexObject :public AABBEntity {
	//Full access to the platform - for the absolute position
	Platform*m_pPlatform;
	
	//keeping the index of the vertex is good enough
	glm::ivec2 m_vertexIndex;

	//store the attached objects to this vertex	
	List<AABBEntity*>m_attachedEntities;
public:
	VertexObject();//for the prototype purpose
	VertexObject( Platform*platform, const glm::ivec2& vertexIndex );
	
	void Draw()override;
	void Attach(AABBEntity*entity);
	void Dettach(AABBEntity*entity);
	inline List<AABBEntity*>&GetAttachedEntitites() { return m_attachedEntities; }

	Spawner*Spawn(InfoPacket*info)override;
	virtual glm::vec4 CalculateAABB();
	//no use
	virtual void OnSelect(const glm::vec4&AABB) {};
	virtual void OnRelease(const glm::vec4&AABB) {};
	virtual void ApplyAABB(const glm::vec4&AABB) {}
};
