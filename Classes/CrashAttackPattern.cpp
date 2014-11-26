//
//  CrashAttackPattern.cpp
//  DGproto
//
//  Created by ksoo k on 2014. 11. 18..
//
//

#include "AttackPattern.h"
#include "SilhouetteData.h"
#include "MissileUnitCCB.h"


void FireWorkWrapper::myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
{
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	scheduleUpdate();
	
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	this->pattern = pattern;
}

void FireWorkWrapper::stopMyAction()
{
	unscheduleUpdate();
	setEndingWithEarly();
	
	//		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCFadeOut::create(0.5f)}));
	//		m_parentMissile->removeFromParentAndCleanup(true);
	startSelfRemoveSchedule();
}

void FireWorkWrapper::update( float dt )
{
	Firework* fw = Firework::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), pattern);
	addChild(fw);
	stopMyAction();
}



void MovingSunflowerWrapper::myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
{
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	scheduleUpdate();
	
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_pattern = pattern;
}

void MovingSunflowerWrapper::stopMyAction()
{
	unscheduleUpdate();
	setEndingWithEarly();
	
	//		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCFadeOut::create(0.5f)}));
	//		m_parentMissile->removeFromParentAndCleanup(true);
	startSelfRemoveSchedule();
}

void MovingSunflowerWrapper::update( float dt )
{
	MovingSunflower* ap = MovingSunflower::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), m_pattern);
	addChild(ap);
	stopMyAction();
}

Saw* Saw::create( CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize )
{
	Saw* t_m11 = new Saw();
	t_m11->myInit(t_sp, t_type, t_speed, t_mSize);
	t_m11->autorelease();
	return t_m11;
}

void Saw::stopMyAction()
{
	
	startSelfRemoveSchedule();
}

void Saw::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void Saw::myInit( CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize )
{
	//		myGD->communication("EP_startCrashAction");
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	
	CCPoint subPosition = ccpSub(jackPosition, t_sp);
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
	
	float throwAngle;
	
	if(distance < 200)			throwAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;// + (rand()%91-45)/2;
	else						throwAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;// + (rand()%31-15)/2;
	
	
	ThrowObject* t_to = ThrowObject::create(t_sp, t_type, t_speed, throwAngle, t_mSize);
	addChild(t_to);
	//t_to->startMyAction();
	stopMyAction();
}

ThunderBoltWrapper* ThunderBoltWrapper::create( CCPoint t_sp, KSCumberBase* cb, int t_type, int t_targetingFrame, int t_shootFrame, float speedr )
{
	ThunderBoltWrapper* t_m12 = new ThunderBoltWrapper();
	t_m12->myInit(t_sp, cb, t_type, t_targetingFrame, t_shootFrame, speedr);
	t_m12->autorelease();
	return t_m12;
}

void ThunderBoltWrapper::stopMyAction()
{
	unschedule(schedule_selector(ThunderBoltWrapper::myAction));
	
	
	setEndingWithEarly();
	startSelfRemoveSchedule();
	fadeFromToDuration.init(255, 0, 1.f);
	schedule(schedule_selector(ThisClassType::hidingAnimation));
}

void ThunderBoltWrapper::hidingAnimation( float dt )
{
	if(fadeFromToDuration.step(1.f/60.f) == false)
	{
		if(wifiImg)
		{
			wifiImg->removeFromParentAndCleanup(true);
			wifiImg = NULL;
			
		}
		if(targetingImg)
		{
			targetingImg->removeFromParentAndCleanup(true);
			targetingImg = NULL;
		}
		if(myBeam)
		{
			myBeam->removeFromParentAndCleanup(true);
			myBeam = NULL;
		}
	}
	else
	{
		KS::setOpacity(wifiImg, fadeFromToDuration.getValue());
		KS::setOpacity(targetingImg, fadeFromToDuration.getValue());
		KS::setOpacity(myBeam, fadeFromToDuration.getValue());
	}
}

void ThunderBoltWrapper::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void ThunderBoltWrapper::startMyAction()
{
	ingFrame = 0;
	schedule(schedule_selector(ThunderBoltWrapper::myAction), 1/30.f);
}

bool ThunderBoltWrapper::groundOfJack(int state)
{
	return state == mapType::mapOldline || state == mapType::mapOldget;
}
void ThunderBoltWrapper::myAction()
{
	ingFrame++;
	
	//	targetingImg->setRotation(targetingImg->getRotation() + 2);
	if(ingFrame <= targetingFrame)
	{
		targetingImg->setScale(targetingImg->getScale() - 2.f/targetingFrame);
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		
		bool found = false;
		for(const auto&  i : visitPoint)
		{
			if(i.equals(jackPosition))
			{
				found = true;
				break;
			}
		}
		if(!found) // 중복된건 ㄴㄴ 해
		{
			pJackArray.push_back(jackPosition);
			visitPoint.push_back(jackPosition);
		}
		
		if(ingFrame == targetingFrame)
		{
			myBeam = SatelliteBeam::create(pJackArray.front(), type, this, callfunc_selector(ThunderBoltWrapper::stopMyAction));
			addChild(myBeam);
		}
	}
	else if(ingFrame <= targetingFrame+shootFrame)
	{
		/////////////////// index 참조 방식.
		int adjIndex = ceilf(curIndex);
		curIndex += speedRatio;
		if(pJackArray.size() - 1 >= adjIndex)
		{
			CCPoint t_p = pJackArray[adjIndex];
			//			pJackArray.pop_front();
			int jack = 0;
			auto mapPoint = ccp2ip(t_p);
			if(groundOfJack(myGD->mapState[mapPoint.x-1][mapPoint.y]))	jack++;
			if(groundOfJack(myGD->mapState[mapPoint.x+1][mapPoint.y]))	jack++;
			if(groundOfJack(myGD->mapState[mapPoint.x][mapPoint.y-1]))	jack++;
			if(groundOfJack(myGD->mapState[mapPoint.x][mapPoint.y+1]))	jack++;
			if(groundOfJack(myGD->mapState[mapPoint.x+1][mapPoint.y+1]))	jack++;
			if(groundOfJack(myGD->mapState[mapPoint.x+1][mapPoint.y-1]))	jack++;
			if(groundOfJack(myGD->mapState[mapPoint.x-1][mapPoint.y+1]))	jack++;
			if(groundOfJack(myGD->mapState[mapPoint.x-1][mapPoint.y+1]))	jack++;
			
			if(jack == 8)
			{
				//		unschedule(schedule_selector(PoisonLine::myAction));
				stopMyAction();
				return;
			}
			targetingImg->setPosition(t_p);
			myBeam->beamSetPosition(t_p);
			
			
		}
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		bool found = false;
		for(const auto&  i : visitPoint)
		{
			if(i.equals(jackPosition))
			{
				found = true;
				break;
			}
		}
		if(!found) // 중복된건 ㄴㄴ 해
		{
			pJackArray.push_back(jackPosition);
			visitPoint.push_back(jackPosition);
		}
		
		
		if(ingFrame == targetingFrame+shootFrame)
		{
			wifiImg->removeFromParentAndCleanup(true);
			wifiImg = NULL;
		}
	}
	else
	{
		int adjIndex = ceilf(curIndex);
		curIndex += speedRatio;
		if(pJackArray.size() - 1 >= adjIndex)
		{
			
			CCPoint t_p = pJackArray[adjIndex];
			//			pJackArray.pop_front();
			
			targetingImg->setPosition(t_p);
			myBeam->beamSetPosition(t_p);
			
		}
		else
		{
			targetingImg->removeFromParentAndCleanup(true);
			targetingImg = NULL;
			myBeam->removeFromParentAndCleanup(true);
			myBeam = NULL;
			stopMyAction();
			
		}
	}
}

void ThunderBoltWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, int t_type, int t_targetingFrame, int t_shootFrame, float speedr )
{
	m_cumber = cb;
	speedRatio = speedr;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	curIndex = 0;
	type = t_type;
	targetingFrame = t_targetingFrame;
	shootFrame = t_shootFrame;
	
	//		myGD->communication("EP_startCrashAction");
	
	wifiImg = CCSprite::create("satelliteBeam_wifi.png", CCRectMake(0, 0, 30, 22));
	CCPoint wifiPosition = ccpAdd(t_sp, ccp(0,40));
	wifiImg->setPosition(wifiPosition);
	addChild(wifiImg);
	
	CCSprite* t_texture = CCSprite::create("satelliteBeam_wifi.png");
	
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.2);
	for(int i=0;i<5;i++)
	{
		t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*30, 0, 30, 22));
	}
	
	CCAnimate* t_animate = CCAnimate::create(t_animation);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
	
	wifiImg->runAction(t_repeat);
	
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
	
	//		auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_lightning_targeting.ccbi");
	auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_lightning_targeting.ccbi");
	//KS::setBlendFunc(ret.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	targetingImg = ret.first;
	
	targetingImg->setScale(2.7);
	
	targetingImg->setPosition(jackPosition);
	
	addChild(targetingImg);
	
	startMyAction();
}

BigSaw* BigSaw::create( CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize )
{
	BigSaw* t_m14 = new BigSaw();
	t_m14->myInit(t_sp, t_type, t_speed, t_tmCnt, t_mSize);
	t_m14->autorelease();
	return t_m14;
}

void BigSaw::stopMyAction()
{
	
	startSelfRemoveSchedule();
}

void BigSaw::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void BigSaw::myInit( CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize )
{
	//		myGD->communication("EP_startCrashAction");
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	
	CCPoint subPosition = ccpSub(jackPosition, t_sp);
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
	
	float baseAngle;
	
	if(distance < 200)			baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;// + rand()%91 - 45;
	else						baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;// + rand()%31 - 15;
	
	
	int left = t_tmCnt / 2;
	int right = t_tmCnt - left;
	int unitDegree = 20;
	for(int i=1; i<=left; i++)
	{
		float t_angle = baseAngle - i*unitDegree;
		if(t_type==11)t_angle=baseAngle;//105패턴은 조준사격
		if(t_angle >= 180)		t_angle -= 360;
		if(t_angle < -180)		t_angle += 360;
		
		ThrowObject* t_to = ThrowObject::create(t_sp, t_type, t_speed, t_angle, t_mSize);
		addChild(t_to);
		//t_to->startMyAction();
	}
	for (int i=0; i<right; i++) {
		float t_angle = baseAngle + i*unitDegree;
		if(t_type==11)t_angle=baseAngle; //105패턴은 조준사격
		if(t_angle >= 180)		t_angle -= 360;
		if(t_angle < -180)		t_angle += 360;
		
		ThrowObject* t_to = ThrowObject::create(t_sp, t_type, t_speed, t_angle, t_mSize);
		addChild(t_to);
		//t_to->startMyAction();
	}
	
	stopMyAction();
}

FlameWrapper* FlameWrapper::create( CCPoint t_sp, KSCumberBase* cb, int t_tmCnt, int t_burnFrame )
{
	FlameWrapper* t_m15 = new FlameWrapper();
	t_m15->myInit(t_sp, cb, t_tmCnt, t_burnFrame);
	t_m15->autorelease();
	return t_m15;
}

void FlameWrapper::stopMyAction()
{
	if(!is_remove_called)
	{
		is_remove_called = true;
		unschedule(schedule_selector(FlameWrapper::myAction));
		myParticle->setDuration(0);
		
		setEndingWithEarly();
		
		CCDelayTime* t_delay = CCDelayTime::create(1.2f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(FlameWrapper::particleRemove));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
		
		startSelfRemoveSchedule();
	}
}

void FlameWrapper::removeEffect()
{
	if(!is_remove_called)
	{
		is_remove_called = true;
		unschedule(schedule_selector(FlameWrapper::myAction));
		myParticle->setDuration(0);
		
		m_cumber->setAttackPattern(nullptr);
		myGD->communication("CP_onPatternEndOf", m_cumber);
		
		CCDelayTime* t_delay = CCDelayTime::create(1.2f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(FlameWrapper::selfRemove));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
	}
}

void FlameWrapper::selfRemove()
{
	particleRemove();
	m_cumber->setAttackPattern(nullptr);
	removeFromParentAndCleanup(true);
}

void FlameWrapper::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		m_cumber->setAttackPattern(nullptr);
		removeFromParentAndCleanup(true);
	}
}

void FlameWrapper::particleRemove()
{
	myParticle->removeFromParentAndCleanup(true);
}

void FlameWrapper::startMyAction()
{
	AudioEngine::sharedInstance()->playEffect("sound_fire_shot.mp3", false);
	ingFrame = 0;
	schedule(schedule_selector(FlameWrapper::myAction));
}

void FlameWrapper::myAction()
{
	ingFrame++;
	
	float shootAngle = minAngle + (maxAngle - minAngle + 0.f)/burnFrame*ingFrame;
	myParticle->setAngle(shootAngle);
	
	if(ingFrame%createBurnFrame == 0)
	{
		Burn* t_b = Burn::create(myPosition, baseDistance, shootAngle, mType,
														 this, callfunc_selector(FlameWrapper::removeEffect));
		addChild(t_b);
		t_b->startMyAction();
	}
	
	
	if(ingFrame >= burnFrame)
	{
		stopMyAction();
	}
}

void FlameWrapper::initParticle()
{
	myParticle = CCParticleSystemQuad::createWithTotalParticles(100);
	myParticle->setPositionType(kCCPositionTypeRelative);
	CCTexture2D* texture;
	if(mType == 1)
		texture = CCTextureCache::sharedTextureCache()->addImage("prison_fire.png");
	else
		texture = CCTextureCache::sharedTextureCache()->addImage("prison_ice.png");
	myParticle->setTexture(texture);
	myParticle->setEmissionRate(100.00);
	myParticle->setAngle(minAngle);						//
	myParticle->setAngleVar(0.0);
	myParticle->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	myParticle->setBlendFunc(blendFunc);
	myParticle->setDuration(-1.0);
	myParticle->setEmitterMode(kCCParticleModeGravity);
	ccColor4F startColor;
	if(mType == 1)
	{
		startColor.r = 1.0;
		startColor.g = 0.43;
		startColor.b = 0.08;
		startColor.a = 1.0;
	}
	else
	{
		startColor.r = 0.4;
		startColor.g = 0.8;
		startColor.b = 1.0;
		startColor.a = 1.0;
	}
	myParticle->setStartColor(startColor);
	ccColor4F startColorVar = {0,0,0,0};
	myParticle->setStartColorVar(startColorVar);
	ccColor4F endColor;
	if(mType == 1)
	{
		endColor.r = 1.0;
		endColor.g = 0.09;
		endColor.b = 0.05;
		endColor.a = 1.0;
	}
	else
	{
		endColor.r = 0;
		endColor.g = 0;
		endColor.b = 1.0;
		endColor.a = 1.0;
	}
	myParticle->setEndColor(endColor);
	ccColor4F endColorVar = {0,0,0,0};
	myParticle->setEndColorVar(endColorVar);
	myParticle->setStartSize(30.00);
	myParticle->setStartSizeVar(0.0);
	myParticle->setEndSize(50.0);
	myParticle->setEndSizeVar(10.0);
	myParticle->setGravity(ccp(0,0));
	myParticle->setRadialAccel(0.0);
	myParticle->setRadialAccelVar(0.0);
	myParticle->setSpeed(baseDistance);					//
	myParticle->setSpeedVar(20.0);						//
	myParticle->setTangentialAccel(0);
	myParticle->setTangentialAccelVar(0);
	myParticle->setTotalParticles(100);
	myParticle->setLife(1.00);
	myParticle->setLifeVar(0.3);
	myParticle->setStartSpin(0.0);
	myParticle->setStartSpinVar(0.0);
	myParticle->setEndSpin(0.0);
	myParticle->setEndSpinVar(0.0);
	myParticle->setPosition(myPosition);
	myParticle->setPosVar(CCPointZero);
	addChild(myParticle);
}

void FlameWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, int t_tmCnt, int t_burnFrame )
{
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	is_remove_called = false;
	
	mType = 1;
	
	myPosition = t_sp;
	tmCnt = t_tmCnt;
	burnFrame = t_burnFrame;
	createBurnFrame = burnFrame/tmCnt + 1;
	
	
	
	//		myGD->communication("EP_startCrashAction");
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	
	CCPoint subPosition = ccpSub(jackPosition, t_sp);
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
	
	if(distance < 200)
	{
		baseDistance = 200;
		baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%31 - 15;
	}
	else
	{
		baseDistance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
	}
	
	baseDistance *= 1.01;
	
	minAngle = baseAngle - 30;
	maxAngle = baseAngle + 30;
	
	initParticle();
	startMyAction();
}

MeteorWrapper* MeteorWrapper::create( int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea, float enableP, KSCumberBase* cb)
{
	MeteorWrapper* t_m16 = new MeteorWrapper();
	t_m16->myInit(t_type, t_tmCnt, t_totalFrame, t_crashArea, enableP, cb);
	t_m16->autorelease();
	return t_m16;
}

void MeteorWrapper::stopMyAction()
{
	if(!isRemoveEffect)
	{
		isRemoveEffect = true;
		unschedule(schedule_selector(MeteorWrapper::myAction));
		setEndingWithEarly();
		
		startSelfRemoveSchedule();
	}
}

//void MeteorWrapper::removeEffect()
//{
//	if(!isRemoveEffect)
//	{
//		isRemoveEffect = true;
//		unschedule(schedule_selector(MeteorWrapper::myAction));
//
//
//		startSelfRemoveSchedule();
//	}
//}

void MeteorWrapper::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void MeteorWrapper::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
		
		if(crashCount <= 100)
		{
			myGD->toScratch();
		}
	}
}

void MeteorWrapper::startMyAction()
{
	ingFrame = 0;
	schedule(schedule_selector(MeteorWrapper::myAction));
}

void MeteorWrapper::myAction()
{
	ingFrame++;
	
	if(ingFrame%mRate == 0)
	{
		CCPoint random_fp;
		int loopLimit = 0;
		// 적절한 위치를 찾을 때 까지.
		while(1)
		{
			loopLimit++;
			int screenHeight = (myGD->limited_step_top - myGD->limited_step_bottom)*pixelSize;
			random_fp.x = m_well512.GetValue(240);
			random_fp.y = m_well512.GetValue(myGD->getJackPoint().y * pixelSize - screenHeight / 2.f,
																			 myGD->getJackPoint().y * pixelSize + screenHeight / 2.f);
			
			IntPoint tempPoint = ccp2ip(random_fp);
			if(tempPoint.isInnerMap())
			{
				if(myGD->mapState[tempPoint.x][tempPoint.y] != mapType::mapOutline) // valid area
				{
					break;
				}
			}
			
			// 혹시나 모를 무한 루프를 대비해 적당히 찾다가 못찾으면 나옴.
			if(loopLimit >= 200)
			{
				break;
			}
		}
		
		CCPoint random_sp;
		random_sp.x = random_fp.x + 500;
		random_sp.y = random_fp.y + 500;
		
		FallMeteor* t_fm = FallMeteor::create(imgFilename, 1, CCSizeMake(crashArea, crashArea), random_sp, random_fp,
																					220, 20, IntSize(12, 12), enableProb,
																					this, callfunc_selector(MeteorWrapper::stopMyAction),
																					bind(&MeteorWrapper::accumCrashCount, this, std::placeholders::_1)); // imgSize, crashSize
		addChild(t_fm);
	}
	
	if(ingFrame >= totalFrame)
	{
		stopMyAction();
	}
}

void MeteorWrapper::myInit( int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea, float enableP, KSCumberBase* cb)
{
	m_cumber = cb;
	enableProb = enableP;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	isRemoveEffect = false;
	
	//		myGD->communication("EP_startCrashAction");
	crashArea = t_crashArea;
	type = t_type;
	tmCnt = t_tmCnt;
	totalFrame = t_totalFrame;
	mRate = totalFrame/tmCnt;
	
	if(type == 1) // stone meteor
	{
		imgFilename = "1.png";
	}
	else
	{
		imgFilename = "2.png";
	}
	imgFilename = "1.png"; // 불돌타입.
	startMyAction();
}
void MeteorWrapper::accumCrashCount(int n)
{
	crashCount += n;
}


void CaromWrapper::stopMyAction()
{
	unscheduleUpdate();
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}

void CaromWrapper::removeEffect()
{
	if(!isRemoveEffect)
	{
		isRemoveEffect = true;
		int loop_cnt = getChildrenCount();
		for(int i=0;i<loop_cnt;i++)
		{
			((ThreeCushion*)getChildren()->objectAtIndex(i))->removeEffect();
		}
	}
}

void CaromWrapper::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void CaromWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	isRemoveEffect = false;
	//		myGD->communication("EP_startCrashAction");
	
	//		CCPoint t_sp = cb->getPosition();
	
	Json::Reader reader;
	reader.parse(patternData, pattern);
	
	this->t_sp = t_sp;
	t_move_speed = pattern.get("speed", 200.0).asDouble() / 100.f;
	t_cushion_cnt = pattern.get("cushioncount", 4).asInt();
	t_is_big_bomb = pattern.get("big", false).asInt();
	t_tmCnt = pattern.get("number", 3).asInt();
	if(t_tmCnt<1)t_tmCnt=1;
	m_crashArea = pattern.get("area", 20).asInt();
	if(m_crashArea<1)m_crashArea=1;
	///////////////////////////////////////////
	
	scheduleUpdate();
}

void CaromWrapper::update( float dt )
{
	for(int i=0;i<t_tmCnt;i++)
	{
		// create
		ThreeCushion* t_tc = ThreeCushion::create(t_sp, t_move_speed, t_cushion_cnt, t_is_big_bomb, m_crashArea,
																							pattern.get("enableprob", 1.f).asFloat(),
																							this, callfunc_selector(ThisClassType::removeEffect));
		addChild(t_tc);
	}
	
	stopMyAction();
}


void SawWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	speed = pattern.get("speed", 150.f).asDouble() / 100.f;
	crashsize = pattern.get("crashsize", 20.f).asDouble();
	if(crashsize<1)crashsize=1;
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	scheduleUpdate();
}

void SawWrapper::update( float dt )
{
	IntPoint mainCumberPoint = ccp2ip(m_cumber->getPosition());
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	Saw* t_m11 = Saw::create(mainCumberPosition, 11, speed, IntSize(round(crashsize),round(crashsize)));
	addChild(t_m11);
	
	//		myGD->communication("CP_onPatternEndOf", m_cumber);
	
	stopMyAction();
}

void SawWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}

void SmallSawWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	number = pattern.get("number", 4).asInt();
	if(number<1)number=1;
	speed = pattern.get("speed", 200.0).asDouble() / 100.f;
	crashsize = pattern.get("crashsize", 10).asInt();
	if(crashsize<1)crashsize=1;
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	scheduleUpdate();
}

void SmallSawWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}

void SmallSawWrapper::update( float dt )
{
	IntPoint mainCumberPoint = myGD->getMainCumberPoint(m_cumber);
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	BigSaw* t_m14 = BigSaw::create(mainCumberPosition, 14, speed, number, IntSize(round(crashsize),round(crashsize)));
	addChild(t_m14);
	
	
	stopMyAction();
}

void CrashLazerWrapper::stopMyAction()
{
	unschedule(schedule_selector(ThisClassType::myAction));
	setEndingWithEarly();
	AudioEngine::sharedInstance()->stopEffect("se_destructionlaser_1.mp3");
	AudioEngine::sharedInstance()->stopEffect("se_destructionlaser_2.mp3");
	
	
	startSelfRemoveSchedule();
	fadeFromToDuration.init(1, 0, 0.2f);
	schedule(schedule_selector(ThisClassType::hidingAnimation));
	
	if(attacked && crashCount <= 30)
	{
		myGD->toScratch();
	}
	
}

void CrashLazerWrapper::hidingAnimation( float dt )
{
	if(fadeFromToDuration.step(1.f/60.f) == false)
	{
		if(lazer_sub)
		{
			lazer_sub->removeFromParentAndCleanup(true);
			lazer_sub = NULL;
			
		}
		if(lazer_main)
		{
			lazer_main->removeFromParentAndCleanup(true);
			lazer_main = NULL;
		}
		if(t_bead)
		{
			t_bead->removeFromParentAndCleanup(true);
			t_bead = NULL;
		}
	}
	else
	{
		//			KS::setOpacity(lazer_sub, fadeFromToDuration.getValue());
		if(lazer_main)
			lazer_main->setScaleY(fadeFromToDuration.getValue());
		if(t_bead)
			t_bead->setScaleY(fadeFromToDuration.getValue());
		//			KS::setOpacity(lazer_main, fadeFromToDuration.getValue());
		//			KS::setOpacity(t_bead, fadeFromToDuration.getValue());
	}
}

void CrashLazerWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	//	t_sp = ip2ccp(myGD->getMainCumberPoint(m_cumber));
	attacked = false;
	lazer_main = t_bead = NULL;
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	crashCount = 0;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	
	
	type = 1;
	createRingFrame = 10;
	chargeFrame = 120;
	crashFrame = pattern.get("crashframe", 180).asInt();
	m_crashSize = pattern.get("crashsize", 40).asInt();
	///////////////////////////////////////////////////////////////////
	sp = t_sp;
	dcolor = 255.f/chargeFrame;
	dscale = 0.7f/chargeFrame;
	IntPoint jackPoint = myGD->getJackPoint();
	jackPosition = ip2ccp(jackPoint);
	CCPoint subPosition = ccpSub(jackPosition, t_sp);
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
	
	if(distance < 200)			angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
	else						angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
	
	CCPoint beadPosition;
	beadPosition.x = 1;
	beadPosition.y = tanf(angle/180.f*M_PI);
	
	if((angle > 90.f && angle < 270.f) || angle < -90.f)
	{
		beadPosition = ccpMult(beadPosition, -1.f);
	}
	
	float div_value = sqrtf(powf(beadPosition.x, 2.f) + powf(beadPosition.y, 2.f));
	dv = ccpMult(beadPosition, 1.f/div_value);
	beadPosition = ccpMult(dv, 20.f);
	
	beadPosition = ccpAdd(beadPosition, t_sp);
	
	t_bead = CCSprite::create("lazer_bead.png");
	t_bead->setPosition(beadPosition);
	addChild(t_bead);
	
	lazer_sub = CCSprite::create("lazer_sub.png");
	lazer_sub->setAnchorPoint(ccp(0,0.5));
	lazer_sub->setRotation(-angle);
	
	CCPoint subP = ccpMult(dv, 5);
	subP = ccpAdd(beadPosition, subP);
	lazer_sub->setPosition(subP);
	addChild(lazer_sub);
	
	startMyAction();
	
	//	cb->stopAnimationDirection();
}

void CrashLazerWrapper::selfRemove()
{
	m_cumber->setAttackPattern(nullptr);
	removeFromParentAndCleanup(true);
}

void CrashLazerWrapper::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		m_cumber->setAttackPattern(nullptr);
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void CrashLazerWrapper::startMyAction()
{
	AudioEngine::sharedInstance()->playEffect("se_destructionlaser_1.mp3", true);
	//	AudioEngine::sharedInstance()->playEffect("sound_angle_beem.mp3", false);
	ingFrame = 0;
	schedule(schedule_selector(ThisClassType::myAction));
}

void CrashLazerWrapper::myAction()
{
	if(ingFrame <= chargeFrame)
	{
		ccColor3B tcolor = t_bead->getColor();
		tcolor.g -= dcolor;
		//		tcolor.b -= dcolor;
		t_bead->setColor(tcolor);
		lazer_sub->setScaleY(lazer_sub->getScaleY()-dscale);
		lazer_sub->setColor(tcolor);
		if(ingFrame%createRingFrame == 0)
		{
			int random_sp = rand()%21-10;
			CCPoint r_sp = ccpMult(dv, 60 + random_sp);
			r_sp = ccpAdd(sp, r_sp);
			CCPoint r_fp = ccpMult(dv, 20);
			r_fp = ccpAdd(sp, r_fp);
			
			int random_frame = rand()%20 + 20;
			float random_s = (rand()%3)/10.f;
			
			Lazer_Ring* t_lr = Lazer_Ring::create(angle, r_sp, r_fp, 1.f-random_s, 0.3f-random_s, random_frame, tcolor);
			addChild(t_lr);
		}
		
		
		if(ingFrame == chargeFrame)
		{
			AudioEngine::sharedInstance()->stopEffect("se_destructionlaser_1.mp3");
			AudioEngine::sharedInstance()->playEffect("se_destructionlaser_2.mp3", true);
			
			auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_laser1_head.ccbi");
			KS::setBlendFunc(ret.first, ccBlendFunc{GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
			lazer_main = ret.first;
			lazer_main->setScale(m_crashSize/8.f*0.7f);
			lazer_main->setAnchorPoint(ccp(0,0.5));
			lazer_main->setRotation(-angle);
			
			
			CCPoint mp = ccpMult(dv, 30);
			mp = ccpAdd(sp, mp);
			lazer_main->setPosition(mp);
			
			addChild(lazer_main);
			
			
			//			//레이저가 자연스럽게 시작하도록 붙여주는것
			//			{
			//			CCSprite* laser3 = CCSprite::create("pattern_laserpuple_back1.png");
			//			//laser3->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
			//			laser3->setPosition(ccp(25, 0));
			//			laser3->setScaleY(m_crashSize/12.f);
			//			lazer_main->addChild(laser3,-1);
			//			}
			//			{
			//				CCSprite* laser3 = CCSprite::create("pattern_laserpuple_back2.png");
			//				//laser3->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
			//				laser3->setPosition(ccp(20, 0));
			//				laser3->setScaleY(m_crashSize/12.f);
			//				lazer_main->addChild(laser3,10);
			//			}
			
			for(int i=1; i<25; i++)
			{
				auto ret2 = KS::loadCCBI<CCSprite*>(this, "pattern_laser1_body.ccbi");
				
				CCSprite* laser3 = ret2.first;
				//				laser3->setContentSize(lazer_main->getContentSize());
				KS::setBlendFunc(laser3, ccBlendFunc{GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
				laser3->setPosition(ccp(20+44 * i, 0));
				//laser3->setScaleY(m_crashSize/8.f);
				//laser3->setScaleY(2.f);
				lazer_main->addChild(laser3);
				//					prev = laser3;
			}
			
			
			
			CCPoint c_sp = ccpMult(dv, 30);
			c_sp = ccpAdd(sp, c_sp);
			//			c_sp = ccpAdd(ip2ccp(myGD->getMainCumberPoint(m_cumber)), c_sp);
			
			float t_scale = m_crashSize/30.f;
			
			crashRect = CCRectMake(0, (-m_crashSize + 10*t_scale), 460, (m_crashSize + 10*t_scale)); //x좌표에 -30추가, 무조건 1자로 깍도록
			//				crashRect = CCRectMake(0, -60/2 + 10, 460, +60/2 + 10);
			lineCrashMap(c_sp, angle, 460, 60);
			
			myGD->communication("MS_resetRects", false);
		}
	}
	else if(ingFrame <= chargeFrame+crashFrame)
	{
		attacked = true;
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		CCPoint t_jp = spinTransform(jackPosition, sp, angle);
		
		if(crashRect.containsPoint(t_jp))
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			//				stopMySchedule();
			//			AudioEngine::sharedInstance()->stopEffect("se_destructionlaser_2.mp3");
			stopMyAction();
		}
	}
	
	if(ingFrame >= chargeFrame+crashFrame)
	{
		//		AudioEngine::sharedInstance()->stopEffect("se_destructionlaser_2.mp3");
		stopMyAction();
	}
	ingFrame++;
	
	m_cumber->onTargetingJack(jackPosition);
}

void CrashLazerWrapper::lineCrashMap( CCPoint t_sp, float t_angle, int t_width, int t_height )
{
	int check_map[mapWidthOutlineEnd][mapHeightOutlineEnd] = {0,};
	
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			CCPoint t_tp = ccp((i-1)*pixelSize+1,(j-1)*pixelSize+1);
			CCPoint a_tp = spinTransform(t_tp, t_sp, t_angle);
			if(crashRect.containsPoint(a_tp) && (myGD->mapState[i][j] == mapOldget || myGD->mapState[i][j] == mapOldline
																					 || myGD->mapState[i][j] == mapNewline) && check_map[i][j] == 0)
			{
				if(a_tp.y < crashRect.origin.y + 1.5f || a_tp.y > crashRect.origin.y + crashRect.size.height - 1.5f)
				{
					for(int k=i-2;k<=i+2;k++)
					{
						for(int l=j-2;l<=j+2;l++)
						{
							if(0 <= k && k < mapWidthOutlineEnd &&
								 0 <= l && l < mapHeightOutlineEnd)
							{
								check_map[k][l] = 1;
							}
							
						}
					}
					
					//					crashMapForIntRect(IntRect(i-2, j-2, 5, 5));
				}
				else
				{
					check_map[i][j] = 1;
				}
				//					crashMapForIntPoint(IntPoint(i,j));
			}
		}
	}
	
	CCArray* rects = CCArray::createWithCapacity(256);
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(check_map[i][j] == 1)
			{
				IntMoveState start = IntMoveState(i, j, directionRightUp);
				
				IntPoint origin = IntPoint(start.origin.x, start.origin.y);
				IntSize size = IntSize(0, 0);
				
				bool isUpper = true;
				bool isRighter = true;
				queue<IntMoveState> loopArray;
				loopArray.push(start);
				
				queue<IntMoveState> nextLoopArray;
				
				while(!loopArray.empty())
				{
					if(isUpper)				size.height++;
					if(isRighter)			size.width++;
					
					bool upable = isUpper;
					bool rightable = isRighter;
					
					while(!loopArray.empty())
					{
						IntMoveState t_ms = loopArray.front();
						loopArray.pop();
						
						
						if(t_ms.direction == directionUp && !isUpper)
							continue;
						if(t_ms.direction == directionRight && !isRighter)
							continue;
						
						if(check_map[t_ms.origin.x][t_ms.origin.y] == 1)				check_map[t_ms.origin.x][t_ms.origin.y] = 2;
						
						if(t_ms.direction == directionUp)
						{
							if(isUpper)
							{
								IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
								if(n_msUp.origin.isInnerMap() && check_map[n_msUp.origin.x][n_msUp.origin.y] == 1)
									nextLoopArray.push(n_msUp);
								else		upable = false;
							}
						}
						else if(t_ms.direction == directionRight)
						{
							if(isRighter)
							{
								IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
								if(n_msRight.origin.isInnerMap() && check_map[n_msRight.origin.x][n_msRight.origin.y] == 1)
									nextLoopArray.push(n_msRight);
								else		rightable = false;
							}
						}
						else if(t_ms.direction == directionRightUp)
						{
							if(isUpper)
							{
								IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
								if(n_msUp.origin.isInnerMap() && check_map[n_msUp.origin.x][n_msUp.origin.y] == 1)
									nextLoopArray.push(n_msUp);
								else		upable = false;
							}
							
							if(isRighter)
							{
								IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
								if(n_msRight.origin.isInnerMap() && check_map[n_msRight.origin.x][n_msRight.origin.y] == 1)
									nextLoopArray.push(n_msRight);
								else		rightable = false;
							}
							
							if(upable && rightable)
							{
								IntMoveState n_msRightUp = IntMoveState(t_ms.origin.x+1, t_ms.origin.y+1, directionRightUp);
								if(n_msRightUp.origin.isInnerMap() && check_map[n_msRightUp.origin.x][n_msRightUp.origin.y] == 1)
									nextLoopArray.push(n_msRightUp);
								else		rightable = false;
							}
						}
					}
					
					isUpper = upable;
					isRighter = rightable;
					
					if(isUpper || isRighter)
					{
						while(!nextLoopArray.empty())
						{
							loopArray.push(nextLoopArray.front());
							nextLoopArray.pop();
						}
					}
				}
				
				IntRect* r_rect = new IntRect(origin.x, origin.y, size.width, size.height);
				r_rect->autorelease();
				
				rects->addObject(r_rect);
			}
		}
	}
	
	for(int i=0;i<rects->count();i++)
	{
		IntRect* t_rect = (IntRect*)rects->objectAtIndex(i);
		crashMapForIntRect(IntRect(t_rect->origin.x, t_rect->origin.y, t_rect->size.width, t_rect->size.height));
	}
}

int CrashLazerWrapper::crashMapForIntRect (IntRect t_r)
{
	IntPoint jackPoint = myGD->getJackPoint();
	
	bool is_die = false;
	
	myGD->communication("VS_divideRects", t_r);
	
	
	for(int i=t_r.origin.x;i<t_r.origin.x+t_r.size.width;i++)
	{
		for(int j=t_r.origin.y;j<t_r.origin.y+t_r.size.height;j++)
		{
			IntPoint t_p = IntPoint(i,j);
			if(t_p.isInnerMap() && (myGD->mapState[t_p.x][t_p.y] == mapOldline || myGD->mapState[t_p.x][t_p.y] == mapOldget))
			{
				myGD->mapState[t_p.x][t_p.y] = mapEmpty;
				myGD->communication("MFP_createNewFragment", t_p);
				crashCount++;
			}
			
			if(!is_die && t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
			{
				is_die = true;
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				stopMyAction();
			}
		}
	}
	
	for(int i=t_r.origin.x-1;i<=t_r.origin.x+t_r.size.width;i++)
	{
		if(myGD->mapState[i][t_r.origin.y+t_r.size.height] == mapOldget)		myGD->mapState[i][t_r.origin.y+t_r.size.height] = mapOldline;
		if(myGD->mapState[i][t_r.origin.y-1] == mapOldget)						myGD->mapState[i][t_r.origin.y-1] = mapOldline;
	}
	for(int j=t_r.origin.y;j<t_r.origin.y+t_r.size.height;j++)
	{
		if(myGD->mapState[t_r.origin.x-1][j] == mapOldget)						myGD->mapState[t_r.origin.x-1][j] = mapOldline;
		if(myGD->mapState[t_r.origin.x+t_r.size.width][j] == mapOldget)			myGD->mapState[t_r.origin.x+t_r.size.width][j] = mapOldline;
	}
	
	if(!is_die && jackPoint.x >= t_r.origin.x && jackPoint.x < t_r.origin.x+t_r.size.width && jackPoint.y >= t_r.origin.y && jackPoint.y < t_r.origin.y+t_r.size.height)
	{
		is_die = true;
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		stopMyAction();
	}
}

void CrashLazerWrapper::crashMapForIntPoint( IntPoint t_p )
{
	if(t_p.isInnerMap() && (myGD->mapState[t_p.x][t_p.y] == mapOldline || myGD->mapState[t_p.x][t_p.y] == mapOldget)) // just moment, only map crash
	{
		myGD->mapState[t_p.x][t_p.y] = mapEmpty;
		for(int k = -1;k<=1;k++)
		{
			for(int l = -1;l<=1;l++)
			{
				if(k == 0 && l == 0)	continue;
				if(myGD->mapState[t_p.x+k][t_p.y+l] == mapOldget)		myGD->mapState[t_p.x+k][t_p.y+l] = mapOldline;
			}
		}
		//			myGD->communication("EP_crashed");
		myGD->communication("MFP_createNewFragment", t_p);
		myGD->communication("VS_divideRect", t_p);
	}
	
	IntPoint jackPoint = myGD->getJackPoint();
	
	if(jackPoint.x == t_p.x && jackPoint.y == t_p.y)
	{
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		stopMyAction();
	}
	
	if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
	{
		//					myGD->communication("PM_pathChainBomb", t_p);
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		myGD->communication("Main_showLineDiePosition", t_p);
		stopMyAction();
	}
}

CCPoint CrashLazerWrapper::spinTransform( CCPoint t_tp, CCPoint t_bp, float t_angle ) /* jack, boss, angle */
{
	CCPoint a_tp = ccpSub(t_tp, t_bp);
	float b_angle = atan2f(a_tp.y, a_tp.x)/M_PI*180.f;
	float a_angle = b_angle - t_angle;
	
	if(a_angle >= 180.f)	a_angle -= 360.f;
	if(a_angle < -180.f)	a_angle += 360.f;
	
	float distance = sqrtf(powf(a_tp.x, 2.f) + powf(a_tp.y, 2.f));
	
	a_tp.x = 1;
	a_tp.y = tanf(a_angle/180.f*M_PI);
	
	float div_value = sqrtf(powf(a_tp.x, 2.f) + powf(a_tp.y, 2.f));
	
	if(a_angle > 90 || a_angle < -90)
		a_tp = ccpMult(a_tp, -1.f);
	
	a_tp = ccpMult(a_tp, 1.f/div_value);
	
	a_tp = ccpMult(a_tp, distance);
	
	return a_tp;
}

void CrashingRush::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	scheduleUpdate();
}

void CrashingRush::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}

void CrashingRush::update( float dt )
{
	
}

void ThrowBombWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	scheduleUpdate();
	schedule(schedule_selector(ThisClassType::targetTraceUpdate));
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	
	m_frame = 0;
	int number = pattern.get("number", 5).asInt();
	if(number<1)number=1;
	
	m_totalFrame =  number * 40;
	m_pattern = pattern;
	m_targetSprite = KS::loadCCBI<CCSprite*>(this, "target3.ccbi").first;
	addChild(m_targetSprite);
}

void ThrowBombWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	//		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCFadeOut::create(0.5f)}));
	//		m_parentMissile->removeFromParentAndCleanup(true);
	startSelfRemoveSchedule(1);
}

void ThrowBombWrapper::update( float dt )
{
	if(m_frame == m_totalFrame)
	{
		stopMyAction();
		m_targetSprite->setVisible(false);
	}else{
		m_frame++;
		if(m_frame % 40 == 0)
		{
			// 쏨~
			ThrowBomb* gun = ThrowBomb::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), m_pattern);
			addChild(gun);
		}
	}
	
}

void ThrowBombWrapper::targetTraceUpdate(float dt)
{
	m_targetSprite->setPosition(ip2ccp(myGD->getJackPoint()));
}
void ScarabWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();
	
	scheduleUpdate();
	
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	
	m_frame = 0;
	m_totalFrame = 100;
	m_pattern = pattern;
}

void ScarabWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	//		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCFadeOut::create(0.5f)}));
	//		m_parentMissile->removeFromParentAndCleanup(true);
	startSelfRemoveSchedule();
}

void ScarabWrapper::update( float dt )
{
	ReaverScarab* gun = ReaverScarab::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), m_pattern);
	addChild(gun);
	stopMyAction();
	
	
	//		m_frame++;
	//		if(m_frame % 20 == 0)
	//		{
	//			// 쏨~
	//			ThrowBomb* gun = ThrowBomb::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()));
	//			addChild(gun);
	//		}
	//		if(m_frame == m_totalFrame)
	//		{
	//			stopMyAction();
	//		}
}



void DynamiteWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void DynamiteWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);
	
	startSelfRemoveSchedule();
}

void DynamiteWrapper::update( float dt )
{
	
}


void RunDownSawWrapper::myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
{
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	Json::Reader reader;
	reader.parse(patternData, m_pattern);
	scheduleUpdate();
}

void RunDownSawWrapper::update(float dt)
{
	//float speed = m_pattern["speed"].asFloat() / 100.f;
	//float crashSize = m_pattern["crashsize"].asInt();
	//IntPoint mainCumberPoint = ccp2ip(m_cumber->getPosition());
	//CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	//Saw* t_m11 = Saw::create(mainCumberPosition, 11, speed, IntSize(round(crashSize),round(crashSize)));
	//addChild(t_m11);
	
	//stopMyAction();
	
	float speed = m_pattern["speed"].asFloat() / 100.f;
	float crashSize = m_pattern["crashsize"].asInt();
	int runDown = m_pattern["rundown"].asInt();
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
	
	CCPoint subPosition = ccpSub(jackPosition, m_cumber->getPosition());
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
	
	float throwAngle;
	
	if(distance < 200)			throwAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + (rand()%91-45)/2;
	else						throwAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + (rand()%31-15)/2;
	
	
	RunDownSaw* t_to = RunDownSaw::create
	( m_cumber->getPosition(), speed, throwAngle, IntSize(round(crashSize),round(crashSize)),
	 runDown);
	addChild(t_to);
	//t_to->startMyAction();
	stopMyAction();
}

void RunDownSawWrapper::stopMyAction()
{
	unscheduleUpdate();
	
	setEndingWithEarly();
	
	startSelfRemoveSchedule();
}


JunirBombWrapper* JunirBombWrapper::create(KSCumberBase* cumber, Json::Value param)
{
	JunirBombWrapper* t_m32 = new JunirBombWrapper();
	t_m32->myInit(cumber, param);
	t_m32->autorelease();
	return t_m32;
}

void JunirBombWrapper::myInit(KSCumberBase* cumber, Json::Value param)
{
	//	myGD->communication("CP_startTeleport", cumber);
	m_cumber = cumber;
	m_earlyRelease = true;
	setStartingWithEarly();
	
	setEndingWithEarly();
	startSelfRemoveSchedule();
	
	vector<KSCumberBase*> copyArray;
	for(auto i : myGD->getSubCumberVector())
	{
		copyArray.push_back(i);
	}
	
	for(auto i : copyArray)
	{
		i->selfBomb(param);
	}
}

