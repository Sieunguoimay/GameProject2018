#pragma once
#include"Entity.h"
#include"../Skin.h"

//why do these two classes look the same???
//even though they are similar
//they are better separate like this
//because:
//- each has its own way of calculating the AABB
//- also way of applying AABB
//- the attribute m_skin, better separate like that, for conveinient purpose
//- if we merge them together, there're still some differences needed tobe specified
//----> thus, don't think too much.... simply don't worry.

class AnimationNoBodyEntity :public AABBEntity {
protected:
	AnimationSkin*m_skin;
	int m_animationIndexBuffer;
public:
	AnimationNoBodyEntity(AnimationSkin*skin,glm::vec4 AABB );
	virtual ~AnimationNoBodyEntity();

	void Update(float deltaTime)override;
	void Draw()override;

	void OnSelect(const glm::vec4&AABB)override;
	void OnRelease(const glm::vec4&AABB)override;
	void ApplyAABB(const glm::vec4&AABB)override;

	const glm::vec4&CalculateAABB() override { return m_skin->GetAABB(); }
};



class NoAnimationNoBodyEntity :public AABBEntity {
	glm::vec2 m_AABBDimensionBuffer;
	glm::vec2 m_sizeBuffer;
protected:
	NoAnimationSkin*m_skin;
public:
	NoAnimationNoBodyEntity(NoAnimationSkin*skin,
		glm::vec2 m_pos = glm::vec2(0.0f),
		glm::vec2 m_size = glm::vec2(10.0f));
	virtual ~NoAnimationNoBodyEntity();

	void Draw()override;

	void OnSelect(const glm::vec4&AABB)override;
	void OnRelease(const glm::vec4&AABB)override;
	void ApplyAABB(const glm::vec4&AABB)override;

	const glm::vec4&CalculateAABB() override{ return m_skin->GetAABB(); }
};
