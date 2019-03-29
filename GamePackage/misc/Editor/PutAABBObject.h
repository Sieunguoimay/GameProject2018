#pragma once
#include"../../GameObjects/ObjectPool.h"
class PutAABBObject {
	ObjectPool*m_pObjectPool;
	ObjectId::ObjectId m_gotId;
public:
	PutAABBObject(ObjectPool*pObjectPool)
		:m_pObjectPool(pObjectPool)
		,m_gotId(ObjectId::OID_OBJECT_NUM){}

	void Update(float deltaTime);
	inline void Trigger(const ObjectId::ObjectId&id) { m_gotId = id; }
};
