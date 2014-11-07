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
#include "ASMotionStreak.h"

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
{
	return static_cast<AttackOption>(static_cast<int>(a) | static_cast<int>(b));
}

//inline AttackOption operator&(AttackOption a, AttackOption b)
//{return static_cast<AttackOption>(static_cast<int>(a) & static_cast<int>(b));}

// startReaction 쪽에서 경직이 될지 안될지 결정하는 인자를 받아야 할 듯 하다.

class PoisonedNiddle : public CCNode
{
public:
	static PoisonedNiddle* create(KSCumberBase* target, int durationFrame, int power, int subPower)
	{
		PoisonedNiddle* obj = new PoisonedNiddle();
		obj->init(target, durationFrame, power, subPower);
		obj->autorelease();
		return obj;
	}
	bool init(KSCumberBase* target, int durationFrame, int power, int subPower)
	{
		m_durationFrame = durationFrame;
		m_power = power;
		m_subPower = subPower;
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

			myGD->communication("Main_startShake", 0.f);
		}	
	}
protected:
	KSCumberBase* m_target;
	float m_durationFrame;
	int m_power;
	int m_subPower;
	int m_jiggleInterval;
	int m_initJiggleInterval;		
};

class MonsterSpeedDownZone : public CCNode
{
public:
	static MonsterSpeedDownZone* create(CCPoint initPosition, float radius, int durationFrame, int power, int subPower)
	{
		MonsterSpeedDownZone* obj = new MonsterSpeedDownZone();
		obj->init(initPosition, radius, durationFrame, power, subPower);
		obj->autorelease();
		return obj;
	}
	bool init(CCPoint initPosition, float radius, int durationFrame, int power, int subPower)
	{

		m_radius = radius;
		m_durationFrame = durationFrame;
		m_power = power;
		m_subPower = subPower;
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
	int m_subPower;
	map<CCNode*, bool> m_applied; // 몬스터가 적용이 됐는지.
	CCSprite* m_rangeSprite;
};


class JackSpeedUpZone : public CCNode
{
public:
	static JackSpeedUpZone* create(CCPoint initPosition, float radius, int durationFrame, int power, int subPower)
	{
		JackSpeedUpZone* obj = new JackSpeedUpZone();
		obj->init(initPosition, radius, durationFrame, power, subPower);
		obj->autorelease();
		return obj;
	}
	bool init(CCPoint initPosition, float radius, int durationFrame, int power, int subPower)
	{

		m_radius = radius;
		m_durationFrame = durationFrame;
		m_power = power;
		m_subPower = subPower;
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
	int m_subPower;
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
	int m_subPower;
	bool m_applied;	
	CCSprite* m_rangeSprite;
};


static std::map<string,CCDictionary*> jm_particleList;
class StoneAttack : public CCNode
{
public:
	static int getPower(int grade, int level)
	{
		return grade * 20 + level * 10;
	}
	bool init()
	{
		if(m_option & AttackOption::kJackSpeedUp)
		{
			// 영역 생성하여 그 안에서는 잭의 속도가 빠름.
			getParent()->addChild(JackSpeedUpZone::create(ip2ccp(myGD->getJackPoint()), 100, 500, 5, 0));
		}
		if(m_option & AttackOption::kUnbeatable)
		{
			// 영역 생성하여 그 안에서는 무적... 
			getParent()->addChild(JackUnbeatableZone::create(ip2ccp(myGD->getJackPoint()), 100, 500));
		}
		return true;
	}

	void executeOption(KSCumberBase* cumber, float damage, float subdamage, float direction, CCPoint damagePosition)
	{
		// 옵션에 대해서 수행함.
		
		TRACE();
		CCPoint cumberPosition = cumber->getPosition();
		if(m_option & AttackOption::kPoisonedNiddle)
		{
			getParent()->addChild(PoisonedNiddle::create(cumber, 500, 20, subdamage));
			// 특정 간격으로 데미지를 깎는다. 부가 기능은  ㄴ ㄴ해.
		}
		if((m_option & AttackOption::kGold))
		{
			FeverCoin* t_fc = FeverCoin::create(ccp2ip(cumberPosition), nullptr, nullptr);
			CCLOG("%x getParent", this);
			getParent()->addChild(t_fc, 5);
			t_fc->startRemove();
			mySGD->addChangeGoodsIngameGold(1, 0);
		}
		if(m_option & AttackOption::kMonsterSpeedDown)
		{
			// 몬스터 속도 하락시킴. n 초간 p% 감소하는 형태.
			getParent()->addChild(MonsterSpeedDownZone::create(cumberPosition, 100, 500, 5, subdamage));
		}
		// directionAngle : Degree 단위.
		// 피격에니메이션.
		myGD->communication("MP_explosion", damagePosition, ccc4f(0, 0, 0, 0), 0.f);
		// 화면 번쩍 번쩍
		myGD->communication("VS_setLight");

		
		
		for(auto iter = cumber->getCharges().begin(); iter != cumber->getCharges().end(); ++iter)
		{
			if((*iter)->getDelayCount() < 10)
			{
				(*iter)->setChargeFrame((*iter)->getChargeFrame() + 6);
				(*iter)->setDelayCount((*iter)->getDelayCount() + 1);
			}
		}
		
		// 캐스팅 캔슬.
		
		cumber->setDamageMeasure(cumber->getDamageMeasure() + damage + subdamage);
		if(cumber->getDamageMeasure() > cumber->getTotalHp() * 0.1f) // 전체 피통의 10% 가 깎이면 캐스팅 취소함.
		{
			m_option = m_option | AttackOption::kCancelCasting; // 캐스팅 속성 추가.
		}

		if(m_option & AttackOption::kCancelCasting)
		{
			myGD->communication("MP_bombCumber", (CCObject*)cumber); // with startMoving
			
			cumber->setDamageMeasure(0.f);
		}

		// 몬스터 리액션하라고.
		myGD->communication("CP_startDamageReaction", cumber, damage + subdamage, direction, m_option & AttackOption::kCancelCasting,
												m_option & AttackOption::kStiffen); // damage : 555
		// 데미지 표시해주는 것. 데미지 숫자 뜸.
		myGD->communication("Main_showDamageMissile", damagePosition, (int)damage, (int)subdamage);

		int combo_cnt = myGD->getCommunication("UI_getComboCnt");
		combo_cnt++;

		int damage_score = ((damage + subdamage)/10*5+100)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d);//(100.f+damage)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d);
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
		if(m_option & AttackOption::kPlusScore)
		{
			addScore *= 1.1f;
		}
		myGD->communication("UI_addScore", addScore, sub_damage_score + sub_combo_score);
		myGD->communication("UI_setComboCnt", combo_cnt);
		myGD->communication("Main_showComboImage", damagePosition, combo_cnt);
		myGD->communication("Main_startShake", ks19937::getFloatValue(0, 360)); // 일단은 완전 랜덤으로.
	}
	
	
	
//	virtual CCDictionary* getDictForParticle(string plistFile);
	
	virtual CCParticleSystemQuad* addParticle(int particleNo)
	{
		
		string plistFile = ccsf("jm_particle_%02d.plist", particleNo);
		
		auto quad = CCParticleSystemQuad::create(plistFile.c_str());
		
		quad->setPositionType(kCCPositionTypeRelative);
		
		//		addChild(quad);
		
		return quad;
	}
	virtual ASMotionStreak* addStreak(int lev)
	{
		float len = 0.03*lev;
		if(len>1)len=1;
		auto streak = ASMotionStreak::create(len, 2, 6, ccWHITE, "streak_temp.png");
		streak->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		return streak;
	}
	void makeBeautifier(int level, ASMotionStreak*& motionStreak, CCParticleSystemQuad*& particleQuad)
	{
		
		
			motionStreak = addStreak(level);
		
			if(level <= 5)
			{
				
			}
			else if(level <= 10)
			{
				particleQuad = addParticle(1);
			}
			else if(level <= 15)
			{
				particleQuad = addParticle(2);
			}
			else if(level <= 20)
			{
				particleQuad = addParticle(3);
				
			}
			else if(level <= 25)
			{
				particleQuad = addParticle(4);
				
			}
			else if(level <= 30)
			{
				particleQuad = addParticle(5);
			}
			else
			{
				
			}
		
	}
protected:
	AttackOption m_option;
};

//

//
//CCDictionary* StoneAttack::getDictForParticle(string plistFile){
//	map<string,CCDictionary*>::iterator it;
//	it=jm_particleList.find(plistFile);
//	CCDictionary *dict;
//	if (it == jm_particleList.end()) {
//		string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(plistFile.c_str());
//		dict = CCDictionary::createWithContentsOfFileThreadSafe(m_sPlistFile.c_str());
//		dict->retain();
//		jm_particleList[plistFile]=dict;
//	}else{
//		dict=jm_particleList[plistFile];
//		dict->retain();
//	}
//	
//	return dict;
//}


// 옛날 GuidedMissile


/*
 #if 0
class GuidedMissile : public StoneAttack
{
public:
	static GuidedMissile* create(CCNode* targetNode, CCPoint initPosition, const string& fileName, 
															 float initSpeed, int power, int range, AttackOption ao, bool selfRotation)
	{
		GuidedMissile* object = new GuidedMissile();
		object->init(targetNode, initPosition, fileName, initSpeed, power, range, ao, selfRotation);

		object->autorelease();
		

		return object;
	}
	static GuidedMissile* createForShowWindow(const string& fileName, bool selfRotation)
	{
		GuidedMissile* object = new GuidedMissile();
		object->initForShowWindow(fileName, selfRotation);
		object->autorelease();
		return object;
	}
	
	bool init(CCNode* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, int power, int range, AttackOption ao, bool selfRotation)
	{
		StoneAttack::init();
		
		m_particle = NULL;
		m_streak = NULL;
		
		m_initSpeed = initSpeed;
		m_option = ao;
		m_power = power;
		m_targetNode = targetNode;	
		m_guided = false;
		m_range = range;
		m_selfRotation = selfRotation;
		m_missileSprite = CCSprite::create(fileName.c_str()); // KS::loadCCBI<CCSprite*>(this, fileName).first;
		//addChild(KSGradualValue<float>::create(0, 360 * 99, 5, [=](float t){
			//m_missileSprite->setRotationY(t);
			//m_missileSprite->setRotationX(t);
		//}));
		addChild(m_missileSprite);
		m_missileSprite->setScale(1.f/myGD->game_scale);
		m_missileSprite->setPosition(initPosition);
		m_showWindow.initPosition = initPosition;
		if(targetNode)
		{
			CCPoint diff = m_targetNode->getPosition() - initPosition;
			
			int random_value = rand()%21 - 10; // -10~10
			float random_float = 1.f + random_value/100.f;
			
			m_initRad = atan2f(diff.y, diff.x) * random_float;
			scheduleUpdate();
		}
		return true;
	}
	
	bool initForShowWindow(const string& fileName, bool selfRotation)
	{
		StoneAttack::init();
		
		m_particle = NULL;
		m_streak = NULL;
		m_selfRotation = selfRotation;
		m_missileSprite = CCSprite::create(fileName.c_str()); // KS::loadCCBI<CCSprite*>(this, fileName).first;
		//addChild(KSGradualValue<float>::create(0, 360 * 99, 5, [=](float t){
		//m_missileSprite->setRotationY(t);
		//m_missileSprite->setRotationX(t);
		//}));
		addChild(m_missileSprite);
		//		m_missileSprite->setScale(1.f/myGD->game_scale);
		m_missileSprite->setPosition(ccp(0, 0));
		schedule(schedule_selector(GuidedMissile::showWindow));
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
				
				if(m_selfRotation)
				{
					m_missileSprite->setRotation(m_missileSprite->getRotation() + 5);
				}
				else
				{
					m_missileSprite->setRotation(-rad2Deg(m_initRad) - 90);
				}
				
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
			else // 그냥 진행중.
			{
				float tt = atan2f(diffPosition.y, diffPosition.x); // 미사일에서 몬스터까지의 각도 
				//m_currentRad += clampf(tt - m_currentRad, deg2Rad(-15), deg2Rad(15));
				float tempTt = tt - m_currentRad;
				bool sign = tt - m_currentRad > 0  ? 1 : -1;
				m_currentRad += (tt - m_currentRad); // , deg2Rad(-15), deg2Rad(15));
				m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cos(m_currentRad) * m_initSpeed * 2,
																																					sin(m_currentRad) * m_initSpeed * 2));
				
				if(m_selfRotation)
				{
					m_missileSprite->setRotation(m_missileSprite->getRotation() + 5);
				}
				else
				{
					m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
				}
//				m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
		 	}
			
			if(m_particle)
				m_particle->setPosition(m_missileSprite->getPosition());
			
			if(m_streak)
				m_streak->setPosition(m_missileSprite->getPosition());
		}
	}
	void beautifier(int grade, int level, ASMotionStreak*& motionStreak = m_streak, CCParticleSystemQuad*& particleQuad = m_particle)
	{
		if(grade >= 3)
			motionStreak = addStreak();

		if(grade == 2)
		{
			if(level <= 2)
				particleQuad = addParticle(0);
			else if(level <= 4)
				particleQuad = addParticle(2);
			else
				particleQuad = addParticle(3);
		}
		else if(grade == 3)
		{
			if(level <= 2)
				particleQuad = addParticle(8);
			else if(level <= 4)
				particleQuad = addParticle(10);
			else
				particleQuad = addParticle(11);
		}
		else if(grade == 4)
		{
			if(level <= 2)
				particleQuad = addParticle(12);
			else if(level <= 4)
				particleQuad = addParticle(14);
			else
				particleQuad = addParticle(15);
		}
		else if(grade == 5)
		{
			if(level <= 2)
				particleQuad = addParticle(16);
			else if(level <= 4)
				particleQuad = addParticle(18);
			else
				particleQuad = addParticle(19);
		}
	}	
	ASMotionStreak* addStreak()
	{
		auto streak = ASMotionStreak::create(0.4f, 2, 12, ccWHITE, "streak_temp.png");
		streak->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		addChild(streak, -2);
		return streak;
	}
	
	void addParticle(int particle_type)
	{
		string plist_name = "jm_particle1_empty.plist";
		if(particle_type == 0)
			plist_name = "jm_particle1_empty.plist"; // 노랑 불
		else if(particle_type == 1)
			plist_name = "jm_particle1_fire.plist"; // 붉은 불(주황)
		else if(particle_type == 2)
			plist_name = "jm_particle1_life.plist"; // 초록 불(노랑)
		else if(particle_type == 3)
			plist_name = "jm_particle1_water.plist"; // 파란 불(하양)
		else if(particle_type == 4)
			plist_name = "jm_particle2_empty.plist"; // 가시
		else if(particle_type == 5)
			plist_name = "jm_particle2_fire.plist";
		else if(particle_type == 6)
			plist_name = "jm_particle2_life.plist";
		else if(particle_type == 7)
			plist_name = "jm_particle2_water.plist";
		else if(particle_type == 8)
			plist_name = "jm_particle3_empty.plist"; // 반사빛/뽀샤시빛
		else if(particle_type == 9)
			plist_name = "jm_particle3_fire.plist";
		else if(particle_type == 10)
			plist_name = "jm_particle3_life.plist";
		else if(particle_type == 11)
			plist_name = "jm_particle3_water.plist";
		else if(particle_type == 12)
			plist_name = "jm_particle4_empty.plist"; // 심한 반짝임
		else if(particle_type == 13)
			plist_name = "jm_particle4_fire.plist";
		else if(particle_type == 14)
			plist_name = "jm_particle4_life.plist";
		else if(particle_type == 15)
			plist_name = "jm_particle4_water.plist";
		else if(particle_type == 16)
			plist_name = "jm_particle5_empty.plist"; // 둥근 파장까지
		else if(particle_type == 17)
			plist_name = "jm_particle5_fire.plist";
		else if(particle_type == 18)
			plist_name = "jm_particle5_life.plist";
		else if(particle_type == 19)
			plist_name = "jm_particle5_water.plist";
		
		auto quad = CCParticleSystemQuad::create(plist_name.c_str());
		quad->setPositionType(kCCPositionTypeRelative);
		addChild(m_particle, -1);
		return quad;
	}
	void showWindow(float dt)
	{
		float r = m_showWindow.rotationRadius;
		m_missileSprite->setPosition(m_showWindow.initPosition + ccp(cos(m_showWindow.rotationRad) * r, sin(m_showWindow.rotationRad) * r));

		m_missileSprite->setRotation(-rad2Deg(m_showWindow.rotationRad) - 90);

		if(m_particle)
			m_particle->setPosition(m_missileSprite->getPosition());

		if(m_streak)
			m_streak->setPosition(m_missileSprite->getPosition());

		
		m_showWindow.rotationRad += m_showWindow.rotationVelocityRad;
	}	
	// 반지름 설정
	void setShowWindowRotationRadius(float r)
	{
		m_showWindow.rotationRadius = r;
	}
	// 각속도 설정
	void setShowWindowVelocityRad(float r)
	{
		m_showWindow.rotationVelocityRad = r;
	}
protected:
	float m_initSpeed; // 초기 속도.
	float m_initRad; // 처음에 날아가는 각도.
	float m_currentRad; // 범위내 들어왔을 때 현재 각도.

	bool m_guided; // 유도 모드인지 여부.
	int m_range; // 유도 범위.
	bool m_selfRotation; // 스스로 도는지 여부.
	CCNode* m_targetNode;
	CCSprite* m_missileSprite; // 미사일 객체.
	CCParticleSystemQuad* m_particle;
	CCMotionStreak* m_streak;
	struct ShowWindow
	{
		ShowWindow()
		{
			rotationRad = rand()%314 / 100.f;
			rotationRadius = 30.f;
			rotationVelocityRad = M_PI / 60.f;
		}
		float rotationRad;
		float rotationRadius;
		float rotationVelocityRad;
		CCPoint initPosition;
	}m_showWindow;
	
	CC_SYNTHESIZE(int, m_power, Power); // 파워.
};

#endif
 
 */



class GuidedMissileForUpgradeWindow : public StoneAttack
{
public:
	static GuidedMissileForUpgradeWindow* create(CCNode* targetNode, CCPoint initPosition, const string& fileName, 
															 float initSpeed, int power, int subPower, int range, AttackOption ao, bool selfRotation)
	{
		GuidedMissileForUpgradeWindow* object = new GuidedMissileForUpgradeWindow();
		object->init(targetNode, initPosition, fileName, initSpeed, power, subPower, range, ao, selfRotation);

		object->autorelease();
		

		return object;
	}
	static GuidedMissileForUpgradeWindow* createForShowWindow(const string& fileName, bool selfRotation)
	{
		GuidedMissileForUpgradeWindow* object = new GuidedMissileForUpgradeWindow();
		object->initForShowWindow(fileName, selfRotation);
		object->autorelease();
		return object;
	}
	
	bool init(CCNode* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, int power, int subPower, int range, AttackOption ao, bool selfRotation)
	{
		StoneAttack::init();
		
		m_particle = NULL;
		m_streak = NULL;
		
		m_initSpeed = initSpeed;
		m_option = ao;
		m_power = power;
		m_subPower = subPower;
		m_targetNode = targetNode;	
		m_guided = false;
		m_range = range;
		m_selfRotation = selfRotation;
		m_missileSprite = CCSprite::create(fileName.c_str()); // KS::loadCCBI<CCSprite*>(this, fileName).first;
		//addChild(KSGradualValue<float>::create(0, 360 * 99, 5, [=](float t){
			//m_missileSprite->setRotationY(t);
			//m_missileSprite->setRotationX(t);
		//}));
		addChild(m_missileSprite);
		m_missileSprite->setScale(1.f/myGD->game_scale);
		m_missileSprite->setPosition(initPosition);
		m_showWindow.initPosition = initPosition;
		if(targetNode)
		{
			CCPoint diff = m_targetNode->getPosition() - initPosition;
			
			int random_value = rand()%21 - 10; // -10~10
			float random_float = 1.f + random_value/100.f;
			
			m_initRad = atan2f(diff.y, diff.x) * random_float;
			m_currentRad = m_initRad; // + ks19937::getFloatValue(deg2Rad(-45), deg2Rad(45));
			scheduleUpdate();
		}
		return true;
	}
	
	bool initForShowWindow(const string& fileName, bool selfRotation)
	{
		StoneAttack::init();
		
		m_particle = NULL;
		m_streak = NULL;
		m_selfRotation = selfRotation;
		m_missileSprite = CCSprite::create(fileName.c_str()); // KS::loadCCBI<CCSprite*>(this, fileName).first;
		//addChild(KSGradualValue<float>::create(0, 360 * 99, 5, [=](float t){
		//m_missileSprite->setRotationY(t);
		//m_missileSprite->setRotationX(t);
		//}));
		addChild(m_missileSprite);
		//		m_missileSprite->setScale(1.f/myGD->game_scale);
		m_missileSprite->setPosition(ccp(0, 0));
		schedule(schedule_selector(ThisClassType::showWindow));
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
		if(distance <= 6)
		{
			AudioEngine::sharedInstance()->playEffect("se_monattacked.mp3", false);
			
			CCPoint effectPosition = m_missileSprite->getPosition();
			effectPosition.x += rand()%21 - 10;
			effectPosition.y += rand()%21 - 10;
			
			float damage = m_power;
			executeOption(dynamic_cast<KSCumberBase*>(m_targetNode), damage, m_subPower, 0.f, effectPosition);
			
			removeFromParentAndCleanup(true);
		}
		else  // 거리가 멀면 몬스터 쪽으로 유도함.
		{
			CCPoint missilePosition = m_missileSprite->getPosition();
			CCPoint cumberPosition;
			cumberPosition = m_targetNode->getPosition();		
			CCPoint diffPosition = cumberPosition - missilePosition;

			bool isNearMonster = false;
			for(auto bosses : myGD->getMainCumberVector())
			{
				if(ccpLength(bosses->getPosition() - m_missileSprite->getPosition()) <= m_range)
				{
					isNearMonster = true;
					break;
				}
			}
			for(auto mob : myGD->getSubCumberVector())
			{
				if(ccpLength(mob->getPosition() - m_missileSprite->getPosition()) <= m_range)
				{
					isNearMonster = true;
					break;
				}
			}
			{
				float tt = atan2f(diffPosition.y, diffPosition.x); // 미사일에서 몬스터까지의 각도
				//KS::KSLog("% ~ % : %", deg2Rad(-90), deg2Rad(90), tt);
//				tt = clampf(tt, deg2Rad(-90), deg2Rad(90));
				
				//m_currentRad += clampf(tt - m_currentRad, deg2Rad(-15), deg2Rad(15));
				float tempTt = tt - m_currentRad;
				bool sign = tt - m_currentRad > 0  ? 1 : -1;
				float missileSpeed = m_initSpeed * 1.3f;
				if(isNearMonster)
				{
					m_currentRad += clampf((tt - m_currentRad), deg2Rad(-2.f), deg2Rad(2.f));
				}
				else 
				{
					//m_currentRad += clampf((tt - m_currentRad), deg2Rad(-0.8f), deg2Rad(0.8f)); // , deg2Rad(-15), deg2Rad(15));
				}
//				m_currentRad = m_currentRad + tt - m_currentRad;
				m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cos(m_currentRad) * missileSpeed,
																																					sin(m_currentRad) * missileSpeed));
				
				if(m_selfRotation)
				{
					m_missileSprite->setRotation(m_missileSprite->getRotation() + 20);
				}
				else
				{
					m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
				}
//				m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
		 	}
			
			if(m_particle)
				m_particle->setPosition(m_missileSprite->getPosition());
			
			if(m_streak)
				m_streak->setPosition(m_missileSprite->getPosition());
		}
	}
	void beautifier(int level)
	{
		makeBeautifier(level, m_streak, m_particle);
		if(m_streak)addChild(m_streak, -1);
		if(m_particle)addChild(m_particle, -2);
	}
	

	
	void showWindow(float dt)
	{
		float r = m_showWindow.rotationRadius;
		m_missileSprite->setPosition(m_showWindow.initPosition + ccp(cos(m_showWindow.rotationRad) * r, sin(m_showWindow.rotationRad) * r));
		if(m_selfRotation)
		{
			m_missileSprite->setRotation(m_missileSprite->getRotation() + 15);
		}
		else
		{
			m_missileSprite->setRotation(-rad2Deg(m_showWindow.rotationRad) + 180);
		}

		if(m_particle)
			m_particle->setPosition(m_missileSprite->getPosition());

		if(m_streak)
			m_streak->setPosition(m_missileSprite->getPosition());

		
		m_showWindow.rotationRad += m_showWindow.rotationVelocityRad;
	}	
	// 반지름 설정
	void setShowWindowRotationRadius(float r)
	{
		m_showWindow.rotationRadius = r;
	}
	// 각속도 설정
	void setShowWindowVelocityRad(float r)
	{
		m_showWindow.rotationVelocityRad = r;
	}
protected:
	float m_initSpeed; // 초기 속도.
	float m_initRad; // 처음에 날아가는 각도.
	float m_currentRad; // 범위내 들어왔을 때 현재 각도.

	bool m_guided; // 유도 모드인지 여부.
	int m_range; // 유도 범위.
	bool m_selfRotation; // 스스로 도는지 여부.
	CCNode* m_targetNode;
	CCSprite* m_missileSprite; // 미사일 객체.
	CCParticleSystemQuad* m_particle;
	ASMotionStreak* m_streak;
	struct ShowWindow
	{
		ShowWindow()
		{
			rotationRad = rand()%314 / 100.f;
			rotationRadius = 30.f;
			rotationVelocityRad = M_PI / 60.f;
		}
		float rotationRad;
		float rotationRadius;
		float rotationVelocityRad;
		CCPoint initPosition;
	}m_showWindow;
	
	CC_SYNTHESIZE(int, m_power, Power); // 파워.
	int m_subPower;
};
class GuidedMissile : public StoneAttack
{
public:
	static GuidedMissile* create(CCNode* targetNode, CCPoint initPosition, const string& fileName, 
															 float initSpeed, int power, int subpower, int range, AttackOption ao, bool selfRotation)
	{
		GuidedMissile* object = new GuidedMissile();
		object->init(targetNode, initPosition, fileName, initSpeed, power, subpower, range, ao, selfRotation);

		object->autorelease();
		

		return object;
	}
	static GuidedMissile* createForShowWindow(const string& fileName, bool selfRotation, int grade, int level)
	{
		GuidedMissile* object = new GuidedMissile();
		object->initForShowWindow(fileName, selfRotation, grade, level);
		object->autorelease();
		return object;
	}
	static GuidedMissile* createForShowStartSettingPopup(CCNode* start_node, const string& filename, bool selfRotation, int grade, int level)
	{
		GuidedMissile* object = new GuidedMissile();
		object->initForShowStartSettingPopup(start_node, filename, selfRotation, grade, level);
		object->autorelease();
		return object;
	}
	
	bool init(CCNode* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, int power, int subPower,
						int range, AttackOption ao, bool selfRotation)
	{
		StoneAttack::init();
		
		m_particle = NULL;
		m_streak = NULL;
		m_start_node = NULL;
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		m_initSpeed = initSpeed * mInfo.get("speedbonus", 1.f).asFloat();
		m_option = ao;
		m_power = power;
		m_subPower = subPower;
		m_targetNode = targetNode;	
		m_guided = false;
		m_range = range;
		m_selfRotation = selfRotation;
		m_missileSprite = CCSprite::create(fileName.c_str()); // KS::loadCCBI<CCSprite*>(this, fileName).first;
		//addChild(KSGradualValue<float>::create(0, 360 * 99, 5, [=](float t){
			//m_missileSprite->setRotationY(t);
			//m_missileSprite->setRotationX(t);
		//}));
		addChild(m_missileSprite);
		m_missileSprite->setScale(1.f/myGD->game_scale);
		m_missileSprite->setPosition(initPosition);

		if(targetNode)
		{
			CCPoint diff = m_targetNode->getPosition() - initPosition;
			
			int random_value = rand()%21 - 10; // -10~10
			float random_float = 1.f + random_value/100.f;
			random_float = 1.f;
			m_initRad = atan2f(diff.y, diff.x) * random_float;
			m_currentRad = m_initRad; // + ks19937::getFloatValue(deg2Rad(-45), deg2Rad(45));
			scheduleUpdate();
		}
		return true;
	}
	
	bool initForShowStartSettingPopup(CCNode* start_node, const string& fileName, bool selfRotation, int grade, int level)
	{
		StoneAttack::init();
		m_start_node = start_node;
		m_showWindow.fileName = fileName;
		m_selfRotation = selfRotation;
		m_showWindow.grade = grade;
		m_showWindow.level = level;
		schedule(schedule_selector(ThisClassType::showWindow));
		
		m_showWindow.whiteBoard = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 150));
		//m_back->setOpacity(color.a);
		
		//addChild(m_showWindow.whiteBoard);
		m_showWindow.clippingNode = CCClippingNode::create();
		CCClippingNode* cNode = m_showWindow.clippingNode;
		//cNode->setContentSize(CCSizeMake(100, 100));
		//cNode->setAnchorPoint(ccp(0.5,0.f));
		cNode->setPosition(ccp(0,0));
		cNode->setStencil(m_showWindow.whiteBoard);
		cNode->setInverted(false);
		this->addChild(cNode,1);
		m_showWindow.explosionNode = CCSpriteBatchNode::create("fx_monster_hit.png");
		
		cNode->addChild(m_showWindow.explosionNode);
		
		return true;
	}
	
	bool initForShowWindow(const string& fileName, bool selfRotation, int grade, int level )
	{
		StoneAttack::init();
		m_start_node = NULL;
		m_showWindow.fileName = fileName;	
		m_selfRotation = selfRotation;
		m_showWindow.grade = grade;
		m_showWindow.level = level;
		schedule(schedule_selector(ThisClassType::showWindow));

		m_showWindow.whiteBoard = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 90));
		//m_back->setOpacity(color.a);

		//addChild(m_showWindow.whiteBoard);
		m_showWindow.clippingNode = CCClippingNode::create();
		CCClippingNode* cNode = m_showWindow.clippingNode;
		//cNode->setContentSize(CCSizeMake(100, 100));
		//cNode->setAnchorPoint(ccp(0.5,0.f));
		cNode->setPosition(ccp(0,0));
		cNode->setStencil(m_showWindow.whiteBoard);
		cNode->setInverted(false);
		this->addChild(cNode,1);
		m_showWindow.explosionNode = CCSpriteBatchNode::create("fx_monster_hit.png");

		cNode->addChild(m_showWindow.explosionNode);

		return true;
	}
	
	function<void(void)> m_func;
	void setFunctionForCrash(function<void(void)> func){
		this->m_func = func;
	}
	void showWindow(float dt)
	{
		//float r = m_showWindow.rotationRadius;
		//m_missileSprite->setPosition(m_showWindow.initPosition + ccp(cos(m_showWindow.rotationRad) * r, sin(m_showWindow.rotationRad) * r));
		//if(m_selfRotation)
		//{
			//m_missileSprite->setRotation(m_missileSprite->getRotation() + 15);
		//}
		//else
		//{
			//m_missileSprite->setRotation(-rad2Deg(m_showWindow.rotationRad) + 180);
		//}

		//if(m_particle)
			//m_particle->setPosition(m_missileSprite->getPosition());

		//if(m_streak)
			//m_streak->setPosition(m_missileSprite->getPosition());
		
		m_showWindow.currentTime += 1 / 60.f;

		if(m_showWindow.currentTime > m_showWindow.lastCreationTime + 1.0f)
		{
			m_showWindow.lastCreationTime = m_showWindow.currentTime;
			float creationRad = ks19937::getFloatValue(0, 2 * M_PI);
			ShowWindow::MissileSprite missile;
			auto stree = m_showWindow.fileName;
			auto ttt = CCSprite::create(stree.c_str());
			missile.missileSprite = ttt;
			makeBeautifier(m_showWindow.level, missile.streak, missile.particleQuad);
			if(missile.streak)addChild(missile.streak, -1);
			if(missile.particleQuad)addChild(missile.particleQuad, -2);
			
			m_showWindow.clippingNode->addChild(missile.missileSprite);
			if(m_start_node)
				missile.missileSprite->setPosition(m_start_node->getPosition() - getPosition());
			else
				missile.missileSprite->setPosition(ccp(100 * cosf(creationRad), 100 * sinf(creationRad)));
			
			if(missile.streak)
				missile.streak->setPosition(missile.missileSprite->getPosition());
			if(missile.particleQuad)
				missile.particleQuad->setPosition(missile.missileSprite->getPosition());
			
			if(m_start_node)
				missile.missileRad = (m_start_node->getPosition() - getPosition()).getAngle();// creationRad;
			else
				missile.missileRad = creationRad;
			
			int random_value = rand()%7 - 3;
			float random_float = random_value/10.f;
			float speed = 1.4f+random_float + m_showWindow.grade / 10.f;
			missile.missileSpeed = speed;
			m_showWindow.missileSprites.push_back(missile);	
		}
		
		for(auto iter = m_showWindow.missileSprites.begin(); iter != m_showWindow.missileSprites.end();)
		{
			ShowWindow::MissileSprite i = *iter;
			i.missileSprite->setPosition(i.missileSprite->getPosition() -
																	 ccp(i.missileSpeed * cosf(i.missileRad), i.missileSpeed * sinf(i.missileRad)));
			if(i.streak)
				i.streak->setPosition(i.missileSprite->getPosition());
			if(i.particleQuad)
				i.particleQuad->setPosition(i.missileSprite->getPosition());
			if(m_selfRotation)
			{
				i.missileSprite->setRotation(i.missileSprite->getRotation() + 15);
			}
			else
			{
				i.missileSprite->setRotation(-rad2Deg(i.missileRad) + 90);
			}

			if(ccpLength(i.missileSprite->getPosition() - CCPointZero) <= 2.f)
			{
				//AudioEngine::sharedInstance()->playEffect("sound_jack_missile_bomb.mp3",false);
				i.missileSprite->removeFromParent();
				if(i.streak)	
					i.streak->removeFromParent();
				if(i.particleQuad)
					i.particleQuad->removeFromParent();
				iter = m_showWindow.missileSprites.erase(iter);
				CCSprite* t_explosion = CCSprite::createWithTexture(m_showWindow.explosionNode->getTexture(), CCRectMake(0, 0, 167, 191));
				t_explosion->setScale(0.65f);
				t_explosion->setRotation(-rad2Deg(i.missileRad)-90 + 180.f);
				m_showWindow.explosionNode->addChild(t_explosion);

				CCAnimation* t_animation = CCAnimation::create();
				t_animation->setDelayPerUnit(0.1f);
				t_animation->addSpriteFrameWithTexture(m_showWindow.explosionNode->getTexture(), CCRectMake(0, 0, 167, 191));
				for(int i=0;i<2;i++)
					for(int j=0;j<3;j++)
						t_animation->addSpriteFrameWithTexture(m_showWindow.explosionNode->getTexture(), CCRectMake(j*167, i*191, 167, 191));
				
				this->m_func();
				CCAnimate* t_animate = CCAnimate::create(t_animation);
				CCFadeOut* t_fade = CCFadeOut::create(0.2f);
				CCRemoveSelf* t_remove = CCRemoveSelf::create();
				CCSequence* t_seq = CCSequence::create(t_animate, t_fade, t_remove, NULL);
				t_explosion->runAction(t_seq);
			}
			else 
			{
				++iter;
			}
		}
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
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		if(distance <= 12 * mInfo.get("guiderangebonus", 1.f).asFloat()) // 원래 4
		{
			AudioEngine::sharedInstance()->playEffect("se_monattacked.mp3", false);
			
			CCPoint effectPosition = m_missileSprite->getPosition();
			effectPosition.x += rand()%21 - 10;
			effectPosition.y += rand()%21 - 10;
			
			float damage = m_power;
			TRACE();
			executeOption(dynamic_cast<KSCumberBase*>(m_targetNode), damage, m_subPower, 0.f, effectPosition);
			
			removeFromParentAndCleanup(true);
		}
		else  // 거리가 멀면 몬스터 쪽으로 유도함.
		{
			CCPoint missilePosition = m_missileSprite->getPosition();
			CCPoint cumberPosition;
			cumberPosition = m_targetNode->getPosition();		
			CCPoint diffPosition = cumberPosition - missilePosition;

			bool isNearMonster = false;
			for(auto bosses : myGD->getMainCumberVector())
			{
				if(ccpLength(bosses->getPosition() - m_missileSprite->getPosition()) <= m_range)
				{
					isNearMonster = true;
					break;
				}
			}
			for(auto mob : myGD->getSubCumberVector())
			{
				if(ccpLength(mob->getPosition() - m_missileSprite->getPosition()) <= m_range)
				{
					isNearMonster = true;
					break;
				}
			}
			{
				float tt = atan2f(diffPosition.y, diffPosition.x); // 미사일에서 몬스터까지의 각도
				//KS::KSLog("% ~ % : %", deg2Rad(-90), deg2Rad(90), tt);
//				tt = clampf(tt, deg2Rad(-90), deg2Rad(90));
				
				//m_currentRad += clampf(tt - m_currentRad, deg2Rad(-15), deg2Rad(15));
				float tempTt = tt - m_currentRad;
				bool sign = tt - m_currentRad > 0  ? 1 : -1;
				float missileSpeed = m_initSpeed * 1.3f;
				if(isNearMonster)
				{
					m_currentRad += clampf((tt - m_currentRad), deg2Rad(-2.5f), deg2Rad(2.5f));
				}
				else 
				{
					//m_currentRad += clampf((tt - m_currentRad), deg2Rad(-0.8f), deg2Rad(0.8f)); // , deg2Rad(-15), deg2Rad(15));
				}
//				m_currentRad = m_currentRad + tt - m_currentRad;
				m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cos(m_currentRad) * missileSpeed,
																																					sin(m_currentRad) * missileSpeed));
				
				if(m_selfRotation)
				{
					m_missileSprite->setRotation(m_missileSprite->getRotation() + 20);
				}
				else
				{
					m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
				}
//				m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
		 	}
			
			if(m_particle)
				m_particle->setPosition(m_missileSprite->getPosition());
			
			if(m_streak)
				m_streak->setPosition(m_missileSprite->getPosition());
		}
	}
	void beautifier(int level)
	{
		makeBeautifier(level, m_streak, m_particle);
		if(m_streak)addChild(m_streak, -1);
		if(m_particle)addChild(m_particle, -2);
	}
//	void beautifier(int level, ASMotionStreak*& motionStreak, CCParticleSystemQuad*& particleQuad)
//	{
//		if(motionStreak == nullptr && particleQuad == nullptr)
//		{
//			motionStreak = addStreak(level);
//			
//			if(level <= 5)
//			{
//				
//			}
//			else if(level <= 10)
//			{
//				particleQuad = addParticle(1);
//			}
//			else if(level <= 15)
//			{
//				particleQuad = addParticle(2);
//			}
//			else if(level <= 20)
//			{
//				particleQuad = addParticle(3);
//				
//			}
//			else if(level <= 25)
//			{
//				particleQuad = addParticle(4);
//				
//			}
//			else if(level <= 30)
//			{
//				particleQuad = addParticle(5);
//			}
//			
//			if(particleQuad)
//			{
//
//				addChild(particleQuad, -1);
//				m_particle = particleQuad;
//	
//			}
//			if(motionStreak)
//			{
//
//				addChild(motionStreak, -2);
//				m_streak = motionStreak;
//			}
//		}
//		
//		else
//		{
//
//			addChild(particleQuad, -1);
//			addChild(motionStreak, -2);
//		}
//		//////////////////////////
//		
//
//	}
	

	
		// 반지름 설정
	void setShowWindowRotationRadius(float r)
	{

	}
	// 각속도 설정
	void setShowWindowVelocityRad(float r)
	{

	}
protected:
	float m_initSpeed; // 초기 속도.
	float m_initRad; // 처음에 날아가는 각도.
	float m_currentRad; // 범위내 들어왔을 때 현재 각도. 

	bool m_guided; // 유도 모드인지 여부.
	int m_range; // 유도 범위.
	bool m_selfRotation; // 스스로 도는지 여부.
	CCNode* m_targetNode;
	CCSprite* m_missileSprite; // 미사일 객체.
	CCParticleSystemQuad* m_particle;
	ASMotionStreak* m_streak;
	CCNode* m_start_node;
	struct ShowWindow
	{
		ShowWindow()
		{
			lastCreationTime = 0.f;
			currentTime = 0.f;
		}
		struct MissileSprite
		{
			MissileSprite()
			{
				streak = nullptr;
				particleQuad = nullptr;
			}

			CCSprite* missileSprite;
			ASMotionStreak* streak;
			CCParticleSystemQuad* particleQuad;
			float missileRad;
			float missileSpeed;
		};
		std::vector<MissileSprite> missileSprites;
		float lastCreationTime;
		float currentTime;
		std::string fileName;
		int grade, level;
		CCSprite* whiteBoard;
		CCClippingNode* clippingNode;
		CCSpriteBatchNode* explosionNode;
	}m_showWindow;
	int m_subPower;
	CC_SYNTHESIZE(int, m_power, Power); // 파워.
	
};



class StraightMissile : public StoneAttack
{
public:
	static StraightMissile* create(CCPoint initPosition, const string& fileName, float rad, float initSpeed, int power, int subPower, AttackOption ao)
	{
		StraightMissile* object = new StraightMissile();
		object->init(initPosition, fileName, rad, initSpeed, power, subPower, ao);

		object->autorelease();
		

		return object;
	}
	bool init(CCPoint initPosition, const string& fileName, float rad, float initSpeed, int power, int subPower, AttackOption ao)
	{
		StoneAttack::init();
		m_initSpeed = initSpeed;
		m_power = power;
		m_subPower = subPower;
		m_option = ao;
		m_missileSprite = CCSprite::create(fileName.c_str());
		addChild(m_missileSprite);
		m_missileSprite->setPosition(initPosition);
		m_missileSprite->setScale(1.f/myGD->game_scale);
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
				if(iter->getDeadState() == false && distance < 10)
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
			executeOption(dynamic_cast<KSCumberBase*>(minDistanceCumber), damage, m_subPower, 0.f, effectPosition);
			removeFromParentAndCleanup(true);
		}
		else  // 거리가 멀면 진행 시킴.
		{
			m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cos(m_initRad) * m_initSpeed,
																																				sin(m_initRad) * m_initSpeed));
			if(m_particle)
			{
				m_particle->setPosition(m_missileSprite->getPosition());
			}
			if(m_streak)
			{
				m_streak->setPosition(m_missileSprite->getPosition());
			}

			m_missileSprite->setRotation(-rad2Deg(m_initRad) - 90);
		}
	}
	void beautifier(int level)
	{
		makeBeautifier(level, m_streak, m_particle);
		if(m_streak)addChild(m_streak, -1);
		if(m_particle)addChild(m_particle, -2);
	}
	
	
protected:
	float m_initSpeed; // 초기 속도.
	float m_initRad; // 처음에 날아가는 각도.
	int m_power; // 파워.
	int m_subPower;
	CCParticleSystemQuad* m_particle;
	ASMotionStreak* m_streak;
	CCSprite* m_missileSprite; // 미사일 객체.
};

//SpreadMissile 적용할 차례.

////////////////////////////////////////////////////
class SpreadMissile : public StoneAttack
{
public:
	static SpreadMissile* create(KSCumberBase* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, int power, int subPower, int directions, int level, AttackOption ao)
	{
		SpreadMissile* object = new SpreadMissile();
		object->init(targetNode, initPosition, fileName, initSpeed, power, subPower, directions, level, ao);

		object->autorelease();
		

		return object;
	}
	virtual ~SpreadMissile()
	{
		CCLOG("Spread ~");
	}
	bool init(KSCumberBase* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, int power, int subPower, int directions,
						int level,
						AttackOption ao)
	{
		StoneAttack::init();
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		
		m_initSpeed = initSpeed * mInfo.get("speedbonus", 1.f).asFloat();
		m_power = power;
		m_subPower = subPower;
		m_targetNode = targetNode;
		m_option = ao;	
		CCPoint diff = targetNode->getPosition() - initPosition;
		float rad = atan2f(diff.y, diff.x);
		int numbers = directions * mInfo.get("dirsbonus", 1.f).asFloat();
		for(int i=0; i<numbers; i++)
		{
			StraightMissile* sm = StraightMissile::create(initPosition, fileName, rad + i * deg2Rad(360 / directions), initSpeed,
																										power, subPower, ao);
			
			addChild(sm);
			sm->beautifier(level);
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
	int m_subPower;
	int m_directions; // 몇 방향인지...
	bool m_cancelCasting;
	bool m_stiffen;
	CCNode* m_targetNode;
};

class MineAttack : public StoneAttack
{
public:
	static MineAttack* create(CCPoint initPosition, CCPoint goalPosition, const string& fileName, float tickCount, int power, int subPower, AttackOption ao)
	{
		MineAttack* ma = new MineAttack();
		ma->init(initPosition, goalPosition, fileName, tickCount, power, subPower, ao);
		ma->autorelease();
		return ma;
	}	

	bool init(CCPoint initPosition, CCPoint goalPosition, const string& fileName, int tickCount, int power, int subPower, AttackOption ao)
	{
		StoneAttack::init();
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		m_initPosition = initPosition;
		m_goalPosition = goalPosition;
		m_tickCount = tickCount * mInfo.get("tickbonus", 1.f).asFloat();
		m_initTickCount = tickCount * mInfo.get("tickbonus", 1.f).asFloat();
		m_power = power;
		m_subPower = subPower;
		m_fileName = fileName;
		m_option = ao;
		m_particle = nullptr;
		m_streak = nullptr;
		m_currentRad = 0.f;
		m_mine = CCSprite::create(fileName.c_str());
		m_mine->setScale(1.f/myGD->game_scale);
		m_mine->setRotation(180);
		addChild(m_mine);
//		goalPosition = goalPosition + ccp(10, 0);
		float runRad = atan2f(goalPosition.y - initPosition.y, goalPosition.x + 10 - initPosition.x);
		m_mine->setRotation(-rad2Deg(runRad));
		addChild(KSGradualValue<CCPoint>::create(initPosition, goalPosition, 1.f,
																				 [=](CCPoint t){
//																					 m_currentRad += M_PI / 180 * 2.f;
																					 CCPoint rotation = ccp(10 * cosf(m_currentRad), 10 * sinf(m_currentRad));
																					 m_mine->setPosition(t + rotation);
																					 CCLog("deg = %f", rad2Deg(runRad) + 90);
																					 m_mineCenterPosition = t;
																					 if(m_particle)
																					 {
																						 m_particle->setPosition(t + rotation);
																					 }
																					 if(m_streak)
																					 {
																						 m_streak->setPosition(t + rotation);
																					 }
																					 
																					 
																					
																				 },
																		[=](CCPoint t){
//																			m_currentRad += M_PI / 180 * 2.f;
																			CCPoint rotation = ccp(10 * cosf(m_currentRad), 10 * sinf(m_currentRad));
																			m_mine->setPosition(t + rotation);
																			m_mineCenterPosition = t;
																			if(m_particle)
																			{
																				m_particle->setPosition(t + rotation);
																			}
																			if(m_streak)
																			{
																				m_streak->setPosition(t + rotation);
																			}


																			scheduleUpdate();
																		}));
		return true;
	}
	void update(float dt)
	{
		// 여기서 몬스터와 부딪힌거 검사하면서 터짐.
		bool isEnable = true;
		bool emptyMonster = false;
		IntPoint missilePoint = ccp2ip(m_mine->getPosition());
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
		
		m_tickCount -= 1.f/60.f;

		if(m_tickCount <= 0)
		{
			removeFromParent();
		}

		CCPoint rotation = ccp(10 * cosf(m_currentRad), 10 * sinf(m_currentRad));
		m_currentRad += M_PI / 180 * 4.f;
		m_mine->setPosition(m_mineCenterPosition + rotation);
		m_mine->setRotation(CC_RADIANS_TO_DEGREES(m_currentRad)*-1+180);
		if(m_particle)
		{
			m_particle->setPosition(m_mineCenterPosition + rotation);
		}
		if(m_streak)
		{
			m_streak->setPosition(m_mineCenterPosition + rotation);
		}
		
		int surroundCnt = 0;
		IntPoint setPoint = ccp2ip(m_mineCenterPosition);
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
				MineAttack* ma = MineAttack::create(m_mineCenterPosition, ip2ccp(mapPoint), m_fileName, m_initTickCount, m_power, m_subPower, m_option);
				ma->beautifier(m_level);
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
			float distance = ccpLength(targetPosition - m_mineCenterPosition);
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
				float distance = ccpLength(targetPosition - m_mineCenterPosition);
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
			executeOption(dynamic_cast<KSCumberBase*>(minDistanceCumber), damage, m_subPower, 0.f, effectPosition);

			removeFromParentAndCleanup(true);
		}
	}
	void beautifier(int level)
	{
		m_level = level;
		makeBeautifier(level, m_streak, m_particle);
		if(m_streak)
			addChild(m_streak, -1);
		if(m_particle)
			addChild(m_particle, -2);
	}
		

protected:
	CCPoint m_initPosition;
	CCPoint m_goalPosition;
	float m_tickCount;
	float m_initTickCount;
	int m_power;
	int m_subPower;
	int m_level;
	CCSprite* m_mine;
	CCParticleSystemQuad* m_particle;
	std::string m_fileName;
	ASMotionStreak* m_streak;
	CCPoint m_mineCenterPosition;
	float m_currentRad;
};



class SpiritAttack : public StoneAttack
{
public:
	static SpiritAttack* create(CCPoint initPosition, CCPoint goalPosition, const string& fileName, float tickCount, int power, int subPower,
															float speed, int coolFrame, AttackOption ao)
	{
		SpiritAttack* ma = new SpiritAttack();
		ma->init(initPosition, goalPosition, fileName, tickCount, power, subPower, speed, coolFrame, ao);
		ma->autorelease();
		return ma;
	}	

	bool init(CCPoint initPosition, CCPoint goalPosition, const string& fileName, int tickCount, int power, int subPower, float speed, int coolFrame, AttackOption ao)
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
																					 m_mine->setRotation(-rad2Deg(rad));
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
																								m_initTickCount, m_power, m_subPower, m_speed, m_coolFrame, m_option);
				ma->beautifier(m_level);
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
				executeOption(dynamic_cast<KSCumberBase*>(minDistanceCumber), damage, m_subPower, 0.f, effectPosition);

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
	
	void beautifier(int level)
	{
		m_level = level;
		makeBeautifier(level, m_streak, m_particle);
		if(m_streak)addChild(m_streak, -1);
		if(m_particle)addChild(m_particle, -2);
	}
	
	
protected:
	CCPoint m_initPosition;
	CCPoint m_goalPosition;
	float m_tickCount;
	float m_initTickCount;
	int m_power;
	int m_subPower;
											 int m_level;
											
	CCSprite* m_mine;
	CCParticleSystemQuad* m_particle;
	ASMotionStreak* m_streak;
	float m_directionRad;
	float m_speed;
	int m_coolFrame; // 0 이어야 타격이 가능함. 공격할 때 initCoolFrame 으로 초기화되고 매 프레임당 -- 됨.
	int m_initCoolFrame;
//	int m_level;
	std::string m_fileName;
};


class RangeAttack : public StoneAttack
{
public:
	struct Missile
	{
		CCSprite* missileSprite;
		CCParticleSystemQuad* particle;
		ASMotionStreak* streak;
		float radius;
		Missile() : missileSprite(nullptr), particle(nullptr), streak(nullptr)
		{
			
		}
	};
	static RangeAttack* create(CCPoint initPosition, const std::string& fileName, float radius, int durationFrame, int power, int subPower, AttackOption ao)
	{
		RangeAttack* obj = new RangeAttack();
		obj->init(initPosition, fileName, radius, durationFrame, power, subPower, ao);
		obj->autorelease();
		return obj;
	}
	virtual ~RangeAttack()
	{
		CCLOG("~RangeAttack");
	}
	bool init(CCPoint initPosition, const std::string& fileName, float radius, int durationFrame, int power, int subPower, AttackOption ao)
	{
		StoneAttack::init();

		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
	
		m_radius = radius * mInfo.get("radiusbonus", 1.f).asFloat();
		m_durationFrame = durationFrame * mInfo.get("durationbonus", 1.f).asFloat();
		m_power = power;
		m_subPower = subPower;
		m_currentRad = 0;
		m_initPosition = initPosition;
		m_particle = nullptr;
		m_streak = nullptr;
		m_radVelocityBonus = mInfo.get("radvelocitybonus", 1.f).asFloat();
//		m_initJiggleInterval = jiggleInterval * mInfo.get("intervalbonus", 1.f).asFloat();
//		m_jiggleInterval = 0;
		m_option = ao;
		for(float r = 15; r<=m_radius; r+=15)
		{
			Missile m;
			m.radius = r;
			m.missileSprite = CCSprite::create(fileName.c_str());
			m.particle = nullptr;
			m.streak = nullptr;
			makeBeautifier(25, m.streak,m.particle);
			addChild(m.missileSprite);
			if(m.particle)addChild(m.particle,-1);
			if(m.streak)addChild(m.streak,-2);
			m.missileSprite->setScale(1 / myGD->game_scale);
			m.missileSprite->setPosition(m_initPosition + ccp(r * cosf(m_currentRad), r * sinf(m_currentRad)));
			m_sprites.push_back(m);
		}
//		CCSprite* spr = KS::loadCCBI<CCSprite*>(this, "me_scope.ccbi").first;
//		spr->setPosition(initPosition);
//		spr->setScale(radius / 100.f);
//		spr->setOpacity(100);
//		addChild(spr);
//		m_rangeSprite = spr;
		scheduleUpdate();
		return true;	
	}
	void update(float dt)
	{
//		m_jiggleInterval = MAX(0, m_jiggleInterval - 1);
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
		m_durationFrame--;
		if(m_durationFrame <= 0)
		{
			removeFromParent();
			return;
		}
		m_currentRad += M_PI / 180.f * 3.f * m_radVelocityBonus;
		for(auto& m : m_sprites)
		{
			m.missileSprite->setPosition(m_initPosition + ccp(m.radius * cosf(m_currentRad), m.radius * sinf(m_currentRad)));
			m.missileSprite->setRotation(CC_RADIANS_TO_DEGREES(m_currentRad)*-1+90);
			if(m.particle)
			{
				m.particle->setPosition(m.missileSprite->getPosition());
			}
			if(m.streak)
			{
				m.streak->setPosition(m.missileSprite->getPosition());
			}
		}
		
			std::vector<KSCumberBase*> nearMonsters;
			for(auto iter : myGD->getMainCumberVector())
			{
				CCPoint targetPosition = iter->getPosition();
				float diffRad = atan2f(targetPosition.y - m_initPosition.y, targetPosition.x - m_initPosition.x);
				if(diffRad < 0)
					diffRad += 2 * M_PI;
				else
				{
					diffRad = diffRad - (int)(diffRad / (2 * M_PI)) * 2 * M_PI;
				}
				m_currentRad = m_currentRad - (int)(m_currentRad / (2 * M_PI)) * 2 * M_PI;
				float distance = ccpLength(m_initPosition - iter->getPosition());
				if(fabsf( m_currentRad - diffRad ) <= M_PI / 180.f * 5.f && distance <= m_radius * 1.2f)
				{
					nearMonsters.push_back(iter);
				}
			}
			for(auto iter : myGD->getSubCumberVector())
			{
				CCPoint targetPosition = iter->getPosition();
				float diffRad = atan2f(targetPosition.y - m_initPosition.y, targetPosition.x - m_initPosition.x);
				if(diffRad < 0)
					diffRad += 2 * M_PI;
				else
				{
					diffRad = diffRad - (int)(diffRad / (2 * M_PI)) * 2 * M_PI;
				}
				m_currentRad = m_currentRad - (int)(m_currentRad / (2 * M_PI)) * 2 * M_PI;
				float distance = ccpLength(m_initPosition - iter->getPosition());
				if(fabsf( m_currentRad - diffRad ) <= M_PI / 180.f * 5.f && distance <= m_radius * 1.2f)
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
				executeOption(dynamic_cast<KSCumberBase*>(iter), damage, m_subPower, 0.f, effectPosition);
			}

//		if(m_jiggleInterval == 0)
//		{
//			m_jiggleInterval = m_initJiggleInterval;
//			// 미사일과 몬스터와 거리가 2 보다 작은 경우가 있다면 폭발 시킴.
//			std::vector<KSCumberBase*> nearMonsters;
//			for(auto iter : myGD->getMainCumberVector())
//			{
//				CCPoint targetPosition = iter->getPosition();
//				float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
//				if(distance < m_radius)
//				{
//					nearMonsters.push_back(iter);
//				}
//			}	
//			for(auto iter : myGD->getSubCumberVector())
//			{
//				CCPoint targetPosition = iter->getPosition();
//				float distance = ccpLength(targetPosition - m_rangeSprite->getPosition());
//				if(distance < m_radius)
//				{
//					nearMonsters.push_back(iter);
//				}
//			}	
//			
//			for(auto iter : nearMonsters)
//			{
//				CCPoint effectPosition = iter->getPosition();
//				effectPosition.x += rand()%21 - 10;
//				effectPosition.y += rand()%21 - 10;
//
//				float damage = m_power;
//				executeOption(dynamic_cast<KSCumberBase*>(iter), damage, m_subPower, 0.f, effectPosition);
//			}
//		}
	}
	
	void beautifier(int level)
	{
		makeBeautifier(level, m_streak, m_particle);
		if(m_streak)addChild(m_streak, -1);
		if(m_particle)addChild(m_particle, -2);
	}
	

protected:
	float m_radius;
	float m_durationFrame;
	int m_power;
	int m_subPower;
	float m_currentRad;
	CCPoint m_initPosition;
	float m_radVelocityBonus;
	std::vector<Missile> m_sprites;
//	int m_jiggleInterval;
//	int m_initJiggleInterval;		
	CCSprite* m_rangeSprite;
	CCParticleSystemQuad* m_particle;
	ASMotionStreak* m_streak;
};


class RandomBomb : public StoneAttack
{
public:
	static RandomBomb* create(float radius, int power, int subPower, AttackOption ao)
	{
		RandomBomb* obj = new RandomBomb();
		obj->init(radius, power, subPower, ao);
		obj->autorelease();
		return obj;
	}
	bool init(float radius, int power, int subPower, AttackOption ao)
	{
		StoneAttack::init();

		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		m_radius = radius * mInfo.get("radiusbonus", 1.f).asFloat();
		m_power = power;
		m_subPower = subPower;
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
																								 executeOption(dynamic_cast<KSCumberBase*>(iter), damage, m_subPower, 0.f, effectPosition);
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
	int m_subPower;
	CCSprite* m_rangeSprite;
};


class LaserAttack : public StoneAttack
{
public:
	static LaserAttack* create(float rad, int durationFrame, int power, int subPower, AttackOption ao)
	{
		LaserAttack* obj = new LaserAttack();
		obj->init(rad, durationFrame, power, subPower, ao);
		obj->autorelease();
		return obj;
	}
	virtual ~LaserAttack()
	{
		CCLOG("~LaserAttack");
	}
	bool init(float rad, int durationFrame, int power, int subPower, AttackOption ao)
	{
		StoneAttack::init();

		CCPoint randomPosition = ip2ccp(IntPoint(ks19937::getIntValue(mapLoopRange::mapWidthInnerBegin, mapLoopRange::mapWidthInnerEnd - 1),
																						 ks19937::getIntValue(mapLoopRange::mapHeightInnerBegin, mapLoopRange::mapHeightInnerEnd - 1)));
		m_rad = rad;
		m_power = power;
		m_subPower = subPower;
		m_option = ao;
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
	
		m_durationFrame = durationFrame * mInfo.get("durationbonus", 1.f).asFloat();
		
		m_initJiggleInterval = 30 * mInfo.get("intervalbonus", 1.f).asFloat();
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
		CCSprite* testBody = KS::loadCCBI<CCSprite*>(this, "me_laser_body.ccbi").first;
		testBody->setPosition(laserHead->getPosition() + ccp(30, 0));
		laserContainer->addChild(testBody);

		CCPoint laserPosition = ccp(cos(m_rad)*ccpLength(testBody->getPosition()), 
																sin(m_rad)*ccpLength(testBody->getPosition()));

		laserContainer->setRotation(-rad2Deg(m_rad));
		float laserLength = 0;
		for(int i=0; ; i++)
		{
			CCPoint testBody = (laserHead->getPosition() + ccp(30 + i * 10, 0));
			
			CCPoint laserPosition = ccp(cos(m_rad)*ccpLength(testBody),
																	sin(m_rad)*ccpLength(testBody));
			if(ccp2ip(laserContainer->getPosition() + laserPosition ).isInnerMap() == false)
			{
				laserLength = ccpLength(laserPosition);
				break;
			}
		}
		testBody->setScaleX(laserLength/10.f);
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
			for(int r=m_radius; ;r += 10)
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
				executeOption(dynamic_cast<KSCumberBase*>(monsters), damage, m_subPower, 0.f, effectPosition);
			}
		}
	}
protected:
	float m_rad;
	int m_power;
	int m_subPower;
	int m_durationFrame;
	int m_radius;
	CCPoint m_startPosition;
	int m_jiggleInterval;
	int m_initJiggleInterval;		
};



class LaserWrapper : public StoneAttack
{
public:
	static LaserWrapper* create(int numbers, int durationFrame, int power, int subPower, AttackOption ao)
	{
		LaserWrapper* obj = new LaserWrapper();
		obj->init(numbers, durationFrame, power, subPower, ao);
		obj->autorelease();
		return obj;
	}
	bool init(int numbers, int durationFrame, int power, int subPower, AttackOption ao)
	{
		StoneAttack::init();
		float addRad = ks19937::getDoubleValue(0, M_PI * 2.f);
		for(int r=0; r<numbers; r++)
		{
			float rad = deg2Rad(360.f / numbers * r);
			rad += addRad;
			addChild(LaserAttack::create(rad, durationFrame, power, subPower, ao));
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
	void beautifier(int level)
	{
		makeBeautifier(level, m_streak, m_particle);
		if(m_streak)addChild(m_streak, -1);
		if(m_particle)addChild(m_particle, -2);
	}
	

protected:
	int m_numbers;
	int m_power;
	int m_subPower;
	int m_durationFrame;
	CCParticleSystemQuad* m_particle;
	ASMotionStreak* m_streak;
};









class ProtectorMissile : public StoneAttack
{
public:
	static ProtectorMissile* create(CCNode* targetNode, CCPoint initPosition, const string& fileName,
																	float initSpeed, int power, int subPower, int range, float finalRadius, int lifeFrames, AttackOption ao, bool selfRotation)
	{
		ProtectorMissile* object = new ProtectorMissile();
		object->init(targetNode, initPosition, fileName, initSpeed, power, subPower, range, finalRadius, lifeFrames, ao, selfRotation);
		
		object->autorelease();
		
		
		return object;
	}
	static ProtectorMissile* createForShowWindow(const string& fileName, bool selfRotation, int grade, int level)
	{
		ProtectorMissile* object = new ProtectorMissile();
		object->initForShowWindow(fileName, selfRotation, grade, level);
		object->autorelease();
		return object;
	}
	static ProtectorMissile* createForShowStartSettingPopup(CCNode* start_node, string filename, bool selfRotation, int level, float finalRadius)
	{
		ProtectorMissile* object = new ProtectorMissile();
		object->initForShowStartSettingPopup(start_node, filename, selfRotation, level, finalRadius);
		object->autorelease();
		return object;
	}
	
	bool init(CCNode* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, int power, int subPower, int range,
						float finalRadius, int lifeFrames, AttackOption ao, bool selfRotation)
	{
		StoneAttack::init();
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		m_hiding = false;
		m_missileStep = 1;
		m_finalRadius = finalRadius * mInfo.get("radiusbonus", 1.f).asFloat();
		m_currentRadius = 0;
		m_particle = NULL;
		m_streak = NULL;
		m_start_node = NULL;
		m_lifeFrames = lifeFrames * mInfo.get("lifebonus", 1.f).asFloat();
		
		m_initSpeed = initSpeed * mInfo.get("speedbonus", 1.f).asFloat();
		m_option = ao;
		m_power = power;
		m_subPower = subPower;
		m_targetNode = targetNode;
		m_guided = false;
		m_range = range;
		m_selfRotation = selfRotation;
		m_missileSprite = CCSprite::create(fileName.c_str()); // KS::loadCCBI<CCSprite*>(this, fileName).first;
		//addChild(KSGradualValue<float>::create(0, 360 * 99, 5, [=](float t){
		//m_missileSprite->setRotationY(t);
		//m_missileSprite->setRotationX(t);
		//}));
		addChild(m_missileSprite);
		m_missileSprite->setScale(1.f/myGD->game_scale);
		m_missileSprite->setPosition(initPosition);
		
		if(targetNode)
		{
			CCPoint diff = m_targetNode->getPosition() - initPosition;
			
			int random_value = rand()%21 - 10; // -10~10
			float random_float = 1.f + random_value/100.f;
			random_float = 1.f;
			m_initRad = atan2f(diff.y, diff.x) * random_float;
			m_currentRad = m_initRad; // + ks19937::getFloatValue(deg2Rad(-45), deg2Rad(45));
			scheduleUpdate();
		}
		return true;
	}
	
	bool initForShowStartSettingPopup(CCNode* start_node, string filename, bool selfRotation, int level, float finalRadius)
	{
		StoneAttack::init();
		m_start_node = start_node;
		m_showWindow.fileName = filename;
		
		CCLOG("fileName = %s", m_showWindow.fileName.c_str());
		m_selfRotation = selfRotation;
//		m_showWindow.grade = grade;
		m_showWindow.level = level;
		schedule(schedule_selector(ThisClassType::showWindow));
		
		m_showWindow.whiteBoard = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 150));
		//m_back->setOpacity(color.a);
		
		//addChild(m_showWindow.whiteBoard);
		m_showWindow.clippingNode = CCClippingNode::create();
		CCClippingNode* cNode = m_showWindow.clippingNode;
		//cNode->setContentSize(CCSizeMake(100, 100));
		//cNode->setAnchorPoint(ccp(0.5,0.f));
		cNode->setPosition(ccp(0,0));
		cNode->setStencil(m_showWindow.whiteBoard);
		cNode->setInverted(false);
		this->addChild(cNode,1);
		m_showWindow.explosionNode = CCSpriteBatchNode::create("fx_monster_hit.png");
		
		cNode->addChild(m_showWindow.explosionNode);
		
		
		/////////////////////////////////////////////////
		m_finalRadius = finalRadius;
		m_currentRadius = 0;
		m_particle = NULL;
		m_streak = NULL;
		m_missileStep = 1;
		m_lifeFrames = 999999;
		m_missileSprite = CCSprite::create(filename.c_str());
		addChild(m_missileSprite);
		
//		m_missileSprite->setPosition( m_start_node->getPosition() - getPosition() );
//		m_missileSprite->setPosition(start_node->getPosition());
		return true;
	}
	
	bool initForShowWindow(const string& fileName, bool selfRotation, int grade, int level )
	{
		StoneAttack::init();
		m_start_node = NULL;
		m_showWindow.fileName = fileName;
		m_selfRotation = selfRotation;
		m_showWindow.grade = grade;
		m_showWindow.level = level;
//		schedule(schedule_selector(ThisClassType::showWindow));
		
		m_showWindow.whiteBoard = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 90));
		//m_back->setOpacity(color.a);
		
		//addChild(m_showWindow.whiteBoard);
		m_showWindow.clippingNode = CCClippingNode::create();
		CCClippingNode* cNode = m_showWindow.clippingNode;
		//cNode->setContentSize(CCSizeMake(100, 100));
		//cNode->setAnchorPoint(ccp(0.5,0.f));
		cNode->setPosition(ccp(0,0));
		cNode->setStencil(m_showWindow.whiteBoard);
		cNode->setInverted(false);
		this->addChild(cNode,1);
		m_showWindow.explosionNode = CCSpriteBatchNode::create("fx_monster_hit.png");
		
		cNode->addChild(m_showWindow.explosionNode);
		
		return true;
	}
	
	function<void(void)> m_func;
	void setFunctionForCrash(function<void(void)> func){
		this->m_func = func;
	}
	void showWindow(float dt)
	{
		if(m_missileStep == 1)
		{
			m_lifeFrames--;
			TRACE();
			if(m_lifeFrames <= 0 && m_hiding == false)
			{
				m_hiding = true;
				addChild(KSGradualValue<float>::create(255, 0, 1.f, [=](float t)
																							 {
																								 m_missileSprite->setOpacity(t);
																								 
																							 }, [=](float t)
																							 {
																								 m_missileSprite->removeFromParent();
																								 m_missileSprite = nullptr;
																								 
																							 }));
			}
			TRACE();
			m_currentRadius += 0.4f;
			m_currentRad += M_PI / 180.f * 4.f;
			
			CCPoint xy = m_start_node->getPosition() - getPosition();
//			CCPoint xy = m_start_node->getPosition() +
//			ccp(m_currentRadius * cosf(m_currentRad), m_currentRadius * sinf(m_currentRad));
			if(m_missileSprite)
			{
				m_missileSprite->setPosition(xy);
			}
			
			if(m_currentRadius >= m_finalRadius)
			{
				m_currentRadius = m_finalRadius;
			}
			
			if(m_hiding == false)
			{
				TRACE();
				CCPoint minDis = ccp(m_currentRadius, m_currentRadius);
				KSCumberBase* nearCumber = nullptr;
				CCPoint nowDis = CCPointZero - m_missileSprite->getPosition();
				if(ccpLength(nowDis)<ccpLength(minDis))
				{
					nearCumber = (KSCumberBase*)true;
					minDis = nowDis;
				}
				if(nearCumber)
				{
					m_missileStep = 3;
				}
			}
		}
		else if(m_missileStep == 2)
		{
			
		}
		else if(m_missileStep == 3)
		{
			TRACE();
			// 공격나가는 도중임...
			CCPoint targetPosition = CCPointZero;
			CCPoint subDistance = ccpSub(targetPosition, m_missileSprite->getPosition());
			float distance = sqrtf(powf(subDistance.x, 2.f) + powf(subDistance.y, 2.f));
			//			CCLOG("distance : %f", distance);
			
			// 몬스터가 맞는 조건
			if(distance <= 6)
			{
				AudioEngine::sharedInstance()->playEffect("se_monattacked.mp3", false);
				
				CCPoint effectPosition = m_missileSprite->getPosition();
				effectPosition.x += rand()%21 - 10;
				effectPosition.y += rand()%21 - 10;
				
				float damage = m_power;
//				executeOption(dynamic_cast<KSCumberBase*>(m_targetNode), damage, m_subPower, 0.f, effectPosition);
				TRACE();
				
				removeFromParentAndCleanup(true);
			}
			else  // 거리가 멀면 몬스터 쪽으로 유도함.
			{
				TRACE();
				CCPoint missilePosition = m_missileSprite->getPosition();
				CCPoint cumberPosition;
				cumberPosition = CCPointZero;
				CCPoint diffPosition = cumberPosition - missilePosition;
				
				bool isNearMonster = false;
				if(ccpLength(CCPointZero - m_missileSprite->getPosition()) <= m_range)
				{
					isNearMonster = true;
				}
				float tt = atan2f(diffPosition.y, diffPosition.x); // 미사일에서 몬스터까지의 각도
				//KS::KSLog("% ~ % : %", deg2Rad(-90), deg2Rad(90), tt);
				//				tt = clampf(tt, deg2Rad(-90), deg2Rad(90));
				
				//m_currentRad += clampf(tt - m_currentRad, deg2Rad(-15), deg2Rad(15));
				float tempTt = tt - m_currentRad;
				bool sign = tt - m_currentRad > 0  ? 1 : -1;
				float missileSpeed = m_initSpeed * 1.3f;
				if(isNearMonster)
				{
					m_currentRad += clampf((tt - m_currentRad), deg2Rad(-2.f), deg2Rad(2.f));
				}
				else
				{
					//m_currentRad += clampf((tt - m_currentRad), deg2Rad(-0.8f), deg2Rad(0.8f)); // , deg2Rad(-15), deg2Rad(15));
				}
				//				m_currentRad = m_currentRad + tt - m_currentRad;
				m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cos(m_currentRad) * missileSpeed,
																																					sin(m_currentRad) * missileSpeed));
				
				if(m_selfRotation)
				{
					m_missileSprite->setRotation(m_missileSprite->getRotation() + 20);
				}
				else
				{
					m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
				}
				//				m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
			}
		}
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
		
		
		if(m_missileStep == 1)
		{
			m_lifeFrames--;
			if(m_lifeFrames <= 0 && m_hiding == false)
			{
				m_hiding = true;
				if(m_particle)
				{
					m_particle->setEmissionRate(0);
				}
				if(m_streak)
				{
					m_streak->removeFromParent();
					m_streak = nullptr;
				}
				addChild(KSGradualValue<float>::create(255, 0, 1.f, [=](float t)
																							 {
																								 m_missileSprite->setOpacity(t);

																								 
																							 }, [=](float t)
																							 {
																								 this->removeFromParent();
//																								 m_missileSprite->removeFromParent();
																								 m_missileSprite = nullptr;
																								
																							 }));
			}
			m_currentRadius += 0.4f;
			m_currentRad += M_PI / 180.f * 4.f;
			
			CCPoint xy = myGD->getJackPointCCP() +
					ccp(m_currentRadius * cosf(m_currentRad), m_currentRadius * sinf(m_currentRad));
			if(m_missileSprite)
			{
				m_missileSprite->setPosition(xy);
				m_missileSprite->setRotation(CC_RADIANS_TO_DEGREES(m_currentRad)*-1+180);
				
				if(m_particle)
					m_particle->setPosition(m_missileSprite->getPosition());
				
				if(m_streak)
					m_streak->setPosition(m_missileSprite->getPosition());
			}
			
			if(m_currentRadius >= m_finalRadius)
			{
				m_currentRadius = m_finalRadius;
			}
			
			if(m_hiding == false)
			{
				std::vector<KSCumberBase*> targets;
				KSCumberBase* target = nullptr;
				targets.insert(targets.end(), myGD->getMainCumberVector().begin(), myGD->getMainCumberVector().end());
				targets.insert(targets.end(), myGD->getSubCumberVector().begin(), myGD->getSubCumberVector().end());
				//			target = targets[ks19937::getIntValue(0, targets.size() - 1)];
				
				CCPoint minDis = ccp(m_currentRadius, m_currentRadius);
				KSCumberBase* nearCumber = nullptr;
				for(int i = 0; i<myGD->getMainCumberCount();i++)
				{
					KSCumberBase* cumber = myGD->getMainCumberVector()[i];
					CCPoint nowDis = cumber->getPosition() - m_missileSprite->getPosition();
					if(ccpLength(nowDis)<ccpLength(minDis))
					{
						nearCumber=cumber;
						minDis = nowDis;
					}
				}
				
				for(int i = 0; i<myGD->getSubCumberCount();i++){
					KSCumberBase* cumber = myGD->getSubCumberVector()[i];
					CCPoint nowDis = cumber->getPosition() - m_missileSprite->getPosition();
					if(cumber->getDeadState() == false)
					{
						if(ccpLength(nowDis)<ccpLength(minDis))
						{
							nearCumber=cumber;
							minDis = nowDis;
						}
					}
				}
				
				if(nearCumber)
				{
					m_missileStep = 3;
					
					m_targetNode = nearCumber;
					CCPoint diff = m_targetNode->getPosition() - m_missileSprite->getPosition();
					
					int random_value = rand()%21 - 10; // -10~10
					float random_float = 1.f + random_value/100.f;
					random_float = 1.f;
					m_initRad = atan2f(diff.y, diff.x) * random_float;
					m_currentRad = m_initRad; // + ks19937::getFloatValue(deg2Rad(-45), deg2Rad(45));
				}
			}
		}
		else if(m_missileStep == 2)
		{
						
		}
		else if(m_missileStep == 3)
		{
			
			// 공격나가는 도중임...
			CCPoint targetPosition = m_targetNode->getPosition();
			CCPoint subDistance = ccpSub(targetPosition, m_missileSprite->getPosition());
			float distance = sqrtf(powf(subDistance.x, 2.f) + powf(subDistance.y, 2.f));
//			CCLOG("distance : %f", distance);
			
			// 몬스터가 맞는 조건
			if(distance <= 6)
			{
				AudioEngine::sharedInstance()->playEffect("se_monattacked.mp3", false);
				
				CCPoint effectPosition = m_missileSprite->getPosition();
				effectPosition.x += rand()%21 - 10;
				effectPosition.y += rand()%21 - 10;
				
				float damage = m_power;
				executeOption(dynamic_cast<KSCumberBase*>(m_targetNode), damage, m_subPower, 0.f, effectPosition);
				
				removeFromParentAndCleanup(true);
			}
			else  // 거리가 멀면 몬스터 쪽으로 유도함.
			{
				CCPoint missilePosition = m_missileSprite->getPosition();
				CCPoint cumberPosition;
				cumberPosition = m_targetNode->getPosition();
				CCPoint diffPosition = cumberPosition - missilePosition;
				
				bool isNearMonster = false;
				for(auto bosses : myGD->getMainCumberVector())
				{
					if(ccpLength(bosses->getPosition() - m_missileSprite->getPosition()) <= m_range)
					{
						isNearMonster = true;
						break;
					}
				}
				for(auto mob : myGD->getSubCumberVector())
				{
					if(ccpLength(mob->getPosition() - m_missileSprite->getPosition()) <= m_range)
					{
						isNearMonster = true;
						break;
					}
				}
				
				float tt = atan2f(diffPosition.y, diffPosition.x); // 미사일에서 몬스터까지의 각도
				//KS::KSLog("% ~ % : %", deg2Rad(-90), deg2Rad(90), tt);
				//				tt = clampf(tt, deg2Rad(-90), deg2Rad(90));
				
				//m_currentRad += clampf(tt - m_currentRad, deg2Rad(-15), deg2Rad(15));
				float tempTt = tt - m_currentRad;
				bool sign = tt - m_currentRad > 0  ? 1 : -1;
				float missileSpeed = m_initSpeed * 1.3f;
				if(isNearMonster)
				{
					m_currentRad += clampf((tt - m_currentRad), deg2Rad(-2.f), deg2Rad(2.f));
				}
				else
				{
					//m_currentRad += clampf((tt - m_currentRad), deg2Rad(-0.8f), deg2Rad(0.8f)); // , deg2Rad(-15), deg2Rad(15));
				}
				//				m_currentRad = m_currentRad + tt - m_currentRad;
				m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cos(m_currentRad) * missileSpeed,
																																					sin(m_currentRad) * missileSpeed));
				
				if(m_selfRotation)
				{
					m_missileSprite->setRotation(m_missileSprite->getRotation() + 20);
				}
				else
				{
					m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
				}
					//				m_missileSprite->setRotation(-rad2Deg(m_currentRad) - 90);
		 	}
		}
	}
	void beautifier(int level)
	{
		makeBeautifier(level, m_streak, m_particle);
		if(m_streak)addChild(m_streak, -1);
		if(m_particle)addChild(m_particle, -2);
	}
	
	// 반지름 설정
	void setShowWindowRotationRadius(float r)
	{
		
	}
	// 각속도 설정
	void setShowWindowVelocityRad(float r)
	{
		
	}
	CCSprite* m_missileSprite; // 미사일 객체.
protected:
	int m_missileStep; // 미사일 단계 : 1 = 캐릭터로 부터 빙글빙글 돌면서 나타나는 과정 2 = 몬스터를 찾는 과정 3 = 날아갈 때.
	float m_initSpeed; // 초기 속도.
	float m_initRad; // 처음에 날아가는 각도.
	float m_currentRad; // 현재 각도.
	float m_finalRadius; // 최종 반지름
	float m_currentRadius; // 반지름이 커지는 도중의 변수
	int m_lifeFrames;
	
	bool m_hiding; // 사라지고 있다.
	bool m_guided; // 유도 모드인지 여부.
	int m_range; // 유도 범위.
	bool m_selfRotation; // 스스로 도는지 여부.
	CCNode* m_targetNode;
	
	CCParticleSystemQuad* m_particle;
	ASMotionStreak* m_streak;
	CCNode* m_start_node;
	struct ShowWindow
	{
		ShowWindow()
		{
			lastCreationTime = 0.f;
			currentTime = 0.f;
		}
		struct MissileSprite
		{
			MissileSprite()
			{
				streak = nullptr;
				particleQuad = nullptr;
			}
			
			CCSprite* missileSprite;
			ASMotionStreak* streak;
			CCParticleSystemQuad* particleQuad;
			float missileRad;
			float missileSpeed;
		};
		std::vector<MissileSprite> missileSprites;
		float lastCreationTime;
		float currentTime;
		std::string fileName;
		int grade, level;
		CCSprite* whiteBoard;
		CCClippingNode* clippingNode;
		CCSpriteBatchNode* explosionNode;
	}m_showWindow;
	int m_subPower;
	CC_SYNTHESIZE(int, m_power, Power); // 파워.
};

class SlowAttack : public StoneAttack
{
public:
	static SlowAttack* create(CCNode* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, float initRad, int power, int subPower,
														AttackOption ao)
	{
		SlowAttack* object = new SlowAttack();
		object->init(targetNode, initPosition, fileName, initSpeed, initRad, power, subPower, ao);
		
		object->autorelease();
		
		
		return object;
	}
	static SlowAttack* createForShowWindow(const string& fileName, bool selfRotation, int grade, int level)
	{
		SlowAttack* object = new SlowAttack();
		object->initForShowWindow(fileName, selfRotation, grade, level);
		object->autorelease();
		return object;
	}
	static SlowAttack* createForShowStartSettingPopup(CCNode* start_node, const string& filename, bool selfRotation, int grade, int level)
	{
		SlowAttack* object = new SlowAttack();
		object->initForShowStartSettingPopup(start_node, filename, selfRotation, grade, level);
		object->autorelease();
		return object;
	}
	
	bool init(CCNode* targetNode, CCPoint initPosition, const string& fileName, float initSpeed, float initRad, int power, int subPower,
						AttackOption ao)
	{
		StoneAttack::init();
		m_missileStep = 1;
		m_particle = NULL;
		m_streak = NULL;
		m_start_node = NULL;
		
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		m_initSpeed = initSpeed * mInfo.get("speedbonus", 1.f).asFloat();
		m_option = ao;
		m_power = power;
		m_subPower = subPower;
		m_targetNode = targetNode;
		m_initRad = initRad;
		
		m_missileSprite = CCSprite::create(fileName.c_str()); // KS::loadCCBI<CCSprite*>(this, fileName).first;
		m_missileSprite->setScale(1.f/myGD->game_scale);
		//addChild(KSGradualValue<float>::create(0, 360 * 99, 5, [=](float t){
		//m_missileSprite->setRotationY(t);
		//m_missileSprite->setRotationX(t);
		//}));
		addChild(m_missileSprite);
		m_missileSprite->setScale(1.f/myGD->game_scale);
		m_missileSprite->setPosition(initPosition);
		
		scheduleUpdate();
//		if(targetNode)
//		{
//			CCPoint diff = m_targetNode->getPosition() - initPosition;
//			
//			int random_value = rand()%21 - 10; // -10~10
//			float random_float = 1.f + random_value/100.f;
//			random_float = 1.f;
//			m_initRad = atan2f(diff.y, diff.x) * random_float;
//			scheduleUpdate();
//		}
		return true;
	}
	
	bool initForShowStartSettingPopup(CCNode* start_node, const string& fileName, bool selfRotation, int grade, int level)
	{
		StoneAttack::init();
		m_start_node = start_node;
		m_showWindow.fileName = fileName;
//		m_selfRotation = selfRotation;
		m_showWindow.grade = grade;
		m_showWindow.level = level;
		schedule(schedule_selector(ThisClassType::showWindow));
		
		m_showWindow.whiteBoard = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 150));
		//m_back->setOpacity(color.a);
		
		//addChild(m_showWindow.whiteBoard);
		m_showWindow.clippingNode = CCClippingNode::create();
		CCClippingNode* cNode = m_showWindow.clippingNode;
		//cNode->setContentSize(CCSizeMake(100, 100));
		//cNode->setAnchorPoint(ccp(0.5,0.f));
		cNode->setPosition(ccp(0,0));
		cNode->setStencil(m_showWindow.whiteBoard);
		cNode->setInverted(false);
		this->addChild(cNode,1);
		m_showWindow.explosionNode = CCSpriteBatchNode::create("fx_monster_hit.png");
		
		cNode->addChild(m_showWindow.explosionNode);
		
		return true;
	}
	
	bool initForShowWindow(const string& fileName, bool selfRotation, int grade, int level )
	{
		StoneAttack::init();
		m_start_node = NULL;
		m_showWindow.fileName = fileName;
		m_showWindow.grade = grade;
		m_showWindow.level = level;
		schedule(schedule_selector(ThisClassType::showWindow));
		
		m_showWindow.whiteBoard = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 90));
		//m_back->setOpacity(color.a);
		
		//addChild(m_showWindow.whiteBoard);
		m_showWindow.clippingNode = CCClippingNode::create();
		CCClippingNode* cNode = m_showWindow.clippingNode;
		//cNode->setContentSize(CCSizeMake(100, 100));
		//cNode->setAnchorPoint(ccp(0.5,0.f));
		cNode->setPosition(ccp(0,0));
		cNode->setStencil(m_showWindow.whiteBoard);
		cNode->setInverted(false);
		this->addChild(cNode,1);
		m_showWindow.explosionNode = CCSpriteBatchNode::create("fx_monster_hit.png");
		
		cNode->addChild(m_showWindow.explosionNode);
		
		return true;
	}
	
	function<void(void)> m_func;
	void setFunctionForCrash(function<void(void)> func){
		this->m_func = func;
	}
	void showWindow(float dt)
	{
		//float r = m_showWindow.rotationRadius;
		//m_missileSprite->setPosition(m_showWindow.initPosition + ccp(cos(m_showWindow.rotationRad) * r, sin(m_showWindow.rotationRad) * r));
		//if(m_selfRotation)
		//{
		//m_missileSprite->setRotation(m_missileSprite->getRotation() + 15);
		//}
		//else
		//{
		//m_missileSprite->setRotation(-rad2Deg(m_showWindow.rotationRad) + 180);
		//}
		
		//if(m_particle)
		//m_particle->setPosition(m_missileSprite->getPosition());
		
		//if(m_streak)
		//m_streak->setPosition(m_missileSprite->getPosition());
		
		m_showWindow.currentTime += 1 / 60.f;
		
		if(m_showWindow.currentTime > m_showWindow.lastCreationTime + 1.0f)
		{
			m_showWindow.lastCreationTime = m_showWindow.currentTime;
			float creationRad = ks19937::getFloatValue(0, 2 * M_PI);
			ShowWindow::MissileSprite missile;
			missile.missileSprite = CCSprite::create(m_showWindow.fileName.c_str());
			
			m_showWindow.clippingNode->addChild(missile.missileSprite);
			if(m_start_node)
				missile.missileSprite->setPosition(m_start_node->getPosition() - getPosition());
			else
				missile.missileSprite->setPosition(ccp(100 * cosf(creationRad), 100 * sinf(creationRad)));
			
			if(missile.streak)
				missile.streak->setPosition(missile.missileSprite->getPosition());
			if(missile.particleQuad)
				missile.particleQuad->setPosition(missile.missileSprite->getPosition());
			
			if(m_start_node)
				missile.missileRad = (m_start_node->getPosition() - getPosition()).getAngle();// creationRad;
			else
				missile.missileRad = creationRad;
			
			int random_value = rand()%7 - 3;
			float random_float = random_value/10.f;
			float speed = 1.4f+random_float + m_showWindow.grade / 10.f;
			missile.missileSpeed = speed;
			m_showWindow.missileSprites.push_back(missile);
		}
		
		for(auto iter = m_showWindow.missileSprites.begin(); iter != m_showWindow.missileSprites.end();)
		{
			ShowWindow::MissileSprite i = *iter;
			i.missileSprite->setPosition(i.missileSprite->getPosition() -
																	 ccp(i.missileSpeed * cosf(i.missileRad), i.missileSpeed * sinf(i.missileRad)));
			if(i.streak)
				i.streak->setPosition(i.missileSprite->getPosition());
			if(i.particleQuad)
				i.particleQuad->setPosition(i.missileSprite->getPosition());
//			if(m_selfRotation)
//			{
//				i.missileSprite->setRotation(i.missileSprite->getRotation() + 15);
//			}
//			else
//			{
//				i.missileSprite->setRotation(-rad2Deg(i.missileRad) + 90);
//			}
			
			if(ccpLength(i.missileSprite->getPosition() - CCPointZero) <= 2.f)
			{
				//AudioEngine::sharedInstance()->playEffect("sound_jack_missile_bomb.mp3",false);
				i.missileSprite->removeFromParent();
				if(i.streak)
					i.streak->removeFromParent();
				if(i.particleQuad)
					i.particleQuad->removeFromParent();
				iter = m_showWindow.missileSprites.erase(iter);
				CCSprite* t_explosion = CCSprite::createWithTexture(m_showWindow.explosionNode->getTexture(), CCRectMake(0, 0, 167, 191));
				t_explosion->setScale(0.65f);
				t_explosion->setRotation(-rad2Deg(i.missileRad)-90 + 180.f);
				m_showWindow.explosionNode->addChild(t_explosion);
				
				CCAnimation* t_animation = CCAnimation::create();
				t_animation->setDelayPerUnit(0.1f);
				t_animation->addSpriteFrameWithTexture(m_showWindow.explosionNode->getTexture(), CCRectMake(0, 0, 167, 191));
				for(int i=0;i<2;i++)
					for(int j=0;j<3;j++)
						t_animation->addSpriteFrameWithTexture(m_showWindow.explosionNode->getTexture(), CCRectMake(j*167, i*191, 167, 191));
				
				this->m_func();
				CCAnimate* t_animate = CCAnimate::create(t_animation);
				CCFadeOut* t_fade = CCFadeOut::create(0.2f);
				CCRemoveSelf* t_remove = CCRemoveSelf::create();
				CCSequence* t_seq = CCSequence::create(t_animate, t_fade, t_remove, NULL);
				t_explosion->runAction(t_seq);
			}
			else
			{
				++iter;
			}
		}
	}
	void update(float dt)
	{
		bool isEnable = true;
		bool emptyMonster = false;
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
		
		
		m_missileSprite->setPosition(m_missileSprite->getPosition() + ccp(cosf(m_initRad), sin(m_initRad)));
		if(m_particle)
			m_particle->setPosition(m_missileSprite->getPosition());
		
		if(m_streak)
			m_streak->setPosition(m_missileSprite->getPosition());
		CCPoint minDis = ccp(60, 60);
		KSCumberBase* nearCumber = nullptr;
		bool found = false;
		for(int i = 0; i<myGD->getMainCumberCount();i++)
		{
			KSCumberBase* cumber = myGD->getMainCumberVector()[i];
			CCPoint nowDis = cumber->getPosition() - m_missileSprite->getPosition();
			if(ccpLength(nowDis)<ccpLength(minDis))
			{
				nearCumber=cumber;
				minDis = nowDis;
				found = true;
				break;
			}
		}
		
		if(found == false)
		{
			for(int i = 0; i<myGD->getSubCumberCount();i++){
				KSCumberBase* cumber = myGD->getSubCumberVector()[i];
				CCPoint nowDis = cumber->getPosition() - m_missileSprite->getPosition();
				if(cumber->getDeadState() == false)
				{
					if(ccpLength(nowDis)<ccpLength(minDis))
					{
						nearCumber=cumber;
						minDis = nowDis;
						found = true;
						break;
					}
				}
			}
		}
		
		if(nearCumber)
		{
			AudioEngine::sharedInstance()->playEffect("se_monattacked.mp3", false);
			
			CCPoint effectPosition = m_missileSprite->getPosition();
			effectPosition.x += rand()%21 - 10;
			effectPosition.y += rand()%21 - 10;
			
			float damage = m_power;
			executeOption(dynamic_cast<KSCumberBase*>(nearCumber), damage, m_subPower, 0.f, effectPosition);
			nearCumber->setSpeedRatioForStone(this, 0.3f);
			nearCumber->setSlowDurationFrame(60 * 3);
//			addChild(KSTimer::create(2.f, ))
			
			
			removeFromParentAndCleanup(true);
			
		}
	}
	
	void beautifier(int level)
	{
		makeBeautifier(level, m_streak, m_particle);
		if(m_streak)addChild(m_streak, -1);
		if(m_particle)addChild(m_particle, -2);
	}
	
	// 반지름 설정
	void setShowWindowRotationRadius(float r)
	{
		
	}
	// 각속도 설정
	void setShowWindowVelocityRad(float r)
	{
		
	}
protected:
	int m_missileStep; // 미사일 단계 : 1 = 캐릭터로 부터 빙글빙글 돌면서 나타나는 과정 2 = 몬스터를 찾는 과정 3 = 날아갈 때.
	float m_initSpeed; // 초기 속도.
	float m_initRad; // 처음에 날아가는 각도.
	
	
	CCNode* m_targetNode;
	CCSprite* m_missileSprite; // 미사일 객체.
	CCParticleSystemQuad* m_particle;
	ASMotionStreak* m_streak;
	CCNode* m_start_node;
	struct ShowWindow
	{
		ShowWindow()
		{
			lastCreationTime = 0.f;
			currentTime = 0.f;
		}
		struct MissileSprite
		{
			MissileSprite()
			{
				streak = nullptr;
				particleQuad = nullptr;
			}
			
			CCSprite* missileSprite;
			ASMotionStreak* streak;
			CCParticleSystemQuad* particleQuad;
			float missileRad;
			float missileSpeed;
		};
		std::vector<MissileSprite> missileSprites;
		float lastCreationTime;
		float currentTime;
		std::string fileName;
		int grade, level;
		CCSprite* whiteBoard;
		CCClippingNode* clippingNode;
		CCSpriteBatchNode* explosionNode;
	}m_showWindow;
	int m_subPower;
	CC_SYNTHESIZE(int, m_power, Power); // 파워.
};