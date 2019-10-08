#pragma once
#include"Animation.h"
#include"BodySegment.h"
class ScmlEntity {
public:
	~ScmlEntity() {
		for (auto&animation : animations)
			delete animation;
	}
	void Log() {
		SDL_Log("entity %s\n", name.c_str());
		for (auto&animation : animations)
			((Animation*)animation)->Log();
	}
	std::string name;
	std::vector<AnimationBase*> animations;
	std::map<std::string, int> animationMap;

	std::vector<std::vector<int>> animationSwitchingTime;
	std::map<std::string, int>bodySegmentMap;

};
