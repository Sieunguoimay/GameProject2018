#include "Picker.h"
#include"../../GameObjects/ObjectPool.h"
//void Picker::attach(AABBEntity * e, VertexObject * v)
//{
//	auto&pos = m_attachedEntityMap.find(e);
//	if (pos != m_attachedEntityMap.end()) {
//		pos->second->Dettach(e);
//		m_attachedEntityMap[e]= v;
//	}else
//		m_attachedEntityMap.insert(std::pair<AABBEntity*, VertexObject*>(e, v));
//
//	v->Attach(e);
//}

void Picker::Process(AABBEntity * entity, MouseEvent & inputEvent)
{
	if (m_pPickedEntity != NULL)
		if (m_pPickedEntity != entity)return;

	static glm::vec4 aabb;
	static glm::vec2 firstMouse;
	
	glm::vec2 pos = inputEvent.GetMousePosInWorld();
	if (check_AABB_against_point(entity->GetAABB(),pos)) {

		if (inputEvent.IsClickSignalled()) {
			entity->Select();
			aabb = entity->GetAABB();
			firstMouse = inputEvent.GetMousePosInWorld();
			m_pPickedEntity = entity;

			if (m_attachFlag) {
				if (entity->GetId() != GWOID_PLATFORM) {
					if (m_pObjectA == NULL&&m_pObjectB == NULL) {
						if (entity->GetId() != GWOID_VERTEX_OBJECT) {
							SDL_Log("One1");
							m_pObjectA = entity;
						}
					}else if (m_pObjectA != NULL) {
						if (entity->GetId() == GWOID_VERTEX_OBJECT) {
							SDL_Log("Two");
							m_pObjectB = entity;
						}
						else {
							SDL_Log("One2");
							m_pObjectA = entity;

						}
					}
				}
			}
		}

		if (inputEvent.IsMousePressed()) {
			//drag and zoom
			if (m_zoomFlag!=0) {
				float ratio = (aabb.w - aabb.y)/ (aabb.z - aabb.x);
				aabb.x -= m_zoomFlag;
				aabb.z += m_zoomFlag;
				
				aabb.y -= ratio*m_zoomFlag;
				aabb.w += ratio*m_zoomFlag;
			}

			glm::vec2 diff = inputEvent.GetMousePosInWorld() - firstMouse;
			entity->SetAABB(glm::vec4(aabb.x + diff.x, aabb.y + diff.y, aabb.z + diff.x, aabb.w + diff.y));
		}



		if(inputEvent.IsReleaseSignalled()){
			entity->Release();
			m_pPickedEntity = NULL;
		}
	}
	else {
		if (entity->IsSelected()) {
			entity->Release();
			m_pPickedEntity = NULL;
		}
	}


	if(m_pObjectA!=NULL&&m_pObjectB!=NULL){
		
		m_pWorld->Attach(m_pObjectA, ((VertexObject*)m_pObjectB));
		m_pObjectA = NULL;
		m_pObjectB = NULL;
		SDL_Log("DONE");
	}
	if (!m_attachFlag) {
		if(m_pObjectA)
			SDL_Log("FAILED");

		m_pObjectA = NULL;
		m_pObjectB = NULL;
	}
	//recover
	m_zoomFlag = 0;
}
