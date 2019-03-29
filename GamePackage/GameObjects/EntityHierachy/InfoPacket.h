#pragma once
#include<glm\glm.hpp>
namespace ObjectId{enum ObjectId;}
//pure virtual class
class InfoPacket {
	ObjectId::ObjectId m_id;
	void*m_pData;
public:
	InfoPacket(const ObjectId::ObjectId&id, void*pData) :m_id(id),m_pData(pData) {}
	inline const ObjectId::ObjectId&GetId() { return m_id; }
	inline const void*GetData() { return m_pData; }
};
