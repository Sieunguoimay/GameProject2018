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
	for (int i = 0; i < ObjectId::OID_OBJECT_NUM; i++) 
		if(m_objectPrototypes[i]) delete m_objectPrototypes[i];

	m_pContainer = NULL;
}
void ObjectPool::Init()
{
	insertMap("player", ObjectId::OID_PLAYER);
	insertMap("animal", ObjectId::OID_ANIMAL);
	insertMap("grass", ObjectId::OID_GRASS);
	insertMap("leaf", ObjectId::OID_LEAF);
	insertMap("leaf", ObjectId::OID_VERTEX_OBJECT);
	insertMap("leaf", ObjectId::OID_PLATFORM);

	m_objectPrototypes[ObjectId::OID_PLAYER] = new Player();
	m_objectPrototypes[ObjectId::OID_ANIMAL] = new Animal();
	m_objectPrototypes[ObjectId::OID_GRASS] = new Grass();
	m_objectPrototypes[ObjectId::OID_LEAF] = new Leaf();
	m_objectPrototypes[ObjectId::OID_VERTEX_OBJECT] = new VertexObject();
	m_objectPrototypes[ObjectId::OID_PLATFORM] = new Platform();

	SDL_Log("Object Pool Initialized.");
}


AABBEntity*ObjectPool::CreateNewObject(InfoPacket * packet, bool atRunTime /*= false*/)
{
	AABBEntity*a = (AABBEntity*)m_objectPrototypes[packet->GetId()]->Spawn(packet);
	if (atRunTime) {
		m_pContainer->push_front(a);
		m_pContainer->head->next->data->Init();
	}else
		m_pContainer->push_back(a);

	selectObjects(a);

	return a;
}

const ObjectId::ObjectId & ObjectPool::GetObjectId(const char*name)
{
	// TODO: insert return statement here
	auto&a = m_nameMap.find(name);
	if (a != m_nameMap.end()) return a->second;
	return ObjectId::OID_OBJECT_NUM;//check for it 
}
const char * ObjectPool::GetObjectName(const ObjectId::ObjectId & id)
{
	auto&a = m_idMap.find(id);
	if (a != m_idMap.end()) return a->second;
	return "";//check for it 
}
AABBEntity * ObjectPool::GetSavedObject(const ObjectId::ObjectId & id)
{
	auto&a = m_objectsOfInterest.find(id);
	if (a != m_objectsOfInterest.end()) return a->second;
	return NULL;
}
void ObjectPool::insertMap(const char * name, const ObjectId::ObjectId & id)
{
	m_nameMap.insert(std::pair<const char*, ObjectId::ObjectId>(name, id));
	m_idMap.insert(std::pair<ObjectId::ObjectId, const char*>(id, name));
}

void ObjectPool::selectObjects(AABBEntity * entity)
{
	switch (entity->GetId()) {
	case ObjectId::OID_PLAYER:
		if (GetSavedObject(ObjectId::OID_PLAYER))
			m_objectsOfInterest[ObjectId::OID_PLAYER] = entity;
		else 
			m_objectsOfInterest.insert(std::pair<ObjectId::ObjectId,AABBEntity*>(ObjectId::OID_PLAYER,entity));
		break;
	}
}
