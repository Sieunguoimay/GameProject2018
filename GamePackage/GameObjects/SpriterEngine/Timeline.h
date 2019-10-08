#pragma once 
#include<vector>
#include<string>
#include"TimelineKey.h"
#include"BodySegment.h"
class Timeline {
public:
	enum TimelineObjectType { SPRITE, BONE, BOX, POINT, SOUND, ENTITY, VARIABLE };

	std::string name;
	int objectType;

	std::vector<TimelineKey*> keys;


	//to control this time line, set this flag to true
	//use this key to control this timeline across animation channel.
	BoneSegment* pControlKey = NULL;

	~Timeline() {
		for (auto&timelineKey : keys)
			delete timelineKey;
	}
	void Log() {
		printf("\t\ttimeline %s %d size of the keys on this time line: %d\n", name.c_str(), objectType,(int)keys.size());
		for (auto&timelineKey : keys)
			timelineKey->Log();
	}
};
