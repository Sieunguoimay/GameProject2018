#pragma once
#include"Folder.h"
#include"ScmlEntity.h"
class ScmlObject {

	float fuck[4];

public:
	int currentEntity;
	int currentAnimation;
	//used for testing purpose
	//int currentTime;
	std::vector<SpriterEngine::Folder*>folders;
	std::vector<ScmlEntity*>entities;
	//std::vector<SpriterEngine::Folder*>activeCharacterMap;


	ScmlObject() :currentEntity(0), currentAnimation(0) {
		SDL_Log("scml object created"); 
	}
	~ScmlObject();
	void Log();
	//float Draw(float newTime);
	void LoadFile(const char*string, class AssetsManager*assets);
	//void ApplyCharacterMap(CharacterMap*charMap,bool reset);

	friend class Animation;
	friend class AnimationSwitcher;
};
