#pragma once
#include"../../2D/Renderer.h"
#include<vector>
#include<glm\glm.hpp>
#include"Spawner.h"
#include"PointerHolder.h"
class Entity {
	bool m_done = false;
public:
	inline virtual ~Entity(){}
	inline virtual void Init(){}

	virtual void Update(float deltaTime) = 0;
	inline virtual void Draw(){}//optional
	inline void Done() { m_done = true; }
	inline bool HasDone() { return m_done; }
};

//At the initialization, AABB tells where to put the object
//and what is the size

//at run time , it is the frame used in many other cases, such as
//-editing the object size/position based on this AABB
//-checking for the out-of-window objects so that it can be ignored
class AABBEntity :public Entity, public Spawner, public PointerHolder{

	bool m_selected = false;
	bool m_trigger = false;
protected:
	glm::vec4 m_AABB;//can be used to deliver AABB from constructor to SetupBody();
public:
	AABBEntity():m_selected(false),m_trigger(false){}
	virtual ~AABBEntity(){}

	//lower level: to call these in their implementation.
	virtual void Update(float deltaTime) override;
	virtual void Draw() ;

	//lower level: must implement these.
	virtual void OnSelect(const glm::vec4&AABB) = 0;
	virtual void OnRelease(const glm::vec4&AABB) = 0;

	//lower level: to provide AABB
	virtual const glm::vec4& CalculateAABB()=0;
	//lower level: maybe used at the initial state or during editing, to adjust the entity
	virtual void ApplyAABB(const glm::vec4&AABB) = 0;





	//higher level: to check AABB
	inline const glm::vec4&GetAABB() { return m_AABB; }
	//higher level: change the AABB while selected flag is On
	inline void SetAABB(const glm::vec4&aabb) { m_AABB = aabb; }

	//higher level: to select this entity
	inline void Select() { m_selected = true; }
	inline void Release() { m_selected = false; }
	inline bool IsSelected()const { return m_selected; }
public:
	//ultilities
	static bool s_drawAABBTrigger;// to enable draw AABB for every child object
};

