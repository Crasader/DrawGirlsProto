//
//  SilhouetteData.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 27..
//
//

#ifndef __galsprototype__SilhouetteData__
#define __galsprototype__SilhouetteData__

#include "cocos2d.h"
#include "GameData.h"
#include "EnumDefine.h"
#include <deque>
#include "ServerDataSave.h"

USING_NS_CC;
using namespace std;

#define mySD SilhouetteData::sharedSilhouetteData()

class SilhouetteData : public CCObject
{
public:
	bool silData[162][217];
	int must_cnt;
	
	static SilhouetteData* sharedSilhouetteData();
	
	void setSilType(int t1);
	
	string getPassiveData();
	
//	void setAnimationLoop(int t_type);
	
	int getSilType();
	
//	int getCardDurability(int stage, int level);
	
//	string getCardOptionScript(int stage, int level);
	
//	void setCardOptions(deque<int>& t_list, int card_number);
	
	CLEAR_CONDITION getClearCondition();
	CLEAR_CONDITION getClearCondition(int t_type); // stage;
	
	float getBossMaxLife();
	
	string getConditionTitle();
	
	string getConditionContent();
	string getConditionContent(int t_type);
	
	int getClearConditionCatchSubCumber();
	int getClearConditionCatchSubCumber(int t_type);
	
	float getClearConditionBigAreaPer();
	float getClearConditionBigAreaPer(int t_type);
	
	int getClearConditionBigAreaCnt();
	int getClearConditionBigAreaCnt(int t_type);
	
	int getClearConditionItemCollect();
	int getClearConditionItemCollect(int t_type);
	
	float getClearConditionPerfectBase();
	float getClearConditionPerfectBase(int t_type);
	
	float getClearConditionPerfectRange();
	float getClearConditionPerfectRange(int t_type);
	
	int getClearConditionTimeLimit();
	int getClearConditionTimeLimit(int t_type);
	
	vector<ITEM_CODE> getStageItemList();
	vector<ITEM_CODE> getStageItemList(int t_type);
	
	string getItemScript(ITEM_CODE t_code);
	
	float getItemPrice(ITEM_CODE t_code);
	string getItemCurrency(ITEM_CODE t_code);
	
	int getSilenceItemOption();
	int getDoubleItemOption();
	
//	int getCardDoubleItemOption(int card_number);
	
	int getLongTimeItemOption();
//	int getCardLongTimeItemOption(int card_number);
	
	int getBossLittleEnergyItemOption();
//	int getCardBossLittleEnergyItemOption(int card_number);
	
	int getSubSmallSizeItemOption();
//	int getCardSubSmallSizeItemOption(int card_number);
	
	int getSmallAreaItemOption();
//	int getCardSmallAreaItemOption(int card_number);
	
	int getWidePerfectItemOption();
//	int getCardWidePerfectItemOption(int card_number);
	
//	bool isAnimationStage();
//	bool isAnimationStage(int t_type);
//	
//	CCSize getAnimationCutSize();
//	CCSize getAnimationCutSize(int t_type);
//	
//	int getAnimationCutLength();
//	int getAnimationCutLength(int t_type);
//	
//	CCPoint getAnimationPosition();
//	CCPoint getAnimationPosition(int t_type);
//	
//	int getAnimationLoopLength();
//	int getAnimationLoopLength(int t_type);
//	
//	int getAnimationLoopPoint(int t_frame);
//	
//	string getScriptString(int level);
//	string getScriptString(int t_type, int level);
	
	void startSetting();
	
	void exchangeSilhouette();
	
	
private:
	int myType;
//	deque<int> animation_frame;
	
	void myInit()
	{
		
	}
};

#endif /* defined(__galsprototype__SilhouetteData__) */
