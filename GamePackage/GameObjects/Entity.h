#pragma once
#include"../2D/Renderer.h"
#include<vector>
#include<glm\glm.hpp>
class Entity {
public:
	virtual ~Entity(){}
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

};