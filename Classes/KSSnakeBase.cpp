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
#include "CumberEmotion.h"

KSSnakeBase::~KSSnakeBase()
{
	CCLOG("~KSSnakeBase");
}

SEL_MenuHandler KSSnakeBase::onResolveCCBCCMenuItemSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCLOG("%s", pSelectorName);
	return NULL;
}

SEL_CallFuncN KSSnakeBase::onResolveCCBCCCallFuncSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCLOG("%s", pSelectorName);
	return NULL;
}

SEL_CCControlHandler KSSnakeBase::onResolveCCBCCControlSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCLOG("%s", pSelectorName);
	return NULL;
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
		m_headImg->addChild(m_headAnimationManager);
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
			body->addChild(reader->getAnimationManager());
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
		m_tailImg->addChild(m_tailAnimationManager);
		this->addChild(m_tailImg, lastZ - 1);
		reader->release();
	}
	
	
	
	
	
	//	startMoving();
	if(isNotShowWindow)
	{
		schedule(schedule_selector(KSSnakeBase::scaleAdjustment), 1/60.f);
		schedule(schedule_selector(KSCumberBase::movingAndCrash));
		schedule(schedule_selector(KSCumberBase::followProcess), 1/5.f);
		
	}
	
	
	return true;
}


void KSSnakeBase::startAnimationNoDirection()
{
	// 돌자...
	CCLOG("돌기");
	if((m_cumberState & kCumberStateNoDirection) == 0)
	{
		m_cumberState |= kCumberStateNoDirection;
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
		float dy = sin(deg2Rad(m_noDirection.rotationDeg)) * m_noDirection.distance * getCumberScale() * 1.3f; // 약간 타원
		
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
			CCLOG("도는것을 멈추고 움직이기 시작함");
			setCumberState(kCumberStateMoving);
			m_noDirection.state = 0;
			
			unschedule(schedule_selector(KSSnakeBase::animationNoDirection));
			
			setPosition(m_noDirection.startingPoint);
			m_snake.setRelocation(getPosition(), m_well512);
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
	CCLOG("조준하기");
	setCumberState(getCumberState() | kCumberStateDirection);
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
		m_headImg->setRotation(rot); // 머리 따라감.
	}
	else if(m_direction.state == 2)
	{
		CCLOG("조준하는것을 멈추고 움직이기 시작함");
		if((getCumberState() & kCumberStateFury) == 0)
		{
			CCLOG("분노 아님!!");
			setCumberState(kCumberStateMoving); //#!
		}
		else
		{
			CCLOG("분노임~");
		}

		
		m_direction.state = 0;
		unschedule(schedule_selector(KSSnakeBase::animationDirection));
		m_headAnimationManager->runAnimationsForSequenceNamed("cast101stop");
		m_tailAnimationManager->runAnimationsForSequenceNamed("cast101stop");
	}
}
bool KSSnakeBase::startDamageReaction(float damage, float angle, bool castCancel, bool stiffen)
{
	KSCumberBase::startDamageReaction(damage, angle, castCancel, stiffen);
	m_remainHp -= damage;
	myGD->communication("UI_subBossLife", damage); //## 보스쪽에서 이걸 호출
	m_invisible.invisibleFrame = m_invisible.VISIBLE_FRAME; // 인비지블 풀어주는 쪽으로 유도.
	
	setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER)); // 맞으면 작게 함.
	m_attackCanceled = true;
	
	// 방사형으로 돌아가고 있는 중이라면
	if(((m_cumberState & kCumberStateNoDirection) || (m_cumberState & kCumberStateCasting)) && castCancel)
	{
		m_noDirection.state = 2; // 돌아가라고 상태 변경때림.
	}
	if(((m_cumberState & kCumberStateDirection) || (m_cumberState & kCumberStateCasting)) && castCancel)
	{
		m_direction.state = 2; // 돌아가라고 상태 변경때림.
	}
//	if((m_cumberState & kCumberStateMoving) == 0 && castCancel)
//	{
//		setCumberState(kCumberStateMoving;
//	}
//	if((m_cumberState & kCumberStateDirection) && stiffen)
//	{
//		CCLOG("(m_cumberState & kCumberStateDirection)");
//		m_direction.state = 2; // 돌아가라고 상태 변경때림.
//		setCumberState(kCumberStateMoving; //#!
//		
//	}
	if( ((m_cumberState & kCumberStateMoving) || (m_cumberState & kCumberStateDamaging)) && stiffen )
	{
		float rad = deg2Rad(angle);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLOG("%f %f", dx, dy);
		
		
		if(m_damageData.setStiffen(damage / getTotalHp() * 4.f))
		{
			setCumberState( m_cumberState | kCumberStateDamaging);
			schedule(schedule_selector(ThisClassType::damageReaction));
			if(damage / getTotalHp() * 4.f >= 3.f)
			{
				getEmotion()->goStun();
			}
			else
			{
				ProbSelector ps = {1.0, 1.0, 7.0};
				int r = ps.getResult();
				if(r == 0)
				{
					getEmotion()->toAnger();
				}
				else if(r == 1)
				{
					getEmotion()->toCry();
				}
			}
		} 
	}
	if((m_cumberState & kCumberStateFury) && castCancel)
	{
		crashMapForPosition(getPosition());
		m_castingCancelCount++;
		setCumberState(kCumberStateMoving);
		//		m_headImg->setColor(ccc3(255, 255, 255));
		myGD->communication("MS_resetRects", false);
		unschedule(schedule_selector(ThisClassType::furyModeScheduler));
		// 다시 벌겋게 만드는 코드.
		
		addChild(KSGradualValue<float>::create(m_furyMode.colorRef, 255, 0.5f,
																					 [=](float t)
																					 {
																						 KS::setColor(this, ccc3(255, t, t));
																					 }, nullptr));
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
	if(m_damageData.timer < m_damageData.stiffenSecond)
	{
		m_headImg->setColor(ccc3(255, 0, 0));
		m_tailImg->setColor(ccc3(255, 0, 0));
		for(auto i : m_Bodies)
		{
			i->setColor(ccc3(255, 0, 0));
		}
	}
	else // if(currentTimelineFooter == "")
	{
		m_headImg->setColor(ccc3(255, 255, 255));
		m_tailImg->setColor(ccc3(255, 255, 255));
		for(auto i : m_Bodies)
		{
			i->setColor(ccc3(255, 255, 255));
		}
		
//		setCumberState( m_cumberState | kCumberStateMoving);
		setCumberState( m_cumberState & ~kCumberStateDamaging);
//		m_cumberState &= ~kCumberStateDamaging;
		getEmotion()->releaseStun();
		unschedule(schedule_selector(KSSnakeBase::damageReaction));
		m_snake.setRelocation(getPosition(), m_well512);
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
void KSSnakeBase::runTimeline( Json::Value patternInfo )
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

void KSSnakeBase::completedAnimationSequenceNamed( const char *name_ )
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
			AudioEngine::sharedInstance()->stopEffect("se_castmap.mp3");
			//			AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
		}
		else if(m_atype == "special")
		{
			AudioEngine::sharedInstance()->stopEffect("se_castspecial.mp3");
			//			AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
		}
		else // normal
		{
			AudioEngine::sharedInstance()->stopEffect("se_castmissile.mp3");
			//			AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
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
		setCumberState(kCumberStateMoving);
		mAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
		myGD->communication("MS_resetRects", false);
	}
}
void KSSnakeBase::scaleAdjustment(float dt)
{
	m_scale.autoIncreaseTimer += 1/60.f;
	
	if(m_scale.increaseTime + 2.f < m_scale.autoIncreaseTimer && m_cumberState == kCumberStateMoving)
	{
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
	setCumberState(kCumberStateFury | kCumberStateMoving);
	m_noDirection.state = 2;	
	m_headImg->setColor(ccc3(0, 255, 0));
	m_tailImg->setColor(ccc3(0, 255, 0));
	for(auto i : m_Bodies)
	{
		i->setColor(ccc3(0, 255, 0));
	}
	
	schedule(schedule_selector(ThisClassType::furyModeScheduler));
	stopAnimationDirection();
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
		
		//setCumberState(kCumberStateMoving;
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

		setCumberState(kCumberStateMoving);
		//		m_headImg->setColor(ccc3(255, 255, 255));
		myGD->communication("MS_resetRects", false);
		unschedule(schedule_selector(ThisClassType::furyModeScheduler));
		// 다시 벌겋게 만드는 코드.

		addChild(KSGradualValue<float>::create(m_furyMode.colorRef, 255, 0.5f,
					[=](float t)
					{
					KS::setColor(this, ccc3(255, t, t));
					}, nullptr));
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

void KSSnakeBase::checkConfine(float dt)
{
	IntPoint mapPoint = m_mapPoint;
	// 갇혀있는지 검사함. 갇혀있으면 없앰.
	if(mapPoint.x - 1 >= 0 && mapPoint.y - 1 >= 0 &&
		 mapPoint.x + 1 < mapLoopRange::mapWidthOutlineEnd && mapPoint.y + 1 < mapLoopRange::mapHeightOutlineEnd &&
		 myGD->mapState[mapPoint.x][mapPoint.y] != mapEmpty &&
		 myGD->mapState[mapPoint.x-1][mapPoint.y] != mapEmpty &&
		 myGD->mapState[mapPoint.x+1][mapPoint.y] != mapEmpty &&
		 myGD->mapState[mapPoint.x][mapPoint.y-1] != mapEmpty &&
		 myGD->mapState[mapPoint.x][mapPoint.y+1] != mapEmpty
		 
		 /* &&
			
			dynamic_cast<KSJuniorBase*>(this)*/)
	{
		AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
		
		
		int rmCnt = 5;
		
		int weapon_level = mySGD->getSelectedCharacterHistory().level.getV();
		
		int weapon_rank = (weapon_level-1)/5 + 1;
		weapon_level = (weapon_level-1)%5 + 1;
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_s, mySGD->getSelectedCharacterHistory().characterIndex.getV());
		int weapon_type = mInfo.get("type", 0).asInt();
	
		myGD->createJackMissileWithStoneFunctor((StoneType)weapon_type, weapon_level, rmCnt, getPosition(), mySGD->getSelectedCharacterHistory().power.getV());
		
		//		string missile_code;
		//		missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
		//		int missile_type = MissileDamageData::getMissileType(missile_code.c_str());
		//
		//		//				myGD->communication("Main_goldGettingEffect", jackPosition, int((t_p - t_beforePercentage)/JM_CONDITION*myDSH->getGoldGetRate()));
		//		float missile_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
		//
		//		myGD->communication("MP_createJackMissile", missile_type, rmCnt, missile_speed, getPosition());
		
//		mySGD->increaseCatchCumber();
		myGD->communication("CP_removeMainCumber", this);
		
		removeFromParentAndCleanup(true);
		
		return;
	}
}


void KSSnakeBase::onStartMoving()
{
	setCumberState(kCumberStateMoving);
}

void KSSnakeBase::onStopMoving()
{
	setCumberState(kCumberStateNothing);
}

void KSSnakeBase::onPatternEnd()
{
	CCLOG("패턴의 끝");
	m_noDirection.state = 2;
	m_direction.state = 2; // 돌아가라고 상태 변경때림.
	if(m_direction.state == 1)
	{
//		setCumberState(kCumberStateMoving;
	}
	else if(m_cumberState & kCumberStateDirection)
	{
//		setCumberState(kCumberStateMoving;
	}
	// setCumberState(kCumberStateMoving; 만약 멈추면 이거 라인 부활시킴.
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
	if((m_cumberState & kCumberStateCasting))
	{
		m_noDirection.state = 2; // 돌아가라고 상태 변경때림.
		m_direction.state = 2; // 돌아가라고 상태 변경때림.
//		setCumberState(kCumberStateMoving;
	}
//		setCumberState(kCumberStateMoving; // 만약 멈춘 다면 여길 부활시킴.
}

void KSSnakeBase::setPosition( const CCPoint& t_sp )
{
	
	CCPoint prevPosition = getPosition();
	if(isnan(prevPosition.x))
	{
	}
	if(m_cumberState & kCumberStateDamaging)
	{
	}
	SnakeTrace tr;
	tr.position = t_sp;
	tr.directionRad = atan2f(t_sp.y - prevPosition.y, t_sp.x - prevPosition.x);
	tr.targeting = m_direction.state != 0;
	//		KSCumberBase::setPosition(t_sp);
	m_headImg->setPosition(t_sp);
	m_cumberTrace.push_back(tr); //
	if(m_cumberTrace.size() >= 1000)
	{
		m_cumberTrace.pop_front();
	}
	
	// 돌때랑 분노 모드일 땐 메인포인트 지정하면 안됨.
	if((m_cumberState & kCumberStateNoDirection) || (m_cumberState & kCumberStateFury))
	{
		// black hole!! 
	}
	else
	{
		myGD->setMainCumberPoint(this, ccp2ip(t_sp));
		m_mapPoint = ccp2ip(t_sp);
	}
//	myGD->setMainCumberPoint(this, ccp2ip(t_sp));
//	m_mapPoint = ccp2ip(t_sp);
	setHeadAndBodies();
	//		myGD->communication("Main_moveGamePosition", t_sp);
	//		myGD->communication("VS_setMoveGamePosition", t_sp);
	//		myGD->communication("Main_moveGamePosition", t_sp);
	//		myGD->communication("Main_moveGamePosition", t_sp);
}
void KSSnakeBase::setHeadAndBodies()
{
	SnakeTrace lastTrace = m_cumberTrace.back();
	//	float tt = rad2Deg( lastTrace.directionRad );
	//	CCLOG("deg %f", tt);
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
	//		CCLOG("snake position : %.2f, %.2f", m_headImg->getPositionX(), m_headImg->getPositionY());

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

void KSSnakeBase::attackBehavior( Json::Value _pattern )
{
	std::string pattern = _pattern["pattern"].asString();
	bool moving = _pattern.get("movingcast", false).asInt();
	if(moving)
	{
		setCumberState(kCumberStateMoving | kCumberStateCasting);
	}
	else
	{
		setCumberState(kCumberStateCasting);
	}
	
	if(pattern == "109")
	{
		startAnimationDirection();
//		setCumberState(0);
	}
	else if( pattern == "1007")
	{
		startAnimationDirection();
//		setCumberState(0);
	}
	else
	{
		m_headAnimationManager->runAnimationsForSequenceNamed("cast101start");
		m_tailAnimationManager->runAnimationsForSequenceNamed("cast101start");
		std::string target = _pattern.get("target", "no").asString();
		if( target == "yes") // 타게팅이라면 조준하라
			startAnimationDirection();
		else if(target == "no") // 타게팅이 아니면 돌아라
			startAnimationNoDirection();
		else
		{
			startAnimationNoDirection();
		} 
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
			float calc_distance = sqrtf((afterPoint.x - i)*(afterPoint.x - i) +
																	(afterPoint.y - j)*(afterPoint.y - j));
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

void KSSnakeBase::attachEmotion()
{
	CumberEmotion* ce = CumberEmotion::create();
	
	m_headImg->getParent()->addChild(ce, m_headImg->getZOrder() + 1);
//	m_headImg->addChild(ce, CumberZorder::kEmotion);
	m_emotion = ce;
	
	addChild(KSSchedule::create([=](float b)->bool
															{
																m_emotion->setPosition(m_headImg->getPosition());
																m_emotion->setScale(m_headImg->getScale());
																return true;
															}));
}
IntPoint KSSnakeBase::getMapPoint( CCPoint c )
{
	return IntPoint(round((c.x - 1) / pixelSize + 1.f),
		round((c.y - 1) / pixelSize + 1.f));
}




void KSSnakeBase::setGameover()
{
	KSCumberBase::setGameover();
	unschedule(schedule_selector(ThisClassType::scaleAdjustment));
	
	unschedule(schedule_selector(ThisClassType::speedAdjustment));
}





















































