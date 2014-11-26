//
//  NormalAttackPattern.cpp
//  DGproto
//
//  Created by ksoo k on 2014. 11. 18..
//
//

#include "AttackPattern.h"
#include "SilhouetteData.h"
#include "MissileUnitCCB.h"

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








