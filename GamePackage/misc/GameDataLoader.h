#pragma once
#include"../GameObjects/ObjectPool.h"
#include"../GameObjects/World/World.h"

class GameLoader {
public:
	static void LoadGameWorldFromXml(const char*filename, World*pWorldObject, ObjectPool*pObjectPool);
	static void WriteWorldStructureToFile(const char*filename, World*pWorld, ObjectPool*pObjectPool);
};
