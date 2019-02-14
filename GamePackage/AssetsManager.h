#pragma once
#include<vector>
#include<map>
#include"2D/Texture.h"
#include"2D/Shaders.h"
#include"2D/AnimationCollection.h"
#include"GameObjects\SpriterEngine\ScmlObject.h"
#include"GameObjects\SpriterEntity.h"
//Note: Use singleton in the local module i.e. the same level of package

class AssetsManager
{
	//AssetsManager&operator=(const AssetsManager&);
	//AssetsManager(const AssetsManager&);
	//static AssetsManager* s_instance;
	void loadFromXml(std::string filename);
	void loadFromTxt(std::string filename);
public:
	AssetsManager();
	~AssetsManager();
	//static AssetsManager& Instance();
	void CleanUp();


	void Init(std::string filename);
	Texture*GetTexture(const std::string& name);
	Shaders*GetShader(const std::string& name);
	SpriterEntity*SpawnSpriterEntity(std::string name);
	AnimationCollection*SpawnAnimationCollection(const std::string&name);
	std::string&GetFontPath(int index) { return m_fontPaths[index]; }
	int GetTextureNum() { return m_textures.size(); }
private:
	std::map<std::string, Texture*>m_textures;
	std::map<std::string, Shaders*>m_shaders;
	std::map<std::string, AnimationCollection*>m_animationCollections;
	std::map<std::string, ScmlObject*> m_scmlObjects;
	std::map<std::string, SpriterEntity*> m_spriterEntities;
	std::vector<std::string> m_fontPaths;
};

#define s_assetsManager AssetsManager::Instance()