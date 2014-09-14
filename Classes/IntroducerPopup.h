//
//  IntroducerPopup.h
//  DGproto
//
//  Created by ksoo k on 2014. 9. 13..
//
//

#ifndef __DGproto__IntroducerPopup__
#define __DGproto__IntroducerPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LoadingLayer;
class TouchSuctionLayer;
class KSLabelTTF;
class IntroducerPopup : public CCLayer, public CCEditBoxDelegate
{
public:
	static IntroducerPopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_success_func);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> success_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCSprite* gray;
	
	CCNode* m_container;
	
	CCEditBox* input_text1;
	//	CCEditBox* input_text2;
	//	CCEditBox* input_text3;
	
	CCSprite* back_case;
	LoadingLayer* loading_layer;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_success_func);
	
	void couponAction(CCObject* sender, CCControlEvent t_event);
	void resultUserData(Json::Value result_data);
	void resultSaveIntroducer(Json::Value result_data);
	void resultUseCoupon(Json::Value result_data);
	
	void initiateEditBox(CCEditBox* editbox);
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
	
	void createResultPopup(string title, string content, bool is_success = false);
};

#endif /* defined(__DGproto__IntroducerPopup__) */
