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
class CardGiftPopup : public CCNode, public CCEditBoxDelegate
{
public:
	static CardGiftPopup* create(int t_touch_priority, int t_gift_card, function<void()> t_end_func, function<void()> t_close_func);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> close_func;
	CCSprite* gray;
	CCScale9Sprite* main_case;
	CCScale9Sprite* main_inner;
	KSProtectVar<int> gift_card_number;
	bool is_menu_enable;
	
	vector<CommandParam> command_list;
	
	LoadingLayer* t_loading;
	
	CCEditBox* input_text;
	KSLabelTTF* result_label;
	CCScale9Sprite* found_back;
	
	void resultSendAction(Json::Value result_data);
	
	void myInit(int t_touch_priority, int t_gift_card, function<void()> t_end_func, function<void()> t_close_func);
	
	void resultGetUserData(Json::Value result_data);
	
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
};

#endif /* defined(__DGproto__CardGiftPopup__) */
