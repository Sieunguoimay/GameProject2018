#pragma once
#include"../../GameObjects/EntityHierachy/Entity.h"
#include"../../Input/InputEvent.h"
class Picker {
public:
	void Process(AABBEntity*entity,InputEvent&inputEvent);

};
