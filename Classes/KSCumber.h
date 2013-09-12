//
//  KSCumber.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#ifndef __DGproto__KSCumber__
#define __DGproto__KSCumber__

#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <vector>
#include "CumberEmotion.h"


/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.
class KSCumber : public KSCumberBase
{
public:
	KSCumber() : m_speed(2.f), m_scale(1.f), RADIUS(22.f), mEmotion(nullptr){}
	virtual ~KSCumber(){}

	virtual void movingAndCrash(float dt);
	virtual void onStartMoving()
	{
		m_state = CUMBERSTATEMOVING;
		m_speed = 2.f;
		schedule(schedule_selector(KSCumberBase::movingAndCrash));
	}
	virtual void onStopMoving()
	{
		m_state = CUMBERSTATESTOP;
		m_speed = 0;
	}
	void attack(float dt);
	virtual bool init();
	CREATE_FUNC(KSCumber);
	virtual void setPosition(CCPoint t_sp)
	{
		KSCumberBase::setPosition(t_sp);
		gameData->setMainCumberPoint(ccp2ip(t_sp));
//		gameData->communication("Main_moveGamePosition", t_sp);
//		gameData->communication("VS_setMoveGamePosition", t_sp);
//		gameData->communication("Main_moveGamePosition", t_sp);
//		gameData->communication("Main_moveGamePosition", t_sp);
//		std::thread t1([](){;});
		
	}
	
	COLLISION_CODE crashWithX(IntPoint check_position);
	COLLISION_CODE crashLooper(const vector<IntPoint> v, IntPoint* cp);
	const float RADIUS;
	void showEmotion(EmotionType t_type)
	{
		if(mEmotion)
			mEmotion->selfRemove();
		mEmotion = Emotion::create(t_type, this, callfunc_selector(KSCumber::nullmEmotion));
		mEmotion->setPosition(ccp(30,20));
		addChild(mEmotion);
	}
	void nullmEmotion()
	{
		mEmotion = NULL;
	}
	void startDamageReaction(float userdata);
	virtual void startSpringCumber(float userdata){}
	virtual void startAnimationNoDirection()
	{
		
	}
	virtual void startAnimationDirection()
	{
		
	}
protected:
	bool isGameover;
	float m_scale;
	float m_speed;
	CCSprite* m_headImg;
	int m_directionAngleDegree;
	IntPoint getMapPoint(CCPoint c){
		return IntPoint(round((c.x - 1) / pixelSize + 1.f),
						round((c.y - 1) / pixelSize + 1.f));
	}
	
	Well512 m_well512;
	Emotion* mEmotion;
};

#endif /* defined(__DGproto__KSCumber__) */
