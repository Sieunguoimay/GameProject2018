#pragma once
#include"Shaders.h"
class Renderer {
protected:
	bool m_disabled = false;
public:

	virtual ~Renderer(){}
	virtual void Render(const float*matrix) = 0;

	void Disable() { m_disabled = true; }
};
