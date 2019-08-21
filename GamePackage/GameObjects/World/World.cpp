#include "World.h"
#include"../../misc/Locator.h"
#include"../../misc/Editor/Editor.h"
#include"../../misc/GameDataLoader.h"

void World::Attach(AABBEntity * e, VertexObject * v)
{
	auto&pos = m_attachedEntityMap.find(e);
	if (pos != m_attachedEntityMap.end()) {
		pos->second->Dettach(e);
		m_attachedEntityMap[e] = v;
	}
	else
		m_attachedEntityMap.insert(std::pair<AABBEntity*, VertexObject*>(e, v));

	v->Attach(e);
}
void World::AddVertexObject(int i, int j, Platform * platform)
{
	VertexObjectData data;
	data.m_vertexIndex.x = i;
	data.m_vertexIndex.y = j;
	data.m_pPlatform = platform;

	VertexObject*a = (VertexObject*)m_pObjectPool->CreateNewObject(new InfoPacket(GWOID_VERTEX_OBJECT, (void*)&data));

	m_verexObjects.push_back(a);
}

VertexObject * World::GetVertexObject(int i)
{
	if (i >= (int)m_verexObjects.size())
		return NULL;
	return m_verexObjects[i];
}




void World::Init()
{
	GameLoader::LoadGameWorldFromXml("Resources/GameData/game_world_data.xml", this, m_pObjectPool);
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
