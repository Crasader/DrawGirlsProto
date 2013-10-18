//
//  PausePopupLayer.h
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 7..
//
//

#ifndef __DrawingJack__PausePopupLayer__
#define __DrawingJack__PausePopupLayer__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "GameData.h"
#include "AudioEngine.h"

using namespace cocos2d;
using namespace std;

enum MenuTagPPL{
	kMenuTagPPL_home = 1,
	kMenuTagPPL_continue,
	kMenuTagPPL_slow,
	kMenuTagPPL_normal,
	kMenuTagPPL_fast
};

class PausePopupLayer : public CCLayer
{
public:
	static PausePopupLayer* create(CCObject* t_home, SEL_CallFunc d_home, CCObject* t_continue, SEL_CallFunc d_continue, CCObject* t_control, SEL_CallFunc d_gesture, SEL_CallFunc d_button, SEL_CallFunc d_joystick);
	void removeSelf();
	
private:
	bool is_ing;
	
	CCMenu* home_menu;
	CCMenu* continue_menu;
//	CCMenu* gesture_menu;
//	CCMenu* button_menu;
//	CCMenu* joystick_menu;
	CCMenu* slow_menu;
	CCMenu* normal_menu;
	CCMenu* fast_menu;
	
	int touched_number;
	
	
	
	bool is_action;
	
	CCSprite* main_case;
	
	CCObject* target_home;
	SEL_CallFunc delegate_home;
	CCObject* target_continue;
	SEL_CallFunc delegate_continue;
	CCObject* target_control;
//	SEL_CallFunc delegate_gesture;
//	SEL_CallFunc delegate_button;
//	SEL_CallFunc delegate_joystick;
	SEL_CallFunc delegate_slow;
	SEL_CallFunc delegate_normal;
	SEL_CallFunc delegate_fast;
	
	void startPopupAnimation()
	{
		is_ing = true;
		
		main_case->setScale(0.1);
		
		CCScaleTo* t_scale = CCScaleTo::create(0.2f, 1.f);
		CCMoveTo* t_move = CCMoveTo::create(0.2f, ccp(240,myDSH->ui_center_y));
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scale, t_move);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PausePopupLayer::endPopAnimation));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
		
		main_case->runAction(t_seq);
	}
	
	void startPopdownAnimation()
	{
		is_ing = true;
		CCScaleTo* t_scale = CCScaleTo::create(0.2f, 0.1f);
		float x_value;
		if(myGD->gamescreen_type == kGT_rightUI)		x_value = 480-25;
		else								x_value = 25;
		CCMoveTo* t_move = CCMoveTo::create(0.2f, ccp(x_value,myDSH->ui_top-25));
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scale, t_move);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PausePopupLayer::removeSelf));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
		
		main_case->runAction(t_seq);
	}
	
	void endPopAnimation()
	{
		is_ing = false;
		AudioEngine::sharedInstance()->setAppBack();
		CCDirector::sharedDirector()->pause();
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		touched_number = 0;
		if(home_menu->ccTouchBegan(pTouch, pEvent))					touched_number = kMenuTagPPL_home;
		else if(continue_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kMenuTagPPL_continue;
		else if(slow_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kMenuTagPPL_slow;
		else if(normal_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kMenuTagPPL_normal;
		else if(fast_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kMenuTagPPL_fast;
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kMenuTagPPL_home)						home_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_continue)				continue_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_slow)					slow_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_normal)				normal_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_fast)					fast_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kMenuTagPPL_home)						home_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_continue)				continue_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_slow)					slow_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_normal)				normal_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_fast)					fast_menu->ccTouchEnded(pTouch, pEvent);
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kMenuTagPPL_home)						home_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_continue)				continue_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_slow)					slow_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_normal)				normal_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kMenuTagPPL_fast)					fast_menu->ccTouchEnded(pTouch, pEvent);
	}
	
	void resetControlMenu();
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -150, true);
	}
	
	void myInit(CCObject* t_home, SEL_CallFunc d_home, CCObject* t_continue, SEL_CallFunc d_continue, CCObject* t_control, SEL_CallFunc d_gesture, SEL_CallFunc d_button, SEL_CallFunc d_joystick);
	void menuAction(CCObject* sender);
};

#endif /* defined(__DrawingJack__PausePopupLayer__) */
