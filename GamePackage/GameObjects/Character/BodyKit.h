#pragma once
#include"Box2D\Box2D.h"
#include"../SpriterEntity.h"

enum EntityType {
	DEFAULT = 1 << 0,
	GROUND = 1 << 1,
	ITEM = 1 << 2,
	CHARACTER = 1 << 3
};
struct UserData {
	EntityType m_type;
	void*m_pointer; //unique part
};

typedef std::map<void*, EntityType> CollisionData;


class BodyKit {
	b2Body*m_body;
	std::vector<b2Fixture*>m_fixtures;
	CollisionData m_collisions;
	UserData m_userData;
	glm::vec2 m_dimension;

	b2World*m_pWorld;
	virtual void SetupBody(glm::vec2 pos, glm::vec2 dimension);


public:
	BodyKit(b2World*world, glm::vec2 pos, glm::vec2 dimension);
	~BodyKit();

	void SetLinearVelocity(float x, float y);
	void ApplyForce(float x, float y);

	void SendInfo(UserData userData);
	void ClearCollisionData();

	float GetAngle();
	glm::vec2 GetPosition();
	CollisionData& GetAllCollisions();
	b2Body*GetBody();
	glm::vec2 GetDimension();


};

class BodyBase {
protected:
	BodyKit m_bodyKit;
public:
	BodyBase(b2World * world, glm::vec2 pos, glm::vec2 dimension)
		:m_bodyKit(world, pos, dimension){}

	virtual void Update(float deltaTime)=0;
	BodyKit&GetBodyKit() {return m_bodyKit;}
};