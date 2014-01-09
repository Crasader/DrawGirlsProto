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
#include "CCMenuLambda.h"
#include "StarGoldData.h"

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
	}
	
private:
	int touch_priority;
	CCMenuLambda* close_menu;
	
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector)
	{
		touch_priority = t_touch_priority;
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
		case_back->setPosition(CCPointZero);
		addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(330, 265));
		
		CCSprite *stencil = CCSprite::create("whitePaper.png");
		stencil->setScale(0.65f);
		stencil->setPosition( ccp(0, 0) );
		
		CCClippingNode *clipper = CCClippingNode::create(stencil);
		clipper->setAnchorPoint(ccp(0.5,0.5));
		clipper->setPosition( ccp(0, -15) );
		clipper->setAlphaThreshold(0.1f);
		addChild(clipper);
		
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		CCSprite* control_tutorial = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("tutorial.ccbi",this));
		reader->getAnimationManager()->setDelegate(this);
		control_tutorial->setScale(0.65f);
		control_tutorial->setPosition(ccp(0,0));
		clipper->addChild(control_tutorial);
		
		CCLabelTTF* title_label = CCLabelTTF::create("게임 설명", mySGD->getFont().c_str(), 15);
		title_label->setPosition(ccp(0, 111));
		addChild(title_label);
		
		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, t_selector);
		
		close_menu = CCMenuLambda::createWithItem(close_item);
		close_menu->setVisible(false);
		close_menu->setTouchPriority(touch_priority-1);
		close_menu->setPosition(ccp(140,112));
		addChild(close_menu);
	}
};

#endif /* defined(__DGproto__ControlTutorialContent__) */
