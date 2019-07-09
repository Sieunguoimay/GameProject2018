#pragma once
#include"EntityHierachy\Entity.h"
#include"../misc/Assistances.h"
#include"../misc/List.h"
#include<string>
#include<map>

enum GameWorldObjectId{
	GWOID_PLAYER,
	GWOID_ANIMAL,
	GWOID_GRASS,
	GWOID_LEAF,
	GWOID_VERTEX_OBJECT,
	GWOID_PLATFORM,
	GWOID_OBJECT_NUM
};

class ObjectPool :public Entity {
	//in the end, when they are all fixed, should we use the id num only??
	//for the purpose of faster loading....
	//---> we would love to consider that suggestion at that point. 
	AABBEntity*m_objectPrototypes[GWOID_OBJECT_NUM] = { 0 };
	std::map<const char*, GameWorldObjectId,cmp_str>m_nameMap;
	std::map<GameWorldObjectId,const char*>m_idMap;
	
	//Utilities
	void insertMap(const char*name, const GameWorldObjectId&id,AABBEntity*entity);
	void selectObjects(AABBEntity*entity);
	//pointers
	List<AABBEntity*>*m_pContainer;
	std::map<GameWorldObjectId,AABBEntity*>m_objectsOfInterest;
public:
	ObjectPool(List<AABBEntity*>*pContainer);
	~ObjectPool();
	void Init()override;

	AABBEntity*CreateNewObject(class InfoPacket*packet, bool atRunTime=false);
	const GameWorldObjectId&GetObjectId(const char*name);
	const char*GetObjectName(const GameWorldObjectId&id);
	AABBEntity*GetSavedObject(const GameWorldObjectId&id);
};
