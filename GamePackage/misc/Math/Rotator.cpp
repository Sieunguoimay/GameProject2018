#include "Rotator.h"
#include<iostream>
#include"../../misc/Math/Geometry.h"
void Rotator::Init(const float& initialAngle, const float& angularSpeed, const CurveType& curveType)
{
	m_angle = initialAngle;
	m_newAngle = initialAngle;
	m_oldAngle = initialAngle;
	m_angularSpeed = angularSpeed;
	m_curveType = curveType;
	m_rotatingFlag = false;
	m_time = 0;
	m_upperBoundTime = 0;
}


void Rotator::RotateTo(float newAngle, float deltaTime)
{
	if (std::abs(newAngle - m_newAngle)>1.0f) {
		m_rotatingFlag = true;
		m_newAngle = newAngle;
		m_time = 0;

		float delta = std::abs(m_newAngle - m_angle);
		float crossZero = 0;
		if (delta > 180) {
			delta = std::abs(360.0f - delta);
			crossZero = (m_newAngle > m_oldAngle ? 360 : -360);
		}
		m_oldAngle = m_angle + crossZero;
		m_upperBoundTime = delta / m_angularSpeed;

		//printf("delta %f old angle %f new angle %f time %f\n", delta, m_oldAngle,m_newAngle, m_upperBoundTime);
	}
	if (m_rotatingFlag) {
		m_time += deltaTime;
		if (m_time >= m_upperBoundTime) {
			m_rotatingFlag = false;
		}
		m_time = std::fmin(m_time, m_upperBoundTime);
		//
		//switch (m_curveType) {
		//case CurveType::LINEAR:
		//	m_angle = m_oldAngle + sign*m_time*m_angularSpeed;
		//	break;
		//case CurveType::QUADRATIC:
		//	m_angle = quadratic(m_oldAngle, 0.25f*(m_newAngle + m_oldAngle), m_newAngle, m_time / m_upperBoundTime);
		//	break;
		//}
		//m_angle = _linear(m_oldAngle, m_newAngle, m_time / m_upperBoundTime);
		m_angle = quadratic(m_oldAngle, m_oldAngle+0.85f*(m_newAngle - m_oldAngle), m_newAngle, m_time / m_upperBoundTime);
		//normalizeDegreeAngle(m_angle);
		//printf("%f\n", m_angle);
	}
}
