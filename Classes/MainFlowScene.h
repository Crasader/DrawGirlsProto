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

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum MainFlowZorder{
	kMainFlowZorder_back = 1,
	kMainFlowZorder_table,
	kMainFlowZorder_top,
	kMainFlowZorder_uiButton,
	kMainFlowZorder_popup
};

class CountingBMLabel;
class HeartTime;
class MainFlowScene : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
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
	
private:
	
//	int is_unlock_puzzle;
//	void endUnlockAnimation();
	
	int keep_card_number;
	
	void updateCardHistory(CCNode* t_loading);
	
	void heartRefresh();
	
	CCNode* tutorial_node;
	
	bool is_menu_enable;
	
	int is_unlock_puzzle;
	
	CountingBMLabel* ruby_label;
	CountingBMLabel* gold_label;
//	CountingBMLabel* friend_point_label;
	
	CCTableView* puzzle_table;
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	vector<int> have_card_count_for_puzzle_index;
	
	function<void(void)> close_friend_point_action;
	void closeFriendPoint();
	
	void showClearPopup();
	void hideClearPopup();
	void showFailPopup();
	void hideFailPopup();
	
	void basicEnter();
	void tableEnter(function<void()> end_func);
	void tableOpenning();
	
	void setTable();
	
	void menuAction(CCObject* sender);
	
	CCSprite* total_star;
	
	void countingMessage();
	
	CCSprite* postbox_count_case;
//	CCLabelTTF* postbox_count_label;
	
	vector<CCNode*> top_list;
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
	void tutorialCardSettingClose();
	
	void endUnlockAnimation();
	
	void puzzleLoadSuccess();
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
};

#endif /* defined(__DGproto__MainFlowScene__) */
