#include "HighLevelCamera.h"
#include"../Locator.h"
#include"../Assistances.h"
HighLevelCamera::HighLevelCamera(Camera2D*pCamera,FreeTypeLoader * pFreeTypeLoader)
	:m_pFreeTypeLoader(pFreeTypeLoader) 
	,m_pCamera(pCamera){}

void HighLevelCamera::Update(float deltaTime)
{
	MouseEvent&mouseEvent = Locator::GetInputEvent()->GetRightMouse();
	static glm::vec2 oldCamPos;

	if (mouseEvent.IsClickSignalled())
		oldCamPos = m_pCamera->GetPosition();
	
	if (mouseEvent.IsMousePressed()) {
		glm::vec2 mouseDiff = mouseEvent.GetMousePosInWorld() - mouseEvent.GetFirstPressedPosInWorld();
		if (m_zoomEnabled) {
			float xDiff = mouseDiff.x;
			m_pCamera->SetScale(glm::max(glm::min(1.0f + xDiff*0.001f,5.0f),0.1f));
		}
		else {
			m_pCamera->SetPos(oldCamPos - 0.8f*mouseDiff);
		}
	}
	//reset the scale to 1 if zoom is disabled
	if (mouseEvent.IsReleaseSignalled())
		m_pCamera->SetScale(1.0f);
	m_zoomEnabled = false;
}

void HighLevelCamera::Draw()
{
	//std::string pos = _to_string((int)m_pCamera->GetPosition().x)+" "+ _to_string((int)m_pCamera->GetPosition().y);
	//m_pFreeTypeLoader->Draw(Locator::GetTextureRenderer(), pos.c_str(), m_pCamera->GetPosition()-m_pCamera->GetSize()*0.45f);
}

void HighLevelCamera::Execute()
{
	m_zoomEnabled = true;
}
