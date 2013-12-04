//
//  PatternTutorialContent.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 3..
//
//

#ifndef __DGproto__PatternTutorialContent__
#define __DGproto__PatternTutorialContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCMenuLambda.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class PatternTutorialContent : public CCNode//, public CCBAnimationManagerDelegate
{
public:
	static PatternTutorialContent* create(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_pattern_list)
	{
		PatternTutorialContent* t_ctc = new PatternTutorialContent();
		t_ctc->myInit(t_touch_priority, t_selector, t_pattern_list);
		t_ctc->autorelease();
		return t_ctc;
	}
	
//	virtual void completedAnimationSequenceNamed (char const * name)
//	{
//		string t_name = name;
//		
//		if(t_name == "Default Timeline")
//		{
//			close_menu->setVisible(true);
//		}
//	}
	
private:
	int touch_priority;
	CCMenu* close_menu;
	vector<int> pattern_list;
	function<void(CCObject*)> end_selector;
	CCSprite* show_content;
	int ing_close_cnt;
	
	bool is_menu_enable;
	
	
	void menuAction(CCObject* sender)
	{
		ing_close_cnt++;
		
		if(ing_close_cnt >= pattern_list.size())
		{
			is_menu_enable = false;
			end_selector(NULL);
		}
		else
		{
			show_content->removeFromParent();
			show_content = CCSprite::create(CCString::createWithFormat("pattern%d_tutorial.png", pattern_list[ing_close_cnt])->getCString());
			show_content->setPosition(ccp(0, -16));
			addChild(show_content);
		}
	}
	
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_pattern_list)
	{
		touch_priority = t_touch_priority;
		end_selector = t_selector;
		
		for(int i=0;i<t_pattern_list.size();i++)
			pattern_list.push_back(t_pattern_list[i]);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup3_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
		case_back->setPosition(CCPointZero);
		addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(350, 265));
		
		ing_close_cnt = 0;
		
		show_content = CCSprite::create(CCString::createWithFormat("pattern%d_tutorial.png", pattern_list[ing_close_cnt])->getCString());
		show_content->setPosition(ccp(0, -16));
		addChild(show_content);
		
		CCSprite* title_img = CCSprite::create("tutorial_popup_title.png");
		title_img->setPosition(ccp(0, 111));
		addChild(title_img);
		
		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItemSprite* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(PatternTutorialContent::menuAction));
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setTouchPriority(touch_priority-1);
		close_menu->setPosition(ccp(150,112));
		addChild(close_menu);
		
		is_menu_enable = true;
	}
};

#endif /* defined(__DGproto__PatternTutorialContent__) */
