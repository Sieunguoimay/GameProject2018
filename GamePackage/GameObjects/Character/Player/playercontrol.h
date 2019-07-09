#pragma once
#include"Player.h"
#include"../../../Input/InputHandler.h"
class PlayerCommand :public Command {
	Player*m_pPlayer;
	char m_commandIndex;
public:
	PlayerCommand(Player*pPlayer,char command)
		:m_pPlayer(pPlayer),m_commandIndex(command){}
	void Execute()override {
	
		if (m_commandIndex == 3) m_pPlayer->HoldTouchPoint();
		if (m_commandIndex == 2) m_pPlayer->JumpSignal();
		if (m_commandIndex == 0) m_pPlayer->LeftSignal();
		else if (m_commandIndex == 1) m_pPlayer->RightSignal();
	}
};
#include"../../ObjectPool.h"
class PlayerControl :public InputHandler{
	ObjectPool*m_pObjectPool;
public:
	PlayerControl(ObjectPool*pObjectPool):m_pObjectPool(pObjectPool){}

	void Init() override{
		Player*player=(Player*)m_pObjectPool->GetSavedObject(GWOID_PLAYER);
		AddCommand(new PlayerCommand(player, 0), HandleType::HT_KEY_A);
		AddCommand(new PlayerCommand(player, 1), HandleType::HT_KEY_D);
		AddCommand(new PlayerCommand(player, 2), HandleType::HT_KEY_W);
		AddCommand(new PlayerCommand(player, 3), HandleType::HT_SPACE);
	}
};