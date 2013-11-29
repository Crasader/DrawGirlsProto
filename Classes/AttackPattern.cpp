//
//  AttackPattern.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 25..
//
//
#include "AttackPattern.h"


void KSTargetAttackPattern8::myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
{
	m_cumber = cb;
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
	
	KS::KSLog("%", pattern);
	m_isCurve = pattern.get("curve", false).asBool();
	
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
	
	addChild(batchNode);
	
	scheduleUpdate();
}


void KSTargetAttackPattern8::update(float dt)
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
				CCPoint mobPosition = ip2ccp(myGD->getMainCumberPoint());
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
//				CCLog("gun degree %f", gun.degree.getValue());
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
						MathmaticalMissileUnit* t_mu = MathmaticalMissileUnit::create(ip2ccp(myGD->getMainCumberPoint()), gun.degree.getValue(), gun.bulletSpeed,
																					  imgFileName.c_str(), t_mSize,
																					  m_path, m_isCurve ? MathmaticalMissileUnit::CURVE : MathmaticalMissileUnit::RIGHTLINE);
						batchNode->addChild(t_mu);
					}
					else
					{
						MissileUnit* t_mu = MissileUnit::create(ip2ccp(myGD->getMainCumberPoint()), gun.degree.getValue(), gun.bulletSpeed,
																																					imgFileName.c_str(), t_mSize,0, 0);
						batchNode->addChild(t_mu);
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

void KSTargetAttackPattern8::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSTargetAttackPattern8::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSTargetAttackPattern8::initGuns()
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
			CCPoint mobPosition = ip2ccp(myGD->getMainCumberPoint());
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




void KSTargetAttackPattern10::myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
{
	m_cumber = cb;
	scheduleUpdate();
	
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	this->pattern = pattern;
}

void KSTargetAttackPattern10::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	//		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCFadeOut::create(0.5f)}));
	//		m_parentMissile->removeFromParentAndCleanup(true);
	startSelfRemoveSchedule();
}

void KSTargetAttackPattern10::update( float dt )
{
	Firework* fw = Firework::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), pattern);
	addChild(fw);
	stopMyAction();
}



void KSTargetAttackPattern11::myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
{
	m_cumber = cb;
	scheduleUpdate();
	
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_pattern = pattern;
}

void KSTargetAttackPattern11::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	//		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCFadeOut::create(0.5f)}));
	//		m_parentMissile->removeFromParentAndCleanup(true);
	startSelfRemoveSchedule();
}

void KSTargetAttackPattern11::update( float dt )
{
	MovingSunflower* ap = MovingSunflower::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), m_pattern);
	addChild(ap);
	stopMyAction();
}






void AttackPattern::startSelfRemoveSchedule()
{
	schedule(schedule_selector(AttackPattern::selfRemoveSchedule));
}

void AttackPattern::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
		CCLog("self remove!!");
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

AP_Missile6* AP_Missile6::create( CCPoint t_sp, int t_type )
{
	AP_Missile6* t_m6 = new AP_Missile6();
	t_m6->myInit(t_sp, t_type);
	t_m6->autorelease();
	return t_m6;
}

void AP_Missile6::stopMyAction()
{
	unschedule(schedule_selector(AP_Missile6::myAction));
	if (beamImg)
	{
		beamImg->removeFromParentAndCleanup(true);
		beamImg = 0;
	}

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void AP_Missile6::removeEffect()
{
	unschedule(schedule_selector(AP_Missile6::myAction));

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile6::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);

	if(beamImg)
		beamImg->runAction(t_seq);
}

void AP_Missile6::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void AP_Missile6::startMyAction()
{
	ingFrame = 0;
	schedule(schedule_selector(AP_Missile6::myAction));
}

void AP_Missile6::myAction()
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
			unschedule(schedule_selector(AP_Missile6::myAction));
			removeEffect();
		}
		else if(beforeAngle > afterAngle && (jackAngle >= beforeAngle || jackAngle <= afterAngle))
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			unschedule(schedule_selector(AP_Missile6::myAction));
			removeEffect();
		}
	}

	if(beamImg)
		beamImg->setRotation(-(beamBaseAngle-10 + 2*ingFrame));

	Json::Reader reader;
	Json::Value root;
	reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "boss"), root);
	Json::Value pattern = root[0u]["pattern"]["1011"];

	int totalFrame = pattern.get("totalframe", 10).asInt();
	if(ingFrame >= totalFrame)
	{
		//			beamImg->removeFromParentAndCleanup(true);
		stopMyAction();
	}
}

void AP_Missile6::myInit( CCPoint t_sp, int t_type )
{
	type = t_type;
	startPosition = t_sp;

	if(type == 1) // lazer
	{
		beamImg = CCSprite::create("beam.png");
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

AP_Missile9* AP_Missile9::create( int t_keepFrame, int t_shootFrame, float t_distance, CCSize mSize, int t_type )
{
	AP_Missile9* t_m9 = new AP_Missile9();
	t_m9->myInit(t_keepFrame, t_shootFrame, t_distance, mSize, t_type);
	t_m9->autorelease();
	return t_m9;
}

void AP_Missile9::stopMyAction()
{
	AudioEngine::sharedInstance()->stopEffect("sound_rock_falling.mp3");
	unschedule(schedule_selector(AP_Missile9::myAction));

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void AP_Missile9::removeEffect()
{
	if(!isRemoveEffect)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_rock_falling.mp3");
		unschedule(schedule_selector(AP_Missile9::myAction));

		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");

		CCDelayTime* t_delay = CCDelayTime::create(1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile9::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);

		runAction(t_seq);
	}
}

void AP_Missile9::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void AP_Missile9::startMyAction()
{
	AudioEngine::sharedInstance()->playEffect("sound_rock_falling.mp3", true);
	ingFrame = 0;
	schedule(schedule_selector(AP_Missile9::myAction));
}

void AP_Missile9::myAction()
{
	ingFrame++;

	if(ingFrame%shootFrame == 0)
	{
		MissileUnit3* t_mu = MissileUnit3::create(type, distance, mSize, this, callfunc_selector(AP_Missile9::removeEffect));
		addChild(t_mu);
	}

	if(ingFrame >= keepFrame)
	{
		stopMyAction();
	}
}

void AP_Missile9::myInit( int t_keepFrame, int t_shootFrame, float t_distance, CCSize t_mSize, int t_type )
{
	keepFrame = t_keepFrame;
	shootFrame = t_shootFrame;
	distance = t_distance;
	type = t_type;
	mSize = t_mSize;

	startMyAction();
}

AP_Missile11* AP_Missile11::create( CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize )
{
	AP_Missile11* t_m11 = new AP_Missile11();
	t_m11->myInit(t_sp, t_type, t_speed, t_mSize);
	t_m11->autorelease();
	return t_m11;
}

void AP_Missile11::stopMyAction()
{
	myGD->communication("MP_endIngActionAP");

	startSelfRemoveSchedule();
}

void AP_Missile11::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void AP_Missile11::myInit( CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize )
{
	//		myGD->communication("EP_startCrashAction");

	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);

	CCPoint subPosition = ccpSub(jackPosition, t_sp);
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));

	float throwAngle;

	if(distance < 200)			throwAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + (rand()%91-45)/2;
	else						throwAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + (rand()%31-15)/2;


	ThrowObject* t_to = ThrowObject::create(t_sp, t_type, 2.f, throwAngle, t_mSize);
	addChild(t_to);
	t_to->startMyAction();
	stopMyAction();
}

AP_Missile12* AP_Missile12::create( CCPoint t_sp, int t_type, int t_targetingFrame, int t_shootFrame )
{
	AP_Missile12* t_m12 = new AP_Missile12();
	t_m12->myInit(t_sp, t_type, t_targetingFrame, t_shootFrame);
	t_m12->autorelease();
	return t_m12;
}

void AP_Missile12::stopMyAction()
{
	unschedule(schedule_selector(AP_Missile12::myAction));



	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");
	startSelfRemoveSchedule();
	fadeFromToDuration.init(255, 0, 1.f);
	schedule(schedule_selector(ThisClassType::hidingAnimation));
}

void AP_Missile12::hidingAnimation( float dt )
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

void AP_Missile12::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void AP_Missile12::startMyAction()
{
	ingFrame = 0;
	schedule(schedule_selector(AP_Missile12::myAction), 1/30.f);
}

void AP_Missile12::myAction()
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
			myBeam = SatelliteBeam::create(pJackArray.front(), type, this, callfunc_selector(AP_Missile12::stopMyAction));
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

void AP_Missile12::myInit( CCPoint t_sp, int t_type, int t_targetingFrame, int t_shootFrame )
{
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

	targetingImg = ret.first;
	targetingImg->setScale(2.7);

	targetingImg->setPosition(jackPosition);

	addChild(targetingImg);

	startMyAction();
}

AP_Missile14* AP_Missile14::create( CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize )
{
	AP_Missile14* t_m14 = new AP_Missile14();
	t_m14->myInit(t_sp, t_type, t_speed, t_tmCnt, t_mSize);
	t_m14->autorelease();
	return t_m14;
}

void AP_Missile14::stopMyAction()
{
	myGD->communication("MP_endIngActionAP");

	startSelfRemoveSchedule();
}

void AP_Missile14::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void AP_Missile14::myInit( CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize )
{
	//		myGD->communication("EP_startCrashAction");

	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);

	CCPoint subPosition = ccpSub(jackPosition, t_sp);
	float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));

	float baseAngle;

	if(distance < 200)			baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%91 - 45;
	else						baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%31 - 15;


	int left = t_tmCnt / 2;
	int right = t_tmCnt - left;
	int unitDegree = 20;
	for(int i=1; i<=left; i++)
	{
		float t_angle = baseAngle - i*unitDegree;
		if(t_angle >= 180)		t_angle -= 360;
		if(t_angle < -180)		t_angle += 360;

		ThrowObject* t_to = ThrowObject::create(t_sp, t_type, 2.f, t_angle, t_mSize);
		addChild(t_to);
		t_to->startMyAction();
	}
	for (int i=0; i<right; i++) {
		float t_angle = baseAngle + i*unitDegree;
		if(t_angle >= 180)		t_angle -= 360;
		if(t_angle < -180)		t_angle += 360;

		ThrowObject* t_to = ThrowObject::create(t_sp, t_type, 2.f, t_angle, t_mSize);
		addChild(t_to);
		t_to->startMyAction();
	}

	stopMyAction();
}

AP_Missile15* AP_Missile15::create( CCPoint t_sp, int t_tmCnt, int t_burnFrame )
{
	AP_Missile15* t_m15 = new AP_Missile15();
	t_m15->myInit(t_sp, t_tmCnt, t_burnFrame);
	t_m15->autorelease();
	return t_m15;
}

void AP_Missile15::stopMyAction()
{
	if(!is_remove_called)
	{
		is_remove_called = true;
		unschedule(schedule_selector(AP_Missile15::myAction));
		myParticle->setDuration(0);

		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");

		CCDelayTime* t_delay = CCDelayTime::create(1.2f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile15::particleRemove));

		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);

		runAction(t_seq);

		startSelfRemoveSchedule();
	}
}

void AP_Missile15::removeEffect()
{
	if(!is_remove_called)
	{
		is_remove_called = true;
		unschedule(schedule_selector(AP_Missile15::myAction));
		myParticle->setDuration(0);

		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");

		CCDelayTime* t_delay = CCDelayTime::create(1.2f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile15::selfRemove));

		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);

		runAction(t_seq);
	}
}

void AP_Missile15::selfRemove()
{
	particleRemove();
	removeFromParentAndCleanup(true);
}

void AP_Missile15::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void AP_Missile15::particleRemove()
{
	myParticle->removeFromParentAndCleanup(true);
}

void AP_Missile15::startMyAction()
{
	AudioEngine::sharedInstance()->playEffect("sound_fire_shot.mp3", false);
	ingFrame = 0;
	schedule(schedule_selector(AP_Missile15::myAction));
}

void AP_Missile15::myAction()
{
	ingFrame++;

	float shootAngle = minAngle + (maxAngle - minAngle + 0.f)/burnFrame*ingFrame;
	myParticle->setAngle(shootAngle);

	if(ingFrame%createBurnFrame == 0)
	{
		Burn* t_b = Burn::create(myPosition, baseDistance, shootAngle, mType,
			this, callfunc_selector(AP_Missile15::removeEffect));
		addChild(t_b);
		t_b->startMyAction();
	}


	if(ingFrame >= burnFrame)
	{
		stopMyAction();
	}
}

void AP_Missile15::initParticle()
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

void AP_Missile15::myInit( CCPoint t_sp, int t_tmCnt, int t_burnFrame )
{
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

AP_Missile16* AP_Missile16::create( int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea)
{
	AP_Missile16* t_m16 = new AP_Missile16();
	t_m16->myInit(t_type, t_tmCnt, t_totalFrame, t_crashArea);
	t_m16->autorelease();
	return t_m16;
}

void AP_Missile16::stopMyAction()
{
	if(!isRemoveEffect)
	{
		isRemoveEffect = true;
		unschedule(schedule_selector(AP_Missile16::myAction));

		myGD->communication("MP_endIngActionAP");

		startSelfRemoveSchedule();
	}
}

void AP_Missile16::removeEffect()
{
	if(!isRemoveEffect)
	{
		isRemoveEffect = true;
		unschedule(schedule_selector(AP_Missile16::myAction));

		myGD->communication("MP_endIngActionAP");

		startSelfRemoveSchedule();
	}
}

void AP_Missile16::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void AP_Missile16::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void AP_Missile16::startMyAction()
{
	ingFrame = 0;
	schedule(schedule_selector(AP_Missile16::myAction));
}

void AP_Missile16::myAction()
{
	ingFrame++;

	if(ingFrame%mRate == 0)
	{
		CCPoint random_fp;

		int screenHeight = (myGD->limited_step_top - myGD->limited_step_bottom)*pixelSize;
		random_fp.x = m_well512.GetValue(240);
		random_fp.y = m_well512.GetValue(myGD->getJackPoint().y * pixelSize - screenHeight / 2.f,
			myGD->getJackPoint().y * pixelSize + screenHeight / 2.f);

		CCPoint random_sp;
		random_sp.x = random_fp.x + 500;
		random_sp.y = random_fp.y + 500;

		FallMeteor* t_fm = FallMeteor::create(imgFilename, 1, CCSizeMake(crashArea, crashArea), random_sp, random_fp, 220, 20, IntSize(15, 15), this, callfunc_selector(AP_Missile16::removeEffect)); // imgSize, crashSize
		addChild(t_fm);
	}

	if(ingFrame >= totalFrame)
	{
		stopMyAction();
	}
}

void AP_Missile16::myInit( int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea)
{
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


AP_Missile21* AP_Missile21::create( CCPoint t_sp, int tf, int sc )
{
	AP_Missile21* t_m21 = new AP_Missile21();
	t_m21->myInit(t_sp, tf, sc);
	t_m21->autorelease();
	return t_m21;
}

void AP_Missile21::myInit( CCPoint t_sp, int tf, int sc )
{
	int totalFrame = tf;
	float scale = sc;
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);

	BlindDrop* t_bd = BlindDrop::create(t_sp, jackPosition, 20, totalFrame, scale);
	addChild(t_bd);
	t_bd->startAction();

	startSelfRemoveSchedule();
}

AP_Missile23* AP_Missile23::create( int t_frame )
{
	AP_Missile23* t_m23 = new AP_Missile23();
	t_m23->myInit(t_frame);
	t_m23->autorelease();
	return t_m23;
}

void AP_Missile23::updateCobweb()
{
	if(!is_stop)
		ingFrame = 0;
}

void AP_Missile23::removeCobweb()
{
	cobwebImg->removeFromParent();
	startSelfRemoveSchedule();
}

void AP_Missile23::stopFrame()
{
	is_stop = true;
	unschedule(schedule_selector(AP_Missile23::framing));

	cobwebImg->stopAllActions();

	CCScaleTo* t_scale = CCScaleTo::create(0.3, 0.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile23::removeCobweb));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	cobwebImg->runAction(t_seq);

	myGD->setAlphaSpeed(myGD->getAlphaSpeed()+0.5f);
	myGD->communication("MP_deleteKeepAP23");
}

void AP_Missile23::myInit( int t_frame )
{
	is_stop = false;

	slowFrame = t_frame;


	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	cobwebImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_slowzone.ccbi",this));
	reader->release();

	cobwebImg->setPosition(ccp(160,215));
	cobwebImg->setScale(0.f);

	addChild(cobwebImg);
	m_scaleFromTo.init(0.0f, 1.0f, 0.3f);
	//		CCScaleTo* t_scale = CCScaleTo::create(0.3, 1.f);
	//		cobwebImg->runAction(t_scale); // 나중에 수동으로 구현해야함.

	myGD->setAlphaSpeed(myGD->getAlphaSpeed()-0.5f);

	startFrame();
}

void AP_Missile23::startFrame()
{
	ingFrame = 0;
	schedule(schedule_selector(AP_Missile23::framing));
}

void AP_Missile23::framing()
{
	ingFrame++;

	m_scaleFromTo.step(1/60.f);
	cobwebImg->setScale(m_scaleFromTo.getValue());
	if(ingFrame >= slowFrame)
	{
		stopFrame();
	}
}



AP_Missile24* AP_Missile24::create( int t_frame )
{
	AP_Missile24* t_m24 = new AP_Missile24();
	t_m24->myInit(t_frame);
	t_m24->autorelease();
	return t_m24;
}

void AP_Missile24::updateSightOut()
{
	ingFrame = 0;

	SightOut* t_so = SightOut::create();
	t_so->setPosition(ccp(160,215));
	addChild(t_so);
	t_so->startAction();
}

void AP_Missile24::startFrame()
{
	ingFrame = 0;
	schedule(schedule_selector(AP_Missile24::framing));
}

void AP_Missile24::framing()
{
	ingFrame++;

	if(ingFrame >= sightOutFrame)
	{
		stopFrame();
	}
}

void AP_Missile24::stopFrame()
{
	unschedule(schedule_selector(AP_Missile24::framing));

	myGD->communication("MP_deleteKeepAP24");
	startSelfRemoveSchedule();
}

void AP_Missile24::myInit( int t_frame )
{
	sightOutFrame = t_frame;

	SightOut* t_so = SightOut::create();
	t_so->setPosition(ccp(160,215));
	addChild(t_so);
	t_so->startAction();

	startFrame();
}

AP_Missile26* AP_Missile26::create( int t_frame )
{
	AP_Missile26* t_m26 = new AP_Missile26();
	t_m26->myInit(t_frame);
	t_m26->autorelease();
	return t_m26;
}

void AP_Missile26::updateFreeze()
{
	myGD->communication("Jack_createFog");
	ingFrame = 0;
}

void AP_Missile26::stopFrame()
{
	unschedule(schedule_selector(AP_Missile26::framing));
	myGD->communication("MP_deleteKeepAP26");
	removeFromParentAndCleanup(true);
}

void AP_Missile26::startFrame()
{
	ingFrame = 0;
	schedule(schedule_selector(AP_Missile26::framing));
}

void AP_Missile26::framing()
{
	ingFrame++;

	if(ingFrame >= freezingFrame)
	{
		myGD->communication("Main_touchOn");
		stopFrame();
	}
}

void AP_Missile26::myInit( int t_frame )
{
	freezingFrame = t_frame;

	myGD->communication("Jack_createFog");
	startFrame();
}

AP_Missile32* AP_Missile32::create()
{
	AP_Missile32* t_m32 = new AP_Missile32();
	t_m32->myInit();
	t_m32->autorelease();
	return t_m32;
}

void AP_Missile32::myInit()
{
	myGD->communication("CP_startTeleport");
	startSelfRemoveSchedule();
}

AP_Missile33* AP_Missile33::create( int t_frame )
{
	AP_Missile33* t_m33 = new AP_Missile33();
	t_m33->myInit(t_frame);
	t_m33->autorelease();
	return t_m33;
}

void AP_Missile33::updateChaos()
{
	ingFrame = 0;
}

void AP_Missile33::stopFrame()
{
	unschedule(schedule_selector(AP_Missile33::framing));
	myGD->communication("MP_deleteKeepAP33");
	startSelfRemoveSchedule();
}

void AP_Missile33::startFrame()
{
	ingFrame = 0;
	schedule(schedule_selector(AP_Missile33::framing));
}

void AP_Missile33::framing()
{
	ingFrame++;

	if(ingFrame >= chaosFrame)
	{
		myGD->communication("Jack_reverseOff");
		stopFrame();
	}
}

void AP_Missile33::myInit( int t_frame )
{
	chaosFrame = t_frame;

	myGD->communication("Jack_createChaos");
	startFrame();
}

void KSAttackPattern1::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSAttackPattern1::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
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

void KSAttackPattern1::update( float dt )
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

void KSAttackPattern1::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSAttackPattern2::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSAttackPattern2::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
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

void KSAttackPattern2::update( float dt )
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

void KSAttackPattern2::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSAttackPattern3::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSAttackPattern3::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
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

void KSAttackPattern3::update( float dt )
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

void KSAttackPattern3::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSAttackPattern4::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSAttackPattern4::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
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

void KSAttackPattern4::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSAttackPattern4::update( float dt )
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

void KSAttackPattern5::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSAttackPattern5::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
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

void KSAttackPattern5::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSAttackPattern5::update( float dt )
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

void KSAttackPattern6::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
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

void KSAttackPattern6::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSAttackPattern6::update( float dt )
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

void KSAttackPattern7::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSAttackPattern7::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
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

void KSAttackPattern7::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSAttackPattern7::update( float dt )
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

void KSAttackPattern8::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSAttackPattern8::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_perFrame = pattern["perframe"].asInt();;		// p
	m_totalFrame = pattern["totalframe"].asInt();;		// p
	m_bulletSpeed = pattern["speed"].asInt() / 100.f;	// p
	m_numberPerFrame =pattern["numberperframe"].asInt();	// p
	m_color = pattern["color"].asInt();
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

void KSAttackPattern8::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSAttackPattern8::update( float dt )
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

void KSAttackPattern9::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSAttackPattern9::removeEffect()
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

void KSAttackPattern9::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void KSAttackPattern9::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	isRemoveEffect = false;
	//		myGD->communication("EP_startCrashAction");

	//		CCPoint t_sp = cb->getPosition();

	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	this->t_sp = t_sp;
	t_move_speed = pattern.get("speed", 200.0).asDouble() / 100.f;
	t_cushion_cnt = pattern.get("cushioncount", 4).asInt();
	t_is_big_bomb = pattern.get("big", false).asBool();
	t_tmCnt = pattern.get("number", 10).asInt();
	m_crashArea = pattern.get("area", 20).asInt();
	///////////////////////////////////////////

	scheduleUpdate();
}

void KSAttackPattern9::update( float dt )
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

void KSTargetAttackPattern1::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSTargetAttackPattern1::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
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

void KSTargetAttackPattern1::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSTargetAttackPattern1::update( float dt )
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

void KSTargetAttackPattern2::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSTargetAttackPattern2::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
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

void KSTargetAttackPattern2::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSTargetAttackPattern2::update( float dt )
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

void KSTargetAttackPattern3::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSTargetAttackPattern3::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
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

void KSTargetAttackPattern3::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSTargetAttackPattern3::update( float dt )
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

void KSTargetAttackPattern4::selfRemoveSchedule()
{
	if(batchNode->getChildrenCount() == 0)
	{
		removeFromParentAndCleanup(true);
	}
}

void KSTargetAttackPattern4::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
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

void KSTargetAttackPattern4::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSTargetAttackPattern4::update( float dt )
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

void KSTargetAttackPattern5::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	speed = pattern.get("speed", 150.f).asDouble() / 100.f;
	crashsize = pattern.get("crashsize", 20.f).asDouble();
	m_cumber = cb;
	scheduleUpdate();
}

void KSTargetAttackPattern5::update( float dt )
{
	IntPoint mainCumberPoint = ccp2ip(m_cumber->getPosition());
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile11* t_m11 = AP_Missile11::create(mainCumberPosition, 11, speed, IntSize(round(crashsize),round(crashsize)));
	addChild(t_m11);

	//		myGD->communication("CP_onPatternEnd");

	stopMyAction();
}

void KSTargetAttackPattern5::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSTargetAttackPattern6::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	number = pattern.get("number", 4).asInt();
	speed = pattern.get("speed", 200.0).asDouble() / 100.f;
	crashsize = pattern.get("crashsize", 10).asInt();
	m_cumber = cb;
	scheduleUpdate();
}

void KSTargetAttackPattern6::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSTargetAttackPattern6::update( float dt )
{
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile14* t_m14 = AP_Missile14::create(mainCumberPosition, 14, speed, number, IntSize(round(crashsize),round(crashsize)));
	addChild(t_m14);


	stopMyAction();
}

void KSTargetAttackPattern7::stopMyAction()
{
	unschedule(schedule_selector(ThisClassType::myAction));

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");



	startSelfRemoveSchedule();
	fadeFromToDuration.init(1, 0, 0.2f);
	schedule(schedule_selector(ThisClassType::hidingAnimation));
}

void KSTargetAttackPattern7::hidingAnimation( float dt )
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

void KSTargetAttackPattern7::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	lazer_main = t_bead = NULL;
	m_cumber = cb;

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

	cb->stopAnimationDirection();
}

void KSTargetAttackPattern7::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void KSTargetAttackPattern7::selfRemoveSchedule()
{
	if(getChildrenCount() == 0)
	{
		//			myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects", false);
		removeFromParentAndCleanup(true);
	}
}

void KSTargetAttackPattern7::startMyAction()
{
	AudioEngine::sharedInstance()->playEffect("sound_angle_beem.mp3", false);
	ingFrame = 0;
	schedule(schedule_selector(ThisClassType::myAction));
}

void KSTargetAttackPattern7::myAction()
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
			auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_laser1_head.ccbi");
			lazer_main = ret.first;

			lazer_main->setAnchorPoint(ccp(0,0.5));
			lazer_main->setRotation(-angle);


			CCPoint mp = ccpMult(dv, 30);
			mp = ccpAdd(sp, mp);
			lazer_main->setPosition(mp);

			addChild(lazer_main);


			//				CCSprite* prev = ret2.first;
			for(int i=0; i<10; i++)
			{
				auto ret2 = KS::loadCCBI<CCSprite*>(this, "pattern_laser1_body.ccbi");
				CCSprite* laser3 = ret2.first;
				laser3->setPosition(ccp(74 + 44 * i, 0));
				lazer_main->addChild(laser3);
				//					prev = laser3;
			}



			CCPoint c_sp = ccpMult(dv, 30);
			c_sp = ccpAdd(sp, c_sp);

			float t_scale = m_crashSize/30.f;

			crashRect = CCRectMake(0, (-m_crashSize + 10*t_scale), 460, (m_crashSize + 10*t_scale));
			//				crashRect = CCRectMake(0, -60/2 + 10, 460, +60/2 + 10);
			lineCrashMap(c_sp, angle, 460, 60);
		}
	}
	else if(ingFrame <= chargeFrame+crashFrame)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);

		CCPoint t_jp = spinTransform(jackPosition, sp, angle);

		if(crashRect.containsPoint(t_jp))
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			//				stopMySchedule();
			stopMyAction();
		}
	}

	if(ingFrame >= chargeFrame+crashFrame)
	{
		stopMyAction();
	}
	ingFrame++;

	m_cumber->onTargetingJack(jackPosition);
}

void KSTargetAttackPattern7::lineCrashMap( CCPoint t_sp, float t_angle, int t_width, int t_height )
{
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			CCPoint t_tp = ccp((i-1)*pixelSize+1,(j-1)*pixelSize+1);
			CCPoint a_tp = spinTransform(t_tp, t_sp, t_angle);
			if(crashRect.containsPoint(a_tp))
			{
				crashMapForIntPoint(IntPoint(i,j));
			}
		}
	}
}

void KSTargetAttackPattern7::crashMapForIntPoint( IntPoint t_p )
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

CCPoint KSTargetAttackPattern7::spinTransform( CCPoint t_tp, CCPoint t_bp, float t_angle ) /* jack, boss, angle */
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

void KSTargetAttackPattern9::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSTargetAttackPattern9::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSTargetAttackPattern9::update( float dt )
{

}

void KSTargetAttackPattern12::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	scheduleUpdate();

	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_frame = 0;
	m_totalFrame = pattern.get("number", 5).asInt() * 20;
	m_pattern = pattern;
}

void KSTargetAttackPattern12::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	//		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCFadeOut::create(0.5f)}));
	//		m_parentMissile->removeFromParentAndCleanup(true);
	startSelfRemoveSchedule();
}

void KSTargetAttackPattern12::update( float dt )
{
	m_frame++;
	if(m_frame % 20 == 0)
	{
		// 쏨~
		ThrowBomb* gun = ThrowBomb::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), m_pattern);
		addChild(gun);
	}
	if(m_frame == m_totalFrame)
	{
		stopMyAction();
	}
}

void KSTargetAttackPattern13::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	m_cumber = cb;
	scheduleUpdate();

	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);

	m_frame = 0;
	m_totalFrame = 100;
	m_pattern = pattern;
}

void KSTargetAttackPattern13::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	//		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCFadeOut::create(0.5f)}));
	//		m_parentMissile->removeFromParentAndCleanup(true);
	startSelfRemoveSchedule();
}

void KSTargetAttackPattern13::update( float dt )
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

void KSSpecialAttackPattern1::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_totalFrame = pattern.get("totalframe", 300).asInt();
	m_scale = pattern.get("scale", 1.5f).asDouble();
	m_cumber = cb;
	m_position = t_sp;
	scheduleUpdate();
}

void KSSpecialAttackPattern1::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern1::update( float dt )
{
	AP_Missile21* t_m21 = AP_Missile21::create(m_position, m_totalFrame, m_scale);
	addChild(t_m21);

	stopMyAction();
}

void KSSpecialAttackPattern2::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern2::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern2::update( float dt )
{

}

void KSSpecialAttackPattern3::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern3::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern3::update( float dt )
{

}

void KSSpecialAttackPattern4::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern4::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern4::update( float dt )
{

}

void KSSpecialAttackPattern5::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern5::update( float dt )
{

}

void KSSpecialAttackPattern5::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern6::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern6::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern6::update( float dt )
{

}

void KSSpecialAttackPattern7::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern7::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern7::update( float dt )
{

}

void KSSpecialAttackPattern8::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern8::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern8::update( float dt )
{

}

void KSSpecialAttackPattern9::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern9::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern9::update( float dt )
{

}

void KSSpecialAttackPattern10::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern10::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern10::update( float dt )
{

}

void KSSpecialAttackPattern11::stopMyAction()
{
	unschedule(schedule_selector(ThisClassType::myAction));
	if (beamImg)
	{
		beamImg->removeFromParentAndCleanup(true);
		beamImg = 0;
	}

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern11::removeEffect()
{
	unschedule(schedule_selector(ThisClassType::myAction));

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ThisClassType::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);

	if(beamImg)
		beamImg->runAction(t_seq);
}

void KSSpecialAttackPattern11::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void KSSpecialAttackPattern11::myAction()
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

void KSSpecialAttackPattern11::startMyAction()
{
	ingFrame = 0;
	schedule(schedule_selector(ThisClassType::myAction));
}

void KSSpecialAttackPattern11::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
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

void KSSpecialAttackPattern11::update( float dt )
{

}

void KSSpecialAttackPattern12::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	area = pattern.get("area", 10).asInt();
	totalFrame = pattern.get("totalframe", 300).asInt();
	movingFrame = pattern.get("movingframe", 80).asInt();
	m_position = t_sp;
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern12::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern12::update( float dt )
{
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);

	PoisonDrop* t_pd = PoisonDrop::create(m_position, jackPosition, movingFrame, area, totalFrame);
	addChild(t_pd);
	t_pd->startAction();

	stopMyAction();
}

void KSSpecialAttackPattern13::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern13::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern13::update( float dt )
{

}

void KSSpecialAttackPattern14::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern14::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern14::update( float dt )
{

}

void KSSpecialAttackPattern15::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern15::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern15::update( float dt )
{

}

void KSSpecialAttackPattern16::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern16::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern16::update( float dt )
{

}

KSSpecialAttackPattern17::~KSSpecialAttackPattern17()
{
	CCLog("remove s 17");
}

void KSSpecialAttackPattern17::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_cumber = cb;
	m_totalFrame = pattern.get("totalframe", 180).asInt();
	m_speed = pattern.get("linespeed", 100).asInt();
	m_number = pattern.get("number", 4).asInt();
	scheduleUpdate();
}

void KSSpecialAttackPattern17::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern17::update( float dt )
{
	AlongOfTheLine* ap = AlongOfTheLine::create(m_cumber->getPosition(), ip2ccp(myGD->getJackPoint()), m_totalFrame, m_number, m_speed);
	addChild(ap);
	stopMyAction();
}

void KSSpecialAttackPattern18::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	m_pattern = pattern;
	m_cumber = cb;
	scheduleUpdate();
}

void KSSpecialAttackPattern18::stopMyAction()
{
	unscheduleUpdate();

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern18::update( float dt )
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

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

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

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

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

	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");

	startSelfRemoveSchedule();
}

void KSSpecialAttackPattern21::update( float dt )
{

}

Cobweb* Cobweb::create( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	Cobweb* t_m23 = new Cobweb();
	t_m23->myInit(t_sp, cb, patternData);
	t_m23->autorelease();
	return t_m23;
}

void Cobweb::updateCobweb()
{
	if(!is_stop)
		ingFrame = 0;
}

void Cobweb::startFrame()
{
	ingFrame = 0;
	schedule(schedule_selector(Cobweb::framing));
}

void Cobweb::framing()
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
				//					CCLog("-0.5");
				myGD->setAlphaSpeed(myGD->getAlphaSpeed() - 0.5f);
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
					myGD->setAlphaSpeed(myGD->getAlphaSpeed() - 0.5f);
					CCLog("-0.5");
					state = kInner;
				}
			}
			else // 밖
			{
				// 안에 있다가 나갔으면
				if(state == kInner)
				{
					myGD->setAlphaSpeed(myGD->getAlphaSpeed() + 0.5f);
					CCLog("+0.5");
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

void Cobweb::removeCobweb()
{
	cobwebImg->removeFromParent();
	startSelfRemoveSchedule();
}

void Cobweb::stopFrame()
{
	is_stop = true;
	unschedule(schedule_selector(Cobweb::framing));

	cobwebImg->stopAllActions();

	CCScaleTo* t_scale = CCScaleTo::create(0.3, 0.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Cobweb::removeCobweb));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	cobwebImg->runAction(t_seq);

	if(state == kInner)
	{
		myGD->setAlphaSpeed(myGD->getAlphaSpeed()+0.5f);
	}
	myGD->communication("MP_deleteKeepAP23");
	state = kFrameTerminated;
}

void Cobweb::myInit( CCPoint t_sp, KSCumberBase* cb, const std::string& patternData )
{
	cumber = cb;
	state = kElse;
	is_stop = false;
	Json::Reader reader;
	reader.parse(patternData, pattern);
	KS::KSLog("%", pattern);
	slowFrame = pattern.get("totalframe", 60*4).asInt();


	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader1 = new CCBReader(nodeLoader);
	cobwebImg = dynamic_cast<CCSprite*>(reader1->readNodeGraphFromFile("pattern_slowzone.ccbi",this));
	reader1->release();

	cobwebImg->setPosition(cumber->getPosition());
	cobwebImg->setScale(0.f);

	addChild(cobwebImg);
	m_scaleFromTo.init(0.0f, 1.0f, 0.3f);
	//		CCScaleTo* t_scale = CCScaleTo::create(0.3, 1.f);
	//		cobwebImg->runAction(t_scale); // 나중에 수동으로 구현해야함.

	//		myGD->setAlphaSpeed(myGD->getAlphaSpeed()-0.5f);

	startFrame();
}

PrisonPattern* PrisonPattern::create( CCPoint t_sp, float radius, int totalFrame )
{
	PrisonPattern* t_m28 = new PrisonPattern();
	t_m28->myInit(t_sp, radius, totalFrame);
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
}

void PrisonPattern::myInit( CCPoint t_sp, float radius, int totalFrame ) /* create 0.5 second */
{
	IntPoint jackPoint = myGD->getJackPoint();
	m_initialJackPosition = ip2ccp(jackPoint);
	m_radius = radius;
	m_totalFrame = totalFrame;
	auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_prison.ccbi");
	m_prisonSprite = ret.first;
	m_prisonSprite->setPosition(m_initialJackPosition);
	addChild(m_prisonSprite);
}
