#include "World.h"
#include"../../misc/Locator.h"
#include"../../misc/Editor/Editor.h"


void World::Init()
{
	SDL_Log("Game World Initialized.");
}

void World::Update(float deltaTime)
{
}


void World::AddEntity(Platform * platform)
{
	//added feature for the purpose of new platform to come which will be put at the end.
	if(m_entities.size()>0){
		const glm::vec4& aabb0 = m_entities.back()->GetAABB();
		const glm::vec4& aabb = platform->GetAABB();

		//shift the AABB away - this function is called before the init of platform i.e. SetupBody()
		platform->SetAABB(glm::vec4(aabb0.z, aabb0.y, aabb0.z + aabb.z - aabb.x , aabb0.y + aabb.w-aabb.y));
	}
	MultiEntity<Platform>::AddEntity(platform);//keep a copy
}




//void World::Update(float deltaTime)
//{
//}

//void World::Draw()
//{
//	//for (auto&platform : m_entities) {
//	//	const glm::vec4&aabb = platform->GetAABB();
//	//	glm::vec4 rect((aabb.x + aabb.z) / 2.0f, (aabb.y + aabb.w) / 2.0,
//	//		aabb.z-aabb.x);
//
//	//	Locator::GetTextureRenderer()->Draw(glm::vec4(
//	//		pos,
//	//		platform->GetTexture()->GetWidth(),
//	//		platform->GetTexture()->GetHeight()),
//	//		glm::vec4(0, 0, 1, 1),
//	//		platform->GetTexture()->GetId(),
//	//		0,
//	//		glm::vec4(1.0f));
//	}
//}
