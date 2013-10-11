//
//  Cumber.h
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 27..
//
//

#ifndef DrawingJack_Cumber_h
#define DrawingJack_Cumber_h

#include "cocos2d.h"
#include "GameData.h"
#include <queue>
#include "MissileParent.h"
#include "SelectedMapData.h"
#include "StarGoldData.h"
#include "AlertEngine.h"
//#include "MainCumber.h"
#include "SubCumber.h"
#include "MapFragment.h"

#include <vector>

#include "Well512.h"

using namespace cocos2d;
using namespace std;
using namespace placeholders;
class CumberParent : public CCNode
{
public:
	static CumberParent* create()
	{
		CumberParent* t_CP = new CumberParent();
		t_CP->myInit();
		t_CP->autorelease();
		return t_CP;
	}
	
	virtual ~CumberParent()
	{
		subCumberArray->release();
	}
	
	void setMainCumberState(int t_cs); // no used
	void onStartGame();
	void onPatternEnd();
	void allStopSchedule(); //  used
	void furyModeOn(); 
	void tickingOn();
	void movingMainCumber();
	void stopMovingMainCumber();
	CCNode* getMainCumberPointer();
	CCArray* getSubCumberArrayPointer();
	void decreaseLifeForSubCumber(CCObject* target, float t_damage, float t_directionAngle);
	void createAllCumberSheild();
	bool startDamageReaction(CCObject* cb, float damage, float angle);
	void subCumberBomb();
	void subCumberReplication();
	void setGameover();
	void startTeleport();
	int getMainCumberSheild();
	void movingSubCumbers();
	void stopSubCumbers();
	void startAutoAttacker();
	void jackCrashDie();
//	void setUI_forEP(CCObject* t_ui, SEL_CallFunc k_ui, SEL_CallFunc c_ui);
//	void mainCumberShowEmotion(int t_e);
	void startDieAnimation();
	void changeMaxSize(float t_p);
	float getNumberFromJsonValue(JsonBox::Value v)
	{
		if(v.getType() == JsonBox::Value::DOUBLE)
			return v.getDouble();
		else if(v.getType() == JsonBox::Value::INTEGER)
			return v.getInt();
		else
			return 0.f;
	}
private:
	struct jrType
	{
		
		int m_jrType;
		
		float m_jrMinSpeed;
		float m_jrStartSpeed;
		float m_jrMaxSpeed;
		
		float m_jrMinScale;
		float m_jrStartScale;
		float m_jrMaxScale;
		
		int m_jrNormalMovement;
		int m_jrDrawMovement;
		int m_jrFuryMovement;
		float m_jrHp;
		jrType(int type, float minv, float startv, float maxv,
			   float mins, float starts, float maxs, int nm, int dr, int fury,
			   float hp)
		{
			m_jrType = type;
			m_jrMinSpeed = minv;
			m_jrStartSpeed = startv;
			m_jrMaxSpeed = maxv;
			
			m_jrMinScale = mins;
			m_jrStartScale = starts;
			m_jrMaxScale = maxs;
			
			m_jrNormalMovement = nm;
			m_jrDrawMovement = dr;
			m_jrFuryMovement = fury;
			m_jrHp = hp;
		}
	};
	vector<jrType> m_juniors; // 생성할 때 참고하려고 저장함.
	
	bool is_die_animationing;
	int die_animation_cnt;
	int die_animation_rate;
	Well512 m_well512;
	
	MissileParent* myMP;
	MapFragmentParent* myMFP;
	
//	MainCumber* mainCumber;
	std::vector<KSCumberBase*> mainCumbers;
//	MainCumberKS* mainCumber;
	
//	EmotionParent* myEP;
	
	CCArray* subCumberArray;
	
	bool isGameover;
	
	
	void realStartDieAnimation();
	void dieAnimation();
	int getSubCumberCount();
	void createSubCumber(IntPoint s_p);
	void initSubCumber();
	void slowItem(float ratio);
	void silenceItem(bool t_b);
	void setCasting(bool t_b);
	void removeSubCumber(CCObject* r_sc);
	void myInit();
};

#endif
