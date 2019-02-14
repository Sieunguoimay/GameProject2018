#include "World.h"
#include"../../misc/Locator.h"
World::World()
{
}

World::~World()
{
	for (auto&a : m_platforms)
		delete a;
}

void World::Update(float deltaTime)
{
}

void World::Draw()
{
	for(auto&platform:m_platforms)
		Locator::GetTextureRenderer()->Draw(glm::vec4(
			platform->GetPosition(),
			platform->GetTexture()->GetWidth(), 
			platform->GetTexture()->GetHeight()),
			glm::vec4(0, 0, 1, 1), 
			platform->GetTexture()->GetId(),
			0,
			glm::vec4(1.0f));
}
