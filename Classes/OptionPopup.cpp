//
//  OptionPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#include "OptionPopup.h"
#include "MyLocalization.h"
//#include "WorldMapScene.h"
#include "PuzzleMapScene.h"
#include "HelpPopup.h"
#include "LogoutPopup.h"
#include "NotiPopup.h"
#include "WithdrawPopup.h"
#include "PuzzleCache.h"
#include "MiniGamePopup.h"
#include "cocos-ext.h"
#include "StageImgLoader.h"
#include "KSAlertView.h"

#include "TutorialScene.h"
#include "MinsuScene.h"

USING_NS_CC_EXT;

enum OptionPopupZorder{
	kOP_Z_gray = 1,
	kOP_Z_back,
	kOP_Z_content,
	kOP_Z_popup
};

enum OptionPopupMenuTag{
	kOP_MT_close = 1,
	kOP_MT_bgm,
	kOP_MT_effect,
	kOP_MT_gameui_left,
	kOP_MT_gameui_full,
	kOP_MT_gameui_right,
	kOP_MT_help,
	kOP_MT_logout,
	kOP_MT_noti,
	kOP_MT_withdraw,
	kOP_MT_controlJoystickRight,
	kOP_MT_controlJoystickLeft,
	kOP_MT_joystickFixed,
	kOP_MT_joystickMove,
	kOP_MT_tutorial,
	kOP_MT_minsu
};

void OptionPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

bool OptionPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	
//	MiniGamePopup* t_popup = MiniGamePopup::create((MiniGameCode)(kMiniGameCode_counting), nullptr);
//	addChild(t_popup, 4);
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kOP_Z_gray);
	
	main_case = CCSprite::create("option_back.png");
	main_case->setAnchorPoint(ccp(0.5,0.5));
	main_case->setPosition(ccp(240,-160));
	addChild(main_case, kOP_Z_back);
	
	if(myDSH->isCheatKeyEnable())
	{
		CCSprite* n_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		n_temp->setOpacity(0);
		CCSprite* s_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		s_temp->setOpacity(0);
		
		CCMenuItemSprite* temp_item = CCMenuItemSprite::create(n_temp, s_temp, this, menu_selector(OptionPopup::cheatAction));
		temp_item->setTag(1);
		CCMenu* temp_menu = CCMenu::createWithItem(temp_item);
		temp_menu->setPosition(ccpSub(ccp(15,305), ccp(240,160)));
		main_case->addChild(temp_menu, kOP_Z_content);
	}
	
	
	
	CCSprite* n_close = CCSprite::create("option_small_close.png");
	CCSprite* s_close = CCSprite::create("option_small_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(OptionPopup::menuAction));
	close_item->setTag(kOP_MT_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kOP_MT_close));
	main_case->addChild(close_menu, kOP_Z_content);
	
	
	
	CCSprite* n_help = CCSprite::create("option_help.png");
	CCSprite* s_help = CCSprite::create("option_help.png");
	s_help->setColor(ccGRAY);
	
	CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(OptionPopup::menuAction));
	help_item->setTag(kOP_MT_help);
	
	CCMenu* help_menu = CCMenu::createWithItem(help_item);
	help_menu->setPosition(getContentPosition(kOP_MT_help));
	main_case->addChild(help_menu, kOP_Z_content);
	
	
	CCSprite* n_logout = CCSprite::create("option_logout.png");
	CCSprite* s_logout = CCSprite::create("option_logout.png");
	s_logout->setColor(ccGRAY);
	
	CCMenuItem* logout_item = CCMenuItemSprite::create(n_logout, s_logout, this, menu_selector(OptionPopup::menuAction));
	logout_item->setTag(kOP_MT_logout);
	
	CCMenu* logout_menu = CCMenu::createWithItem(logout_item);
	logout_menu->setPosition(getContentPosition(kOP_MT_logout));
	main_case->addChild(logout_menu, kOP_Z_content);
	
	
	CCSprite* n_noti = CCSprite::create("option_noti.png");
	CCSprite* s_noti = CCSprite::create("option_noti.png");
	s_noti->setColor(ccGRAY);
	
	CCMenuItem* noti_item = CCMenuItemSprite::create(n_noti, s_noti, this, menu_selector(OptionPopup::menuAction));
	noti_item->setTag(kOP_MT_noti);
	
	CCMenu* noti_menu = CCMenu::createWithItem(noti_item);
	noti_menu->setPosition(getContentPosition(kOP_MT_noti));
	main_case->addChild(noti_menu, kOP_Z_content);
	
	
	CCSprite* n_withdraw = CCSprite::create("option_withdraw.png");
	CCSprite* s_withdraw = CCSprite::create("option_withdraw.png");
	s_withdraw->setColor(ccGRAY);
	
	CCMenuItem* withdraw_item = CCMenuItemSprite::create(n_withdraw, s_withdraw, this, menu_selector(OptionPopup::menuAction));
	withdraw_item->setTag(kOP_MT_withdraw);
	
	CCMenu* withdraw_menu = CCMenu::createWithItem(withdraw_item);
	withdraw_menu->setPosition(getContentPosition(kOP_MT_withdraw));
	main_case->addChild(withdraw_menu, kOP_Z_content);
	
	
	CCSprite* n_joystick_right = CCSprite::create("option_mode_right.png");
	n_joystick_right->setColor(ccGRAY);
	CCSprite* s_joystick_right = CCSprite::create("option_mode_right.png");
	s_joystick_right->setColor(ccc3(100, 100, 100));
	CCSprite* d_joystick_right = CCSprite::create("option_mode_right.png");
	
	joystick_right_item = CCMenuItemSprite::create(n_joystick_right, s_joystick_right, d_joystick_right, this, menu_selector(OptionPopup::menuAction));
	joystick_right_item->setTag(kOP_MT_controlJoystickRight);
	
	CCMenu* joystick_right_menu = CCMenu::createWithItem(joystick_right_item);
	joystick_right_menu->setPosition(getContentPosition(kOP_MT_controlJoystickRight));
	main_case->addChild(joystick_right_menu, kOP_Z_content);
	
	
	CCSprite* n_joystick_left = CCSprite::create("option_mode_left.png");
	n_joystick_left->setColor(ccGRAY);
	CCSprite* s_joystick_left = CCSprite::create("option_mode_left.png");
	s_joystick_left->setColor(ccc3(100, 100, 100));
	CCSprite* d_joystick_left = CCSprite::create("option_mode_left.png");
	
	joystick_left_item = CCMenuItemSprite::create(n_joystick_left, s_joystick_left, d_joystick_left, this, menu_selector(OptionPopup::menuAction));
	joystick_left_item->setTag(kOP_MT_controlJoystickLeft);
	
	CCMenu* joystick_left_menu = CCMenu::createWithItem(joystick_left_item);
	joystick_left_menu->setPosition(getContentPosition(kOP_MT_controlJoystickLeft));
	main_case->addChild(joystick_left_menu, kOP_Z_content);
	
	if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_right)
	{
		joystick_right_item->setEnabled(false);
		joystick_left_item->setEnabled(true);
	}
	else
	{
		joystick_right_item->setEnabled(true);
		joystick_left_item->setEnabled(false);
	}
	
	
	CCSprite* n_joystick_fixed = CCSprite::create("option_mode_lock.png");
	n_joystick_fixed->setColor(ccGRAY);
	CCSprite* s_joystick_fixed = CCSprite::create("option_mode_lock.png");
	s_joystick_fixed->setColor(ccc3(100, 100, 100));
	CCSprite* d_joystick_fixed = CCSprite::create("option_mode_lock.png");
	
	joystick_fixed_item = CCMenuItemSprite::create(n_joystick_fixed, s_joystick_fixed, d_joystick_fixed, this, menu_selector(OptionPopup::menuAction));
	joystick_fixed_item->setTag(kOP_MT_joystickFixed);
	
	CCMenu* joystick_fixed_menu = CCMenu::createWithItem(joystick_fixed_item);
	joystick_fixed_menu->setPosition(getContentPosition(kOP_MT_joystickFixed));
	main_case->addChild(joystick_fixed_menu, kOP_Z_content);
	
	
	CCSprite* n_joystick_move = CCSprite::create("option_mode_move.png");
	n_joystick_move->setColor(ccGRAY);
	CCSprite* s_joystick_move = CCSprite::create("option_mode_move.png");
	s_joystick_move->setColor(ccc3(100, 100, 100));
	CCSprite* d_joystick_move = CCSprite::create("option_mode_move.png");
	
	joystick_move_item = CCMenuItemSprite::create(n_joystick_move, s_joystick_move, d_joystick_move, this, menu_selector(OptionPopup::menuAction));
	joystick_move_item->setTag(kOP_MT_joystickMove);
	
	CCMenu* joystick_move_menu = CCMenu::createWithItem(joystick_move_item);
	joystick_move_menu->setPosition(getContentPosition(kOP_MT_joystickMove));
	main_case->addChild(joystick_move_menu, kOP_Z_content);
	
	if(myDSH->getBoolForKey(kDSH_Key_isControlJoystickFixed))
	{
		joystick_fixed_item->setEnabled(false);
		joystick_move_item->setEnabled(true);
	}
	else
	{
		joystick_fixed_item->setEnabled(true);
		joystick_move_item->setEnabled(false);
	}
	
	
	CCSprite* n_tutorial = CCSprite::create("option_tutorial.png");
	CCSprite* s_tutorial = CCSprite::create("option_tutorial.png");
	s_tutorial->setColor(ccGRAY);
	
	CCMenuItem* tutorial_item = CCMenuItemSprite::create(n_tutorial, s_tutorial, this, menu_selector(OptionPopup::menuAction));
	tutorial_item->setTag(kOP_MT_tutorial);
	
	CCMenu* tutorial_menu = CCMenu::createWithItem(tutorial_item);
	tutorial_menu->setPosition(getContentPosition(kOP_MT_tutorial));
	main_case->addChild(tutorial_menu, kOP_Z_content);
	
	CCSprite* n_minsu = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
	n_minsu->setOpacity(0);
	CCSprite* s_minsu = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
	s_minsu->setOpacity(0);
	
	CCMenuItem* minsu_item = CCMenuItemSprite::create(n_minsu, s_minsu, this, menu_selector(OptionPopup::menuAction));
	minsu_item->setTag(kOP_MT_minsu);
	
	CCMenu* minsu_menu = CCMenu::createWithItem(minsu_item);
	minsu_menu->setPosition(getContentPosition(kOP_MT_minsu));
	main_case->addChild(minsu_menu, kOP_Z_content);
	
	
	CCSprite* n_bgm = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 94, 40));
	n_bgm->setOpacity(0);
	CCSprite* s_bgm = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 94, 40));
	s_bgm->setOpacity(0);
	
	CCMenuItem* bgm_item = CCMenuItemSprite::create(n_bgm, s_bgm, this, menu_selector(OptionPopup::menuAction));
	bgm_item->setTag(kOP_MT_bgm);
	
	bgm_menu = CCMenu::createWithItem(bgm_item);
	bgm_menu->setPosition(getContentPosition(kOP_MT_bgm));
	main_case->addChild(bgm_menu, kOP_Z_content);
	
	bgm_img = NULL;
	resetBgmMenu();
	
	
	CCSprite* n_effect = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 94, 40));
	n_effect->setOpacity(0);
	CCSprite* s_effect = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 94, 40));
	s_effect->setOpacity(0);
	
	CCMenuItem* effect_item = CCMenuItemSprite::create(n_effect, s_effect, this, menu_selector(OptionPopup::menuAction));
	effect_item->setTag(kOP_MT_effect);
	
	effect_menu = CCMenu::createWithItem(effect_item);
	effect_menu->setPosition(getContentPosition(kOP_MT_effect));
	main_case->addChild(effect_menu, kOP_Z_content);
	
	effect_img = NULL;
	resetEffectMenu();
	
	is_menu_enable = false;

#if 0 // 심플 예제
	KSAlertView* av = KSAlertView::create();
	
	av->setContentNode(
										 CCNode::create()
										 );
	
	av->setVScroll(CCScale9Sprite::create("popup_bar_v.png", CCRectMake(0, 0, 23, 53),
																				CCRectMake(7, 7, 23 - 7*2, 53 - 7*2 - 4)));
	av->setHScroll(CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
																				CCRectMake(10, 7, 53 - 10*2, 23 - 7*2)));
	auto m1 = CCMenuItemImageLambda::create("ui_common_ok.png", "ui_common_ok.png",
																					[](CCObject* e){
																						CCLog("press!!");
																					});
	av->addButton(m1);
	av->addButton(CCMenuItemImageLambda::create("ticket_send.png", "ticket_send.png",
																							[](CCObject* e){
																								CCLog("press!!");
																							}));
	addChild(av, kPMS_Z_helpRequest);
	av->setCloseButton(CCMenuItemImageLambda::create("tutorial_common_cancel.png", "tutorial_common_cancel.png",
																									 [=](CCObject* sender){
																										 CCLog("close!!");
																									 }));
	av->show();
#endif
#if 0 // 가운데 정렬 예제
	KSAlertView* av = KSAlertView::create();
	CCNode* __con = CCNode::create();
	
	KSNode* con2 = new KSNode();
	con2->init();
	con2->autorelease();
	__con->addChild(con2);
	
	//	con->setAnchorPoint(ccp(0, 0));
	con2->addChild(CCLabelTTF::create(CCString::createWithFormat("%d Ruby 로 오픈", 1)->getCString(), mySGD->getFont().c_str(), 25));
	con2->addChild(CCLabelTTF::create(CCString::createWithFormat("%d Ruby 로 오zzz픈", 1)->getCString(), mySGD->getFont().c_str(), 25));
	
	av->setContentNode(__con);
	av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
	av->setContentBorder(CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0,0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
	av->setBorderScale(0.9f);
	//	av->setTitleStr("지금 열기");
	addChild(av, kPMS_Z_helpRequest);
	av->setCloseButton(CCMenuItemImageLambda::create("tutorial_common_cancel.png", "tutorial_common_cancel.png",
																									 [=](CCObject* sender){
																										 is_menu_enable = true;
																									 }));
	
	con2->setPosition(ccp((av->getViewSize() / 2.f).width,
												-(av->getViewSize() / 2.f).height));
	con2->alignItemsVerticallyWithPadding(30);
	av->show();
#endif
	
	
	return true;
}

void OptionPopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void OptionPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(OptionPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void OptionPopup::endShowPopup()
{
	is_menu_enable = true;
}

void OptionPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,-160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(OptionPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void OptionPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void OptionPopup::cheatAction(CCObject *sender)
{
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == 1) // gold up 10000
	{
		myDSH->setIntegerForKey(kDSH_Key_savedGold, myDSH->getIntegerForKey(kDSH_Key_savedGold)+10000);
	}
}

void OptionPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kOP_MT_close)
	{
		hidePopup();
	}
	else if(tag == kOP_MT_bgm)
	{
		myDSH->setBoolForKey(kDSH_Key_bgmOff, !myDSH->getBoolForKey(kDSH_Key_bgmOff));
		resetBgmMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_effect)
	{
		myDSH->setBoolForKey(kDSH_Key_effectOff, !myDSH->getBoolForKey(kDSH_Key_effectOff));
		resetEffectMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_gameui_left)
	{
		myGD->setUItype(kGT_leftUI);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_gameui_full)
	{
		myGD->setUItype(kGT_full);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_gameui_right)
	{
		myGD->setUItype(kGT_rightUI);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_help)
	{
		HelpPopup* t_hp = HelpPopup::create(this, callfunc_selector(OptionPopup::popupClose));
		addChild(t_hp, kOP_Z_popup);
	}
	else if(tag == kOP_MT_logout)
	{
		LogoutPopup* t_lp = LogoutPopup::create(this, callfunc_selector(OptionPopup::popupClose));
		addChild(t_lp, kOP_Z_popup);
	}
	else if(tag == kOP_MT_noti)
	{
		NotiPopup* t_np = NotiPopup::create(this, callfunc_selector(OptionPopup::popupClose));
		addChild(t_np, kOP_Z_popup);
	}
	else if(tag == kOP_MT_withdraw)
	{
		WithdrawPopup* t_wp = WithdrawPopup::create(this, callfunc_selector(OptionPopup::popupClose));
		addChild(t_wp, kOP_Z_popup);
	}
	else if(tag == kOP_MT_controlJoystickRight)
	{
		myDSH->setIntegerForKey(kDSH_Key_controlJoystickDirection, kControlJoystickDirection_right);
		joystick_right_item->setEnabled(false);
		joystick_left_item->setEnabled(true);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_controlJoystickLeft)
	{
		myDSH->setIntegerForKey(kDSH_Key_controlJoystickDirection, kControlJoystickDirection_left);
		joystick_right_item->setEnabled(true);
		joystick_left_item->setEnabled(false);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_joystickFixed)
	{
		myDSH->setBoolForKey(kDSH_Key_isControlJoystickFixed, true);
		joystick_fixed_item->setEnabled(false);
		joystick_move_item->setEnabled(true);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_joystickMove)
	{
		myDSH->setBoolForKey(kDSH_Key_isControlJoystickFixed, false);
		joystick_fixed_item->setEnabled(true);
		joystick_move_item->setEnabled(false);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_tutorial)
	{
		CCDirector::sharedDirector()->replaceScene(TutorialScene::scene());
	}
	else if(tag == kOP_MT_minsu)
	{
		CCDirector::sharedDirector()->replaceScene(MinsuScene::scene());
	}
}

void OptionPopup::popupClose()
{
	is_menu_enable = true;
}

void OptionPopup::resetBgmMenu()
{
	if(bgm_img)
	{
		bgm_img->removeFromParent();
		bgm_img = NULL;
	}
	
	string filename;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_bgmOff))
	{
		filename = "option_sound_off.png";
		img_position = ccp(-20,0);
	}
	else
	{
		filename = "option_sound_on.png";
		img_position = ccp(20,0);
	}
	
	bgm_img = CCSprite::create(filename.c_str());
	bgm_img->setPosition(ccpAdd(getContentPosition(kOP_MT_bgm), img_position));
	main_case->addChild(bgm_img, kOP_Z_content);
}

void OptionPopup::resetEffectMenu()
{
	if(effect_img)
	{
		effect_img->removeFromParent();
		effect_img = NULL;
	}
	
	string filename;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_effectOff))
	{
		filename = "option_sound_off.png";
		img_position = ccp(-20,0);
	}
	else
	{
		filename = "option_sound_on.png";
		img_position = ccp(20,0);
	}
	
	effect_img = CCSprite::create(filename.c_str());
	effect_img->setPosition(ccpAdd(getContentPosition(kOP_MT_effect), img_position));
	main_case->addChild(effect_img, kOP_Z_content);
}

CCPoint OptionPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kOP_MT_close)				return_value = ccp(400,279);
	else if(t_tag == kOP_MT_bgm)			return_value = ccp(178,227);
	else if(t_tag == kOP_MT_effect)			return_value = ccp(354,227);
	else if(t_tag == kOP_MT_gameui_left)	return_value = ccp(143,113);
	else if(t_tag == kOP_MT_gameui_full)	return_value = ccp(204, 113);
	else if(t_tag == kOP_MT_gameui_right)	return_value = ccp(265, 113);
	else if(t_tag == kOP_MT_help)			return_value = ccp(224,54);
	else if(t_tag == kOP_MT_logout)			return_value = ccp(350, 54);
	else if(t_tag == kOP_MT_noti)			return_value = ccp(117, 54);
	else if(t_tag == kOP_MT_withdraw)		return_value = ccp(356, 120);
	else if(t_tag == kOP_MT_controlJoystickRight)	return_value = ccp(173, 135);
	else if(t_tag == kOP_MT_controlJoystickLeft)	return_value = ccp(230, 135);
	else if(t_tag == kOP_MT_joystickFixed)			return_value = ccp(173, 100);
	else if(t_tag == kOP_MT_joystickMove)			return_value = ccp(230, 100);
	else if(t_tag == kOP_MT_tutorial)		return_value = ccp(200, 180);
	else if(t_tag == kOP_MT_minsu)			return_value = ccp(455,25);
	
	return_value = ccpSub(return_value, ccp(240,160));
	return_value = ccpAdd(return_value, ccp(main_case->getContentSize().width/2.f, main_case->getContentSize().height/2.f));
	
	return return_value;
}