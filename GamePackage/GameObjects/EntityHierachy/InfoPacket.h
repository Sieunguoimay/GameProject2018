#pragma once
#include<glm\glm.hpp>

//pure virtual class
class InfoPacket {
	int m_id;
	void*m_pData;
public:
	InfoPacket(int id, void*pData) :m_id(id),m_pData(pData) {}
	inline int GetId() { return m_id; }
	inline const void*GetData() { return m_pData; }
};
