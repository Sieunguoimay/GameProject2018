#pragma once
#include"State.h"
template<class T>
class StateMachine
{
	T*m_owner;
	State<T>*m_currentState;
	State<T>*m_previousState;
	State<T>*m_globalState;
public:


	StateMachine()
		:m_owner(nullptr)
		, m_currentState(nullptr)
		, m_previousState(nullptr)
		, m_globalState(nullptr)
	{
	}


	~StateMachine()
	{
	}
	void Init(T * owner, State<T>* currentState, State<T>* previousState, State<T>* globalSate)
	{
		this->m_owner = owner;
		this->m_currentState = currentState;
		this->m_previousState = previousState;
		this->m_globalState = globalSate;
		this->m_currentState->Enter(m_owner);
		this->m_globalState->Enter(m_owner);
	}
	void Update(float deltaTime) {
		if (!m_owner) return;
		if (m_currentState) m_currentState->Execute(m_owner,deltaTime);
		if (m_globalState) m_globalState->Execute(m_owner,deltaTime);
	}
	bool ChangeState(State<T>*newState) {
		if (IsInState(newState))return false;
		if (m_currentState) {
			m_previousState = m_currentState;
			m_currentState->Exit(m_owner);
		}
		m_currentState = newState;
		m_currentState->Enter(m_owner);
		return true;
	}
	void RevertToPrevState() {
		if (m_previousState)
			ChangeState(m_previousState);
	}
	bool IsInState(State<T>*state) {
		if (m_currentState == state) return true;
		return false;
	}

	State<T>*GetCurrentState() { return m_currentState; }
	State<T>*GetPreviousState() { return m_previousState; }
	State<T>*GetGlobalState() { return m_globalState; }
};

