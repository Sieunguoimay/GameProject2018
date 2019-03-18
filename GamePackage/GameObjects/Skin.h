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
	SpriterEntity*m_spriterEntity;

public:
	AnimationSkin(SpriterEntity*spriterEntity, float scale, glm::vec2 pos = glm::vec2(0.0, 0.0), float angle = 0.0);
	~AnimationSkin();

	void Draw()override;

	//the are specific functions, to use it, you have to cast the Skin Object.
	void Update(float deltaTime);
	inline void SetAnimation(int index) {m_spriterEntity->SetAnimation(index);}
	void SetAnimationSwitchingTime(const char*animationA, const char*animationB, int time);
	inline SpriterEntity*GetSpriterEntity() { return m_spriterEntity; }
};

//a skin that use static texture
class NoAnimationSkin :public Skin{
	Texture*m_pTexture;
public:
	NoAnimationSkin(Texture*texture);
	~NoAnimationSkin() { m_pTexture = NULL; }

	void Draw()override;
};