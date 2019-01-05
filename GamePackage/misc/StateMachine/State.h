#pragma once
//Base State class
template<class T>
class State {
public:
	virtual ~State(){}
	virtual void Enter(T*entity) = 0;
	virtual void Execute(T*entity,float deltaTime) = 0;
	virtual void Exit(T*entity) = 0;
};
