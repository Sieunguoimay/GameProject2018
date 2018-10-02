#pragma once
#include"../misc/Assistances.h"
#include"../GameObjects/BaseObject.h"
#include"Tile.h"
enum ThingsOnMap {
	DEFAULT_MAP_OBJECT = 0,
	TILE = 1,
	GRASS = 2,
	ROCK = 4,
};
class GameWorld {
	std::vector<b2Vec2>m_wallVertices;
	PhysicsBody*m_wallBody;

	Sprite*m_pTileSprite;
	std::vector<Tile*>m_tiles;

	std::vector<NoPhysicsObject*>m_plants;
	std::vector<PhysicsObject*>m_obstacles;

	void LoadMap(const char*filename);
	void PutTile(int type, float x, float y,float w, float h);
	void PutSomethingOnTile(int32 type, float x, float y, float w, float h);
public:
	GameWorld();
	~GameWorld();
	void Init(const char*filename);
	void Update(float deltaTime);
	void Draw();

	std::vector<b2Vec2>&GetWallVertices() { return this->m_wallVertices; }
	void SetWallBody(PhysicsBody*wallBody) { this->m_wallBody = wallBody; }
	std::vector<Tile*>&GetTiles() { return this->m_tiles; }
};
