#pragma once
#include"../../misc/Math/Rotator.h"
#include"TimelineKey.h"
#include"../../2D/Texture.h"
class BodySegment {
	Rotator m_rotator;
	SpatialInfo m_info;
	bool underControl = false;
	int id;
	int parentId;
public:
	BodySegment(int id, int parentId):id(id), parentId(parentId),underControl(false) {}
	~BodySegment() {}

	inline int GetId() { return id; }
	inline int GetParentId() { return parentId; }

	void Update(const FlipType&flip);


	
	//to control this segment
	void TakeControl(const float& oldAngle, const float& angularSpeed, Rotator::CurveType curveType);
	void SteerAngle(float targetAngle, float deltaTime);
	inline void Release() { underControl = false;}
	inline bool IsControlled() { return underControl; }

	//remove this function as things are done
	inline void SetInfo(const SpatialInfo& info) { m_info = info; }
	inline SpatialInfo&GetInfo() { return m_info; }
	inline glm::vec2 GetPos() { return glm::vec2(m_info.x, m_info.y); }
	inline void SetAngle(float angle) { m_info.angle = angle; }
	inline float GetAngle() { return m_info.angle; }
};

class SpriteSegment {
	SpatialInfo m_info;

	int m_id;
	int m_parentId;
	int m_z_index;

	bool m_useDefaultPivot; // true if missing pivot_x and pivot_y in object tag
	float m_pivot_x = 0;
	float m_pivot_y = 1;
	glm::vec4 m_AABB;
	Texture*m_pTexture;
public:
	SpriteSegment(int id, int parentId, int z_index) :/*timelineKey(new SpriteTimelineKey()), */
		m_id(id),m_parentId(parentId),m_z_index(z_index){}
	~SpriteSegment() { /*delete timelineKey; */}

	void Update(const FlipType& flip);
	void Draw(const FlipType& flip);

	inline int GetId() { return m_id; }
	inline int GetParentId() { return m_parentId; }

	void SetInfo(const SpatialInfo&info, float pivot_x, float pivot_y, bool useDefaultPivot,Texture*pTexture);
	const glm::vec4& GetAABB() { return m_AABB; }
	inline SpatialInfo& GetInfo() { return m_info; }

};
