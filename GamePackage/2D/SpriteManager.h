#pragma once
#include"Sprite.h"
#include"Animation.h"
class SpriteManager {
	SpriteManager(){}
	SpriteManager(const SpriteManager&);
	SpriteManager&operator=(const SpriteManager&);

	std::map<std::string ,Sprite*>m_sprites;
	std::map<Sprite*, std::map<std::string, Animation*>*>m_animationSets;
	static SpriteManager*s_instance;
public:
	~SpriteManager();
	static SpriteManager&Instance();
	bool Init(const char*filename);
	void CleanUp();

	std::map<std::string,Sprite*>&GetSprite() { return this->m_sprites; }
	void AddSprite(std::string name,Sprite*newSprite) { this->m_sprites[name]=newSprite; }
	Sprite*GetSprite(std::string name) { return m_sprites[name];}
	std::map<std::string, Sprite*>&GetAllSprites() { return this->m_sprites; }
	std::map<std::string, Animation*>*GetAnimationSet(std::string name);
	std::map<std::string, Animation*>*GetAnimationSet(Sprite*sprite);
	void DrawLine(float x1, float y1, float x2, float y2);
};
#define s_spriteManager SpriteManager::Instance()