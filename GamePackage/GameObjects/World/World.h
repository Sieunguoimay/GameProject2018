#pragma once
#include"Platform.h"
#include"../EntityHierachy/Entity.h"

class World :public AABBEntity{
	std::vector<Platform*>m_platforms;

public:
	World();
	~World();
	void Update(float deltaTime)override;
	void Draw()override;

	//implement unused
	void OnSelect(const glm::vec2&AA, const glm::vec2&BB) {};
	void OnRelease(const glm::vec2&AA, const glm::vec2&BB) {};
	//getters and setters

	const std::vector<Platform*>*GetPlatform() { return &m_platforms; }
	void AddPlatform(Platform*newPlatform) { m_platforms.push_back(newPlatform); }
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
