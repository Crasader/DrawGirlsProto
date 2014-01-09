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
private:
	CCNode* tutorial_node;
	
	bool is_menu_enable;
	
	CountingBMLabel* ruby_label;
	CountingBMLabel* gold_label;
	CountingBMLabel* friend_point_label;
	
	CCSprite* friend_point_popup;
	void closeFriendPointPopup();
	
	CCTableView* puzzle_table;
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void showClearPopup();
	void hideClearPopup();
	void showFailPopup();
	void hideFailPopup();
	
	void setTable();
	
	void menuAction(CCObject* sender);
	void setTop();
	void setBottom();
	
	void popupClose();
	void tutorialCardSettingClose();
	
	void puzzleLoadSuccess();
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
};

#endif /* defined(__DGproto__MainFlowScene__) */
