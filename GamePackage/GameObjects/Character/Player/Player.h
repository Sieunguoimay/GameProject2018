#pragma once
#include"../../EntityHierachy/HavingBodyEntity.h"
#include"../../../misc/StateMachine/StateMachine.h"
#include"ActionStates.h"
#include"PlayerActuator.h"
#include"PlayerSkin.h"
#include"PlayerDecisionMaker.h"
#include"PlayerControl.h"
#include"PlayerSensor.h"
#include"PlayerMemory.h"
class Player :public AnimationBodyEntity {

	PlayerDecisionMaker m_decisionMaker;
	PlayerActuator m_actuator;
	PlayerControl m_control;
	PlayerSensor m_sensor;
	PlayerMemory m_memory;
	PlayerSkin m_skin;
public:
	Player(const glm::vec4&AABB);
	~Player();

	//seting functions
	static Spawner*Spawn(InfoPacket*info);
	void SetupBody(bool atRunTime=false)override;
	void Update(float deltaTime)override;

	friend class PlayerDecisionMaker;
	friend class PlayerActuator;
	friend class PlayerControl;
	friend class PlayerSensor;
	friend class PlayerMemory;
	friend class PlayerSkin;

	friend class BrainStateOnTheFloor;
	friend class BrainStateNextToRockLHS;
	friend class BrainStateNextToRockRHS;
	friend class BrainStateOnRockTopRight;
	friend class BrainStateOnRockTopLeft;
	friend class BrainStateLongAction;

	friend class _GlobalCharacterState;


	PlayerDecisionMaker&	GetDecisionMaker()	{ return m_decisionMaker; }
	PlayerActuator&			GetActuator()		{ return m_actuator; }
	PlayerControl&			GetControl()		{ return m_control; }
	PlayerSensor&			GetSensor()			{ return m_sensor; }
	PlayerMemory&			GetMemory()			{ return m_memory; }
	PlayerSkin&				GetSkin()			{ return m_skin; }
};

