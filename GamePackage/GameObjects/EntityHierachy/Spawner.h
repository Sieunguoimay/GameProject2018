#pragma once
#include"InfoPacket.h"
//abstract class
class Spawner {
	enum GameWorldObjectId m_id;
protected:
	inline Spawner* SetId(const GameWorldObjectId&id) { m_id = id; return this; }
public:
	//pure virtual functions
	virtual Spawner*Spawn(InfoPacket*infoPacket) { return NULL; }
	inline const GameWorldObjectId&GetId() { return m_id; }
};
