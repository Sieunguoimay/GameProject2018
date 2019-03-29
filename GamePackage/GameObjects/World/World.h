#pragma once
#include"Platform.h"
#include"../EntityHierachy/MultiEntity.h"
//if you want your multiEntity class to support
//high level services such as Selecting provided by Editor
//you must Give all the Entities to the higher level container i.e. m_entities
//And you keep one copy of them for further needs other then highlevel services.

//now you don't have full control over the entities any more: no deletion
class World :public MultiEntity<Platform>{
public:
	World(ObjectPool*pObjectPool):MultiEntity<Platform>(pObjectPool){}
	void Init()override;
	void Update(float deltaTime)override;
	void AddEntity(Platform*platform);//my own Adding function
};










/*
	game file structure


	<world>
		<platform id="0" x="0" y="0" texture="name">
			<vertex id="0" x="0" y="0">
			<vertex id="1" x="0" y="0">
			<vertex id="2" x="0" y="0">
			<vertex id="3" x="0" y="0">
		</platform>

		<platform id="1">
		</platform>
	</world>
*/
