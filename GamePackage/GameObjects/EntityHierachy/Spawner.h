#pragma once
#include"InfoPacket.h"
//abstract class
class Spawner {
	ObjectId::ObjectId m_id;
protected:
	inline Spawner* SetId(const ObjectId::ObjectId&id) { m_id = id; return this; }
public:
	//pure virtual functions
	virtual Spawner*Spawn(InfoPacket*infoPacket) { return NULL; }
	inline const ObjectId::ObjectId&GetId() { return m_id; }
};
