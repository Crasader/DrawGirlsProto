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
#include <set>
#include "CumberEmotion.h"
#include <deque>
#include "FromTo.h"

struct Trace
{
	CCPoint position; // 자취의 위치와
	float directionRad; // 자취의 방향.
};
/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.


class SnakeCumber : public KSCumberBase
{
public:
	SnakeCumber() :
	m_speed(2.f), RADIUS(15.f), mEmotion(nullptr),
		LIMIT_COLLISION_PER_SEC(3)
		
	{
		m_state = (CUMBERSTATEMOVING);
	}
	
	virtual ~SnakeCumber();
	
	virtual void onStartMoving()
	{
		// 방사형으로 돌아가고 있는 중이라면 
		if(m_state == CUMBERSTATENODIRECTION)
		{
//			noDirection.state = 2; // 돌아가라고 상태 변경때림.
		}
		else if(m_state == CUMBERSTATESTOP)
		{
			m_state = CUMBERSTATEMOVING;
		}
	}
	virtual void onStopMoving()
	{
		m_state = CUMBERSTATESTOP;
		CCLog("stop!!");
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
		
		if(m_state != CUMBERSTATENODIRECTION)
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
	COLLISION_CODE crashLooper(const set<IntPoint>& v, IntPoint* cp);
	const float RADIUS;
	void showEmotion(EmotionType t_type)
	{
		if(mEmotion)
			mEmotion->selfRemove();
		mEmotion = Emotion::create(t_type, this, callfunc_selector(SnakeCumber::nullmEmotion));
		mEmotion->setPosition(ccpAdd(getPosition(), ccp(30,20)));
		addChild(mEmotion, 11);
	}
	void nullmEmotion()
	{
		mEmotion = NULL;
	}
	void setHeadAndBodies();
	virtual void startDamageReaction(float userdata);
	virtual void startAnimationNoDirection();
	virtual void startAnimationDirection();
//	virtual void startSpringCumber(float userdata)
//	{
//		startDamageReaction(userdata);
//	}
	void damageReaction(float dt);
	void animationNoDirection(float dt);
	void setCumberScale(float r)
	{
		scale.scale.init(scale.scale.getValue(), r, 0.005f);
	}
	float getCumberScale()
	{
		return scale.scale.getValue();
	}
	void scaleAdjustment(float dt);
protected:
	const int LIMIT_COLLISION_PER_SEC; /// 초당 변수만큼 충돌시 스케일 줄임.
	bool isGameover;
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
	
	struct DamageData
	{
		float m_damageX;
		float m_damageY;
		float timer;
	}damageData;
	
	/// 방사형 에니메이션 용.
	struct NoDirection
	{
		float rotationDeg;
		float distance;
		float timer;
		CCPoint startingPoint;
		int rotationCnt;
		int state; // 1 : 도는상태, 2 : 다시 제 위치로 돌아가는 상태
	}noDirection;
	
	/// 지향형 에니메이션 용.
	struct Direction
	{
	}direction;
	
	struct Scale
	{
		Scale() : SCALE_ADDER(0.1f), SCALE_SUBER(0.1f), scale(1.f, 1.f, 0.f),
			timer(0), autoIncreaseTimer(0), collisionStartTime(0), increaseTime(0),
		collisionCount(0){}
		const float SCALE_ADDER;
		const float SCALE_SUBER;
		int collisionCount; // 초당 충돌횟수 세기 위해
		float collisionStartTime;
		float timer; //
		
		float increaseTime;
		float autoIncreaseTimer;
		FromTo<float> scale; // 서서히 변하는것을 구현하기 위해.
	}scale;
	
};

#endif /* defined(__DGproto__SnakeCumber__) */
