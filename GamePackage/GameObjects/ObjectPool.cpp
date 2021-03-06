#include "ObjectPool.h"
#include"Character\Animal.h"
#include"Character/Player/Player.h"
#include"../GameObjects/World/Platform.h"
#include"../GameObjects/World/VertexObject.h"
ObjectPool::ObjectPool(List<AABBEntity*>* pContainer)
	:m_pContainer(pContainer)
{
}
ObjectPool::~ObjectPool()
{
	//for (int i = 0; i < GWOID_OBJECT_NUM; i++) 
	//	if(m_objectPrototypes[i]) delete m_objectPrototypes[i];

	m_pContainer = NULL;
}
void ObjectPool::Init()
{
	insertMap("player", GWOID_PLAYER, Player::Spawn);
	insertMap("animal", GWOID_ANIMAL, Animal::Spawn);
	insertMap("grass", GWOID_GRASS, Grass::Spawn);
	insertMap("leaf", GWOID_LEAF, Leaf::Spawn);
	insertMap("leaf", GWOID_VERTEX_OBJECT, VertexObject::Spawn);
	insertMap("leaf", GWOID_PLATFORM, Platform::Spawn);
	
	SDL_Log("Object Pool Initialized.");
}


AABBEntity*ObjectPool::CreateNewObject(InfoPacket * packet, bool atRunTime /*= false*/)
{
	AABBEntity*a = (AABBEntity*)m_objectPrototypes[packet->GetId()](packet);
	if (atRunTime) {
		m_pContainer->push_front(a);
		m_pContainer->first()->data->Init();
	}else
		m_pContainer->push_back(a);

	selectObjects(a);

	return a;
}

int ObjectPool::GetObjectId(const char*name)
{
	// TODO: insert return statement here
	auto&a = m_nameMap.find(name);
	if (a != m_nameMap.end()) return a->second;
	return GWOID_OBJECT_NUM;//check for it 
}
const char * ObjectPool::GetObjectName(int id)
{
	auto&a = m_idMap.find(id);
	if (a != m_idMap.end()) return a->second;
	return "";//check for it 
}
AABBEntity * ObjectPool::GetSavedObject(int id)
{
	auto&a = m_objectsOfInterest.find(id);
	if (a != m_objectsOfInterest.end()) return a->second;
	return NULL;
}
void ObjectPool::insertMap(const char * name,int  id, std::function<Spawner*(InfoPacket*)>entity)
{
	m_nameMap.insert(std::pair<const char*, int >(name, id));
	m_idMap.insert(std::pair<int , const char*>(id, name));
	m_objectPrototypes[id] = entity;
}

void ObjectPool::selectObjects(AABBEntity * entity)
{
	switch (entity->GetId()) {
	case GWOID_PLAYER:
		if (GetSavedObject(GWOID_PLAYER))
			m_objectsOfInterest[GWOID_PLAYER] = entity;
		else 
			m_objectsOfInterest.insert(std::pair<GameWorldObjectId,AABBEntity*>(GWOID_PLAYER,entity));
		break;
	}
}
