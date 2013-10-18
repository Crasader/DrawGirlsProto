//
//  Melon.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 25..
//
//

#ifndef __DGproto__Melon__
#define __DGproto__Melon__


#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <set>
//#include "CumberEmotion.h"
#include <deque>
#include "Jack.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;


/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.


class Melon : public KSCumberBase
{
public:
	Melon() :
	
	RADIUS(110.f / 4.f), // 머리에 대한 충돌 반지름
	BODY_RADIUS(70/4.f), // 몸통에 대한 충돌 반지름
	TAIL_RADIUS(50/4.f), // 꼬리에 대한 충돌 반지름
//	mEmotion(nullptr),
	LIMIT_COLLISION_PER_SEC(3), /// 초당 변수만큼 충돌시 스케일 줄임.
	ATTACK_POINT_X(-18), // 가운데 위치로부터 떨어진 공격포인턴데, 축소한 그림에서의 기준.
	ATTACK_POINT_Y(0),   // 가운데 위치로부터 떨어진 공격포인턴데, 축소한 그림에서의 기준.
	BODY_MARGIN(20),     // 몸통 사이의 거리.
	TAIL_MARGIN(40)      // 몸통과 꼬리사이의 거리.

	{
		m_state = (CUMBERSTATEMOVING);
	}
	
	virtual ~Melon();
	
	virtual void onStartMoving()
	{
		m_state = CUMBERSTATEMOVING;
	}
	virtual void onStopMoving()
	{
		m_state = CUMBERSTATESTOP;
		CCLog("stop!!");
	}
	virtual void onPatternEnd()
	{
		CCLog("onPatternEnd!!");
		m_noDirection.state = 2;
		if(m_direction.state == 1)
		{
			m_direction.state = 2; // 돌아가라고 상태 변경때림.
			m_state = CUMBERSTATEMOVING;
		}
		else if(m_state == CUMBERSTATEDIRECTION)
		{
			m_state = CUMBERSTATEMOVING;
		}
	}
	virtual void onStartGame()
	{
		m_noDirection.state = 2;
		
		
	}
	virtual void crashMapForPosition(CCPoint targetPt);
	//	virtual void movingAndCrash(float dt);


	virtual bool init();
	CREATE_FUNC(Melon);
	virtual void setPosition(const CCPoint& t_sp)
	{
		CCPoint prevPosition = getPosition();
		SnakeTrace tr;
		tr.position = t_sp;
		tr.directionRad = atan2f(t_sp.y - prevPosition.y, t_sp.x - prevPosition.x);
		
		//		KSCumberBase::setPosition(t_sp);
		m_headImg->setPosition(t_sp);
		m_cumberTrace.push_back(tr); //
		if(m_cumberTrace.size() >= 350)
		{
			m_cumberTrace.pop_front();
		}
		
		if(m_state != CUMBERSTATENODIRECTION)
		{
			myGD->setMainCumberPoint(ccp2ip(t_sp));
			m_mapPoint = ccp2ip(t_sp);
		}
		setHeadAndBodies();
		//		myGD->communication("Main_moveGamePosition", t_sp);
		//		myGD->communication("VS_setMoveGamePosition", t_sp);
		//		myGD->communication("Main_moveGamePosition", t_sp);
		//		myGD->communication("Main_moveGamePosition", t_sp);
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
	
//	void showEmotion(EmotionType t_type)
//	{
//		if(mEmotion)
//			mEmotion->selfRemove();
//		mEmotion = Emotion::create(t_type, this,
//								   callfunc_selector(Melon::nullmEmotion));
//		mEmotion->setPosition(ccpAdd(getPosition(), ccp(30,20)));
//		addChild(mEmotion, 11);
//	}
//	void nullmEmotion()
//	{
//		mEmotion = NULL;
//	}
	void setHeadAndBodies();
	virtual bool startDamageReaction(float damage, float angle);
	virtual void attackBehavior(Json::Value pattern)
	{
		if(pattern["pattern"].asString() == "109")
		{
			m_headAnimationManager->runAnimationsForSequenceNamed("cast101start");
			for(auto bodyAniManager : m_bodyAnimationManagers)
			{
				bodyAniManager->runAnimationsForSequenceNamed("cast101start");
			}
			m_tailAnimationManager->runAnimationsForSequenceNamed("cast101start");
		}
		else
		{
			m_headAnimationManager->runAnimationsForSequenceNamed("cast101start");
			for(auto bodyAniManager : m_bodyAnimationManagers)
			{
				bodyAniManager->runAnimationsForSequenceNamed("cast101start");
			}
			std::string target = pattern.get("target", "no").asString();
			if( target == "yes") // 타게팅이라면 조준하라
				startAnimationDirection();
			else if(target == "no") // 타게팅이 아니면 돌아라
				startAnimationNoDirection();
		}
	}
	virtual void startAnimationNoDirection();
	virtual void startAnimationDirection();
	//	virtual void startSpringCumber(float userdata)
	//	{
	//		startDamageReaction(userdata);
	//	}
	void damageReaction(float dt);
	void animationNoDirection(float dt);
	void animationDirection(float dt);
	
	
	void scaleAdjustment(float dt);
	CCPoint getMissilePoint()
	{
		float theta = deg2Rad(-m_headImg->getRotation());
		float x = ATTACK_POINT_X;
		float y = ATTACK_POINT_Y;
		return m_headImg->getPosition() +
		ccp(x*cos(theta) - y*sin(theta), x*sin(theta) + y * cos(theta));
	}
	
	virtual void startInvisible(int totalframe);
	void invisibling(float dt);
	
	virtual void furyModeOn(int tf);
	void furyModeScheduler(float dt);
	virtual void furyModeOff();
	
	virtual void getRandomPosition(IntPoint* ip, bool* finded);
	
	virtual void lightSmaller(){}
	
	virtual void endTeleport(){}
	virtual void startTeleport(){}
	virtual void smaller() {}
	virtual void onTargetingJack(CCPoint jackPosition)
	{
		CCPoint cumberPosition = getPosition();
		float deg = rad2Deg(atan2(jackPosition.y - cumberPosition.y, jackPosition.x - cumberPosition.x));
		m_headImg->setRotation(-deg);
	}
	virtual void stopAnimationNoDirection()
	{
		m_noDirection.state = 2;
	}
	virtual void stopAnimationDirection()
	{
		m_direction.state = 2;
	}
	virtual COLLISION_CODE getCrashCode(IntPoint point, IntPoint* checkPosition){
		float half_distance = RADIUS*getCumberScale(); // 20.f : radius for base scale 1.f
		int ip_half_distance = half_distance / 2;
		IntPoint afterPoint = point;
		
		set<IntPoint> ips;
		
		for(int i=afterPoint.x-ip_half_distance;i<=afterPoint.x+ip_half_distance;i++)
		{
			for(int j=afterPoint.y-ip_half_distance;j<=afterPoint.y+ip_half_distance;j++)
			{
				float calc_distance = sqrtf(powf((afterPoint.x - i)*1,2) + powf((afterPoint.y - j)*1, 2));
				if(calc_distance < ip_half_distance)
				{
					ips.insert(IntPoint(i, j));
				}
			}
		}
		
		COLLISION_CODE collisionCode = crashLooper(ips, checkPosition);
		
		if(collisionCode == kCOLLISION_NONE)
		{
			// 몸통에 대한 충돌처리 ver2 : 잭과의 거리만 측정해서 계산함.
			if(myGD->getJackState() != jackStateNormal)
			{
				for(auto body : m_Bodies)
				{
					CCPoint cumberPosition = body->getPosition();
					CCPoint bodyPosition = cumberPosition;
					IntPoint afterPoint = ccp2ip(bodyPosition);
					IntPoint checkPosition;
					float half_distance = BODY_RADIUS*getCumberScale(); // 20.f : radius for base scale 1.f
					int ip_half_distance = half_distance / 2;
					
					
					IntPoint jackPoint = myGD->getJackPoint();
					float calc_distance = sqrtf(powf((afterPoint.x - jackPoint.x)*1,2) + powf((afterPoint.y - jackPoint.y)*1, 2));
					if(calc_distance < ip_half_distance)
					{
						collisionCode = COLLISION_CODE::kCOLLISION_JACK;
						break;
					}
				}
			}
			
			// 꼬리에 대한 충돌처리 ver2 : 잭과의 거리만 측정해서 계산함.
			if(myGD->getJackState() != jackStateNormal)
			{
				CCPoint cumberPosition = m_tailImg->getPosition();
				CCPoint bodyPosition = cumberPosition;
				IntPoint afterPoint = ccp2ip(bodyPosition);
				IntPoint checkPosition;
				float half_distance = TAIL_RADIUS*getCumberScale(); // 20.f : radius for base scale 1.f
				int ip_half_distance = half_distance / 2;
				
				
				IntPoint jackPoint = myGD->getJackPoint();
				float calc_distance = sqrtf(powf((afterPoint.x - jackPoint.x)*1,2) + powf((afterPoint.y - jackPoint.y)*1, 2));
				if(calc_distance < ip_half_distance)
				{
					collisionCode = COLLISION_CODE::kCOLLISION_JACK;
				}
			}
		}
		
		return collisionCode;
	}
protected:

	const float RADIUS;
	const float BODY_RADIUS;
	const float TAIL_RADIUS;
	const float BODY_MARGIN;
	const float TAIL_MARGIN;
	const int LIMIT_COLLISION_PER_SEC; /// 초당 변수만큼 충돌시 스케일 줄임.
	const float ATTACK_POINT_X;
	const float ATTACK_POINT_Y;
	bool isGameover;
	CCBAnimationManager* m_headAnimationManager;
	vector<CCBAnimationManager*> m_bodyAnimationManagers;
	CCBAnimationManager* m_tailAnimationManager;
	
	CCSprite* m_headImg;
	vector<CCSprite*> m_Bodies;
	CCSprite* m_tailImg;
	IntPoint getMapPoint(CCPoint c){
		return IntPoint(round((c.x - 1) / pixelSize + 1.f),
						round((c.y - 1) / pixelSize + 1.f));
	}
	
	
//	Emotion* mEmotion;
	
	deque< SnakeTrace > m_cumberTrace; // back 은 항상 머리를 가르킴.
	
	struct DamageData
	{
		float m_damageX;
		float m_damageY;
		float timer;
	}m_damageData;
	
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
	
	/// 지향형 에니메이션 용.
	struct Direction
	{
		float lastDirectionDeg;
		int state; // 1 : 바라보는 상태, 2 : 다시 제 방향 보는 상태.
		float timer;
		void initVars(){
			state = 1;
			lastDirectionDeg = 0.f;
			timer = 0.f;
		}
	}m_direction;
	

};



#endif /* defined(__DGproto__Melon__) */
