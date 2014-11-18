//
//  SpecialAttackPattern.cpp
//  DGproto
//
//  Created by ksoo k on 2014. 11. 18..
//
//


#include "AttackPattern.h"
#include "SilhouetteData.h"
#include "MissileUnitCCB.h"

FallingStoneWrapper* FallingStoneWrapper::create( KSCumberBase* cb, Json::Value pattern, CCSize mSize, int t_type )
{
	FallingStoneWrapper* t_m9 = new FallingStoneWrapper();
	t_m9->myInit(cb, pattern, mSize, t_type);
	t_m9->autorelease();
	return t_m9;
}

void FallingStoneWrapper::stopMyAction()
{
	AudioEngine::sharedInstance()->stopEffect("sound_rock_falling.mp3");
	unschedule(schedule_selector(FallingStoneWrapper::myAction));
	
	setEndingWithEarly();
	//	m_cumber->setAttackPattern(nullptr);
	//	myGD->communication("CP_onPatternEndOf", m_cumber);
	
	startSelfRemoveSchedule();
}

void FallingStoneWrapper::removeEffect()
{
	if(!isRemoveEffect)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_rock_falling.mp3");
		unschedule(schedule_selector(FallingStoneWrapper::myAction));
		
		m_cumber->setAttackPattern(nullptr);
		//		myGD->communication("CP_onPatternEndOf", m_cumber);
		
		CCDelayTime* t_delay = CCDelayTime::create(1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(FallingStoneWrapper::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
	}
}

void FallingStoneWrapper::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void FallingStoneWrapper::startMyAction()
{
	
	//	AudioEngine::sharedInstance()->playEffect("sound_rock_falling.mp3", true);
	ingFrame = 0;
	schedule(schedule_selector(FallingStoneWrapper::myAction));
}

void FallingStoneWrapper::myAction()
{
	ingFrame++;
	
	if(ingFrame%shootFrame == 0)
	{
		MissileUnit3* t_mu = MissileUnit3::create(type, distance, mSize, patternData.get("enableprob", 1.f).asFloat(), this, callfunc_selector(FallingStoneWrapper::removeEffect));
		addChild(t_mu);
	}
	
	if(ingFrame >= keepFrame)
	{
		stopMyAction();
	}
}

void FallingStoneWrapper::myInit( KSCumberBase* cb, Json::Value pattern, CCSize t_mSize, int t_type )
{
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	patternData = pattern;
	
	keepFrame = patternData.get("totalframe", 300).asInt();
	shootFrame = patternData.get("shootframe", 30).asInt();
	distance = patternData.get("speed", 250.f).asDouble() / 100.f;
	type = t_type;
	mSize = t_mSize;
	
	startMyAction();
}



TornadoWrapper* TornadoWrapper::create( CCPoint t_sp, int tf, int sc, KSCumberBase* cb )
{
	TornadoWrapper* t_m21 = new TornadoWrapper();
	t_m21->myInit(t_sp, tf, sc, cb);
	t_m21->autorelease();
	return t_m21;
}

void TornadoWrapper::myInit( CCPoint t_sp, int tf, int sc, KSCumberBase* cb )
{
	m_cumber = cb;
	m_earlyRelease = true;
	// BlindDrop 이 끝날 때 setEndingWithEarly(); 호출해야 하는데 일단 true 인 경우에 "반드시"
	// 호출할 필요는 없음
	setStartingWithEarly();
	int totalFrame = tf;
	float scale = sc;
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	
	BlindDrop* t_bd = BlindDrop::create(t_sp, jackPosition, 20, totalFrame, scale);
	addChild(t_bd);
	t_bd->startAction();
	
	startSelfRemoveSchedule();
}




SightOutAttack* SightOutAttack::create( int t_frame, KSCumberBase* cb )
{
	SightOutAttack* t_m24 = new SightOutAttack();
	t_m24->myInit(t_frame, cb);
	t_m24->autorelease();
	return t_m24;
}

void SightOutAttack::updateSightOut()
{
	ingFrame = 0;
	
	SightOut* t_so = SightOut::create(sightOutFrame);
	t_so->setPosition(ccp(160,215));
	addChild(t_so);
	t_so->startAction();
	
	m_earlyRelease = true;
	setStartingWithEarly();
}

void SightOutAttack::startFrame()
{
	ingFrame = 0;
	schedule(schedule_selector(SightOutAttack::framing));
}

void SightOutAttack::framing()
{
	ingFrame++;
	
	if(ingFrame >= sightOutFrame)
	{
		stopFrame();
	}
}

void SightOutAttack::stopFrame()
{
	unschedule(schedule_selector(SightOutAttack::framing));
	
	m_cumber->setSightOutAttack(nullptr);
	startSelfRemoveSchedule();
}

void SightOutAttack::myInit( int t_frame, KSCumberBase* cb )
{
	sightOutFrame = t_frame;
	
	m_cumber = cb;
	
	m_earlyRelease = true;
	setStartingWithEarly();
	
	SightOut* t_so = SightOut::create(sightOutFrame);
	t_so->setPosition(ccp(160,215));
	addChild(t_so);
	t_so->startAction();
	
	startFrame();
}

FreezeAttack* FreezeAttack::create( int t_frame, KSCumberBase* cb )
{
	FreezeAttack* t_m26 = new FreezeAttack();
	t_m26->myInit(t_frame, cb);
	t_m26->autorelease();
	return t_m26;
}

FreezeAttack::~FreezeAttack()
{
	myGD->communication("Main_touchOn");
}
void FreezeAttack::updateFreeze()
{
	myGD->communication("Jack_createFog");
	ingFrame = 0;
	m_earlyRelease = true;
	setStartingWithEarly();
	m_cumber->setFreezeAttack(this);
}

void FreezeAttack::stopFrame()
{
	unschedule(schedule_selector(FreezeAttack::framing));
	m_cumber->setFreezeAttack(nullptr);
	removeFromParentAndCleanup(true);
}

void FreezeAttack::startFrame()
{
	AudioEngine::sharedInstance()->playEffect("se_sun.mp3");
	ingFrame = 0;
	schedule(schedule_selector(FreezeAttack::framing));
}

void FreezeAttack::framing()
{
	ingFrame++;
	
	if(ingFrame >= freezingFrame)
	{
		myGD->communication("Main_touchOn");
		stopFrame();
	}
}

void FreezeAttack::myInit( int t_frame, KSCumberBase* cb )
{
	freezingFrame = t_frame;
	
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	myGD->communication("Jack_createFog");
	startFrame();
}

TeleportWrapper* TeleportWrapper::create(KSCumberBase* cumber)
{
	TeleportWrapper* t_m32 = new TeleportWrapper();
	t_m32->myInit(cumber);
	t_m32->autorelease();
	return t_m32;
}

void TeleportWrapper::myInit(KSCumberBase* cumber)
{
	myGD->communication("CP_startTeleport", cumber);
	startSelfRemoveSchedule();
}

ChaosAttack* ChaosAttack::create( int t_frame, KSCumberBase* cb )
{
	ChaosAttack* t_m33 = new ChaosAttack();
	t_m33->myInit(t_frame, cb);
	t_m33->autorelease();
	return t_m33;
}
ChaosAttack::~ChaosAttack()
{
	m_cumber->setChaosAttack(nullptr);
	myGD->communication("Jack_reverseOff");
}
void ChaosAttack::updateChaos()
{
	myGD->communication("Jack_createChaos");
	ingFrame = 0;
	m_earlyRelease = true;
	setStartingWithEarly();
	m_cumber->setChaosAttack(this);
}

void ChaosAttack::stopFrame()
{
	unschedule(schedule_selector(ChaosAttack::framing));
	m_cumber->setChaosAttack(nullptr);
	removeFromParent();
	myGD->communication("Jack_reverseOff");
	//	startSelfRemoveSchedule();
}

void ChaosAttack::startFrame()
{
	ingFrame = 0;
	schedule(schedule_selector(ChaosAttack::framing));
}

void ChaosAttack::framing()
{
	ingFrame++;
	
	if(ingFrame >= chaosFrame)
	{
		
		stopFrame();
	}
}

void ChaosAttack::myInit( int t_frame, KSCumberBase* cb )
{
	chaosFrame = t_frame;
	
	m_cumber = cb;
	myGD->communication("Jack_createChaos");
	startFrame();
	m_earlyRelease = true;
	setStartingWithEarly();
}

void FreezingWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void FreezingWrapper::update( float dt )
{
	
}

void FreezingWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);
	
	startSelfRemoveSchedule();
}

void ChaosWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void ChaosWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);
	
	startSelfRemoveSchedule();
}

void ChaosWrapper::update( float dt )
{
	
}



void LazerScanWrapper::stopMyAction()
{
	unschedule(schedule_selector(ThisClassType::myAction));
	if (beamImg)
	{
		beamImg->removeFromParentAndCleanup(true);
		beamImg = 0;
	}
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}

void LazerScanWrapper::removeEffect()
{
	unschedule(schedule_selector(ThisClassType::myAction));
	
	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);
	
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ThisClassType::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	if(beamImg)
		beamImg->runAction(t_seq);
}

void LazerScanWrapper::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void LazerScanWrapper::myAction()
{
	ingFrame++;
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	CCPoint subPosition = ccpSub(jackPosition, startPosition);
	
	if(myGD->getJackState())
	{
		float jackAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180;
		float beforeAngle = -beamImg->getRotation();
		float afterAngle = beamBaseAngle-10 + 2*ingFrame;
		
		if(beforeAngle <= afterAngle && jackAngle >= beforeAngle && jackAngle <= afterAngle)
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			unschedule(schedule_selector(ThisClassType::myAction));
			removeEffect();
		}
		else if(beforeAngle > afterAngle && (jackAngle >= beforeAngle || jackAngle <= afterAngle))
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			unschedule(schedule_selector(ThisClassType::myAction));
			removeEffect();
		}
	}
	
	if(beamImg)
		beamImg->setRotation(-(beamBaseAngle-10 + 2*ingFrame));
	
	
	if(ingFrame >= totalFrame)
	{
		//			beamImg->removeFromParentAndCleanup(true);
		stopMyAction();
	}
}

void LazerScanWrapper::startMyAction()
{
	ingFrame = 0;
	schedule(schedule_selector(ThisClassType::myAction));
}

void LazerScanWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
	
	totalFrame = pattern.get("totalframe", 10).asInt();
	
	type = 1;
	startPosition = t_sp;
	
	if(type == 1) // lazer
	{
		beamImg = CCSprite::create("beam.png");
		//KS::setBlendFunc(beamImg, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		beamImg->setAnchorPoint(ccp(0,0.5));
		beamImg->setPosition(startPosition);
		beamImg->setColor(ccRED);
	}
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
	
	CCPoint subPosition = ccpSub(jackPosition, startPosition);
	beamBaseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
	beamImg->setRotation(-(beamBaseAngle-10));
	
	addChild(beamImg);
	
	startMyAction();
}

void LazerScanWrapper::update( float dt )
{
	
}

void RadioactivityWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	area = pattern.get("area", 10).asInt();
	totalFrame = pattern.get("totalframe", 300).asInt();
	movingFrame = pattern.get("movingframe", 80).asInt();
	m_position = t_sp;
	m_cumber = cb;
	m_earlyRelease = false;
	enableRatio = pattern.get("enableratio", 1.f).asFloat();
	setStartingWithEarly();
	
	scheduleUpdate();
}

void RadioactivityWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}

void RadioactivityWrapper::update( float dt )
{
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	
	PoisonDrop* t_pd = PoisonDrop::create(m_position, jackPosition, movingFrame, area, totalFrame, enableRatio);
	addChild(t_pd);
	t_pd->startAction();
	
	stopMyAction();
}



void WindMillWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void WindMillWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);
	
	startSelfRemoveSchedule();
}

void WindMillWrapper::update( float dt )
{
	
}



AlongOfTheLineWrapper::~AlongOfTheLineWrapper()
{
	CCLOG("remove s 17");
}

void AlongOfTheLineWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	m_totalFrame = pattern.get("totalframe", 180).asInt();
	m_speed = pattern.get("linespeed", 100).asInt();
	m_number = pattern.get("number", 4).asInt();
	if(m_number<1)m_number=1;
	scheduleUpdate();
}

void AlongOfTheLineWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}

void AlongOfTheLineWrapper::update( float dt )
{
	AlongOfTheLine* ap = AlongOfTheLine::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), m_totalFrame, m_number, m_speed);
	addChild(ap);
	stopMyAction();
}

void CloudWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_pattern = pattern;
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	scheduleUpdate();
}

void CloudWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}

void CloudWrapper::update( float dt )
{
	int loop = m_well512.GetValue(2, 4);
	for(int i=0; i<loop; i++)
	{
		int x = m_well512.GetValue(mapLoopRange::mapWidthInnerBegin, mapLoopRange::mapWidthInnerEnd - 1);
		int y = m_well512.GetValue(mapLoopRange::mapHeightInnerBegin, mapLoopRange::mapHeightInnerEnd - 1);
		
		//			CloudBomb* ap = CloudBomb::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()));
		CloudBomb* ap = CloudBomb::create(ip2ccp(IntPoint(x, y)), ip2ccp(myGD->getJackPoint()), m_pattern);
		
		addChild(ap);
	}
	
	stopMyAction();
}

void PutChildWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_pattern = pattern;
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	
	IntPoint mapPoint;
	bool finded;
	cb->getRandomPositionToJack(&mapPoint, &finded);
	
	auto grang = KS::loadCCBI<CCSprite*>(this, "summons_01.ccbi").first;
	addChild(grang);
	grang->setPosition(ip2ccp(mapPoint));
	stopMyAction();
	addChild(KSTimer::create(1.5f, [=](){
		addChild(KSSchedule::create([=](float dt){
			Json::Reader reader;
			Json::Value root;
			reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "junior"), root);
			
			// 기본값으로 서버에서 설정된 부하몹 개수로 함.
			int n = MIN(m_pattern.get("maxchilds", root.size()).asInt() - myGD->getSubCumberCount(),
									m_pattern.get("childs", 1).asInt());
			n = MAX(n, 1);
			for(int i=0; i<n; ++i)
			{
				myGD->communication("CP_createSubCumber", mapPoint);
			}
			//			m_cumber->setAttackPattern(nullptr);
			//			myGD->communication("CP_onPatternEndOf", m_cumber);
			removeFromParent();
			
			return false; // 한번만 실행
		}));
	}));
}

void PutChildWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	startSelfRemoveSchedule();
}

void PutChildWrapper::update( float dt )
{
}
void KSSpecialAttackPattern19::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern19::stopMyAction()
{
	unscheduleUpdate();
	
	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);
	
	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern19::update( float dt )
{
	
}

void KSSpecialAttackPattern20::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern20::stopMyAction()
{
	unscheduleUpdate();
	
	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);
	
	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern20::update( float dt )
{
	
}

void KSSpecialAttackPattern21::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern21::stopMyAction()
{
	unscheduleUpdate();
	
	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);
	
	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern21::update( float dt )
{
	
}

CobWeb* CobWeb::create( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	CobWeb* t_m23 = new CobWeb();
	t_m23->myInit(t_sp, cb, patternData);
	t_m23->autorelease();
	return t_m23;
}
CobWeb::~CobWeb()
{
	if(state == kInner)
	{
		myGD->setAlphaSpeed(myGD->getAlphaSpeed()+decreaseSpeed);
	}
	m_cumber->setCobWebAttack(nullptr);
}
void CobWeb::updateCobWeb()
{
	if(!is_stop)
		ingFrame = 0;
	m_earlyRelease = true;
	setStartingWithEarly();
}

void CobWeb::startFrame()
{
	AudioEngine::sharedInstance()->playEffect("se_slowzone.mp3");
	ingFrame = 0;
	schedule(schedule_selector(CobWeb::framing));
}

void CobWeb::framing()
{
	ingFrame++;
	
	m_scaleFromTo.step(1/60.f);
	cobwebImg->setScale(m_scaleFromTo.getValue());
	if(ingFrame < slowFrame)
	{
		CCPoint cobwebPosition = cobwebImg->getPosition();
		CCPoint jackPosition = ip2ccp(myGD->getJackPoint());
		if(state == kElse)
		{
			if(cobwebPosition.x - 100 <= jackPosition.x && jackPosition.x <= cobwebPosition.x + 100 &&
				 cobwebPosition.y - 100 <= jackPosition.y && jackPosition.y <= cobwebPosition.y + 100)
			{
				//					CCLOG("-0.5");
				myGD->setAlphaSpeed(myGD->getAlphaSpeed() - decreaseSpeed);
				state = kInner;
			}
			else
			{
				state = kOuter;
			}
			
		}
		else if(state == kInner || state == kOuter)
		{
			if(cobwebPosition.x - 100 <= jackPosition.x && jackPosition.x <= cobwebPosition.x + 100 &&
				 cobwebPosition.y - 100 <= jackPosition.y && jackPosition.y <= cobwebPosition.y + 100)
			{
				// 밖에 있다가 들어옴.
				if(state == kOuter)
				{
					myGD->setAlphaSpeed(myGD->getAlphaSpeed() - decreaseSpeed);
					CCLOG("-0.5");
					state = kInner;
				}
			}
			else // 밖
			{
				// 안에 있다가 나갔으면
				if(state == kInner)
				{
					myGD->setAlphaSpeed(myGD->getAlphaSpeed() + decreaseSpeed);
					CCLOG("+0.5");
					state = kOuter;
				}
			}
		}
	}
	else if(ingFrame >= slowFrame)
	{
		stopFrame();
	}
}

void CobWeb::removeCobWeb()
{
	cobwebImg->removeFromParent();
	startSelfRemoveSchedule();
}

void CobWeb::stopFrame()
{
	is_stop = true;
	unschedule(schedule_selector(CobWeb::framing));
	
	cobwebImg->stopAllActions();
	
	CCScaleTo* t_scale = CCScaleTo::create(0.3, 0.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CobWeb::removeCobWeb));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	cobwebImg->runAction(t_seq);
	
	//	if(state == kInner)
	//	{
	//		myGD->setAlphaSpeed(myGD->getAlphaSpeed()+decreaseSpeed);
	//	}
	//	m_cumber->setCobWebAttack(nullptr);
	//	state = kFrameTerminated;
}

void CobWeb::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	state = kElse;
	is_stop = false;
	Json::Reader reader;
	reader.parse(patternData, pattern);
	KS::KSLog("%", pattern);
	slowFrame = pattern.get("totalframe", 60*4).asInt();
	decreaseSpeed = pattern.get("decrease", 0.5f).asFloat();
	decreaseSpeed = 1.f;
	
	//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	//	CCBReader* reader1 = new CCBReader(nodeLoader);
	//	cobwebImg = dynamic_cast<CCSprite*>(reader1->readNodeGraphFromFile("pattern_slowzone.ccbi",this));
	//	//KS::setBlendFunc(cobwebImg, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	//	reader1->release();
	cobwebImg = KS::loadCCBI<CCSprite*>(this, "pattern_slowzone.ccbi").first;
	
	cobwebImg->setPosition(m_cumber->getPosition());
	cobwebImg->setScale(0.f);
	
	addChild(cobwebImg);
	m_scaleFromTo.init(0.0f, 1.0f, 0.3f);
	//		CCScaleTo* t_scale = CCScaleTo::create(0.3, 1.f);
	//		cobwebImg->runAction(t_scale); // 나중에 수동으로 구현해야함.
	
	//		myGD->setAlphaSpeed(myGD->getAlphaSpeed()-0.5f);
	
	startFrame();
}

PrisonPattern* PrisonPattern::create( CCPoint t_sp, float radius, int totalFrame, KSCumberBase* cb )
{
	PrisonPattern* t_m28 = new PrisonPattern();
	t_m28->myInit(t_sp, radius, totalFrame, cb);
	t_m28->autorelease();
	return t_m28;
}

void PrisonPattern::startMyAction()
{
	m_frameCount = 0;
	schedule(schedule_selector(PrisonPattern::myAction));
}

void PrisonPattern::hidingAnimation( float dt )
{
	if(m_fadeFromToDuration.step(1.f/60.f) == false)
	{
		m_prisonSprite->removeFromParent();
	}
	else
	{
		KS::setOpacity(m_prisonSprite, m_fadeFromToDuration.getValue());
	}
}

void PrisonPattern::myAction()
{
	m_frameCount++;
	// 종료조건
	if(m_frameCount >= m_totalFrame)
	{
		stopMyAction();
	}
	else
	{
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ip2ccp(jackPoint);
		// initialJackPosition 와 jackPosition 거리가 radius 이상이면 죽임.
		if(ccpLength(m_initialJackPosition - jackPosition) >= m_radius)
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			stopMyAction();
		}
	}
	
	// PrisonObject
}

void PrisonPattern::stopMyAction()
{
	//		m_prisonSprite->removeFromParent();
	m_fadeFromToDuration.init(255, 0.f, 1.f);
	schedule(schedule_selector(PrisonPattern::hidingAnimation));
	unschedule(schedule_selector(PrisonPattern::myAction));
	startSelfRemoveSchedule();
	setEndingWithEarly();
}

void PrisonPattern::myInit( CCPoint t_sp, float radius, int totalFrame, KSCumberBase* cb ) /* create 0.5 second */
{
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	IntPoint jackPoint = myGD->getJackPoint();
	m_initialJackPosition = ip2ccp(jackPoint);
	m_radius = radius;
	m_totalFrame = totalFrame;
	auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_prison.ccbi");
	//KS::setBlendFunc(ret.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	m_prisonSprite = ret.first;
	m_prisonSprite->setPosition(m_initialJackPosition);
	AudioEngine::sharedInstance()->playEffect("se_prison.mp3");
	addChild(m_prisonSprite);
}




void GodOfDeath::myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
{
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	Json::Reader reader;
	reader.parse(patternData, m_pattern);
	scheduleUpdate();
	
	
	m_followFrames = 60;
	m_followSpeed = 1.5f;
	m_alpha = 5;
	m_isFollow = false;
	m_frameCount = 0;
	auto pair = KS::loadCCBI<CCSprite*>(this, "pattern_god.ccbi");
	m_godOfDeathSprite = pair.first;
	m_manager = pair.second;
	//	 = CCSprite::create("godofdeath.png");
	m_godOfDeathSprite->setPosition(m_cumber->getPosition());
	addChild(m_godOfDeathSprite);
	m_killed = false;
}

void GodOfDeath::update(float dt)
{
	CCLOG("char distance : %f", ccpLength( myGD->getJackPoint().convertToCCP() - m_godOfDeathSprite->getPosition() ));
	
	if(ccpLength( myGD->getJackPoint().convertToCCP() - m_godOfDeathSprite->getPosition()) <= 5
		 && myGD->getCommunicationBool("Jack_isDie") == false && m_killed == false)
	{
		m_killed = true;
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		m_manager->runAnimationsForSequenceNamed("die");
		unschedule(schedule_selector(ThisClassType::update));
		addChild(KSTimer::create(1.5f, [=](){
			stopMyAction();
		}));
	}
	m_frameCount++;
	if(m_isFollow)
	{
		if(m_frameCount >= m_followFrames + m_alpha)
		{
			m_isFollow = false;
			m_frameCount = 0;
		}
		
		auto deather = m_godOfDeathSprite->getPosition();
		auto jackPosition = myGD->getJackPoint().convertToCCP();
		float angle = atan2f(jackPosition.y - deather.y, jackPosition.x - deather.x);
		m_godOfDeathSprite->setPosition(m_godOfDeathSprite->getPosition() + ccp(m_followSpeed * cosf(angle), m_followSpeed * sinf(angle)));
		if(cosf(angle) < 0)
		{
			KS::setFlipX(m_godOfDeathSprite, false);
			//			m_godOfDeathSprite->setFlipX(false);
		}
		else
		{
			KS::setFlipX(m_godOfDeathSprite, true);
			//			m_godOfDeathSprite->setFlipX(true);
		}
	}
	else
	{
		if(m_frameCount >= m_followFrames * 4 - m_alpha)
		{
			m_frameCount = 0;
			m_alpha += 5;
			m_isFollow = true;
		}
		
	}
}

void GodOfDeath::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}

void MeshWrapper::myInit(CCPoint t_sp, KSCumberBase* cb, const Json::Value& patternJson)
{
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	m_pattern = patternJson;
	scheduleUpdate();
	
	
	// j 프레임동안 x 가 몇개 나올것이냐. 각 패턴의 딜레이를 설정가능함...
	// patternJson = {"duration" : 180, "hcount" : 4, "delay" : 120}
	m_currentFrames = 0;
}

void MeshWrapper::update(float dt)
{
	m_currentFrames++;
	
	int vInterval = m_pattern.get("duration", 180).asFloat() / m_pattern.get("vcount", 4).asFloat();
	if(m_currentFrames % vInterval == 0)
	{
		Json::Value param;
		param["x"] = ks19937::getIntValue(0, mapWidthInnerEnd * 2);
		param["delayframes"] = m_pattern.get("delay", 120).asInt();
		param["enableprob"] = m_pattern.get("enableprob", 1.f).asFloat();
		VMesh* t = VMesh::create(param);
		addChild(t);
		CCLOG("VMESH create");
	}
	
	int hInterval = m_pattern.get("duration", 180).asFloat() / m_pattern.get("hcount", 4).asFloat();
	if(m_currentFrames % hInterval == 0)
	{
		Json::Value param;
		param["y"] = ks19937::getIntValue(0, mapHeightInnerEnd * 2);
		param["delayframes"] = m_pattern.get("delay", 120).asInt();
		param["enableprob"] = m_pattern.get("enableprob", 1.f).asFloat();
		HMesh* t = HMesh::create(param);
		addChild(t);
		CCLOG("HMESH create");
		
	}
	
	if(m_currentFrames >= m_pattern.get("duration", 180).asInt())
	{
		stopMyAction();
	}
}

void MeshWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}




WiperMissileWrapper* WiperMissileWrapper::create(KSCumberBase* cumber, Json::Value param)
{
	WiperMissileWrapper* t_m32 = new WiperMissileWrapper();
	t_m32->myInit(cumber, param);
	t_m32->autorelease();
	return t_m32;
}

void WiperMissileWrapper::myInit(KSCumberBase* cumber, Json::Value param)
{
	//	myGD->communication("CP_startTeleport", cumber);
	m_cumber = cumber;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", param.get("color", 1).asInt())->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);
	
	//	batchNode->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	addChild(batchNode);
	
	
	float theta = atan2f(myGD->getJackPointCCP().y - cumber->getPosition().y, myGD->getJackPointCCP().x - cumber->getPosition().x) ;
	float originalTheta = theta;
	theta += M_PI / 2.f;
	int number = param.get("number", 4).asInt();
	float margin = param.get("margin", 1.f).asFloat();
	float speed = param.get("speed", 100.f).asFloat() / 100.f;
	
	if(number % 2 == 0)
	{
		float startNumber = -(number / 2 - 0.5f) * margin;
		
		for(int i=0; i<number; ++i)
		{
			CCPoint mPoint = cumber->getPosition() + ccp(startNumber * cosf(theta), startNumber * sinf(theta));
			///////////////////////////////////////
			MissileUnit* t_mu2 = MissileUnit::create(mPoint, rad2Deg(originalTheta), speed,
																							 fileName.c_str(), CCSizeMake(4, 4),0, 0, true);
			t_mu2->setLineTouch(false); // 선 터치는 안하도록함.
			batchNode->addChild(t_mu2);
			t_mu2->setOpacity(255);
			
			startNumber += margin;
			
			float tempProb = param.get("enableprob", 1.f).asFloat();
			ProbSelector ps = {tempProb, 1.f - tempProb};
			if(ps.getResult() == 1)
			{
				t_mu2->setOpacity(100);
				t_mu2->setEnabled(false);
			}
		}
	}
	else
	{
		float startNumber = -((int)(number / 2));
		for(int i=0; i<number; ++i)
		{
			CCPoint mPoint = cumber->getPosition() + ccp(startNumber * cosf(theta), startNumber * sinf(theta));
			///////////////////////////////////////
			MissileUnit* t_mu2 = MissileUnit::create(mPoint, rad2Deg(originalTheta), speed,
																							 fileName.c_str(), CCSizeMake(4, 4),0, 0, true);
			t_mu2->setLineTouch(false); // 선 터치는 안하도록함.
			batchNode->addChild(t_mu2);
			t_mu2->setOpacity(255);
			
			startNumber += margin;
			
			float tempProb = param.get("enableprob", 1.f).asFloat();
			ProbSelector ps = {tempProb, 1.f - tempProb};
			if(ps.getResult() == 1)
			{
				t_mu2->setOpacity(100);
				t_mu2->setEnabled(false);
			}
			
		}
	}
	////////////////
	
	//////////////////
	
	stopMyAction();
}


void WiperMissileWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
	AudioEngine::sharedInstance()->stopEffect("se_missile.mp3");
}

/////////////
HideCloudWrapper* HideCloudWrapper::create(KSCumberBase* cumber, Json::Value param)
{
	HideCloudWrapper* t_m32 = new HideCloudWrapper();
	t_m32->myInit(cumber, param);
	t_m32->autorelease();
	return t_m32;
}

void HideCloudWrapper::myInit(KSCumberBase* cumber, Json::Value param)
{
	//	myGD->communication("CP_startTeleport", cumber);
	m_cumber = cumber;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	
	////////////////
	// number, speed, duration
	//////////////////
	int number = param.get("number", 3).asInt();
	for(int i=0; i<number; i++)
	{
		//		float tempProb = param.get("enableprob", 1.f).asFloat();
		//		ProbSelector ps = {tempProb, 1.f - tempProb};
		//		if(ps.getResult() == 0)
		{
			Json::Value tParam;
			tParam["speed"] = param.get("speed", 100.f).asFloat() / 100.f;
			tParam["duration"] = param.get("duration", 400).asInt();
			tParam["enableprob"] = param.get("enableprob", 1.f).asFloat();
			HideCloud* hc = HideCloud::create(cumber, tParam);
			addChild(hc);
		}
	}
	
	stopMyAction();
}


void HideCloudWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
	//	AudioEngine::sharedInstance()->stopEffect("se_missile.mp3");
}
///////////////////////////////////////////
JellyWrapper* JellyWrapper::create(KSCumberBase* cumber, Json::Value param)
{
	JellyWrapper* t_m32 = new JellyWrapper();
	t_m32->myInit(cumber, param);
	t_m32->autorelease();
	return t_m32;
}

void JellyWrapper::myInit(KSCumberBase* cumber, Json::Value param)
{
	//	myGD->communication("CP_startTeleport", cumber);
	m_cumber = cumber;
	m_earlyRelease = true;
	setStartingWithEarly();
	////////////////
	// number, speed, duration
	//////////////////
	stopMyAction();
}


void JellyWrapper::stopMyAction()
{
	
	unscheduleUpdate();
	setEndingWithEarly();
	startSelfRemoveSchedule();
	//	AudioEngine::sharedInstance()->stopEffect("se_missile.mp3");
}

