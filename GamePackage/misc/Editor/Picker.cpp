#include "Picker.h"

void Picker::Process(AABBEntity * entity, InputEvent & inputEvent)
{
	glm::vec2 pos = inputEvent.GetMousePos();
	if (entity->CheckAABBAgainstPoint(pos)) {
		entity->Select();
		//entity->OnSelect(entity->GetAA(), entity->GetBB());
	}else
		entity->Release();
}
