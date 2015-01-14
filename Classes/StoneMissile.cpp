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


void PoisonedNiddle::update(float dt)
{
	m_jiggleInterval = MAX(0, m_jiggleInterval - 1);
	m_durationFrame--;
	if(m_durationFrame <= 0)
	{
		m_target->setPoisonedNiddle(nullptr);
		removeFromParent();
		return;
	}
	
	
	bool emptyMonster = !myGD->isValidMainCumber((CCNode*)m_target) && !myGD->isValidSubCumber((CCNode*)m_target);
	if(emptyMonster)
	{
		removeFromParent();
		return;
	}
	if(m_jiggleInterval == 0)
	{
		m_jiggleInterval = m_initJiggleInterval;
		CCPoint effectPosition = m_target->getPosition();
		effectPosition.x += rand()%21 - 10;
		effectPosition.y += rand()%21 - 10;
		CCPoint damagePosition = effectPosition;
		float damage = m_power;
		
		{
			KSCumberBase* cumber = m_target;
			// 캐스팅 캔슬.
			
			// directionAngle : Degree 단위.
			// 피격에니메이션.
			myGD->communication("MP_explosion", damagePosition, ccc4f(0, 0, 0, 0), 0.f);
			// 화면 번쩍 번쩍
			myGD->communication("VS_setLight");
			
			cumber->setDamageMeasure(cumber->getDamageMeasure() + damage );
			
			// 전체 피통의 10% 가 깎이면 캐스팅 취소함.
			bool castingCancelSign = false;
			if(cumber->getDamageMeasure() > cumber->getTotalHp() * 0.1f && (cumber->getAttackPattern() || cumber->getCharges().empty() == false))
			{
				castingCancelSign = true;
			}
			
			if(castingCancelSign)
			{
				myGD->communication("MP_bombCumber", (CCObject*)cumber); // with startMoving
				CCLog("cast cancel");
				
				cumber->setDamageMeasure(0.f);
				myGD->showDetailMessage("warning_boss_success.ccbi", "i"); 
				myGD->communication("UI_setIsCasting", false);
				myGD->communication("UI_castingCancel");
			}
			
			// 몬스터 리액션하라고.
			
			myGD->communication("CP_startDamageReaction", (CCNode*)cumber, damage, (float)(rand()%360-180.f), castingCancelSign,
													false); // damage : 555
			// 데미지 표시해주는 것. 데미지 숫자 뜸.
			myGD->communication("Main_showDamageMissile", damagePosition, (int)damage, (int)0);
			
			int combo_cnt = myGD->getCommunication("UI_getComboCnt");
			combo_cnt++;
			
			int damage_score = ((damage)/10*5+100)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d);//(100.f+damage)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d);
			int combo_score = (combo_cnt*10)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d); //damage_score*(combo_cnt-1);
			
			CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
			double score_rate = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_score_d, t_history.characterIndex.getV(), t_history.characterLevel.getV());
			if(score_rate < 1.0)
				score_rate = 1.0;
			score_rate -= 1.0;
			int sub_damage_score = damage_score*score_rate;
			int sub_combo_score = combo_score*score_rate;
			
			mySGD->damage_score = mySGD->damage_score.getV() + damage_score + sub_damage_score;
			mySGD->combo_score = mySGD->combo_score.getV() + combo_score + sub_combo_score;
			
			int addScore = damage_score + combo_score;
			
			myGD->communication("UI_addScore", addScore, sub_damage_score + sub_combo_score);
			myGD->communication("UI_setComboCnt", combo_cnt);
			myGD->communication("Main_showComboImage", damagePosition, combo_cnt);
			//				myGD->communication("Main_startShake", ks19937::getFloatValue(0, 360)); // 일단은 완전 랜덤으로.
		}
		
		// directionAngle : Degree 단위.
		// 피격에니메이션.
		myGD->communication("MP_explosion", damagePosition, ccc4f(0, 0, 0, 0), 0.f);
		// 화면 번쩍 번쩍
		myGD->communication("VS_setLight");
		// 데미지 표시해주는 것. 데미지 숫자 뜸.
		myGD->communication("Main_showDamageMissile", damagePosition, (int)damage, m_subPower);
		
		int combo_cnt = myGD->getCommunication("UI_getComboCnt");
		//combo_cnt++;
		
		int addScore = (100.f+damage)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d)*combo_cnt;
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		double score_rate = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_score_d, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		if(score_rate < 1.0)
			score_rate = 1.0;
		score_rate -= 1.0;
		int sub_score = addScore*score_rate;
		
		myGD->communication("UI_addScore", addScore, sub_score);
		//myGD->communication("UI_setComboCnt", combo_cnt);
		//myGD->communication("Main_showComboImage", damagePosition, combo_cnt);
		
		//			myGD->communication("Main_startShake", 0.f);
	}
}

void NoShockWave::update(float dt)
{
	m_durationFrame--;
	bool emptyMonster = !myGD->isValidMainCumber((CCNode*)m_target) && !myGD->isValidSubCumber((CCNode*)m_target);
	if(emptyMonster || m_durationFrame <= 0)
	{
		if(emptyMonster == false)
			m_target->setNoShockWave(nullptr);
		removeFromParent();
		return;
	}
}



void Silent::update(float dt)
{
	m_durationFrame--;
	
	bool emptyMonster = !myGD->isValidMainCumber((CCNode*)m_target) && !myGD->isValidSubCumber((CCNode*)m_target);
	if(emptyMonster || m_durationFrame <= 0)
	{
		if(emptyMonster == false)
		{
			m_target->setSilent(nullptr);
			m_target->setSlience(false);
		}
		
		removeFromParent();
		return;
	}
}

void StopTime::update(float dt)
{
	m_durationFrame--;
	bool emptyMonster = !myGD->isValidMainCumber((CCNode*)m_target) && !myGD->isValidSubCumber((CCNode*)m_target);
	if(emptyMonster || m_durationFrame <= 0)
	{
		if(emptyMonster == false)
			m_target->setStopTime(nullptr);
		bool haveStopTime = false;
		
		for(auto cumber : myGD->getMainCumberVector())
		{
			if(cumber->getStopTime())
			{
				haveStopTime = true;
				break;
			}
		}
		if(haveStopTime == false)
		{
			for(auto cumber : myGD->getSubCumberVector())
			{
				if(cumber->getStopTime())
				{
					haveStopTime = true;
					break;
				}
				
			}
		}
		if(haveStopTime == false)
		{
			myGD->communication("UI_resumeCounting");
		}

		removeFromParent();
		return;
	}
}
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
			
			float final_angle_rad = 360.f/final_cnt/180.f*M_PI;
			for(int i=0;i<final_cnt;i++)
			{
				StraightMissile* sm = StraightMissile::create(keep_position, m_fileName, m_initRad + final_angle_rad*i, 1.8f,
															  m_power, m_subPower, m_ao);
				
				addChild(sm);
				sm->beautifier(mm_level);
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
				CCPoint Pn = m_missileSprite->getPosition();//  + ccp( cosf(m_currentRad + 2 * M_PI / m_dotNumber * n) * m_initRadius * m_elipticA,
															//	   sinf(m_currentRad + 2 * M_PI / m_dotNumber * n) * m_initRadius * m_elipticB);
				
				
				
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
//				sm->beautifier(mm_level);
				
				sm->addChild(KSTimer::create(0.4f, [=](){sm->removeFromParent();}));
			}
			
		}
		
		if((m_missileSprite->getPosition() - m_initPosition).getLength() > m_length)
		{
			CCPoint keep_position = m_missileSprite->getPosition();
			
			float final_angle_rad = 360.f/final_cnt/180.f*M_PI;
			for(int i=0;i<final_cnt;i++)
			{
				StraightMissile* sm = StraightMissile::create(keep_position, m_fileName, m_initRad + final_angle_rad*i, 1.8f,
															  m_power, m_subPower, m_ao);
				
				addChild(sm);
				sm->beautifier(mm_level);
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
		return;
	}
	m_currentFrame++;
	
	
	
	
	if(m_missileSprite)
	{
		bool isNearMonster = false;
		KSCumberBase* nearCumber = nullptr;
		for(auto bosses : myGD->getMainCumberVector())
		{
			if(ccpLength(bosses->getPosition() - m_missileSprite->getPosition()) <= 60)
			{
				nearCumber = bosses;
				isNearMonster = true;
				break;
			}
		}
		for(auto mob : myGD->getSubCumberVector())
		{
			if(ccpLength(mob->getPosition() - m_missileSprite->getPosition()) <= 60)
			{
				nearCumber = mob;
				isNearMonster = true;
				break;
			}
		}
		
		// 유도하귀
		{
			
			
			float tt;
			if(isNearMonster)
			{
				
				float missile2MonsterRad = ccpToAngle((nearCumber->getPosition() - m_missileSprite->getPosition()));
				float Cangle = toPositiveAngle(toPositiveAngle(missile2MonsterRad) - toPositiveAngle(m_currentRad) );
				float deltaRad = Cangle;
				if(Cangle >= M_PI)
				{
					deltaRad = Cangle - 2 * M_PI;
				}
				//		float signRad = toPositiveAngle(diffRad) - toPositiveAngle(m_centerRad);
				m_currentRad += clampf(deltaRad, deg2Rad(-0.7f), deg2Rad(0.7f));
				
				
			}
			else
			{
				
			}
			
		}
		
		m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cosf(m_currentRad) * m_initSpeed, sin(m_currentRad) * m_initSpeed));
		
		
//		for(auto i : m_satellites)
		for(auto iter = m_satellites.begin(); iter != m_satellites.end(); )
		{
			Satellite& i = *iter;
			i.rad += M_PI / 180.f * 5.f;
			i.sprite->setMissilePosition(m_missileSprite->getPosition() + ccp(m_initRadius * cosf(i.rad), m_initRadius * sin(i.rad)));
			i.sprite->setRotation(rad2Deg(i.rad));
			
			
			bool isEnable = true;
			bool emptyMonster = false;
			bool invalidRange;
			IntPoint missilePoint = ccp2ip(i.sprite->getPosition());
			invalidRange = (missilePoint.x < mapLoopRange::mapWidthInnerBegin - 20 || missilePoint.x > mapLoopRange::mapWidthInnerEnd + 20 ||
											missilePoint.y < mapLoopRange::mapHeightInnerBegin -20 || missilePoint.y > mapLoopRange::mapHeightInnerEnd + 20);

			if(
				 myGD->getIsGameover() ||
				 emptyMonster ||
				 invalidRange ||
				 i.sprite->m_touched == true
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

void Boomerang::update(float dt)
{
	if(getChildrenCount() == 1) // 자식이 없어지면 삭쿠제.
	{
		removeFromParentAndCleanup(true);
		return;
	}
	
	if(m_missileStep == 1)
	{
		// 날아가는 구간
		m_centerRad += M_PI / 180.f * 3.f * 50.f / m_centerA;
		
		
		CCPoint tracer = ccp(cosf(m_initRad)*(m_centerA * cosf(m_centerRad) + m_centerA) - m_centerA / 4.f * sinf(m_centerRad) * sinf(m_initRad),
												 sinf(m_initRad)*(m_centerA * cosf(m_centerRad) + m_centerA) + cosf(m_initRad) * m_centerA / 4.f * sinf(m_centerRad));
		
		m_missileSprite->setPosition(m_params.initPosition + tracer);
		
		if(m_centerRad >= 2 * M_PI)
		{
			m_missileStep = 2;
			m_missileSprite->setColor(ccc3(255, 0, 0));
		}
	}
	else if(m_missileStep == 2)
	{
		// 돌아오는 구간
		// 미사일 중심에서 캐릭터로 방향 전환을 시도해야됨.
		// 방향 전환 기준은 m_centerRad 를 변화시키는 방향.
		
		float diffRad = atan2f(myGD->getJackPointCCP().y - m_missileSprite->getPosition().y,
													 myGD->getJackPointCCP().x - m_missileSprite->getPosition().x);
		// 					m_currentRad += clampf((tt - m_currentRad), deg2Rad(-2.5f), deg2Rad(2.5f));
		int sign = toPositiveAngle(diffRad) - toPositiveAngle(m_centerRad) > 0 ? 1 : -1;
		m_centerRad = toPositiveAngle(m_centerRad);
		float signRad = toPositiveAngle(diffRad) - toPositiveAngle(m_centerRad);
		m_centerRad += clampf(signRad, deg2Rad(-5.5f), deg2Rad(5.5f));
//		m_centerRad += (diffRad - m_centerRad);
		m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cosf(m_centerRad) * 1.5f, sinf(m_centerRad) * 1.5f));

		if( ccpLength(myGD->getJackPointCCP() - m_missileSprite->getPosition()) < 2 )
		{
			m_missileStep = 3;
			removeFromParent();
			return;
		}
		
		
	}
	
	
	m_revolutionRad += M_PI / 180.f * 8.f;
	for(auto iter = m_satellites.begin(); iter != m_satellites.end(); )
	{
		Satellite& i = *iter;
		//		iter.rad += M_PI / 180.f * 4.f;
		float tx = 0.f;
		float revolB = m_params.revelutionA / 2.f;
		float ty = -revolB;
		CCPoint tracer2 = ccp(cosf(m_revolutionRad)*(m_params.revelutionA * cosf(i.rad) + tx) - sinf(m_revolutionRad) * (revolB * sinf(i.rad) + ty) ,
													sinf(m_revolutionRad) * (m_params.revelutionA * cosf(i.rad) + tx) + cosf(m_revolutionRad) * (revolB * sinf(i.rad) + ty) );
		//		CCPoint tr = ccp(m_params.revelutionA * cosf(iter.rad), m_params.revelutionA / 2.f * sinf(iter.rad));
		i.sprite->setMissilePosition(m_missileSprite->getPosition() + tracer2);
		
		
		bool invalidRange;
		IntPoint missilePoint = ccp2ip(i.sprite->getMissilePosition());
		invalidRange = (missilePoint.x < mapLoopRange::mapWidthInnerBegin - 20 || missilePoint.x > mapLoopRange::mapWidthInnerEnd + 20 ||
										missilePoint.y < mapLoopRange::mapHeightInnerBegin -20 || missilePoint.y > mapLoopRange::mapHeightInnerEnd + 20);
		
		if(
			 myGD->getIsGameover() ||
			 
			 invalidRange ||
			 i.sprite->m_touched == true
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

void Chain::update(float dt)
{
	m_frameCount++;
	bool invalidRange;
	bool emptyMonster = !myGD->isValidMainCumber(m_targetNode) && !myGD->isValidSubCumber(m_targetNode);
	IntPoint missilePoint = ccp2ip(m_chainMissile->getMissilePosition());
	invalidRange = (missilePoint.x < mapLoopRange::mapWidthInnerBegin - 20 || missilePoint.x > mapLoopRange::mapWidthInnerEnd + 20 ||
									missilePoint.y < mapLoopRange::mapHeightInnerBegin -20 || missilePoint.y > mapLoopRange::mapHeightInnerEnd + 20);
	
	if(
		 myGD->getIsGameover() ||
		 emptyMonster ||
		 
		 invalidRange
		 
		 )
	{
		removeFromParent();
		return;
	}
	else if(m_chainMissile->m_touched == true)
	{
		CCPoint pos = m_chainMissile->getPosition();
		m_chainMissile->removeFromParent();
		m_params.depth--;
		if(m_params.depth >= 1)
		{
			KSCumberBase* nearCumber = myGD->getNearestCumberWithExclude(pos, m_targeted);
			
			// 가까이에 타겟이 있고 빈사상태가 아니면...
			if(nearCumber && ccpLength(pos - nearCumber->getPosition()) < m_params.chainDistance && nearCumber->getDeadState() == false)
			{
				m_targeted.push_back(nearCumber);
				float ny = nearCumber->getPosition().y;
				float nx = nearCumber->getPosition().x;
				
				m_targetNode = nearCumber;
				m_params.power *= 0.6f;
				m_chainMissile = StaticMissile::create(pos, m_params.fileName,
																							 m_params.power,
																							 m_params.subPower, 2, 5, m_params.ao);
				m_chainMissile->beautifier(m_params.level);
				addChild(m_chainMissile);
				m_currentRad = atan2f(ny - pos.y, nx - pos.x);
				
			}
			
			else
			{
				// 자기 자신 여러방...
				float ny = m_targetNode->getPosition().y;
				float nx = m_targetNode->getPosition().x;
				
//				m_targetNode = nearCumber;
				m_params.power *= 0.6f;
				m_chainMissile = StaticMissile::create(pos, m_params.fileName,
																							 m_params.power,
																							 m_params.subPower, 2, 1, m_params.ao);
				m_chainMissile->beautifier(m_params.level);
				addChild(m_chainMissile);
				m_currentRad = atan2f(ny - pos.y, nx - pos.x);

			}
			
		}
		else
		{
			removeFromParent();
		}
		return;
	}
	else
	{
		float missile2MonsterRad = ccpToAngle((m_targetNode->getPosition() - m_chainMissile->getMissilePosition()));
		float Cangle = toPositiveAngle(toPositiveAngle(missile2MonsterRad) - toPositiveAngle(m_currentRad) );
		float deltaRad = Cangle;
		if(Cangle >= M_PI)
		{
			deltaRad = Cangle - 2 * M_PI;
		}
//		float signRad = toPositiveAngle(diffRad) - toPositiveAngle(m_centerRad);
		m_currentRad += clampf(deltaRad, deg2Rad(-1.5f), deg2Rad(1.5f));
//		m_centerRad += clampf(signRad, deg2Rad(-5.5f), deg2Rad(5.5f));
//		m_currentRad += deltaRad;
		
		m_chainMissile->setMissilePosition(m_chainMissile->getMissilePosition() + ccp(cosf(m_currentRad) * m_params.speed, sinf(m_currentRad) * m_params.speed));
		m_chainMissile->setRotation(-rad2Deg(m_currentRad) - 90.f);

	
		CCPoint pos = m_chainMissile->getPosition();
		
		
//		KSCumberBase* nearCumber = myGD->getNearestCumberWithExclude(pos, m_targeted);
		// 목표물로 잡은 놈과 거의 붙었는데 기절상태라면
		// 기절 안한놈과 때리지 않았던 놈을 타겟으로 잡고 공격을 함.
		if(m_targetNode && ccpLength(pos - m_targetNode->getPosition()) < 2 && m_targetNode->getDeadState() == true)
		{
			m_chainMissile->removeFromParent();
			m_chainMissile = nullptr;
			
			auto tempTargets = m_targeted;
			tempTargets.push_back(m_targetNode);
			KSCumberBase* nearCumber = myGD->getNearestCumberWithExclude(pos, tempTargets);
			
			if(nearCumber && ccpLength(pos - nearCumber->getPosition()) < m_params.chainDistance && nearCumber->getDeadState() == false)
			{
				float ny = nearCumber->getPosition().y;
				float nx = nearCumber->getPosition().x;
				
				m_targetNode = nearCumber;
				m_chainMissile = StaticMissile::create(pos, m_params.fileName,
																							 m_params.power,
																							 m_params.subPower, 2, 5, m_params.ao);
				m_chainMissile->beautifier(m_params.level);
				addChild(m_chainMissile);
				m_currentRad = atan2f(ny - pos.y, nx - pos.x);
				
			}
			else
			{
				removeFromParent();
				return;
			}
		}
		
		
	}
	
	if(m_frameCount == 80)
	{
		
		addChild(KSGradualValue<float>::create(255.f, 0.f, 0.3f, [=](float t){
			KS::setOpacity(m_chainMissile, t);
		}, [=](float t) {
			KS::setOpacity(m_chainMissile, t);
			removeFromParent();
			
		}));
	}
	
	
	
	
	
	
}
#undef LZZ_INLINE
