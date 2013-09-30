//
//  StartMapGacha.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 30..
//
//

#ifndef __DGproto__StartMapGacha__
#define __DGproto__StartMapGacha__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum StartMapGachaZorder{
	kSMG_Z_back = 1,
	kSMG_Z_content
};

enum StartMapGachaMenuTag{
	kSMG_MT_gacha = 1
};

class StartMapGacha : public CCLayer
{
public:
	static StartMapGacha* create(CCObject* t_gacha, SEL_CallFunc d_gacha)
	{
		StartMapGacha* t_ep = new StartMapGacha();
		t_ep->myInit(t_gacha, d_gacha);
		t_ep->autorelease();
		return t_ep;
	}
	
private:
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* gacha_menu;
	
	CCObject* target_gacha;
	SEL_CallFunc delegate_gacha;
	
	void myInit(CCObject* t_gacha, SEL_CallFunc d_gacha)
	{
		target_gacha = t_gacha;
		delegate_gacha = d_gacha;
		
		CCSprite* back = CCSprite::create("start_map_gacha_back.png");
		back->setAnchorPoint(ccp(0,0.5));
		back->setPosition(ccp(-190,120));
		addChild(back, kSMG_Z_back);
		
		
		CCSprite* n_gacha = CCSprite::create("start_map_gacha_button.png");
		CCSprite* s_gacha = CCSprite::create("start_map_gacha_button.png");
		s_gacha->setColor(ccGRAY);
		
		CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(StartMapGacha::menuAction));
		gacha_item->setTag(kSMG_MT_gacha);
		
		gacha_menu = CCMenu::createWithItem(gacha_item);
		gacha_menu->setPosition(getContentPosition(kSMG_MT_gacha));
		back->addChild(gacha_menu, kSMG_Z_content);
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
		
		CCMoveTo* t_move1 = CCMoveTo::create(1.f, ccp(0,120));
		CCDelayTime* t_delay = CCDelayTime::create(3.f);
		CCMoveTo* t_move2 = CCMoveTo::create(1.f, ccp(-190,120));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartMapGacha::removeFromParent));
		CCAction* t_seq = CCSequence::create(t_move1, t_delay, t_move2, t_call, NULL);
		
		back->runAction(t_seq);
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		if(t_tag == kSMG_MT_gacha)	return_value = ccp(100,35);
		
		return return_value;
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kSMG_MT_gacha)
		{
			(target_gacha->*delegate_gacha)();
		}
		
		removeFromParent();
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(gacha_menu->ccTouchBegan(pTouch, pEvent))	touched_number = kSMG_MT_gacha;
		return touched_number != 0;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kSMG_MT_gacha)				gacha_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kSMG_MT_gacha){			gacha_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kSMG_MT_gacha){			gacha_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__StartMapGacha__) */
