#include "Skin.h"
#include"../misc/Locator.h"
AnimationSkin::AnimationSkin(SpriterEntity * spriterEntity, 
	float scale, glm::vec2 pos /*= glm::vec2(0.0, 0.0)*/, 
	float angle /*= 0.0*/)
	:Skin(pos, glm::vec2(100, 100), angle)
	, m_spriterEntity(spriterEntity)
	, m_firstTimeFlag(true),m_scale(1,1)
{
	m_spriterEntity->SetAnimation(0);
	m_spriterEntity->SetInfo(m_pos, m_angle, m_scale.x, m_scale.y, m_flip);
	m_spriterEntity->Update(0);
	m_spriterEntity->Draw();
	m_originalAABB = m_spriterEntity->GetAABB();
	m_AABBBuffer = m_originalAABB;
	m_originalCenter = m_pos - glm::vec2(m_originalAABB.x, m_originalAABB.y);
}

AnimationSkin::~AnimationSkin()
{
	delete m_spriterEntity;//clue of the single-copy property.
}



void AnimationSkin::Draw()
{
	m_spriterEntity->Draw();
}

//this function represents the  problem of born 
void AnimationSkin::SetAABB(const glm::vec4&AABB)
{
	m_AABBBuffer = AABB;
	m_scale.x = (AABB.z - AABB.x) / (m_originalAABB.z - m_originalAABB.x);
	m_scale.y = (AABB.w - AABB.y) / (m_originalAABB.w - m_originalAABB.y);

	m_pos = glm::vec2(AABB.x, AABB.y) + m_originalCenter*m_scale;
}



void AnimationSkin::Update(float deltaTime)
{
	glm::vec2 p = m_pos-m_originalCenter*m_scale;
	glm::vec2 d = glm::vec2(m_AABBBuffer.z - m_AABBBuffer.x, m_AABBBuffer.w - m_AABBBuffer.y);
	m_AABBBuffer.x = p.x;
	m_AABBBuffer.y = p.y;
	m_AABBBuffer.z = p.x+d.x;
	m_AABBBuffer.w = p.y+d.y;

	m_spriterEntity->SetInfo(m_pos, m_angle, m_scale.x, m_scale.y, m_flip);
	m_spriterEntity->Update(deltaTime);
}















NoAnimationSkin::NoAnimationSkin(Texture * texture)
	:m_pTexture(texture)
	,m_center(0.5f,0.5f)
{
	//if(size.x == -1.0f)
	SetSize(glm::vec2(m_pTexture->GetWidth(), m_pTexture->GetHeight()));
}

void NoAnimationSkin::Draw()
{
	//draw with pivot at the center of rectangle and angle in radian 
	glm::vec4 rect = glm::vec4(m_pos - 0.5f*m_size, m_size);
	const SpriteMesh*a=Locator::GetTextureRenderer()->Draw(
		rect,											//rectangle
		glm::vec4(0, 0, 1, 1),							//UVs
		m_pTexture->GetId(), 0,							//id and depth
		glm::vec4(1.0f), m_angle,						//color and angle
		glm::vec2(m_center.x*rect.z, m_center.y*rect.w),
		m_flip==FlipType::HORIZONTAL_FLIP,				//horizontal flip
		m_flip == FlipType::VERTICAL_FLIP);				//vertical flip

	//Handle NULL 
	if (!a) {m_AABB = glm::vec4(rect.x, rect.y, rect.x, rect.y);return;}

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
