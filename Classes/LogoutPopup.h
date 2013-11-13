//
//  LogoutPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 29..
//
//

#ifndef __DGproto__LogoutPopup__
#define __DGproto__LogoutPopup__

#include "cocos2d.h"
#include "hspConnector.h"

USING_NS_CC;
using namespace std;

enum LogoutPopupZorder{
	kLP_Z_gray = 1,
	kLP_Z_back,
	kLP_Z_content
};

enum LogoutPopupMenuTag{
	kLP_MT_close = 1,
	kLP_MT_logout
};

class LogoutPopup : public CCLayer
{
public:
	static LogoutPopup* create(CCObject* t_close, SEL_CallFunc d_close)
	{
		LogoutPopup* t_lp = new LogoutPopup();
		t_lp->myInit(t_close, d_close);
		t_lp->autorelease();
		return t_lp;
	}
	
private:
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* close_menu;
	CCMenu* logout_menu;
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close)
	{
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kLP_Z_gray);
		
		CCSprite* back = CCSprite::create("option_logout_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kLP_Z_back);
		
		CCSprite* n_logout = CCSprite::create("option_long_close.png");
		CCSprite* s_logout = CCSprite::create("option_long_close.png");
		s_logout->setColor(ccGRAY);
		
		CCMenuItem* logout_item = CCMenuItemSprite::create(n_logout, s_logout, this, menu_selector(LogoutPopup::menuAction));
		logout_item->setTag(kLP_MT_logout);
		
		logout_menu = CCMenu::createWithItem(logout_item);
		logout_menu->setPosition(getContentPosition(kLP_MT_logout));
		addChild(logout_menu, kLP_Z_content);
		
		
		CCSprite* n_close = CCSprite::create("option_long_close.png");
		CCSprite* s_close = CCSprite::create("option_long_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(LogoutPopup::menuAction));
		close_item->setTag(kLP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kLP_MT_close));
		addChild(close_menu, kLP_Z_content);
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		if(t_tag == kLP_MT_close)	return_value = ccp(241,97);
		else if(t_tag == kLP_MT_logout)	return_value = ccp(241,160);
		
		return return_value;
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kLP_MT_close)
		{
			(target_close->*delegate_close)();
			removeFromParent();
		}
		else if(tag == kLP_MT_logout)
		{
			hspConnector::get()->kLogout(json_selector(this, LogoutPopup::resultLogoutAction));
		}
	}
	
	void resultLogoutAction(Json::Value result_data)
	{
		CCLog("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kLP_MT_close;
		else if(logout_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kLP_MT_logout;
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kLP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kLP_MT_logout)					logout_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kLP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kLP_MT_logout){	logout_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kLP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kLP_MT_logout){	logout_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__LogoutPopup__) */
