//
#pragma once
#include "cocos2d.h"
#include "GameData.h"
#include "MissileDamageData.h"
#include "DataStorageHub.h"
#include "ServerDataSave.h"
#include <random>
#include "cocos-ext.h"
#include "KSCumberBase.h"
#include "KSUtil.h"
#include "ks19937.h"
#include "GameItemManager.h"
#include "StarGoldData.h"

enum AttackOption
{
	kNoOption = 0,
	kGold = 1 << 0,
	kStiffen = 1 << 1,
	kCancelCasting = 1 << 2,
	kMonsterSpeedDown = 1 << 3,
	kJackSpeedUp = 1 << 4,
	kUnbeatable = 1 << 5,
	kPoisonedNiddle = 1 << 6,
	kPlusScore = 1 << 7	
};
inline AttackOption operator|(AttackOption a, AttackOption b)
{return static_cast<AttackOption>(static_cast<int>(a) | static_cast<int>(b));}

//inline AttackOption operator&(AttackOption a, AttackOption b)
//{return static_cast<AttackOption>(static_cast<int>(a) & static_cast<int>(b));}

// startReaction 쪽에서 경직이 될지 안될지 결정하는 인자를 받아야 할 듯 하다.
class StoneAttack : public CCNode
{
public:
	void executeOption(KSCumberBase* cumber, float damage, float direction, CCPoint damagePosition)
	{
		// 옵션에 대해서 수행함.
		
		// directionAngle : Degree 단위.
		// 피격에니메이션.
		myGD->communication("MP_explosion", damagePosition, ccc4f(0, 0, 0, 0), 0.f);
		// 화면 번쩍 번쩍
		myGD->communication("VS_setLight");

		// 캐스팅 캔슬.

		if(m_option & AttackOption::kCancelCasting)
		{
			myGD->communication("MP_bombCumber", (CCObject*)cumber); // with startMoving
		}

		// 몬스터 리액션하라고.
		myGD->communication("CP_startDamageReaction", cumber, damage, direction, m_option & AttackOption::kCancelCasting,
												m_option & AttackOption::kStiffen); // damage : 555

		// 데미지 표시해주는 것. 데미지 숫자 뜸.
		myGD->communication("Main_showDamageMissile", damagePosition, (int)damage);

		int combo_cnt = myGD->getCommunication("UI_getComboCnt");
		combo_cnt++;

		int addScore = (100.f+damage)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d)*combo_cnt;
		if(m_option & AttackOption::kPlusScore)
		{
			addScore *= 1.1f;
		}
		myGD->communication("UI_addScore", addScore);
		myGD->communication("UI_setComboCnt", combo_cnt);
		myGD->communication("Main_showComboImage", damagePosition, combo_cnt);

		myGD->communication("Main_startShake", 0.f);
		if((m_option & AttackOption::kGold))
		{
			FeverCoin* t_fc = FeverCoin::create(ccp2ip(cumber->getPosition()), nullptr, nullptr);
			getParent()->addChild(t_fc, 5);
			t_fc->startRemove();
			mySGD->setGold(mySGD->getGold() + 1);
		}
		if(m_option & AttackOption::kMonsterSpeedDown)
		{
			// 몬스터 속도 하락시킴. n 초간 p% 감소하는 형태.
		}
		if(m_option & AttackOption::kPoisonedNiddle)
		{
			// 특정 간격으로 데미지를 깎는다. 부가 기능은  ㄴ ㄴ해.
		}
		if(m_option & AttackOption::kJackSpeedUp)
		{
			// 영역 생성하여 그 안에서는 잭의 속도가 빠름.
		}
		if(m_option & AttackOption::kUnbeatable)
		{
			// 영역 생성하여 그 안에서는 무적... 
		}
	}
protected:
	AttackOption m_option;
};


class GuidedMissile : public StoneAttack
{
public:
	static GuidedMissile* create(CCNode* targetNode, CCPoint initPosition, float initSpeed, int power, int range, AttackOption ao)
	{
		GuidedMissile* object = new GuidedMissile();
		object->init(targetNode, initPosition, initSpeed, power, range, ao);

		object->autorelease();
		

		return object;
	}
	bool init(CCNode* targetNode, CCPoint initPosition, float initSpeed, int power, int range, AttackOption ao)
	{
		StoneAttack::init();
		m_initSpeed = initSpeed;
		m_option = ao;
		m_power = power;
		m_targetNode = targetNode;	
		m_guided = false;
		m_range = range;
		m_missileSprite = CCSprite::create("jm_plasma1_particle.png"); // 0 도가 위방향.
		addChild(m_missileSprite);
		m_missileSprite->setPosition(initPosition);
		CCPoint diff = m_targetNode->getPosition() - initPosition;
		m_initRad = atan2f(diff.y, diff.x);
		scheduleUpdate();


		return true;
	}
	void update(float dt)
	{
		bool isEnable = true;
		bool emptyMonster = !myGD->isValidMainCumber(m_targetNode) && !myGD->isValidSubCumber(m_targetNode);
		IntPoint missilePoint = ccp2ip(m_missileSprite->getPosition());
		bool invalidRange = (missilePoint.x < mapLoopRange::mapWidthInnerBegin - 20 || missilePoint.x > mapLoopRange::mapWidthInnerEnd + 20 ||
											 missilePoint.y < mapLoopRange::mapHeightInnerBegin -20 || missilePoint.y > mapLoopRange::mapHeightInnerEnd + 20);
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

		CCPoint targetPosition = m_targetNode->getPosition();
		CCPoint subDistance = ccpSub(targetPosition, m_missileSprite->getPosition());
		float distance = sqrtf(powf(subDistance.x, 2.f) + powf(subDistance.y, 2.f));

		// 몬스터가 맞는 조건
		if(distance <= 2)
		{
			CCPoint effectPosition = m_missileSprite->getPosition();
			effectPosition.x += rand()%21 - 10;
			effectPosition.y += rand()%21 - 10;
			
			float damage = m_power;
			executeOption(dynamic_cast<KSCumberBase*>(m_targetNode), damage, 0.f, effectPosition);

			removeFromParentAndCleanup(true);
		}
		else  // 거리가 멀면 몬스터 쪽으로 유도함.
		{
			CCPoint missilePosition = m_missileSprite->getPosition();
			CCPoint cumberPosition;
			cumberPosition = m_targetNode->getPosition();		
			CCPoint diffPosition = cumberPosition - missilePosition;
			if(m_guided == false)
			{
				m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cos(m_initRad) * m_initSpeed,
																																				sin(m_initRad) * m_initSpeed));

				for(auto bosses : myGD->getMainCumberVector())
				{
					if(ccpLength(bosses->getPosition() - m_missileSprite->getPosition()) <= m_range)
					{
						m_targetNode = bosses; // 목표 설정
						m_guided = true;
						break;
					}
				}
				for(auto mob : myGD->getSubCumberVector())
				{
					if(ccpLength(mob->getPosition() - m_missileSprite->getPosition()) <= m_range)
					{
						m_targetNode = mob; // 목표 설정
						m_guided = true;
						break;
					}
				}
			}
			else
			{
				float tt = atan2f(diffPosition.y, diffPosition.x);
				m_currentRad += clampf(tt - m_currentRad, deg2Rad(-15), deg2Rad(15));
				m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cos(m_currentRad) * m_initSpeed * 2,
																																					sin(m_currentRad) * m_initSpeed * 2)); }
		}
	}
protected:
	float m_initSpeed; // 초기 속도.
	float m_initRad; // 처음에 날아가는 각도.
	float m_currentRad; // 범위내 들어왔을 때 현재 각도.
	int m_power; // 파워.
	bool m_guided; // 유도 모드인지 여부.
	int m_range; // 유도 범위.
	CCNode* m_targetNode;
	CCSprite* m_missileSprite; // 미사일 객체.
};



class StraightMissile : public StoneAttack
{
public:
	static StraightMissile* create(CCPoint initPosition, float rad, float initSpeed, int power, AttackOption ao)
	{
		StraightMissile* object = new StraightMissile();
		object->init(initPosition, rad, initSpeed, power, ao);

		object->autorelease();
		

		return object;
	}
	bool init(CCPoint initPosition, float rad, float initSpeed, int power, AttackOption ao)
	{
		StoneAttack::init();
		m_initSpeed = initSpeed;
		m_power = power;
		m_option = ao;
		m_missileSprite = CCSprite::create("jm_plasma2_main.png"); // 0 도가 위방향.
		addChild(m_missileSprite);
		m_missileSprite->setPosition(initPosition);
		m_initRad = rad;
		scheduleUpdate();


		return true;
	}
	void update(float dt)
	{
		bool isEnable = true;
		IntPoint missilePoint = ccp2ip(m_missileSprite->getPosition());
		bool invalidRange = (missilePoint.x < mapLoopRange::mapWidthInnerBegin - 20 || missilePoint.x > mapLoopRange::mapWidthInnerEnd + 20 ||
											 missilePoint.y < mapLoopRange::mapHeightInnerBegin -20 || missilePoint.y > mapLoopRange::mapHeightInnerEnd + 20);
		if(
			 myGD->getIsGameover() ||
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

		float minDistance = std::numeric_limits<float>::max();
		KSCumberBase* minDistanceCumber = nullptr;
		// 미사일과 몬스터와 거리가 2 보다 작은 경우가 있다면 폭발 시킴.
		bool found = false;
		for(auto iter : myGD->getMainCumberVector())
		{
			CCPoint targetPosition = iter->getPosition();
			float distance = ccpLength(targetPosition - m_missileSprite->getPosition());
			if(distance < 2)
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
				float distance = ccpLength(targetPosition - m_missileSprite->getPosition());
				if(distance < 2)
				{
					minDistance = distance;
					minDistanceCumber = iter;
					found = true;
					break;
				}
			}	
		}

		// 몬스터가 맞는 조건
		if(found)
		{
			CCPoint effectPosition = m_missileSprite->getPosition();
			effectPosition.x += rand()%21 - 10;
			effectPosition.y += rand()%21 - 10;
			
			float damage = m_power;
			executeOption(dynamic_cast<KSCumberBase*>(minDistanceCumber), damage, 0.f, effectPosition);
			removeFromParentAndCleanup(true);
		}
		else  // 거리가 멀면 진행 시킴.
		{
			m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cos(m_initRad) * m_initSpeed,
																																				sin(m_initRad) * m_initSpeed));
		}
	}
protected:
	float m_initSpeed; // 초기 속도.
	float m_initRad; // 처음에 날아가는 각도.
	int m_power; // 파워.
	CCSprite* m_missileSprite; // 미사일 객체.
};














//SpreadMissile 적용할 차례.

////////////////////////////////////////////////////
class SpreadMissile : public StoneAttack
{
public:
	static SpreadMissile* create(KSCumberBase* targetNode, CCPoint initPosition, float initSpeed, int power, int directions, AttackOption ao)
	{
		SpreadMissile* object = new SpreadMissile();
		object->init(targetNode, initPosition, initSpeed, power, directions, ao);

		object->autorelease();
		

		return object;
	}
	virtual ~SpreadMissile()
	{
		CCLog("Spread ~");
	}
	bool init(KSCumberBase* targetNode, CCPoint initPosition, float initSpeed, int power, int directions, AttackOption ao)
	{
		StoneAttack::init();
		m_initSpeed = initSpeed;
		m_power = power;
		m_targetNode = targetNode;
		m_option = ao;	
		CCPoint diff = targetNode->getPosition() - initPosition;
		float rad = atan2f(diff.y, diff.x);
		for(int i=0; i<directions; i++)
		{
			StraightMissile* sm = StraightMissile::create(initPosition, rad + i * deg2Rad(360 / directions), 2.f, 33.f, ao);
			addChild(sm);
		}
		
		scheduleUpdate();
		return true;
	}
	void update(float dt)
	{
		if(getChildrenCount() <= 0)
		{
			removeFromParent();
		}
	}
protected:
	float m_initSpeed; // 초기 속도.
	int m_power; // 파워.
	int m_directions; // 몇 방향인지...
	bool m_cancelCasting;
	bool m_stiffen;
	CCNode* m_targetNode;
};

class MineAttack : public StoneAttack
{
public:
	static MineAttack* create(CCPoint initPosition, CCPoint goalPosition, float tickCount, int power, AttackOption ao)
	{
		MineAttack* ma = new MineAttack();
		ma->init(initPosition, goalPosition, tickCount, power, ao);
		ma->autorelease();
		return ma;
	}	

	bool init(CCPoint initPosition, CCPoint goalPosition, int tickCount, int power, AttackOption ao)
	{
		StoneAttack::init();
		m_initPosition = initPosition;
		m_goalPosition = goalPosition;
		m_tickCount = tickCount;
		m_initTickCount = tickCount;
		m_power = power;
		m_option = ao;	
		m_mine = CCSprite::create("jm_wind_edge.png");	
		addChild(m_mine);
		addChild(KSGradualValue<CCPoint>::create(initPosition, goalPosition, 1.f,
																				 [=](CCPoint t){
																					 m_mine->setPosition(t);
																					
																				 },
																		[=](CCPoint t){
																			m_mine->setPosition(t);

																			scheduleUpdate();
																		}));			
		return true;
	}
	void update(float dt)
	{
		// 여기서 몬스터와 부딪힌거 검사하면서 터짐.
		m_tickCount -= 1.f/60.f;

		if(m_tickCount <= 0)
		{
			removeFromParent();
		}

		int surroundCnt = 0;
		IntPoint setPoint = ccp2ip(m_mine->getPosition());
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
			removeFromParent();
			IntPoint mapPoint;
			bool found = myGD->getEmptyRandomPoint(&mapPoint, 5.f);
			if(found)
			{
				MineAttack* ma = MineAttack::create(m_mine->getPosition(), ip2ccp(mapPoint), m_initTickCount, m_power, m_option);
				parentNode->addChild(ma);
			}

			return;
		}

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


		if(found)
		{
			CCPoint effectPosition = m_mine->getPosition();
			effectPosition.x += rand()%21 - 10;
			effectPosition.y += rand()%21 - 10;
			
			float damage = m_power;
			executeOption(dynamic_cast<KSCumberBase*>(minDistanceCumber), damage, 0.f, effectPosition);

			removeFromParentAndCleanup(true);
		}
	}
protected:
	CCPoint m_initPosition;
	CCPoint m_goalPosition;
	float m_tickCount;
	float m_initTickCount;
	int m_power;
	CCSprite* m_mine;
};



class SpiritAttack : public StoneAttack
{
public:
	static SpiritAttack* create(CCPoint initPosition, CCPoint goalPosition, float tickCount, int power, float speed, int coolFrame, AttackOption ao)
	{
		SpiritAttack* ma = new SpiritAttack();
		ma->init(initPosition, goalPosition, tickCount, power, speed, coolFrame, ao);
		ma->autorelease();
		return ma;
	}	

	bool init(CCPoint initPosition, CCPoint goalPosition, int tickCount, int power, float speed, int coolFrame, AttackOption ao)
	{
		StoneAttack::init();
		m_initPosition = initPosition;
		m_goalPosition = goalPosition;
		m_tickCount = tickCount;
		m_initTickCount = tickCount;
		m_power = power;
		m_speed = speed;	
		m_initCoolFrame = coolFrame;
		m_coolFrame = 0; 
		m_mine = CCSprite::create("jm_wind1_main.png");	
		m_option = ao;
		addChild(m_mine);
		addChild(KSGradualValue<CCPoint>::create(initPosition, goalPosition, 1.f,
																				 [=](CCPoint t){
																					 m_mine->setPosition(t);
																					
																				 },
																		[=](CCPoint t){
																			m_mine->setPosition(t);

																			scheduleUpdate();
																		}));			
		return true;
	}
	void update(float dt)
	{
		// 여기서 몬스터와 부딪힌거 검사하면서 터짐.
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
			removeFromParent();
			IntPoint mapPoint;
			bool found = myGD->getEmptyRandomPoint(&mapPoint, 5.f);
			if(found)
			{
				SpiritAttack* ma = SpiritAttack::create(m_mine->getPosition(), ip2ccp(mapPoint), m_initTickCount, m_power, m_speed, m_coolFrame, m_option);
				parentNode->addChild(ma);
			}

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


			if(found)
			{
				CCPoint effectPosition = m_mine->getPosition();
				effectPosition.x += rand()%21 - 10;
				effectPosition.y += rand()%21 - 10;

				float damage = m_power;
				executeOption(dynamic_cast<KSCumberBase*>(minDistanceCumber), damage, 0.f, effectPosition);

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
					CCLog("what!?");
					validPosition = true;
				}


				if(cnt >= 30)
				{
					pathFound = false;
					validPosition = true;
				}
			}
			if(pathFound)
				m_mine->setPosition(afterPosition);
	
		}
	}
protected:
	CCPoint m_initPosition;
	CCPoint m_goalPosition;
	float m_tickCount;
	float m_initTickCount;
	int m_power;
	CCSprite* m_mine;
	float m_directionRad;
	float m_speed;
	int m_coolFrame; // 0 이어야 타격이 가능함. 공격할 때 initCoolFrame 으로 초기화되고 매 프레임당 -- 됨.
	int m_initCoolFrame;
};


class RangeAttack : public StoneAttack
{
public:
	static RangeAttack* create(CCPoint initPosition, float radius, int durationFrame, int power, int jiggleInterval, AttackOption ao)
	{
		RangeAttack* obj = new RangeAttack();
		obj->init(initPosition, radius, durationFrame, power, jiggleInterval, ao);
		obj->autorelease();
		return obj;
	}
	bool init(CCPoint initPosition, float radius, int durationFrame, int power, int jiggleInterval, AttackOption ao)
	{
		StoneAttack::init();

		m_radius = radius;
		m_durationFrame = durationFrame;
		m_power = power;
		m_initJiggleInterval = jiggleInterval;
		m_jiggleInterval = 0;
		m_option = ao;
		CCSprite* spr = CCSprite::create("rcircle.png"); // width : 100
		spr->setPosition(initPosition);
		spr->setScale(radius / 50.f);
		spr->setOpacity(100);
		addChild(spr);
		m_rangeSprite = spr;
		scheduleUpdate();
		return true;	
	}
	void update(float dt)
	{
		m_jiggleInterval = MAX(0, m_jiggleInterval - 1);
		m_durationFrame--;
		if(m_durationFrame <= 0)
		{
			removeFromParent();
			return;
		}

		
		if(m_jiggleInterval == 0)
		{
			m_jiggleInterval = m_initJiggleInterval;
			// 미사일과 몬스터와 거리가 2 보다 작은 경우가 있다면 폭발 시킴.
			std::vector<KSCumberBase*> nearMonsters;
			for(auto iter : myGD->getMainCumberVector())
			{
				CCPoint targetPosition = iter->getPosition();
				float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
				if(distance < m_radius)
				{
					nearMonsters.push_back(iter);
				}
			}	
			for(auto iter : myGD->getSubCumberVector())
			{
				CCPoint targetPosition = iter->getPosition();
				float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
				if(distance < m_radius)
				{
					nearMonsters.push_back(iter);
				}
			}	

			for(auto iter : nearMonsters)
			{
				CCPoint effectPosition = iter->getPosition();
				effectPosition.x += rand()%21 - 10;
				effectPosition.y += rand()%21 - 10;

				float damage = m_power;
				executeOption(dynamic_cast<KSCumberBase*>(iter), damage, 0.f, effectPosition);
				//removeFromParentAndCleanup(true);
			}
		}	
	}
protected:
	float m_radius;
	float m_durationFrame;
	int m_power;
	int m_jiggleInterval;
	int m_initJiggleInterval;		
	CCSprite* m_rangeSprite;
};


class RandomBomb : public StoneAttack
{
public:
	static RandomBomb* create(float radius, int power, AttackOption ao)
	{
		RandomBomb* obj = new RandomBomb();
		obj->init(radius, power, ao);
		obj->autorelease();
		return obj;
	}
	bool init(float radius, int power, AttackOption ao)
	{
		StoneAttack::init();

		m_radius = radius;
		m_power = power;
		m_option = ao;
		CCPoint randomPosition = ip2ccp(IntPoint(ks19937::getIntValue(mapLoopRange::mapWidthInnerBegin, mapLoopRange::mapWidthInnerEnd - 1),
																						 ks19937::getIntValue(mapLoopRange::mapHeightInnerBegin, mapLoopRange::mapHeightInnerEnd - 1)));

		CCSprite* spr = KS::loadCCBI<CCSprite*>(this, "fx_bossbomb.ccbi").first;
		
		spr->setPosition(randomPosition);
		spr->setScale(1.f);
		spr->setOpacity(100);
		addChild(spr);
		m_rangeSprite = spr;

		
		std::vector<KSCumberBase*> nearMonsters;
		for(auto iter : myGD->getMainCumberVector())
		{
			CCPoint targetPosition = iter->getPosition();
			float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
			if(distance < m_radius)
			{
				nearMonsters.push_back(iter);
			}
		}	
		for(auto iter : myGD->getSubCumberVector())
		{
			CCPoint targetPosition = iter->getPosition();
			float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
			if(distance < m_radius)
			{
				nearMonsters.push_back(iter);
			}
		}	

		for(auto iter : nearMonsters)
		{
			CCPoint effectPosition = iter->getPosition();
			effectPosition.x += rand()%21 - 10;
			effectPosition.y += rand()%21 - 10;

			float damage = m_power;
			executeOption(dynamic_cast<KSCumberBase*>(iter), damage, 0.f, effectPosition);
			//removeFromParentAndCleanup(true);
		}
		addChild(KSTimer::create(1.f, [=]()
														 {
															 removeFromParent();
														 }));
		//scheduleUpdate();
		return true;	
	}
protected:
	float m_radius;
	int m_power;
	CCSprite* m_rangeSprite;
};













