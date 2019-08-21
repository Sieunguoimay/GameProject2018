#include "AssetsManager.h"
#include"misc\Assistances.h"
#include<sstream>


AssetsManager::AssetsManager()
{
}


AssetsManager::~AssetsManager()
{
}

void AssetsManager::CleanUp()
{
	for (auto&a : m_textures)
		delete a.second;
	for (auto&a : m_shaders)
		delete a.second;
	for (auto&a : m_animationCollections)
		delete a.second;
	for (auto&a : m_spriterEntities)
		delete a.second;
	for (auto&a : m_scmlObjects)
		delete a.second;

}



void AssetsManager::Init(std::string filename)
{
	loadFromXml(filename);
}


Texture * AssetsManager::GetTexture(const std::string & name)
{
	auto mit = m_textures.find(name);
	if (mit == m_textures.end()) {
		Utils::logError("texture " + name + " Not Found");
	}
	return mit->second;
}

Shaders * AssetsManager::GetShader(const std::string & name)
{
	auto mit = m_shaders.find(name);
	if (mit == m_shaders.end()) {
		Utils::logError("Shader " + name + " Not Found");
	}
	return mit->second;
}

SpriterEntity * AssetsManager::SpawnSpriterEntity(std::string name)
{
	auto mit = m_spriterEntities.find(name);
	if (mit == m_spriterEntities.end()) {
		Utils::logError("SpriterEntity " + name + " Not Found");
	}
	return mit->second->Spawn();
}

//ScmlObject * AssetsManager::GetScmlObject(std::string name)
//{
//	auto mit = m_scmlObjects.find(name);
//	if (mit == m_scmlObjects.end()) {
//		logError("scmlObject " + name + " Not Found");
//	}
//	return mit->second;
//}

AnimationCollection * AssetsManager::SpawnAnimationCollection(const std::string&name)
{
	auto mit = m_animationCollections.find(name);
	if (mit == m_animationCollections.end()) {
		Utils::logError("texture " + name + " Not Found");
	}
	return mit->second->Clone();
}




#include<map>
#include"misc\tinyXML2\tinyxml2.h"
void AssetsManager::loadFromXml(std::string filename)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename.c_str());

	std::string relative_path = "";
	auto found = filename.find_first_of('/');
	if (found != std::string::npos) {
		relative_path = filename.substr(0, found + 1);
	}


	std::stringstream ss_scmlFiles;
	tinyxml2::XMLElement*pAssets = doc.FirstChildElement("assets");
	if (pAssets) {
		tinyxml2::XMLElement*pFolder = pAssets->FirstChildElement("folder");
		while (pFolder) {
			std::string folder_name = Utils::_atos(pFolder->Attribute("name"));
			if (folder_name!="") folder_name += "/";

			tinyxml2::XMLElement*pFile = pFolder->FirstChildElement("file");
			std::string name,tag;
			std::map<std::string, bool> shader_names;
			while (pFile) {
				name = Utils::_atos(pFile->Attribute("name"));
				auto found = name.find_last_of('.');
				if (found != std::string::npos) {
					tag = name.substr(found + 1, name.length() - found - 1);

					if (tag == "png") {
						Texture*newTexture = new Texture();
						//
						if (newTexture->Init((relative_path + folder_name + name).c_str())) {
							m_textures.insert(std::pair<std::string, Texture*>(name, newTexture));
							newTexture->SetName(name);
						}
						else {
							delete newTexture;
						}
					}
					else if (tag == "vs"|| tag == "fs") {
						std::string body_name = name.substr(0, found);
						auto found = shader_names.find(body_name);
						if (found != shader_names.end()) {

							Shaders *newShader = new Shaders();
							if (newShader->Init(relative_path + folder_name + body_name + ".vs", relative_path + folder_name + body_name + ".fs")) {
								m_shaders.insert(std::pair<std::string, Shaders*>(body_name, newShader));
							}
							else {
								delete newShader;
							}

							shader_names.erase(body_name);
						}
						else {
							shader_names.insert(std::pair<std::string, bool>(body_name, true));
						}
					}
					else if (tag == "scml") {
						ss_scmlFiles << relative_path + folder_name + name << "\n";
						ss_scmlFiles << name << "\n";
					}
					else if (tag == "TTF"||tag == "ttf" || tag == "otf") {
						m_fontPaths.push_back(relative_path + folder_name + name);
					}
				}
				name = "";
				pFile = pFile->NextSiblingElement("file");
			}
			pFolder = pFolder->NextSiblingElement("folder");
		}
	}
	std::string line;
	while (std::getline(ss_scmlFiles, line)) {
		std::string name;
		std::getline(ss_scmlFiles, name);
	
		auto found = name.find_last_of('.');
		std::string body_name = "null";
		if(found!=std::string::npos)
			body_name = name.substr(0, found);

		ScmlObject*newScmlObject = new ScmlObject();
		newScmlObject->LoadFile(line.c_str(),this);


		m_scmlObjects.insert(std::pair<std::string,ScmlObject*>(body_name,newScmlObject));

		for (int i = 0,n = newScmlObject->entities.size();i<n; i++)
			m_spriterEntities.insert(
				std::pair<std::string, SpriterEntity*>(
					body_name +"/"+newScmlObject->entities[i]->name,
					new SpriterEntity(newScmlObject, i)));
	}

	//how to use it
	SDL_Log("----------HOW TO USE IT-----------------");
		for (auto&a : m_textures)
		SDL_Log("Texture: %s", (a.first).c_str());
	for (auto&a : m_shaders)
		SDL_Log("Shader: %s", (a.first).c_str());
	for (auto&a : m_spriterEntities)
		SDL_Log("SpriterEntity: %s", (a.first).c_str());
	for (auto&a : m_fontPaths)
		SDL_Log("Fonts: %s", a.c_str());
	SDL_Log("----------------------------------------");
}

//not used any more...
void AssetsManager::loadFromTxt(std::string filename)
{


	//char*data = NULL;
	//loadFile(data, filename);
	//if (data == NULL)
	//	return;
	//std::string s_data(data);
	//free(data);
	//std::stringstream ss(s_data);
	//std::string type;
	//while (ss >> type) {
	//	std::string prefix;
	//	if (type == "#Textures") {
	//		int id, n;
	//		ss >> n;
	//		for (int i = 0; i < n; i++) {
	//			std::string name;
	//			ss >> prefix >> id >> prefix >> name;
	//			SDL_Log("___ %d %s", id, name.c_str());

	//			Texture*newTexture = new Texture();
	//			if (newTexture->Init(name.c_str()))
	//				m_textures.insert(std::pair<std::string, Texture*>(newTexture->GetName(), newTexture));
	//			else
	//				delete newTexture;
	//		}
	//	}
	//	else if (type == "#AnimationCollections") {
	//		int n, numAnimation, numIndices, fps;
	//		glm::ivec2 dimension;
	//		ss >> n;
	//		for (int i = 0; i < n; i++) {
	//			std::string name, animationName;
	//			ss >> prefix >> name
	//				>> prefix >> dimension.x >> dimension.y
	//				>> prefix >> numAnimation;
	//			AnimationCollection*newAnimationCollection = new AnimationCollection();
	//			newAnimationCollection->Init(GetTexture(name), dimension);

	//			for (int j = 0; j < numAnimation; j++) {
	//				ss >> prefix >> animationName
	//					>> prefix >> fps
	//					>> prefix >> numIndices >> prefix;
	//				std::vector<int>indices;
	//				indices.resize(numIndices);
	//				for (int k = 0; k < numIndices; k++)
	//					ss >> indices[k];
	//				newAnimationCollection->AddAnimation(animationName, indices, fps);
	//			}
	//			m_animationCollections.insert(std::pair<std::string, AnimationCollection*>(name, newAnimationCollection));
	//		}
	//	}
	//	else if (type == "#Shaders") {
	//		int id, n;
	//		ss >> n;
	//		for (int i = 0; i < n; i++) {
	//			std::string vs, fs;
	//			ss >> prefix >> id
	//				>> prefix >> vs
	//				>> prefix >> fs;
	//			SDL_Log("___ %d %s %s", id, vs.c_str(), fs.c_str());



	//			std::string name = vs;
	//			name.erase(0, name.find_last_of('/') + 1);
	//			name.erase(name.find_last_of('.'), name.size());

	//			Shaders*newShaders = new Shaders();
	//			if (newShaders->Init(vs, fs)) {

	//				m_shaders.insert(std::pair<std::string, Shaders*>(name, newShaders));
	//			}
	//			else
	//				delete newShaders;
	//		}
	//	}
	//}
}
