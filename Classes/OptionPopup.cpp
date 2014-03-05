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
	kOP_MT_testBack,
	kOP_MT_testIrregularDirection,
	kOP_MT_testDrawButton,
	kOP_MT_testShowJoystick,
	kOP_MT_testCenterCharacter,
	kOP_MT_testLineOver,
	kOP_MT_testJoystickCenterFix,
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
	main_case->setPosition(ccp(205,-160));
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
		temp_menu->setTouchPriority(-171);
	}
	
	
	
	CCSprite* n_close = CCSprite::create("option_small_close.png");
	CCSprite* s_close = CCSprite::create("option_small_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(OptionPopup::menuAction));
	close_item->setTag(kOP_MT_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kOP_MT_close));
	main_case->addChild(close_menu, kOP_Z_content);
	close_menu->setTouchPriority(-171);
	
	
	CCLabelTTF* my_id_label = CCLabelTTF::create(KS::longLongToStr(hspConnector::get()->myKakaoInfo.get("user_id", "").asInt64()).c_str(), mySGD->getFont().c_str(), 13);
	my_id_label->setColor(ccBLACK);
	my_id_label->setPosition(ccp(300,100));
	main_case->addChild(my_id_label, kOP_Z_content);
	
	
	CCSprite* n_help = CCSprite::create("option_help.png");
	CCSprite* s_help = CCSprite::create("option_help.png");
	s_help->setColor(ccGRAY);
	
	CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(OptionPopup::menuAction));
	help_item->setTag(kOP_MT_help);
	
	CCMenu* help_menu = CCMenu::createWithItem(help_item);
	help_menu->setPosition(getContentPosition(kOP_MT_help));
	main_case->addChild(help_menu, kOP_Z_content);
	help_menu->setTouchPriority(-171);
	
	
	CCSprite* n_logout = CCSprite::create("option_logout.png");
	CCSprite* s_logout = CCSprite::create("option_logout.png");
	s_logout->setColor(ccGRAY);
	
	CCMenuItem* logout_item = CCMenuItemSprite::create(n_logout, s_logout, this, menu_selector(OptionPopup::menuAction));
	logout_item->setTag(kOP_MT_logout);
	
	CCMenu* logout_menu = CCMenu::createWithItem(logout_item);
	logout_menu->setPosition(getContentPosition(kOP_MT_logout));
	main_case->addChild(logout_menu, kOP_Z_content);
	logout_menu->setTouchPriority(-171);
	
	
	CCSprite* n_noti = CCSprite::create("option_noti.png");
	CCSprite* s_noti = CCSprite::create("option_noti.png");
	s_noti->setColor(ccGRAY);
	
	CCMenuItem* noti_item = CCMenuItemSprite::create(n_noti, s_noti, this, menu_selector(OptionPopup::menuAction));
	noti_item->setTag(kOP_MT_noti);
	
	CCMenu* noti_menu = CCMenu::createWithItem(noti_item);
	noti_menu->setPosition(getContentPosition(kOP_MT_noti));
	main_case->addChild(noti_menu, kOP_Z_content);
	noti_menu->setTouchPriority(-171);
	
	
	CCSprite* n_withdraw = CCSprite::create("option_withdraw.png");
	CCSprite* s_withdraw = CCSprite::create("option_withdraw.png");
	s_withdraw->setColor(ccGRAY);
	
	CCMenuItem* withdraw_item = CCMenuItemSprite::create(n_withdraw, s_withdraw, this, menu_selector(OptionPopup::menuAction));
	withdraw_item->setTag(kOP_MT_withdraw);
	
	CCMenu* withdraw_menu = CCMenu::createWithItem(withdraw_item);
	withdraw_menu->setPosition(getContentPosition(kOP_MT_withdraw));
	main_case->addChild(withdraw_menu, kOP_Z_content);
	withdraw_menu->setTouchPriority(-171);
	
	
	CCSprite* n_joystick_positioning = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 40));
	n_joystick_positioning->setOpacity(0);
	CCSprite* s_joystick_positioning = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 40));
	s_joystick_positioning->setOpacity(0);
	
	CCMenuItem* joystick_positioning_item = CCMenuItemSprite::create(n_joystick_positioning, s_joystick_positioning, this, menu_selector(OptionPopup::menuAction));
	joystick_positioning_item->setTag(kOP_MT_joystickPositioning);
	
	joystick_positioning_menu = CCMenu::createWithItem(joystick_positioning_item);
	joystick_positioning_menu->setPosition(getContentPosition(kOP_MT_joystickPositioning));
	main_case->addChild(joystick_positioning_menu, kOP_Z_content);
	joystick_positioning_menu->setTouchPriority(-171);
	
	joystick_positioning_img = NULL;
	resetJoystickPositioningMenu();
	
	
	CCSprite* n_joystick_moving = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 40));
	n_joystick_moving->setOpacity(0);
	CCSprite* s_joystick_moving = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 120, 40));
	s_joystick_moving->setOpacity(0);
	
	CCMenuItem* joystick_moving_item = CCMenuItemSprite::create(n_joystick_moving, s_joystick_moving, this, menu_selector(OptionPopup::menuAction));
	joystick_moving_item->setTag(kOP_MT_joystickMoving);
	
	joystick_moving_menu = CCMenu::createWithItem(joystick_moving_item);
	joystick_moving_menu->setPosition(getContentPosition(kOP_MT_joystickMoving));
	main_case->addChild(joystick_moving_menu, kOP_Z_content);
	joystick_moving_menu->setTouchPriority(-171);
	
	joystick_moving_img = NULL;
	resetJoystickMovingMenu();
	
	
	CCSprite* n_tutorial = CCSprite::create("option_tutorial.png");
	CCSprite* s_tutorial = CCSprite::create("option_tutorial.png");
	s_tutorial->setColor(ccGRAY);
	
	KSLabelTTF* ttt = KSLabelTTF::create("the 테스트", mySGD->getFont().c_str(), 16.f);
	ttt->setAnchorPoint(ccp(0.5f, 0.5f));
	ttt->setPosition(ccp(20, 20));
	ttt->setColor(ccc3(0, 255, 0));
	ttt->enableOuterStroke(ccc3(255, 0, 0), 2.f);
	ttt->setString("the 테스트");
	ttt->setString("the 테스트");
	
	n_tutorial->addChild(ttt, 1000000);
	CCMenuItem* tutorial_item = CCMenuItemSprite::create(n_tutorial, s_tutorial, this, menu_selector(OptionPopup::menuAction));
	tutorial_item->setTag(kOP_MT_tutorial);
	
	CCMenu* tutorial_menu = CCMenu::createWithItem(tutorial_item);
	tutorial_menu->setPosition(getContentPosition(kOP_MT_tutorial));
	main_case->addChild(tutorial_menu, kOP_Z_content);
	tutorial_menu->setTouchPriority(-171);
	
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
	bgm_menu->setTouchPriority(-171);
	
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
	effect_menu->setTouchPriority(-171);
	
	effect_img = NULL;
	resetEffectMenu();
	
	CCSprite* test_option_back = CCSprite::create("test_option_back.png");
	test_option_back->setPosition(getContentPosition(kOP_MT_testBack));
	main_case->addChild(test_option_back, kOP_Z_content);
	
	irregular_direction_menu = NULL;
	resetIrregularDirection();
	
	draw_button_menu = NULL;
	resetDrawButton();
	
	show_joystick_menu = NULL;
	resetShowJoystick();
	
	center_character_menu = NULL;
	resetCenterCharacter();
	
	line_over_menu = NULL;
	resetLineOver();
	
	CCLabelTTF* test_joystick_center_fix_label = CCLabelTTF::create("조이스틱 터치 후\n기준점 이동가능", mySGD->getFont().c_str(), 11);
	test_joystick_center_fix_label->setColor(ccBLACK);
	test_joystick_center_fix_label->setPosition(ccpAdd(getContentPosition(kOP_MT_testJoystickCenterFix), ccp(-80,0)));
	main_case->addChild(test_joystick_center_fix_label, kOP_Z_content);
	
	joystick_center_fix_menu = NULL;
	resetJoystickCenterFix();
	
	is_menu_enable = false;

	//KHAlertView* av = KHAlertView::create(); av->setTitleFileName("msg_challenge.png");
	//av->setCloseButton(CCMenuItemImageLambda::create("cardchange_cancel.png", "cardchange_cancel.png",
				//[=](CCObject*){
				//}
																									 //));
	//av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
	//av->setWidth(240);
	//av->setHeight(240);
	//av->setTitleHeight(10);
	//av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
	//av->setCenterY(150);

	//CCNode* emptyNode = CCNode::create();
	//auto ttf = CCLabelTTF::create("asdasda", "", 12.f); 
	//ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
	////	con->setAnchorPoint(ccp(0, 0));
	////ttf->setAnchorPoint(ccp(0.5f, 0.5f));
	//ttf->setColor(ccc3(255, 255, 255));
	//ttf->setPosition(ccp(av->getContentRect().size.width / 2.f, ttf->getPositionY() + 10));
	//emptyNode->addChild(ttf);
	//av->setContentNode(
		//emptyNode
			//);
	//av->setContentSize(ttf->getDimensions());
	//av->addButton(CommonButton::create("거절", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
								//[=](CCObject* e)
								//{
									//CCLog("거절!!");
								//});
	//av->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
								//[=](CCObject* e)
								//{
									//CCLog("ok!!");
								//});
	//addChild(av, 99999999);
	//av->show();
	
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
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(210,160));
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
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(210,-160));
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
								   param["memberID"] = hspConnector::get()->myKakaoInfo["user_id"].asString();
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
		myDSH->setBoolForKey(kDSH_Key_isControlJoystickNotFixed, !myDSH->getBoolForKey(kDSH_Key_isControlJoystickNotFixed));
		resetJoystickMovingMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_tutorial)
	{
		CCDirector::sharedDirector()->replaceScene(TutorialScene::scene());
	}
	else if(tag == kOP_MT_testIrregularDirection)
	{
		myDSH->setBoolForKey(kDSH_Key_isEnableIrregularDirection, !myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection));
		resetIrregularDirection();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_testDrawButton)
	{
		myDSH->setBoolForKey(kDSH_Key_isDisableDrawButton, !myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton));
		resetDrawButton();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_testShowJoystick)
	{
		myDSH->setBoolForKey(kDSH_Key_isAlwaysInvisibleJoystick, !myDSH->getBoolForKey(kDSH_Key_isAlwaysInvisibleJoystick));
		resetShowJoystick();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_testCenterCharacter)
	{
		myDSH->setBoolForKey(kDSH_Key_isAlwaysCenterCharacter, !myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter));
		resetCenterCharacter();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_testLineOver)
	{
		myDSH->setBoolForKey(kDSH_Key_isDisableLineOver, !myDSH->getBoolForKey(kDSH_Key_isDisableLineOver));
		resetLineOver();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_testJoystickCenterFix)
	{
		myDSH->setBoolForKey(kDSH_Key_isJoystickCenterNotFixed, !myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed));
		resetJoystickCenterFix();
		is_menu_enable = true;
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

void OptionPopup::resetJoystickPositioningMenu()
{
	if(joystick_positioning_img)
	{
		joystick_positioning_img->removeFromParent();
		joystick_positioning_img = NULL;
	}
	
	string filename;
	CCPoint img_position;
	if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_left)
	{
		filename = "option_mode_left.png";
		img_position = ccp(-20,0);
	}
	else
	{
		filename = "option_mode_right.png";
		img_position = ccp(20,0);
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
	if(!myDSH->getBoolForKey(kDSH_Key_isControlJoystickNotFixed))
	{
		filename = "option_mode_lock.png";
		img_position = ccp(-20,0);
	}
	else
	{
		filename = "option_mode_move.png";
		img_position = ccp(20,0);
	}
	
	joystick_moving_img = CCSprite::create(filename.c_str());
	joystick_moving_img->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickMoving), img_position));
	main_case->addChild(joystick_moving_img, kOP_Z_content);
}

void OptionPopup::resetIrregularDirection()
{
	if(irregular_direction_menu)
	{
		irregular_direction_menu->removeFromParent();
		irregular_direction_menu = NULL;
	}
	
	string filename;
	if(!myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection))
		filename = "test_option_off.png";
	else
		filename = "test_option_on.png";
	
	CCSprite* n_irregular = CCSprite::create(filename.c_str());
	CCSprite* s_irregular = CCSprite::create(filename.c_str());
	s_irregular->setColor(ccGRAY);
	
	CCMenuItem* irregular_item = CCMenuItemSprite::create(n_irregular, s_irregular, this, menu_selector(OptionPopup::menuAction));
	irregular_item->setTag(kOP_MT_testIrregularDirection);
	
	irregular_direction_menu = CCMenu::createWithItem(irregular_item);
	irregular_direction_menu->setPosition(getContentPosition(kOP_MT_testIrregularDirection));
	main_case->addChild(irregular_direction_menu, kOP_Z_content);
	irregular_direction_menu->setTouchPriority(-171);
}
void OptionPopup::resetDrawButton()
{
	if(draw_button_menu)
	{
		draw_button_menu->removeFromParent();
		draw_button_menu = NULL;
	}
	
	string filename;
	if(myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton))
		filename = "test_option_off.png";
	else
		filename = "test_option_on.png";
	
	CCSprite* n_draw_button = CCSprite::create(filename.c_str());
	CCSprite* s_draw_button = CCSprite::create(filename.c_str());
	s_draw_button->setColor(ccGRAY);
	
	CCMenuItem* draw_button_item = CCMenuItemSprite::create(n_draw_button, s_draw_button, this, menu_selector(OptionPopup::menuAction));
	draw_button_item->setTag(kOP_MT_testDrawButton);
	
	draw_button_menu = CCMenu::createWithItem(draw_button_item);
	draw_button_menu->setPosition(getContentPosition(kOP_MT_testDrawButton));
	main_case->addChild(draw_button_menu, kOP_Z_content);
	draw_button_menu->setTouchPriority(-171);
}
void OptionPopup::resetShowJoystick()
{
	if(show_joystick_menu)
	{
		show_joystick_menu->removeFromParent();
		show_joystick_menu = NULL;
	}
	
	string filename;
	if(myDSH->getBoolForKey(kDSH_Key_isAlwaysInvisibleJoystick))
		filename = "test_option_off.png";
	else
		filename = "test_option_on.png";
	
	CCSprite* n_show_joystick = CCSprite::create(filename.c_str());
	CCSprite* s_show_joystick = CCSprite::create(filename.c_str());
	s_show_joystick->setColor(ccGRAY);
	
	CCMenuItem* show_joystick_item = CCMenuItemSprite::create(n_show_joystick, s_show_joystick, this, menu_selector(OptionPopup::menuAction));
	show_joystick_item->setTag(kOP_MT_testShowJoystick);
	
	show_joystick_menu = CCMenu::createWithItem(show_joystick_item);
	show_joystick_menu->setPosition(getContentPosition(kOP_MT_testShowJoystick));
	main_case->addChild(show_joystick_menu, kOP_Z_content);
	show_joystick_menu->setTouchPriority(-171);
}
void OptionPopup::resetCenterCharacter()
{
	if(center_character_menu)
	{
		center_character_menu->removeFromParent();
		center_character_menu = NULL;
	}
	
	string filename;
	if(!myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
		filename = "test_option_off.png";
	else
		filename = "test_option_on.png";
	
	CCSprite* n_center_character = CCSprite::create(filename.c_str());
	CCSprite* s_center_character = CCSprite::create(filename.c_str());
	s_center_character->setColor(ccGRAY);
	
	CCMenuItem* center_character_item = CCMenuItemSprite::create(n_center_character, s_center_character, this, menu_selector(OptionPopup::menuAction));
	center_character_item->setTag(kOP_MT_testCenterCharacter);
	
	center_character_menu = CCMenu::createWithItem(center_character_item);
	center_character_menu->setPosition(getContentPosition(kOP_MT_testCenterCharacter));
	main_case->addChild(center_character_menu, kOP_Z_content);
	center_character_menu->setTouchPriority(-171);
}
void OptionPopup::resetLineOver()
{
	if(line_over_menu)
	{
		line_over_menu->removeFromParent();
		line_over_menu = NULL;
	}
	
	string filename;
	if(myDSH->getBoolForKey(kDSH_Key_isDisableLineOver))
		filename = "test_option_off.png";
	else
		filename = "test_option_on.png";
	
	CCSprite* n_line_over = CCSprite::create(filename.c_str());
	CCSprite* s_line_over = CCSprite::create(filename.c_str());
	s_line_over->setColor(ccGRAY);
	
	CCMenuItem* line_over_item = CCMenuItemSprite::create(n_line_over, s_line_over, this, menu_selector(OptionPopup::menuAction));
	line_over_item->setTag(kOP_MT_testLineOver);
	
	line_over_menu = CCMenu::createWithItem(line_over_item);
	line_over_menu->setPosition(getContentPosition(kOP_MT_testLineOver));
	main_case->addChild(line_over_menu, kOP_Z_content);
	line_over_menu->setTouchPriority(-171);
}
void OptionPopup::resetJoystickCenterFix()
{
	if(joystick_center_fix_menu)
	{
		joystick_center_fix_menu->removeFromParent();
		joystick_center_fix_menu = NULL;
	}
	
	string filename;
	if(myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed))
		filename = "test_option_off.png";
	else
		filename = "test_option_on.png";
	
	CCSprite* n_joystick_center_fix = CCSprite::create(filename.c_str());
	CCSprite* s_joystick_center_fix = CCSprite::create(filename.c_str());
	s_joystick_center_fix->setColor(ccGRAY);
	
	CCMenuItem* joystick_center_fix_item = CCMenuItemSprite::create(n_joystick_center_fix, s_joystick_center_fix, this, menu_selector(OptionPopup::menuAction));
	joystick_center_fix_item->setTag(kOP_MT_testJoystickCenterFix);
	
	joystick_center_fix_menu = CCMenu::createWithItem(joystick_center_fix_item);
	joystick_center_fix_menu->setPosition(getContentPosition(kOP_MT_testJoystickCenterFix));
	main_case->addChild(joystick_center_fix_menu, kOP_Z_content);
	joystick_center_fix_menu->setTouchPriority(-171);
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
	else if(t_tag == kOP_MT_joystickPositioning)	return_value = ccp(202, 135);
	else if(t_tag == kOP_MT_joystickMoving)			return_value = ccp(202, 100);
	else if(t_tag == kOP_MT_tutorial)		return_value = ccp(200, 180);
	
	else if(t_tag == kOP_MT_testBack)		return_value = ccp(460,160);
	else if(t_tag == kOP_MT_testIrregularDirection)	return_value = ccp(460,270);
	else if(t_tag == kOP_MT_testDrawButton)			return_value = ccp(460,210);
	else if(t_tag == kOP_MT_testShowJoystick)		return_value = ccp(460,150);
	else if(t_tag == kOP_MT_testCenterCharacter)	return_value = ccp(460,90);
	else if(t_tag == kOP_MT_testLineOver)			return_value = ccp(460,30);
	
	else if(t_tag == kOP_MT_testJoystickCenterFix)	return_value = ccp(377, 170);
	
	else if(t_tag == kOP_MT_minsu)			return_value = ccp(455,25);
	
	return_value = ccpSub(return_value, ccp(240,160));
	return_value = ccpAdd(return_value, ccp(main_case->getContentSize().width/2.f, main_case->getContentSize().height/2.f));
	
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
