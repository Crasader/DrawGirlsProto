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
#include "KHAlertView.h"
#include "TutorialScene.h"
#include "MinsuScene.h"
#include "KSUtil.h"
#include "ASPopupView.h"
#include "CommonButton.h"
#include "hspConnector.h"
#include "TitleRenewal.h"

#include "KSLabelTTF.h"
#include "PopCake.h"

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
	kOP_MT_joystickPositioning,
	kOP_MT_joystickMoving,
	kOP_MT_tutorial,
	kOP_MT_safety,
	kOP_MT_push,
	kOP_MT_message,
	kOP_MT_coupon,
	kOP_MT_community,
	kOP_MT_tip
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
	
	setTouchEnabled(true);
	
	//MiniGamePopup* t_popup = MiniGamePopup::create((MiniGameCode)(kMiniGameCode_counting), nullptr);
	//addChild(t_popup, 4);

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
	main_case->setPosition(ccp(240,160-450));
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
		temp_menu->setPosition(ccp(15,main_case->getContentSize().height+15));
		main_case->addChild(temp_menu, kOP_Z_content);
		temp_menu->setTouchPriority(-171);
	}
	
	CCSprite* n_popcake = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
	n_popcake->setOpacity(100);
	CCSprite* s_popcake = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
	s_popcake->setOpacity(100);
	
	CCMenuItemLambda* popcake_item = CCMenuItemSpriteLambda::create(n_popcake, s_popcake, [=](CCObject* sender)
																	{
																		CCDirector::sharedDirector()->replaceScene(PopCake::scene());
																	});
	
	CCMenuLambda* popcake_menu = CCMenuLambda::createWithItem(popcake_item);
	popcake_menu->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height+15));
	main_case->addChild(popcake_menu, kOP_Z_content);
	popcake_menu->setTouchPriority(-171);
	
	CommonButton* close_menu = CommonButton::createCloseButton(-171);
	close_menu->setPosition(getContentPosition(kOP_MT_close));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kOP_MT_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kOP_Z_content);
	
	
	CCSprite* n_bgm = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	n_bgm->setOpacity(0);
	CCSprite* s_bgm = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	s_bgm->setOpacity(0);
	
	CCMenuItem* bgm_item = CCMenuItemSprite::create(n_bgm, s_bgm, this, menu_selector(OptionPopup::menuAction));
	bgm_item->setTag(kOP_MT_bgm);
	
	bgm_menu = CCMenu::createWithItem(bgm_item);
	bgm_menu->setPosition(getContentPosition(kOP_MT_bgm));
	main_case->addChild(bgm_menu, kOP_Z_content);
	bgm_menu->setTouchPriority(-171);
	
	bgm_img = NULL;
	resetBgmMenu();
	
	
	CCSprite* n_effect = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	n_effect->setOpacity(0);
	CCSprite* s_effect = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	s_effect->setOpacity(0);
	
	CCMenuItem* effect_item = CCMenuItemSprite::create(n_effect, s_effect, this, menu_selector(OptionPopup::menuAction));
	effect_item->setTag(kOP_MT_effect);
	
	effect_menu = CCMenu::createWithItem(effect_item);
	effect_menu->setPosition(getContentPosition(kOP_MT_effect));
	main_case->addChild(effect_menu, kOP_Z_content);
	effect_menu->setTouchPriority(-171);
	
	effect_img = NULL;
	resetEffectMenu();
	
	
	CCSprite* n_joystick_positioning = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	n_joystick_positioning->setOpacity(0);
	CCSprite* s_joystick_positioning = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	s_joystick_positioning->setOpacity(0);
	
	CCMenuItem* joystick_positioning_item = CCMenuItemSprite::create(n_joystick_positioning, s_joystick_positioning, this, menu_selector(OptionPopup::menuAction));
	joystick_positioning_item->setTag(kOP_MT_joystickPositioning);
	
	joystick_positioning_menu = CCMenu::createWithItem(joystick_positioning_item);
	joystick_positioning_menu->setPosition(getContentPosition(kOP_MT_joystickPositioning));
	main_case->addChild(joystick_positioning_menu, kOP_Z_content);
	joystick_positioning_menu->setTouchPriority(-171);
	
	joystick_positioning_img = NULL;
	resetJoystickPositioningMenu();
	
	
	CCSprite* n_joystick_moving = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	n_joystick_moving->setOpacity(0);
	CCSprite* s_joystick_moving = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	s_joystick_moving->setOpacity(0);
	
	CCMenuItem* joystick_moving_item = CCMenuItemSprite::create(n_joystick_moving, s_joystick_moving, this, menu_selector(OptionPopup::menuAction));
	joystick_moving_item->setTag(kOP_MT_joystickMoving);
	
	joystick_moving_menu = CCMenu::createWithItem(joystick_moving_item);
	joystick_moving_menu->setPosition(getContentPosition(kOP_MT_joystickMoving));
	main_case->addChild(joystick_moving_menu, kOP_Z_content);
	joystick_moving_menu->setTouchPriority(-171);
	
	joystick_moving_img = NULL;
	resetJoystickMovingMenu();
	
	
	CCSprite* n_safety = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	n_safety->setOpacity(0);
	CCSprite* s_safety = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	s_safety->setOpacity(0);
	
	CCMenuItem* safety_item = CCMenuItemSprite::create(n_safety, s_safety, this, menu_selector(OptionPopup::menuAction));
	safety_item->setTag(kOP_MT_safety);
	
	safety_menu = CCMenu::createWithItem(safety_item);
	safety_menu->setPosition(getContentPosition(kOP_MT_safety));
	main_case->addChild(safety_menu, kOP_Z_content);
	safety_menu->setTouchPriority(-171);
	
	safety_img = NULL;
	resetSafetyMenu();
	
	
	CCSprite* n_push = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	n_push->setOpacity(0);
	CCSprite* s_push = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	s_push->setOpacity(0);
	
	CCMenuItem* push_item = CCMenuItemSprite::create(n_push, s_push, this, menu_selector(OptionPopup::menuAction));
	push_item->setTag(kOP_MT_push);
	
	push_menu = CCMenu::createWithItem(push_item);
	push_menu->setPosition(getContentPosition(kOP_MT_push));
	main_case->addChild(push_menu, kOP_Z_content);
	push_menu->setTouchPriority(-171);
	
	push_img = NULL;
	resetPushMenu();
	
	
	CCSprite* n_message = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	n_message->setOpacity(0);
	CCSprite* s_message = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 75, 30));
	s_message->setOpacity(0);
	
	CCMenuItem* message_item = CCMenuItemSprite::create(n_message, s_message, this, menu_selector(OptionPopup::menuAction));
	message_item->setTag(kOP_MT_message);
	
	message_menu = CCMenu::createWithItem(message_item);
	message_menu->setPosition(getContentPosition(kOP_MT_message));
	main_case->addChild(message_menu, kOP_Z_content);
	message_menu->setTouchPriority(-171);
	
	message_img = NULL;
	resetMessageMenu();
	
	
	CCLabelTTF* my_id_label = CCLabelTTF::create(KS::longLongToStr(hspConnector::get()->myKakaoInfo.get("userIndex", 0).asInt64() + 1000).c_str(), mySGD->getFont().c_str(), 11);
	my_id_label->setAnchorPoint(ccp(0,0.5));
	my_id_label->setPosition(ccp(78,50));
	main_case->addChild(my_id_label, kOP_Z_content);
	
	
	CCSprite* n_withdraw = CCSprite::create("option_withdraw.png");
	CCSprite* s_withdraw = CCSprite::create("option_withdraw.png");
	s_withdraw->setColor(ccGRAY);
	
	CCMenuItem* withdraw_item = CCMenuItemSprite::create(n_withdraw, s_withdraw, this, menu_selector(OptionPopup::menuAction));
	withdraw_item->setTag(kOP_MT_withdraw);
	
	CCMenu* withdraw_menu = CCMenu::createWithItem(withdraw_item);
	withdraw_menu->setPosition(getContentPosition(kOP_MT_withdraw));
	main_case->addChild(withdraw_menu, kOP_Z_content);
	withdraw_menu->setTouchPriority(-171);
	
	
	CCSprite* n_logout = CCSprite::create("option_logout.png");
	CCSprite* s_logout = CCSprite::create("option_logout.png");
	s_logout->setColor(ccGRAY);
	
	CCMenuItem* logout_item = CCMenuItemSprite::create(n_logout, s_logout, this, menu_selector(OptionPopup::menuAction));
	logout_item->setTag(kOP_MT_logout);
	
	CCMenu* logout_menu = CCMenu::createWithItem(logout_item);
	logout_menu->setPosition(getContentPosition(kOP_MT_logout));
	main_case->addChild(logout_menu, kOP_Z_content);
	logout_menu->setTouchPriority(-171);
	
	
	CCSprite* n_coupon = CCSprite::create("option_coupon.png");
	CCSprite* s_coupon = CCSprite::create("option_coupon.png");
	s_coupon->setColor(ccGRAY);
	
	CCMenuItem* coupon_item = CCMenuItemSprite::create(n_coupon, s_coupon, this, menu_selector(OptionPopup::menuAction));
	coupon_item->setTag(kOP_MT_coupon);
	
	CCMenu* coupon_menu = CCMenu::createWithItem(coupon_item);
	coupon_menu->setPosition(getContentPosition(kOP_MT_coupon));
	main_case->addChild(coupon_menu, kOP_Z_content);
	coupon_menu->setTouchPriority(-171);
	
	
	CCSprite* n_community = CCSprite::create("option_community.png");
	CCSprite* s_community = CCSprite::create("option_community.png");
	s_community->setColor(ccGRAY);
	
	CCMenuItem* community_item = CCMenuItemSprite::create(n_community, s_community, this, menu_selector(OptionPopup::menuAction));
	community_item->setTag(kOP_MT_community);
	
	CCMenu* community_menu = CCMenu::createWithItem(community_item);
	community_menu->setPosition(getContentPosition(kOP_MT_community));
	main_case->addChild(community_menu, kOP_Z_content);
	community_menu->setTouchPriority(-171);
	
	
	CCSprite* n_help = CCSprite::create("option_help.png");
	CCSprite* s_help = CCSprite::create("option_help.png");
	s_help->setColor(ccGRAY);
	
	CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(OptionPopup::menuAction));
	help_item->setTag(kOP_MT_help);
	
	CCMenu* help_menu = CCMenu::createWithItem(help_item);
	help_menu->setPosition(getContentPosition(kOP_MT_help));
	main_case->addChild(help_menu, kOP_Z_content);
	help_menu->setTouchPriority(-171);
	
	
	CCSprite* n_tip = CCSprite::create("option_gametip.png");
	CCSprite* s_tip = CCSprite::create("option_gametip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(OptionPopup::menuAction));
	tip_item->setTag(kOP_MT_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(getContentPosition(kOP_MT_tip));
	main_case->addChild(tip_menu, kOP_Z_content);
	tip_menu->setTouchPriority(-171);
	
	
	CCSprite* n_tutorial = CCSprite::create("option_tutorial.png");
	CCSprite* s_tutorial = CCSprite::create("option_tutorial.png");
	s_tutorial->setColor(ccGRAY);
	
	CCMenuItem* tutorial_item = CCMenuItemSprite::create(n_tutorial, s_tutorial, this, menu_selector(OptionPopup::menuAction));
	tutorial_item->setTag(kOP_MT_tutorial);
	
	CCMenu* tutorial_menu = CCMenu::createWithItem(tutorial_item);
	tutorial_menu->setPosition(getContentPosition(kOP_MT_tutorial));
	main_case->addChild(tutorial_menu, kOP_Z_content);
	tutorial_menu->setTouchPriority(-171);
	
	
	
//	CCSprite* n_noti = CCSprite::create("option_noti.png");
//	CCSprite* s_noti = CCSprite::create("option_noti.png");
//	s_noti->setColor(ccGRAY);
//	
//	CCMenuItem* noti_item = CCMenuItemSprite::create(n_noti, s_noti, this, menu_selector(OptionPopup::menuAction));
//	noti_item->setTag(kOP_MT_noti);
//	
//	CCMenu* noti_menu = CCMenu::createWithItem(noti_item);
//	noti_menu->setPosition(getContentPosition(kOP_MT_noti));
//	main_case->addChild(noti_menu, kOP_Z_content);
//	noti_menu->setTouchPriority(-171);
	
	
	is_menu_enable = false;
	
	
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
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
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
		AudioEngine::sharedInstance()->setSoundOnOff(!myDSH->getBoolForKey(kDSH_Key_bgmOff));
		resetBgmMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_effect)
	{
		myDSH->setBoolForKey(kDSH_Key_effectOff, !myDSH->getBoolForKey(kDSH_Key_effectOff));
		AudioEngine::sharedInstance()->setEffectOnOff(!myDSH->getBoolForKey(kDSH_Key_effectOff));
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
//		LogoutPopup* t_lp = LogoutPopup::create(this, callfunc_selector(OptionPopup::popupClose));
//		addChild(t_lp, kOP_Z_popup);
		
		ASPopupView* t_popup = ASPopupView::create(-300);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, 160));
		t_popup->setBasePosition(ccp(240, 160));
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		addChild(t_popup, kOP_Z_popup);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setPosition(ccp(0,0));
		t_container->addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(240, 210));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		content_back->setPosition(ccp(0,25));
		t_container->addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(220, 140));
		
		CCLabelTTF* ment_label = CCLabelTTF::create("로그아웃 하시겠습니까?",	mySGD->getFont().c_str(), 15);
		ment_label->setPosition(ccp(0,25));
		t_container->addChild(ment_label);
		
		
		
		CommonButton* cancel_button = CommonButton::create("취소", 15, CCSizeMake(100, 50), CommonButtonGreen, t_popup->getTouchPriority()-5);
		cancel_button->setPosition(ccp(-55,-75));
		cancel_button->setFunction([=](CCObject* sender)
								   {
									   is_menu_enable = true;
									   t_popup->removeFromParent();
								   });
		t_container->addChild(cancel_button);
		
		
		CommonButton* ok_button = CommonButton::create("확인", 15, CCSizeMake(100, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
		ok_button->setPosition(ccp(55,-75));
		ok_button->setFunction([=](CCObject* sender)
							   {
								   cancel_button->setEnabled(false);
								   ok_button->setEnabled(false);
								   
								   cancel_button->setVisible(false);
								   ok_button->setVisible(false);
								   
								   CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
								   CCBReader* reader = new CCBReader(nodeLoader);
								   CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
								   loading_progress_img->setPosition(ccp(0,-75));
								   t_container->addChild(loading_progress_img);
								   reader->release();
								   
								   
								   hspConnector::get()->logout([=](Json::Value result_data)
															   {
																   CCLog("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
																   if(result_data["error"]["isSuccess"].asBool())
																   {
																	   myDSH->clear();
																	   myDSH->resetDSH();
																	   CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
																   }
																   else
																   {
																	   loading_progress_img->removeFromParent();
																	   cancel_button->setEnabled(true);
																	   ok_button->setEnabled(true);
																	   
																	   cancel_button->setVisible(true);
																	   ok_button->setVisible(true);
																	   
																	   CCLog("fail logout");
																   }
															   });
							   });
		t_container->addChild(ok_button);
	}
	else if(tag == kOP_MT_noti)
	{
		NotiPopup* t_np = NotiPopup::create(this, callfunc_selector(OptionPopup::popupClose));
		addChild(t_np, kOP_Z_popup);
	}
	else if(tag == kOP_MT_withdraw)
	{
//		WithdrawPopup* t_wp = WithdrawPopup::create(this, callfunc_selector(OptionPopup::popupClose));
//		addChild(t_wp, kOP_Z_popup);
		
		ASPopupView* t_popup = ASPopupView::create(-300);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, 160));
		t_popup->setBasePosition(ccp(240, 160));
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		addChild(t_popup, kOP_Z_popup);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setPosition(ccp(0,0));
		t_container->addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(280, 250));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		content_back->setPosition(ccp(0,25));
		t_container->addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(260, 180));
		
		CCLabelTTF* ment1_label = CCLabelTTF::create("탈퇴시 게임을 이용하실 수 없으며",	mySGD->getFont().c_str(), 15);
		ment1_label->setPosition(ccp(0,70));
		t_container->addChild(ment1_label);
		
		CCLabelTTF* ment2_label = CCLabelTTF::create("모든 데이터가 영구적으로 삭제됩니다.",	mySGD->getFont().c_str(), 15);
		ment2_label->setPosition(ccp(0,35));
		t_container->addChild(ment2_label);
		
		CCLabelTTF* ment3_label = CCLabelTTF::create("정말 탈퇴하시겠습니까?",	mySGD->getFont().c_str(), 15);
		ment3_label->setPosition(ccp(0,-15));
		t_container->addChild(ment3_label);
		
		
		
		CommonButton* cancel_button = CommonButton::create("취소", 15, CCSizeMake(110, 50), CommonButtonGreen, t_popup->getTouchPriority()-5);
		cancel_button->setPosition(ccp(-65,-95));
		cancel_button->setFunction([=](CCObject* sender)
								   {
									   is_menu_enable = true;
									   t_popup->removeFromParent();
								   });
		t_container->addChild(cancel_button);
		
		
		CommonButton* ok_button = CommonButton::create("확인", 15, CCSizeMake(110, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
		ok_button->setPosition(ccp(65,-95));
		ok_button->setFunction([=](CCObject* sender)
							   {
								   cancel_button->setEnabled(false);
								   ok_button->setEnabled(false);
								   
								   cancel_button->setVisible(false);
								   ok_button->setVisible(false);
								   
								   CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
								   CCBReader* reader = new CCBReader(nodeLoader);
								   CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
								   loading_progress_img->setPosition(ccp(0,-95));
								   t_container->addChild(loading_progress_img);
								   reader->release();
								   
								   Json::Value param;
								   param["memberID"] = hspConnector::get()->getKakaoID();// hspConnector::get()->myKakaoInfo["user_id"].asString(); 기능 제거한다고 하면서 그래프도그가 수정됨
								   hspConnector::get()->command("dropoutuser", param, [=](Json::Value result_data)
																{
																	if(result_data["result"]["code"].asInt() == GDSUCCESS)
																	{
																		myDSH->clear();
																		myDSH->resetDSH();
																		CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
																	}
																	else
																	{
																		loading_progress_img->removeFromParent();
																		cancel_button->setEnabled(true);
																		ok_button->setEnabled(true);
																		
																		cancel_button->setVisible(true);
																		ok_button->setVisible(true);
																		
																		CCLog("fail dropoutuser");
																	}
																});
							   });
		t_container->addChild(ok_button);
	}
	else if(tag == kOP_MT_joystickPositioning)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_left)
			myDSH->setIntegerForKey(kDSH_Key_controlJoystickDirection, kControlJoystickDirection_right);
		else if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_right)
			myDSH->setIntegerForKey(kDSH_Key_controlJoystickDirection, kControlJoystickDirection_left);
		
		resetJoystickPositioningMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_joystickMoving)
	{
		myDSH->setBoolForKey(kDSH_Key_isJoystickCenterNotFixed, !myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed));
		resetJoystickMovingMenu();
		is_menu_enable = true;
		
//		myDSH->setBoolForKey(kDSH_Key_isControlJoystickNotFixed, !myDSH->getBoolForKey(kDSH_Key_isControlJoystickNotFixed));
//		resetJoystickMovingMenu();
//		is_menu_enable = true;
	}
	else if(tag == kOP_MT_tutorial)
	{
		CCDirector::sharedDirector()->replaceScene(TutorialScene::scene());
	}
	else if(tag == kOP_MT_safety)
	{
		myDSH->setBoolForKey(kDSH_Key_isSafetyMode, !myDSH->getBoolForKey(kDSH_Key_isSafetyMode));
		resetSafetyMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_push)
	{
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_message)
	{
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_coupon)
	{
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_community)
	{
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_tip)
	{
		is_menu_enable = true;
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
		img_position = ccp(17,0);
	}
	else
	{
		filename = "option_sound_on.png";
		img_position = ccp(-17,0);
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
		img_position = ccp(17,0);
	}
	else
	{
		filename = "option_sound_on.png";
		img_position = ccp(-17,0);
	}
	
	effect_img = CCSprite::create(filename.c_str());
	effect_img->setPosition(ccpAdd(getContentPosition(kOP_MT_effect), img_position));
	main_case->addChild(effect_img, kOP_Z_content);
}

void OptionPopup::resetJoystickPositioningMenu()
{
	if(joystick_positioning_img)
	{
		joystick_positioning_img->removeFromParent();
		joystick_positioning_img = NULL;
	}
	
	string filename;
	CCPoint img_position;
	if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_right)
	{
		filename = "option_sound_off.png";
		img_position = ccp(17,0);
	}
	else
	{
		filename = "option_sound_on.png";
		img_position = ccp(-17,0);
	}
	
	joystick_positioning_img = CCSprite::create(filename.c_str());
	joystick_positioning_img->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickPositioning), img_position));
	main_case->addChild(joystick_positioning_img, kOP_Z_content);
}

void OptionPopup::resetJoystickMovingMenu()
{
	if(joystick_moving_img)
	{
		joystick_moving_img->removeFromParent();
		joystick_moving_img = NULL;
	}
	
	string filename;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed))//ControlJoystickNotFixed))
	{
		filename = "option_sound_off.png";
		img_position = ccp(17,0);
	}
	else
	{
		filename = "option_sound_on.png";
		img_position = ccp(-17,0);
	}
	
	joystick_moving_img = CCSprite::create(filename.c_str());
	joystick_moving_img->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickMoving), img_position));
	main_case->addChild(joystick_moving_img, kOP_Z_content);
}

void OptionPopup::resetSafetyMenu()
{
	if(safety_img)
	{
		safety_img->removeFromParent();
		safety_img = NULL;
	}
	
	string filename;
	CCPoint img_position;
	if(!myDSH->getBoolForKey(kDSH_Key_isSafetyMode))
	{
		filename = "option_sound_off.png";
		img_position = ccp(17,0);
	}
	else
	{
		filename = "option_sound_on.png";
		img_position = ccp(-17,0);
	}
	
	safety_img = CCSprite::create(filename.c_str());
	safety_img->setPosition(ccpAdd(getContentPosition(kOP_MT_safety), img_position));
	main_case->addChild(safety_img, kOP_Z_content);
}

void OptionPopup::resetPushMenu()
{
	if(push_img)
	{
		push_img->removeFromParent();
		push_img = NULL;
	}
	
	string filename;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_isPushOff))
	{
		filename = "option_sound_off.png";
		img_position = ccp(17,0);
	}
	else
	{
		filename = "option_sound_on.png";
		img_position = ccp(-17,0);
	}
	
	push_img = CCSprite::create(filename.c_str());
	push_img->setPosition(ccpAdd(getContentPosition(kOP_MT_push), img_position));
	main_case->addChild(push_img, kOP_Z_content);
}

void OptionPopup::resetMessageMenu()
{
	if(message_img)
	{
		message_img->removeFromParent();
		message_img = NULL;
	}
	
	string filename;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_isMessageOff))
	{
		filename = "option_sound_off.png";
		img_position = ccp(17,0);
	}
	else
	{
		filename = "option_sound_on.png";
		img_position = ccp(-17,0);
	}
	
	message_img = CCSprite::create(filename.c_str());
	message_img->setPosition(ccpAdd(getContentPosition(kOP_MT_message), img_position));
	main_case->addChild(message_img, kOP_Z_content);
}

CCPoint OptionPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kOP_MT_close)				return_value = ccp(448, 256);
	
	else if(t_tag == kOP_MT_gameui_left)	return_value = ccp(143, 113);
	else if(t_tag == kOP_MT_gameui_full)	return_value = ccp(204, 113);
	else if(t_tag == kOP_MT_gameui_right)	return_value = ccp(265, 113);
	
	else if(t_tag == kOP_MT_bgm)					return_value = ccp(137, 203);
	else if(t_tag == kOP_MT_effect)					return_value = ccp(287, 203);
	else if(t_tag == kOP_MT_joystickPositioning)	return_value = ccp(137, 173);
	else if(t_tag == kOP_MT_joystickMoving)			return_value = ccp(287, 173);
	else if(t_tag == kOP_MT_safety)					return_value = ccp(137, 143);
	else if(t_tag == kOP_MT_push)					return_value = ccp(137, 89);
	else if(t_tag == kOP_MT_message)				return_value = ccp(287, 89);
	
	else if(t_tag == kOP_MT_withdraw)		return_value = ccp(177, 43);
	else if(t_tag == kOP_MT_logout)			return_value = ccp(273, 43);
	
	else if(t_tag == kOP_MT_coupon)			return_value = ccp(400, 214);
	else if(t_tag == kOP_MT_community)		return_value = ccp(400, 170);
	else if(t_tag == kOP_MT_help)			return_value = ccp(400, 126);
	else if(t_tag == kOP_MT_tip)			return_value = ccp(400, 82);
	else if(t_tag == kOP_MT_tutorial)		return_value = ccp(400, 38);
	
	else if(t_tag == kOP_MT_noti)			return_value = ccp(117, 54);
	
	return return_value;
}

bool OptionPopup::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void OptionPopup::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void OptionPopup::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void OptionPopup::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void OptionPopup::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
