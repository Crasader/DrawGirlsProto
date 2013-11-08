#include "Grape.h"
#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
//#include "CumberEmotion.h"
#include <algorithm>



bool Grape::init()
{
	KSCumberBase::init();
	
	m_directionAngleDegree = m_well512.GetValue(0, 360);
	
	std::string ccbiName = "boss_grape.ccbi";
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
	
		
	lastCastNum = m_well512.GetValue(1, 3);
	mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstart", lastCastNum)->getCString());

	
	schedule(schedule_selector(Grape::scaleAdjustment), 1/60.f);
	schedule(schedule_selector(KSCumberBase::movingAndCrash));

	
	return true;
}






bool Grape::startDamageReaction(float damage, float angle)
{
	KSCumberBase::startDamageReaction(damage, angle);
	CCLog("damaga!!!");
	m_remainHp -= damage;
	myGD->communication("UI_subBossLife", damage); //## 보스쪽에서 이걸 호출
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
		float rad = deg2Rad(angle);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLog("%f %f", dx, dy);
		m_state = CUMBERSTATEDAMAGING;
		
		m_damageData.timer = 0;
		schedule(schedule_selector(Grape::damageReaction));
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
		schedule(schedule_selector(Grape::damageReaction));
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
		schedule(schedule_selector(Grape::damageReaction));
		crashMapForPosition(getPosition());
		myGD->communication("MS_resetRects", false);
	}
	
	if(m_remainHp <= 0)
		return true;
	else
		return false;
}

void Grape::startAnimationNoDirection()
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
		schedule(schedule_selector(Grape::animationNoDirection));
	}
}

void Grape::damageReaction(float)
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
		unschedule(schedule_selector(Grape::damageReaction));
		mAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
	}
}


void Grape::animationNoDirection(float dt)
{
	m_noDirection.timer += 1.f/60.f;
	
	if(m_noDirection.state == 1)
	{
//		/// 좀 돌았으면 돌아감.
//		if(m_noDirection.timer >= 5)
//		{
//			m_noDirection.state = 2;
//			return;
//		}
	}
	else if(m_noDirection.state == 2)
	{
		m_state = CUMBERSTATEMOVING;
		unschedule(schedule_selector(Grape::animationNoDirection));
		mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstop", lastCastNum)->getCString());
	}
}

void Grape::onPatternEnd()
{
	CCLog("onPatternEnd!!");
	m_noDirection.state = 2;
}

void Grape::onStartGame()
{
	KSCumberBase::onStartGame();
	m_noDirection.state = 2;
	CCLog("onStartGame!!");
}

COLLISION_CODE Grape::crashLooper(const set<IntPoint>& v, IntPoint* cp)
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

void Grape::startInvisible(int totalframe)
{
	//	if(!isScheduled(schedule_selector(KSCumber::invisibling)))
	if(m_invisible.startInvisibleScheduler == false)
	{
		m_invisible.VISIBLE_FRAME = totalframe;
		m_invisible.invisibleFrame = 0;
		m_invisible.invisibleValue = 0;
		schedule(schedule_selector(Grape::invisibling));
		m_invisible.startInvisibleScheduler = true;
	}
}

void Grape::invisibling(float dt)
{
	m_invisible.invisibleFrame++;
	
	if(m_invisible.invisibleFrame < m_invisible.VISIBLE_FRAME)
	{
		KS::setOpacity(m_headImg, MAX(0, 255 - m_invisible.invisibleFrame*5));
	}
	else
	{
		// 최소 1 최대 255
		m_invisible.invisibleValue = MIN(255, MAX(1, m_invisible.invisibleValue * 1.2f));
		
		KS::setOpacity(m_headImg, m_invisible.invisibleValue);
		if(m_invisible.invisibleValue == 255)
		{
			m_invisible.startInvisibleScheduler = false;
			unschedule(schedule_selector(ThisClassType::invisibling));
		}
	}
	
}

void Grape::randomPosition()
{
	IntPoint mapPoint;
	bool finded;
	getRandomPosition(&mapPoint, &finded);
	
	//	myGD->setMainCumberPoint(mapPoint);
	setPosition(ip2ccp(mapPoint));
	m_circle.setRelocation(getPosition(), m_well512);
	CCScaleTo* t_scale = CCScaleTo::create(0.5f, 1.f); //##
	m_headImg->runAction(t_scale);
	
	
	lightSmaller();
	
}

void Grape::crashMapForPosition(CCPoint targetPt)
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

void Grape::furyModeOn(int tf)
{
	m_furyMode.startFury(tf);
	m_noDirection.state = 2;
	m_state = CUMBERSTATEFURY;
	
	//	m_headImg->setColor(ccc3(0, 255, 0));
	
	schedule(schedule_selector(ThisClassType::furyModeScheduler));
}

void Grape::furyModeScheduler(float dt)
{
	if(m_furyMode.furyFrameCount >= m_furyMode.totalFrame)
	{
		crashMapForPosition(getPosition());
		
		m_state = CUMBERSTATEMOVING;
		//		m_headImg->setColor(ccc3(255, 255, 255));
		myGD->communication("MS_resetRects", false);
		unschedule(schedule_selector(ThisClassType::furyModeScheduler));
	}
}
void Grape::furyModeOff()
{
//	myGD->communication("EP_stopCrashAction");
	myGD->communication("MS_resetRects", false);
}



void Grape::scaleAdjustment(float dt)
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

