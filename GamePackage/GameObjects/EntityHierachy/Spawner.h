#pragma once
#include"InfoPacket.h"
//abstract class
class Spawner {
	int m_id;
protected:
	inline Spawner* SetId(int id) { m_id = id; return this; }
public:
	//pure virtual functions
	virtual Spawner*Spawn(InfoPacket*infoPacket) { return NULL; }
	inline int GetId() { return m_id; }
};
