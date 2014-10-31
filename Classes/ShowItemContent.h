//
//  ShowItemContent.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 9..
//
//

#ifndef __DGproto__ShowItemContent__
#define __DGproto__ShowItemContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCMenuLambda.h"
#include "EnumDefine.h"
#include "StarGoldData.h"
#include "KSLabelTTF.h"
#include "SilhouetteData.h"
#include "CommonButton.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ShowItemContent : public CCNode
{
public:
	static ShowItemContent* create(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list);
	void endShowAnimation();
	function<void(function<void()>)> hide_func;
	
private:
	int touch_priority;
	CommonButton* close_menu;
	vector<int> item_list;
	function<void(CCObject*)> end_selector;
	CCSprite* show_content;
	KSLabelTTF* item_title;
	KSLabelTTF* item_ment;
	int ing_close_cnt;
	
	bool is_menu_enable;
	
	string kt_string1, kt_string2;
	
	function<void(CCNode* obj)> addKindTutorial;
	
	
	void menuAction(CCObject* sender);
	
	void startShowAnimation();
	
	void startHideAnimation();
	void endHideAnimation();
	
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list);
	
	string convertToItemCodeToItemName(ITEM_CODE t_code);
};

#endif /* defined(__DGproto__ShowItemContent__) */
