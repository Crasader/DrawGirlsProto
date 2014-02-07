//
//  NewMainFlowScene.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 7..
//
//

#ifndef __DGproto__NewMainFlowScene__
#define __DGproto__NewMainFlowScene__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum NewMainFlowZorder{
	kNewMainFlowZorder_back = 1,
	kNewMainFlowZorder_table,
	kNewMainFlowZorder_top,
	kNewMainFlowZorder_uiButton,
	kNewMainFlowZorder_right,
	kNewMainFlowZorder_popup
};

class CountingBMLabel;
class HeartTime;
class NewStageInfoView;
class NewMainFlowScene : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(NewMainFlowScene);
	
	HeartTime* heart_time;
private:
	
	int is_unlock_puzzle;
	void endUnlockAnimation();
	
	void heartRefresh();
	
	bool is_menu_enable;
	
	CountingBMLabel* ruby_label;
	CountingBMLabel* gold_label;
	CountingBMLabel* friend_point_label;
	
	CCTableView* puzzle_table;
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	vector<int> have_card_count_for_puzzle_index;
	
	NewStageInfoView* new_stage_info_view;
	
	function<void(void)> close_friend_point_action;
	void closeFriendPoint();
	
	void showClearPopup();
	void hideClearPopup();
	void showFailPopup();
	void hideFailPopup();
	
	void setTable();
	
	void menuAction(CCObject* sender);
	
	void countingMessage();
	
	CCSprite* postbox_count_case;
	CCLabelTTF* postbox_count_label;
	
	void countingAchievement();
	
	CCSprite* achievement_count_case;
	CCLabelTTF* achievement_count_label;
	
	void setTop();
	void setBottom();
	
	void popupClose();
	void mailPopupClose();
	void achievePopupClose();
	void tutorialCardSettingClose();
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
};

#endif /* defined(__DGproto__NewMainFlowScene__) */
