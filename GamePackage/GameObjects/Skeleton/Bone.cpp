#include "Bone.h"
#include<glm/gtc/matrix_transform.hpp>
#include"../../misc/Locator.h"
BoneRenderer::BoneRenderer(glm::vec4 box, Texture*texture)
	:m_box(box),m_pTexture(texture)
	,m_hover(false)
{
}

void BoneRenderer::Draw(Transform world,glm::vec2 localPos)
{
	//glm::mat3 m = glm::mat3(glm::translate(glm::mat4(world.m_matrix), glm::vec3(m_box.x + m_box.z / 2, m_box.y + m_box.w / 2, 1.0f)));
	glm::vec2 bottom_left(m_box.x, m_box.y);
	glm::vec2 top_left(m_box.x, m_box.y + m_box.w);
	glm::vec2 top_right(m_box.x + m_box.z, m_box.y + m_box.w);

	bottom_left = glm::vec3(bottom_left, 1.0f)*world.m_matrix;
	top_left = glm::vec3(top_left, 1.0f)*world.m_matrix;
	top_right = glm::vec3(top_right, 1.0f)*world.m_matrix;

	glm::vec2 diff1 = top_right - top_left;
	float angle = atan2f(diff1.y, diff1.x);

	glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
	if (m_hover) color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(bottom_left.x, bottom_left.y, m_box.z, m_box.w), angle,glm::vec2(0,0),color);
	Locator::GetPrimitiveRenderer()->FillCircle(bottom_left, 5);

	if(m_pTexture)
		Locator::GetTextureRenderer()->Draw(glm::vec4(bottom_left.x, bottom_left.y, m_box.z, m_box.w), glm::vec4(0, 0, 1, 1),m_pTexture->GetId(),
			0,glm::vec4(1.0f,1.0f,1.0f,1.0f),angle, glm::vec2(0, 0));
}




Bone::Bone(float length, glm::vec4 box, Texture * texture)
	:GraphNode()
	,m_length(length)
	,m_angle(0)
	,m_angleSpeed(0)
	,m_boneRenderer(box, texture)
	,m_vel(0, 0)
	, m_pos(0,0)
{
}

void Bone::Update(float deltaTime)
{
	SetRotation(m_angle+m_angleSpeed*deltaTime);
	SetOrigin(m_pos + m_vel*deltaTime);
}

void Bone::Draw(Transform parentWorld, bool dirty)
{
	GraphNode::Draw(parentWorld,dirty);
	m_boneRenderer.Draw(m_world,glm::vec2(0,0));
}

void Bone::MoveTo(glm::vec2 targetLocal, float movingTime)
{
	if (movingTime <= 0.005) movingTime = 0.005f;
	m_vel = (targetLocal-m_pos)/movingTime;
}

void Bone::RotateTo(float angle,float rotatingTime)
{
	if (rotatingTime < 0.005) rotatingTime = 0.005f;
	m_angleSpeed = (angle - m_angle)/rotatingTime;
}

void Bone::SetRotation(float angle)
{
	GraphNode::SetRotation(angle);
	m_angle = angle;
}

void Bone::SetOrigin(glm::vec2 local)
{
	GraphNode::SetOrigin(local);
	m_pos = local;
}
