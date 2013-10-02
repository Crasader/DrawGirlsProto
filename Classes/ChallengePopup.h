//
//  ChallengePopup.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 1..
//
//

#ifndef __DGproto__ChallengePopup__
#define __DGproto__ChallengePopup__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum ChallengePopupZorder{
	kCP_Z_gray = 1,
	kCP_Z_back,
	kCP_Z_content
};

enum ChallengePopupMenuTag{
	kCP_MT_return = 1
};

class ChallengePopup : public CCLayer
{
public:
	static ChallengePopup* create(CCObject* t_return, SEL_CallFunc d_return)
	{
		ChallengePopup* t_cp = new ChallengePopup();
		t_cp->myInit(t_return, d_return);
		t_cp->autorelease();
		return t_cp;
	}
	
private:
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* return_menu;
	
	CCObject* target_return;
	SEL_CallFunc delegate_return;
	
	void myInit(CCObject* t_return, SEL_CallFunc d_return)
	{
		target_return = t_return;
		delegate_return = d_return;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kCP_Z_gray);
		
		CCSprite* back = CCSprite::create("challenge_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kCP_Z_back);
		
		
		CCSprite* n_return = CCSprite::create("challenge_return.png");
		CCSprite* s_return = CCSprite::create("challenge_return.png");
		s_return->setColor(ccGRAY);
		
		CCMenuItem* return_item = CCMenuItemSprite::create(n_return, s_return, this, menu_selector(ChallengePopup::menuAction));
		return_item->setTag(kCP_MT_return);
		
		return_menu = CCMenu::createWithItem(return_item);
		return_menu->setPosition(getContentPosition(kCP_MT_return));
		addChild(return_menu, kCP_Z_content);
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		if(t_tag == kCP_MT_return)	return_value = ccp(345,283);
		
		return return_value;
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kCP_MT_return)
		{
			(target_return->*delegate_return)();
		}
		
		removeFromParent();
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(return_menu->ccTouchBegan(pTouch, pEvent))	touched_number = kCP_MT_return;
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kCP_MT_return)				return_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kCP_MT_return){			return_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kCP_MT_return){			return_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__ChallengePopup__) */
