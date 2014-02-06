//
//  KSSnakeBase.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 12..
//
//

#include "KSSnakeBase.h"

#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
//#include "CumberEmotion.h"
#include "Jack.h"
#include "RandomSelector.h"
#include "StageImgLoader.h"


KSSnakeBase::~KSSnakeBase()
{
	CCLog("~KSSnakeBase");
}



bool KSSnakeBase::init(const string& ccbiFile, bool isNotShowWindow)
{
	if(isNotShowWindow)
		KSCumberBase::init();
	else
		CCNode::init();
	
	m_directionAngleDegree = m_well512.GetValue(0, 360);
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	{
		CCBReader* reader = new CCBReader(nodeLoader);
//		auto fileName = "boss_"+ccbiFile+"_"+"head.ccbi";
		auto fileName = ccbiFile+"_"+"head.ccbi";
//		m_headImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile(fileName.c_str(),this));
		m_headImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+fileName).c_str(), this));
		m_headAnimationManager = reader->getAnimationManager();
		this->addChild(m_headImg, 10);
		reader->release();
	}
	int lastZ=-1;
	{
		
		for(int i=0; i<7; i++)
		{
			CCBReader* reader = new CCBReader(nodeLoader);
//			auto fileName = "boss_"+ccbiFile+"_"+"body.ccbi";
			auto fileName = ccbiFile+"_"+"body.ccbi";
//			CCSprite* body = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile(fileName.c_str(),this));
			CCSprite* body = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+fileName).c_str(), this));
			m_bodyAnimationManagers.push_back(reader->getAnimationManager());
			addChild(body, 9 - i);
			lastZ = 9 - i;
			m_Bodies.push_back(body);
			reader->release();
		}
	}
	
	
	{
		CCBReader* reader = new CCBReader(nodeLoader);
//		auto fileName = "boss_"+ccbiFile+"_"+"tail.ccbi";
		auto fileName = ccbiFile+"_"+"tail.ccbi";
//		m_tailImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile(fileName.c_str(),this));
		m_tailImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+fileName).c_str(), this));
		m_tailAnimationManager = reader->getAnimationManager();
		this->addChild(m_tailImg, lastZ - 1);
		reader->release();
	}
	
	
	
	
	
	//	startMoving();
	if(isNotShowWindow)
	{
		schedule(schedule_selector(KSSnakeBase::scaleAdjustment), 1/60.f);
		schedule(schedule_selector(KSCumberBase::movingAndCrash));
	}
	
	
	return true;
}

void KSSnakeBase::setHeadAndBodies()
{
	SnakeTrace lastTrace = m_cumberTrace.back();
//	float tt = rad2Deg( lastTrace.directionRad );
	//	CCLog("deg %f", tt);
	//	m_headImg->setVisible(false);
	m_headImg->setRotation(-rad2Deg( lastTrace.directionRad ));
	
	int lastTraceIndex = m_cumberTrace.size() - 1; // to 0
	//	int bodyIndex = 0;
	//	for(auto i : m_cumberTrace)
	for(int bodyIndex = 0; bodyIndex < m_Bodies.size(); ++bodyIndex)
	{
		// 순서대로 머리에 가까운 몸통처리.
		float distance = 0;
		for(int traceIndex = lastTraceIndex - 1; traceIndex >= 0; traceIndex--)
		{
			SnakeTrace t = m_cumberTrace[traceIndex];
			// t 와 tr 의 거리차이.
			//			float distance = ccpLength(lastTrace.position - t.position);
			//			int distance = lastTraceIndex - traceIndex;
			
			// traceIndex + 1 위치와 traceIndex 위치의 누적
			distance += ccpLength(m_cumberTrace[traceIndex + 1].position - t.position);
			if(distance >= BODY_MARGIN * getCumberScale())
			{
				lastTraceIndex = traceIndex;
				lastTrace = t;
				break;
			}
		}
		m_Bodies[bodyIndex]->setRotation(-rad2Deg(lastTrace.directionRad));
		m_Bodies[bodyIndex]->setPosition(lastTrace.position);
	}
	{
		float distance = 0;
		for(int traceIndex = lastTraceIndex - 1; traceIndex >= 0; traceIndex--)
		{
			SnakeTrace t = m_cumberTrace[traceIndex];
			// t 와 tr 의 거리차이.
			//			float distance = ccpLength(lastTrace.position - t.position);
			//			int distance = lastTraceIndex - traceIndex;
			
			// traceIndex + 1 위치와 traceIndex 위치의 누적
			distance += ccpLength(m_cumberTrace[traceIndex + 1].position - t.position);
			if(distance >= TAIL_MARGIN * getCumberScale())
			{
				lastTraceIndex = traceIndex;
				lastTrace = t;
				break;
			}
		}
		m_tailImg->setRotation(-rad2Deg(lastTrace.directionRad));
		m_tailImg->setPosition(lastTrace.position);
		//		m_tailImg->setScale(3.f);
	}
	
	//	m_headImg->setScale(tt / 360);
	
}

void KSSnakeBase::startAnimationNoDirection()
{
	// 돌자...
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
		schedule(schedule_selector(KSSnakeBase::animationNoDirection));
	}
}

void KSSnakeBase::animationNoDirection(float dt)
{
	//	CCLog("animationNoDirection");
	m_noDirection.timer += 1.f/60.f;
	
	if(m_noDirection.state == 1)
	{
		m_noDirection.rotationDeg += 6.f;
		if(m_noDirection.rotationDeg >= 360)
		{
			m_noDirection.rotationDeg -= 360;
			m_noDirection.rotationCnt++;
			//			/// 좀 돌았으면 돌아감.
			//			if(m_noDirection.rotationCnt >= 5)
			//			{
			//				m_noDirection.state = 2;
			//				return;
			//			}
		}
		m_noDirection.distance += 0.5f;
		m_noDirection.distance = MIN(m_noDirection.distance, 30);
		float dx = cos(deg2Rad(m_noDirection.rotationDeg)) * m_noDirection.distance * getCumberScale();
		float dy = sin(deg2Rad(m_noDirection.rotationDeg)) * m_noDirection.distance * getCumberScale() * 1.2f; // 약간 타원
		
		//	float speed = 2.f;
		//	dx *= speed;
		//	dy *= speed;
		
		setPosition(m_noDirection.startingPoint + ccp(dx, dy));
	}
	else if(m_noDirection.state == 2)
	{
		CCPoint dir = m_noDirection.startingPoint - getPosition();
		float rad = atan2(dir.y, dir.x);
		float dx = cos(rad);
		float dy = sin(rad);
		
		
		if(ccpLength(m_noDirection.startingPoint - getPosition()) <= 0.5f)
		{
			m_state = CUMBERSTATEMOVING;
			m_noDirection.state = 0;
			unschedule(schedule_selector(KSSnakeBase::animationNoDirection));
			setPosition(m_noDirection.startingPoint);
			m_headAnimationManager->runAnimationsForSequenceNamed("cast101stop");
			m_tailAnimationManager->runAnimationsForSequenceNamed("cast101stop");
		}
		else
			setPosition(getPosition() + ccp(dx, dy));
	}
}



void KSSnakeBase::startAnimationDirection()
{
	// 잭을 바라보자.
	m_state = CUMBERSTATEDIRECTION;
	m_direction.initVars();
	schedule(schedule_selector(KSSnakeBase::animationDirection));
}

void KSSnakeBase::animationDirection(float dt)
{
	m_direction.timer += 1 / 60.f;
	if(m_direction.state == 1)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		IntPoint headPoint = ccp2ip(getPosition());
		float rot = rad2Deg(atan2(jackPoint.x - headPoint.x, jackPoint.y - headPoint.y));
		rot -= 90;
		m_headImg->setRotation(rot);
	}
	else if(m_direction.state == 2)
	{
		//		m_state = CUMBERSTATEMOVING; //#!
		m_direction.state = 0;
		unschedule(schedule_selector(KSSnakeBase::animationDirection));
		m_headAnimationManager->runAnimationsForSequenceNamed("cast101stop");
		m_tailAnimationManager->runAnimationsForSequenceNamed("cast101stop");
	}
}
bool KSSnakeBase::startDamageReaction(float damage, float angle)
{
	KSCumberBase::startDamageReaction(damage, angle);
	m_remainHp -= damage;
	myGD->communication("UI_subBossLife", damage); //## 보스쪽에서 이걸 호출
	CCLog("remain hp %f", m_remainHp);
	m_invisible.invisibleFrame = m_invisible.VISIBLE_FRAME; // 인비지블 풀어주는 쪽으로 유도.
	
	setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER)); // 맞으면 작게 함.
	
	
	// 방사형으로 돌아가고 있는 중이라면
	if(m_state == CUMBERSTATENODIRECTION)
	{
		CCLog("m_state == CUMBERSTATENODIRECTION");
		m_noDirection.state = 2; // 돌아가라고 상태 변경때림.
		
	}
	else if(m_state == CUMBERSTATEDIRECTION)
	{
		CCLog("m_state == CUMBERSTATEDIRECTION");
		m_direction.state = 2; // 돌아가라고 상태 변경때림.
		m_state = CUMBERSTATEMOVING; //#!
		
	}
	else if(m_state == CUMBERSTATEMOVING)
	{
		CCLog("m_state == CUMBERSTATEMOVING");
		float rad = deg2Rad(angle);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLog("%f %f", dx, dy);
		m_state = CUMBERSTATEDAMAGING;
		
		m_damageData.timer = 0;
		schedule(schedule_selector(KSSnakeBase::damageReaction));
	}
	else if(m_state == CUMBERSTATESTOP)
	{
		CCLog("m_state == CUMBERSTATESTOP");
		float rad = deg2Rad(angle);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLog("%f %f", dx, dy);
		m_state = CUMBERSTATEDAMAGING;
		
		m_damageData.timer = 0;
		schedule(schedule_selector(KSSnakeBase::damageReaction));
	}
	else if(m_state == CUMBERSTATEFURY)
	{
		CCLog("m_state == CUMBERSTATEMOVING");
		float rad = deg2Rad(angle);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLog("%f %f", dx, dy);
		m_state = CUMBERSTATEDAMAGING;
		
		m_damageData.timer = 0;
		schedule(schedule_selector(KSSnakeBase::damageReaction));
		crashMapForPosition(getPosition());
		myGD->communication("MS_resetRects", false);
	}
	
	if(m_remainHp <= 0)
	{
		
		return true;
	}
	else
	{
		return false;
	}
}

void KSSnakeBase::damageReaction(float)
{
	m_damageData.timer += 1 / 60.f;
	if(m_damageData.timer < 1)
	{
		m_headImg->setColor(ccc3(255, 0, 0));
		m_tailImg->setColor(ccc3(255, 0, 0));
		for(auto i : m_Bodies)
		{
			i->setColor(ccc3(255, 0, 0));
		}
	}
	else
	{
		m_headImg->setColor(ccc3(255, 255, 255));
		m_tailImg->setColor(ccc3(255, 255, 255));
		for(auto i : m_Bodies)
		{
			i->setColor(ccc3(255, 255, 255));
		}
		m_state = CUMBERSTATEMOVING;
		unschedule(schedule_selector(KSSnakeBase::damageReaction));
		m_headAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
		m_tailAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
		m_furyMode.furyFrameCount = m_furyMode.totalFrame;
	}
}
void KSSnakeBase::startInvisible(int totalframe)
{
	//	if(!isScheduled(schedule_selector(KSCumber::invisibling)))
	if(m_invisible.startInvisibleScheduler == false)
	{
		m_invisible.VISIBLE_FRAME = totalframe;
		m_invisible.invisibleFrame = 0;
		m_invisible.invisibleValue = 0;
		//		m_headImg->stopAllActions();
		schedule(schedule_selector(KSSnakeBase::invisibling));
		m_invisible.startInvisibleScheduler = true;
	}
}

void KSSnakeBase::invisibling(float dt)
{
	m_invisible.invisibleFrame++;
	
	if(m_invisible.invisibleFrame < m_invisible.VISIBLE_FRAME)
	{
		KS::setOpacity(m_headImg, MAX(0, 255 - m_invisible.invisibleFrame*5));
		for(auto body : m_Bodies)
		{
			KS::setOpacity(body, MAX(0, 255 - m_invisible.invisibleFrame*5));
		}
		KS::setOpacity(m_tailImg, MAX(0, 255 - m_invisible.invisibleFrame*5));
	}
	else
	{
		// 최소 1 최대 255
		
		m_invisible.invisibleValue = MIN(255, MAX(1, m_invisible.invisibleValue * 1.2f));
		
		KS::setOpacity(m_headImg, m_invisible.invisibleValue);
		for(auto body : m_Bodies)
		{
			KS::setOpacity(body, m_invisible.invisibleValue);
		}
		KS::setOpacity(m_tailImg, m_invisible.invisibleValue);
		if(m_invisible.invisibleValue == 255)
		{
			m_invisible.startInvisibleScheduler = false;
			unschedule(schedule_selector(ThisClassType::invisibling));
		}
	}
	
}

void KSSnakeBase::scaleAdjustment(float dt)
{
	m_scale.autoIncreaseTimer += 1/60.f;
	
	if(m_scale.increaseTime + 2.f < m_scale.autoIncreaseTimer && m_state != CUMBERSTATENODIRECTION)
	{
		CCLog("upSize!");
		m_scale.increaseTime = m_scale.autoIncreaseTimer;
		setCumberScale(MIN(m_maxScale, getCumberScale() + m_scale.SCALE_ADDER));
	}
	
	m_scale.scale.step();
	
	m_headImg->setScale(getCumberScale());
	m_tailImg->setScale(getCumberScale());
	for(auto i : m_Bodies)
	{
		i->setScale(getCumberScale());
	}
	
}



COLLISION_CODE KSSnakeBase::crashLooper(const set<IntPoint>& v, IntPoint* cp)
{
	for(const auto& i : v)
	{
		auto result = crashWithX(i);
		if(result != kCOLLISION_NONE)
		{
			if(cp)
				*cp = i;
			return result;
		}
	}
	return kCOLLISION_NONE;
}

void KSSnakeBase::furyModeOn(int tf)
{
	m_furyMode.startFury(tf);
	m_state = CUMBERSTATEFURY;
	
	m_headImg->setColor(ccc3(0, 255, 0));
	m_tailImg->setColor(ccc3(0, 255, 0));
	for(auto i : m_Bodies)
	{
		i->setColor(ccc3(0, 255, 0));
	}
	
	schedule(schedule_selector(ThisClassType::furyModeScheduler));
}


void KSSnakeBase::crashMapForPosition(CCPoint targetPt)
{
	CCPoint afterPosition = targetPt;
	IntPoint afterPoint = ccp2ip(afterPosition);
	set<IntPoint> crashArea;
	float half_distance = RADIUS*getCumberScale() * 1.2f; // 깎을 영역은 충돌 영역크기보다 1.2 배.
	int ip_half_distance = half_distance / 2;
	// 충돌 영역에 대한 포인트 추가.
	for(int i=afterPoint.x-ip_half_distance;i<=afterPoint.x+ip_half_distance;i++)
	{
		for(int j=afterPoint.y-ip_half_distance;j<=afterPoint.y+ip_half_distance;j++)
		{
			crashArea.insert(IntPoint(i, j));
#if 0 // 원 형태로 부숨.
			float calc_distance = sqrtf(powf((afterPoint.x - i)*1,2) + powf((afterPoint.y - j)*1, 2));
			if(calc_distance < ip_half_distance)
			{
				if(i >= mapLoopRange::mapWidthInnerBegin && i < mapLoopRange::mapWidthInnerEnd &&
				   j >= mapLoopRange::mapHeightInnerBegin && j < mapLoopRange::mapHeightInnerEnd )
					crashArea.insert(IntPoint(i, j));
			}
#endif
		}
	}
	for(auto& i : crashArea)
	{
		crashMapForIntPoint(i);
	}
	
}
void KSSnakeBase::furyModeScheduler(float dt)
{
	//if(m_furyMode.furyFrameCount >= m_furyMode.totalFrame)
	//{
		//crashMapForPosition(getPosition());
		
		//m_state = CUMBERSTATEMOVING;
		//m_headImg->setColor(ccc3(255, 255, 255));
		//m_tailImg->setColor(ccc3(255, 255, 255));
		//for(auto i : m_Bodies)
		//{
			//i->setColor(ccc3(255, 255, 255));
		//}
		//myGD->communication("MS_resetRects", false);
		//unschedule(schedule_selector(ThisClassType::furyModeScheduler));
	//}
	if(m_furyMode.furyFrameCount >= m_furyMode.totalFrame)
	{
		// 시간이 다되서 끝나는 조건.
		crashMapForPosition(getPosition());

		m_state = CUMBERSTATEMOVING;
		//		m_headImg->setColor(ccc3(255, 255, 255));
		myGD->communication("MS_resetRects", false);
		unschedule(schedule_selector(ThisClassType::furyModeScheduler));
		// 다시 벌겋게 만드는 코드.

		addChild(KSGradualValue<float>::create(m_furyMode.colorRef, 255, 0.5f,
					[=](float t)
					{
					KS::setColor(this, ccc3(255, t, t));
					}));
	}
	else
	{
		int c = clampf(m_furyMode.colorRef, 0, 255);
		KS::setColor(this, ccc3(255, c, c)); 
		if(m_furyMode.colorDir > 0)
		{
			m_furyMode.colorRef += 4;
			if(m_furyMode.colorRef >= 256)
			{
				m_furyMode.colorDir *= -1;
			}
		}
		else
		{
			m_furyMode.colorRef -= 4;
			if(m_furyMode.colorRef < 0)
			{
				m_furyMode.colorDir *= -1;
			}
		}
	}
}
void KSSnakeBase::furyModeOff()
{
	//##
	//	if(isFuryMode)
	//	{
	//		myGD->communication("EP_stopCrashAction");
	//		myGD->communication("MS_resetRects", false);
	//		isFuryMode = false;
	//		furyMode->removeFromParentAndCleanup(true);
	//	}
}

void KSSnakeBase::onStartMoving()
{
	m_state = CUMBERSTATEMOVING;
}

void KSSnakeBase::onStopMoving()
{
	m_state = CUMBERSTATESTOP;
	CCLog("stop!!");
}

void KSSnakeBase::onPatternEnd()
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

void KSSnakeBase::onStartGame()
{
	KSCumberBase::onStartGame();
	m_noDirection.state = 2;
}

void KSSnakeBase::stopCasting()
{
	myGD->communication("MP_bombCumber", this);
	// 방사형으로 돌아가고 있는 중이라면
	if(m_state == CUMBERSTATENODIRECTION)
	{
		CCLog("m_state == CUMBERSTATENODIRECTION");
		m_noDirection.state = 2; // 돌아가라고 상태 변경때림.

	}
	else if(m_state == CUMBERSTATEDIRECTION)
	{
		CCLog("m_state == CUMBERSTATEDIRECTION");
		m_direction.state = 2; // 돌아가라고 상태 변경때림.
		m_state = CUMBERSTATEMOVING; //#!

	}
}

void KSSnakeBase::setPosition( const CCPoint& t_sp )
{
	CCPoint prevPosition = getPosition();
	if(isnan(prevPosition.x))
	{
		CCLog("hg!!!!");
	}
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

	if(m_state != CUMBERSTATENODIRECTION) // 도는것이 아니라면
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

void KSSnakeBase::setPositionX( float t_x )
{
	setPosition(ccp(t_x, getPositionY()));
}

void KSSnakeBase::setPositionY( float t_y )
{
	setPosition(ccp(getPositionX(), t_y));
}

const CCPoint& KSSnakeBase::getPosition()
{
	//		CCLog("snake position : %.2f, %.2f", m_headImg->getPositionX(), m_headImg->getPositionY());

	return m_headImg->getPosition();
}

float KSSnakeBase::getPositionX()
{
	return m_headImg->getPositionX();
}

float KSSnakeBase::getPositionY()
{
	return m_headImg->getPositionY();
}

void KSSnakeBase::attackBehavior( Json::Value pattern )
{
	if(pattern["pattern"].asString() == "109")
	{
		m_state = CUMBERSTATESTOP;
	}
	else if( pattern["pattern"].asString() == "1007")
	{
		m_state = CUMBERSTATESTOP;
	}
	else
	{
		m_headAnimationManager->runAnimationsForSequenceNamed("cast101start");
		m_tailAnimationManager->runAnimationsForSequenceNamed("cast101start");
		std::string target = pattern.get("target", "no").asString();
		if( target == "yes") // 타게팅이라면 조준하라
			startAnimationDirection();
		else if(target == "no") // 타게팅이 아니면 돌아라
			startAnimationNoDirection();
	}
}

void KSSnakeBase::stopAnimationNoDirection()
{
	m_noDirection.state = 2;
}

void KSSnakeBase::stopAnimationDirection()
{
	m_direction.state = 2;
}

CCPoint KSSnakeBase::getMissilePoint()
{
	float theta = deg2Rad(-m_headImg->getRotation());
	float x = ATTACK_POINT_X;
	float y = ATTACK_POINT_Y;
	return m_headImg->getPosition() +
		ccp(x*cos(theta) - y*sin(theta), x*sin(theta) + y * cos(theta));
}

void KSSnakeBase::randomPosition()
{
	IntPoint mapPoint;
	bool finded;
	getRandomPositionToJack(&mapPoint, &finded);

	//	myGD->setMainCumberPoint(mapPoint);
	for(int i=0; i<350; i++)
	{
		setPosition(ip2ccp(mapPoint));
	}
	//		setPosition(ip2ccp(mapPoint));

	m_circle.setRelocation(getPosition(), m_well512);
	m_snake.setRelocation(getPosition(), m_well512);
	{
		lightSmaller();
	}
}

void KSSnakeBase::onTargetingJack( CCPoint jackPosition )
{
	CCPoint cumberPosition = getPosition();
	float deg = rad2Deg(atan2(jackPosition.y - cumberPosition.y, jackPosition.x - cumberPosition.x));
	m_headImg->setRotation(-deg);
}

COLLISION_CODE KSSnakeBase::getCrashCode( IntPoint point, IntPoint* checkPosition )
{
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

float KSSnakeBase::getRadius()
{
	return RADIUS;
}

IntPoint KSSnakeBase::getMapPoint( CCPoint c )
{
	return IntPoint(round((c.x - 1) / pixelSize + 1.f),
		round((c.y - 1) / pixelSize + 1.f));
}
























































