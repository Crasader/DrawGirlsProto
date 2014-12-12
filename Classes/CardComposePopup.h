//
//  CardComposePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 12. 11..
//
//

#ifndef __DGproto__CardComposePopup__
#define __DGproto__CardComposePopup__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CardComposePopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static CardComposePopup* create(int t_touch_priority);
	void setHideFinalAction(CCObject* t_target, SEL_CallFunc t_delegate);
	
private:
	int touch_priority;
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	bool is_menu_enable;
	CCSprite* gray;
	CCSprite* main_case;
	CCScale9Sprite* main_inner;
	CCTableView* compose_table;

	void myInit(int t_touch_priority);
	
	void menuAction(CCObject* t_sender);
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
};

#endif /* defined(__DGproto__CardComposePopup__) */
