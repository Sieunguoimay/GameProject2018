#pragma once
#include"GamePackage\GameBase.h"
class RealGame :public GameBase {
public:

	void InitGameObjects()override;
	void Update()override;
};