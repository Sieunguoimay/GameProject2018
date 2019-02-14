#pragma once
#include"../2D/Renderer.h"
#include<vector>
#include<glm\glm.hpp>
class Entity {
	bool m_done = false;
public:
	virtual ~Entity(){}
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	
	inline void Done() { m_done = true; }
	inline bool HasDone() { return m_done; }

};