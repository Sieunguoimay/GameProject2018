#pragma once
#include"../GraphicsTree/GraphNode.h"
#include"../../2D/Texture.h"

class BoneRenderer : public NodeRenderer{
public:
	BoneRenderer(glm::vec4 box, Texture*texture);
	void Draw(Transform world, glm::vec2 localPos)override;

	glm::vec4 m_box;
	Texture*m_pTexture;
	bool m_hover;
};
class Bone :public GraphNode {
	BoneRenderer m_boneRenderer;
	float m_length;

	float m_angle;
	float m_angleSpeed;
	glm::vec2 m_vel;
	glm::vec2 m_pos;
public:
	Bone(float length,glm::vec4 box, Texture*texture);
	void Update(float deltaTime);
	void Draw(Transform parentWorld, bool dirty);

	void MoveTo(glm::vec2 targetLocal, float movingTime);
	void RotateTo(float angle,float rotatingTime);//degree
	void SetRotation(float angle)override;
	void SetOrigin(glm::vec2 local)override;

	glm::vec2 GetEndTerminal() { return glm::vec2(m_length, 0.0); }
	BoneRenderer& GetBoneRenderer() { return m_boneRenderer; }
	const float GetAngle() const { return m_angle; }
};
