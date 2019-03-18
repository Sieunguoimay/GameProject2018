#include "Skin.h"
#include"../misc/Locator.h"
AnimationSkin::AnimationSkin(SpriterEntity * spriterEntity, float scale, glm::vec2 pos /*= glm::vec2(0.0, 0.0)*/, float angle /*= 0.0*/)
	:Skin(pos,glm::vec2(100,100),angle)
	,m_spriterEntity(spriterEntity){}

AnimationSkin::~AnimationSkin()
{
	delete m_spriterEntity;//clue of the single-copy property.
}

void AnimationSkin::Draw()
{
	m_spriterEntity->Draw(m_pos, m_angle, 1, 1, m_flip);
	
	const glm::vec4&aabb = m_spriterEntity->GetAABB();
	Locator::GetPrimitiveRenderer()->DrawBox(
		glm::vec4(aabb.x, aabb.y, aabb.z - aabb.x, aabb.w - aabb.y),0,glm::vec2(0.0),
		glm::vec4(1.0f,0,0,1.0));
}

void AnimationSkin::Update(float deltaTime)
{
	m_spriterEntity->Update(deltaTime);

	//const glm::vec4&aabb = m_spriterEntity->GetAABB();
	//m_pos.x = aabb.x;
	//m_pos.y = aabb.y;
	//m_size.x = aabb.z - aabb.x;
	//m_size.y = aabb.w - aabb.y;
}
void AnimationSkin::SetAnimationSwitchingTime(const char*animationA, const char*animationB, int time)
{
	m_spriterEntity->GetSclmEntity()->animationSwitchingTime
		.at(m_spriterEntity->GetAnimationIndex(animationA))
		.at(m_spriterEntity->GetAnimationIndex(animationB)) = time;
}



NoAnimationSkin::NoAnimationSkin(Texture * texture)
	:m_pTexture(texture) 
{
	SetSize(glm::vec2(m_pTexture->GetWidth(), m_pTexture->GetHeight()));
} 

void NoAnimationSkin::Draw()
{
	//draw with pivot at the center of rectangle and angle in radian 
	Locator::GetTextureRenderer()->Draw(
		glm::vec4(m_pos-0.5f*m_size, m_size),			//rectangle
		glm::vec4(0, 0, 1, 1),							//UVs
		m_pTexture->GetId(), 0,							//id and depth
		glm::vec4(1.0f), m_angle,						//color and angle
		glm::vec2(m_size.x / 2.0f, m_size.y / 2.0f),	//center
		m_flip==FlipType::HORIZONTAL_FLIP,				//horizontal flip
		m_flip == FlipType::VERTICAL_FLIP);				//vertical flip
}
