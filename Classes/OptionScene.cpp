//
//  OptionScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 29..
//
//

#include "OptionScene.h"
#include "ScreenSide.h"
#include "MyLocalization.h"
//#include "WorldMapScene.h"
#include "PuzzleMapScene.h"
#include "HelpPopup.h"
#include "LogoutPopup.h"
#include "NotiPopup.h"
#include "WithdrawPopup.h"
#include "PuzzleCache.h"

#include "cocos-ext.h"
#include "StageImgLoader.h"
#include "KSAlertView.h"


USING_NS_CC_EXT;

CCScene* OptionScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    OptionScene *layer = OptionScene::create();
	
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

enum OptionZorder{
	kO_Z_back = 1,
	kO_Z_content,
	kO_Z_popup
};

enum OptionMenuTag{
	kO_MT_close = 1,
	kO_MT_bgm,
	kO_MT_effect,
	kO_MT_gameui_left,
	kO_MT_gameui_full,
	kO_MT_gameui_right,
	kO_MT_help,
	kO_MT_logout,
	kO_MT_noti,
	kO_MT_withdraw
};

bool OptionScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	setKeypadEnabled(true);
	

	CCSprite* back = CCSprite::create("option_back.png");
	back->setPosition(ccp(240,160));
	addChild(back, kO_Z_back);
	
	if(myDSH->isCheatKeyEnable())
	{
		CCSprite* n_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		n_temp->setOpacity(0);
		CCSprite* s_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		s_temp->setOpacity(0);
		
		CCMenuItemSprite* temp_item = CCMenuItemSprite::create(n_temp, s_temp, this, menu_selector(OptionScene::cheatAction));
		temp_item->setTag(1);
		CCMenu* temp_menu = CCMenu::createWithItem(temp_item);
		temp_menu->setPosition(ccp(15,305));
		addChild(temp_menu, kO_Z_content);
	}
	
	
	
	CCSprite* n_close = CCSprite::create("option_small_close.png");
	CCSprite* s_close = CCSprite::create("option_small_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(OptionScene::menuAction));
	close_item->setTag(kO_MT_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kO_MT_close));
	addChild(close_menu, kO_Z_content);

	
//	CCSprite* n_left = CCSprite::create("ui_setting_left.png");
//	CCSprite* s_left = CCSprite::create("ui_setting_left.png");
//	s_left->setColor(ccGRAY);
//	
//	CCMenuItem* left_item = CCMenuItemSprite::create(n_left, s_left, this, menu_selector(OptionScene::menuAction));
//	left_item->setTag(kO_MT_gameui_left);
//	
//	CCMenu* left_menu = CCMenu::createWithItem(left_item);
//	left_menu->setPosition(getContentPosition(kO_MT_gameui_left));
//	addChild(left_menu, kO_Z_content);
//	
//	
//	CCSprite* n_full = CCSprite::create("ui_setting_full.png");
//	CCSprite* s_full = CCSprite::create("ui_setting_full.png");
//	s_full->setColor(ccGRAY);
//	
//	CCMenuItem* full_item = CCMenuItemSprite::create(n_full, s_full, this, menu_selector(OptionScene::menuAction));
//	full_item->setTag(kO_MT_gameui_full);
//	
//	CCMenu* full_menu = CCMenu::createWithItem(full_item);
//	full_menu->setPosition(getContentPosition(kO_MT_gameui_full));
//	addChild(full_menu, kO_Z_content);
//	
//	
//	CCSprite* n_right = CCSprite::create("ui_setting_right.png");
//	CCSprite* s_right = CCSprite::create("ui_setting_right.png");
//	s_right->setColor(ccGRAY);
//	
//	CCMenuItem* right_item = CCMenuItemSprite::create(n_right, s_right, this, menu_selector(OptionScene::menuAction));
//	right_item->setTag(kO_MT_gameui_right);
//	
//	CCMenu* right_menu = CCMenu::createWithItem(right_item);
//	right_menu->setPosition(getContentPosition(kO_MT_gameui_right));
//	addChild(right_menu, kO_Z_content);
	
	
	CCSprite* n_help = CCSprite::create("option_help.png");
	CCSprite* s_help = CCSprite::create("option_help.png");
	s_help->setColor(ccGRAY);
	
	CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(OptionScene::menuAction));
	help_item->setTag(kO_MT_help);
	
	CCMenu* help_menu = CCMenu::createWithItem(help_item);
	help_menu->setPosition(getContentPosition(kO_MT_help));
	addChild(help_menu, kO_Z_content);
	
	
	CCSprite* n_logout = CCSprite::create("option_logout.png");
	CCSprite* s_logout = CCSprite::create("option_logout.png");
	s_logout->setColor(ccGRAY);
	
	CCMenuItem* logout_item = CCMenuItemSprite::create(n_logout, s_logout, this, menu_selector(OptionScene::menuAction));
	logout_item->setTag(kO_MT_logout);
	
	CCMenu* logout_menu = CCMenu::createWithItem(logout_item);
	logout_menu->setPosition(getContentPosition(kO_MT_logout));
	addChild(logout_menu, kO_Z_content);
	
	
	CCSprite* n_noti = CCSprite::create("option_noti.png");
	CCSprite* s_noti = CCSprite::create("option_noti.png");
	s_noti->setColor(ccGRAY);
	
	CCMenuItem* noti_item = CCMenuItemSprite::create(n_noti, s_noti, this, menu_selector(OptionScene::menuAction));
	noti_item->setTag(kO_MT_noti);
	
	CCMenu* noti_menu = CCMenu::createWithItem(noti_item);
	noti_menu->setPosition(getContentPosition(kO_MT_noti));
	addChild(noti_menu, kO_Z_content);
	
	
	CCSprite* n_withdraw = CCSprite::create("option_withdraw.png");
	CCSprite* s_withdraw = CCSprite::create("option_withdraw.png");
	s_withdraw->setColor(ccGRAY);
	
	CCMenuItem* withdraw_item = CCMenuItemSprite::create(n_withdraw, s_withdraw, this, menu_selector(OptionScene::menuAction));
	withdraw_item->setTag(kO_MT_withdraw);
	
	CCMenu* withdraw_menu = CCMenu::createWithItem(withdraw_item);
	withdraw_menu->setPosition(getContentPosition(kO_MT_withdraw));
	addChild(withdraw_menu, kO_Z_content);
	
	
	CCSprite* n_bgm = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 94, 40));
	n_bgm->setOpacity(0);
	CCSprite* s_bgm = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 94, 40));
	s_bgm->setOpacity(0);
	
	CCMenuItem* bgm_item = CCMenuItemSprite::create(n_bgm, s_bgm, this, menu_selector(OptionScene::menuAction));
	bgm_item->setTag(kO_MT_bgm);
	
	bgm_menu = CCMenu::createWithItem(bgm_item);
	bgm_menu->setPosition(getContentPosition(kO_MT_bgm));
	addChild(bgm_menu, kO_Z_content);
	
	bgm_img = NULL;
	resetBgmMenu();
	
	
	CCSprite* n_effect = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 94, 40));
	n_effect->setOpacity(0);
	CCSprite* s_effect = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 94, 40));
	s_effect->setOpacity(0);
	
	CCMenuItem* effect_item = CCMenuItemSprite::create(n_effect, s_effect, this, menu_selector(OptionScene::menuAction));
	effect_item->setTag(kO_MT_effect);
	
	effect_menu = CCMenu::createWithItem(effect_item);
	effect_menu->setPosition(getContentPosition(kO_MT_effect));
	addChild(effect_menu, kO_Z_content);
	
	effect_img = NULL;
	resetEffectMenu();
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	

//	KSAlertView* av = KSAlertView::create();
//	
//	av->setVScroll(CCScale9Sprite::create("popup_bar_v.png", CCRectMake(0, 0, 23, 53),
//																				CCRectMake(7, 7, 23 - 7*2, 53 - 7*2 - 4)));
//	av->setHScroll(CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
//																				CCRectMake(10, 7, 53 - 10*2, 23 - 7*2)));
//	auto m1 = CCMenuItemImageLambda::create("ui_common_ok.png", "ui_common_ok.png",
//																					[](CCObject* e){
//																						CCLog("press!!");
//																					});
//	
//	av->addButton(m1);
//	av->addButton(CCMenuItemImageLambda::create("ticket_send.png", "ticket_send.png",
//																							[](CCObject* e){
//																								CCLog("press!!");
//																							}));
//	addChild(av, kO_MT_withdraw);
//	av->show();
	return true;
}

void OptionScene::cheatAction(CCObject *sender)
{
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == 1) // gold up 10000
	{
		myDSH->setIntegerForKey(kDSH_Key_savedGold, myDSH->getIntegerForKey(kDSH_Key_savedGold)+10000);
	}
}

void OptionScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kO_MT_close)
	{
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
	else if(tag == kO_MT_bgm)
	{
		myDSH->setBoolForKey(kDSH_Key_bgmOff, !myDSH->getBoolForKey(kDSH_Key_bgmOff));
		resetBgmMenu();
		is_menu_enable = true;
	}
	else if(tag == kO_MT_effect)
	{
		myDSH->setBoolForKey(kDSH_Key_effectOff, !myDSH->getBoolForKey(kDSH_Key_effectOff));
		resetEffectMenu();
		is_menu_enable = true;
	}
	else if(tag == kO_MT_gameui_left)
	{
		myGD->setUItype(kGT_leftUI);
		is_menu_enable = true;
	}
	else if(tag == kO_MT_gameui_full)
	{
		myGD->setUItype(kGT_full);
		is_menu_enable = true;
	}
	else if(tag == kO_MT_gameui_right)
	{
		myGD->setUItype(kGT_rightUI);
		is_menu_enable = true;
	}
	else if(tag == kO_MT_help)
	{
		HelpPopup* t_hp = HelpPopup::create(this, callfunc_selector(OptionScene::popupClose));
		addChild(t_hp, kO_Z_popup);
	}
	else if(tag == kO_MT_logout)
	{
		LogoutPopup* t_lp = LogoutPopup::create(this, callfunc_selector(OptionScene::popupClose));
		addChild(t_lp, kO_Z_popup);
	}
	else if(tag == kO_MT_noti)
	{
		NotiPopup* t_np = NotiPopup::create(this, callfunc_selector(OptionScene::popupClose));
		addChild(t_np, kO_Z_popup);
	}
	else if(tag == kO_MT_withdraw)
	{
		WithdrawPopup* t_wp = WithdrawPopup::create(this, callfunc_selector(OptionScene::popupClose));
		addChild(t_wp, kO_Z_popup);
	}
}

void OptionScene::popupClose()
{
	is_menu_enable = true;
}

void OptionScene::resetBgmMenu()
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
	bgm_img->setPosition(ccpAdd(getContentPosition(kO_MT_bgm), img_position));
	addChild(bgm_img, kO_Z_content);
}

void OptionScene::resetEffectMenu()
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
	effect_img->setPosition(ccpAdd(getContentPosition(kO_MT_effect), img_position));
	addChild(effect_img, kO_Z_content);
}

CCPoint OptionScene::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kO_MT_close)				return_value = ccp(400,270);
	else if(t_tag == kO_MT_bgm)				return_value = ccp(178,219);
	else if(t_tag == kO_MT_effect)			return_value = ccp(354,219);
	else if(t_tag == kO_MT_gameui_left)		return_value = ccp(143,113);
	else if(t_tag == kO_MT_gameui_full)		return_value = ccp(204, 113);
	else if(t_tag == kO_MT_gameui_right)	return_value = ccp(265, 113);
	else if(t_tag == kO_MT_help)			return_value = ccp(224,70);
	else if(t_tag == kO_MT_logout)			return_value = ccp(350, 70);
	else if(t_tag == kO_MT_noti)			return_value = ccp(117, 70);
	else if(t_tag == kO_MT_withdraw)		return_value = ccp(356, 125);
	
	return return_value;
}

void OptionScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void OptionScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(OptionScene::alertAction));
}