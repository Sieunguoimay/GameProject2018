#include"BodySegment.h"
#include"../../misc/Locator.h"
#include"../../misc/Math/Geometry.h"
void BodySegment::Update(const FlipType & flip)
{
	if (flip == HORIZONTAL_FLIP) {
		m_info.angle = -m_info.angle;
		m_info.y = -m_info.y;
		normalizeDegreeAngle(m_info.angle);
		return;
	}
	else if (flip == VERTICAL_FLIP) {
		//m_info.angle = -m_info.angle;
		//m_info.x = -m_info.x;
		//normalizeDegreeAngle(m_info.angle);
	}

}
void BodySegment::TakeControl(const float& oldAngle, const float& angularSpeed, Rotator::CurveType curveType)
{
	underControl = true;
	m_rotator.Init(oldAngle, angularSpeed, curveType);
}

void BodySegment::SteerAngle(float targetAngle, float deltaTime) {

	m_rotator.RotateTo(targetAngle, deltaTime);
	m_info.angle = m_rotator.GetAngle();
}






void SpriteSegment::Update(const FlipType& flip)
{
	if (flip == HORIZONTAL_FLIP) {
		m_info.y = -m_info.y;
		m_info.angle = -m_info.angle;
		normalizeDegreeAngle(m_info.angle);
		m_pivot_y = 1 - m_pivot_y;
	}
	else if (flip == VERTICAL_FLIP) {
		//m_info.x = -m_info.x;
		//m_info.angle = -m_info.angle;
		//normalizeDegreeAngle(m_info.angle);
		//m_pivot_x = 1 - m_pivot_x;
	}

}

void SpriteSegment::Draw(const FlipType& flip) {

	float w = m_info.scaleX*((float)m_pTexture->GetWidth());
	float h = m_info.scaleY*((float)m_pTexture->GetHeight());
	m_pivot_x = w*(float)m_pivot_x;
	m_pivot_y = h*(float)m_pivot_y;
	
	if(flip != HORIZONTAL_FLIP)
		m_info.y = m_info.y - h;
	
	//what crossing this line are: pos, angle, pivot, w,h
	float angle = glm::radians(m_info.angle);
	const SpriteMesh*a = Locator::GetTextureRenderer()->Draw(
		glm::vec4(m_info.x, m_info.y, w, h),
		glm::vec4(0.0, 0.0, 1.0, 1.0),
		m_pTexture->GetId(), 0,
		glm::vec4(1.0f), angle,
		glm::vec2(m_pivot_x, m_pivot_y), flip== VERTICAL_FLIP, flip== HORIZONTAL_FLIP);

	if (a == NULL) {
		m_AABB = glm::vec4(m_info.x, m_info.y, m_info.x, m_info.y);
		return;
	}
	//calculate AABB
	glm::vec2 p[3] = { a->bottomLeft.position,
		a->topRight.position,a->bottomRight.position };


	m_AABB = glm::vec4(a->topLeft.position, a->topLeft.position);
	for (int i = 0; i < 3; i++) {
		if (m_AABB.x > p[i].x) m_AABB.x = p[i].x;
		else if (m_AABB.z < p[i].x) m_AABB.z = p[i].x;
		if (m_AABB.y > p[i].y) m_AABB.y = p[i].y;
		else if (m_AABB.w < p[i].y) m_AABB.w = p[i].y;
	}


}

void SpriteSegment::SetInfo(const SpatialInfo & info, float pivot_x, float pivot_y, bool useDefaultPivot, Texture*pTexture)
{
	this->m_info = info;
	this->m_pivot_x = pivot_x;
	this->m_pivot_y = pivot_y;
	this->m_useDefaultPivot = useDefaultPivot;
	this->m_pTexture = pTexture;
}
