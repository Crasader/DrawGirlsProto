// 
//

#include "JackMissile.h"
#include "AudioEngine.h"
#include "KSCumberBase.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"

#include "StoneMissile.h"
#define LZZ_INLINE inline
using namespace cocos2d;
using namespace std;

SpiritAttack* SpiritAttack::create(CCPoint initPosition, CCPoint goalPosition, const string& fileName, float tickCount, int power, int subPower,
														float speed, int coolFrame, AttackOption ao)
{
	SpiritAttack* ma = new SpiritAttack();
	ma->init(initPosition, goalPosition, fileName, tickCount, power, subPower, speed, coolFrame, ao);
	ma->autorelease();
	return ma;
}

bool SpiritAttack::init(CCPoint initPosition, CCPoint goalPosition, const string& fileName, int tickCount, int power, int subPower, float speed, int coolFrame, AttackOption ao)
{
	StoneAttack::init();
	m_fileName = fileName;
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
	m_initPosition = initPosition ;
	m_goalPosition = goalPosition ;
	m_tickCount = tickCount* mInfo.get("tickbonus", 1.f).asFloat();
	m_initTickCount = tickCount* mInfo.get("tickbonus", 1.f).asFloat();
	m_power = power;
	m_subPower = subPower;
	m_speed = speed;
	m_initCoolFrame = coolFrame * mInfo.get("intervalbonus", 1.f).asFloat();
	m_coolFrame = 0;
	m_mine = CCSprite::create(fileName.c_str()); // KS::loadCCBI<CCSprite*>(this, fileName).first;
	//		m_mine->setScale(1.f/myGD->game_scale);
	//		m_mine->setPosition(initPosition);
	m_mine->setScale(1.f/myGD->game_scale);
	m_particle = NULL;
	m_streak = NULL;
	m_option = ao;
	m_limitAttackCount = 3; // 3대 까지 때림
	m_currentAttackCount = 0;
	addChild(m_mine);
	addChild(KSGradualValue<CCPoint>::create(initPosition, goalPosition, 1.f,
																					 [=](CCPoint t){
																						 m_mine->setPosition(t);
																						 if(m_particle)
																						 {
																							 m_particle->setPosition(t);
																						 }
																						 if(m_streak)
																						 {
																							 m_streak->setPosition(t);
																						 }
																						 CCPoint diff = goalPosition - initPosition;
																						 float rad = atan2f(diff.y, diff.x);
																						 m_mine->setRotation(-rad2Deg(rad) - 90);
																					 },
																					 [=](CCPoint t){
																						 m_mine->setPosition(t);
																						 if(m_particle)
																						 {
																							 m_particle->setPosition(t);
																						 }
																						 if(m_streak)
																						 {
																							 m_streak->setPosition(t);
																						 }
																						 
																						 CCPoint diff = goalPosition - initPosition;
																						 float rad = atan2f(diff.y, diff.x);
																						 m_mine->setRotation(-rad2Deg(rad) - 90);
																						 scheduleUpdate();
																					 }));
	return true;
}
//__attribute__ ((noinline)) 
void SpiritAttack::update(float dt)
{
	// 여기서 몬스터와 부딪힌거 검사하면서 터짐.
	
	bool isEnable = true;
	bool emptyMonster = false;
	bool invalidRange = false;
	if(
		 myGD->getIsGameover() ||
		 emptyMonster ||
		 invalidRange
		 )
	{
		isEnable = false;
	}
	
	if(!isEnable)
	{
		removeFromParentAndCleanup(true);
		return;
	}
	
	m_tickCount -= 1.f/60.f;
	m_coolFrame = MAX(m_coolFrame - 1, 0);
	if(m_tickCount <= 0)
	{
		removeFromParent();
	}
	
	
	int surroundCnt = 0;
	IntPoint setPoint = ccp2ip(m_mine->getPosition());
	//if(setPoint.isInnerMap() && myGD->mapState[setPoint.x][setPoint.y] != mapEmpty)
	//surroundCnt = 4;
	
	IntPoint checkPoint = IntPoint(setPoint.x-1, setPoint.y);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] != mapEmpty)		surroundCnt++;
	checkPoint = IntPoint(setPoint.x+1, setPoint.y);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] != mapEmpty)		surroundCnt++;
	checkPoint = IntPoint(setPoint.x, setPoint.y-1);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] != mapEmpty)		surroundCnt++;
	checkPoint = IntPoint(setPoint.x, setPoint.y+1);
	if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] != mapEmpty)		surroundCnt++;
	if(surroundCnt >= 4) // 갇힘.
	{
		CCNode* parentNode = getParent();
		IntPoint mapPoint;
		bool found = myGD->getEmptyRandomPoint(&mapPoint, 5.f);
		if(found && !myGD->getIsGameover())
		{
			SpiritAttack* ma = SpiritAttack::create(m_mine->getPosition(), ip2ccp(mapPoint), m_fileName,
																							m_initTickCount, m_power, m_subPower, m_speed, m_coolFrame, m_option);
			ma->beautifier(m_level);
			parentNode->addChild(ma);
		}
		
		removeFromParent();
		return;
	}
	
	
	
	if(m_coolFrame == 0)
	{
		m_coolFrame = m_initCoolFrame;
		float minDistance = std::numeric_limits<float>::max();
		KSCumberBase* minDistanceCumber = nullptr;
		// 미사일과 몬스터와 거리가 2 보다 작은 경우가 있다면 폭발 시킴.
		bool found = false;
		for(auto iter : myGD->getMainCumberVector())
		{
			CCPoint targetPosition = iter->getPosition();
			float distance = ccpLength(targetPosition - m_mine->getPosition());
			if(distance < 25)
			{
				minDistance = distance;
				minDistanceCumber = iter;
				found = true;
				break;
			}
		}
		if(found == false)
		{
			for(auto iter : myGD->getSubCumberVector())
			{
				CCPoint targetPosition = iter->getPosition();
				float distance = ccpLength(targetPosition - m_mine->getPosition());
				if(distance < 25)
				{
					minDistance = distance;
					minDistanceCumber = iter;
					found = true;
					break;
				}
			}
		}
		
		if(found && !myGD->getIsGameover())
		{
			CCPoint effectPosition = m_mine->getPosition();
			effectPosition.x += rand()%21 - 10;
			effectPosition.y += rand()%21 - 10;
			
			float damage = m_power;
			executeOption(dynamic_cast<KSCumberBase*>(minDistanceCumber), damage, m_subPower, 0.f, effectPosition);
			m_currentAttackCount++;
			if(m_currentAttackCount == m_limitAttackCount)
			{
				m_tickCount = 0.f;
				
			}
			
			//removeFromParentAndCleanup(true);
		}
	}
	
	{ // 움직임.
		
		CCPoint afterPosition;
		IntPoint afterPoint;
		int changeDirection = ProbSelector::sel(0.05, 1.0 - 0.05, 0.0);
		if(changeDirection == 1)
		{
			m_directionRad += ks19937::getFloatValue(deg2Rad(-4), deg2Rad(+4));
		}
		
		
		int sela = ProbSelector::sel(0.005, 1.0 - 0.005, 0.0);
		if(sela == 0)
		{
			m_directionRad += ks19937::getFloatValue(deg2Rad(90), deg2Rad(270));
		}
		
		
		bool validPosition = false;
		int cnt = 0;
		bool onceOutlineAndMapCollision = false;
		bool pathFound = true; // 일단 찾았다고 셋 해놓고 특수한 경우에 false 시킴.
		
		
		while(!validPosition)
		{
			cnt++;
			float speedX = m_speed * cos(m_directionRad); // * (1 + cnt / 30.f * (3.f / (0.5f * MAX(0.1f, m_speed)) - 1));
			float speedY = m_speed * sin(m_directionRad); //  * (1 + cnt / 30.f * (3.f / (0.5f * MAX(0.1f, m_speed)) - 1));
			CCPoint spiritPosition = m_mine->getPosition();
			
			afterPosition = spiritPosition + ccp(speedX, speedY);
			afterPoint = ccp2ip(afterPosition);
			
			IntPoint checkPosition;
			int collisionCode = 0;
			if(afterPoint.isInnerMap() == false || myGD->mapState[afterPoint.x][afterPoint.y] == mapType::mapOldget ||
				 myGD->mapState[afterPoint.x][afterPoint.y] == mapType::mapOldline ||
				 myGD->mapState[afterPoint.x][afterPoint.y] == mapType::mapOutline)
			{
				collisionCode = 1;
			}
			else if(myGD->mapState[afterPoint.x][afterPoint.y] == mapType::mapNewline)
			{
				collisionCode = 2;
			}
			else if(myGD->mapState[afterPoint.x][afterPoint.y] == mapType::mapEmpty)
			{
				collisionCode = 3;
			}
			
			
			if(collisionCode == 1)
			{
				onceOutlineAndMapCollision = true;
				m_directionRad += ks19937::getFloatValue(deg2Rad(90), deg2Rad(270));
				
				if(m_directionRad < 0)                        m_directionRad += deg2Rad(360);
				else if(m_directionRad > deg2Rad(360))        m_directionRad -= deg2Rad(360);
			}
			else if(collisionCode == 2)
			{
				m_directionRad += ks19937::getFloatValue(deg2Rad(90), deg2Rad(270));
				if(m_directionRad < 0)                        m_directionRad += deg2Rad(360);
				else if(m_directionRad > deg2Rad(360))        m_directionRad -= deg2Rad(360);
			}
			else if(collisionCode == 3)
			{
				validPosition = true;
			}
			else
			{
				CCLOG("what!?");
				validPosition = true;
			}
			
			
			if(cnt >= 30)
			{
				pathFound = false;
				validPosition = true;
			}
		}
		if(pathFound)
		{
			m_mine->setPosition(afterPosition);
			if(m_particle)
			{
				m_particle->setPosition(afterPosition);
			}
			if(m_streak)
			{
				m_streak->setPosition(afterPosition);
			}
			
			m_mine->setRotation(-rad2Deg(m_directionRad) - 90);
		}
		
	}
}

void SpiritAttack::beautifier(int level)
{
	m_level = level;
	makeBeautifier(level, m_streak, m_particle);
	if(m_streak)addChild(m_streak, -1);
	if(m_particle)addChild(m_particle, -2);
}

void Tornado::update(float dt)
{
	if(getChildrenCount() == 0) // 자식이 없어지면 삭쿠제.
	{
		//			myGD->communication("EP_stopCrashAction");
		removeFromParentAndCleanup(true);
	}
	m_currentFrame++;
	
	bool isEnable = true;
	bool emptyMonster = false;
	bool invalidRange;
	if(m_missileSprite)
	{
		IntPoint missilePoint = ccp2ip(m_missileSprite->getPosition());
		invalidRange = (missilePoint.x < mapLoopRange::mapWidthInnerBegin - 20 || missilePoint.x > mapLoopRange::mapWidthInnerEnd + 20 ||
						missilePoint.y < mapLoopRange::mapHeightInnerBegin -20 || missilePoint.y > mapLoopRange::mapHeightInnerEnd + 20);
		
		if(invalidRange)
		{
			CCPoint keep_position = m_missileSprite->getPosition();
			
			int final_cnt = 10;
			float final_angle_rad = 360.f/final_cnt/180.f*M_PI;
			for(int i=0;i<final_cnt;i++)
			{
				StraightMissile* sm = StraightMissile::create(keep_position, m_fileName, m_initRad + final_angle_rad*i, 1.8f,
															  m_power, m_subPower, m_ao);
				
				addChild(sm);
			}
		}
		
	}
	else
	{
		invalidRange = false;
	}
	if(
			 myGD->getIsGameover() ||
			 emptyMonster ||
			 invalidRange
			 )
	{
		isEnable = false;
	}
	
	if(!isEnable)
	{
		if(m_missileSprite)
		{
			m_missileSprite->removeFromParentAndCleanup(true);
			
			m_missileSprite = nullptr;
			
			if(m_streak)
			{
				m_streak->removeFromParent();
				m_streak = nullptr;
			}
			if(m_particle)
			{
				
				m_particle->removeFromParent();
				m_particle = nullptr;
			}
			
			return;
			
		}
		
	}
	
	if(m_missileSprite)
	{
		m_missileSprite->setRotation(m_missileSprite->getRotation() + m_angleVelocity/m_frameInterval);
		m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cosf(m_initRad) * m_initSpeed, sin(m_initRad) * m_initSpeed));
		if(m_particle)
			m_particle->setPosition(m_missileSprite->getPosition());
		
		if(m_streak)
			m_streak->setPosition(m_missileSprite->getPosition());
		
		if(m_currentFrame % m_frameInterval == 0)
		{
			m_currentRad += m_angleVelocity;
			for(int n=0; n<m_dotNumber; n++)
			{
				CCPoint Pn = m_missileSprite->getPosition()  + ccp( cosf(m_currentRad + 2 * M_PI / m_dotNumber * n) * m_initRadius * m_elipticA,
																   sinf(m_currentRad + 2 * M_PI / m_dotNumber * n) * m_initRadius * m_elipticB);
				
				
				
				float tempRad = m_currentRad + (float)n * 2 * M_PI / (float)m_dotNumber;
				// tempRad 각인 타원의 접선의 기울기 구하기 위해... (미분한걸 아탄)
				float x = -m_elipticA * sinf(tempRad);
				float y = m_elipticB * cosf(tempRad);
				float elipticRad = atan2f(y, x) - M_PI + m_initRad;
				//				elipticRad += M_PI / 2.f;
				if(fabsf(atan2f(y, x) - M_PI) < 60.f/180.f*M_PI && rand()%7 < 6)
					continue;
				
				StraightMissile* sm = StraightMissile::create(Pn, m_fileName, elipticRad, 1.3f,
															  m_power, m_subPower, m_ao);
				
				addChild(sm);
			}
			
		}
		
		if((m_missileSprite->getPosition() - m_initPosition).getLength() > m_length)
		{
			CCPoint keep_position = m_missileSprite->getPosition();
			
			int final_cnt = 10;
			float final_angle_rad = 360.f/final_cnt/180.f*M_PI;
			for(int i=0;i<final_cnt;i++)
			{
				StraightMissile* sm = StraightMissile::create(keep_position, m_fileName, m_initRad + final_angle_rad*i, 1.8f,
															  m_power, m_subPower, m_ao);
				
				addChild(sm);
			}
			
			m_missileSprite->removeFromParent();
			m_missileSprite = nullptr;
			
			if(m_streak)
			{
				m_streak->removeFromParent();
				m_streak = nullptr;
			}
			if(m_particle)
			{
				
				m_particle->removeFromParent();
				m_particle = nullptr;
			}
		}
	}
}

////////////
void CircleDance::update(float dt)
{
	if(getChildrenCount() == 1) // 자식이 없어지면 삭쿠제.
	{
		//			myGD->communication("EP_stopCrashAction");
		removeFromParentAndCleanup(true);
	}
	m_currentFrame++;
	
	
	
	
	if(m_missileSprite)
	{
		m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cosf(m_initRad) * m_initSpeed, sin(m_initRad) * m_initSpeed));
		
		
//		for(auto i : m_satellites)
		for(auto iter = m_satellites.begin(); iter != m_satellites.end(); )
		{
			Satellite i = *iter;
			i.rad += M_PI / 180.f * 5.f;
			i.sprite->setPosition(m_missileSprite->getPosition() + ccp(m_initRadius * cosf(i.rad), m_initRadius * sin(i.rad)));
			
			
			bool isEnable = true;
			bool emptyMonster = false;
			bool invalidRange;
			IntPoint missilePoint = ccp2ip(i.sprite->getPosition());
			invalidRange = (missilePoint.x < mapLoopRange::mapWidthInnerBegin - 20 || missilePoint.x > mapLoopRange::mapWidthInnerEnd + 20 ||
											missilePoint.y < mapLoopRange::mapHeightInnerBegin -20 || missilePoint.y > mapLoopRange::mapHeightInnerEnd + 20);

			if(
				 myGD->getIsGameover() ||
				 emptyMonster ||
				 invalidRange
				 )
			{
				i.sprite->removeFromParent();
				iter = m_satellites.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		
		
		
		
		
	}

}
#undef LZZ_INLINE
