#include "Puppeteer.h"


Puppeteer::Puppeteer()
	:m_pCurrentKeyFrame(NULL)
	, m_timer(0)
	, m_timelineLength(0)
	,m_frameTime(0)
	,m_endLoopFlag(true)
	,m_nextKeyFrameFlag(0)
{
}

Puppeteer::~Puppeteer()
{
	for (Node<KeyFrame*>*node = m_timeline.head->next; node != m_timeline.tail; node = node->next) {
		for (auto&keyFrameData : node->data->m_data)
			delete keyFrameData;
		delete node->data ;
	}
}

void Puppeteer::Drive(Bone * bone, int index)
{
	if (m_nextKeyFrameFlag) {
		float angle = m_pNextKeyFrame->data->m_data.at(index)->m_angle;
		if (angle != bone->GetAngle())
			bone->RotateTo(angle, m_frameTime);

		glm::vec2 local = m_pNextKeyFrame->data->m_data.at(index)->m_local;
		glm::vec2 origin = bone->GetOriginInLocal();
		if (local.x != origin.x || local.y != origin.y)
			bone->MoveTo(local, m_frameTime);
		m_nextKeyFrameFlag--;
	}
}

void Puppeteer::Update(float deltaTime)
{

	if (m_timeline.empty())return;
	if (m_timer >= m_timelineLength) {
		m_timer = 0;
		m_endLoopFlag = true;
		m_pNextKeyFrame = m_timeline.head;
		NextKeyFrame();
	}
	m_timer += deltaTime;


	if (m_endLoopFlag) {
		if (m_timer >= m_pNextKeyFrame->data->m_time) {
			m_pCurrentKeyFrame = m_pNextKeyFrame;
			NextKeyFrame();
		}
	}
}

void Puppeteer::AddKeyFrame(KeyFrame * newKeyFrame)
{
	//atleast one node available 
	if (!m_timeline.empty()) {
		for (Node<KeyFrame*>*node = m_timeline.head->next; node != m_timeline.tail; node = node->next)
		{
			
			if (node->data->m_time == newKeyFrame->m_time) {
				delete newKeyFrame;
				return;
			}
			if (node->data->m_time > newKeyFrame->m_time) {
				m_timeline.insert(node->prev, newKeyFrame);
				return;
			}
		}
	}
	//no node available or all time is smaller then push back
	m_timeline.push_back(newKeyFrame);
	m_timelineLength = newKeyFrame->m_time;//by default
}
void Puppeteer::Ready()
{
	if (m_timeline.empty())return;
	m_pNextKeyFrame = m_timeline.head;
	NextKeyFrame();
}

void Puppeteer::NextKeyFrame()
{
	m_pNextKeyFrame = m_pNextKeyFrame->next;// m_timeline[m_index%m_numKeyFrames];
	if (m_pNextKeyFrame == m_timeline.tail) {
		m_pNextKeyFrame = m_timeline.head->next;
		m_endLoopFlag = false;
	}
	calculateFrameTime();
	m_nextKeyFrameFlag = m_pNextKeyFrame->data->m_data.size();;
}

void Puppeteer::SetTimelineLength(float timelineLength)
{
	m_timelineLength = timelineLength;
}


void Puppeteer::calculateFrameTime()
{
	if (m_pCurrentKeyFrame != m_timeline.tail->prev)
		m_frameTime = m_pNextKeyFrame->data->m_time - m_timer;
	else m_frameTime = m_timelineLength - m_timer + m_pNextKeyFrame->data->m_time;
}

KeyFrame * KeyFrame::SpawnKeyFrame(int skeletonSize)
{

	KeyFrame*newKeyFrame = new KeyFrame;
	newKeyFrame->m_data.reserve(skeletonSize);
	return newKeyFrame;
}



PuppeteerEditMode::PuppeteerEditMode()
{
}

PuppeteerEditMode::~PuppeteerEditMode()
{
}

void PuppeteerEditMode::Drive(Bone * bone, int index)
{
	if (m_nextKeyFrameFlag) {
		float angle = m_pNextKeyFrame->data->m_data.at(index)->m_angle;
		if (angle != bone->GetAngle())
			bone->SetRotation(angle);

		glm::vec2 local = m_pNextKeyFrame->data->m_data.at(index)->m_local;
		glm::vec2 origin = bone->GetOriginInLocal();

		if (local.x != origin.x || local.y != origin.y)
			bone->SetOrigin(local);
		m_nextKeyFrameFlag--;
	}
}

void PuppeteerEditMode::Update(float deltaTime)
{

}
