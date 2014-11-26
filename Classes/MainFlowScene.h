//
//  MainFlowScene.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 16..
//
//

#ifndef __DGproto__MainFlowScene__
#define __DGproto__MainFlowScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <deque>
#include "jsoncpp/json.h"
#include "BackKeyBase.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum MainFlowZorder{
	kMainFlowZorder_back = 1,
	kMainFlowZorder_table,
	kMainFlowZorder_top,
	kMainFlowZorder_uiButton,
	kMainFlowZorder_uiButton2,
	kMainFlowZorder_popup
};

class PuzzleOpenInfo
{
public:
	bool is_open;
	bool is_base_condition_success;
	bool is_have_week_condition;
	int keep_weekday;
	int keep_week_start;
	int keep_week_end;
	bool is_have_date_condition;
	string keep_date_start;
	bool is_have_ruby_condition;
	int need_ruby_value;
	int need_star_count;
	int before_locked_puzzle_count;
	int puzzle_number;
	int need_card_count;
	
	PuzzleOpenInfo()
	{
		is_open = false;
		is_base_condition_success = false;
		is_have_week_condition = false;
		keep_weekday = 0;
		keep_week_start = 0;
		keep_week_end = 0;
		is_have_date_condition = false;
		keep_date_start = "";
		is_have_ruby_condition = false;
		need_ruby_value = 0;
		need_star_count = 0;
		need_card_count = 0;
		before_locked_puzzle_count = 0;
	}
};

class CountingBMLabel;
class HeartTime;
class KSLabelTTF;
class MainFlowScene : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource//, public BackKeyBase
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainFlowScene);
	
	HeartTime* heart_time;
	
	virtual ~MainFlowScene();
	
	void showEndlessOpening();
	void showEndlessResult();
    
	void showHellResult();
	void showHellOpening();
	
    CCNode* showShopPopup(int t_code);
	void showGachaPopup();
    
    virtual void onEnterTransitionDidFinish();
	
private:
	
//	int is_unlock_puzzle;
//	void endUnlockAnimation();
	
	int keep_card_number;
	int locked_puzzle_count;
	
	bool is_not_first;
	
	function<void(function<void()>)> start_unlock_animation;
	
	void updateCardHistory(CCNode* t_loading);
	
	void heartRefresh();
	
	vector<int> not_event_puzzle_list;
	
	CCNode* tutorial_node;
	
	bool is_menu_enable;
	
	int is_unlock_puzzle;
	int is_perfect_puzzle;
	
	CountingBMLabel* ruby_label;
	CountingBMLabel* gold_label;
//	CountingBMLabel* friend_point_label;
	
	int last_open_puzzle_index;
	CCTableView* puzzle_table;
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	vector<int> have_card_count_for_puzzle_index;
	vector<PuzzleOpenInfo> is_puzzle_enter_list;
	
	function<void(void)> close_friend_point_action;
	void closeFriendPoint();
	
	function<void()> kind_tutorial_pvp;
	
	void showClearPopup();
	void hideClearPopup();
	void showFailPopup();
	void hideFailPopup();
	
	void basicEnter();
	void tableEnter(function<void()> end_func);
	void tableDownloading(function<void()> end_func);
	void tableOpenning();
	
	void setTable();
	
	void menuAction(CCObject* sender);
	void detailCondition(CCObject* sender, CCControlEvent t_event);
	
	void loadTempFunc(CCObject* sender);
	
	CCSprite* total_star;
	
	CCSprite* gold_img;
	CCSprite* ruby_img;
	
	void countingMessage();
	void countingAchievement();
	
	CCScale9Sprite* postbox_count_case;
	KSLabelTTF* postbox_count_label;
	CCScale9Sprite* achievement_count_case;
	KSLabelTTF* achievement_count_label;
	
	function<void()> refresh_ing_win_func;
	
	vector<CCNode*> top_list;
	void topOnLight();
	void topOpenning();
	void topOuting();
	void topPuzzleMode();
	void topReturnMode();
	void setTop();
	
	vector<CCNode*> bottom_list;
	void bottomOpenning();
	void bottomPuzzleMode();
	void setBottom();
	
	void popupClose();
	void mailPopupClose();
	void achievePopupClose();
	void friendPopupClose();
	void tutorialCardSettingClose();
	
	void endUnlockAnimation();
	
	void puzzleLoadSuccess();
	
	CCNode* heart_refresh_loading;
	void puzzleHeartRefresh(Json::Value result_data);
	
	void cgpReward(CCObject* sender, CCControlEvent t_event);
	void cgpAllReward(CCObject* sender, CCControlEvent t_event);
	
	bool is_table_openning;
	void callTimeInfo();
	void tableRefresh();
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
};

#endif /* defined(__DGproto__MainFlowScene__) */
