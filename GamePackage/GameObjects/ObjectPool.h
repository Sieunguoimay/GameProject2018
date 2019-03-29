#pragma once
#include"EntityHierachy\Entity.h"
#include"../misc/Assistances.h"
#include"../misc/List.h"
#include<string>
#include<map>

namespace ObjectId {
	enum ObjectId {
		OID_PLAYER,
		OID_ANIMAL,
		OID_GRASS,
		OID_LEAF,
		OID_VERTEX_OBJECT,
		OID_PLATFORM,
		OID_OBJECT_NUM
	};
};

class InfoPacket;
class ObjectPool :public Entity {
	//in the end, when they are all fixed, should we use the id num only??
	//for the purpose of faster loading....
	//---> we would love to consider that suggestion at that point. 
	std::map<const char*, ObjectId::ObjectId,cmp_str>m_nameMap;
	std::map<ObjectId::ObjectId,const char*>m_idMap;
	AABBEntity*m_objectPrototypes[ObjectId::OID_OBJECT_NUM] = { 0 };
	void insertMap(const char*name, const ObjectId::ObjectId&id);
	//pointers
	List<AABBEntity*>*m_pContainer;
public:
	ObjectPool(List<AABBEntity*>*pContainer);
	~ObjectPool();
	void Init()override;
	void Update(float deltaTime)override;
	AABBEntity*CreateNewObject(InfoPacket*packet, bool atRunTime=false);
	const ObjectId::ObjectId&GetObjectId(const char*name);
	const char*GetObjectName(const ObjectId::ObjectId&id);
};
