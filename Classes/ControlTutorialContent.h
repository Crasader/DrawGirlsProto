//
//  ControlTutorialContent.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 3..
//
//

#ifndef __DGproto__ControlTutorialContent__
#define __DGproto__ControlTutorialContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "CCMenuLambda.h"
#include "AudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ControlTutorialContent : public CCNode, public CCBAnimationManagerDelegate
{
public:
	static ControlTutorialContent* create(int t_touch_priority, function<void(CCObject*)> t_selector)
	{
		ControlTutorialContent* t_ctc = new ControlTutorialContent();
		t_ctc->myInit(t_touch_priority, t_selector);
		t_ctc->autorelease();
		return t_ctc;
	}
	
	virtual void completedAnimationSequenceNamed (char const * name)
	{
		string t_name = name;
		
		if(t_name == "Default Timeline")
		{
			close_menu->setVisible(true);
		}
		else if(t_name == "tutorial1")
		{
			click_animaition->setVisible(true);
			state_number = 1;
		}
		else if(t_name == "tutorial2")
		{
			click_animaition->setVisible(true);
			state_number = 3;
		}
		else if(t_name == "tutorial3")
		{
			click_animaition->setVisible(true);
			state_number = 5;
		}
		else if(t_name == "tutorial4")
		{
			click_animaition->setVisible(true);
			state_number = 7;
//			close_menu->setVisible(true);
//			touch_menu->setVisible(false);
		}
	}
	
	virtual ~ControlTutorialContent()
	{
		ani_manager->setDelegate(NULL);
//		ani_manager->release();
	}
	
private:
	int touch_priority;
	CCMenuLambda* close_menu;
	CCMenu* touch_menu;
	
	CCBAnimationManager* ani_manager;
	CCSprite* click_animaition;
	
	function<void(CCObject*)> end_selector;
	
	int state_number;
	
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector)
	{
		state_number = 0;
		end_selector = t_selector;
		touch_priority = t_touch_priority;
//		CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
//		case_back->setPosition(CCPointZero);
//		addChild(case_back);
//		
//		case_back->setContentSize(CCSizeMake(330, 265));
//		
//		CCSprite *stencil = CCSprite::create("whitePaper.png");
//		stencil->setScale(0.65f);
//		stencil->setPosition( ccp(0, 0) );
//		
//		CCClippingNode *clipper = CCClippingNode::create(stencil);
//		clipper->setAnchorPoint(ccp(0.5,0.5));
//		clipper->setPosition( ccp(0, -15) );
//		clipper->setAlphaThreshold(0.1f);
//		addChild(clipper);
		
//		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//		CCBReader* reader = new CCBReader(nodeLoader);
//		CCSprite* control_tutorial = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("tutorial.ccbi",this));
		auto t_ccb = KS::loadCCBI<CCSprite*>(this, "tutorial.ccbi");
		CCSprite* control_tutorial = t_ccb.first;
		ani_manager = t_ccb.second;
		ani_manager->setDelegate(this);
//		control_tutorial->setScale(0.65f);
		control_tutorial->setPosition(ccp(0,0));
//		clipper->addChild(control_tutorial);
		addChild(control_tutorial);
		
//		CCLabelTTF* title_label = CCLabelTTF::create("게임 설명", mySGD->getFont().c_str(), 15);
//		title_label->setPosition(ccp(0, 111));
//		addChild(title_label);
		
//		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
//		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
//		s_close->setColor(ccGRAY);
//		
//		CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, t_selector);
//		
//		close_menu = CCMenuLambda::createWithItem(close_item);
//		close_menu->setVisible(false);
//		close_menu->setTouchPriority(touch_priority-1);
//		close_menu->setPosition(ccp(140,112));
//		addChild(close_menu);
		
		click_animaition = KS::loadCCBI<CCSprite*>(this, "tutorial_nextbutton.ccbi").first;
		click_animaition->setPosition(ccp(180,-100));
		addChild(click_animaition);
		click_animaition->setVisible(false);
		
		CCSprite* n_touch = CCSprite::create("whitePaper.png");
		n_touch->setOpacity(0);
		CCSprite* s_touch = CCSprite::create("whitePaper.png");
		s_touch->setOpacity(0);
		
		CCMenuItem* t_touch_item = CCMenuItemSprite::create(n_touch, s_touch, this, menu_selector(ControlTutorialContent::menuAction));
		touch_menu = CCMenu::createWithItem(t_touch_item);
		touch_menu->setPosition(CCPointZero);
		addChild(touch_menu);
		touch_menu->setTouchPriority(touch_priority-2);
	}
	
	void menuAction(CCObject* sender)
	{
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		
		if(state_number == 1)
		{
			click_animaition->setVisible(false);
			state_number = 2;
			ani_manager->runAnimationsForSequenceNamed("tutorial2");
		}
		else if(state_number == 3)
		{
			click_animaition->setVisible(false);
			state_number = 4;
			ani_manager->runAnimationsForSequenceNamed("tutorial3");
		}
		else if(state_number == 5)
		{
			click_animaition->setVisible(false);
			state_number = 6;
			ani_manager->runAnimationsForSequenceNamed("tutorial4");
		}
		else if(state_number == 7)
		{
			click_animaition->setVisible(false);
			touch_menu->setVisible(false);
			ani_manager->setDelegate(NULL);
			end_selector(NULL);
		}
	}
};

#endif /* defined(__DGproto__ControlTutorialContent__) */
