#include"Model3D.h"
#include"glm\gtc\matrix_transform.hpp"
#include"../misc/Assistances.h"
Model3D::Model3D()
	:m_pMesh(NULL), m_pShaders(NULL),
	m_pos(0.0f, 0.0f, 0.0f),
	m_rot(0.0f, 1.0f, 0.0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_worldMaxtrix(1.0f),
	m_angle(0)
{
}

Model3D::~Model3D()
{
	m_pMesh = NULL;
	m_pShaders = NULL;
}
void Model3D::Update(float deltaTime) {
	m_worldMaxtrix = glm::mat4(1.0f); //T*R*S
	m_worldMaxtrix = glm::translate(m_worldMaxtrix, m_pos);
	m_worldMaxtrix = glm::rotate(m_worldMaxtrix, glm::radians(m_angle), m_rot);
	m_worldMaxtrix = glm::scale(m_worldMaxtrix, m_scale);
}

void Model3D::Draw()
{
	if (m_pMesh == NULL) return;


	m_pShaders->UseProgram(); 
	glUniformMatrix4fv(m_pShaders->u_model,1,GL_FALSE, &(m_worldMaxtrix[0][0]));

	for (int i = 0; i < m_textures.size(); i++) {
		m_textures[i]->Bind(i);
		//m_pShaders->SetUniform1i("u_texture" + _to_string(i), i);
	}

	m_pMesh->Draw(m_pShaders);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

