#pragma once
#include"Bone.h"
class SkeletonBuilder {
public:
	static void BuildFromFile(std::vector<Bone*>&skeleton,const std::string& file);
};
