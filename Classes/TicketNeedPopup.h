//
//  TicketNeedPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 15..
//
//

#ifndef __DGproto__TicketNeedPopup__
#define __DGproto__TicketNeedPopup__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum TicketNeedPopupZorder{
	kTNP_Z_gray = 1,
	kTNP_Z_back,
	kTNP_Z_content
};

enum TicketNeedPopupMenuTag{
	kTNP_MT_close = 1,
	kTNP_MT_rightnow,
	kTNP_MT_rightNow_back,
	kTNP_MT_rightNow_buy
};

enum TicketNeedPopupState{
	kTNP_State_default = 0,
	kTNP_State_rightNow
};

class TicketNeedPopup : public CCLayer
{
public:
	static TicketNeedPopup* create(CCObject* t_close, SEL_CallFunc d_close, CCObject* t_pass, SEL_CallFunc d_pass)
	{
		TicketNeedPopup* t_tnp = new TicketNeedPopup();
		t_tnp->myInit(t_close, d_close, t_pass, d_pass);
		t_tnp->autorelease();
		return t_tnp;
	}
	
private:
	TicketNeedPopupState my_state;
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* close_menu;
	CCMenu* rightnow_menu;
	CCMenu* rightNow_back_menu;
	CCMenu* rightNow_buy_menu;
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	CCObject* target_pass;
	SEL_CallFunc delegate_pass;
	
	CCSprite* back_img;              
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close, CCObject* t_pass, SEL_CallFunc d_pass)
	{
		target_close = t_close;
		delegate_close = d_close;
		target_pass = t_pass;
		delegate_pass = d_pass;
		
		back_img = NULL;
		close_menu = NULL;
		rightnow_menu = NULL;
		rightNow_back_menu = NULL;
		rightNow_buy_menu = NULL;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kTNP_Z_gray);
		
		my_state = kTNP_State_default;
		setBack();
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	void setBack()
	{
		if(back_img)
		{
			back_img->removeFromParent();
			back_img = NULL;
		}
		
		if(my_state == kTNP_State_default)
		{
			back_img = CCSprite::create("ticket_back.png");
			back_img->setPosition(ccp(240,160));
			addChild(back_img, kTNP_Z_back);
			
			CCSprite* n_close = CCSprite::create("ui_common_close.png");
			CCSprite* s_close = CCSprite::create("ui_common_close.png");
			s_close->setColor(ccGRAY);
			
			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(TicketNeedPopup::menuAction));
			close_item->setTag(kTNP_MT_close);
			
			close_menu = CCMenu::createWithItem(close_item);
			close_menu->setPosition(getContentPosition(kTNP_MT_close));
			back_img->addChild(close_menu, kTNP_Z_content);
			
			
			CCSprite* n_rightnow = CCSprite::create("ticket_rightnow.png");
			CCSprite* s_rightnow = CCSprite::create("ticket_rightnow.png");
			s_rightnow->setColor(ccGRAY);
			
			CCMenuItem* rightnow_item = CCMenuItemSprite::create(n_rightnow, s_rightnow, this, menu_selector(TicketNeedPopup::menuAction));
			rightnow_item->setTag(kTNP_MT_rightnow);
			
			rightnow_menu = CCMenu::createWithItem(rightnow_item);
			rightnow_menu->setPosition(getContentPosition(kTNP_MT_rightnow));
			back_img->addChild(rightnow_menu, kTNP_Z_content);
		}
		else if(my_state == kTNP_State_rightNow)
		{
			back_img = CCSprite::create("ticket_rightnow_back.png");
			back_img->setPosition(ccp(240,160));
			addChild(back_img, kTNP_Z_back);
			
			CCSprite* n_rightNow_back = CCSprite::create("ui_common_back.png");
			CCSprite* s_rightNow_back = CCSprite::create("ui_common_back.png");
			s_rightNow_back->setColor(ccGRAY);
			
			CCMenuItem* rightNow_back_item = CCMenuItemSprite::create(n_rightNow_back, s_rightNow_back, this, menu_selector(TicketNeedPopup::menuAction));
			rightNow_back_item->setTag(kTNP_MT_rightNow_back);
			
			rightNow_back_menu = CCMenu::createWithItem(rightNow_back_item);
			rightNow_back_menu->setPosition(getContentPosition(kTNP_MT_rightNow_back));
			back_img->addChild(rightNow_back_menu, kTNP_Z_content);
			
			
			CCSprite* n_rightNow_buy = CCSprite::create("ticket_rightnow_buy.png");
			CCSprite* s_rightNow_buy = CCSprite::create("ticket_rightnow_buy.png");
			s_rightNow_buy->setColor(ccGRAY);
			
			CCMenuItem* rightNow_buy_item = CCMenuItemSprite::create(n_rightNow_buy, s_rightNow_buy, this, menu_selector(TicketNeedPopup::menuAction));
			rightNow_buy_item->setTag(kTNP_MT_rightNow_buy);
			
			rightNow_buy_menu = CCMenu::createWithItem(rightNow_buy_item);
			rightNow_buy_menu->setPosition(getContentPosition(kTNP_MT_rightNow_buy));
			back_img->addChild(rightNow_buy_menu, kTNP_Z_content);
		}
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		if(t_tag == kTNP_MT_close)				return_value = ccp(245,275);
		else if(t_tag == kTNP_MT_rightnow)		return_value = ccp(142,60);
		else if(t_tag == kTNP_MT_rightNow_back)	return_value = ccp(245,275);
		else if(t_tag == kTNP_MT_rightNow_buy)	return_value = ccp(142,80);
		
		return return_value;
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kTNP_MT_close)
		{
			(target_close->*delegate_close)();
			removeFromParent();
		}
		else if(tag == kTNP_MT_rightnow)
		{
			my_state = kTNP_State_rightNow;
			setBack();
			
			is_menu_enable = true;
		}
		else if(tag == kTNP_MT_rightNow_back)
		{
			my_state = kTNP_State_default;
			setBack();
			
			is_menu_enable = true;
		}
		else if(tag == kTNP_MT_rightNow_buy)
		{
			(target_pass->*delegate_pass)();
			removeFromParent();
		}
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(my_state == kTNP_State_default)
		{
			if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kTNP_MT_close;
			else if(rightnow_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kTNP_MT_rightnow;
		}
		else if(my_state == kTNP_State_rightNow)
		{
			if(rightNow_back_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kTNP_MT_rightNow_back;
			else if(rightNow_buy_menu->ccTouchBegan(pTouch, pEvent))	touched_number = kTNP_MT_rightNow_buy;
		}
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kTNP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kTNP_MT_rightnow)					rightnow_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kTNP_MT_rightNow_back)			rightNow_back_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kTNP_MT_rightNow_buy)				rightNow_buy_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kTNP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kTNP_MT_rightnow){	rightnow_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		else if(touched_number == kTNP_MT_rightNow_back){	rightNow_back_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		else if(touched_number == kTNP_MT_rightNow_buy){	rightNow_buy_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kTNP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);			touched_number = 0;	}
		else if(touched_number == kTNP_MT_rightnow){	rightnow_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kTNP_MT_rightNow_back){	rightNow_back_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		else if(touched_number == kTNP_MT_rightNow_buy){	rightNow_buy_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__TicketNeedPopup__) */
