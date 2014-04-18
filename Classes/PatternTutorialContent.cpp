//
//  PatternTutorialContent.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 3..
//
//

#include "PatternTutorialContent.h"
#include "CCMenuLambda.h"
#include "KSUtil.h"

PatternTutorialContent* PatternTutorialContent::create(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_pattern_list)
{
	PatternTutorialContent* t_ctc = new PatternTutorialContent();
	t_ctc->myInit(t_touch_priority, t_selector, t_pattern_list);
	t_ctc->autorelease();
	return t_ctc;
}

void PatternTutorialContent::menuAction(CCObject* sender)
{
	ing_close_cnt++;
	
	if(ing_close_cnt >= pattern_list.size())
	{
		is_menu_enable = false;
		end_selector(NULL);
		
		addChild(KSTimer::create(0.2f, [=](){
			getParent()->removeFromParent();
		}));
		
		addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(1.2f);
			addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(0.f);}));}));
		
		addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(this, t);}, [=](int t){KS::setOpacity(this, 0);}));
	}
	else
	{
		show_content->removeFromParent();
		show_content = CCSprite::create(CCString::createWithFormat("pattern%d_tutorial.png", pattern_list[ing_close_cnt])->getCString());
		show_content->setPosition(ccp(0, -9));
		addChild(show_content);
	}
}

void PatternTutorialContent::myInit(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_pattern_list)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_selector = t_selector;
	
	for(int i=0;i<t_pattern_list.size();i++)
		pattern_list.push_back(t_pattern_list[i]);
	
	CCSprite* case_back = CCSprite::create("pattern_tutorial_back.png");
	case_back->setPosition(CCPointZero);
	addChild(case_back);
	
	ing_close_cnt = 0;
	
	show_content = CCSprite::create(CCString::createWithFormat("pattern%d_tutorial.png", pattern_list[ing_close_cnt])->getCString());
	if(show_content)
	{
		show_content->setPosition(ccp(0, -9));
		addChild(show_content);
	}
	
	CCSprite* n_close = CCSprite::create("whitePaper.png");
	n_close->setOpacity(0);
	CCSprite* s_close = CCSprite::create("whitePaper.png");
	s_close->setOpacity(0);
	
	CCMenuItemSprite* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(PatternTutorialContent::menuAction));
	
	close_menu = CCMenu::createWithItem(close_item);
	close_menu->setTouchPriority(touch_priority-1);
	close_menu->setPosition(ccp(0,0));
	addChild(close_menu);
	
	setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(1.f);
				is_menu_enable = true;}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(this, t);}, [=](int t){KS::setOpacity(this, 255);}));
}