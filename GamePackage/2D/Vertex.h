#pragma once
#include<glm\glm.hpp>
//struct Position {
//	float x;
//	float y;
//};
//struct Color {
//	GLubyte r, g, b, a;
//};
//struct UV {
//	float u, v;
//};

struct Vertex {
	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 uv;
	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}
	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
	void setUV(float u, float v) {
		uv.x = u;
		uv.y = v;
	}
};