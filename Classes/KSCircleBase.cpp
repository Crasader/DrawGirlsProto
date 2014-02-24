#include "KSCircleBase.h"
#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
//#include "CumberEmotion.h"
#include <algorithm>
#include <cocos-ext.h>
#include "StageImgLoader.h"
#include "KSUtil.h"
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
	//CCNode* p = reader->readNodeGraphFromFile(("boss_" + ccbiname2 + ".ccbi").c_str(), this);
	CCNode* p = reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+ccbiname2+".ccbi").c_str(), this);
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
	schedule(schedule_selector(KSCumberBase::followProcess));	
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
			m_furyMode.furyFrameCount = m_furyMode.totalFrame;
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
	getRandomPositionToJack(&mapPoint, &finded);
	
	setPosition(ip2ccp(mapPoint));
	m_circle.setRelocation(getPosition(), m_well512);
	m_snake.setRelocation(getPosition(), m_well512);
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

SEL_MenuHandler KSCircleBase::onResolveCCBCCMenuItemSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCLog("%s", pSelectorName);
	return NULL;
}

SEL_CallFuncN KSCircleBase::onResolveCCBCCCallFuncSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCLog("%s", pSelectorName);
	return NULL;
}

SEL_CCControlHandler KSCircleBase::onResolveCCBCCControlSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCLog("%s", pSelectorName);
	return NULL;
}

void KSCircleBase::runTimeline( Json::Value patternInfo )
{
	if(currentTimelineFooter == "") // 아무것도 공격중이 아니면 발싸 !!
	{
		std::string timeline = patternInfo["pattern"].asString();
		m_atype = patternInfo.get("atype", "special").asString();
		m_repeatNumber = patternInfo.get("repeat", 1).asInt();
		m_attackCanceled = false;
		currentTimeline = timeline;
		currentTimelineFooter = "_b";
		mAnimationManager->runAnimationsForSequenceNamed((currentTimeline + currentTimelineFooter).c_str());
	}
}

void KSCircleBase::completedAnimationSequenceNamed( const char *name_ )
{
	string name = name_;
	if(name.size() < 2)
		return;
	char lastChar = name[name.size() - 1];
	char lastPrevChar = name[name.size() - 2];
	std::string tl(name.begin(), name.end() - 2);
	if(lastPrevChar != '_')
		return;
	if(lastChar == 'b')
	{
		// 캔슬이 되었는지의 여부를 알아야 됨.
		// middle 액션을 하기전에 속성을 설정함.

		if(m_atype == "crash")
		{
			AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
		}
		else if(m_atype == "special")
		{
			AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
		}
		else // normal
		{
			AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
		}

		if(m_attackCanceled) // 맞아서 캔슬이 되었다면
		{
			currentTimelineFooter = "_e";
			mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
		}
		else
		{
			currentTimelineFooter = "_m";
			mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
		}
	}
	else if(lastChar == 'm')
	{

		// 반복을 시킬 건지 검사하고 캔슬이 되었다면 캔슬 동작을 작동시킴.
		// 캔슬이면 속성을 해제함.
		if(m_attackCanceled) // 캔슬
		{
			currentTimelineFooter = "_e";
			mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
		}
		else
		{
			m_repeatNumber--;
			if(m_repeatNumber > 0)
			{
				currentTimelineFooter = "_m";
				mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
			}
			else
			{
				currentTimelineFooter = "_e";
				mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
			}
		}
	}
	else if(lastChar == 'e')
	{
		currentTimelineFooter = "";
		m_state = CUMBERSTATEMOVING;
		mAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
		myGD->communication("MS_resetRects", false);
	}
}

void KSCircleBase::onStartMoving()
{
	m_state = CUMBERSTATEMOVING;
	schedule(schedule_selector(KSCumberBase::movingAndCrash));
}

void KSCircleBase::onStopMoving()
{
	m_state = CUMBERSTATESTOP;
}

void KSCircleBase::stopCasting()
{
	myGD->communication("MP_bombCumber", this);
	// 방사형으로 돌아가고 있는 중이라면
	if(m_state == CUMBERSTATENODIRECTION)
	{
		CCLog("m_state == CUMBERSTATENODIRECTION");
		m_noDirection.state = 2; // 돌아가라고 상태 변경때림.
	}
}

void KSCircleBase::setPosition( const CCPoint& t_sp )
{
	//CCLog("setPos %f %f", t_sp.x, t_sp.y);
	//		KSCumberBase::setPosition(t_sp);
	m_headImg->setPosition(t_sp);
	// 돌때랑 분노 모드일 땐 메인포인트 지정하면 안됨.
	if(m_state == CUMBERSTATENODIRECTION || m_state == CUMBERSTATEFURY)
	{
		// black hole!! 
	}
	else
	{
		myGD->setMainCumberPoint(this, ccp2ip(t_sp));
		m_mapPoint = ccp2ip(t_sp);
	}
}

void KSCircleBase::setPositionX( float t_x )
{
	setPosition(ccp(t_x, getPositionY()));
}

void KSCircleBase::setPositionY( float t_y )
{
	setPosition(ccp(getPositionX(), t_y));
}

const CCPoint& KSCircleBase::getPosition()
{
	return m_headImg->getPosition();
}

void KSCircleBase::attackBehavior( Json::Value _pattern )
{
	std::string pattern = _pattern["pattern"].asString();
	if(pattern == "109")
	{
		m_state = CUMBERSTATESTOP;
	}
	else if( pattern == "1007")
	{
		m_state = CUMBERSTATESTOP;
	}
	else if(pattern.size() >= 2 && pattern[0] == 'a' && pattern[1] == 't') // ccb 관련 공격.
	{
		m_state = CUMBERSTATESTOP;
		//			startAnimationNoDirection();
	}
	else
	{
		std::string atype = _pattern.get("atype", "normal").asString();
		std::map<std::string, int> mapper = {{"normal", 1}, {"special", 2}, {"crash", 3}};
		if(mapper.find(atype) == mapper.end())
			lastCastNum = 1;
		else
			lastCastNum = mapper[atype];

		mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstart", lastCastNum)->getCString());

		std::string target = _pattern.get("target", "no").asString();
		if( target == "yes") // 타게팅이라면 조준하라
			startAnimationDirection();
		else if(target == "no") // 타게팅이 아니면 돌아라
			startAnimationNoDirection();
	}
}

CCPoint KSCircleBase::getMissilePoint()
{
	return getPosition() + ccp(0, 0);
}

void KSCircleBase::setScale( float scale )
{
	m_headImg->setScale(scale);
}

void KSCircleBase::setScaleX( float x )
{
	m_headImg->setScaleX(x);
}

void KSCircleBase::setScaleY( float y )
{
	m_headImg->setScaleY(y);
}

void KSCircleBase::stopAnimationNoDirection()
{
	m_noDirection.state = 2;
}

void KSCircleBase::stopAnimationDirection()
{
	//		m_direction.state = 2;
}

COLLISION_CODE KSCircleBase::getCrashCode( IntPoint point, IntPoint* checkPosition )
{
	IntPoint afterPoint = point;
	float half_distance = RADIUS*getCumberScale(); // 20.f : radius for base scale 1.f
	int ip_half_distance = half_distance / 2;
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
	return collisionCode;
}

float KSCircleBase::getRadius()
{
	return RADIUS;
}

void KSCircleBase::update( float dt )
{
	CCNode* endP = myGD->getCommunicationNode("Main_gameNodePointer");
	auto function = [&](CCNode* node)->CCAffineTransform
	{
		CCAffineTransform t = this->nodeToParentTransform();
		for (CCNode *p = node->getParent(); p != endP; p = p->getParent())
		{
			if( p == NULL)
				return CCAffineTransformIdentity;
			else
				t = CCAffineTransformConcat(t, p->nodeToParentTransform());
		}
		return t;
	};



	for(auto i : m_headImg->m_s)
	{
		CCPoint ret = CCPointApplyAffineTransform(i->getPosition(), function(i));
		if(currentTimelineFooter == "_m")
		{
			if(m_atype == "crash")
			{
				crashMapForPosition(ret);
			}
			else if(m_atype == "normal")
			{
				IntPoint bulletPoint = ccp2ip(ret);
				IntPoint jackPoint = myGD->getJackPoint();
				int sizex = i->getContentSize().width * getCumberScale() * 1.2f / 2.f;
				int sizey = i->getContentSize().height * getCumberScale() * 1.2 / 2.f;
				auto mapState = myGD->mapState;

				//					if(myGD->getJackState() == jackStateDrawing &&
				//						 (bulletPoint - jackPoint).length() <= sizex)
				//					{
				//						myGD->communication("CP_jackCrashDie");
				//						myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				//					}
				//					else
				{
					for(int y = bulletPoint.y - sizey; y<=bulletPoint.y + sizey; y++)
					{
						for(int x = bulletPoint.x - sizex; x<=bulletPoint.x + sizex; x++)
						{
							//CCLog("\t %d %d", x, y);
							if(IntPoint(x, y).isInnerMap() &&
								mapState[x][y] == mapType::mapNewline)
							{
								myGD->communication("PM_addPathBreaking", IntPoint(x, y));
								goto end;
							}
						}
					}
end:
					;
				}

			}
			else if(m_atype == "special")
			{
				IntPoint bulletPoint = ccp2ip(ret);
				IntPoint jackPoint = myGD->getJackPoint();
				if( (bulletPoint - jackPoint).length() <= 2)
				{
					myGD->communication("CP_jackCrashDie");
					myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				}
			}
		}
	}
}

IntPoint KSCircleBase::getMapPoint( CCPoint c )
{
	return IntPoint(round((c.x - 1) / pixelSize + 1.f),
		round((c.y - 1) / pixelSize + 1.f));
}

























bool CircleBossCCB::onAssignCCBCustomProperty( cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::extension::CCBValue *pCCBValue )
{
	return false;
}

bool CircleBossCCB::onAssignCCBMemberVariable( CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode )
{
	/*
	* CCB_MEMBERVARIABLEASSIGNER_GLUE parameters
	* 1. 타겟. 커스텀 클래스, 여기선 this (this == pTarget)
	* 2. 매칭시킬 변수 이름. pMemberVariableName == "m_pSprBg" 코코스빌더에서 지정한 이름과 같아야 매칭이 됩니다.
	* 3. 멤버변수의 타입. (CCSprite*)
	* 4. 헤더에 선언한 실제 멤버변수 this->m_pSprBg
	*/
	//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,   "m_7", CCSprite *, m_7);
	CCSprite* temp = 0;
	if (pTarget == this) {
		temp = dynamic_cast<CCSprite*>(pNode);
		temp->retain();
		m_s.push_back(temp);
		return true;

	}
	/*
	* 성공하면 위에서 return true 됩니다.
	*/
	return false;
}

void CircleBossCCB::onNodeLoaded( CCNode * pNode, CCNodeLoader * pNodeLoader )
{

}
