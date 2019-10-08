#pragma once
#include"../../misc/Math/Rotator.h"
#include"TimelineKey.h"
#include"../../2D/Texture.h"
class BoneSegment:public BoneTimelineKey {
public:
	enum State {
		BSS_IDLE,
		BSS_CONTROL_CAPTURED,
		BSS_RELEASE_CONTROL_SIGNAL
	};
private:
	Rotator					m_rotator;
	State					m_state;
	int						m_parentId;
	int						m_id;
	friend class SpriterEntity;
public:
	BoneSegment(int id, int parentId):m_id(id), m_parentId(parentId), m_state(State::BSS_IDLE) {}

	void Update();
	void Flip(const FlipType&flip);
	
	//to control this segment
	void SteerAngle(float targetAngle, float deltaTime, float angularSpeed, Rotator::CurveType curveType);
	bool IsControlled()					{ return (m_state == State::BSS_CONTROL_CAPTURED); }
	bool IsReleaseSignaled()			{ return (m_state == State::BSS_RELEASE_CONTROL_SIGNAL); }
	bool IsIdle()						{ return (m_state == State::BSS_IDLE); }
	float GetOldAngleAfterSteering()	{ return m_rotator.GetOldAngle(); }

	//remove this function as things are done
	void			SetInfo	(const SpatialInfo& info)	{ m_info = info; }
	void			SetAngle (float angle)				{ m_info.angle = angle; }
	SpatialInfo&	GetInfo()							{ return m_info; }
	glm::vec2		GetPos()							{ return glm::vec2(m_info.x, m_info.y); }
	float			GetAngle()							{ return m_info.angle; }
	int				GetId()								{ return m_id; }
	int				GetParentId()						{ return m_parentId; }

};

class SpriteSegment:public SpriteTimelineKey {

	int m_id;
	int m_parentId;
	int m_z_index;

	glm::vec4 m_AABB;
	Texture*m_pTexture;
public:
	SpriteSegment(int id, int parentId, int z_index) :/*timelineKey(new SpriteTimelineKey()), */
		m_id(id),m_parentId(parentId),m_z_index(z_index){}
	~SpriteSegment() { /*delete timelineKey; */}

	void Flip(const FlipType& flip);
	void Draw(const FlipType& flip,bool calculateSizeFlag = true);

	void SetInfo(const SpatialInfo&info, float pivot_x, float pivot_y, bool useDefaultPivot,Texture*pTexture);
	const glm::vec4& GetAABB() { return m_AABB; }
	SpatialInfo& GetInfo() { return m_info; }
	int GetId() { return m_id; }
	int GetParentId() { return m_parentId; }

};
