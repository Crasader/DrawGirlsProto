//
//  CharacterStrengthPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 12. 5..
//
//

#ifndef __DGproto__CharacterStrengthPopup__
#define __DGproto__CharacterStrengthPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSProtect.h"
#include "jsoncpp/json.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class KSLabelTTF;
class ConvexGraph;
class CardSortInfo;
class CCMenuLambda;
class CCMenuItemSpriteLambda;
class LoadingLayer;
class CharacterHistory;
class CharacterStrengthPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static CharacterStrengthPopup* create(int t_touch_priority, int t_character_idx, function<void()> t_end_func);
	virtual ~CharacterStrengthPopup();
	
private:
	bool is_max;
	bool is_full;
	int character_idx;
	bool is_menu_enable;
	int touch_priority;
	function<void()> end_func;
	
	KSProtectVar<int> up_exp_value;
	CCSprite* gray;
	CCSprite* main_case;
	
	int before_exp;
	int after_exp;
	int before_level;
	int after_level;
	
	float before_percentage;
	
	CCSprite* character_img;
	KSLabelTTF* level_value;
	ConvexGraph* exp_graph;
	KSLabelTTF* exp_value;
	KSLabelTTF* maybe_exp_value;
	CCScale9Sprite* price_back;
	CCControlButton* strength_button;
	CCSprite* price_icon;
	KSLabelTTF* price_value;
	CCMenuLambda* target_menu;
	KSLabelTTF* need_card_count_2_label;
	
	CCTableView* card_table;
	
	vector<CCMenuItemSpriteLambda*> target_item_list;
	vector<int> target_card_number_list;
	vector<CardSortInfo*> card_data_list;
	
	LoadingLayer* loading_layer;
	void resultStrength(Json::Value result_data);
	
	void strengthAction(CCObject* t_sender, CCControlEvent t_event);
	CharacterHistory* t_after_history;
	void expUpAction();
	void myInit(int t_touch_priority, int t_character_idx, function<void()> t_end_func);
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
	
	void cardAction(CCObject* t_sender);
};

#endif /* defined(__DGproto__CharacterStrengthPopup__) */
