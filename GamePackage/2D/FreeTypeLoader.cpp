#include "FreeTypeLoader.h"
#include"../misc/Assistances.h"

FreeTypeLoader::FreeTypeLoader()
	:m_flag(false)
{
	SDL_Log("Created FreeType");
}

FreeTypeLoader::~FreeTypeLoader()
{
	for (int i = 0; i < MAX_CHAR; i++)
		glDeleteTextures(1, &m_characters[i].textureId);

	SDL_Log("Deleted FreeType");
}

void FreeTypeLoader::Init(const char* font_file, int char_height)
{
	TTF_Font*font = TTF_OpenFont(font_file,char_height);
	if (font) {
		createTextures(font);
		TTF_CloseFont(font);
	}

	//FT_Library freetype;
	//FT_Face face;
	//if (FT_Init_FreeType(&freetype)) {
	//	SDL_Log("Error: Couldnot Init the FreeType");
	//	m_flag = true;
	//	return;
	//}
	//if (FT_New_Face(freetype, font_file, 0, &face)) {
	//	SDL_Log("Error: Couldnot load the font file %s", font_file);
	//	m_flag = true;
	//	return;
	//}

	//FT_Set_Pixel_Sizes(face, 0, char_height);

	////process the face to get textures
	//createTextures(face);

	//FT_Done_Face(face);
	//FT_Done_FreeType(freetype);
	//SDL_Log("FreeType initialized with font %s", font_file);

}
//void FreeTypeLoader::createTextures(FT_Face&face) {
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//Disable byte-alignment restriction
//	for (int c = 0; c < MAX_CHAR; c++) {
//		//load character glyph
//		if (FT_Load_Char(face, (char)c, FT_LOAD_RENDER)) {
//			SDL_Log("Error: Failed to load glyph of %c %d", c, c);
//			continue;
//		}
//		//generate Texture
//		GLuint texture=0;
//		glGenTextures(1, &texture);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
//			face->glyph->bitmap.width,
//			face->glyph->bitmap.rows,
//			0, GL_ALPHA, GL_UNSIGNED_BYTE,
//			face->glyph->bitmap.buffer);
//
//		if ((char)c == STANDARD_CHAR)
//			m_standardSize = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		CharacterTexture character = {
//			texture,
//			glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
//			glm::ivec2(face->glyph->bitmap_left,face->glyph->bitmap_top),
//			face->glyph->advance.x
//		};
//		m_characters[c] = character;//copy baby
//		if(c>='0'&&c<='9')
//			SDL_Log("%c %d %d %d %d",c, character.size.x, character.size.y, character.bearing.x, character.bearing.y);
//	}
//}

void FreeTypeLoader::createTextures(TTF_Font*font)
{
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//Disable byte-alignment restriction

	SDL_Color color = { 0,0,0,255};
	int ascent = TTF_FontAscent(font);
	for (int c = 0; c < MAX_CHAR; c++) {
		int minx = 0;
		int maxx = 0;
		int miny = 0;
		int maxy = 0;
		int advance = 0;
		SDL_Surface*surface = TTF_RenderGlyph_Blended(font, c, color);
		int success = TTF_GlyphMetrics(font, c, &minx, NULL, NULL, &maxy, &advance);
		if (surface) {

			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				surface->w,
				surface->h,
				0, GL_RGBA, GL_UNSIGNED_BYTE,
				surface->pixels);
			SDL_FreeSurface(surface);


			if ((char)c == STANDARD_CHAR)
				m_standardSize = glm::ivec2(surface->w,surface->h);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			CharacterTexture character = {
				texture,
				glm::ivec2(surface->w,surface->h),
				glm::ivec2(minx,ascent-maxy),
				advance
			};
			m_characters[c] = character;//copy baby
		}
	}

}

//FT_Load_Char() to load the glyph of a character stored inside the m_face.

void FreeTypeLoader::Draw(TextureRenderer*pTextureRenderer,const char*text, glm::vec2 pos, int lineSpace,int charSpace,float scale,Alignment align,glm::vec4 color)
{
	if (m_flag) return;
	//std::vector<int> lineWidth;
	//if (align == ALIGN_CENTER || ALIGN_RIGHT) {
	//	auto s = split(text, '\n');
	//	for (auto&w : s) {
	//		int width = 0;
	//		for (auto&c : w) {
	//			if (c == ' ') {
	//				width += m_standardSize.x;
	//				continue;
	//			}
	//			width += m_characters[(int)c].size.x;				
	//		}
	//		lineWidth.push_back(width);
	//	}
	//}

	float lengthX = 0;
	float lengthY = 0;
	float offsetX = 0;

	//std::vector<int>::iterator it = lineWidth.begin();
	//if (align == ALIGN_RIGHT)
	//	offsetX = -*it;
	//else if(align == ALIGN_CENTER)
	//	offsetX = -*it/2;

	//for (auto&c:text) {
	unsigned int i = 0;
	while (text[i] != '\0'){
		char c = text[i++];
		if (c == '\n') {
			lengthX = 0;
			lengthY -= (m_standardSize.y + lineSpace);
			//if (align == ALIGN_RIGHT) {
			//	it++;
			//	offsetX = -*it;
			//}
			//else if (align == ALIGN_CENTER) {
			//	it++;
			//	offsetX = -*it / 2;
			//}
			continue;
		}

		if ((int)c == 32|| c > MAX_CHAR) {
			lengthX += m_standardSize.x;
			continue;
		}

		CharacterTexture&textCharacter = m_characters[(int)c];

		glm::vec4 rect;
		rect.x = pos.x + (lengthX+textCharacter.bearing.x+offsetX)*scale;
		rect.y = pos.y - (textCharacter.size.y - textCharacter.bearing.y - lengthY)*scale;
		rect.z = textCharacter.size.x*scale;
		rect.w = textCharacter.size.y*scale;
		lengthX += (rect.z + charSpace);

		pTextureRenderer->Draw(rect, glm::vec4(0.0, 0.0, 1.0, 1.0),
			textCharacter.textureId, 0, color, 0.0f,
			glm::vec2(0.0f, 0.0f), false, false);
	}
}
