#include "Apricot.h"
#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
//#include "CumberEmotion.h"
#include <algorithm>



bool Apricot::init()
{
	KSCumberBase::init();
	
	m_directionAngleDegree = m_well512.GetValue(0, 360);
	
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("BossCCB", ApricotLoader::loader());
	
	
	cocos2d::extension::CCBReader * reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	
	CCNode * node = reader->readNodeGraphFromFile("boss_apricot.ccbi", this);
	
	m_headImg = dynamic_cast<BossCCB*>(node);
	
//	std::string ccbiName = "img_ccb_test/boss_apricot_1.ccbi";
//    CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//    CCBReader* reader = new CCBReader(nodeLoader);
//	CCNode* p = reader->readNodeGraphFromFile(ccbiName.c_str(),this);
//	m_headImg = dynamic_cast<CCSprite*>(p);
	
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
	//	myGD->setMainCumberPoint(mapPoint);
	setPosition(ip2ccp(mapPoint));
	//	startMoving();
	CCLog("start Position!!!!!! : %d %d", mapPoint.x, mapPoint.y);
	lastCastNum = m_well512.GetValue(1, 1);
	mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstart", lastCastNum)->getCString());
	startAnimationNoDirection();
	
	schedule(schedule_selector(Apricot::scaleAdjustment), 1/60.f);
	schedule(schedule_selector(KSCumberBase::movingAndCrash));
	schedule(schedule_selector(Apricot::cumberAttack));
	scheduleUpdate();
//	m_headImg->m_7->setColor(ccc3(255, 0, 0));
	return true;
}






bool Apricot::startDamageReaction(float damage, float angle)
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
		schedule(schedule_selector(Apricot::damageReaction));
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
		schedule(schedule_selector(Apricot::damageReaction));
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
		schedule(schedule_selector(Apricot::damageReaction));
		crashMapForPosition(getPosition());
		myGD->communication("MS_resetRects");
	}
	
	if(m_remainHp <= 0)
		return true;
	else
		return false;
}

void Apricot::startAnimationNoDirection()
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
		schedule(schedule_selector(Apricot::animationNoDirection));
	}
}

void Apricot::damageReaction(float)
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
		unschedule(schedule_selector(Apricot::damageReaction));
		mAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
	}
}


void Apricot::animationNoDirection(float dt)
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
		unschedule(schedule_selector(Apricot::animationNoDirection));
//		mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstop", lastCastNum)->getCString()); //##
	}
}

void Apricot::onPatternEnd()
{
	CCLog("onPatternEnd!!");
	m_noDirection.state = 2;
}

void Apricot::onStartGame()
{
	m_noDirection.state = 2;
	CCLog("onStartGame!!");
}

COLLISION_CODE Apricot::crashLooper(const set<IntPoint>& v, IntPoint* cp)
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

void Apricot::startInvisible(int totalframe)
{
	//	if(!isScheduled(schedule_selector(KSCumber::invisibling)))
	if(m_invisible.startInvisibleScheduler == false)
	{
		m_invisible.VISIBLE_FRAME = totalframe;
		m_invisible.invisibleFrame = 0;
		m_invisible.invisibleValue = 0;
		schedule(schedule_selector(Apricot::invisibling));
		m_invisible.startInvisibleScheduler = true;
	}
}

void Apricot::invisibling(float dt)
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

void Apricot::getRandomPosition(IntPoint* ip, bool* finded)
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
		if(mapPoint.isInnerMap() && myGD->mapState[mapPoint.x][mapPoint.y] == mapEmpty)
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
						if(!check_position.isInnerMap() || myGD->mapState[check_position.x][check_position.y] != mapEmpty)
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
		//		myGD->setMainCumberPoint(mapPoint);
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

void Apricot::randomPosition()
{
	IntPoint mapPoint;
	bool finded;
	getRandomPosition(&mapPoint, &finded);
	
	//	myGD->setMainCumberPoint(mapPoint);
	setPosition(ip2ccp(mapPoint));
	m_circle.setRelocation(getPosition(), m_well512);
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

void Apricot::crashMapForPosition(CCPoint targetPt)
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

void Apricot::furyModeOn(int tf)
{
	m_furyMode.startFury(tf);
	m_noDirection.state = 2;
	m_state = CUMBERSTATEFURY;
	
	//	m_headImg->setColor(ccc3(0, 255, 0));
	
	schedule(schedule_selector(ThisClassType::furyModeScheduler));
}

void Apricot::furyModeScheduler(float dt)
{
	if(m_furyMode.furyFrameCount >= m_furyMode.totalFrame)
	{
		crashMapForPosition(getPosition());
		
		m_state = CUMBERSTATEMOVING;
		//		m_headImg->setColor(ccc3(255, 255, 255));
		myGD->communication("MS_resetRects");
		unschedule(schedule_selector(ThisClassType::furyModeScheduler));
	}
}
void Apricot::furyModeOff()
{
//	myGD->communication("EP_stopCrashAction");
	myGD->communication("MS_resetRects");
}



void Apricot::scaleAdjustment(float dt)
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

