//
//  KSCumber.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#include "Banana.h"
#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
#include "CumberEmotion.h"
#include <algorithm>



bool Banana::init()
{
	KSCumberBase::init();
	
	m_directionAngleDegree = m_well512.GetValue(0, 360);
	
	std::string ccbiName = "boss_banana.ccbi";
    CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    CCBReader* reader = new CCBReader(nodeLoader);
	CCNode* p = reader->readNodeGraphFromFile(ccbiName.c_str(),this);
	m_headImg = dynamic_cast<CCSprite*>(p);
    
	
	
    mAnimationManager = reader->getAnimationManager();
    reader->release();
    if(m_headImg != NULL) {
        this->addChild(m_headImg);
    }
	
	//	m_headImg = CCSprite::create("chapter10_boss.png");
	//	addChild(m_headImg);
	
	IntPoint mapPoint;
	bool finded;
	getRandomPosition(&mapPoint, &finded);
	//	gameData->setMainCumberPoint(mapPoint);
	setPosition(ip2ccp(mapPoint));
	//	startMoving();
	
	lastCastNum = m_well512.GetValue(1, 3);
	mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstart", lastCastNum)->getCString());
	startAnimationNoDirection();
	
	schedule(schedule_selector(Banana::scaleAdjustment), 1/60.f);
	schedule(schedule_selector(KSCumberBase::movingAndCrash));
	schedule(schedule_selector(Banana::cumberAttack));
	
	return true;
}


void Banana::normalMoving(float dt)
{
	m_scale.timer += 1 / 60.f;
	
	
	
	if(m_scale.collisionStartTime + 1 < m_scale.timer || m_state != CUMBERSTATEMOVING)
	{
		m_scale.collisionCount = 0;
		m_scale.collisionStartTime = m_scale.timer;
		//		setCumberSize(MIN(1.0, getCumberSize() + scale.SCALE_ADDER));
	}
	
	
	m_directionAngleDegree += m_well512.GetValue(-4, +4);
	
	if(m_state == CUMBERSTATEMOVING)
	{
		int sela = ProbSelector::sel(0.005, 1.0 - 0.005, 0.0);
		if(sela == 0)
		{
			m_directionAngleDegree += m_well512.GetValue(90, 270);
		}
	}
	
	CCPoint afterPosition;
	
	bool validPosition = false;
	int cnt = 0;
	bool onceOutlineAndMapCollision = false;
	
	
	while(!validPosition)
	{
		cnt++;
		float speedX = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + 0.1f*cnt);
		float speedY = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + 0.1f*cnt);
		
		CCPoint cumberPosition = getPosition();
		afterPosition = cumberPosition + ccp(speedX, speedY);
		IntPoint afterPoint = ccp2ip(afterPosition);
		IntPoint checkPosition;
		COLLISION_CODE collisionCode = getCrashCode(afterPoint, &checkPosition);
		if(m_state != CUMBERSTATEFURY)
		{
			if(collisionCode == kCOLLISION_JACK)
			{
				// 즉사 시킴.
				gameData->communication("Jack_startDieEffect");
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				onceOutlineAndMapCollision = true;
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//			gameData->communication("Jack_startDieEffect");
				//			gameData->communication("SW_createSW", checkPosition, 0, 0);
				//									callfuncI_selector(KSCumber::showEmotion)); //##
				gameData->communication("SW_createSW", checkPosition, 0, 0);
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				onceOutlineAndMapCollision = true;
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
			}
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else if(afterPoint.isInnerMap())
			{
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
				//			CCLog("collision!!");
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
			}
			else
			{
				validPosition = true;
			}
			if(m_furyMode.furyFrameCount % 8 == 0) // n 프레임당 한번 깎음.
			{
				crashMapForPosition(afterPosition);
			}
		}
		
		
		//		setPosition(afterPosition);
		if(cnt % 100 == 0)
		{
			CCLog("cnt !! = %d", cnt);
		}
	}
	
	//	CCLog("cnt outer !! = %d", cnt);
	
	if(m_state == CUMBERSTATEMOVING)
		setPosition(afterPosition);
	
	if(onceOutlineAndMapCollision)
	{
		
		if(m_scale.collisionCount == 0)
		{
			m_scale.collisionStartTime = m_scale.timer;
			
		}
		m_scale.collisionCount++;
		if(m_scale.collisionCount >= LIMIT_COLLISION_PER_SEC)
		{
			CCLog("decrese Size !!");
			setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER));
		}
	}
}





void Banana::startDamageReaction(float userdata)
{
	CCLog("damaga!!!");
	// 방사형으로 돌아가고 있는 중이라면
	m_invisible.invisibleFrame = m_invisible.VISIBLE_FRAME; // 인비지블 풀어주는 쪽으로 유도.
	setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER)); // 맞으면 작게 함.
	
	
	if(m_state == CUMBERSTATENODIRECTION)
	{
		CCLog("m_state == CUMBERSTATENODIRECTION");
		m_noDirection.state = 2; // 돌아가라고 상태 변경때림.
	}
	else if(m_state == CUMBERSTATEMOVING)
	{
		CCLog("m_state == CUMBERSTATEMOVING");
		float rad = deg2Rad(userdata);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLog("%f %f", dx, dy);
		m_state = CUMBERSTATEDAMAGING;
		
		m_damageData.timer = 0;
		schedule(schedule_selector(Banana::damageReaction));
	}
	else if(m_state == CUMBERSTATESTOP)
	{
		CCLog("m_state == CUMBERSTATESTOP");
		float rad = deg2Rad(userdata);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLog("%f %f", dx, dy);
		m_state = CUMBERSTATEDAMAGING;
		
		m_damageData.timer = 0;
		schedule(schedule_selector(Banana::damageReaction));
	}
	else if(m_state == CUMBERSTATEFURY)
	{
		CCLog("m_state == CUMBERSTATEMOVING");
		float rad = deg2Rad(userdata);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLog("%f %f", dx, dy);
		m_state = CUMBERSTATEDAMAGING;
		
		m_damageData.timer = 0;
		schedule(schedule_selector(Banana::damageReaction));
		crashMapForPosition(getPosition());
		myGD->communication("MS_resetRects");
	}
}

void Banana::startAnimationNoDirection()
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
		schedule(schedule_selector(Banana::animationNoDirection));
	}
}

void Banana::damageReaction(float)
{
	m_damageData.timer += 1 / 60.f;
	if(m_damageData.timer < 1)
	{
		//		m_headImg->setColor(ccc3(255, 0, 0)); //##
	}
	else
	{
		//		m_headImg->setColor(ccc3(255, 255, 255));
		m_state = CUMBERSTATEMOVING;
		unschedule(schedule_selector(Banana::damageReaction));
		mAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
	}
}


void Banana::animationNoDirection(float dt)
{
	m_noDirection.timer += 1.f/60.f;
	
	if(m_noDirection.state == 1)
	{
		/// 좀 돌았으면 돌아감.
		if(m_noDirection.rotationCnt >= 5)
		{
			m_noDirection.state = 2;
			return;
		}
	}
	else if(m_noDirection.state == 2)
	{
		m_state = CUMBERSTATEMOVING;
		unschedule(schedule_selector(Banana::animationNoDirection));
		mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstop", lastCastNum)->getCString());
	}
}

void Banana::onPatternEnd()
{
	CCLog("onPatternEnd!!");
	m_noDirection.state = 2;
}

void Banana::onStartGame()
{
	m_noDirection.state = 2;
	CCLog("onStartGame!!");
}


void Banana::cumberAttack(float dt)
{
	float w = ProbSelector::sel(m_attackPercent / 100.f, 1.0 - m_attackPercent / 100.f, 0.0);
	
	// 1% 확률로.
	if(w == 0 && m_state == CUMBERSTATEMOVING)
	{
		int attackCode = 0;
		//		std::vector<int> attacks = {kAP_CODE_pattern10, kAP_CODE_pattern13, kAP_CODE_pattern17, kAP_CODE_pattern23,
		//			kAP_CODE_pattern101, kAP_CODE_pattern101, kAP_CODE_pattern102, kAP_CODE_pattern102,
		//			kAP_CODE_pattern103, kAP_CODE_pattern103};
		//		std::vector<int> attacks = {
		////			kCrashAttack1,
		//			kNonTargetAttack1,
		//			kTargetAttack4
		//			kSpecialAttack7, // 텔레포트.          // 32
		//		};
		//		std::vector<int> attacks = {kNonTargetAttack1, kNonTargetAttack2,
		//		kNonTargetAttack3, kNonTargetAttack4, kNonTargetAttack5, kNonTargetAttack6, kNonTargetAttack7,
		//		kNonTargetAttack8, kTargetAttack1, kTargetAttack2, kTargetAttack3, kTargetAttack4};
		
		
		
		
		bool searched = false;
		while(!searched)
		{
			random_shuffle(m_attacks.begin(), m_attacks.end(), [=](int n){
				return this->m_well512.GetValue(n-1);
			});
			attackCode = m_attacks[0];
			searched = true;
			if(attackCode == 34 && m_invisible.startInvisibleScheduler)
				searched = false;
			if(attackCode == 13 && m_state == CUMBERSTATEFURY)
				searched = false;
		}
		
		//		attackCode = 13;
		if(attackCode == 13) // fury
		{
			CCLog("aaa %f %f", getPosition().x, getPosition().y);
			m_state = CUMBERSTATESTOP;
			gameData->communication("MP_attackWithCode", getPosition(), attackCode);
		}
		else
		{
			CCLog("acode %d", attackCode);
			lastCastNum = m_well512.GetValue(1, 3);
			mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstart", lastCastNum)->getCString());
			startAnimationNoDirection();
			gameData->communication("MP_attackWithKSCode", getPosition(), attackCode, this);
		}
	}
}
COLLISION_CODE Banana::crashWithX(IntPoint check_position)
{
	if(check_position.x < mapLoopRange::mapWidthInnerBegin || check_position.x >= mapLoopRange::mapWidthInnerEnd ||
	   check_position.y < mapLoopRange::mapHeightInnerBegin || check_position.y >= mapLoopRange::mapHeightInnerEnd )
	{
		// 나갔을 시.
		return COLLISION_CODE::kCOLLISION_OUTLINE;
	}
	
	// 이미 그려진 곳에 충돌했을 경우.
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
COLLISION_CODE Banana::crashLooper(const set<IntPoint>& v, IntPoint* cp)
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

void Banana::startInvisible()
{
	//	if(!isScheduled(schedule_selector(KSCumber::invisibling)))
	if(m_invisible.startInvisibleScheduler == false)
	{
		m_invisible.invisibleFrame = 0;
		m_invisible.invisibleValue = 0;
		schedule(schedule_selector(Banana::invisibling));
		m_invisible.startInvisibleScheduler = true;
	}
}

void Banana::invisibling(float dt)
{
	m_invisible.invisibleFrame++;
	
	if(m_invisible.invisibleFrame < m_invisible.VISIBLE_FRAME)
	{
		//		m_headImg->setOpacity(MAX(0, 255 - m_invisible.invisibleFrame*5));
	}
	else
	{
		// 최소 1 최대 255
		m_invisible.invisibleValue = MIN(255, MAX(1, m_invisible.invisibleValue * 1.2f));
		
		//		m_headImg->setOpacity(m_invisible.invisibleValue);
		if(m_invisible.invisibleValue == 255)
		{
			m_invisible.startInvisibleScheduler = false;
			unschedule(schedule_selector(Banana::invisibling));
		}
	}
	
}

void Banana::getRandomPosition(IntPoint* ip, bool* finded)
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
	});
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
		//		CCLog("map point %d %d", mapPoint.x, mapPoint.y);
		//		CCLog("scale %f", m_headImg->getScale());
		//		CCScaleTo* t_scale = CCScaleTo::create(0.5f, m_scale);
		//		m_headImg->runAction(t_scale);
		//		gameData->setMainCumberPoint(mapPoint);
		//
		//		setPosition(ccp((mapPoint.x-1)*pixelSize + 1,(mapPoint.y-1)*pixelSize + 1));
	}
	else
	{
		*finded = false;
		// nothing.
		CCAssert(false, "");
	}
}

void Banana::randomPosition()
{
	IntPoint mapPoint;
	bool finded;
	getRandomPosition(&mapPoint, &finded);
	
	//	gameData->setMainCumberPoint(mapPoint);
	setPosition(ip2ccp(mapPoint));
	
	CCScaleTo* t_scale = CCScaleTo::create(0.5f, 1.f); //##
	m_headImg->runAction(t_scale);
	
	
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.2, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this,
												callfunc_selector(KSCumberBase::lightSmaller));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		runAction(t_seq);
	}
	
}

void Banana::crashMapForPosition(CCPoint targetPt)
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

void Banana::furyModeOn()
{
	m_furyMode.startFury();
	m_noDirection.state = 2;
	m_state = CUMBERSTATEFURY;
	
	//	m_headImg->setColor(ccc3(0, 255, 0));
	
	schedule(schedule_selector(ThisClassType::furyModeScheduler));
}

void Banana::furyModeScheduler(float dt)
{
	m_furyMode.furyTimer += 1.f / 60.f;
	
	if(m_furyMode.furyTimer >= FURY_DURATION)
	{
		crashMapForPosition(getPosition());
		
		m_state = CUMBERSTATEMOVING;
		//		m_headImg->setColor(ccc3(255, 255, 255));
		myGD->communication("MS_resetRects");
		unschedule(schedule_selector(ThisClassType::furyModeScheduler));
	}
}
void Banana::furyModeOff()
{
	myGD->communication("EP_stopCrashAction");
	myGD->communication("MS_resetRects");
}

void Banana::setGameover()
{
	m_state = CUMBERSTATESTOP;
}


void Banana::scaleAdjustment(float dt)
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
	
}

