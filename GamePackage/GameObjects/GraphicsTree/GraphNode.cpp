#include "GraphNode.h"
#include<glm\gtc\matrix_transform.hpp>
#include"../../misc/Locator.h"
void NodeRenderer::Draw(Transform world, glm::vec2 localPos)
{
	glm::vec2 pos = glm::vec2(glm::vec3(localPos,1.0f)*world.m_matrix);//use the local position to calculate world position
	Locator::GetPrimitiveRenderer()->FillCircle(pos, 10);
}

GraphNode::~GraphNode()
{
	for (auto&c : m_children)
		delete c;
}
void GraphNode::Draw(Transform parentWorld, bool dirty)
{

	dirty |= m_dirty;
	if (dirty) {
		m_world = m_local.Combine(parentWorld);
		m_dirty = false;
	}
	//if (m_pEntity) renderEntity(m_pEntity, m_world);
	m_nodeRenderer.Draw(m_world,glm::vec2(0.0f,0.0f));

	for (auto&c : m_children)
		c->Draw(m_world,dirty);
}

void GraphNode::SetTransform(Transform local)
{
	m_local.m_matrix[0][2] = local.m_matrix[0][2];
	m_local.m_matrix[1][2] = local.m_matrix[0][2];
	m_dirty = true;
}


void GraphNode::AddChild(GraphNode * newChild)
{
	m_children.push_back(newChild);
}

void GraphNode::AddChild(GraphNode * newChild, glm::vec2 local)
{
	newChild->SetOrigin(local);
	m_children.push_back(newChild);
}

void GraphNode::SetOrigin(glm::vec2 origin){
	m_local.m_matrix[0][2] = origin.x;
	m_local.m_matrix[1][2] = origin.y;
	m_dirty = true;
}

void GraphNode::SetRotation(float angle)
{
	glm::mat4 rotation(1.0f);
	rotation = glm::mat3(glm::rotate(rotation, glm::radians(angle), glm::vec3(0.0, 0.0, -1.0f)));

	m_local.m_matrix[0][0] = rotation[0][0];
	m_local.m_matrix[0][1] = rotation[0][1];
	m_local.m_matrix[1][0] = rotation[1][0];
	m_local.m_matrix[1][1] = rotation[1][1];

	m_dirty = true;
}

