#include"ResourceManager.h"
#include<sstream>
#include"../misc/Assistances.h"
ResourceManager*ResourceManager::s_instance = NULL;
ResourceManager&ResourceManager::Instance() {
	if (s_instance == NULL) {
		s_instance = new ResourceManager();
	}
	return *s_instance;
}
ResourceManager::~ResourceManager() {
}

void ResourceManager::Init(std::string filename) {
	char*data = NULL;
	loadFile(data, filename);
	if (data == NULL)
		return;
	std::string s_data(data);
	free(data);
	std::stringstream ss(s_data);
	std::string type;
	while (ss >> type) {
		std::string prefix;
		if (type == "#Meshes") {
			int id, n;
			ss >> n;
			for (int i = 0; i < n; i++) {
				std::string name;
				ss >> prefix >> id >> prefix >> name;
				SDL_Log("___ %d %s", id, name.c_str());

				Mesh*newMesh = new Mesh();
				if (newMesh->Init(name))
					m_meshes.push_back(newMesh);
				else
					delete newMesh;
			}
		}
		else if (type == "#Textures") {
			int id, n;
			ss >> n;
			for (int i = 0; i < n; i++) {
				std::string name;
				ss >> prefix >> id >> prefix >> name;
				SDL_Log("___ %d %s", id, name.c_str());
			
				Texture*newTexture = new Texture();
				if (newTexture->Init(name.c_str()))
					m_textures.push_back(newTexture);
				else
					delete newTexture;
			}
		}
		else if (type == "#Shaders") {
			int id, n;
			ss >> n;
			for (int i = 0; i < n; i++) {
				std::string vs, fs;
				ss >> prefix >> id
					>> prefix >> vs
					>> prefix >> fs;
				SDL_Log("___ %d %s %s", id, vs.c_str(), fs.c_str());

				Shaders*newShaders = new Shaders();
				if (newShaders->Init(vs, fs))
					m_shaders.push_back(newShaders);
				else
					delete newShaders;
			}
		}
	}
	SDL_Log("Loaded Resources %d %d %d\n\n",m_meshes.size(), m_textures.size(),m_shaders.size());
}
void ResourceManager::CleanUp() {
	for (auto&a : m_meshes)
		delete a;
	for (auto&a : m_textures)
		delete a;
	for (auto&a : m_shaders)
		delete a;
	delete s_instance;
}
