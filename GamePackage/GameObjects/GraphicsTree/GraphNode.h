#pragma once
#include<vector>
#include"Transform.h"
#include"../Entity.h"
class NodeRenderer {
public:
	virtual void Draw(Transform world,glm::vec2 localPos);
};
class GraphNode {
public:
	GraphNode()
		:m_local(Transform::Origin())
		, m_dirty(true)
	{}
	~GraphNode();
	virtual void Draw(Transform parentWorld, bool dirty);
	//virtual void renderEntity(Entity*entity, Transform world);

	void AddChild(GraphNode*newChild);
	void AddChild(GraphNode*newChild, glm::vec2 local);

	void SetTransform(Transform local);
	virtual void SetOrigin(glm::vec2 origin);
	virtual void SetRotation(float angle);

	glm::vec2 GetOriginInLocal() { return glm::vec2(glm::vec3(0, 0, 1)*m_local.m_matrix); }
	glm::vec2 GetOriginInWorld() { return glm::vec2(glm::vec3(0, 0, 1)*m_world.m_matrix); }
	const Transform& GetTransform() const { return m_world; }
	const Transform& GetLocalTransform()const { return m_local; }
protected:
	Transform m_local;
	Transform m_world;
	bool m_dirty;
	//entity here with the role of a mesh
	NodeRenderer m_nodeRenderer;
	std::vector<GraphNode*>m_children;
};