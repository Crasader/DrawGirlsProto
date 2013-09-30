//
//  GachaPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 30..
//
//

#ifndef __DGproto__GachaPopup__
#define __DGproto__GachaPopup__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum GachaPopupZorder{
	kGP_Z_gray = 1,
	kGP_Z_back,
	kGP_Z_content
};

enum GachaPopupMenuTag{
	kGP_MT_close = 1,
	kGP_MT_action,
	kGP_MT_result_close
};

class GachaPopup : public CCLayer
{
public:
	static GachaPopup* create(CCObject* t_close, SEL_CallFunc d_close)
	{
		GachaPopup* t_gp = new GachaPopup();
		t_gp->myInit(t_close, d_close);
		t_gp->autorelease();
		return t_gp;
	}
	
private:
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* close_menu;
	CCMenu* button_menu;
	CCMenu* result_close_menu;
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	
	bool is_roulette;
	
	CCSprite* back;
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close)
	{
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kGP_Z_gray);
		
		back = CCSprite::create("gacha_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kGP_Z_back);
		
		
		CCSprite* n_button = CCSprite::create("gacha_action.png");
		CCSprite* s_button = CCSprite::create("gacha_action.png");
		s_button->setColor(ccGRAY);
		
		CCMenuItem* button_item = CCMenuItemSprite::create(n_button, s_button, this, menu_selector(GachaPopup::menuAction));
		button_item->setTag(kGP_MT_action);
		
		button_menu = CCMenu::createWithItem(button_item);
		button_menu->setPosition(getContentPosition(kGP_MT_action));
		back->addChild(button_menu);
		
		CCSprite* n_close = CCSprite::create("ui_common_close.png");
		CCSprite* s_close = CCSprite::create("ui_common_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(GachaPopup::menuAction));
		close_item->setTag(kGP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kGP_MT_close));
		back->addChild(close_menu);
		
		is_roulette = true;
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		if(t_tag == kGP_MT_action)		return_value = ccp(240,47);
		else if(t_tag == kGP_MT_close)	return_value = ccp(320,278);
		else if(t_tag == kGP_MT_result_close)	return_value = ccp(320,278);
		
		return return_value;
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kGP_MT_close)
		{
			(target_close->*delegate_close)();
			removeFromParent();
		}
		else if(tag == kGP_MT_action)
		{
			touched_number = -1;
			back->removeFromParent();
			
			back = CCSprite::create("gacha_result_back.png");
			back->setPosition(ccp(240,160));
			addChild(back, kGP_Z_back);
			
			CCSprite* n_result_close = CCSprite::create("ui_common_close.png");
			CCSprite* s_result_close = CCSprite::create("ui_common_close.png");
			s_result_close->setColor(ccGRAY);
			
			CCMenuItem* result_close_item = CCMenuItemSprite::create(n_result_close, s_result_close, this, menu_selector(GachaPopup::menuAction));
			result_close_item->setTag(kGP_MT_result_close);
			
			result_close_menu = CCMenu::createWithItem(result_close_item);
			result_close_menu->setPosition(getContentPosition(kGP_MT_result_close));
			back->addChild(result_close_menu);
			
			is_roulette = false;
			touched_number = 0;
			is_menu_enable = true;
		}
		else if(tag == kGP_MT_result_close)
		{
			touched_number = -1;
			back->removeFromParent();
			
			back = CCSprite::create("gacha_back.png");
			back->setPosition(ccp(240,160));
			addChild(back, kGP_Z_back);
			
			
			CCSprite* n_button = CCSprite::create("gacha_action.png");
			CCSprite* s_button = CCSprite::create("gacha_action.png");
			s_button->setColor(ccGRAY);
			
			CCMenuItem* button_item = CCMenuItemSprite::create(n_button, s_button, this, menu_selector(GachaPopup::menuAction));
			button_item->setTag(kGP_MT_action);
			
			button_menu = CCMenu::createWithItem(button_item);
			button_menu->setPosition(getContentPosition(kGP_MT_action));
			back->addChild(button_menu);
			
			CCSprite* n_close = CCSprite::create("ui_common_close.png");
			CCSprite* s_close = CCSprite::create("ui_common_close.png");
			s_close->setColor(ccGRAY);
			
			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(GachaPopup::menuAction));
			close_item->setTag(kGP_MT_close);
			
			close_menu = CCMenu::createWithItem(close_item);
			close_menu->setPosition(getContentPosition(kGP_MT_close));
			back->addChild(close_menu);
			
			is_roulette = true;
			touched_number = 0;
			is_menu_enable = true;
		}
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(is_roulette)
		{
			if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kGP_MT_close;
			else if(button_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kGP_MT_action;
		}
		else
		{
			if(result_close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kGP_MT_result_close;
		}
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(is_roulette)
		{
			if(touched_number == kGP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
			else if(touched_number == kGP_MT_action)					button_menu->ccTouchMoved(pTouch, pEvent);
		}
		else
		{
			if(touched_number == kGP_MT_result_close)					result_close_menu->ccTouchMoved(pTouch, pEvent);
		}
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(is_roulette)
		{
			if(touched_number == kGP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
			else if(touched_number == kGP_MT_action){	button_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		}
		else
		{
			if(touched_number == kGP_MT_result_close){	result_close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(is_roulette)
		{
			if(touched_number == kGP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
			else if(touched_number == kGP_MT_action){	button_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		}
		else
		{
			if(touched_number == kGP_MT_result_close){	result_close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__GachaPopup__) */
