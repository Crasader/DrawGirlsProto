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

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ShowItemContent : public CCNode
{
public:
	static ShowItemContent* create(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list)
	{
		ShowItemContent* t_ctc = new ShowItemContent();
		t_ctc->myInit(t_touch_priority, t_selector, t_item_list);
		t_ctc->autorelease();
		return t_ctc;
	}
	
private:
	int touch_priority;
	CCMenu* close_menu;
	vector<int> item_list;
	function<void(CCObject*)> end_selector;
	CCSprite* show_content;
	int ing_close_cnt;
	
	bool is_menu_enable;
	
	
	void menuAction(CCObject* sender)
	{
		ing_close_cnt++;
		
		if(ing_close_cnt >= item_list.size())
		{
			is_menu_enable = false;
			end_selector(NULL);
		}
		else
		{
			show_content->removeFromParent();
			show_content = CCSprite::create(CCString::createWithFormat("newitem_item%d.png", item_list[ing_close_cnt])->getCString());
			show_content->setPosition(ccp(0, 0));
			addChild(show_content);
		}
	}
	
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list)
	{
		touch_priority = t_touch_priority;
		end_selector = t_selector;
		
		for(int i=0;i<t_item_list.size();i++)
			item_list.push_back(t_item_list[i]);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
		case_back->setPosition(CCPointZero);
		addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(290, 172));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		content_back->setPosition(ccp(0,0));
		addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(260, 80));
		
		ing_close_cnt = 0;
		
		show_content = CCSprite::create(CCString::createWithFormat("newitem_item%d.png", item_list[ing_close_cnt])->getCString());
		show_content->setPosition(ccp(0, 0));
		addChild(show_content);
		
		CCSprite* title_img = CCSprite::create("newitem_title.png");
		title_img->setPosition(ccp(0, 67));
		addChild(title_img);
		
		CCSprite* bonus_ment_img = CCSprite::create("newitem_bonus_ment.png");
		bonus_ment_img->setPosition(ccp(0,-59));
		addChild(bonus_ment_img);
		
		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItemSprite* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(ShowItemContent::menuAction));
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setTouchPriority(touch_priority-1);
		close_menu->setPosition(ccp(120,68));
		addChild(close_menu);
		
		is_menu_enable = true;
	}
};

#endif /* defined(__DGproto__ShowItemContent__) */
