//
//  NoticeContent.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 16..
//
//

#ifndef __DGproto__NoticeContent__
#define __DGproto__NoticeContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCMenuLambda.h"
#include "jsoncpp/json.h"
#include "StyledLabelTTF.h"
#include <chrono>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum NoticeContentMenuTag{
	kNoticeContentMenuTag_ok = 1,
	kNoticeContentMenuTag_check
};

class NoticeContent : public CCNode//, public CCBAnimationManagerDelegate
{
public:
	static NoticeContent* create(int t_touch_priority, function<void(CCObject*)> t_selector, Json::Value t_noti_list)
	{
		NoticeContent* t_ctc = new NoticeContent();
		t_ctc->myInit(t_touch_priority, t_selector, t_noti_list);
		t_ctc->autorelease();
		return t_ctc;
	}
	
private:
	int touch_priority;
	//CCMenu* close_menu;
	Json::Value notice_list;
	function<void(CCObject*)> end_selector;
	CCSprite* show_content;
	CCScale9Sprite* case_back;
	CCLabelTTF* title_label;
	CCLabelTTF* content_label;
	StyledLabelTTF* stTxt;
	CCSprite* loading_circle;
	//CCMenu* check_menu;
	//CCSprite* check_img;
	
	bool is_not_show_three_day;
	
	int ing_close_cnt;
	
	bool is_menu_enable;
	
	void loadNotice();
	void menuAction(CCObject* sender);
	
	void loadedAction()
	{
		
		loading_circle->setVisible(false);
		is_menu_enable=true;
		//show_content->setPosition(ccp(-show_content->getContentSize().width/2.f, -show_content->getContentSize().height/2.f-16));
	}
	
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector, Json::Value t_noti_list);
};

#endif /* defined(__DGproto__NoticeContent__) */
