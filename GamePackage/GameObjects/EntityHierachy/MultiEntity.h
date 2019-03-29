#pragma once
#include"Entity.h"
#include"../ObjectPool.h"


template<class T>
class MultiEntity :public AABBEntity {
protected:
	//keep them for usage, and care nothing about update, draw or delete
	std::vector<T*> m_entities;
	//pointer
	ObjectPool*m_pObjectPool;

public:

	MultiEntity(ObjectPool*pObjectPool):m_pObjectPool(pObjectPool){}

	//do you need to delete m_entities by yourself???
	virtual ~MultiEntity(){}


	inline std::vector<T*>*GetEntities() { return &m_entities; }
	void AddEntity(T*entity);


	//override some unused functions
	//if your multiEntity needs no AABB access 
	//then ignore it.
	const glm::vec4&CalculateAABB()override;
	void OnSelect(const glm::vec4&AABB)override{}
	void OnRelease(const glm::vec4&AABB)override{}
	//to apply AABB here means, to change AABB of all the m_entities
	void ApplyAABB(const glm::vec4&AABB)override{}
};












//template class does not support separating cpp file from header file

template<class T>
inline void MultiEntity<T>::AddEntity(T * entity)
{
	m_entities.push_back(entity);
	m_pObjectPool->CreateNewObject(entity);
}

template<class T>
inline const glm::vec4 & MultiEntity<T>::CalculateAABB()
{
	// TODO: insert return statement here
	if (m_entities.empty()) return glm::vec4(0, 0, 50, 50);
	return ((AABBEntity*)m_entities[0])->GetAABB();
}
