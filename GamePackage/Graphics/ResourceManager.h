#pragma once
#include"Shaders.h"
#include"Mesh.h"
#include"Texture.h"

class ResourceManager {
	ResourceManager(){}

	ResourceManager(const ResourceManager&);
	ResourceManager&operator=(const ResourceManager&);

	std::vector<Mesh*>m_meshes;
	std::vector<Texture*>m_textures;
	std::vector<Shaders*>m_shaders;
	static ResourceManager*s_instance;
public:
	//singleton class
	static ResourceManager&Instance();
	~ResourceManager();

	void Init(std::string filename);
	void CleanUp();
	//getters and setters
	void AddNewMesh(Mesh*newMesh) { this->m_meshes.push_back(newMesh); }
	Mesh*GetMesh(int id) { if (id < m_meshes.size())return this->m_meshes[id]; return NULL; }
	Texture*GetTexture(int id) { if (id < this->m_textures.size()) return this->m_textures[id]; return NULL; }
	Shaders*GetShader(int id) { if (id < m_shaders.size()) return this->m_shaders[id]; return NULL; }
	std::vector<Mesh*>*GetAllMeshes() { return &(this->m_meshes); }
	std::vector<Texture*>*GetAllTextures() { return &(this->m_textures); }
	std::vector<Shaders*>*GetAllShaders() { return &this->m_shaders; }
};

//to make life easier..
#define s_resourceManager ResourceManager::Instance()