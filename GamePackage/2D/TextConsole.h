
#pragma once
#include"FreeTypeLoader.h"
#include"Renderer.h"
class TextConsole{
	FreeTypeLoader m_text;
	TextureRenderer*m_pRenderer;

	glm::vec2 m_pos;
	glm::vec4 m_color;
	FreeTypeLoader::Alignment m_align;
	int m_lineSpace;
	int m_charSpace;
	float m_scale;

	std::string m_logData;
	int m_counter;
public:
	TextConsole(TextureRenderer*pRenderer,const char*font_file_path,int pixel_height);
	~TextConsole();
	void Log(const char*s);

	void Draw();
	//gettters and setters
	void SetPos(float x, float y) { m_pos.x = x; m_pos.y = y; }
	void SetColor(float r, float g, float b, float a) { m_color = glm::vec4(r,g,b,a); }
	void SetAlign(FreeTypeLoader::Alignment align) { m_align = align; }
	void SetLineSpace(int space) { m_lineSpace = space; }
	void SetCharSpace(int space) { m_charSpace = space; }
	void SetScale(float scale) { m_scale = scale; }

};
