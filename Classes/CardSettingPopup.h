//
//  CardSettingPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#ifndef __DGproto__CardSettingPopup__
#define __DGproto__CardSettingPopup__

#include "cocos2d.h"
#include "cocos-ext.h"

//#include <deque>
#include <map>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CardListViewer;
class ListViewerScroll;
class IntPoint;
class CardSettingPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CardSettingPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	CCNode* tutorial_node;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* main_case;
	CCSprite* gray;
	
	int server_puzzle_list_count;
	vector<int> server_puzzle_list_no;
	vector<int> server_puzzle_stage_count;
	vector<int> server_puzzle_start_stage;
	
	int recent_selected_card_number;
	
	int recent_sort_type;
	
	map<int, CCPoint> align_default_position_list;
	
	unsigned int default_align_number_of_cell;
	CCTableView* card_table;
	
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	CCPoint getContentPosition(int t_tag);
//	void removeMountingCard();
//	void mountingCard(int card_stage, int card_level);
	
	void alignChange();
//	void addMountedCase();
//	void removeMountedCase();
	
	bool is_menu_enable;
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};

#endif /* defined(__DGproto__CardSettingPopup__) */
