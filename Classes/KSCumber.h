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
	KSCumber() : m_speed(2.f), m_scale(1.f), RADIUS(22.f), mEmotion(nullptr){
		m_state = (CUMBERSTATEMOVING);
	}
	virtual ~KSCumber(){}

	virtual void movingAndCrash(float dt);
	virtual void onStartMoving()
	{
		m_state = CUMBERSTATEMOVING;
		schedule(schedule_selector(KSCumberBase::movingAndCrash));
	}
	virtual void onStopMoving()
	{
		m_state = CUMBERSTATESTOP;
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
		CCLog("Lets rotate");
		if(m_state != CUMBERSTATENODIRECTION)
		{
			m_state = CUMBERSTATENODIRECTION;
			m_noDirection.distance = 0;
			m_noDirection.rotationDeg = 0;
			m_noDirection.timer = 0;
			m_noDirection.startingPoint = getPosition();
			m_noDirection.rotationCnt = 0;
			m_noDirection.state = 1;
			schedule(schedule_selector(KSCumber::animationNoDirection));
		}
	}
	void damageReaction(float dt);
	void animationNoDirection(float dt);
	virtual void startAnimationDirection(){}
	CCPoint getMissilePoint()
	{
		return getPosition() + ccp(0, 0);
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
	/// 방사형 에니메이션 용.
	struct NoDirection
	{
		float rotationDeg;
		float distance;
		float timer;
		CCPoint startingPoint;
		int rotationCnt;
		int state; // 1 : 도는상태, 2 : 다시 제 위치로 돌아가는 상태
	}m_noDirection;
	
	struct DamageData
	{
		float m_damageX;
		float m_damageY;
		float timer;
	}m_damageData;
};

#endif /* defined(__DGproto__KSCumber__) */
