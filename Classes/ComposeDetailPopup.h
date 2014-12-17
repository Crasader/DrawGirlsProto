//
//  ComposeDetailPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 12. 12..
//
//

#ifndef __DGproto__ComposeDetailPopup__
#define __DGproto__ComposeDetailPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSProtect.h"
#include "jsoncpp/json.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class KSLabelTTF;
class CardSortInfo;
class LoadingLayer;
class ComposeDetailPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static ComposeDetailPopup* create(int t_touch_priority, int t_compose_idx);
	void setHideFinalAction(CCObject* t_target, SEL_CallFunc t_delegate);
	virtual ~ComposeDetailPopup();
	
private:
	int compose_idx;
	bool is_menu_enable;
	int touch_priority;
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	KSProtectVar<int> sum_exp_value;
	KSLabelTTF* sum_exp_label;
	CCSprite* gray;
	CCSprite* main_case;
	bool is_necessary_on;
	bool is_full;
	
	CCScale9Sprite* price_back;
	CCControlButton* compose_button;
	CCSprite* price_icon;
	KSLabelTTF* price_value;
	CCScale9Sprite* take_back;
	
	CCTableView* card_table;
	KSLabelTTF* need_card_count_2_label;
	CCTableView* selected_table;
	
	LoadingLayer* loading_layer;
	
	vector<int> target_card_number_list;
	vector<CardSortInfo*> card_data_list;
	
	void composeAction(CCObject* t_sender, CCControlEvent t_event);
	void myInit(int t_touch_priority, int t_compose_idx);
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
	
	void cardAction(CCObject* t_sender);
	void unselectedAction(CCObject* t_sender);
	void composeOn();
	void resultCompose(Json::Value result_data);
};

#endif /* defined(__DGproto__ComposeDetailPopup__) */
