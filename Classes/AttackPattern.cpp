//
//  AttackPattern.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 25..
//
//
#include "AttackPattern.h"
#include "SilhouetteData.h"
#include "MissileUnitCCB.h"
void CommonBulletPattern::myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
{
	m_cumber = cb;
	
	m_earlyRelease = false;
	setStartingWithEarly();
	//		m_position = t_sp;
	//		firstJackPosition = ip2ccp(myGD->getJackPoint());
	
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_oneShotNumber = pattern["oneshot"].asInt();
	m_oneShotTerm = pattern["oneshotterm"].asInt();
	m_gunNumber = pattern["gunnumber"].asInt();
	m_targetingType = (TargetType)pattern["targettype"].asInt();
	m_rotationDegreeVelocity = pattern["degreev"].asInt();
	m_color = pattern["color"].asInt();
	m_totalDegree = pattern["totaldegree"].asInt();
	m_totalFrame = pattern["totalframe"].asInt(); // 200 프레임 동안
	m_randomDegree = pattern["randomdegree"].asInt(); // 랜덤각.
	m_isSuper = pattern.get("super", false).asBool(); // ["issuper"].asBool();
	
	
	KS::KSLog("%", pattern);
	m_isCurve = pattern.get("curve", false).asInt();
	
	Json::Value path = pattern["path"];
	m_havePath = !path.empty();
	
	for(int index = 0; index < path.size(); index += 2)
	{
		float x = path[index].asFloat();
		float y = path[index + 1].asFloat();
		
		m_path.push_back(ccp(x, y));
	}
	m_frame = 0;
	
	for(int i=0; i<m_gunNumber; i++)
	{
		Gun g;
		g.bulletSpeed = pattern.get("speed", 180).asInt() / 100.f;
		g.fireTerm = pattern.get("fireterm", 1).asInt();
		g.fireCount = 0;
		g.idleCount = 0;
		pan.push_back(g);
	}
	
	initGuns();
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);
		
//	batchNode->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	addChild(batchNode);
	
	missileNode = CCNode::create();
	addChild(missileNode);
	
	AudioEngine::sharedInstance()->playEffect("se_missile.mp3", true);
	scheduleUpdate();
}


void CommonBulletPattern::update(float dt)
{
	m_frame++;
	// 중간각 기준으로 onTargetingJack 호출해야하는 작업을 아침에 해야함!
	
	
	if(m_frame >= m_totalFrame)
	{
		// 종료 조건
		stopMyAction();
	}
	else
	{
		
		if(m_targetingType == kCCW)
		{
			for(auto& gun : pan)
			{
				gun.degree.init(gun.degree.getValue() - m_rotationDegreeVelocity,
												gun.degree.getValue() - m_rotationDegreeVelocity,
												0);
				
			}
		}
		else if(m_targetingType == kCW)
		{
			for(auto& gun : pan)
			{
				gun.degree.init(gun.degree.getValue() + m_rotationDegreeVelocity,
												gun.degree.getValue() + m_rotationDegreeVelocity,
												0);
				
				
			}
		}
		else
		{
			for(auto& gun : pan)
			{
				CCPoint jackPoint = ip2ccp(myGD->getJackPoint());
				CCPoint mobPosition = ip2ccp(myGD->getMainCumberPoint(m_cumber));
				float rad = atan2(jackPoint.y - mobPosition.y, jackPoint.x - mobPosition.x);
				
				float deg = rad2Deg(rad);
				deg += m_well512.GetFloatValue(-m_randomDegree, +m_randomDegree);
				gun.degree.init(gun.degree.getValue(), gun.initDegree + deg - m_totalDegree / 2.f, m_rotationDegreeVelocity);
				
				//					if(gun.degree.getValue() >= 360)
				//					{
				//						gun.degree.init(gun.degree.getValue() - 360, gun.degree.getValue() - 360, m_rotationDegreeVelocity);
				//					}
				//					if(gun.degree.getValue() < 0)
				//					{
				//						gun.degree.init(gun.degree.getValue() + 360, gun.degree.getValue() + 360, m_rotationDegreeVelocity);
				//					}
				
				gun.degree.step();
				m_cumber->onTargetingJack(jackPoint);
//				CCLOG("gun degree %f", gun.degree.getValue());
			}
		}
		for(auto& gun : pan)
		{
			if(m_frame % gun.fireTerm == 0)
			{
				if(m_oneShotNumber > gun.fireCount)
				{
					// 빵.
					std::string imgFileName;
					std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
					if(KS::isExistFile(fileName))
						imgFileName = fileName;
					else
						imgFileName = "cumber_missile1.png";
					CCSize t_mSize = CCSize(4.f, 4.f);
					//						MissileUnit* t_mu = MissileUnit::create(ip2ccp(myGD->getMainCumberPoint()), gun.degree.getValue(), gun.bulletSpeed,
					//																																					imgFileName.c_str(), t_mSize, 0, 0);
					
					if(m_havePath)
					{
						MathmaticalMissileUnit* t_mu = MathmaticalMissileUnit::create(ip2ccp(myGD->getMainCumberPoint(m_cumber)), gun.degree.getValue(), gun.bulletSpeed,
																					  imgFileName.c_str(), t_mSize,
																					  m_path, m_isCurve ? MathmaticalMissileUnit::CURVE : MathmaticalMissileUnit::RIGHTLINE,
																																					m_isSuper);
						batchNode->addChild(t_mu);
						t_mu->setOpacity(200);
					}
					else
					{
//						MissileUnitCCB* t_mu = MissileUnitCCB::create(ip2ccp(myGD->getMainCumberPoint(m_cumber)), gun.degree.getValue(), gun.bulletSpeed,
//																																					"ingame_missile.ccbi", t_mSize,0, 0);
//						missileNode->addChild(t_mu);
//						t_mu->setScale(0.8f);
						MissileUnit* t_mu2 = MissileUnit::create(ip2ccp(myGD->getMainCumberPoint(m_cumber)), gun.degree.getValue(), gun.bulletSpeed,
																																					imgFileName.c_str(), t_mSize,0, 0, m_isSuper);
						batchNode->addChild(t_mu2);
						t_mu2->setOpacity(200);
					}
					gun.fireCount++;
				}
				else
				{
					if(gun.idleCount >= m_oneShotTerm)
					{
						// 다 쉬었다.
						//,, 중간각을 기준으로
						
						gun.fireCount = 0;
						gun.idleCount = 0;
					}
					gun.idleCount++;
				}
			}
		}
	}
}

void CommonBulletPattern::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0 && missileNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void CommonBulletPattern::stopMyAction()
{
	unscheduleUpdate();

	setEndingWithEarly();

	startSelfRemoveSchedule();
	AudioEngine::sharedInstance()->stopEffect("se_missile.mp3");
}

void CommonBulletPattern::initGuns()
{
	// 여기서 총의 개수와 m_totalDegree 를 통해 총을 배치함.

	int counter = 0;
	float termDegree;
	if(m_totalDegree == 360)
		termDegree = m_totalDegree / (m_gunNumber);
	else
		termDegree = m_totalDegree / (m_gunNumber - 1);
	if(pan.size() == 1)
	{
		pan[0].degree.init(0, 0, 0);
	}
	else for(auto iter = pan.begin(); iter != pan.end(); ++iter, ++counter)
	{
		iter->degree.init(counter * termDegree, counter * termDegree, 0);
		iter->initDegree = counter * termDegree;
	}

	if(m_targetingType == kToUser)
	{
		for(auto& gun : pan)
		{
			CCPoint jackPoint = ip2ccp(myGD->getJackPoint());
			CCPoint mobPosition = ip2ccp(myGD->getMainCumberPoint(m_cumber));
			float rad = atan2(jackPoint.y - mobPosition.y, jackPoint.x - mobPosition.x);

			float deg = rad2Deg(rad);
			deg += m_well512.GetFloatValue(-m_randomDegree, +m_randomDegree);
			gun.degree.init(gun.degree.getValue() + deg - m_totalDegree / 2.f, gun.degree.getValue() + deg - m_totalDegree / 2.f, 0);
			//				if(gun.degree.getValue() >= 360)
			//				{
			//					gun.degree.init(gun.degree.getValue() - 360, gun.degree.getValue() - 360, 0);
			//				}
			//				if(gun.degree.getValue() < 0)
			//				{
			//					gun.degree.init(gun.degree.getValue() + 360, gun.degree.getValue() + 360, 0);
			//				}
		}
	}

	// 전체를 조금씩 회전 시킴.
}




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






void AttackPattern::startSelfRemoveSchedule(int baseChild)
{
	m_baseChildCount = baseChild;
	schedule(schedule_selector(AttackPattern::selfRemoveSchedule));
}

void AttackPattern::selfRemoveSchedule()
{
	if(getChildrenCount() == m_baseChildCount)
	{
		removeFromParentAndCleanup(true);
		CCLOG("self remove!!");
	}
}

SelfSpinMissile* SelfSpinMissile::create( CCPoint t_sp, CCPoint t_dv, int t_mCnt, float t_r, string imgFilename, CCSize t_mSize )
{
	SelfSpinMissile* t_ssm = new SelfSpinMissile();
	t_ssm->myInit(t_sp, t_dv, t_mCnt, t_r, imgFilename, t_mSize);
	t_ssm->autorelease();
	return t_ssm;
}

void SelfSpinMissile::startMove()
{
	schedule(schedule_selector(SelfSpinMissile::move));
}

void SelfSpinMissile::stopMove()
{
	unschedule(schedule_selector(SelfSpinMissile::move));
}

void SelfSpinMissile::move()
{
	CCPoint r_p = getPosition();

	r_p = ccpAdd(r_p, dv);

	setPosition(r_p);

	if(r_p.x < 0.f - r || r_p.x > 320.f + r || r_p.y < -60.f - r || r_p.y > 490.f + r)
	{
		stopMove();
		removeFromParentAndCleanup(true);
		return;
	}
}

void SelfSpinMissile::myInit( CCPoint t_sp, CCPoint t_dv, int t_mCnt, float t_r, string imgFilename, CCSize t_mSize )
{
	setPosition(t_sp);
	dv = t_dv;
	r = t_r;

	int start_angle = rand()%(360/t_mCnt);

	int random_value = rand()%7 - 3;

	for(int i=0;i<t_mCnt;i++)
	{
		float temp_angle = start_angle+(360.f/t_mCnt)*i;

		CCPoint t_position;
		t_position.x = 1;
		t_position.y = tanf(temp_angle/180.f*M_PI);

		float div_value = sqrtf(powf(t_position.x, 2.f) + powf(t_position.y, 2.f));
		t_position = ccpMult(t_position, 1.f/div_value);

		if(temp_angle >= 90 && temp_angle < 270)		t_position = ccpMult(t_position, -1.f);

		t_position = ccpMult(t_position, t_r);

		MissileUnit* t_mu = MissileUnit::create(t_position, temp_angle+90.f, 1.f, imgFilename, t_mSize, 6.28f+random_value, 1.f);
		addChild(t_mu);
	}

	startMove();
}


FallingStoneWrapper* FallingStoneWrapper::create( int t_keepFrame, KSCumberBase* cb, int t_shootFrame, float t_distance, CCSize mSize, int t_type )
{
	FallingStoneWrapper* t_m9 = new FallingStoneWrapper();
	t_m9->myInit(t_keepFrame, cb, t_shootFrame, t_distance, mSize, t_type);
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
		MissileUnit3* t_mu = MissileUnit3::create(type, distance, mSize, this, callfunc_selector(FallingStoneWrapper::removeEffect));
		addChild(t_mu);
	}

	if(ingFrame >= keepFrame)
	{
		stopMyAction();
	}
}

void FallingStoneWrapper::myInit( int t_keepFrame, KSCumberBase* cb, int t_shootFrame, float t_distance, CCSize t_mSize, int t_type )
{
	m_cumber = cb;
	m_earlyRelease = true;
	setStartingWithEarly();

	keepFrame = t_keepFrame;
	shootFrame = t_shootFrame;
	distance = t_distance;
	type = t_type;
	mSize = t_mSize;

	startMyAction();
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

ThunderBoltWrapper* ThunderBoltWrapper::create( CCPoint t_sp, KSCumberBase* cb, int t_type, int t_targetingFrame, int t_shootFrame )
{
	ThunderBoltWrapper* t_m12 = new ThunderBoltWrapper();
	t_m12->myInit(t_sp, cb, t_type, t_targetingFrame, t_shootFrame);
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

void ThunderBoltWrapper::myAction()
{
	ingFrame++;

	targetingImg->setRotation(targetingImg->getRotation() + 2);
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
		if(pJackArray.empty() == false)
		{
			CCPoint t_p = pJackArray.front();
			pJackArray.pop_front();

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
		if(pJackArray.empty())
		{
			targetingImg->removeFromParentAndCleanup(true);
			targetingImg = NULL;
			myBeam->removeFromParentAndCleanup(true);
			myBeam = NULL;
			stopMyAction();
		}
		else
		{
			CCPoint t_p = pJackArray.front();
			pJackArray.pop_front();

			targetingImg->setPosition(t_p);
			myBeam->beamSetPosition(t_p);
		}
	}
}

void ThunderBoltWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, int t_type, int t_targetingFrame, int t_shootFrame )
{
	m_cumber = cb;
	m_earlyRelease = false;
	setStartingWithEarly();

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

MeteorWrapper* MeteorWrapper::create( int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea, KSCumberBase* cb)
{
	MeteorWrapper* t_m16 = new MeteorWrapper();
	t_m16->myInit(t_type, t_tmCnt, t_totalFrame, t_crashArea, cb);
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
																					220, 20, IntSize(12, 12),
																					this, callfunc_selector(MeteorWrapper::stopMyAction),
																					bind(&MeteorWrapper::accumCrashCount, this, std::placeholders::_1)); // imgSize, crashSize
		addChild(t_fm);
	}

	if(ingFrame >= totalFrame)
	{
		stopMyAction();
	}
}

void MeteorWrapper::myInit( int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea, KSCumberBase* cb)
{
	m_cumber = cb;
	
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
	
	myGD->ccb_delegate_null_list.push_back(t_bd);

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

void UnusedMissile5::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile5::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	m_frameCnt = 0;
	m_position = t_sp;

	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_perFrame = pattern["perframe"].asInt();
	m_totalFrame = pattern["totalframe"].asInt();
	m_bulletSpeed = pattern["speed"].asInt() / 100.f;
	m_numberPerFrame = pattern["numberperframe"].asInt();
	m_color = pattern["color"].asInt();


	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);

	scheduleUpdate();
}

void UnusedMissile5::update( float dt )
{
	m_frameCnt++;

	if(m_frameCnt == m_totalFrame)
	{
		stopMyAction();
	}
	else if(m_frameCnt % m_perFrame == 0)
	{
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);

		std::string imgFileName;
		std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
		if(KS::isExistFile(fileName))
			imgFileName = fileName;
		else
			imgFileName = "cumber_missile1.png";

		CCSize t_mSize = CCSize(4.f, 4.f);
		int start_angle = m_well512.GetFloatValue(360);

		for(int i=0;i<m_numberPerFrame;i++)
		{
			float temp_angle = start_angle+(360.f/m_numberPerFrame)*i;

			MissileUnit* t_mu = MissileUnit::create(m_position, temp_angle, m_bulletSpeed,
				imgFileName.c_str(), t_mSize, 0.f, 0.f);
			batchNode->addChild(t_mu);
		}
	}
}

void UnusedMissile5::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile6::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile6::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_perFrame = pattern["perframe"].asInt();
	m_totalFrame = pattern["totalframe"].asInt();
	m_bulletSpeed = pattern["speed"].asInt() / 100.f;
	m_color = pattern["color"].asInt();

	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);

	m_frameCnt = 0;
	m_position = t_sp;
	angle = m_well512.GetValue(360);
	scheduleUpdate();
}

void UnusedMissile6::update( float dt )
{
	m_frameCnt++;

	if(m_frameCnt == m_totalFrame)
	{
		stopMyAction();
	}
	else if(m_frameCnt % m_perFrame == 0)
	{

		angle += 11 * m_perFrame;
		if(angle >= 360)
			angle -= 360;

		std::string imgFileName;
		std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
		if(KS::isExistFile(fileName))
			imgFileName = fileName;
		else
			imgFileName = "cumber_missile1.png";
		CCSize t_mSize = CCSize(4.f, 4.f);
		MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
			imgFileName.c_str(), t_mSize, 0.f, 0.f);
		batchNode->addChild(t_mu);



		//			int start_angle = rand()%(360/t_mCnt);
		//
		//			for(int i=0;i<t_mCnt;i++)
		//			{
		//				float temp_angle = start_angle+(360.f/t_mCnt)*i;
		//
		//
		//			}
	}
	if(m_frameCnt % 5 == 0)
	{
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
	}
}

void UnusedMissile6::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile7::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile7::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_bulletSpeed = pattern["speed"].asInt() / 100.f;
	m_numberPerFrame = pattern["numberperframe"].asInt();
	m_color = pattern["color"].asInt();
	m_frameCnt = 0;
	m_position = t_sp;

	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);

	scheduleUpdate();
}

void UnusedMissile7::update( float dt )
{
	float angle = m_well512.GetValue(360);
	for(int i=0; i<m_numberPerFrame;i++)
	{
		std::string imgFileName;
		std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
		if(KS::isExistFile(fileName))
			imgFileName = fileName;
		else
			imgFileName = "cumber_missile1.png";
		CCSize t_mSize = CCSize(4.f,4.f);
		MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
			imgFileName.c_str(), t_mSize, 0.f, 0.f);
		batchNode->addChild(t_mu);
		angle += 360 / m_numberPerFrame;
		if(angle >= 360)
			angle -= 360;
	}
	angle = m_well512.GetValue(360);
	stopMyAction();
}

void UnusedMissile7::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile8::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile8::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_perFrame = pattern["perframe"].asInt();;
	m_totalFrame = pattern["totalframe"].asInt();;
	m_bulletSpeed = pattern["speed"].asInt() / 100.f;
	m_numberPerFrame = pattern["numberperframe"].asInt();;
	m_color = pattern["color"].asInt();

	m_frameCnt = 0;
	m_position = t_sp;

	angle = m_well512.GetValue(360);
	//		angle = m_well512.GetValue(360);
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);
	scheduleUpdate();
}

void UnusedMissile8::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile8::update( float dt )
{
	m_frameCnt++;

	if(m_frameCnt == m_totalFrame)
	{
		stopMyAction();
	}
	else if(m_frameCnt % m_perFrame == 0)
	{
		float startAngle = angle;
		for(int i=0; i<m_numberPerFrame; i++)
		{
			std::string imgFileName;
			std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
			if(KS::isExistFile(fileName))
				imgFileName = fileName;
			else
				imgFileName = "cumber_missile1.png";
			CCSize t_mSize = CCSize(4.f, 4.f);
			MissileUnit* t_mu = MissileUnit::create(m_position, startAngle, m_bulletSpeed,
				imgFileName.c_str(), t_mSize, 0.f, 0.f);
			batchNode->addChild(t_mu);
			startAngle += 360 / m_numberPerFrame; // 10 개라면
		}
	}
	if(m_frameCnt % 5 == 0)
	{
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
	}
}

void UnusedMissile9::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile9::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_perFrame = pattern["perframe"].asInt();;        // p
	m_totalFrame = pattern["totalframe"].asInt();;    // p
	m_bulletSpeed = pattern["speed"].asInt() / 100.f;  // p
	m_numberPerFrame = pattern["numberperframe"].asInt(); // p
	m_color = pattern["color"].asInt();

	m_frameCnt = 0;
	m_position = t_sp;

	angle = m_well512.GetValue(360);
	//		angle = m_well512.GetValue(360);
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);
	scheduleUpdate();
}

void UnusedMissile9::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile9::update( float dt )
{
	m_frameCnt++;

	if(m_frameCnt == m_totalFrame)
	{
		stopMyAction();
	}
	else if(m_frameCnt % m_perFrame == 0)
	{
		float startAngle = angle;
		for(int i=0; i<m_numberPerFrame; i++)
		{
			std::string imgFileName;
			std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
			if(KS::isExistFile(fileName))
				imgFileName = fileName;
			else
				imgFileName = "cumber_missile1.png";
			CCSize t_mSize = CCSize(4.f,4.f);
			MissileUnit* t_mu = MissileUnit::create(m_position, startAngle, m_bulletSpeed,
				imgFileName.c_str(), t_mSize, 0.f, 0.f);
			batchNode->addChild(t_mu);
			startAngle += 360 / m_numberPerFrame; // 10 개라면
		}
		angle += 6;
	}
	if(m_frameCnt % 5 == 0)
	{
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
	}
}

void UnusedMissile10::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_bulletSpeed = pattern["speed"].asInt() / 100.f;  // p
	m_numberPerFrame = pattern["numberperframe"].asInt(); // p
	m_color = pattern["color"].asInt();
	m_frameCnt = 0;
	m_position = t_sp;

	angle = m_well512.GetValue(360);
	//		angle = m_well512.GetValue(360);
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);
	scheduleUpdate();
}

void UnusedMissile10::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile10::update( float dt )
{
	m_frameCnt++;

	if(m_frameCnt == 1*8)
	{
		stopMyAction();
	}
	else if(m_frameCnt % 1 == 0)
	{
		float startAngle = angle;
		for(int i=0; i<m_numberPerFrame; i++)
		{
			std::string imgFileName;
			std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
			if(KS::isExistFile(fileName))
				imgFileName = fileName;
			else
				imgFileName = "cumber_missile1.png";
			CCSize t_mSize = CCSize(4.f, 4.f);
			MissileUnit* t_mu = MissileUnit::create(m_position, startAngle, m_bulletSpeed,
				imgFileName.c_str(), t_mSize, 0.f, 0.f);
			batchNode->addChild(t_mu);
			startAngle += 360 / m_numberPerFrame; // 10 개라면
		}
		angle += 3;
	}
	if(m_frameCnt % 5 == 0)
	{
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
	}
}

void UnusedMissile11::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile11::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_perFrame = pattern["perframe"].asInt();;
	m_totalFrame = pattern["totalframe"].asInt();;   // p
	m_bulletSpeed = pattern["speed"].asInt() / 100.f; // p
	m_numberPerFrame = pattern["numberperframe"].asInt();
	m_color = pattern["color"].asInt();
	m_term = 6; // p

	m_frameCnt = 0;
	m_position = t_sp;
	m_fireCount = 0;
	angle = m_well512.GetValue(360);
	//		angle = m_well512.GetValue(360);
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);
	scheduleUpdate();
}

void UnusedMissile11::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile11::update( float dt )
{
	m_frameCnt++;

	if(m_frameCnt == m_totalFrame)
	{
		stopMyAction();
	}
	else if(m_frameCnt % m_perFrame == 0)
	{
		if(m_idleValue <= 0)
		{
			m_fireCount++;
			float startAngle = angle;
			for(int i=0; i<m_numberPerFrame; i++)
			{
				std::string imgFileName;
				std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
				if(KS::isExistFile(fileName))
					imgFileName = fileName;
				else
					imgFileName = "cumber_missile1.png";
				CCSize t_mSize = CCSize(4.f, 4.f);
				MissileUnit* t_mu = MissileUnit::create(m_position, startAngle, m_bulletSpeed,
					imgFileName.c_str(), t_mSize, 0.f, 0.f);
				batchNode->addChild(t_mu);
				startAngle += m_numberPerFrame; // 10 개라면
			}
			angle += 2;

			// 다섯번 쐈으면 15프레임 쉰다.
			if(m_fireCount == 5)
			{
				m_fireCount = 0;
				m_idleValue = 5 * m_term;
			}
		}
		else
		{
			angle += 2;
			m_idleValue--;
		}
	}
	if(m_frameCnt % 5 == 0)
	{
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
	}
}

void Mugunghwa::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void Mugunghwa::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);


	m_perFrame = pattern.get("perframe", 5).asInt(); // ["perframe"].asInt();;		// p
	m_totalFrame = pattern.get("totalframe", 300).asInt(); // ["totalframe"].asInt();;		// p
	m_bulletSpeed = pattern.get("speed", 200.f).asInt() / 100.f; // ["speed"hhhhhhhh].asInt() / 100.f;	// p
	m_numberPerFrame = pattern.get("numberperframe", 10).asInt(); // ["numberperframe"].asInt();	// p
	m_color = pattern.get("color", 1).asInt(); // ["color"].asInt();
	m_frameCnt = 0;
	m_position = t_sp;

	angle2 = angle = m_well512.GetValue(360);
	//		angle = m_well512.GetValue(360);
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);
	scheduleUpdate();
}

void Mugunghwa::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void Mugunghwa::update( float dt )
{
	m_frameCnt++;
	CCSize t_mSize = CCSize(4.f,4.f);
	if(m_frameCnt == m_totalFrame)
	{
		stopMyAction();
	}
	else if(m_frameCnt % m_perFrame == 0)
	{
		float startAngle = angle;
		for(int i=0; i<m_numberPerFrame; i++)
		{
			std::string imgFileName;
			std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
			if(KS::isExistFile(fileName))
				imgFileName = fileName;
			else
				imgFileName = "cumber_missile1.png";

			MissileUnit* t_mu = MissileUnit::create(m_position, startAngle, m_bulletSpeed,
				imgFileName.c_str(), t_mSize, 0.f, 0.f);
			batchNode->addChild(t_mu);
			startAngle += 360 / m_numberPerFrame; // 10 개라면
		}

		float startAngle2 = angle2;
		for(int i=0; i<m_numberPerFrame; i++)
		{
			std::string imgFileName;
			std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
			if(KS::isExistFile(fileName))
				imgFileName = fileName;
			else
				imgFileName = "cumber_missile1.png";
			MissileUnit* t_mu = MissileUnit::create(m_position, startAngle2, m_bulletSpeed,
				imgFileName.c_str(), t_mSize, 0.f, 0.f);
			batchNode->addChild(t_mu);
			startAngle2 += 360 / m_numberPerFrame; // 10 개라면
		}

		angle += m_perFrame / 3;
		angle2 -= m_perFrame / 3;
	}
	if(m_frameCnt % 5 == 0)
	{
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
	}
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
	Json::Value pattern;
	reader.parse(patternData, pattern);

	this->t_sp = t_sp;
	t_move_speed = pattern.get("speed", 200.0).asDouble() / 100.f;
	t_cushion_cnt = pattern.get("cushioncount", 4).asInt();
	t_is_big_bomb = pattern.get("big", false).asInt();
	t_tmCnt = pattern.get("number", 10).asInt();
	m_crashArea = pattern.get("area", 20).asInt();
	///////////////////////////////////////////

	scheduleUpdate();
}

void CaromWrapper::update( float dt )
{
	for(int i=0;i<t_tmCnt;i++)
	{
		// create
		ThreeCushion* t_tc = ThreeCushion::create(t_sp, t_move_speed, t_cushion_cnt, t_is_big_bomb, m_crashArea,
			this, callfunc_selector(ThisClassType::removeEffect));
		addChild(t_tc);
	}

	stopMyAction();
}

void UnusedMissile1::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile1::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_bulletSpeed = pattern["speed"].asInt() / 100.f;
	m_numberPerFrame = pattern["numberperframe"].asInt();
	m_color = pattern.get("color", 1).asInt();

	m_frameCnt = 0;
	m_position = t_sp;

	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);
	scheduleUpdate();
}

void UnusedMissile1::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile1::update( float dt )
{
	CCPoint jackPoint = ip2ccp(myGD->getJackPoint());
	float rad = atan2(jackPoint.y - m_position.y, jackPoint.x - m_position.x);
	float angle = rad2Deg(rad);
	float angle2 = rad2Deg(rad);

	for(int i=0; i<m_numberPerFrame;i++)
	{
		std::string imgFileName;
		std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
		if(KS::isExistFile(fileName))
			imgFileName = fileName;
		else
			imgFileName = "cumber_missile1.png";
		CCSize t_mSize = CCSize(4.f, 4.f);
		if(angle == angle2)
		{
			MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
				imgFileName.c_str(), t_mSize, 0, 0);
			batchNode->addChild(t_mu);
		}
		else
		{
			MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
				imgFileName.c_str(), t_mSize, 0, 0);
			batchNode->addChild(t_mu);

			MissileUnit* t_mu2 = MissileUnit::create(m_position, angle2, m_bulletSpeed,
				imgFileName.c_str(), t_mSize, 0, 0);
			batchNode->addChild(t_mu2);
		}

		angle += 35 / m_numberPerFrame; // 10 개라면
		angle2 -= 35 / m_numberPerFrame;

		if(angle >= 360)
			angle -= 360;
		if(angle2 < 0)
			angle2 += 360;
	}

	stopMyAction();
	m_cumber->onTargetingJack(jackPoint);
}

void UnusedMissile2::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile2::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_perFrame = pattern["perframe"].asInt();;
	m_totalFrame = pattern["totalframe"].asInt();;
	m_bulletSpeed = pattern["speed"].asInt() / 100.f;
	m_numberPerFrame = pattern["numberperframe"].asInt();;
	m_color = pattern["color"].asInt();
	m_frameCnt = 0;
	m_position = t_sp;

	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);
	scheduleUpdate();
	//		cb->stopAnimationDirection();
}

void UnusedMissile2::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile2::update( float dt )
{
	m_frameCnt++;
	CCPoint jackPoint = ip2ccp(myGD->getJackPoint());
	if(m_frameCnt == m_totalFrame)
	{
		stopMyAction();
	}
	else
	{
		if(m_frameCnt % m_perFrame == 0)
		{

			float rad = atan2(jackPoint.y - m_position.y, jackPoint.x - m_position.x);
			float angle = rad2Deg(rad);
			float angle2 = rad2Deg(rad);

			for(int i=0; i<m_numberPerFrame;i++)
			{
				std::string imgFileName;
				std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
				if(KS::isExistFile(fileName))
					imgFileName = fileName;
				else
					imgFileName = "cumber_missile1.png";
				CCSize t_mSize = CCSize(4.f, 4.f);
				if(angle == angle2)
				{
					MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
						imgFileName.c_str(), t_mSize, 0.f, 0.f);
					batchNode->addChild(t_mu);
				}
				else
				{
					MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
						imgFileName.c_str(), t_mSize, 0.f, 0.f);
					batchNode->addChild(t_mu);

					MissileUnit* t_mu2 = MissileUnit::create(m_position, angle2, m_bulletSpeed,
						imgFileName.c_str(), t_mSize, 0.f, 0.f);
					batchNode->addChild(t_mu2);
				}

				angle += 35 / m_numberPerFrame; // 10 개라면
				angle2 -= 35 / m_numberPerFrame;

				if(angle >= 360)
					angle -= 360;
				if(angle2 < 0)
					angle2 += 360;
			}

		}
	}
	m_cumber->onTargetingJack(jackPoint);
}

void UnusedMissile3::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile3::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_perFrame = pattern["perframe"].asInt();;
	m_totalFrame = pattern["totalframe"].asInt();
	m_bulletSpeed = pattern["speed"].asInt() / 100.f;
	m_numberPerFrame = pattern["numberperframe"].asInt();
	m_color = pattern["color"].asInt();
	m_frameCnt = 0;
	m_position = t_sp;

	firstJackPosition = ip2ccp(myGD->getJackPoint());
	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);
	scheduleUpdate();
	cb->stopAnimationDirection();
}

void UnusedMissile3::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile3::update( float dt )
{
	m_frameCnt++;
	CCPoint jackPoint = firstJackPosition;
	if(m_frameCnt == m_totalFrame)
	{
		stopMyAction();
	}
	else
	{
		if(m_frameCnt % m_perFrame == 0)
		{

			float rad = atan2(jackPoint.y - m_position.y, jackPoint.x - m_position.x);
			float angle = rad2Deg(rad);
			float angle2 = rad2Deg(rad);

			for(int i=0; i<m_numberPerFrame;i++)
			{
				std::string imgFileName;
				std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
				if(KS::isExistFile(fileName))
					imgFileName = fileName;
				else
					imgFileName = "cumber_missile1.png";
				CCSize t_mSize = CCSize(4.f, 4.f);
				if(angle == angle2)
				{
					MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
						imgFileName.c_str(), t_mSize, 0.f, 0.f);
					batchNode->addChild(t_mu);
				}
				else
				{
					MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
						imgFileName.c_str(), t_mSize, 0.f, 0.f);
					batchNode->addChild(t_mu);

					MissileUnit* t_mu2 = MissileUnit::create(m_position, angle2, m_bulletSpeed,
						imgFileName.c_str(), t_mSize, 0.f, 0.f);
					batchNode->addChild(t_mu2);
				}

				angle += 35 / m_numberPerFrame; // 10 개라면
				angle2 -= 35 / m_numberPerFrame;

				if(angle >= 360)
					angle -= 360;
				if(angle2 < 0)
					angle2 += 360;
			}

		}
	}
	m_cumber->onTargetingJack(jackPoint);
}

void UnusedMissile4::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void UnusedMissile4::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_perFrame = pattern["perframe"].asInt();
	m_totalFrame = pattern["totalframe"].asInt();
	m_bulletSpeed = pattern["speed"].asInt() / 100.f;
	m_numberPerFrame = pattern["numberperframe"].asInt();
	m_color = pattern["color"].asInt();
	m_frameCnt = 0;
	m_position = t_sp;

	firstJackPosition = ip2ccp(myGD->getJackPoint());
	fireCount = 0;

	std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
	if(KS::isExistFile(fileName))
		batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
	else
		batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);

	addChild(batchNode);

	scheduleUpdate();
	cb->stopAnimationDirection();
}

void UnusedMissile4::stopMyAction()
{
	unscheduleUpdate();

	m_cumber->setAttackPattern(nullptr);
	myGD->communication("CP_onPatternEndOf", m_cumber);

	startSelfRemoveSchedule();
}

void UnusedMissile4::update( float dt )
{
	m_frameCnt++;
	CCPoint jackPoint = firstJackPosition;
	if(m_frameCnt == m_totalFrame)
	{
		stopMyAction();
	}
	else
	{
		if(m_frameCnt % m_perFrame == 0)
		{


			float rad = atan2(jackPoint.y - m_position.y, jackPoint.x - m_position.x);
			rad += m_well512.GetFloatValue(-5 * M_PI / 180.f, +5 * M_PI / 180.f);
			float angle = rad2Deg(rad);
			float angle2 = rad2Deg(rad);

			for(int i=0; i<m_numberPerFrame;i++)
			{
				std::string imgFileName;
				std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
				if(KS::isExistFile(fileName))
					imgFileName = fileName;
				else
					imgFileName = "cumber_missile1.png";
				CCSize t_mSize = CCSize(4.f, 4.f);
				if(angle == angle2)
				{
					MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
						imgFileName.c_str(), t_mSize, 0.f, 0.f);
					batchNode->addChild(t_mu);
				}
				else
				{
					MissileUnit* t_mu = MissileUnit::create(m_position, angle, m_bulletSpeed,
						imgFileName.c_str(), t_mSize, 0.f, 0.f);
					batchNode->addChild(t_mu);

					MissileUnit* t_mu2 = MissileUnit::create(m_position, angle2, m_bulletSpeed,
						imgFileName.c_str(), t_mSize, 0.f, 0.f);
					batchNode->addChild(t_mu2);
				}

				angle += 35 / m_numberPerFrame; // 10 개라면
				angle2 -= 35 / m_numberPerFrame;

				if(angle >= 360)
					angle -= 360;
				if(angle2 < 0)
					angle2 += 360;
			}
			fireCount++;
		}
	}
	m_cumber->onTargetingJack(jackPoint);
}

void SawWrapper::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	speed = pattern.get("speed", 150.f).asDouble() / 100.f;
	crashsize = pattern.get("crashsize", 20.f).asDouble();
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
	speed = pattern.get("speed", 200.0).asDouble() / 100.f;
	crashsize = pattern.get("crashsize", 10).asInt();
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
	m_totalFrame = pattern.get("number", 5).asInt() * 40;
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
	m_frame++;
	if(m_frame % 40 == 0)
	{
		// 쏨~
		ThrowBomb* gun = ThrowBomb::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), m_pattern);
		addChild(gun);
	}
	if(m_frame == m_totalFrame)
	{
		stopMyAction();
		m_targetSprite->setVisible(false);
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

	PoisonDrop* t_pd = PoisonDrop::create(m_position, jackPosition, movingFrame, area, totalFrame);
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



