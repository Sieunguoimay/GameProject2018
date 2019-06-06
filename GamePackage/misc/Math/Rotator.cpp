#include "Rotator.h"
#include<iostream>
#include"../../misc/Math/Geometry.h"
void Rotator::Init(const float& initialAngle, const float& angularSpeed, const CurveType& curveType)
{
	m_angle = initialAngle;
	m_newAngle = initialAngle;
	m_angularSpeed = angularSpeed;
	m_rotatingFlag = false;
	m_curveType = curveType;
	printf("%f initially\n", initialAngle);
}


void Rotator::RotateTo(float newAngle, float deltaTime)
{
	if (std::abs(newAngle - m_newAngle)>1.0f) {
		m_rotatingFlag = true;
		m_newAngle = newAngle;
		m_time = 0;

		float delta = std::abs(m_newAngle - m_angle);
		m_crossZero = (delta > 180 ? (m_newAngle > m_oldAngle ? 360 : -360) : 0);

		m_oldAngle = m_angle + m_crossZero;
		m_upperBoundTime = delta / m_angularSpeed;
		printf("%f old %f new\n", m_oldAngle,m_newAngle);
	}
	if (m_rotatingFlag) {
		m_time += deltaTime;
		if (m_time >= m_upperBoundTime) {
			m_rotatingFlag = false;
		}
		m_time = std::fmin(m_time, m_upperBoundTime);
		float sign = (m_oldAngle < m_newAngle ? 1: -1);
		
		switch (m_curveType) {
		case CurveType::LINEAR:
			m_angle = m_oldAngle + sign*m_time*m_angularSpeed;
			break;
		case CurveType::QUADRATIC:
			m_angle = quadratic(m_oldAngle, 0.25f*(m_newAngle + m_oldAngle), m_newAngle, m_time / m_upperBoundTime);
			break;
		}
	}
}
