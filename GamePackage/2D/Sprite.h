#pragma once
#include"../Graphics/Model3D.h"
//new sprite for new object
//why reuse this? why points to it?
//One Sprite One texture
class Sprite {
protected:

	float m_width;
	float m_height;

	int m_colNum;
	int m_rowNum;

	//utilities
	float m_pixelToOpenGL;//opengl unit
	Texture*m_pTexture;
	Mesh*m_pMesh;

	GLuint vboId;
	GLuint eboId;
	Shaders*m_pShader;

public:
	Sprite(Mesh*mesh,Texture*texture, Shaders*shaders);
	~Sprite();

	//public use
	void Draw(int xIndex, int yIndex, float x, float y, float w, float h,float angle = 0.0f,float alpha=1.0f,float z = 0.0f,int flip = FLIP_DEFAULT);


	//initialization...
	void SetColNum(int colNum) { this->m_colNum = colNum; }
	void SetRowNum(int rowNum) { this->m_rowNum = rowNum; }
	//getters
	const float&GetWidth() { return this->m_width; }
	const float&GetHeight() { return this->m_height; }
	const int&GetRowNum() { return this->m_rowNum; }
	const int&GetColNum() { return this->m_colNum; }
	int GetSpriteNum(){ return (m_rowNum*m_colNum); }
	enum {
		FLIP_DEFAULT,
		FLIP_HORIZONTAL,
		FLIP_VERTICAL
	};
};
