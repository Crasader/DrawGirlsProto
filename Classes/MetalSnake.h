//
//  MetalSnake.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 12..
//
//

#ifndef __DGproto__MetalSnake__
#define __DGproto__MetalSnake__


#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <set>
#include "CumberEmotion.h"
#include <deque>
#include "FromTo.h"

struct MetalSnakeTrace
{
	CCPoint position; // 자취의 위치와
	float directionRad; // 자취의 방향.
};
/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.


class MetalSnake : public KSCumberBase
{
public:
	MetalSnake() :
	m_speed(2.f),
	RADIUS(90.f / 4.f), // 머리에 대한 충돌 반지름
	BODY_RADIUS(70/4.f), // 몸통에 대한 충돌 반지름
	TAIL_RADIUS(50/4.f), // 꼬리에 대한 충돌 반지름
	mEmotion(nullptr),
	LIMIT_COLLISION_PER_SEC(3), /// 초당 변수만큼 충돌시 스케일 줄임.
	ATTACK_POINT_X(-18), // 가운데 위치로부터 떨어진 공격포인턴데, 축소한 그림에서의 기준.
	ATTACK_POINT_Y(0),   // 가운데 위치로부터 떨어진 공격포인턴데, 축소한 그림에서의 기준.
	BODY_MARGIN(20),     // 몸통 사이의 거리.
	TAIL_MARGIN(40)      // 몸통과 꼬리사이의 거리.
	{
		m_state = (CUMBERSTATEMOVING);
	}
	
	virtual ~MetalSnake();
	
	virtual void onStartMoving()
	{
		m_state = CUMBERSTATEMOVING;
	}
	virtual void onStopMoving()
	{
		m_state = CUMBERSTATESTOP;
		CCLog("stop!!");
	}
	virtual void movingAndCrash(float dt);
	void attack(float dt);
	virtual bool init();
	CREATE_FUNC(MetalSnake);
	virtual void setPosition(CCPoint t_sp)
	{
		CCPoint prevPosition = getPosition();
		MetalSnakeTrace tr;
		tr.position = t_sp;
		tr.directionRad = atan2f(t_sp.y - prevPosition.y, t_sp.x - prevPosition.x);
		
		//		KSCumberBase::setPosition(t_sp);
		m_headImg->setPosition(t_sp);
		m_cumberTrace.push_back(tr); //
		if(m_cumberTrace.size() >= 200)
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
	
	void showEmotion(EmotionType t_type)
	{
		if(mEmotion)
			mEmotion->selfRemove();
		mEmotion = Emotion::create(t_type, this, callfunc_selector(MetalSnake::nullmEmotion));
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
		m_scale.scale.init(m_scale.scale.getValue(), r, 0.005f);
	}
	float getCumberScale()
	{
		return m_scale.scale.getValue();
	}
	void scaleAdjustment(float dt);
	CCPoint getMissilePoint()
	{
		float theta = deg2Rad(-m_headImg->getRotation());
		float x = ATTACK_POINT_X;
		float y = ATTACK_POINT_Y;
		return m_headImg->getPosition() +
		ccp(x*cos(theta) - y*sin(theta), x*sin(theta) + y * cos(theta));
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
	float m_speed;
	CCSprite* m_headImg;
	vector<CCSprite*> m_Bodies;
	CCSprite* m_tailImg;
	int m_directionAngleDegree;
	IntPoint getMapPoint(CCPoint c){
		return IntPoint(round((c.x - 1) / pixelSize + 1.f),
						round((c.y - 1) / pixelSize + 1.f));
	}
	
	Well512 m_well512;
	Emotion* mEmotion;
	
	deque< MetalSnakeTrace > m_cumberTrace; // back 은 항상 머리를 가르킴.
	
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
	}m_direction;
	
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
	}m_scale;
	

};


#endif /* defined(__DGproto__MetalSnake__) */
