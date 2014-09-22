//
//  CardGiftPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 7. 7..
//
//

#ifndef __DGproto__CardGiftPopup__
#define __DGproto__CardGiftPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSProtect.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class KSLabelTTF;
class LoadingLayer;
class CardGiftPopup : public CCNode, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static CardGiftPopup* create(int t_touch_priority, int t_gift_card, function<void()> t_end_func, function<void()> t_close_func);
	virtual ~CardGiftPopup()
	{
		myHSP->removeTarget(this);
	}
private:
	int m_touchPriority;
	function<void()> end_func;
	function<void()> close_func;
	CCSprite* gray;
	CCSprite* main_case;
	CCScale9Sprite* main_inner;
	KSProtectVar<int> gift_card_number;
	bool is_menu_enable;
	
	vector<CommandParam> command_list;
	
	LoadingLayer* t_loading;
	
	KSLabelTTF* result_label;
	CCScale9Sprite* found_back;
	
	
	Json::Value m_friends;
	CCTableView* m_friendTable;
	bool m_failed;
public:
	void myInit(int t_touch_priority, int t_gift_card, function<void()> t_end_func, function<void()> t_close_func);
	void resultSendAction(Json::Value result_data)	;

public: // table
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	
	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);
	
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
};

#endif /* defined(__DGproto__CardGiftPopup__) */
