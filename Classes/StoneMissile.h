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

class PoisonedNiddle : public CCNode
{
public:
	static PoisonedNiddle* create(KSCumberBase* target, int durationFrame, int power)
	{
		PoisonedNiddle* obj = new PoisonedNiddle();
		obj->init(target, durationFrame, power);
		obj->autorelease();
		return obj;
	}
	bool init(KSCumberBase* target, int durationFrame, int power)
	{
		m_durationFrame = durationFrame;
		m_power = power;
		m_initJiggleInterval = 30;
		m_jiggleInterval = 0;
		m_target = target;
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

		
		bool emptyMonster = !myGD->isValidMainCumber(m_target) && !myGD->isValidSubCumber(m_target);
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


			// directionAngle : Degree 단위.
			// 피격에니메이션.
			myGD->communication("MP_explosion", damagePosition, ccc4f(0, 0, 0, 0), 0.f);
			// 화면 번쩍 번쩍
			myGD->communication("VS_setLight");
			// 데미지 표시해주는 것. 데미지 숫자 뜸.
			myGD->communication("Main_showDamageMissile", damagePosition, (int)damage);

			int combo_cnt = myGD->getCommunication("UI_getComboCnt");
			//combo_cnt++;

			int addScore = (100.f+damage)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d)*combo_cnt;
			myGD->communication("UI_addScore", addScore);
			//myGD->communication("UI_setComboCnt", combo_cnt);
			//myGD->communication("Main_showComboImage", damagePosition, combo_cnt);

			myGD->communication("Main_startShake", 0.f);
		}	
	}
protected:
	KSCumberBase* m_target;
	float m_durationFrame;
	int m_power;
	int m_jiggleInterval;
	int m_initJiggleInterval;		
};

class MonsterSpeedDownZone : public CCNode
{
public:
	static MonsterSpeedDownZone* create(CCPoint initPosition, float radius, int durationFrame, int power)
	{
		MonsterSpeedDownZone* obj = new MonsterSpeedDownZone();
		obj->init(initPosition, radius, durationFrame, power);
		obj->autorelease();
		return obj;
	}
	bool init(CCPoint initPosition, float radius, int durationFrame, int power)
	{

		m_radius = radius;
		m_durationFrame = durationFrame;
		m_power = power;
		CCSprite* spr = KS::loadCCBI<CCSprite*>(this, "me_scope.ccbi").first;
		spr->setPosition(initPosition);
		spr->setScale(radius / 100.f);
		spr->setOpacity(100);
		addChild(spr);
		m_rangeSprite = spr;
		scheduleUpdate();
		return true;	
	}
	void update(float dt)
	{
		m_durationFrame--;
		if(m_durationFrame <= 0)
		{
			for(auto iter : myGD->getMainCumberVector())
			{
				if(m_applied[iter] == true)
				{
					iter->setSpeedRatioForStone(nullptr, 1.f);
				}
			}	
			for(auto iter : myGD->getSubCumberVector())
			{
				if(m_applied[iter] == true)
				{
					iter->setSpeedRatioForStone(this, 1.f);
				}
			}	
			removeFromParent();
			return;
		}

		setCumberSpeedAdjust();
	}
	void setCumberSpeedAdjust()
	{
		std::vector<KSCumberBase*> nearMonsters;

		std::vector<KSCumberBase*> farMonsters;
		for(auto iter : myGD->getMainCumberVector())
		{
			CCPoint targetPosition = iter->getPosition();
			float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
			if(distance < m_radius)
			{
				nearMonsters.push_back(iter);
			}
			else
			{
				farMonsters.push_back(iter);
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
			else
			{
				farMonsters.push_back(iter);
			}
		}	

		for(auto iter : nearMonsters)
		{
			iter->setSpeedRatioForStone(nullptr, 0.3f);
			m_applied[iter] = true;
			//removeFromParentAndCleanup(true);
		}
		for(auto iter : farMonsters)
		{
			if(m_applied[iter] == true)
			{
				iter->setSpeedRatioForStone(nullptr, 1.f);
			}
		}	
	}
protected:
	float m_radius;
	float m_durationFrame;
	int m_power;
	map<CCNode*, bool> m_applied; // 몬스터가 적용이 됐는지.
	CCSprite* m_rangeSprite;
};


class JackSpeedUpZone : public CCNode
{
public:
	static JackSpeedUpZone* create(CCPoint initPosition, float radius, int durationFrame, int power)
	{
		JackSpeedUpZone* obj = new JackSpeedUpZone();
		obj->init(initPosition, radius, durationFrame, power);
		obj->autorelease();
		return obj;
	}
	bool init(CCPoint initPosition, float radius, int durationFrame, int power)
	{

		m_radius = radius;
		m_durationFrame = durationFrame;
		m_power = power;
		m_applied = false;
		CCSprite* spr = KS::loadCCBI<CCSprite*>(this, "me_speedzone.ccbi").first; // width : 100
		spr->setPosition(initPosition);
		spr->setColor(ccc3(0, 255, 0));
		spr->setScale(radius / 100.f);
		spr->setOpacity(100);
		addChild(spr);
		m_rangeSprite = spr;
		scheduleUpdate();
		return true;	
	}
	void update(float dt)
	{
		m_durationFrame--;
		if(m_durationFrame <= 0)
		{
			// 잭의 속도를 원래대로 돌려놓음.
			if(m_applied)
			{
				myGD->setAlphaSpeed(myGD->getAlphaSpeed() - 0.3f);
				m_applied = false;
			}
			removeFromParent();
			return;
		}

		CCPoint targetPosition = ip2ccp(myGD->getJackPoint());
		float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
		if(distance < m_radius)
		{
			// 속도업에 잭이 있음.
			if(m_applied == false)
			{
				myGD->setAlphaSpeed(myGD->getAlphaSpeed() + 0.3f);
				m_applied = true;
			}
		}
		else
		{
			// 속도업에 잭이 없음. 
			if(m_applied)
			{
				myGD->setAlphaSpeed(myGD->getAlphaSpeed() - 0.3f);
				m_applied = false;
			}
		}
	}
protected:
	float m_radius;
	float m_durationFrame;
	int m_power;
	bool m_applied;
	CCSprite* m_rangeSprite;
};

class JackUnbeatableZone : public CCNode
{
public:
	static JackUnbeatableZone* create(CCPoint initPosition, float radius, int durationFrame)
	{
		JackUnbeatableZone* obj = new JackUnbeatableZone();
		obj->init(initPosition, radius, durationFrame);
		obj->autorelease();
		return obj;
	}
	bool init(CCPoint initPosition, float radius, int durationFrame)
	{

		m_radius = radius;
		m_durationFrame = durationFrame;
		CCSprite* spr = KS::loadCCBI<CCSprite*>(this, "me_shieldzone.ccbi").first; // width : 100
		spr->setPosition(initPosition);
		spr->setColor(ccc3(0, 0, 255));
		spr->setScale(radius / 100.f);
		spr->setOpacity(100);
		addChild(spr);
		m_rangeSprite = spr;
		scheduleUpdate();
		return true;	
	}
	void update(float dt)
	{
		m_durationFrame--;
		if(m_durationFrame <= 0)
		{
			// 잭의 무적을 해제함.
			
			if(m_applied)
			{
				m_applied = false;

				myGD->setJackIsUnbeatable(false);
			}
			removeFromParent();
			return;
		}

		CCPoint targetPosition = ip2ccp(myGD->getJackPoint());
		float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
		if(distance < m_radius)
		{
			// 잭이 무적
			if(m_applied == false)
			{
				m_applied = true;
				myGD->setJackIsUnbeatable(true);
			}
		}
		else
		{
			// 무적을 해제.
			if(m_applied)
			{
				m_applied = false;
				myGD->setJackIsUnbeatable(false);
			}
		}
	}
protected:
	float m_radius;
	float m_durationFrame;
	int m_power;
	bool m_applied;	
	CCSprite* m_rangeSprite;
};
class StoneAttack : public CCNode
{
public:
	bool init()
	{
		if(m_option & AttackOption::kJackSpeedUp)
		{
			// 영역 생성하여 그 안에서는 잭의 속도가 빠름.
			getParent()->addChild(JackSpeedUpZone::create(ip2ccp(myGD->getJackPoint()), 100, 500, 5));
		}
		if(m_option & AttackOption::kUnbeatable)
		{
			// 영역 생성하여 그 안에서는 무적... 
			getParent()->addChild(JackUnbeatableZone::create(ip2ccp(myGD->getJackPoint()), 100, 500));
		}
		return true;
	}

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
			getParent()->addChild(MonsterSpeedDownZone::create(cumber->getPosition(), 100, 500, 5));
		}
		if(m_option & AttackOption::kPoisonedNiddle)
		{
			getParent()->addChild(PoisonedNiddle::create(cumber, 500, 20));
			// 특정 간격으로 데미지를 깎는다. 부가 기능은  ㄴ ㄴ해.
		}
	}
protected:
	AttackOption m_option;
};


class GuidedMissile : public StoneAttack
{
public:
	static GuidedMissile* create(CCNode* targetNode, CCPoint initPosition, const string& fileName, 
															 float initSpeed, int power, int range, AttackOption ao)
	{
		GuidedMissile* object = new GuidedMissile();
		object->init(targetNode, initPosition, fileName, initSpeed, power, range, ao);

		object->autorelease();
		

		return object;
	}
	bool init(CCNode* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, int power, int range, AttackOption ao)
	{
		StoneAttack::init();
		m_initSpeed = initSpeed;
		m_option = ao;
		m_power = power;
		m_targetNode = targetNode;	
		m_guided = false;
		m_range = range;
		m_missileSprite = KS::loadCCBI<CCSprite*>(this, fileName).first;
		//addChild(KSGradualValue<float>::create(0, 360 * 99, 5, [=](float t){
			//m_missileSprite->setRotationY(t);
			//m_missileSprite->setRotationX(t);
		//}));
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

				m_missileSprite->setRotation(-rad2Deg(m_initRad));
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
																																					sin(m_currentRad) * m_initSpeed * 2));
				m_missileSprite->setRotation(-rad2Deg(m_currentRad));
		 	}
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
	static StraightMissile* create(CCPoint initPosition, const string& fileName, float rad, float initSpeed, int power, AttackOption ao)
	{
		StraightMissile* object = new StraightMissile();
		object->init(initPosition, fileName, rad, initSpeed, power, ao);

		object->autorelease();
		

		return object;
	}
	bool init(CCPoint initPosition, const string& fileName, float rad, float initSpeed, int power, AttackOption ao)
	{
		StoneAttack::init();
		m_initSpeed = initSpeed;
		m_power = power;
		m_option = ao;
		m_missileSprite = KS::loadCCBI<CCSprite*>(this, fileName).first;
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
			if(distance < 10)
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
				if(distance < 10)
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
			m_missileSprite->setRotation(-rad2Deg(m_initRad));
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
	static SpreadMissile* create(KSCumberBase* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, int power, int directions, AttackOption ao)
	{
		SpreadMissile* object = new SpreadMissile();
		object->init(targetNode, initPosition, fileName, initSpeed, power, directions, ao);

		object->autorelease();
		

		return object;
	}
	virtual ~SpreadMissile()
	{
		CCLog("Spread ~");
	}
	bool init(KSCumberBase* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, int power, int directions, AttackOption ao)
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
			StraightMissile* sm = StraightMissile::create(initPosition, fileName, rad + i * deg2Rad(360 / directions), 2.f, 33.f, ao);
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
		m_mine = KS::loadCCBI<CCSprite*>(this, "me_timebomb.ccbi").first;
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
	static SpiritAttack* create(CCPoint initPosition, CCPoint goalPosition, const string& fileName, float tickCount, int power, float speed, int coolFrame, AttackOption ao)
	{
		SpiritAttack* ma = new SpiritAttack();
		ma->init(initPosition, goalPosition, fileName, tickCount, power, speed, coolFrame, ao);
		ma->autorelease();
		return ma;
	}	

	bool init(CCPoint initPosition, CCPoint goalPosition, const string& fileName, int tickCount, int power, float speed, int coolFrame, AttackOption ao)
	{
		StoneAttack::init();
		m_fileName = fileName;
		m_initPosition = initPosition;
		m_goalPosition = goalPosition;
		m_tickCount = tickCount;
		m_initTickCount = tickCount;
		m_power = power;
		m_speed = speed;	
		m_initCoolFrame = coolFrame;
		m_coolFrame = 0; 
		m_mine = KS::loadCCBI<CCSprite*>(this, fileName).first;
		m_option = ao;
		addChild(m_mine);
		addChild(KSGradualValue<CCPoint>::create(initPosition, goalPosition, 1.f,
																				 [=](CCPoint t){
																					 m_mine->setPosition(t);
																					 CCPoint diff = goalPosition - initPosition;
																					 float rad = atan2f(diff.y, diff.x);
																					 m_mine->setRotation(-rad2Deg(rad));
																						
																				 },
																		[=](CCPoint t){
																			m_mine->setPosition(t);
																			CCPoint diff = goalPosition - initPosition;
																			float rad = atan2f(diff.y, diff.x);
																			m_mine->setRotation(-rad2Deg(rad));
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
				SpiritAttack* ma = SpiritAttack::create(m_mine->getPosition(), ip2ccp(mapPoint), m_fileName,
																								m_initTickCount, m_power, m_speed, m_coolFrame, m_option);
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
			{
				m_mine->setPosition(afterPosition);
				m_mine->setRotation(-rad2Deg(m_directionRad));
			}
	
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
	int m_level;
	std::string m_fileName;
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
		CCSprite* spr = KS::loadCCBI<CCSprite*>(this, "me_scope.ccbi").first;
		spr->setPosition(initPosition);
		spr->setScale(radius / 100.f);
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

		CCPoint initPosition = randomPosition + ccp(0, 300);
		CCSprite* arrow = KS::loadCCBI<CCSprite*>(this, "me_allattack_arrow.ccbi").first;
		addChild(arrow);
		addChild(KSGradualValue<CCPoint>::create(initPosition, randomPosition, 1.f,
																						 [=](CCPoint t){
																							 arrow->setPosition(t);
																						 },
																						 [=](CCPoint t){
																							 arrow->removeFromParent();
																							 CCSprite* spr = KS::loadCCBI<CCSprite*>(this, "me_allattack_bomb.ccbi").first;

																							 spr->setPosition(t);
																							 spr->setScale(1.f);
																							 spr->setOpacity(100);
																							 addChild(spr);
																							 m_rangeSprite = spr;
																							 std::vector<KSCumberBase*> nearMonsters;
																							 for(auto iter : myGD->getMainCumberVector()) {
																								 CCPoint targetPosition = iter->getPosition();
																								 float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
																								 if(distance < m_radius) {
																									 nearMonsters.push_back(iter);
																								 }
																							 }	
																							 for(auto iter : myGD->getSubCumberVector()) {
																								 CCPoint targetPosition = iter->getPosition();
																								 float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
																								 if(distance < m_radius) {
																									 nearMonsters.push_back(iter);
																								 }
																							 }	

																							 for(auto iter : nearMonsters) {
																								 CCPoint effectPosition = iter->getPosition();
																								 effectPosition.x += rand()%21 - 10;
																								 effectPosition.y += rand()%21 - 10;

																								 float damage = m_power;
																								 executeOption(dynamic_cast<KSCumberBase*>(iter), damage, 0.f, effectPosition);
																								 //removeFromParentAndCleanup(true);
																							 }
																							 addChild(KSTimer::create(1.f, [=]() {
																								 removeFromParent();
																							 }));
																						 }
		));

		
		//scheduleUpdate();
		return true;	
	}
protected:
	float m_radius;
	int m_power;
	CCSprite* m_rangeSprite;
};


class LaserAttack : public StoneAttack
{
public:
	static LaserAttack* create(float rad, int durationFrame, int power, AttackOption ao)
	{
		LaserAttack* obj = new LaserAttack();
		obj->init(rad, durationFrame, power, ao);
		obj->autorelease();
		return obj;
	}
	bool init(float rad, int durationFrame, int power, AttackOption ao)
	{
		StoneAttack::init();

		CCPoint randomPosition = ip2ccp(IntPoint(ks19937::getIntValue(mapLoopRange::mapWidthInnerBegin, mapLoopRange::mapWidthInnerEnd - 1),
																						 ks19937::getIntValue(mapLoopRange::mapHeightInnerBegin, mapLoopRange::mapHeightInnerEnd - 1)));
		m_rad = rad;
		m_power = power;
		m_option = ao;
		m_durationFrame = durationFrame;	
		
		m_initJiggleInterval = 30;
		m_jiggleInterval = 0;
		m_radius = 30;
		CCNode* laserContainer = CCNode::create();
		addChild(laserContainer);

		// 레이저 구현중.
		CCSprite* laserHead = KS::loadCCBI<CCSprite*>(this, "me_laser_head.ccbi").first;
		laserContainer->addChild(laserHead);
		laserContainer->setPosition(ip2ccp(myGD->getJackPoint()));
		m_startPosition = laserContainer->getPosition();
		//KS::setBlendFunc(laserHead, {GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
		for(int i=0; ; i++)
		{
			CCSprite* testBody = KS::loadCCBI<CCSprite*>(this, "me_laser_body.ccbi").first;
			testBody->setPosition(laserHead->getPosition() + ccp(30 + i * 10, 0));
			laserContainer->addChild(testBody);

			if(ccp2ip(testBody->getPosition()).isInnerMap() == false)
				break;
		}

		laserContainer->setRotation(-rad2Deg(m_rad));
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

		std::set<KSCumberBase*> nearMonsters;
		// startPosition 으로부터 rad 각도로 거리가 r 인 곳을 구하자.
		// startPosition + (cos(rad), sin(rad)) * r	
		if(m_jiggleInterval == 0)
		{
			m_jiggleInterval = m_initJiggleInterval;
			for(int r=m_radius; ;r += 4)
			{
				CCPoint nextPosition = m_startPosition + ccp(cos(m_rad), sin(m_rad)) * r;
				if(ccp2ip(nextPosition).isInnerMap() == false)
				{
					break;
				}

				for(auto iter : myGD->getMainCumberVector())
				{
					CCPoint targetPosition = iter->getPosition();
					float distance = ccpLength(targetPosition - nextPosition);
					if(distance < m_radius)
					{
						nearMonsters.insert(iter);
					}
				}	
				for(auto iter : myGD->getSubCumberVector())
				{
					CCPoint targetPosition = iter->getPosition();
					float distance = ccpLength(targetPosition - nextPosition);
					if(distance < m_radius)
					{
						nearMonsters.insert(iter);
					}
				}	
			}

			for(auto monsters : nearMonsters)
			{

				CCPoint effectPosition = monsters->getPosition();
				effectPosition.x += rand()%21 - 10;
				effectPosition.y += rand()%21 - 10;

				float damage = m_power;
				executeOption(dynamic_cast<KSCumberBase*>(monsters), damage, 0.f, effectPosition);
			}
		}
	}
protected:
	float m_rad;
	int m_power;
	int m_durationFrame;
	int m_radius;
	CCPoint m_startPosition;
	int m_jiggleInterval;
	int m_initJiggleInterval;		
};



class LaserWrapper : public StoneAttack
{
public:
	static LaserWrapper* create(int numbers, int durationFrame, int power, AttackOption ao)
	{
		LaserWrapper* obj = new LaserWrapper();
		obj->init(numbers, durationFrame, power, ao);
		obj->autorelease();
		return obj;
	}
	bool init(int numbers, int durationFrame, int power, AttackOption ao)
	{
		StoneAttack::init();
		float addRad = ks19937::getDoubleValue(0, M_PI * 2.f);
		for(int r=0; r<numbers; r++)
		{
			float rad = deg2Rad(360.f / numbers * r);
			rad += addRad;
			addChild(LaserAttack::create(rad, durationFrame, power, ao));
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
	int m_numbers;
	int m_power;
	int m_durationFrame;
};









