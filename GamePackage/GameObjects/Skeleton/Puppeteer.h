#pragma once
#include<vector>
#include<glm\glm.hpp>
#include"Bone.h"
#include"../../misc/List.h"
struct KeyFrameData {
	KeyFrameData(glm::vec2 local, float angle)
		:m_local(local), m_angle(angle){}
	KeyFrameData(float x, float y, float angle)
		:m_local(x,y), m_angle(angle) {}

	glm::vec2 m_local;
	float m_angle;
};

//Given: A KeyFrame container is sychronized with the Skeleton metadata
struct KeyFrame {
	std::vector<KeyFrameData*> m_data;;
	float m_time = 0;
	static KeyFrame*SpawnKeyFrame(int skeletonSize);
};

class Puppeteer {
protected:
	//A list of position of each bone at each particular key time
	List<KeyFrame*>m_timeline;
	Node<KeyFrame*>*m_pCurrentKeyFrame;
	Node<KeyFrame*>*m_pNextKeyFrame;
	float m_timer;
	float m_timelineLength;
	bool m_endLoopFlag;
	int m_nextKeyFrameFlag;
	float m_frameTime;
	void calculateFrameTime();
public:
	Puppeteer();
	virtual ~Puppeteer();

	//once this function is called, once key frame Data is used together with one bone
	//and the key frame data is picked up from the current key frame.
	//after the last key frame data is used, the current key frame goes to the next one.
	virtual void Drive(Bone*bone, int index);
	virtual void Update(float deltaTime);

	void AddKeyFrame(KeyFrame*newKeyFrame);
	void Ready();
	void NextKeyFrame();
	void SetTimelineLength(float timelineLength);
};

class PuppeteerEditMode :public Puppeteer {
public:
	PuppeteerEditMode();
	~PuppeteerEditMode();
	void Drive(Bone*bone, int index);
	void Update(float deltaTime);
};