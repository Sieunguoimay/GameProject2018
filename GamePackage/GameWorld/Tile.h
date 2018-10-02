#pragma once
#include"../GameObjects/BaseObject.h"
class Tile :public NoPhysicsObject{
public:
	Tile(Sprite*sprite, float x, float y,int w, int h);
	~Tile();
	void PutOnMe(BaseObject*object){}
};
