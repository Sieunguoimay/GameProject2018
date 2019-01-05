#pragma once
#include"Bone.h"
class BoneToucher {
	static void unTouch(Bone*bone);
public:
	static void Touch(Bone*bone);
	static Bone*m_pBone;
};