#pragma once
#include"InfoPacket.h"
//abstract class
class Spawner {
	int m_id;
protected:
	inline Spawner* SetId(int id) { m_id = id; return this; }
public:
	//pure virtual functions
	inline int GetId() { return m_id; }
};
