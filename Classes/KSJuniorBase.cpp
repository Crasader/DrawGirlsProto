#include "KSJuniorBase.h"
#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
//#include "CumberEmotion.h"
#include <algorithm>
#include "StageImgLoader.h"
#include "DataStorageHub.h"
//#include "Jack.h"
//#include "PlayUI.h"
#include "MissileDamageData.h"
#include "StarGoldData.h"
#include "AchieveNoti.h"


bool KSJuniorBase::init(const string& ccbiName)
{
	KSCumberBase::init();
	
	
	////////////////////////////////////// by hs
	string ccbiname2 = ccbiName;
	if(ccbiName.substr(0,1)=="1") {
		ccbiname2="bear";
	}
	////////////////////////////////////////////
	
	m_directionAngleDegree = m_well512.GetValue(0, 360);
	
	
//	std::string _ccbiName = ("mob_" + ccbiname2 + ".ccbi").c_str();
	std::string _ccbiName = (ccbiname2 + ".ccbi").c_str();
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
//	CCNode* p = reader->readNodeGraphFromFile(_ccbiName.c_str(),this);
	CCNode* p = reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+_ccbiName).c_str(), this);
	m_headImg = dynamic_cast<CCSprite*>(p);
	
	KS::setPositionType(m_headImg, kCCPositionTypeGrouped);
	
	
	mAnimationManager = reader->getAnimationManager();
	m_headImg->addChild(mAnimationManager);
	reader->release();
	if(m_headImg != NULL) {
		this->addChild(m_headImg);
	}
	
	//	m_headImg = CCSprite::create("chapter10_boss.png");
	//	addChild(m_headImg);
	
		
	lastCastNum = m_well512.GetValue(1, 3);
	//	mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstart", lastCastNum)->getCString());

	
	schedule(schedule_selector(KSJuniorBase::scaleAdjustment), 1/60.f);
	schedule(schedule_selector(KSCumberBase::movingAndCrash));
	schedule(schedule_selector(KSCumberBase::followProcess), 1/5.f);

	
	return true;
}






bool KSJuniorBase::startDamageReaction(float damage, float angle, bool castCancel, bool stiffen)
{
	KSCumberBase::startDamageReaction(damage, angle, castCancel, stiffen);
	m_remainHp -= damage;
//	CCLOG("KSJuniorBase Hp %f", m_remainHp);
//	CCLOG("damaga!!!");
	// 방사형으로 돌아가고 있는 중이라면
	m_invisible.invisibleFrame = m_invisible.VISIBLE_FRAME; // 인비지블 풀어주는 쪽으로 유도.
	setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER)); // 맞으면 작게 함.
	

	if(((m_cumberState & kCumberStateMoving) || m_cumberState == kCumberStateDamaging) && stiffen)
	{
		float rad = deg2Rad(angle);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLOG("%f %f", dx, dy);
		
		
		if(m_damageData.setStiffen(damage / getTotalHp() * 4.f))
		{
			m_cumberState |= kCumberStateDamaging;
			schedule(schedule_selector(ThisClassType::damageReaction));
		}
	}
	
	
	
	if(m_remainHp <= 0)
	{
		AudioEngine::sharedInstance()->playEffect("se_subdie.mp3", false);
		
		
		
		//		removeFromParentAndCleanup(true);

		if(mySD->getClearCondition() == kCLEAR_subCumberCatch)
		{
			// 보스에서 부하 생성패턴이 있는지 살펴보고 없으면 강제로 넣어줌.
			/////////////////////////////// 다음 패턴 부하생성.
			if(0)
			{
				bool found2 = false;
				for(auto iter = myGD->getMainCumberVector().begin();
						iter != myGD->getMainCumberVector().end();
						++iter)
				{
					KSCumberBase* cumber = *iter;
					bool found = false;
					for(auto attackIter = cumber->getAttacks().begin();
							attackIter != cumber->getAttacks().end();
							++attackIter)
					{
						//					KS::KSLog("%", *attackIter);
						if( (*attackIter).get("pattern", "").asString() == "1020" )
						{
							cumber->getAttackQueue().push_back(*attackIter);
							found = true;
							break;
						}
					}
					if(found)
					{
						found2 = true;
						break;
					}
					//						attackIter = cumber->getAttackPattern().en
				}
				if(found2 == false)
				{
					if(myGD->getMainCumberVector().empty() == false)
					{
						Json::Reader reader;
						Json::Value createCumberPattern;
						reader.parse(R"({
												 "atype" : "special",
												 "childs" : 1,
												 "name" : "부하몹리젠",
												 "pattern" : 1020,
												 "percent" : 1,
												 "target" : "no"
												 } )", createCumberPattern);
						
						auto cumber = myGD->getMainCumberVector()[0];
						cumber->getAttackQueue().push_back(createCumberPattern);
					}
				}
			
			}
			setCumberState(m_cumberState & ~kCumberStateMoving);
			
			auto emotion = KS::loadCCBI<CCSprite*>(this, "emoticon_stun.ccbi");
			m_deadState = true;
			m_headImg->addChild(emotion.first, 10);
			emotion.first->setPositionY(emotion.first->getPositionY() + 28);
			emotion.first->setPositionX(15);
			emotion.first->setScale(1.5f);
			addChild(KSTimer::create(7.f, [=](){
				setCumberState(m_cumberState | kCumberStateMoving);
				m_remainHp = m_totalHp;
				m_deadState = false;
				emotion.first->removeFromParent();
			}));
		}
		else
		{
			mySGD->increaseCatchCumber();
			myGD->communication("CP_removeSubCumber", this);
			auto ret = KS::loadCCBI<CCSprite*>(this, "bossbomb1.ccbi");
			KS::setBlendFunc(ret.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
			
			CCPoint t = getPosition();
			ret.first->setPosition(t);
			addChild(ret.first, 11);
			
			scheduleOnce(schedule_selector(ThisClassType::removeFromParent), 1.f);
		}

		return true;
	}
	else
		return false;
}


void KSJuniorBase::checkConfine(float dt)
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
		
		mySGD->hunt_value = mySGD->hunt_value.getV() + 1;
		
		for(int i=kAchievementCode_hunter1;i<=kAchievementCode_hunter3;i++)
		{
			if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
			   mySGD->getUserdataAchieveHunter() + mySGD->hunt_value.getV() >= myAchieve->getCondition((AchievementCode)i))
			{
				myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		
		int rmCnt = 5;
		
		int weapon_level = mySGD->getUserdataCharLevel();
		
		int weapon_rank = (weapon_level-1)/5 + 1;
		weapon_level = (weapon_level-1)%5 + 1;
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), mySGD->getUserdataCharLevel());
		int weapon_type = mInfo.get("type", 0).asInt();
		
		double power_rate = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_power_d, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		if(power_rate < 1.0)
			power_rate = 1.0;
		
		myGD->createJackMissileWithStoneFunctor((StoneType)weapon_type, weapon_level, rmCnt, getPosition(), mySGD->getUserdataMissileInfoPower(), int((power_rate-1.0)*mySGD->getUserdataMissileInfoPower()));
		
//		string missile_code;
//		missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
//		int missile_type = MissileDamageData::getMissileType(missile_code.c_str());
//		
//		//				myGD->communication("Main_goldGettingEffect", jackPosition, int((t_p - t_beforePercentage)/JM_CONDITION*myDSH->getGoldGetRate()));
//		float missile_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
//		
//		myGD->communication("MP_createJackMissile", missile_type, rmCnt, missile_speed, getPosition());
		
		mySGD->increaseCatchCumber();
		myGD->communication("CP_removeSubCumber", this);
		
		
		if(mySD->getClearCondition() == kCLEAR_subCumberCatch)
		{
			removeFromParentAndCleanup(true);
			caughtAnimation();
		}
		else
		{
			removeFromParentAndCleanup(true);
		}
		return;
	}
}
void KSJuniorBase::startAnimationNoDirection()
{
	CCLOG("Lets rotate");
	if((m_cumberState & kCumberStateNoDirection) == 0)
	{
		m_cumberState |= kCumberStateNoDirection;
		m_noDirection.distance = 0;
		m_noDirection.rotationDeg = 0;
		m_noDirection.timer = 0;
		m_noDirection.startingPoint = getPosition();
		m_noDirection.rotationCnt = 0;
		m_noDirection.state = 1;
		schedule(schedule_selector(KSJuniorBase::animationNoDirection));
	}
}

void KSJuniorBase::damageReaction(float)
{
	m_damageData.timer += 1 / 60.f;
	if(m_damageData.timer < m_damageData.stiffenSecond)
	{
		//		m_headImg->setColor(ccc3(255, 0, 0)); //##
	}
	else
	{
		//		m_headImg->setColor(ccc3(255, 255, 255));
//		setCumberState( m_cumberState | kCumberStateMoving);
		setCumberState( m_cumberState & ~kCumberStateDamaging);
//		m_cumberState &= ~kCumberStateDamaging;
//		m_cumberState = kCumberStateMoving;
		unschedule(schedule_selector(KSJuniorBase::damageReaction));
		m_furyMode.furyFrameCount = m_furyMode.totalFrame;
		//		mAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
	}
}


void KSJuniorBase::animationNoDirection(float dt)
{
	m_noDirection.timer += 1.f/60.f;
	if(m_noDirection.state == 1)	
	{
		/// 좀 돌았으면 돌아감.
		//		if(m_noDirection.timer >= 5)
		//		{
		//			m_noDirection.state = 2;
		//			return;
		//		}
	}
	else if(m_noDirection.state == 2)
	{
		m_cumberState = kCumberStateMoving;
		m_noDirection.state = 0;
//		unschedule(schedule_selector(KSJuniorBase::animationNoDirection));
//		mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstop", lastCastNum)->getCString());
	}
}

void KSJuniorBase::onPatternEnd()
{
	CCLOG("onPatternEnd!!");
	m_noDirection.state = 2;
}

void KSJuniorBase::onStartGame()
{
	m_noDirection.state = 2;
	CCLOG("onStartGame!!");
}


void KSJuniorBase::cumberAttack(float dt)
{
	// 서브는 공격을 하지 않음... ㅋㅋ.
}COLLISION_CODE KSJuniorBase::crashLooper(const set<IntPoint>& v, IntPoint* cp)
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

void KSJuniorBase::startInvisible(int totalframe)
{
	//	if(!isScheduled(schedule_selector(KSCumber::invisibling)))
	if(m_invisible.startInvisibleScheduler == false)
	{
		m_invisible.VISIBLE_FRAME = totalframe;
		m_invisible.invisibleFrame = 0;
		m_invisible.invisibleValue = 0;
		schedule(schedule_selector(KSJuniorBase::invisibling));
		m_invisible.startInvisibleScheduler = true;
	}
}

void KSJuniorBase::invisibling(float dt)
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
void KSJuniorBase::randomPosition()
{
	IntPoint mapPoint;
	bool finded;
	getRandomPositionToJack(&mapPoint, &finded);
	
	
	setPosition(ip2ccp(mapPoint));
	m_circle.setRelocation(getPosition(), m_well512);
	m_snake.setRelocation(getPosition(), m_well512);
	CCScaleTo* t_scale = CCScaleTo::create(0.5f, 1.f); //##
	m_headImg->runAction(t_scale);
	
	
	lightSmaller();
	
}

void KSJuniorBase::crashMapForPosition(CCPoint targetPt)
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

void KSJuniorBase::furyModeOn(int tf)
{
	m_furyMode.startFury(tf);
	m_noDirection.state = 2;
	m_cumberState = kCumberStateFury | kCumberStateMoving;
	
	//	m_headImg->setColor(ccc3(0, 255, 0));
	
	schedule(schedule_selector(ThisClassType::furyModeScheduler));
}

void KSJuniorBase::furyModeScheduler(float dt)
{


	if(m_furyMode.furyFrameCount >= m_furyMode.totalFrame)
	{
		// 시간이 다되서 끝나는 조건.
		crashMapForPosition(getPosition());

		m_cumberState = kCumberStateMoving;
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
void KSJuniorBase::furyModeOff()
{
	//	myGD->communication("EP_stopCrashAction");
	myGD->communication("MS_resetRects", false);
}


void KSJuniorBase::scaleAdjustment(float dt)
{
	m_scale.autoIncreaseTimer += 1/60.f;
	
	if(m_scale.increaseTime + 2.f < m_scale.autoIncreaseTimer && (m_cumberState & kCumberStateNoDirection) == 0)
	{
		m_scale.increaseTime = m_scale.autoIncreaseTimer;
		setCumberScale(MIN(m_maxScale, getCumberScale() + m_scale.SCALE_ADDER));
	}
	
	m_scale.scale.step();
	
	m_headImg->setScale(getCumberScale());
	
}

void KSJuniorBase::onStartMoving()
{
	m_cumberState = kCumberStateMoving;
	schedule(schedule_selector(KSCumberBase::movingAndCrash));
}

void KSJuniorBase::onStopMoving()
{
	m_cumberState = kCumberStateNothing;
	CCLOG("%s %d kCumberStateStop", __FILE__, __LINE__);
}

void KSJuniorBase::setPosition( const CCPoint& t_sp )
{
	auto beforePosition = getPosition();
	
	if(t_sp.x - beforePosition.x >= 0)
	{
		// right
	}
	else
	{
//		m_headImg->setFlipX(true);
	}
	
	m_headImg->setPosition(t_sp);
	m_mapPoint = ccp2ip(t_sp);
}

void KSJuniorBase::setPositionX( float t_x )
{
	setPosition(ccp(t_x, getPositionY()));
}

void KSJuniorBase::setPositionY( float t_y )
{
	setPosition(ccp(getPositionX(), t_y));
}

const CCPoint& KSJuniorBase::getPosition()
{
	return m_headImg->getPosition();
}

CCPoint KSJuniorBase::getMissilePoint()
{
	return getPosition() + ccp(0, 0);
}

void KSJuniorBase::setScale( float scale )
{
	m_headImg->setScale(scale);
}

void KSJuniorBase::setScaleX( float x )
{
	m_headImg->setScaleX(x);
}

void KSJuniorBase::setScaleY( float y )
{
	m_headImg->setScaleY(y);
}

void KSJuniorBase::endTeleport()
{
	startMoving();
	myGD->communication("CP_onPatternEndOf", this);
}

void KSJuniorBase::startTeleport()
{
	smaller();
	AudioEngine::sharedInstance()->playEffect("sound_teleport.mp3",false);
}

void KSJuniorBase::smaller()
{
	CCBlink* t_scale = CCBlink::create(0.5, 8);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(KSJuniorBase::randomPosition));

	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);

	runAction(t_seq);
}

void KSJuniorBase::stopAnimationNoDirection()
{
	m_noDirection.state = 2;
}

COLLISION_CODE KSJuniorBase::getCrashCode( IntPoint point, IntPoint* checkPosition )
{
	IntPoint afterPoint = point;
	float half_distance = RADIUS*getCumberScale(); // 20.f : radius for base scale 1.f
	int ip_half_distance = half_distance / 2;
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
	return collisionCode;
}

float KSJuniorBase::getRadius()
{
	return RADIUS;
}



void KSJuniorBase::setGameover()
{
	KSCumberBase::setGameover();
	unschedule(schedule_selector(ThisClassType::scaleAdjustment));
	unschedule(schedule_selector(ThisClassType::speedAdjustment));
}

















