//
//  OnePercentGacha.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 12..
//
//

#ifndef __DGproto__OnePercentGacha__
#define __DGproto__OnePercentGacha__

#include "cocos2d.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "CountingBMLabel.h"

USING_NS_CC;
using namespace std;

enum OnePercentGachaZorder{
	kOnePercentGacha_Z_gray = 1,
	kOnePercentGacha_Z_back,
	kOnePercentGacha_Z_content
};

enum OnePercentGachaMenuTag{
	kOnePercentGacha_MT_cancel = 1,
	kOnePercentGacha_MT_ok,
	kOnePercentGacha_MT_stop
};

class OnePercentGacha : public CCLayer
{
public:
	static OnePercentGacha* create(CCObject* t_cancel, SEL_CallFunc d_cancel, CCObject* t_ok, SEL_CallFuncF d_ok, float t_recent_percent)
	{
		OnePercentGacha* t_tnp = new OnePercentGacha();
		t_tnp->myInit(t_cancel, d_cancel, t_ok, d_ok, t_recent_percent);
		t_tnp->autorelease();
		return t_tnp;
	}
	
private:
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* cancel_menu;
	CCMenu* ok_menu;
	
	CCObject* target_cancel;
	SEL_CallFunc delegate_cancel;
	CCObject* target_ok;
	SEL_CallFuncF delegate_ok;
	
	CCSprite* back_img;
	
	float recent_percent;
	
	void myInit(CCObject* t_cancel, SEL_CallFunc d_cancel, CCObject* t_ok, SEL_CallFuncF d_ok, float t_recent_percent)
	{
		recent_percent = t_recent_percent;
		target_cancel = t_cancel;
		delegate_cancel = d_cancel;
		target_ok = t_ok;
		delegate_ok = d_ok;
		
		back_img = NULL;
		cancel_menu = NULL;
		ok_menu = NULL;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,myDSH->ui_center_y));
		gray->setScaleY(myDSH->ui_top/320.f);
		addChild(gray, kOnePercentGacha_Z_gray);
		
		setBack();
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	void gachaOn()
	{
		CCMoveTo* t_move1 = CCMoveTo::create(0.05f, ccpAdd(ccp(240, myDSH->ui_center_y+25.f), ccp(111,0)));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(OnePercentGacha::repeatAction));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_move1, t_call);
		arrow_img->runAction(t_seq);
	}
	
	void repeatAction()
	{
		is_menu_enable = true;
		ok_menu->setEnabled(true);
		CCMoveTo* t_move2 = CCMoveTo::create(0.1f, ccpAdd(ccp(240, myDSH->ui_center_y+25.f), ccp(-111,0)));
		CCMoveTo* t_move3 = CCMoveTo::create(0.1f, ccpAdd(ccp(240, myDSH->ui_center_y+25.f), ccp(111,0)));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_move3);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
		arrow_img->runAction(t_repeat);
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kOnePercentGacha_MT_ok)
		{
			mySGD->setGold(mySGD->getGold()-500);
			gachaOn();
			ok_menu->removeFromParent();
			
			CCSprite* n_stop = CCSprite::create("one_percent_gacha_stop.png");
			CCSprite* s_stop = CCSprite::create("one_percent_gacha_stop.png");
			s_stop->setColor(ccGRAY);
			
			CCMenuItem* stop_item = CCMenuItemSprite::create(n_stop, s_stop, this, menu_selector(OnePercentGacha::menuAction));
			stop_item->setTag(kOnePercentGacha_MT_stop);
			
			ok_menu = CCMenu::createWithItem(stop_item);
			ok_menu->setEnabled(false);
			ok_menu->setPosition(ccp(240, myDSH->ui_center_y-65.f));
			addChild(ok_menu, kOnePercentGacha_Z_content);
			
			cancel_menu->setEnabled(false);
//			(target_ok->*delegate_ok)(recent_percent);
		}
		else if(tag == kOnePercentGacha_MT_cancel)
		{
			(target_cancel->*delegate_cancel)();
			removeFromParent();
		}
		else if(tag == kOnePercentGacha_MT_stop)
		{
			ok_menu->setEnabled(false);
			arrow_img->stopAllActions();
			float position_value = (arrow_img->getPositionX() - 129.f)/222.f;
			recent_percent += position_value/100.f;
			if(recent_percent > 1.f)
				recent_percent = 1.f;
			
			CCProgressFromTo* t_progress_action = CCProgressFromTo::create(1.f, 0.f, position_value*100.f);
			CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
			CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(OnePercentGacha::changePercentLabel));
			CCDelayTime* t_delay2 = CCDelayTime::create(3.f);
			CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(OnePercentGacha::endAction));
			CCSequence* t_seq = CCSequence::create(t_delay1, t_call1, t_delay2, t_call2, NULL);
			CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_progress_action, t_seq);
			progress_img->runAction(t_spawn);
		}
	}
	
	void endAction()
	{
		(target_ok->*delegate_ok)(recent_percent);
		removeFromParent();
	}
	
	void changePercentLabel()
	{
		percent_label->setString(CCString::createWithFormat("%.1f", recent_percent*100.f)->getCString());
	}
	
	CountingBMLabel* percent_label;
	CCProgressTimer* progress_img;
	CCSprite* arrow_img;
	
	void setBack()
	{
		back_img = CCSprite::create("one_percent_gacha_back.png");
		back_img->setPosition(ccp(240,myDSH->ui_center_y));
		addChild(back_img, kOnePercentGacha_Z_back);
		
		percent_label = CountingBMLabel::create(CCString::createWithFormat("%.1f", recent_percent*100.f)->getCString(), "bb_white_font.fnt", 0.5f, "%.1f");
		percent_label->setPosition(ccp(305, myDSH->ui_center_y+87.f));
		addChild(percent_label, kOnePercentGacha_Z_content);
		
		progress_img = CCProgressTimer::create(CCSprite::create("one_percent_gacha_graph.png"));
		progress_img->setType(kCCProgressTimerTypeBar);
		progress_img->setMidpoint(ccp(0,0));
		progress_img->setBarChangeRate(ccp(1,0));
		progress_img->setPercentage(0.f);
		progress_img->setPosition(ccp(240, myDSH->ui_center_y+10.f));
		addChild(progress_img, kOnePercentGacha_Z_content);
		
		arrow_img = CCSprite::create("one_percent_gacha_arrow.png");
		arrow_img->setPosition(ccp(240, myDSH->ui_center_y+25.f));
		addChild(arrow_img, kOnePercentGacha_Z_content);
		
		CCSprite* n_ok = CCSprite::create("one_percent_gacha_button.png");
		CCSprite* s_ok = CCSprite::create("one_percent_gacha_button.png");
		s_ok->setColor(ccGRAY);
		
		CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(OnePercentGacha::menuAction));
		ok_item->setTag(kOnePercentGacha_MT_ok);
		ok_menu = CCMenu::createWithItem(ok_item);
		ok_menu->setPosition(ccp(240, myDSH->ui_center_y-65.f));
		addChild(ok_menu, kOnePercentGacha_Z_content);
		
		
		CCSprite* n_cancel = CCSprite::create("one_percent_gacha_cancel.png");
		CCSprite* s_cancel = CCSprite::create("one_percent_gacha_cancel.png");
		s_cancel->setColor(ccGRAY);
		
		CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(OnePercentGacha::menuAction));
		cancel_item->setTag(kOnePercentGacha_MT_cancel);
		cancel_menu = CCMenu::createWithItem(cancel_item);
		cancel_menu->setPosition(ccp(385, myDSH->ui_center_y+55.f));
		addChild(cancel_menu, kOnePercentGacha_Z_content);
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(cancel_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kOnePercentGacha_MT_cancel;
		else if(ok_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kOnePercentGacha_MT_ok;
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kOnePercentGacha_MT_cancel)							cancel_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kOnePercentGacha_MT_ok)						ok_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kOnePercentGacha_MT_cancel){			cancel_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kOnePercentGacha_MT_ok){			ok_menu->ccTouchEnded(pTouch, pEvent);			touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kOnePercentGacha_MT_cancel){			cancel_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		else if(touched_number == kOnePercentGacha_MT_ok){			ok_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__OnePercentGacha__) */
