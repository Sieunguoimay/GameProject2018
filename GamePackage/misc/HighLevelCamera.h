#pragma once
#include"../2D/Camera2D.h"
#include"../2D/FreeTypeLoader.h"
#include"../GameObjects/EntityHierachy/Entity.h"
#include"../Input/InputHandler.h"

class HighLevelCamera :public Command,public Entity, Camera2D{
	Camera2D*m_pCamera;
	FreeTypeLoader*m_pFreeTypeLoader;
	bool m_zoomEnabled;
public:
	HighLevelCamera(Camera2D*pCamera, FreeTypeLoader*pFreeTypeLoader);

	
	void Update(float deltaTime)override;
	void Draw()override;
	void Execute()override;
	inline bool IsUnderInputHandler() override{ return false; }
};
