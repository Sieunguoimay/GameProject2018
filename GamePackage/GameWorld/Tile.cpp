#include"Tile.h"
Tile::Tile(Sprite*sprite, float x, float y, int w, int h)
	:NoPhysicsObject(sprite,x,y,w,h)
{
}
Tile::~Tile() {
}
