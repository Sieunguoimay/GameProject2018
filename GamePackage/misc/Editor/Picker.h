#pragma once
#include"../../GameObjects/EntityHierachy/Entity.h"
#include"../../Input/InputEvent.h"
#include"VertexObject.h"

class Picker {
	friend class Editor;
	AABBEntity*m_pPickedEntity = NULL;
	int m_zoomFlag = 0;

	bool m_attachFlag = false;
	std::map<AABBEntity*, VertexObject*>m_attachedEntityMap;
	void attach(AABBEntity*e, VertexObject*v);
	AABBEntity*m_pObjectA;
	AABBEntity*m_pObjectB;
public:
	void Process(AABBEntity*entity,MouseEvent&inputEvent);
	inline AABBEntity*GetAABBEntity() { return m_pPickedEntity; }
	inline void SetZoomFlag(int f) { m_zoomFlag = f; }
	inline void EnableAttach() { m_attachFlag = true; }
	inline void DisableAttach() { m_attachFlag = false; }

	//first, when the button , say S, is held,
	//the flag is on.
	//mouse click event will select the AABBEntity A and B
	//after none of them are NULL, check for
	//one of them is VertexObject and Another is Not,
	//then we add the non-vertexObject to the Vertex object
};

