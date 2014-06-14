//
//  AttendancePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 4..
//
//

#ifndef __DGproto__AttendancePopup__
#define __DGproto__AttendancePopup__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class AttendancePopup : public CCLayer//, public CCTableViewDataSource
{
public:
	static AttendancePopup* create(int t_touch_priority, function<void()> t_end_func);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	int dayCount;
//	bool is_reloaded;
	
	TouchSuctionLayer* suction;
	
	CCSprite* gray;
	CCNode* m_container;
	CCScale9Sprite* back_case;
//	CCControlButton* close_button;
	
	
//	CCTableView* day_table;
//	CCTableView* reward_table;
	
	void closeAction();
	
	string getGoodsTypeLocalData(string t_type);
	
	CCNode* getGoodsNode(string t_type, CCPoint base_position);
	CCNode* getGoodsSmallNodeAndCount(string t_type, int t_count);
	
	void myInit(int t_touch_priority, function<void()> t_end_func);
	void closeAction(CCObject* sender, CCControlEvent t_event);
	
//	virtual CCSize cellSizeForTable(CCTableView *table) {
//		if(table == reward_table)
//			return CCSizeMake(420, 65);
//		else if(table == day_table)
//			return CCSizeMake(60, 75);
//    };
//    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
//    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
};

#endif /* defined(__DGproto__AttendancePopup__) */
