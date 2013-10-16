//
//  Melon.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 25..
//
//

#include "Melon.h"
#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
//#include "CumberEmotion.h"
#include "Jack.h"
#include "RandomSelector.h"
Melon::~Melon()
{
	
}



bool Melon::init()
{
	KSCumberBase::init();
	
	m_directionAngleDegree = m_well512.GetValue(0, 360);

	
    CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	{
		CCBReader* reader = new CCBReader(nodeLoader);
		m_headImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("boss_melon_head.ccbi",this));
		m_headAnimationManager = reader->getAnimationManager();
		this->addChild(m_headImg, 10);
		reader->release();
    }
	int lastZ=-1;
	{
		
		for(int i=0; i<7; i++)
		{
			CCBReader* reader = new CCBReader(nodeLoader);
			CCSprite* body = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("boss_melon_body.ccbi",this));
			m_bodyAnimationManagers.push_back(reader->getAnimationManager());
			addChild(body, 9 - i);
			lastZ = 9 - i;
			m_Bodies.push_back(body);
			reader->release();
		}
	}
	
	
	{
		CCBReader* reader = new CCBReader(nodeLoader);
		m_tailImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("boss_melon_tail.ccbi",this));
		m_tailAnimationManager = reader->getAnimationManager();
		this->addChild(m_tailImg, lastZ - 1);
		reader->release();
	}
	
	
	IntPoint mapPoint;
	bool finded;
	getRandomPosition(&mapPoint, &finded);
	gameData->setMainCumberPoint(mapPoint);
	setPosition(ip2ccp(mapPoint));
	
	
	//	startMoving();
	schedule(schedule_selector(Melon::scaleAdjustment), 1/60.f);
	schedule(schedule_selector(KSCumberBase::movingAndCrash));
	schedule(schedule_selector(Melon::cumberAttack));
	
	startAnimationNoDirection();
	return true;
}

void Melon::setHeadAndBodies()
{
	SnakeTrace lastTrace = m_cumberTrace.back();
	float tt = rad2Deg( lastTrace.directionRad );
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

void Melon::startAnimationNoDirection()
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
		schedule(schedule_selector(Melon::animationNoDirection));
	}
}

void Melon::animationNoDirection(float dt)
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
			unschedule(schedule_selector(Melon::animationNoDirection));
			setPosition(m_noDirection.startingPoint);
			m_headAnimationManager->runAnimationsForSequenceNamed("cast101stop");
			for(auto bodyAniManager : m_bodyAnimationManagers)
			{
				bodyAniManager->runAnimationsForSequenceNamed("cast101stop");
			}
			m_tailAnimationManager->runAnimationsForSequenceNamed("cast101stop");
		}
		else
			setPosition(getPosition() + ccp(dx, dy));
	}
}



void Melon::startAnimationDirection()
{
	// 잭을 바라보자.
	m_state = CUMBERSTATEDIRECTION;
	m_direction.initVars();
	schedule(schedule_selector(Melon::animationDirection));
}

void Melon::animationDirection(float dt)
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
		unschedule(schedule_selector(Melon::animationDirection));
		m_headAnimationManager->runAnimationsForSequenceNamed("cast101stop");
		for(auto bodyAniManager : m_bodyAnimationManagers)
		{
			bodyAniManager->runAnimationsForSequenceNamed("cast101stop");
		}
		m_tailAnimationManager->runAnimationsForSequenceNamed("cast101stop");
	}
}
bool Melon::startDamageReaction(float damage, float angle)
{
	KSCumberBase::startDamageReaction(damage, angle);
	m_remainHp -= damage;
	CCLog("Melon Hp %f", m_remainHp);
	myGD->communication("UI_subBossLife", damage); //## 보스쪽에서 이걸 호출
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
		m_state = CUMBERSTATEMOVING;
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
		schedule(schedule_selector(Melon::damageReaction));
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
		schedule(schedule_selector(Melon::damageReaction));
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
		schedule(schedule_selector(Melon::damageReaction));
		crashMapForPosition(getPosition());
		myGD->communication("MS_resetRects");
	}
	
	if(m_remainHp <= 0)
		return true;
	else
		return false;
}

void Melon::damageReaction(float)
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
		unschedule(schedule_selector(Melon::damageReaction));
		m_headAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
		for(auto bodyAniManager : m_bodyAnimationManagers)
		{
			bodyAniManager->runAnimationsForSequenceNamed("Default Timeline");
		}
		m_tailAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
	}
}
void Melon::startInvisible(int totalframe)
{
	//	if(!isScheduled(schedule_selector(KSCumber::invisibling)))
	if(m_invisible.startInvisibleScheduler == false)
	{
		m_invisible.VISIBLE_FRAME = totalframe;
		m_invisible.invisibleFrame = 0;
		m_invisible.invisibleValue = 0;
		schedule(schedule_selector(Melon::invisibling));
		m_invisible.startInvisibleScheduler = true;
	}
}

void Melon::invisibling(float dt)
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

void Melon::scaleAdjustment(float dt)
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



COLLISION_CODE Melon::crashWithX(IntPoint check_position)
{
	/// 나갔을 시.
	if(check_position.x < mapLoopRange::mapWidthInnerBegin || check_position.x >= mapLoopRange::mapWidthInnerEnd ||
	   check_position.y < mapLoopRange::mapHeightInnerBegin || check_position.y >= mapLoopRange::mapHeightInnerEnd )
	{
		
		return COLLISION_CODE::kCOLLISION_OUTLINE;
	}
	
	/// 이미 그려진 곳에 충돌했을 경우.
	if(gameData->mapState[check_position.x][check_position.y] == mapOldline ||
	   gameData->mapState[check_position.x][check_position.y] == mapOldget)
	{
		return COLLISION_CODE::kCOLLISION_MAP;
	}
	
	if(gameData->mapState[check_position.x][check_position.y] == mapNewline)
	{
		return COLLISION_CODE::kCOLLISION_NEWLINE;
	}
	IntPoint jackPoint = gameData->getJackPoint();
	if(jackPoint.x == check_position.x && jackPoint.y == check_position.y)
	{
		return COLLISION_CODE::kCOLLISION_JACK;
	}
	
	
	
	
	
	return COLLISION_CODE::kCOLLISION_NONE;
	
}
COLLISION_CODE Melon::crashLooper(const set<IntPoint>& v, IntPoint* cp)
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

void Melon::furyModeOn()
{
	m_furyMode.startFury();
	m_state = CUMBERSTATEFURY;
	
	m_headImg->setColor(ccc3(0, 255, 0));
	m_tailImg->setColor(ccc3(0, 255, 0));
	for(auto i : m_Bodies)
	{
		i->setColor(ccc3(0, 255, 0));
	}
	
	schedule(schedule_selector(ThisClassType::furyModeScheduler));
}


void Melon::crashMapForPosition(CCPoint targetPt)
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
void Melon::furyModeScheduler(float dt)
{
	if(m_furyMode.furyFrameCount >= m_furyMode.totalFrame)
	{
		crashMapForPosition(getPosition());
		
		m_state = CUMBERSTATEMOVING;
		m_headImg->setColor(ccc3(255, 255, 255));
		m_tailImg->setColor(ccc3(255, 255, 255));
		for(auto i : m_Bodies)
		{
			i->setColor(ccc3(255, 255, 255));
		}
		myGD->communication("MS_resetRects");
		unschedule(schedule_selector(ThisClassType::furyModeScheduler));
	}
}
void Melon::furyModeOff()
{
	//##
	//	if(isFuryMode)
	//	{
	//		myGD->communication("EP_stopCrashAction");
	//		myGD->communication("MS_resetRects");
	//		isFuryMode = false;
	//		furyMode->removeFromParentAndCleanup(true);
	//	}
}

void Melon::getRandomPosition(IntPoint* ip, bool* finded)
{
	bool isGoodPointed = false;
	
	IntPoint mapPoint;
	vector<IntPoint> shuffledPositions;
	for(int x = 1; x <= mapLoopRange::mapWidthInnerEnd - 1; x++)
	{
		for(int y = 1; y <= mapLoopRange::mapHeightInnerEnd - 1; y++)
		{
			shuffledPositions.push_back(IntPoint(x, y));
		}
	}
	
	random_shuffle(shuffledPositions.begin(), shuffledPositions.end(), [=](int n){
		return this->m_well512.GetValue(n-1);
	} );
	for(auto& mp : shuffledPositions)
	{
		mapPoint = mp;
		
		float myScale = getCumberScale();
		if(mapPoint.isInnerMap() && gameData->mapState[mapPoint.x][mapPoint.y] == mapEmpty)
		{
			float half_distance = RADIUS*myScale; // 20.f : radius for base scale 1.f
			float calc_distance;
			IntPoint check_position;
			
			bool is_not_position = false;
			
			for(int i=mapPoint.x-half_distance/2;i<=mapPoint.x+half_distance/2 && !is_not_position;i++)
			{
				for(int j=mapPoint.y-half_distance/2;j<=mapPoint.y+half_distance/2 && !is_not_position;j++)
				{
					calc_distance = sqrtf(powf((mapPoint.x - i)*pixelSize,2) + powf((mapPoint.y - j)*pixelSize, 2));
					if(calc_distance < half_distance)
					{
						check_position = IntPoint(i,j);
						if(!check_position.isInnerMap() || gameData->mapState[check_position.x][check_position.y] != mapEmpty)
						{
							is_not_position = true;
						}
					}
				}
			}
			if(!is_not_position)
			{
				isGoodPointed = true;
				break;
			}
		}
	}
	
	if(isGoodPointed == true)
	{
		*ip = mapPoint;
		*finded = true;
	}
	else
	{
		*finded = false;
		// nothing.
		CCAssert(false, "");
	}
	
}

