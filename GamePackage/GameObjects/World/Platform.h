#pragma once
#include<vector>
#include"../../2D/Texture.h"
#include"Box2D\Box2D.h"
#include"../PhysicsEngine/BodyBase.h"
struct PlatformPhysicsBody{
	std::vector<glm::vec2>m_vertices;
};



class Platform :public BodyBase{
private:
	//plaform bodies' polygons are created dynamically out side of this class
	//and awared by this class through AddPlatformPhysicsBody()
	std::vector<PlatformPhysicsBody*> m_polygons;
	glm::vec2 m_position;
	Texture*m_pTexture;
	b2Body*m_body;
	void createBody(const glm::vec2&pos);

public:
	Platform(Texture*pTexture);
	~Platform();
	void Init();


	//getters and setters
	//inline does not allow to define function in cpp file
	inline void SetPosition(glm::vec2 pos) { m_position = pos; }
	inline glm::vec2&GetPosition() { return m_position; }
	
	//allows full access
	inline void AddPlatformPhysicsBody(PlatformPhysicsBody*newPolygon) { m_polygons.push_back(newPolygon); }
	inline std::vector<PlatformPhysicsBody*>&GetPolygons() { return m_polygons; }
	inline Texture*GetTexture() { return m_pTexture; }
};
