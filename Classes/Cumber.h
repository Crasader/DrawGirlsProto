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
#include "StarGoldData.h"
#include "AlertEngine.h"
//#include "MainCumber.h"
#include "MapFragment.h"
#include "MobHpGraph.h"

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
		CCLOG("~CumberParent");
		
		for(int i=0;i<mainCumbers.size();i++)
		{
			CCLOG("boss %d retain count : %d", i, mainCumbers[i]->retainCount());
			while(mainCumbers[i]->retainCount() > 1)
			{
				mainCumbers[i]->release();
			}
		}
	}
	
	void setMainCumberState(int t_cs); // no used
	void onJackDrawLine();
	void onStartGame();
	void onPatternEnd();
	void onPatternEndOf(CCObject* cb);
	void allStopSchedule(); //  used
	void tickingOn();
	void movingMainCumber();
	void stopMovingMainCumber();
//	CCNode* getMainCumberPointer();
	std::vector<KSCumberBase*>& getSubCumberArrayPointer();
	std::vector<KSCumberBase*>& getMainCumbers();


	void subCumberReplication();
	void setGameover();
	void startTeleport(CCObject* cumber);
	int getMainCumberSheild();


	void startAutoAttacker();
	void jackCrashDie();
//	void setUI_forEP(CCObject* t_ui, SEL_CallFunc k_ui, SEL_CallFunc c_ui);
//	void mainCumberShowEmotion(int t_e);
	void startDieAnimation();
	void changeMaxSize(float t_p);
	void onJackDie();
	void onJackRevived();
	bool startDamageReaction(CCObject* cb, float damage, float angle, bool castCancel, bool stiffen);
	void changePassiveData(const std::string& passive_data);
	void hideBosses();
//	float getNumberFromJsonValue(JsonBox::Value v)
//	{
//		if(v.getType() == JsonBox::Value::DOUBLE)
//			return v.getDouble();
//		else if(v.getType() == JsonBox::Value::INTEGER)
//			return v.getInt();
//		else
//			return 0.f;
//	}
	
	void cumberCcbDelegateNull();
	
private:
	
	bool is_die_animationing;
	int die_animation_cnt;
	int die_animation_rate;
	Well512 m_well512;
	
	MissileParent* myMP;
	MapFragmentParent* myMFP;
	
	std::vector<std::function<void()>> cumber_ccb_delegate_null_list;
	
//	MainCumber* mainCumber;
	std::vector<KSCumberBase*> mainCumbers;
//	MainCumberKS* mainCumber;
//	EmotionParent* myEP;
	std::vector<KSCumberBase*> subCumberArray;
	vector<MobHpGraph*> hp_graphs;
	
	bool isGameover;
	
	KSProtectVar<float> total_damage_to_gold;
	
	void realStartDieAnimation();
	void dieAnimation();
	int getSubCumberCount();
	void createSubCumber(IntPoint s_p);
	void initSubCumber();
	void slowItem(float ratio);
	void silenceItem(bool t_b);
	void setCasting(bool t_b);
	void removeSubCumber(CCObject* r_sc);
	void removeMainCumber(CCObject* r_sc);
	void myInit();
	void mappingFunctor();
	void toFun();
	void toScratch();
};

#endif
