//
//  HellModeOpening.h
//  DGproto
//
//  Created by 사원3 on 2014. 9. 20..
//
//

#ifndef __DGproto__HellModeOpening__
#define __DGproto__HellModeOpening__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

//#include "CommonButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class KSLabelTTF;
class LoadingLayer;
class CommonButton;
class StyledLabelTTF;
class CCMenuLambda;
class CCMenuItemLambda;
class HellInfo
{
public:
	bool is_open;
	int stage_number;
	int condition_stage;
	string title;
	string content;
	string character_name;
	bool is_take;
};
class HellModeOpening : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	virtual ~HellModeOpening()
	{
		myHSP->removeTarget(this);
	}
	virtual bool init();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	CREATE_FUNC(HellModeOpening);
	
private:
	
	int touch_priority;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	TouchSuctionLayer* suction;
	
	bool is_menu_enable;
	
	CCSprite* gray;
	CCSprite* loading_right_circle;
	CCSprite* main_case;
	
	CCNode* right_info_node;
	
	CommonButton* ready_button;
	
	vector<HellInfo> hell_list;
	
	CCTableView* hell_table;
	LoadingLayer* ready_loading;
	
	int selected_stage;
	int called_stage;
	Json::Value saved_stage_rank;
	int clicked_stage;
	
	void resultGetRank(Json::Value result_data);
	
	void setMain();
	void setRight(int t_stage_number);
	
	void menuAction(CCObject* sender);
	
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
		return cellSizeForTable(table);
	};
	virtual CCSize cellSizeForTable(CCTableView *table) {
		return CCSizeMake(265, 52);
	};
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(CCScrollView* view){
	}
	virtual void scrollViewDidZoom(CCScrollView* view){}
};

#endif /* defined(__DGproto__HellModeOpening__) */
