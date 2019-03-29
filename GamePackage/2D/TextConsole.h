
#pragma once
#include"FreeTypeLoader.h"
class TextConsole:public Renderer{
	FreeTypeLoader m_text;
	TextureRenderer*m_pRenderer;

	glm::vec2 m_pos;
	glm::vec4 m_color;
	FreeTypeLoader::Alignment m_align;
	int m_lineSpace;
	int m_charSpace;
	float m_scale;

	std::string m_logData;
public:
	TextConsole(TextureRenderer*pRenderer,const char*font_file_path,int pixel_height);
	void Log(const char*s, bool oneTimeFlag=false);

	void Render(const float*matrix) override;
	//gettters and setters
	void SetPos(float x, float y) { m_pos.x = x; m_pos.y = y; }
	void SetColor(float r, float g, float b, float a) { m_color = glm::vec4(r,g,b,a); }
	void SetAlign(FreeTypeLoader::Alignment align) { m_align = align; }
	void SetLineSpace(int space) { m_lineSpace = space; }
	void SetCharSpace(int space) { m_charSpace = space; }
	void SetScale(float scale) { m_scale = scale; }

	FreeTypeLoader*GetFreeTypeLoader() { return &m_text; }
};
