#pragma once
#include"../2D/Texture.h"
#include"SpriterEngine/TimelineKey.h"

//a skin that use static texture
class NoAnimationSkin /*:public Skin*/{
	Texture*m_pTexture;

	glm::vec2 m_center;//0-1
	float m_angle;
	FlipType m_flip;
	glm::vec2 m_pos;
	glm::vec2 m_size;

	bool m_aabbFlag = true;
public:
	NoAnimationSkin(Texture*texture);
	~NoAnimationSkin()						{ m_pTexture = NULL; }

	void Draw();
	void SetCenter(const glm::vec2&center)	{ m_center = center; }

	void SetSize(glm::vec2 size)			{ m_size = size; }
	void SetPos(glm::vec2 pos)				{ m_pos = pos; }
	void SetAngle(float angle)				{ m_angle = angle; }
	void SetFlip(FlipType flip)				{ m_flip = flip; }

	const FlipType& GetFlip()const			{ return m_flip; }
	const glm::vec2& GetPosition() const	{ return m_pos; }
	float GetAngle() const					{ return m_angle; }
	const glm::vec2&GetSize()const			{ return m_size; }
	Texture*GetTexture()					{ return m_pTexture; }
	glm::vec4 GetAABB()						{ return glm::vec4(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2); }

};
