#pragma once
#include<Box2D/Box2D.h>
#include<functional>
#include<vector>
namespace pe {
	class BaseAction {
		bool m_autorelease;
	public:
		BaseAction():m_autorelease(true){}
		inline virtual ~BaseAction() { }
		inline virtual void Run(float deltaTime) = 0;
		inline void Retain() { m_autorelease = false; }
		inline bool IsAutorelease()const { return m_autorelease; }
		inline virtual bool Timeout() { return true; }//true means run one time and die
	};
	class TangibleAction :public BaseAction {
	protected:
		b2Body*m_pBody;
		float m_duration;
		float m_timer;
	public:
		TangibleAction(b2Body*pBody, float duration)
			:m_pBody(pBody), m_duration(duration){}
		virtual ~TangibleAction(){ m_pBody = NULL; }
		inline virtual void Run(float deltaTime)override { m_timer += deltaTime; if (m_timer > m_duration) m_timer = m_duration; }
		inline bool Timeout() override { return m_timer >= m_duration; }
	};

	class MoveTo :public TangibleAction {
		b2Vec2 m_target;
		b2Vec2 m_oldPos;
	public:
		MoveTo(b2Body*pBody, const b2Vec2&target, float duration);		
		~MoveTo()override;
		void Run(float deltaTime)override;
	};


	class Sequence : public BaseAction {
		std::vector<BaseAction*>m_actions;
		int m_actionIndex;
	public:
		Sequence(BaseAction*action, ...);
		~Sequence()override;
		void Run(float deltaTime)override;
		inline bool Timeout()override { return (m_actionIndex==m_actions.size()); }
	};


	class Callback :public BaseAction {
		std::function<void()>m_callback;
	public:
		Callback(std::function<void()>callback):m_callback(callback){}
		void Run(float deltaTime)override { m_callback(); }
	};
}