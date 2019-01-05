#pragma once
#include"Shaders.h"
class Renderer {
protected:
public:
	virtual ~Renderer(){}
	virtual void Render(const float*matrix) = 0;
};
