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

enum StartSettingZorder{
	kStartSettingZorder_back = 1,
	kStartSettingZorder_main,
	kStartSettingZorder_top,
	kStartSettingZorder_popup
};

class CountingBMLabel;
class HeartTime;
class LoadingLayer;
//class RankFriendInfo;
class StartSettingScene : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
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
	
//	vector<RankFriendInfo> friend_list;
	CCTableView* rank_table;
	int selected_friend_idx;
	
//	void resultGetStageScoreList(Json::Value result_data);
	
	virtual CCTableViewCell* tableCellAtIndex( CCTableView *table, unsigned int idx );
	
	virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view){}
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table)
	{
		return CCSizeMake(180, 40);
	}
	virtual unsigned int numberOfCellsInTableView(CCTableView *table)
	{
		return 0;// friend_list.size();
	}
	
	
	bool is_menu_enable;
	
	CCNode* tutorial_node;
	
	CCNode* missile_img;
	
	CountingBMLabel* ruby_label;
	CountingBMLabel* gold_label;
	
	void setMain();
	CCSprite* main_case;
	
	void setStageRank();
	
	KSProtectVar<int> use_item_price_gold;
	KSProtectVar<int> use_item_price_ruby;
	
	bool is_before_selected_event_stage;
	
	deque<bool> is_have_item;
	
	int clicked_item_idx;
	vector<ITEM_CODE> item_list;
	deque<bool> is_selected_item;
	deque<int> card_options;
	vector<int> show_item_popup;
	void itemAction(CCObject* sender);
	
	CCSprite* gacha_item;
	CCSprite* gacha_item_cover;
	
	CCLabelTTF* item_title_label;
	CCLabelTTF* option_label;
	
	int gacha_item_frame;
	ITEM_CODE selected_gacha_item;
	void startItemGacha();
	void itemGachaAction();
	void stopItemGacha();
	void endItemGacha();
	void menuAction(CCObject* sender);
	void setTop();
	
	
	CCMenu* upgrade_menu;
	CCLabelTTF* missile_data;
	void upgradeAction(CCObject* sender);
	
	
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
