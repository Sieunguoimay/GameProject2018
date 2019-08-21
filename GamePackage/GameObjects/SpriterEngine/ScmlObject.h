#pragma once
#include<map>
#include"Folder.h"
#include"ScmlEntity.h"
class AssetsManager;
class ScmlObject {
	AnimationCallBack*callback;	
public:
	
	SpatialInfo characterInfo;
	int currentEntity;
	int currentAnimation;
	
	//used for testing purpose
	//int currentTime;

	std::vector<SpriterEngine::Folder*>folders;
	std::vector<ScmlEntity*>entities;
	std::vector<SpriterEngine::Folder*>activeCharacterMap;




	ScmlObject() :currentEntity(0), currentAnimation(0) {}
	~ScmlObject();
	void Log();
	float Draw(float newTime);
	void LoadFile(const char*string, AssetsManager*assets);
	//void ApplyCharacterMap(CharacterMap*charMap,bool reset);

	//query for callback
	void SetAnimationCallback(AnimationCallBack*callback) { this->callback = callback; }
	friend class Animation;
	friend class AnimationSwitcher;
};
