#pragma once
class Rotator {
public:
	enum CurveType {
		LINEAR,
		QUADRATIC
	};
private:
	float m_oldAngle;
	float m_newAngle;
	float m_angle;
	float m_time;
	float m_upperBoundTime;
	float m_angularSpeed;
	bool m_rotatingFlag;
	CurveType m_curveType;
public:
	void Init(const float& initialAngle,const float& angularSpeed,const CurveType& curveType);
	void RotateTo(float newAngle, float deltaTime);
	inline float GetAngle() { return m_angle; }
};