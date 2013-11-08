#include "KSCircleBase.h"
#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
//#include "CumberEmotion.h"
#include <algorithm>
#include <cocos-ext.h>


bool KSCircleBase::init(const string& ccbiName)
{
	KSCumberBase::init();
	
	////////////////////////////////////// by hs
	string ccbiname2 = ccbiName;
	if(ccbiName.length()<3) {
		ccbiname2="cherry";
	}
	////////////////////////////////////////////
	
	m_directionAngleDegree = m_well512.GetValue(0, 360);
	
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("CircleBossCCB", CircleLoader::loader());
	
	cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	CCNode* p = reader->readNodeGraphFromFile(("boss_" + ccbiname2 + ".ccbi").c_str(), this);
	m_headImg = dynamic_cast<CircleBossCCB*>(p);
	mAnimationManager = reader->getAnimationManager();
	mAnimationManager->setDelegate(this);
	reader->release();
	
	
	if(m_headImg != NULL) {
		this->addChild(m_headImg);
	}
	
	//	m_headImg = CCSprite::create("chapter10_boss.png");
	//	addChild(m_headImg);
	
	
	lastCastNum = m_well512.GetValue(1, 1);
	mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstart", lastCastNum)->getCString());
	
	
	schedule(schedule_selector(KSCircleBase::scaleAdjustment), 1/60.f);
	schedule(schedule_selector(KSCumberBase::movingAndCrash));
	
	schedule(schedule_selector(ThisClassType::update), 1/20.f);
	//	m_headImg->m_7->setColor(ccc3(255, 0, 0));
	return true;
}

bool KSCircleBase::startDamageReaction(float damage, float angle)
{
	KSCumberBase::startDamageReaction(damage, angle);
	CCLog("damaga!!!");
	m_remainHp -= damage;
	myGD->communication("UI_subBossLife", damage); //## 보스쪽에서 이걸 호출
	// 방사형으로 돌아가고 있는 중이라면
	m_invisible.invisibleFrame = m_invisible.VISIBLE_FRAME; // 인비지블 풀어주는 쪽으로 유도.
	setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER)); // 맞으면 작게 함.
	
	m_attackCanceled = true;
	
	// 방사형으로 돌아가고 있는 중이라면
	if(m_state == CUMBERSTATENODIRECTION)
	{
		CCLog("m_state == CUMBERSTATENODIRECTION");
		m_noDirection.state = 2; // 돌아가라고 상태 변경때림.
		
	}
	if(m_state == CUMBERSTATEMOVING)
	{
		CCLog("m_state == CUMBERSTATEMOVING");
		float rad = deg2Rad(angle);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLog("%f %f", dx, dy);
		m_state = CUMBERSTATEDAMAGING;
		
		m_damageData.timer = 0;
		schedule(schedule_selector(KSCircleBase::damageReaction));
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
		schedule(schedule_selector(KSCircleBase::damageReaction));
		
		if(currentTimelineFooter == "_b")
		{
			m_castingCancelCount++;
		}
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
		schedule(schedule_selector(KSCircleBase::damageReaction));
		crashMapForPosition(getPosition());
		myGD->communication("MS_resetRects", false);
	}
	
	if(m_remainHp <= 0)
		return true;
	else
		return false;
}

void KSCircleBase::startAnimationNoDirection()
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
		schedule(schedule_selector(KSCircleBase::animationNoDirection));
	}
}

void KSCircleBase::damageReaction(float)
{
	m_damageData.timer += 1 / 60.f;
	if(m_damageData.timer < 1)
	{
		//		m_headImg->setColor(ccc3(255, 0, 0)); //##
	}
	else if(currentTimelineFooter == "")
	{
		//		m_headImg->setColor(ccc3(255, 255, 255));
		if(m_state != CUMBERSTATEMOVING)
		{
			m_state = CUMBERSTATEMOVING;
			unschedule(schedule_selector(KSCircleBase::damageReaction));
			mAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
		}
		
	}
}


void KSCircleBase::animationNoDirection(float dt)
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
		unschedule(schedule_selector(KSCircleBase::animationNoDirection));
		mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstop", lastCastNum)->getCString()); //##
	}
}

void KSCircleBase::onPatternEnd()
{
	CCLog("onPatternEnd!!");
	m_noDirection.state = 2;
}

void KSCircleBase::onStartGame()
{
	KSCumberBase::onStartGame();
	m_noDirection.state = 2;
	CCLog("onStartGame!!");
}

COLLISION_CODE KSCircleBase::crashLooper(const set<IntPoint>& v, IntPoint* cp)
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

void KSCircleBase::startInvisible(int totalframe)
{
	//	if(!isScheduled(schedule_selector(KSCumber::invisibling)))
	if(m_invisible.startInvisibleScheduler == false)
	{
		m_invisible.VISIBLE_FRAME = totalframe;
		m_invisible.invisibleFrame = 0;
		m_invisible.invisibleValue = 0;
		schedule(schedule_selector(KSCircleBase::invisibling));
		m_invisible.startInvisibleScheduler = true;
	}
}

void KSCircleBase::invisibling(float dt)
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

void KSCircleBase::randomPosition()
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

void KSCircleBase::crashMapForPosition(CCPoint targetPt)
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

void KSCircleBase::furyModeOn(int tf)
{
	m_furyMode.startFury(tf);
	m_noDirection.state = 2;
	m_state = CUMBERSTATEFURY;
	
	//	m_headImg->setColor(ccc3(0, 255, 0));
	
	schedule(schedule_selector(ThisClassType::furyModeScheduler));
}

void KSCircleBase::furyModeScheduler(float dt)
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
void KSCircleBase::furyModeOff()
{
	//	myGD->communication("EP_stopCrashAction");
	myGD->communication("MS_resetRects", false);
}



void KSCircleBase::scaleAdjustment(float dt)
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

