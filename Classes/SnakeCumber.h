//
//  SnakeCumber.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 10..
//
//

#ifndef __DGproto__SnakeCumber__
#define __DGproto__SnakeCumber__

#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <vector>
#include "CumberEmotion.h"
#include <deque>

struct Trace
{
	CCPoint position; // 자취의 위치와
	float directionRad; // 자취의 방향.
};
/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.
class SnakeCumber : public KSCumberBase
{
public:
	SnakeCumber() : m_speed(2.f), m_scale(1.f), RADIUS(22.f), mEmotion(nullptr)
	{
		
	}
	
	virtual ~SnakeCumber();
	
	virtual void onStartMoving()
	{
		m_state = CUMBERSTATEMOVING;
		schedule(schedule_selector(KSCumberBase::movingAndCrash));
	}
	virtual void onStopMoving()
	{
		m_state = CUMBERSTATESTOP;
	}
	virtual void movingAndCrash(float dt);
	void attack(float dt);
	virtual bool init();
	CREATE_FUNC(SnakeCumber);
	virtual void setPosition(CCPoint t_sp)
	{
		CCPoint prevPosition = getPosition();
		Trace tr;
		tr.position = t_sp;
		tr.directionRad = atan2f(t_sp.y - prevPosition.y, t_sp.x - prevPosition.x);
		
//		KSCumberBase::setPosition(t_sp);
		m_headImg->setPosition(t_sp);
		m_cumberTrace.push_back(tr); //
		if(m_cumberTrace.size() >= 100)
		{
			m_cumberTrace.pop_front();
		}
		gameData->setMainCumberPoint(ccp2ip(t_sp));
		
		setHeadAndBodies();
		//		gameData->communication("Main_moveGamePosition", t_sp);
		//		gameData->communication("VS_setMoveGamePosition", t_sp);
		//		gameData->communication("Main_moveGamePosition", t_sp);
		//		gameData->communication("Main_moveGamePosition", t_sp);
	}
	virtual void setPositionX(float t_x)
	{
		setPosition(ccp(t_x, getPositionY()));
	}
	virtual void setPositionY(float t_y)
	{
		setPosition(ccp(getPositionX(), t_y));
	}
	
	virtual const CCPoint& getPosition()
	{
//		CCLog("snake position : %.2f, %.2f", m_headImg->getPositionX(), m_headImg->getPositionY());
		
		return m_headImg->getPosition();
	}
	virtual float getPositionX()
	{
		return m_headImg->getPositionX();
	}
	virtual float getPositionY()
	{
		return m_headImg->getPositionY();
	}
	
	COLLISION_CODE crashWithX(IntPoint check_position);
	COLLISION_CODE crashLooper(const vector<IntPoint> v, IntPoint* cp);
	const float RADIUS;
	void showEmotion(EmotionType t_type)
	{
		if(mEmotion)
			mEmotion->selfRemove();
		mEmotion = Emotion::create(t_type, this, callfunc_selector(SnakeCumber::nullmEmotion));
		mEmotion->setPosition(ccp(30,20));
		addChild(mEmotion);
	}
	void nullmEmotion()
	{
		mEmotion = NULL;
	}
	void setHeadAndBodies();
	virtual void startAttackReaction(float userdata);
protected:
	bool isGameover;
	float m_scale;
	float m_speed;
	CCSprite* m_headImg;
	vector<CCSprite*> m_Bodies;
	int m_directionAngleDegree;
	IntPoint getMapPoint(CCPoint c){
		return IntPoint(round((c.x - 1) / pixelSize + 1.f),
						round((c.y - 1) / pixelSize + 1.f));
	}
	
	Well512 m_well512;
	Emotion* mEmotion;
	
	deque< Trace > m_cumberTrace; // back 은 항상 머리를 가르킴.
};

#endif /* defined(__DGproto__SnakeCumber__) */
