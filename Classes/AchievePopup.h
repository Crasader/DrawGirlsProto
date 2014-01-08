//
//  AchievePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 1. 4..
//
//

#ifndef __DGproto__AchievePopup__
#define __DGproto__AchievePopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "AchieveData.h"
#include "jsoncpp/json.h"
#include "LoadingLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum AchievePopupListCode{
	kAchievePopupListCode_all = 0,
	kAchievePopupListCode_success,
	kAchievePopupListCode_ing,
	kAchievePopupListCode_reward
};

class AchievePopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(AchievePopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:

	AchievePopupListCode recent_code;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	CCSprite* main_case;
	
	bool is_menu_enable;
	
	CCMenu* all_menu;
	CCMenu* success_menu;
	CCMenu* ing_menu;
	CCMenu* reward_menu;
	
	CCTableView* achieve_table;
	
	vector<AchievementCode> achieve_list;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	void setAchieveTable();
	
	void cellAction(CCObject* sender);
	int keep_tag;
	int keep_value;
	LoadingLayer* loading_layer;
	void resultSaveUserData(Json::Value result_data);
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
	
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};

#endif /* defined(__DGproto__AchievePopup__) */