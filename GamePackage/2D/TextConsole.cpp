#include "TextConsole.h"

TextConsole::TextConsole(TextureRenderer * pRenderer, const char * font_file_path, int pixel_height)
	:m_pRenderer(pRenderer),
	m_pos(0.0f,0.0f),
	m_color(0.0f,0.0f,0.0f,1.0f),
	m_align(FreeTypeLoader::ALIGN_LEFT),
	m_charSpace(5),
	m_lineSpace(20),
	m_scale(1.0f),
	m_counter(0)
{
	m_text.Init(font_file_path, pixel_height);
}

TextConsole::~TextConsole()
{
}

void TextConsole::Log(const char*s)
{
	m_logData.append(s);
	m_logData.append("\n");
	m_counter++;
}

void TextConsole::Draw()
{
	if (!m_counter) return;
	m_text.Draw(m_pRenderer, m_logData.c_str(), m_pos, m_lineSpace, m_charSpace, m_scale, m_align, m_color);
	m_logData.clear();
	m_counter = 0;
}
