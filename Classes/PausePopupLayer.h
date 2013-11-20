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
	kMenuTagPPL_fast,
	kMenuTagPPL_replay
};

class PausePopupLayer : public CCLayer
{
public:
	static PausePopupLayer* create(CCObject* t_home, SEL_CallFunc d_home, CCObject* t_continue, SEL_CallFunc d_continue, CCObject* t_speed, SEL_CallFunc d_slow, SEL_CallFunc d_normal, SEL_CallFunc d_fast, CCObject* t_replay, SEL_CallFunc d_replay);
	void removeSelf();
	
private:
	bool is_ing;
	
	CCMenu* home_menu;
	CCMenu* continue_menu;
//	CCMenu* gesture_menu;
//	CCMenu* button_menu;
//	CCMenu* joystick_menu;
//	CCMenu* slow_menu;
//	CCMenu* normal_menu;
//	CCMenu* fast_menu;
	CCMenu* replay_menu;
	
	int touched_number;
	
	bool is_action;
	
	CCSprite* main_case;
	
	CCObject* target_home;
	SEL_CallFunc delegate_home;
	CCObject* target_continue;
	SEL_CallFunc delegate_continue;
	CCObject* target_speed;
//	SEL_CallFunc delegate_gesture;
//	SEL_CallFunc delegate_button;
//	SEL_CallFunc delegate_joystick;
	SEL_CallFunc delegate_slow;
	SEL_CallFunc delegate_normal;
	SEL_CallFunc delegate_fast;
	CCObject* target_replay;
	SEL_CallFunc delegate_replay;
	
	void startPopupAnimation();
	
	void startPopdownAnimation();
	
	void endPopAnimation();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	void resetControlMenu();
	
	virtual void registerWithTouchDispatcher();
	
	void myInit(CCObject* t_home, SEL_CallFunc d_home, CCObject* t_continue, SEL_CallFunc d_continue, CCObject* t_speed, SEL_CallFunc d_slow, SEL_CallFunc d_normal, SEL_CallFunc d_fast, CCObject* t_replay, SEL_CallFunc d_replay);
	void menuAction(CCObject* sender);
};

#endif /* defined(__DrawingJack__PausePopupLayer__) */
