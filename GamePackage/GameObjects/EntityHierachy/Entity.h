#pragma once
#include"../../2D/Renderer.h"
#include<vector>
#include<glm\glm.hpp>
class Entity {
	bool m_done = false;
public:
	virtual ~Entity(){}
	virtual void Update(float deltaTime) = 0;
	inline virtual void Draw(){}//optional
	inline void Done() { m_done = true; }
	inline bool HasDone() { return m_done; }

};
class AABBEntity :public Entity{
	glm::vec2 m_AA;
	glm::vec2 m_BB;

	bool m_selected = false;
	bool m_trigger = false;

public:
	virtual ~AABBEntity(){}

	//lower level: to call these in their implementation.
	virtual void Update(float deltaTime) override;
	virtual void Draw() ;

	//lower level: must implement these.
	virtual void OnSelect(const glm::vec2&AA,const glm::vec2&BB) = 0;
	virtual void OnRelease(const glm::vec2&AA, const glm::vec2&BB) = 0;

	//lower level: to provide AABB
	inline void SetAABB(const glm::vec2&AA, const glm::vec2&BB) { m_AA = AA; m_BB = BB; }





	//higher level: to check AABB
	inline const glm::vec2&GetAA() { return m_AA; }
	inline const glm::vec2&GetBB() { return m_BB; }

	//higher level: to select this entity
	inline void Select() { m_selected = true; }
	inline void Release() { m_selected = false; }

	//global use
	inline static void ShowAABB() { m_drawTrigger = true; }
	inline static void HideAABB() { m_drawTrigger = false; }
private:
	static bool m_drawTrigger;// to enable draw AABB for every child object
public:
	//ultilities
	bool CheckAABBAgainstPoint(const glm::vec2& point);
	bool CheckAABBAgainstAABB(const glm::vec2& aa2, const glm::vec2& bb2);
};
