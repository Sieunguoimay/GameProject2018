#include"Sprite.h"
#include"../misc/GameAccessor.h"
#include"../Graphics/ResourceManager.h"
#include"glm\glm.hpp"
#include"glm\gtc\matrix_transform.hpp"
Sprite::Sprite(Mesh*mesh, Texture*texture, Shaders*m_pShader)
	:m_pMesh(mesh),m_pTexture(texture),m_pShader(m_pShader),
	m_width(0.0f),
	m_height(0.0f),
	m_colNum(1),
	m_rowNum(1)
{
	m_width = m_pTexture->GetWidth() / m_colNum;
	m_height = m_pTexture->GetHeight() / m_rowNum;
	m_pixelToOpenGL = s_gameAccessor.PixelToOpenGL();
}
Sprite::~Sprite() {
	m_pMesh = NULL;
	m_pTexture = NULL;
	m_pShader = NULL;
}
//public use
void Sprite::Draw(int xIndex, int yIndex, float x, float y, float w, float h, float angle, float alpha, float z, int flip) {
	float scaleX = (flip == FLIP_VERTICAL ? -1.0f : 1.0f)*w * m_pixelToOpenGL;
	float scaleY = (flip == FLIP_HORIZONTAL ? -1.0f : 1.0f)*h * m_pixelToOpenGL;
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(x*m_pixelToOpenGL, y* m_pixelToOpenGL, z * m_pixelToOpenGL));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0f));
	
	m_pShader->UseProgram();
	glUniformMatrix4fv(m_pShader->u_model, 1, GL_FALSE, &(model[0][0]));
	glUniform2f(m_pShader->u_texSize, (float)m_colNum, (float)m_rowNum);
	glUniform2f(m_pShader->u_spriteOffset, (float)xIndex, (float)yIndex);
	glUniform1f(m_pShader->u_alpha, alpha);

	m_pTexture->Bind();
	m_pMesh->Draw(m_pShader);

}
