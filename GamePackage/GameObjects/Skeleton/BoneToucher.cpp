#include "BoneToucher.h"
#include"Bone.h"
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"

Bone*BoneToucher::m_pBone = NULL;

void BoneToucher::unTouch(Bone * bone)
{
	m_pBone = NULL;
	bone->GetBoneRenderer().m_hover = false;
}

void BoneToucher::Touch(Bone * bone)
{
	//condition to continue
	if (m_pBone != NULL) if (m_pBone != bone) return;
	

	glm::vec2 mousePosInWorld = Locator::GetInput()->GetMousePosInWorld();
	glm::vec2 firstPress = Locator::GetInput()->GetFirstPressedPosInWorld();
	bool translation = Locator::GetInput()->IsKeyPressed(Key::KEY_SPACE) && Locator::GetInput()->IsMousePressed();
	bool rotation = Locator::GetInput()->IsMousePressed();


	glm::vec4 box = bone->GetBoneRenderer().m_box;
	glm::mat3 into_local_matrix = glm::inverse(bone->GetTransform().m_matrix);
	glm::vec2 mousePosInLocal = glm::vec3(mousePosInWorld, 1.0f)*into_local_matrix;

	//first press flag
	static bool flag = false;

	if (!flag) {
		if (mousePosInLocal.x < box.x)			{ unTouch(bone); return; }
		if (mousePosInLocal.x > box.x + box.z)	{ unTouch(bone); return; }
		if (mousePosInLocal.y < box.y)			{ unTouch(bone); return; }
		if (mousePosInLocal.y > box.x + box.w)	{ unTouch(bone); return; }
	}

	//if the mouse is on the box
	//capture it
	if (m_pBone == NULL)
		m_pBone = bone;
	m_pBone->GetBoneRenderer().m_hover = true;



	//now find the angle difference to apply for the bone by using mouse pressed position

	static glm::vec2 oldOrigin;
	static glm::mat3 oldTransform;

	glm::vec2 boneOriginInWorld = bone->GetOriginInWorld();

	if (translation) {
		if (!flag) {
			flag = true;
			glm::mat3 localTransform = bone->GetLocalTransform().m_matrix;
			oldTransform = into_local_matrix*localTransform;
			oldOrigin = glm::vec2(localTransform[0][2], localTransform[1][2]);// same as bone->GetOriginInLocal();
		}
		glm::vec2 diff = glm::vec3(mousePosInWorld - firstPress, 0.0)*oldTransform;
		bone->SetOrigin(oldOrigin + diff);
		return;
	}



	float distance_allowed = box.z / 4.0f;
	static float oldAngle = 0;

	if (rotation) {
		if (!flag) {
			flag = true;
			oldAngle = bone->GetAngle();
		}

		glm::vec2 c = firstPress - boneOriginInWorld;
		float diff = c.x*c.x+c.y*c.y;
		if (diff > distance_allowed*distance_allowed) {
			glm::vec2 d = mousePosInWorld - boneOriginInWorld;;
			float angle1 = getAngle(c.x, c.y);
			float angle2 = getAngle(d.x, d.y);
			float deltaAngle = glm::degrees(angle2 - angle1);
			if (std::abs(deltaAngle) > 0.5) {
				bone->SetRotation(oldAngle + deltaAngle);
			}
		}
		return;
	}
		
	if(flag)
		flag = false;
}
