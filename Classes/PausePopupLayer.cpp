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

PausePopupLayer* PausePopupLayer::create(CCObject* t_home, SEL_CallFunc d_home, CCObject* t_continue, SEL_CallFunc d_continue, CCObject* t_control, SEL_CallFunc d_gesture, SEL_CallFunc d_button, SEL_CallFunc d_joystick)
{
	PausePopupLayer* t_ppl = new PausePopupLayer();
	if(t_ppl->init())
	{
		t_ppl->myInit(t_home, d_home, t_continue, d_continue, t_control, d_gesture, d_button, d_joystick);
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

void PausePopupLayer::myInit(CCObject* t_home, SEL_CallFunc d_home, CCObject* t_continue, SEL_CallFunc d_continue, CCObject* t_control, SEL_CallFunc d_gesture, SEL_CallFunc d_button, SEL_CallFunc d_joystick)
{
	is_action = false;
	
	target_home = t_home;
	delegate_home = d_home;
	target_continue = t_continue;
	delegate_continue = d_continue;
	target_control= t_control;
//	delegate_gesture = d_gesture;
//	delegate_button = d_button;
//	delegate_joystick = d_joystick;
	delegate_slow = d_gesture;
	delegate_normal = d_button;
	delegate_fast = d_joystick;
	
	setTouchEnabled(true);
	
	main_case = CCSprite::create("pause_popup_case.png");
	main_case->setPosition(ccp(40,280+DataStorageHub::sharedInstance()->ui_top_control));
	addChild(main_case);
	
	CCMenuItem* home_item = CCMenuItemImage::create("pause_popup_home_normal.png", "pause_popup_home_selected.png", this, menu_selector(PausePopupLayer::menuAction));
	home_item->setTag(kMenuTagPPL_home);
	home_menu = CCMenu::createWithItem(home_item);
	home_menu->setPosition(ccp(30,220));
	main_case->addChild(home_menu);
	
	CCMenuItem* continue_item = CCMenuItemImage::create("pause_popup_continue_normal.png", "pause_popup_continue_selected.png", this, menu_selector(PausePopupLayer::menuAction));
	continue_item->setTag(kMenuTagPPL_continue);
	continue_menu = CCMenu::createWithItem(continue_item);
	continue_menu->setPosition(ccp(main_case->getContentSize().width/2.f,40));
	main_case->addChild(continue_menu);
	
	myDSH = DataStorageHub::sharedInstance();
	
//	gesture_menu = NULL;
//	button_menu = NULL;
//	joystick_menu = NULL;
	slow_menu = NULL;
	normal_menu = NULL;
	fast_menu = NULL;
	
	resetControlMenu();
	
	startPopupAnimation();
}

void PausePopupLayer::resetControlMenu()
{
	is_action = true;
//	if(gesture_menu)		gesture_menu->removeFromParentAndCleanup(true);
//	if(button_menu)			button_menu->removeFromParentAndCleanup(true);
//	if(joystick_menu)		joystick_menu->removeFromParentAndCleanup(true);
	if(slow_menu)			slow_menu->removeFromParentAndCleanup(true);
	if(normal_menu)			normal_menu->removeFromParentAndCleanup(true);
	if(fast_menu)			fast_menu->removeFromParentAndCleanup(true);
	
	string gesture_filename;
	string button_filename;
	string joystick_filename;
//	ControlType recent_type = ControlType(myDSH->getIntegerForKey(kDSH_Key_lastSelectedControler));
	float jack_base_speed = GameData::sharedGameData()->jack_base_speed;
	
//	if(recent_type == kControlType_unsetted)
//	{
//		recent_type = kControlType_gesture;
//		myDSH->setIntegerForKey(kDSH_Key_lastSelectedControler, recent_type);
//		myDSH->setIntegerForKey(kDSH_Key_controlOriginX, 160);
//		myDSH->setIntegerForKey(kDSH_Key_controlOriginY, 80);
//	}
	
//	if(recent_type == kControlType_gesture)
	if(jack_base_speed == 1.2f)
	{
		gesture_filename = "speed1_color_";
		button_filename = "speed2_gray_";
		joystick_filename = "speed3_gray_";
	}
	else if(jack_base_speed == 1.5f)
	{
		gesture_filename = "speed1_gray_";
		button_filename = "speed2_color_";
		joystick_filename = "speed3_gray_";
	}
	else if(jack_base_speed == 2.f)
	{
		gesture_filename = "speed1_gray_";
		button_filename = "speed2_gray_";
		joystick_filename = "speed3_color_";
	}
	else
	{
		gesture_filename = "speed1_gray_";
		button_filename = "speed2_gray_";
		joystick_filename = "speed3_gray_";
	}
	
	CCMenuItem* gesture_item = CCMenuItemImage::create((gesture_filename+"normal.png").c_str(), (gesture_filename+"selected.png").c_str(), this, menu_selector(PausePopupLayer::menuAction));
	gesture_item->setTag(kMenuTagPPL_slow);
	slow_menu = CCMenu::createWithItem(gesture_item);
	slow_menu->setEnabled(jack_base_speed != 1.2f);
	slow_menu->setPosition(ccp(main_case->getContentSize().width/2.f-95,125));
	main_case->addChild(slow_menu);
	
	CCMenuItem* button_item = CCMenuItemImage::create((button_filename+"normal.png").c_str(), (button_filename+"selected.png").c_str(), this, menu_selector(PausePopupLayer::menuAction));
	button_item->setTag(kMenuTagPPL_normal);
	normal_menu = CCMenu::createWithItem(button_item);
	normal_menu->setEnabled(jack_base_speed != 1.5f);
	normal_menu->setPosition(ccp(main_case->getContentSize().width/2.f,125));
	main_case->addChild(normal_menu);
	
	CCMenuItem* joystick_item = CCMenuItemImage::create((joystick_filename+"normal.png").c_str(), (joystick_filename+"selected.png").c_str(), this, menu_selector(PausePopupLayer::menuAction));
	joystick_item->setTag(kMenuTagPPL_fast);
	fast_menu = CCMenu::createWithItem(joystick_item);
	fast_menu->setEnabled(jack_base_speed != 2.f);
	fast_menu->setPosition(ccp(main_case->getContentSize().width/2.f+95,125));
	main_case->addChild(fast_menu);
	
	is_action = false;
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
			AudioEngine::sharedInstance()->playSound("sound_background.mp3", true);
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
			(target_control->*delegate_slow)();
			resetControlMenu();
		}
		else if(tag == kMenuTagPPL_normal)
		{
			is_action = true;
//			myDSH->setIntegerForKey(kDSH_Key_lastSelectedControler, kControlType_button);
			(target_control->*delegate_normal)();
			resetControlMenu();
		}
		else if(tag == kMenuTagPPL_fast)
		{
			is_action = true;
//			myDSH->setIntegerForKey(kDSH_Key_lastSelectedControler, kControlType_joystick_button);
			(target_control->*delegate_fast)();
			resetControlMenu();
		}
	}
}