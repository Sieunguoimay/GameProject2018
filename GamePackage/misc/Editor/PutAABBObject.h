#pragma once
#include"../../GameObjects/ObjectPool.h"
class PutAABBObject {
	ObjectPool*m_pObjectPool;
	GameWorldObjectId m_gotId;
public:
	PutAABBObject(ObjectPool*pObjectPool)
		:m_pObjectPool(pObjectPool)
		,m_gotId(GWOID_OBJECT_NUM){}

	void Update(float deltaTime);
	inline void Trigger(const GameWorldObjectId&id) { m_gotId = id; }
};
