//
//  ContinuePopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 10..
//
//

#ifndef __DGproto__ContinuePopup__
#define __DGproto__ContinuePopup__

#include "cocos2d.h"
#include "StarGoldData.h"

USING_NS_CC;

enum CPL_MenuTag{
	kCPL_MT_end = 1,
	kCPL_MT_continue
};

enum CPL_Zorder{
	kCPL_Z_back = 1,
	kCPL_Z_menu
};

class ContinuePopup : public CCLayer
{
public:
	static ContinuePopup* create(CCObject* t_end, SEL_CallFunc d_end, CCObject* t_continue, SEL_CallFunc d_continue)
	{
		ContinuePopup* t_cpl = new ContinuePopup();
		t_cpl->myInit(t_end, d_end, t_continue, d_continue);
		t_cpl->autorelease();
		return t_cpl;
	}
	
private:
	bool is_menu_enable;
	
	CCObject* target_end;
	SEL_CallFunc delegate_end;
	CCObject* target_continue;
	SEL_CallFunc delegate_continue;
	
	int touched_number;
	CCMenu* end_menu;
	CCMenu* continue_menu;
	
	void myInit(CCObject* t_end, SEL_CallFunc d_end, CCObject* t_continue, SEL_CallFunc d_continue)
	{
		target_end = t_end;
		delegate_end = d_end;
		target_continue = t_continue;
		delegate_continue = d_continue;
		
		CCSprite* cpl_back = CCSprite::create("cpl_back.png");
		cpl_back->setPosition(ccp(240,myDSH->ui_center_y));
		addChild(cpl_back, kCPL_Z_back);
		
		CCSprite* n_end = CCSprite::create("cpl_end.png");
		CCSprite* s_end = CCSprite::create("cpl_end.png");
		s_end->setColor(ccGRAY);
		
		CCMenuItem* end_item = CCMenuItemSprite::create(n_end, s_end, this, menu_selector(ContinuePopup::menuAction));
		end_item->setTag(kCPL_MT_end);
		
		end_menu = CCMenu::createWithItem(end_item);
		end_menu->setPosition(ccp(158,myDSH->ui_center_y-60));
		addChild(end_menu, kCPL_Z_menu);
		
		CCSprite* continue_img = CCSprite::create("cpl_continue.png");
		continue_img->setColor(ccc3(100, 100, 100));
		addChild(continue_img, kCPL_Z_menu);
		
		CCSprite* n_continue = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		n_continue->setOpacity(0);
		CCSprite* s_continue = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		s_continue->setOpacity(0);
//		s_continue->setColor(ccGRAY);
		
		CCMenuItem* continue_item = CCMenuItemSprite::create(n_continue, s_continue, this, menu_selector(ContinuePopup::menuAction));
		continue_item->setTag(kCPL_MT_continue);
		
		continue_menu = CCMenu::createWithItem(continue_item);
		continue_menu->setPosition(ccp(15,myDSH->ui_top-15));
		addChild(continue_menu, kCPL_Z_menu);
		
		is_menu_enable = true;
		
		setTouchEnabled(true);
		
		AudioEngine::sharedInstance()->setAppBack();
		mySGD->is_paused = true;
		CCDirector::sharedDirector()->pause();
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kCPL_MT_end)
		{
			(target_end->*delegate_end)();
		}
		else if(tag == kCPL_MT_continue)
		{
			(target_continue->*delegate_continue)();
		}
		touched_number = -1;
		mySGD->is_paused = false;
		AudioEngine::sharedInstance()->setAppFore();
		CCDirector::sharedDirector()->resume();
		removeFromParent();
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		touched_number = 0;
		if(end_menu->ccTouchBegan(pTouch, pEvent))					touched_number = kCPL_MT_end;
		else if(continue_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kCPL_MT_continue;
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kCPL_MT_end)							end_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kCPL_MT_continue)					continue_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kCPL_MT_end)							end_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kCPL_MT_continue)					continue_menu->ccTouchEnded(pTouch, pEvent);
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kCPL_MT_end)							end_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kCPL_MT_continue)					continue_menu->ccTouchEnded(pTouch, pEvent);
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -150, true);
	}
};

#endif /* defined(__DGproto__ContinuePopup__) */
