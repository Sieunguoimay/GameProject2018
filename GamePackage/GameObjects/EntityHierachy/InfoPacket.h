#pragma once
#include<glm\glm.hpp>

//pure virtual class
class InfoPacket {
	enum GameWorldObjectId m_id;
	void*m_pData;
public:
	InfoPacket(const GameWorldObjectId&id, void*pData) :m_id(id),m_pData(pData) {}
	inline const GameWorldObjectId&GetId() { return m_id; }
	inline const void*GetData() { return m_pData; }
};
