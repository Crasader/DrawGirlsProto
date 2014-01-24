//
//  StartSettingScene.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 18..
//
//

#ifndef __DGproto__StartSettingScene__
#define __DGproto__StartSettingScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <deque>
#include "EnumDefine.h"
#include "hspConnector.h"
#include "KSProtect.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CountingBMLabel;
class HeartTime;
class LoadingLayer;
class StartSettingScene : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StartSettingScene);
	
	HeartTime* heart_time;
private:
	
	bool is_menu_enable;
	
	CCNode* tutorial_node;
	
	CountingBMLabel* ruby_label;
	CountingBMLabel* gold_label;
	CountingBMLabel* friend_point_label;
	
	CCSprite* friend_point_popup;
	void closeFriendPointPopup();
	
	void setMain();
	CCSprite* main_case;
	
	void changeCard();
	CCNode* card_img;
	
	CCMenu* card_turn_menu;
	
	KSProtectVar<int> use_item_price_gold;
	KSProtectVar<int> use_item_price_ruby;
	KSProtectVar<int> use_item_price_social;
	
	deque<bool> is_have_item;
	
	int clicked_item_idx;
	vector<ITEM_CODE> item_list;
	deque<bool> is_selected_item;
	deque<int> card_options;
	vector<int> show_item_popup;
	void itemAction(CCObject* sender);
	
	CCLabelTTF* item_title_label;
	CCLabelTTF* option_label;
	
	void menuAction(CCObject* sender);
	void setTop();
	
	bool was_end_startAction;
	bool was_end_removeMessage;
	LoadingLayer* start_loading;
	void callStart();
	void realStartAction();
	void acceptStartAction();
	void finalSetting();
	void finalRemoveMessage(Json::Value result_data);
	void goToGame();
	void cancelGame();
	void finalAcceptStartAction(Json::Value result_data);
	void finalStartAction(Json::Value result_data);
	
	void durabilityCancelAction(CCObject* sender);
	void durabilityOkAction(CCObject* sender); // remove noti
	
	int getSelectedItemCount();
	
	void popupClose();
	void popupCloseCardSetting();
	void buySuccessItem(int t_clicked_item_idx, int cnt);
	
	string convertToItemCodeToItemName(ITEM_CODE t_code);
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
};

#endif /* defined(__DGproto__StartSettingScene__) */
