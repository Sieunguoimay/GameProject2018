#pragma once
#include"TextureRenderer.h"
#include<ft2build.h>
#include FT_FREETYPE_H
struct CharacterTexture {
	GLuint textureId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	long int advance;//offset to advance to the next glyph 
};
#define MAX_CHAR 256
#define STANDARD_CHAR 'h'
class FreeTypeLoader{
public:
	enum Alignment {
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_CENTER
	};

private:
	bool m_flag;
	glm::ivec2 m_standardSize;
	CharacterTexture m_characters[MAX_CHAR];
	void createTextures(FT_Face&face);
public:
	FreeTypeLoader();
	~FreeTypeLoader();
	void Init(const char* font_file, int char_height);
	void Draw(TextureRenderer*m_pTextureRenderer,std::string text, glm::vec2 pos,int lineSpace = 2, int charSpace = 2,float scale = 1.0f, Alignment align = ALIGN_LEFT, glm::vec4 color = glm::vec4(0.0f,0.0f,0.0f,1.0f));
};