#pragma once
#include<Box2D/Box2D.h>
//#define MAX_PERCEPT_VERTEX_NUM 10
struct Percept {
	b2Body*m_pBody;
	char m_type;
	char m_sensingMechanism;
	char m_flag;

	Percept(
		b2Body*pBody = NULL, 
		char type = 255, 
		char sensingMechanism = 255,
		char flag = 255
	)
		:m_pBody(pBody)
		,m_type(type)
		,m_sensingMechanism(sensingMechanism)
		,m_flag(flag)
	{}
};

