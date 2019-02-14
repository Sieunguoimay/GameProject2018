#include"../Entity.h"
#include"Box2D\Box2D.h"
class TestBox2D:public Entity,public b2QueryCallback
	,public b2RayCastCallback{
	b2World*m_pWorld;
	b2Body*body;
	b2DistanceJoint*dj;
public:
	TestBox2D(b2World*pWorld);
	void Update(float deltaTime)override;
	void Draw()override;

	bool ReportFixture(b2Fixture*fixture)override;
	float32 ReportFixture(b2Fixture*fixture, const b2Vec2&point, const b2Vec2&normal, float32 fraction);

	static void Distance(b2DistanceOutput*output,b2Shape*shapeA,const b2Transform&transformA, b2Shape*shapeB,const b2Transform&trasformB);
};
