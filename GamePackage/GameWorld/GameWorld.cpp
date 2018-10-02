#include"GameWorld.h"
#include"../2D/SpriteManager.h"
#include"../GameObjects/MovingObject.h"
#include"../misc/GameAccessor.h"
GameWorld::GameWorld() {

}
GameWorld::~GameWorld() {
	for (auto&a : m_obstacles)
		delete a;
	for (auto&a : m_plants)
		delete a;
	for (auto&a : m_tiles)
		delete a;
	delete m_wallBody;
	m_pTileSprite = NULL;
}
void GameWorld::Init(const char*filename) {
	m_pTileSprite = s_spriteManager.GetSprite("tile");
	LoadMap(filename);

	Sprite*rockSprite = s_spriteManager.GetSprite("rock");
	for (int i = 0; i < 20; i++) {
		float x = RandomClamped(-(float)s_gameAccessor.GetWidth()/ 2.0f, (float)s_gameAccessor.GetWidth() / 2.0f);
		float y = RandomClamped(-(float)s_gameAccessor.GetHeight() / 2.0f, (float)s_gameAccessor.GetHeight() / 2.0f);
		float w = RandomClamped(10, 50);
		float h = w*(rockSprite->GetHeight() / rockSprite->GetWidth());

		PhysicsObject*rock = new MovingObject(rockSprite, x, y, w, h, PhysicsObject::BOX_SHAPE, NULL);
		rock->GetPhysicsBody()->GetBody()->SetLinearDamping(1.0f);
		m_obstacles.push_back(rock);
	}
}
void GameWorld::Update(float deltaTime)
{
	for (auto&a : m_obstacles) 
		a->Update(deltaTime);
}
void GameWorld::Draw() {
	//for (int i = 0; i < m_wallVertices.size() - 1; i++)
		//s_spriteManager.DrawLine(m_wallVertices[i].x, m_wallVertices[i].y, m_wallVertices[i + 1].x, m_wallVertices[i + 1].y);
	for (int i = 0; i < m_tiles.size(); i++)
		m_tiles[i]->Draw();
	for (auto&a : m_plants)
		a->Draw();
	for (auto&a : m_obstacles)
		a->Draw();
}

void GameWorld::LoadMap(const char * filename)
{
	char*data;
	loadFile(data, filename);
	std::stringstream ss(data); 
	free(data);
	std::string prefix;


	float w = 150;
	float h = 150;

	std::vector<b2Vec2>zeroVertices, &oneVertices = m_wallVertices;
	while (ss >> prefix) {
		if (prefix == "#Map") {
			std::string line;
			int i = 0,row,col;
			ss >> col >> row;
			b2Vec2 offset(-(float)col*w*0.5f,-(float)row*h*0.5f);
			while (std::getline(ss, line)) {
				std::stringstream ssline(line);
				int value, j = 0;
				bool one_flag = false, zero_flag = false;
				while (ssline >> value) {
					printf("%d ", value);
					if (value > 0) {
						if (!one_flag) {
							//fist one is here..
							printf("->%d here<-", j);
							if (oneVertices.empty())
								oneVertices.push_back(b2Vec2(w*j + offset.x, i*h + offset.y));
							else {
								b2Vec2 newUpperVertex(w*j + offset.x, i*h + offset.y);
								if (newUpperVertex.x != oneVertices[oneVertices.size() - 1].x)
									oneVertices.push_back(newUpperVertex);
							}
							oneVertices.push_back(b2Vec2(w*j+offset.x, (i + 1)*h + offset.y));
							one_flag = true;
						}
						//create new tile 
						PutTile(value, w*j + w*.5f + offset.x, i*h + 0.5f*h + offset.y,w,h);
						//whatever on the tile is puting here... depend on the value
						PutSomethingOnTile(ThingsOnMap::DEFAULT_MAP_OBJECT|value, w*j + w*.5f + offset.x, i*h + 0.5f*h + offset.y, w, h);

					}
					if (value == 0 && one_flag) {
						if (!zero_flag) {
							//first zero after one is here..
							printf("->%d here<-", j);
							if (zeroVertices.empty())
								zeroVertices.push_back(b2Vec2(w*j + offset.x, i*h + offset.y));
							else {
								b2Vec2 newUpperVertex(w*j + offset.x, i*h + offset.y);
								if (newUpperVertex.x != zeroVertices[zeroVertices.size() - 1].x)
									zeroVertices.push_back(newUpperVertex);
							}
							zeroVertices.push_back(b2Vec2(w*j + offset.x, (i + 1)*h + offset.y) );
							zero_flag = true;
						}
					}
					j++;
				}
				printf("\n");
				i++;
			}
		}
	}
	for (int i = zeroVertices.size() - 1; i >= 0; i--)
		oneVertices.push_back(zeroVertices[i]);
	oneVertices.push_back(oneVertices[0]);
	//for(auto&a:oneVertices)
	//SDL_Log("%f %f", a.x, a.y);

}

void GameWorld::PutTile(int type, float x, float y, float w, float h)
{
	Tile*newTile = new Tile(m_pTileSprite, x, y, w, h);
	m_tiles.push_back(newTile);
}

void GameWorld::PutSomethingOnTile(int32 type, float x, float y, float w, float h)
{
	if ((type&ThingsOnMap::GRASS) == ThingsOnMap::GRASS) {
		m_plants.push_back(new NoPhysicsObject(s_spriteManager.GetSprite("plant"), x,y,w, h));
	}
	if ((type&ThingsOnMap::ROCK) == ThingsOnMap::ROCK) {
		PhysicsObject*rock = new MovingObject(s_spriteManager.GetSprite("rock"),x,y, w, h, PhysicsObject::BOX_SHAPE,NULL);
		rock->GetPhysicsBody()->GetBody()->SetLinearDamping(1.0f);
		m_obstacles.push_back(rock);
	}
}
