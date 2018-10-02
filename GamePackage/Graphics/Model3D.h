#pragma once
#include"Mesh.h"
#include"Texture.h"
#include"glm/glm.hpp"
class Model3D
{
	Shaders*m_pShaders;
	Mesh*m_pMesh;
	std::vector<Texture*> m_textures;

	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
	float m_angle;

	glm::mat4 m_worldMaxtrix;
public:
	Model3D();
	~Model3D();

	void Update(float deltaTime);
	void Draw();
	
	//use this to adjust the matrix as you want and send to GPU whenever you want

	void Transform(glm::mat4&matrix) { this->m_worldMaxtrix = matrix*m_worldMaxtrix; }


	void SetPos(glm::vec3 pos) { this->m_pos = pos; }
	void SetPos(float x, float y, float z) { this->m_pos = glm::vec3(x,y,z); }
	void SetRot(glm::vec3 rot) { this->m_rot = rot; }
	void SetScale(glm::vec3 scale) { this->m_scale = scale; }
	void SetAngle(float angle) { this->m_angle = angle; }

	//use this to setup Model3D at initalization
	void SetMesh(Mesh*mesh) { this->m_pMesh = mesh; }
	void AddTextrue(Texture*texture) { if(texture!=NULL) this->m_textures.push_back(texture); }
	void SetShader(Shaders*shaders) { this->m_pShaders = shaders; }

	Mesh*GetMesh() { return this->m_pMesh; }
	Shaders*GetShader() { return this->m_pShaders; }
	glm::vec3&GetPos() { return this->m_pos; }

	Texture*GetTexture(int id) { if (id < m_textures.size())return m_textures[id]; return NULL; }
};

