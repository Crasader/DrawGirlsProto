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
		g.bulletSpeed = 1.8f;
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

void KSTargetAttackPattern10::myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
{
	m_frame = 0;
	m_cumber = cb;
	scheduleUpdate();
	
	Json::Reader reader;
	Json::Value pattern;
	reader.parse(patternData, pattern);
	
	m_bombFrame = pattern.get("bombframe", 400).asInt();
	
	m_parentMissile = CCParticleSystemQuad::create("pm.plist");
	m_parentMissile->setPosition(cb->getPosition());
	addChild(m_parentMissile);
	
	CCPoint jackPosition = ip2ccp(myGD->getJackPoint());
	CCPoint subtract = jackPosition - cb->getPosition();
	float angle = atan2(subtract.y, subtract.x);
	m_dx = 2 * cos(angle);
	m_dy = 2 * sin(angle);
	
	//		m_parentMissile = CCParticleSystemQuad::createWithTotalParticles(150);
	//		///////**** Assignment Texture Filename!  ****///////
	//		CCTexture2D *texture= CCTextureCache::sharedTextureCache()->addImage("parent_missile.png");
	//		m_parentMissile->setTexture(texture);
	//		m_parentMissile->setEmissionRate(155.68);
	//		m_parentMissile->setAngle(90.0);
	//		m_parentMissile->setAngleVar(191.2);
	//		ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE};
	//		m_parentMissile->setBlendFunc(blendFunc);
	//		m_parentMissile->setDuration(-1.00);
	//		m_parentMissile->setEmitterMode(kCCParticleModeRadius);
	//		ccColor4F startColor={0.20,0.40,0.70,1.00};
	//		m_parentMissile->setStartColor(startColor);
	//		ccColor4F startColorVar={0.00,0.00,0.20,0.10};
	//		m_parentMissile->setStartColorVar(startColorVar);
	//		ccColor4F endColor={0.00,0.00,0.00,1.00};
	//		m_parentMissile->setEndColor(endColor);
	//		ccColor4F endColorVar={0.00,0.00,0.00,0.00};
	//		m_parentMissile->setEndColorVar(endColorVar);
	//		m_parentMissile->setStartSize(92.76);
	//		m_parentMissile->setStartSizeVar(43.09);
	//		m_parentMissile->endSize=-1.00;
	//		m_parentMissile->endSizeVar=0.00;
	//		m_parentMissile->rotatePerSecond=31.95;
	//		m_parentMissile->rotatePerSecondVar=121.78;
	//		m_parentMissile->startRadius=0.00;
	//		m_parentMissile->startRadiusVar=0.00;
	//		m_parentMissile->endRadius=0.00;
	//		m_parentMissile->totalParticles=150;
	//		m_parentMissile->life=0.96;
	//		m_parentMissile->lifeVar=1.00;
	//		m_parentMissile->startSpin=0.00;
	//		m_parentMissile->startSpinVar=0.00;
	//		m_parentMissile->endSpin=0.00;
	//		m_parentMissile->endSpinVar=0.00;
	//		m_parentMissile->position=ccp(382.32,129.75);
	//		m_parentMissile->posVar=ccp(0.00,0.00);
	//		addChild(m_parentMissile);
}




void KSTargetAttackPattern10::update(float dt)
{
	m_frame++;

	if(m_frame < m_bombFrame)
	{
		CCPoint t = m_parentMissile->getPosition();
		m_parentMissile->setPosition(ccp(t.x + m_dx, t.y + m_dy));
		crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), 10);
	}
	else if(m_frame < 700)
	{
		
		stopMyAction();
	}
	
}



