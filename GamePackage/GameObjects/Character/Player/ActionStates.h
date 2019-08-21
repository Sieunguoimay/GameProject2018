#pragma once
#include"../../../misc/StateMachine/StateMachine.h"
#include"../../SpriterEntity.h"

enum ActionStateEnum {
	AS_RUN_LEFT			,
	AS_RUN_RIGHT		,
	AS_JUMP				,
	AS_FALL				,
	AS_JUMP_LEFT		,
	AS_JUMP_RIGHT		,
	AS_PUSH_LEFT		,
	AS_PUSH_RIGHT		,
	AS_PULL_LEFT		,
	AS_PULL_RIGHT		,
	AS_CLIMB_ON_LEFT	,
	AS_CLIMB_ON_RIGHT	,
	AS_STOP				,
	AS_STAND			,
	AS_GLOBAL_STATE		,
	AS_TOTAL_NUM		
};

class BaseActionState :public State<class PlayerBody> {
protected:
	std::string m_name = "No name";
	int m_animationIndex;
	class PlayerBody*m_pPlayerBody;
	class PlayerSkin*m_pPlayerSkin;
public:
	BaseActionState(int index):m_animationIndex(index){}
	BaseActionState() { SDL_Log("Created BaseCharacterState"); }
	~BaseActionState() { SDL_Log("Destroyed BaseCharacterState"); }

	void Enter(PlayerBody*entity) override;
	inline void Execute(PlayerBody*entity, float deltaTime) override { Execute(); }
	inline void Exit(PlayerBody*entity) override { Exit(); }

	inline BaseActionState* SetAnimationIndex(int index) { m_animationIndex = index; return this; }
	inline BaseActionState* SetName(const std::string&name) { m_name = name; return this;}

	inline virtual void Enter(){}
	inline virtual void Execute(){}
	inline virtual void Exit(){}
};



class _JumpingState :public BaseActionState {
public:
	void Execute() override;
};

class _JumpingBufferState :public BaseActionState {
public:
	void Execute() override;
};

class _StoppingState :public BaseActionState {
public:
	void Execute() override;
};


class _GlobalCharacterState :public BaseActionState {
public:
	void Enter() override{}
	void Execute() override;
};
