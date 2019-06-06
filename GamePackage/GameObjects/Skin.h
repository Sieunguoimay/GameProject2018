#pragma once
#include"SpriterEntity.h"

//store all the attributes to specify an image which are used at draw()
class Skin {
protected:
	glm::vec2 m_pos;
	glm::vec2 m_size;
	float m_angle;
	FlipType m_flip;
public:
	Skin(glm::vec2 pos = glm::vec2(0, 0), 
		glm::vec2 size = glm::vec2(10, 10), 
		float angle = 0, FlipType flip = FlipType::FLIP_NONE)
		:m_pos(pos),m_size(size),m_angle(angle),m_flip(flip){}

	virtual ~Skin(){}
	virtual void Draw() = 0;


	inline void SetSize(glm::vec2 size) { m_size = size; }
	inline void SetPos(glm::vec2 pos) { m_pos = pos; }
	inline void SetAngle(float angle) { m_angle = angle; }
	inline void SetFlip(FlipType flip) { m_flip = flip; }

	inline glm::vec2*GetPosPointer() { return &m_pos; }
	inline const FlipType& GetFlip()const { return m_flip; }
	inline const glm::vec2&GetPosition() { return m_pos; }
	inline float GetAngle() { return m_angle; }
	inline const glm::vec2&GetSize() { return m_size; }
};


//a skin that use animation

//assume that the SpriterEntity class already solve problem of boundary rectangle
//of an animation object.
class AnimationSkin:public Skin {

	bool m_firstTimeFlag;
	glm::vec4 m_originalAABB;
	glm::vec2 m_originalCenter;
	glm::vec4 m_AABBBuffer;
	glm::vec2 m_scale;

protected:
	SpriterEntity*m_spriterEntity;

public:
	AnimationSkin(SpriterEntity*spriterEntity, float scale, glm::vec2 pos = glm::vec2(0.0, 0.0), float angle = 0.0);
	virtual ~AnimationSkin();

	void Draw()override;
	virtual void Update(float deltaTime);

	void SetAABB(const glm::vec4&AABB);
	const glm::vec4& GetAABB() { return m_AABBBuffer; }
	const glm::vec2& GetScale() { return m_scale; }


	//the are specific functions, to use it, you have to cast the Skin Object.
	inline void SetAnimation(int index) {m_spriterEntity->SetAnimation(index);}
	inline SpriterEntity*GetSpriterEntity() { return m_spriterEntity; }
};

//a skin that use static texture
class NoAnimationSkin :public Skin{
	Texture*m_pTexture;
	glm::vec4 m_AABB;
	glm::vec2 m_center;
public:
	NoAnimationSkin(Texture*texture, const glm::vec2& pos=glm::vec2(0,0),const glm::vec2& size=glm::vec2(-1.0f), const float& angle=0.0);
	~NoAnimationSkin() { m_pTexture = NULL; }

	void Draw()override;
	inline Texture*GetTexture() { return m_pTexture; }
	inline const glm::vec4&GetAABB() { return m_AABB; }
	inline void SetCenter(const glm::vec2&center) { m_center = center; }
};
