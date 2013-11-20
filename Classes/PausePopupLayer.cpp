//
//  PausePopupLayer.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 7..
//
//

#include "PausePopupLayer.h"
#include "EnumDefine.h"
#include "AudioEngine.h"
#include "GameData.h"

PausePopupLayer* PausePopupLayer::create(CCObject* t_home, SEL_CallFunc d_home, CCObject* t_continue, SEL_CallFunc d_continue, CCObject* t_speed, SEL_CallFunc d_slow, SEL_CallFunc d_normal, SEL_CallFunc d_fast, CCObject* t_replay, SEL_CallFunc d_replay)
{
	PausePopupLayer* t_ppl = new PausePopupLayer();
	if(t_ppl->init())
	{
		t_ppl->myInit(t_home, d_home, t_continue, d_continue, t_speed, d_slow, d_normal, d_fast, t_replay, d_replay);
		t_ppl->autorelease();
		return t_ppl;
	}
	CC_SAFE_DELETE(t_ppl);
	return NULL;
}

void PausePopupLayer::removeSelf()
{
	setTouchEnabled(false);
	removeFromParentAndCleanup(true);
}

void PausePopupLayer::myInit(CCObject* t_home, SEL_CallFunc d_home, CCObject* t_continue, SEL_CallFunc d_continue, CCObject* t_speed, SEL_CallFunc d_slow, SEL_CallFunc d_normal, SEL_CallFunc d_fast, CCObject* t_replay, SEL_CallFunc d_replay)
{
	is_action = false;
	
	target_home = t_home;
	delegate_home = d_home;
	target_continue = t_continue;
	delegate_continue = d_continue;
	target_speed = t_speed;
//	delegate_gesture = d_gesture;
//	delegate_button = d_button;
//	delegate_joystick = d_joystick;
	delegate_slow = d_slow;
	delegate_normal = d_normal;
	delegate_fast = d_fast;
	target_replay = t_replay;
	delegate_replay = d_replay;
	
	touched_number = 0;
	
	setTouchEnabled(true);
	
	
	CCSprite* gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,myDSH->ui_center_y));
	gray->setScaleY(myDSH->ui_top/320.f);
	addChild(gray);
	
	
	main_case = CCSprite::create("pause_popup_case.png");
	main_case->setPosition(ccp(40,myDSH->ui_top-25));
	addChild(main_case);
	
	CCSprite* n_home = CCSprite::create("pause_popup_home.png");
	CCSprite* s_home = CCSprite::create("pause_popup_home.png");
	s_home->setColor(ccGRAY);
	
	CCMenuItem* home_item = CCMenuItemSprite::create(n_home, s_home, this, menu_selector(PausePopupLayer::menuAction));
	home_item->setTag(kMenuTagPPL_home);
	home_menu = CCMenu::createWithItem(home_item);
	home_menu->setPosition(ccp(main_case->getContentSize().width/2.f,132));
	main_case->addChild(home_menu);
	
	
	CCSprite* n_replay = CCSprite::create("pause_popup_replay.png");
	CCSprite* s_replay = CCSprite::create("pause_popup_replay.png");
	s_replay->setColor(ccGRAY);
	
	CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(PausePopupLayer::menuAction));
	replay_item->setTag(kMenuTagPPL_replay);
	replay_menu = CCMenu::createWithItem(replay_item);
	replay_menu->setPosition(ccp(main_case->getContentSize().width/2.f,86));
	main_case->addChild(replay_menu);
	
	
	CCSprite* n_continue = CCSprite::create("pause_popup_continue.png");
	CCSprite* s_continue = CCSprite::create("pause_popup_continue.png");
	s_continue->setColor(ccGRAY);
	
	CCMenuItem* continue_item = CCMenuItemSprite::create(n_continue, s_continue, this, menu_selector(PausePopupLayer::menuAction));
	continue_item->setTag(kMenuTagPPL_continue);
	continue_menu = CCMenu::createWithItem(continue_item);
	continue_menu->setPosition(ccp(main_case->getContentSize().width/2.f,40));
	main_case->addChild(continue_menu);
	
	
//	gesture_menu = NULL;
//	button_menu = NULL;
//	joystick_menu = NULL;
//	slow_menu = NULL;
//	normal_menu = NULL;
//	fast_menu = NULL;
	
	resetControlMenu();
	
	startPopupAnimation();
}

void PausePopupLayer::resetControlMenu()
{
//	is_action = true;
//	if(gesture_menu)		gesture_menu->removeFromParentAndCleanup(true);
//	if(button_menu)			button_menu->removeFromParentAndCleanup(true);
//	if(joystick_menu)		joystick_menu->removeFromParentAndCleanup(true);
//	if(slow_menu)			slow_menu->removeFromParentAndCleanup(true);
//	if(normal_menu)			normal_menu->removeFromParentAndCleanup(true);
//	if(fast_menu)			fast_menu->removeFromParentAndCleanup(true);
//	
//	string gesture_filename;
//	string button_filename;
//	string joystick_filename;
//	ControlType recent_type = ControlType(myDSH->getIntegerForKey(kDSH_Key_lastSelectedControler));
//	float jack_base_speed = myGD->jack_base_speed;
	
//	if(recent_type == kControlType_unsetted)
//	{
//		recent_type = kControlType_gesture;
//		myDSH->setIntegerForKey(kDSH_Key_lastSelectedControler, recent_type);
//		myDSH->setIntegerForKey(kDSH_Key_controlOriginX, 160);
//		myDSH->setIntegerForKey(kDSH_Key_controlOriginY, 80);
//	}
	
//	if(recent_type == kControlType_gesture)
//	if(jack_base_speed == 1.2f)
//	{
//		gesture_filename = "speed1_color_";
//		button_filename = "speed2_gray_";
//		joystick_filename = "speed3_gray_";
//	}
//	else if(jack_base_speed == 1.5f)
//	{
//		gesture_filename = "speed1_gray_";
//		button_filename = "speed2_color_";
//		joystick_filename = "speed3_gray_";
//	}
//	else if(jack_base_speed == 2.f)
//	{
//		gesture_filename = "speed1_gray_";
//		button_filename = "speed2_gray_";
//		joystick_filename = "speed3_color_";
//	}
//	else
//	{
//		gesture_filename = "speed1_gray_";
//		button_filename = "speed2_gray_";
//		joystick_filename = "speed3_gray_";
//	}
//	
//	CCMenuItem* gesture_item = CCMenuItemImage::create((gesture_filename+"normal.png").c_str(), (gesture_filename+"selected.png").c_str(), this, menu_selector(PausePopupLayer::menuAction));
//	gesture_item->setTag(kMenuTagPPL_slow);
//	slow_menu = CCMenu::createWithItem(gesture_item);
//	slow_menu->setEnabled(jack_base_speed != 1.2f);
////	slow_menu->setEnabled(false);
//	slow_menu->setPosition(ccp(main_case->getContentSize().width/2.f-95,125));
//	main_case->addChild(slow_menu);
//	
//	CCMenuItem* button_item = CCMenuItemImage::create((button_filename+"normal.png").c_str(), (button_filename+"selected.png").c_str(), this, menu_selector(PausePopupLayer::menuAction));
//	button_item->setTag(kMenuTagPPL_normal);
//	normal_menu = CCMenu::createWithItem(button_item);
//	normal_menu->setEnabled(jack_base_speed != 1.5f);
////	normal_menu->setEnabled(false);
//	normal_menu->setPosition(ccp(main_case->getContentSize().width/2.f,125));
//	main_case->addChild(normal_menu);
//	
//	CCMenuItem* joystick_item = CCMenuItemImage::create((joystick_filename+"normal.png").c_str(), (joystick_filename+"selected.png").c_str(), this, menu_selector(PausePopupLayer::menuAction));
//	joystick_item->setTag(kMenuTagPPL_fast);
//	fast_menu = CCMenu::createWithItem(joystick_item);
//	fast_menu->setEnabled(jack_base_speed != 2.f);
////	fast_menu->setEnabled(false);
//	fast_menu->setPosition(ccp(main_case->getContentSize().width/2.f+95,125));
//	main_case->addChild(fast_menu);
//	
//	is_action = false;
}

void PausePopupLayer::menuAction(CCObject* sender)
{
	int tag = ((CCNode*)sender)->getTag();
	if(!is_action && !is_ing)
	{
		AudioEngine::sharedInstance()->playEffect("sound_buttonClick_Low.mp3", false);
		if(tag == kMenuTagPPL_home)
		{
			is_action = true;
//			AudioEngine::sharedInstance()->playSound("sound_background.mp3", true);
			(target_home->*delegate_home)();
			startPopdownAnimation();
		}
		else if(tag == kMenuTagPPL_continue)
		{
			is_action = true;
			(target_continue->*delegate_continue)();
			startPopdownAnimation();
		}
		else if(tag == kMenuTagPPL_slow)
		{
			is_action = true;
//			myDSH->setIntegerForKey(kDSH_Key_lastSelectedControler, kControlType_gesture);
			(target_speed->*delegate_slow)();
			resetControlMenu();
		}
		else if(tag == kMenuTagPPL_normal)
		{
			is_action = true;
//			myDSH->setIntegerForKey(kDSH_Key_lastSelectedControler, kControlType_button);
			(target_speed->*delegate_normal)();
			resetControlMenu();
		}
		else if(tag == kMenuTagPPL_fast)
		{
			is_action = true;
//			myDSH->setIntegerForKey(kDSH_Key_lastSelectedControler, kControlType_joystick_button);
			(target_speed->*delegate_fast)();
			resetControlMenu();
		}
		else if(tag == kMenuTagPPL_replay)
		{
			is_action = true;
			(target_replay->*delegate_replay)();
			startPopdownAnimation();
		}
	}
}

void PausePopupLayer::startPopupAnimation()
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

void PausePopupLayer::startPopdownAnimation()
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

void PausePopupLayer::endPopAnimation()
{
	is_ing = false;
	AudioEngine::sharedInstance()->setAppBack();
	CCDirector::sharedDirector()->pause();
}

bool PausePopupLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number != 0)		return true;
	if(home_menu->ccTouchBegan(pTouch, pEvent))					touched_number = kMenuTagPPL_home;
	else if(continue_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kMenuTagPPL_continue;
	else if(replay_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kMenuTagPPL_replay;
	//		else if(slow_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kMenuTagPPL_slow;
	//		else if(normal_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kMenuTagPPL_normal;
	//		else if(fast_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kMenuTagPPL_fast;
	return true;
}

void PausePopupLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kMenuTagPPL_home)						home_menu->ccTouchMoved(pTouch, pEvent);
	else if(touched_number == kMenuTagPPL_continue)				continue_menu->ccTouchMoved(pTouch, pEvent);
	else if(touched_number == kMenuTagPPL_replay)				replay_menu->ccTouchMoved(pTouch, pEvent);
	//		else if(touched_number == kMenuTagPPL_slow)					slow_menu->ccTouchMoved(pTouch, pEvent);
	//		else if(touched_number == kMenuTagPPL_normal)				normal_menu->ccTouchMoved(pTouch, pEvent);
	//		else if(touched_number == kMenuTagPPL_fast)					fast_menu->ccTouchMoved(pTouch, pEvent);
}

void PausePopupLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kMenuTagPPL_home){						home_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;		}
	else if(touched_number == kMenuTagPPL_continue){			continue_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;		}
	else if(touched_number == kMenuTagPPL_replay){				replay_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;		}
	//		else if(touched_number == kMenuTagPPL_slow)					slow_menu->ccTouchEnded(pTouch, pEvent);
	//		else if(touched_number == kMenuTagPPL_normal)				normal_menu->ccTouchEnded(pTouch, pEvent);
	//		else if(touched_number == kMenuTagPPL_fast)					fast_menu->ccTouchEnded(pTouch, pEvent);
}

void PausePopupLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kMenuTagPPL_home){						home_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;		}
	else if(touched_number == kMenuTagPPL_continue){			continue_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;		}
	else if(touched_number == kMenuTagPPL_replay){				replay_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;		}
	//		else if(touched_number == kMenuTagPPL_slow)					slow_menu->ccTouchEnded(pTouch, pEvent);
	//		else if(touched_number == kMenuTagPPL_normal)				normal_menu->ccTouchEnded(pTouch, pEvent);
	//		else if(touched_number == kMenuTagPPL_fast)					fast_menu->ccTouchEnded(pTouch, pEvent);
}

void PausePopupLayer::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -150, true);
}
