#pragma once
#include"Entity.h"

template<class T>
class MultiEntity :public AABBEntity {
protected:
	std::vector<T*> m_entities;
public:
	~MultiEntity();

	virtual void Update(float deltaTime) override;
	virtual void Draw()override;

	virtual void OnSelect(const glm::vec2&AA, const glm::vec2&BB);
	virtual void OnRelease(const glm::vec2&AA, const glm::vec2&BB);
};














//template class does not support separating cpp file from header file

template<class T> MultiEntity<T>::~MultiEntity()
{
	for (auto&a : m_entities)
		delete a;
}

template<class T> void MultiEntity<T>::Update(float deltaTime)
{
	for (auto&entity : m_entities)
		((AABBEntity*)entity)->Update(deltaTime);
}

template<class T> void MultiEntity<T>::Draw()
{
	for (auto&entity : m_entities)
		((AABBEntity*)entity)->Draw();
}

//to implement these functions:
//take out the AABB of their enetites and then ...
template<class T> void MultiEntity<T>::OnSelect(const glm::vec2 & AA, const glm::vec2 & BB)
{
}

template<class T> void MultiEntity<T>::OnRelease(const glm::vec2 & AA, const glm::vec2 & BB)
{
}
