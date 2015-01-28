//
//  StageSettingPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 19..
//
//

#include "StageSettingPopup.h"

#include "MyLocalization.h"
#include "MaingameScene.h"
#include "ItemBuyPopup.h"
#include "CardSettingPopup.h"
#include "GachaPopup.h"
//#include "DurabilityNoti.h"
#include "CardCase.h"
#include "DownloadFile.h"
#include "ListViewerScroll.h"
#include "HeartTime.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "ItemListViewer.h"
#include "hspConnector.h"
#include "StageInfoPopup.h"
#include "StageRankPopup.h"
#include "LoadingLayer.h"
#include "ShopPopup.h"
#include "KnownFriend.h"
#include "UnknownFriends.h"
#include <random>
#include "ASPopupView.h"
#include "ShowItemContent.h"

enum SSP_Zorder{
	kSSP_Z_gray = 1,
	kSSP_Z_back,
	kSSP_Z_selectedImg,
	kSSP_Z_content,
	kSSP_Z_top,
	kSSP_Z_popup
};

enum CARD_Zorder{
	kCARD_Z_ani = 1,
	kCARD_Z_cardCase
};

void StageSettingPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool StageSettingPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	is_menu_enable = false;
	
	friend_point_popup = NULL;
	
	selected_stage = mySD->getSilType();
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kSSP_Z_gray);
	
	main_case = CCSprite::create("stagesetting_back.png");
	main_case->setAnchorPoint(ccp(0,0));
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kSSP_Z_back);
	
	CCLabelTTF* stage_label = CCLabelTTF::create(CCString::createWithFormat("STAGE %d - %d", NSDS_GI(selected_stage, kSDS_SI_puzzle_i), selected_stage)->getCString(), mySGD->getFont().c_str(), 13);
	stage_label->setPosition(ccp(250,248));
	main_case->addChild(stage_label, kSSP_Z_content);
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard); // 1, 2, 3 / 11, 12, 13 / 14, ...
	if(selected_card_number > 0 && mySGD->isHasGottenCards(selected_card_number) > 0)
	{
		CCSprite* card_img = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png", selected_card_number)->getCString());
		card_img->setScale(0.45);
		card_img->setPosition(getContentPosition(kSSP_MT_changeCard));
		main_case->addChild(card_img, kSSP_Z_content);
		
		if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, selected_card_number))
		{
			CCSize ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, selected_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, selected_card_number));
			CCSprite* card_ani = mySIL->getLoadedImg(CCString::createWithFormat("card%d_animation.png", selected_card_number)->getCString(),
													 CCRectMake(0, 0, ani_size.width, ani_size.height));
			card_ani->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, selected_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, selected_card_number)));
			card_img->addChild(card_ani, kCARD_Z_ani);
		}
		
		CardCase* t_case = CardCase::create(selected_card_number);
		t_case->setPosition(CCPointZero);
		card_img->addChild(t_case, kCARD_Z_cardCase);
	}
	else
	{
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
	}
	
	int ability_cnt = NSDS_GI(kSDS_CI_int1_abilityCnt_i, selected_card_number);
	
	for(int i=0;i<ability_cnt;i++)
		card_options.push_back(NSDS_GI(kSDS_CI_int1_ability_int2_type_i, selected_card_number, i));
	
	CCSprite* n_gacha = CCSprite::create("stagesetting_item_gacha_on.png");
	CCSprite* s_gacha = CCSprite::create("stagesetting_item_gacha_on.png");
	s_gacha->setColor(ccGRAY);
	
	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(StageSettingPopup::menuAction));
	gacha_item->setTag(kSSP_MT_gacha);
	
	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
	gacha_menu->setPosition(getContentPosition(kSSP_MT_gacha));
	main_case->addChild(gacha_menu, kSSP_Z_content);
	
	
	my_ilv = ItemListViewer::create();
	main_case->addChild(my_ilv, kSSP_Z_content);
	
	item_list = mySD->getStageItemList(selected_stage);
	
	for(int i=0;i<item_list.size();i++)
	{
		ITEM_CODE t_code = item_list[i];
		if(!myDSH->getBoolForKey(kDSH_Key_isShowItem_int1, t_code))
		{
			show_item_popup.push_back(t_code);
			myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, t_code, myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_code)+mySGD->getBonusItemCnt(t_code));
			myDSH->setBoolForKey(kDSH_Key_isShowItem_int1, t_code, true);
		}
	}
//	myDSH->saveUserData({kSaveUserData_Key_item}, nullptr);
	
	if(!show_item_popup.empty())
	{
		ASPopupView* t_popup = ASPopupView::create(-200);
		
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
		
		ShowItemContent* t_container = ShowItemContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
		{
			t_popup->removeFromParent();
		}, show_item_popup);
		t_popup->setContainerNode(t_container);
		addChild(t_popup, kSSP_Z_popup);
	}
	
	itemSetting();
	
	my_ilv->setMaxPositionY();
	
	t_lvs = ListViewerScroll::create(CCRectMake(430, 80, 27, 138), my_ilv, "stagesetting_scroll.png", ccp(443,102), ccp(443,195));
	main_case->addChild(t_lvs, kSSP_Z_content);
	
	my_ilv->setScroll(t_lvs);
	
	CCSprite* n_cardmenu = CCSprite::create("cardsetting_cardmenu_big.png", CCRectMake(0, 0, 320*0.45f, 430*0.45f));
	CCSprite* s_cardmenu = CCSprite::create("cardsetting_cardmenu_big.png", CCRectMake(0, 0, 320*0.45f, 430*0.45f));
	
	CCMenuItem* changeCard_item = CCMenuItemSprite::create(n_cardmenu, s_cardmenu, this, menu_selector(StageSettingPopup::menuAction));
	changeCard_item->setTag(kSSP_MT_changeCard);
	
	CCMenu* change_menu = CCMenu::createWithItem(changeCard_item);
	change_menu->setPosition(getContentPosition(kSSP_MT_changeCard));
	main_case->addChild(change_menu, kSSP_Z_content);
	
	
	CCSprite* n_backward = CCSprite::create("stagesetting_backward.png");
	CCSprite* s_backward = CCSprite::create("stagesetting_backward.png");
	s_backward->setColor(ccGRAY);
	
	CCMenuItem* backward_item = CCMenuItemSprite::create(n_backward, s_backward, this, menu_selector(StageSettingPopup::menuAction));
	backward_item->setTag(kSSP_MT_back);
	
	CCMenu* backward_menu = CCMenu::createWithItem(backward_item);
	backward_menu->setPosition(getContentPosition(kSSP_MT_back));
	main_case->addChild(backward_menu, kSSP_Z_content);
	
	
	CCSprite* n_info = CCSprite::create("stagesetting_stageinfo.png");
	CCSprite* s_info = CCSprite::create("stagesetting_stageinfo.png");
	s_info->setColor(ccGRAY);
	
	CCMenuItem* info_item = CCMenuItemSprite::create(n_info, s_info, this, menu_selector(StageSettingPopup::menuAction));
	info_item->setTag(kSSP_MT_info);
	
	CCMenu* info_menu = CCMenu::createWithItem(info_item);
	info_menu->setPosition(getContentPosition(kSSP_MT_info));
	main_case->addChild(info_menu, kSSP_Z_content);
	
//	if(!mySGD->getIsAcceptChallenge() && !mySGD->getIsAcceptHelp())
//	{
		CCSprite* n_challenge = CCSprite::create("stagesetting_challenge.png");
		CCSprite* s_challenge = CCSprite::create("stagesetting_challenge.png");
		s_challenge->setColor(ccGRAY);
		
		CCMenuItem* challenge_item = CCMenuItemSprite::create(n_challenge, s_challenge, this, menu_selector(StageSettingPopup::menuAction));
		challenge_item->setTag(kSSP_MT_challenge);
		
		CCMenu* challenge_menu = CCMenu::createWithItem(challenge_item);
		challenge_menu->setPosition(getContentPosition(kSSP_MT_challenge));
		main_case->addChild(challenge_menu, kSSP_Z_content);
//	}
	
	
	CCSprite* n_start = CCSprite::create("stagesetting_start.png");
	CCSprite* s_start = CCSprite::create("stagesetting_start.png");
	s_start->setColor(ccGRAY);
	
	CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(StageSettingPopup::menuAction));
	start_item->setTag(kSSP_MT_start);
	
	CCMenu* start_menu = CCMenu::createWithItem(start_item);
	start_menu->setPosition(getContentPosition(kSSP_MT_start));
	main_case->addChild(start_menu, kSSP_Z_content);
	
	if(myDSH->isCheatKeyEnable())
	{
		CCSprite* n_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		n_temp->setOpacity(0);
		CCSprite* s_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		s_temp->setOpacity(0);
		
		CCMenuItemSprite* temp_item = CCMenuItemSprite::create(n_temp, s_temp, this, menu_selector(StageSettingPopup::cheatAction));
		temp_item->setTag(1);
		CCMenu* temp_menu = CCMenu::createWithItem(temp_item);
		temp_menu->setPosition(ccp(15,305));
		main_case->addChild(temp_menu, kSSP_Z_content);
	}
	
	top_case = CCSprite::create("test_ui_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f + 33.f));
	addChild(top_case, kSSP_Z_top);
	
	CountingBMLabel* ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_ruby))->getCString(), "allfont.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(94,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CountingBMLabel* gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_gold))->getCString(), "allfont.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(185,top_case->getContentSize().height/2.f));
	top_case->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(250,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time, 0, kSSP_MT_heartTime);
	
	
	CCSprite* n_ruby = CCSprite::create("test_ui_shop.png");
	CCSprite* s_ruby = CCSprite::create("test_ui_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(StageSettingPopup::menuAction));
	ruby_item->setTag(kSSP_MT_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(124,top_case->getContentSize().height/2.f-2));
	top_case->addChild(ruby_menu);
	
	CCSprite* n_gold = CCSprite::create("test_ui_shop.png");
	CCSprite* s_gold = CCSprite::create("test_ui_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(StageSettingPopup::menuAction));
	gold_item->setTag(kSSP_MT_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(220,top_case->getContentSize().height/2.f-2));
	top_case->addChild(gold_menu);
	
	CCSprite* n_heart = CCSprite::create("test_ui_shop.png");
	CCSprite* s_heart = CCSprite::create("test_ui_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(StageSettingPopup::menuAction));
	heart_item->setTag(kSSP_MT_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(369,top_case->getContentSize().height/2.f-2));
	top_case->addChild(heart_menu);
	
	CCSprite* n_friend_point = CCSprite::create("test_ui_shop.png");
	CCSprite* s_friend_point = CCSprite::create("test_ui_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(StageSettingPopup::menuAction));
	friend_point_item->setTag(kSSP_MT_friendPoint);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(460,top_case->getContentSize().height/2.f-2));
	top_case->addChild(friend_point_menu);
	
	
	showPopup();
	
    return true;
}

void StageSettingPopup::showPopup()
{
	CCMoveTo* top_move = CCMoveTo::create(0.3f, ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	top_case->runAction(top_move);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(StageSettingPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void StageSettingPopup::endShowPopup()
{
	is_menu_enable = true;
	t_lvs->setTouchEnabled(true);
	my_ilv->setTouchEnabled(true);
}

void StageSettingPopup::hidePopup()
{
	is_menu_enable = false;
	t_lvs->setTouchEnabled(false);
	my_ilv->setTouchEnabled(false);
	
	CCMoveTo* top_move = CCMoveTo::create(0.3f, ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f + 33.f));
	top_case->runAction(top_move);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(StageSettingPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void StageSettingPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void StageSettingPopup::cheatAction(CCObject* sender)
{
	int tag = ((CCNode*)sender)->getTag();
	if(tag == 1)
		myDSH->setIntegerForKey(kDSH_Key_heartCnt, 5);
}

CCPoint StageSettingPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kSSP_MT_changeCard)				return_value = ccp(98,163);
	else if(t_tag == kSSP_MT_back)				return_value = ccp(55,29);
	else if(t_tag == kSSP_MT_challenge)			return_value = ccp(267,29);
	else if(t_tag == kSSP_MT_start)				return_value = ccp(395,29);
	else if(t_tag == kSSP_MT_info)				return_value = ccp(150,29);
	else if(t_tag == kSSP_MT_gacha)				return_value = ccp(385,250);
	else if(t_tag == kSSP_MT_itemBase)			return_value = ccp(305,200);
	else if(t_tag == kSSP_MT_selectedBase)		return_value = ccp(95,1);
	
	return return_value;
}

void StageSettingPopup::itemSetting()
{
	for(int i=0;i<item_list.size();i++)
	{
		ITEM_CODE t_ic = item_list[i];
		
		deque<int>::iterator iter = find(card_options.begin(), card_options.end(), t_ic);
		if(iter == card_options.end()) // not same option card // enable item
		{
			CCNode* item_parent = CCNode::create();
			item_parent->setPosition(ccpAdd(getContentPosition(kSSP_MT_itemBase), ccpMult(ccp(0,-46), i)));
			my_ilv->addChild(item_parent, kSSP_Z_content, kSSP_MT_itemBase+i);
			
			CCSprite* item_back = CCSprite::create("stagesetting_item_normal_back.png");
			item_back->setPosition(CCPointZero);
			item_parent->addChild(item_back, kSSP_Z_content);
			
			
			string item_filename = "";
//			if(t_ic == kIC_subOneDie || t_ic == kIC_longTime || t_ic == kIC_bossLittleEnergy || t_ic == kIC_smallArea || t_ic == kIC_widePerfect || t_ic == kIC_randomChange) // mission
//				item_filename = "stagesetting_item_case_mission.png";
//			else
				item_filename = "stagesetting_item_case_normal.png";
			
			CCSprite* n_case = CCSprite::create(item_filename.c_str());
			CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			n_img->setScale(0.53);
			n_img->setPosition(ccp(n_case->getContentSize().width/2.f,n_case->getContentSize().height/2.f));
			n_case->addChild(n_img);
			CCSprite* s_case = CCSprite::create(item_filename.c_str());
			s_case->setColor(ccGRAY);
			CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			s_img->setColor(ccGRAY);
			s_img->setScale(0.53);
			s_img->setPosition(ccp(s_case->getContentSize().width/2.f,s_case->getContentSize().height/2.f));
			s_case->addChild(s_img);
			
			CCMenuItem* select_item = CCMenuItemSprite::create(n_case, s_case, this, menu_selector(StageSettingPopup::menuAction));
			select_item->setTag(kSSP_MT_itemBase+i);
			
			CCMenu* select_menu = CCMenu::createWithItem(select_item);
			select_menu->setPosition(ccp(-100,0));
			item_parent->addChild(select_menu, kSSP_Z_content, kSSP_MT_itemBase+i);
			
			
			CCLabelTTF* option_label = CCLabelTTF::create(mySD->getItemScript(t_ic).c_str(), mySGD->getFont().c_str(), 8, CCSizeMake(130, 23), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
			option_label->setAnchorPoint(ccp(0,1));
			option_label->setPosition(ccp(-73,8));
			item_parent->addChild(option_label, kSSP_Z_content);
			
			
			string buy_type = mySD->getItemCurrency(t_ic);
			if(buy_type == "gold")
				buy_type = "price_gold_img.png";
			else if(buy_type == "ruby")
				buy_type = "price_ruby_img.png";
			else if(buy_type == "social")
				buy_type = "price_candy_img.png";
			
			CCSprite* n_buy = CCSprite::create("stagesetting_item_buy.png");
			CCSprite* n_buy_type = CCSprite::create(buy_type.c_str());
			n_buy_type->setScale(0.5f);
			n_buy_type->setPosition(ccp(12, 15));
			n_buy->addChild(n_buy_type);
			CCLabelTTF* n_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", 0/*mySD->getItemPrice(t_ic)*/)->getCString(), mySGD->getFont().c_str(), 8);
			n_label->setAnchorPoint(ccp(0.5f, 0.5f));
			n_label->setPosition(ccp(35, 15));
			n_buy->addChild(n_label);
			CCSprite* s_buy = CCSprite::create("stagesetting_item_buy.png");
			s_buy->setColor(ccGRAY);
			CCSprite* s_buy_type = CCSprite::create(buy_type.c_str());
			s_buy_type->setScale(0.5f);
			s_buy_type->setPosition(ccp(12, 15));
			s_buy->addChild(s_buy_type);
			CCLabelTTF* s_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", 0/*mySD->getItemPrice(t_ic)*/)->getCString(), mySGD->getFont().c_str(), 8);
			s_label->setAnchorPoint(ccp(0.5f, 0.5f));
			s_label->setPosition(ccp(35, 15));
			s_buy->addChild(s_label);
			
			CCMenuItem* buy_item = CCMenuItemSprite::create(n_buy, s_buy, this, menu_selector(StageSettingPopup::menuAction));
			buy_item->setTag(kSSP_MT_itemBuy+i);
			CCMenu* buy_menu = CCMenu::createWithItem(buy_item);
			buy_menu->setPosition(ccp(90,0));
			item_parent->addChild(buy_menu, kSSP_Z_content, kSSP_MT_itemBuy+i);
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			if(item_cnt > 0)
			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("+%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setPosition(ccp(-115, -15));
				item_parent->addChild(cnt_label, kSSP_Z_content, kSSP_MT_itemCntBase + i);
			}
			
			CCSprite* selected_img = CCSprite::create("stagesetting_item_selected.png");
			selected_img->setVisible(false);
			selected_img->setPosition(ccp(-100, 0));
			item_parent->addChild(selected_img, kSSP_Z_content, kSSP_MT_selectedBase+i);
			
			
			auto t_iter = find(show_item_popup.begin(), show_item_popup.end(), t_ic);
			if(t_iter != show_item_popup.end())
			{
				is_selected_item.push_back(true);
				selected_img->setVisible(true);
			}
			else
			{
				is_selected_item.push_back(false);
			}
		}
		else
		{
			is_selected_item.push_back(false);
		}
	}
}

void StageSettingPopup::callStart()
{
		if(heart_time->isStartable())
		{
			if(heart_time->startGame())
				realStartAction();
			else
			{
				is_menu_enable = true;
			}
		}
		else
		{
			is_menu_enable = true;
		}
}

void StageSettingPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kSSP_MT_start)
	{
		callStart();
	}
	else if(tag == kSSP_MT_back)
	{
		mySGD->resetLabels();
		
		hidePopup();
	}
	else if(tag == kSSP_MT_changeCard)
	{
		mySGD->resetLabels();
		mySGD->before_cardsetting = kSceneCode_StageSetting;
		
		CardSettingPopup* t_popup = CardSettingPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, kPMS_Z_popup);
		
		target_final = NULL;
		hidePopup();
	}
	else if(tag == kSSP_MT_challenge)
	{
		is_menu_enable = false;
//		StageRankPopup* t_sip = StageRankPopup::create(this, callfunc_selector(StageSettingPopup::popupClose), this, callfunc_selector(StageSettingPopup::callStart), selected_stage);
//		addChild(t_sip, kSSP_Z_popup);
	}
	else if(tag == kSSP_MT_gacha)
	{
		is_menu_enable = false;
		GachaPopup* t_gp = GachaPopup::create(this, callfunc_selector(StageSettingPopup::popupClose));
		addChild(t_gp, kSSP_Z_popup);
	}
	else if(tag == kSSP_MT_info)
	{
		is_menu_enable = false;
		StageInfoPopup* t_sip = StageInfoPopup::create(this, callfunc_selector(StageSettingPopup::popupClose), selected_stage);
		addChild(t_sip, kSSP_Z_popup);
	}
	else if(tag == kSSP_MT_rubyShop)
	{
//		ShopPopup* t_shop = ShopPopup::create();
//		t_shop->setHideFinalAction(NULL, NULL);
//		t_shop->targetHeartTime((HeartTime*)(top_case->getChildByTag(kSSP_MT_heartTime)));
//		t_shop->setShopCode(kSC_ruby);
//		addChild(t_shop, kSSP_Z_popup);
//		is_menu_enable = true;
	}
	else if(tag == kSSP_MT_goldShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(NULL, NULL);
		t_shop->targetHeartTime((HeartTime*)(top_case->getChildByTag(kSSP_MT_heartTime)));
		t_shop->setShopCode(kSC_gold);
		addChild(t_shop, kSSP_Z_popup);
		is_menu_enable = true;
	}
	else if(tag == kSSP_MT_heartShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(NULL, NULL);
		t_shop->targetHeartTime((HeartTime*)(top_case->getChildByTag(kSSP_MT_heartTime)));
		t_shop->setShopCode(kSC_heart);
		addChild(t_shop, kSSP_Z_popup);
		is_menu_enable = true;
	}
	else if(tag >= kSSP_MT_itemBase && tag < kSSP_MT_itemBuy)
	{
		int clicked_item_number = tag-kSSP_MT_itemBase;
		
		if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[clicked_item_number]) > 0)
		{
			if(is_selected_item[clicked_item_number])
			{
				is_selected_item[clicked_item_number] = false;
				
				CCNode* t_item = my_ilv->getChildByTag(kSSP_MT_itemBase+clicked_item_number);
				((CCSprite*)t_item->getChildByTag(kSSP_MT_selectedBase+clicked_item_number))->setVisible(false);
			}
			else
			{
				is_selected_item[clicked_item_number] = true;
				
				CCNode* t_item = my_ilv->getChildByTag(kSSP_MT_itemBase+clicked_item_number);
				((CCSprite*)t_item->getChildByTag(kSSP_MT_selectedBase+clicked_item_number))->setVisible(true);
			}
		}
		else
		{
			ItemBuyPopup* t_ibp = ItemBuyPopup::create(item_list[clicked_item_number], clicked_item_number, this, callfuncII_selector(StageSettingPopup::buySuccessItem));
			addChild(t_ibp, kSSP_Z_popup);
		}
		
		is_menu_enable = true;
	}
	else if(tag >= kSSP_MT_itemBuy && tag < kSSP_MT_selectedBase)
	{
		int clicked_item_number = tag-kSSP_MT_itemBuy;
		
		ItemBuyPopup* t_ibp = ItemBuyPopup::create(item_list[clicked_item_number], clicked_item_number, this, callfuncII_selector(StageSettingPopup::buySuccessItem));
		addChild(t_ibp, kSSP_Z_popup);
		is_menu_enable = true;
	}
	else if(tag == kSSP_MT_noti_ok)
	{
		removeChildByTag(kSSP_MT_noti);
		heart_time->startGame();
		realStartAction();
	}
	else if(tag == kSSP_MT_noti_cancel)
	{
		removeChildByTag(kSSP_MT_noti);
		
		is_menu_enable = true;
	}
	else if(tag == kSSP_MT_friendPoint)
	{
		if(!friend_point_popup)
		{
			CCNode* menu_node = ((CCNode*)pSender)->getParent();
			CCNode* top_node = menu_node->getParent();
			friend_point_popup = CCSprite::create("candy_popup.png");
			friend_point_popup->setAnchorPoint(ccp(0.5,1.f));
			friend_point_popup->setPosition(ccp(427,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
			top_node->addChild(friend_point_popup, -1);
			
			CCSprite* n_close = CCSprite::create("candy_popup_close.png");
			CCSprite* s_close = CCSprite::create("candy_popup_close.png");
			s_close->setColor(ccGRAY);
			
			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(StageSettingPopup::menuAction));
			close_item->setTag(kSSP_MT_friendPointClose);
			
			CCMenu* close_menu = CCMenu::createWithItem(close_item);
			close_menu->setPosition(ccp(friend_point_popup->getContentSize().width/2.f, 25));
			friend_point_popup->addChild(close_menu);
			
			CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(427,menu_node->getPositionY()-12));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StageSettingPopup::popupClose));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
			friend_point_popup->runAction(t_seq);
		}
		else
			is_menu_enable = true;
	}
	else if(tag == kSSP_MT_friendPointClose)
	{
		CCNode* menu_node = ((CCNode*)pSender)->getParent();
		CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(427,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StageSettingPopup::closeFriendPointPopup));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
		friend_point_popup->runAction(t_seq);
	}
}

void StageSettingPopup::closeFriendPointPopup()
{
	friend_point_popup->removeFromParent();
	friend_point_popup = NULL;
	is_menu_enable = true;
}

void StageSettingPopup::realStartAction()
{
	finalSetting();
	
//	myDSH->saveAllUserData(json_selector(this, StageSettingPopup::finalStartAction));
}

void StageSettingPopup::acceptStartAction()
{
	finalSetting();
	
	was_end_startAction = false;
	was_end_removeMessage = false;
	
//	vector<CommandParam> command_list;
//	
//	////////////////////////////// ksks
//	Json::Value p;
//	p["no"] = mySGD->getRemoveMessageMailNo();
//	p["memberID"] = mySGD->getRemoveMessageMemberId();
//	CommandParam ksooParam("removemessage", p, bind(&ThisClassType::finalRemoveMessage, this, _1));
//	command_list.push_back(ksooParam);
//	// create message remove command
//	// command_list.push_back(message remove command);
//	
//	//////////////////////////////
//	
//	command_list.push_back(CommandParam("updateUserData", myDSH->getSaveAllUserDataParam(), json_selector(this, StageSettingPopup::finalAcceptStartAction)));
//	
//	hspConnector::get()->command(command_list);
}
void StageSettingPopup::finalSetting()
{
	start_loading = LoadingLayer::create();
	addChild(start_loading, kSSP_Z_popup);
	
	deque<bool> is_using_item;
	is_using_item.push_back(false);
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
		is_using_item.push_back(false);
	
	for(int i=0;i<is_selected_item.size();i++)
	{
		if(is_selected_item[i])
		{
			myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i])-1);
			myLog->addLog(kLOG_useItem_s, -1, convertToItemCodeToItemName(item_list[i]).c_str());
			is_using_item[item_list[i]] = true;
		}
	}
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
		mySGD->setIsUsingItem(ITEM_CODE(i), is_using_item[i]);
}

void StageSettingPopup::finalRemoveMessage(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		was_end_removeMessage = true;
		if(was_end_startAction)
			goToGame();
	}
	else
	{
		cancelGame();
	}
}

void StageSettingPopup::goToGame()
{
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
	
//	Json::Value param;
//	param["key"] = CCSTR_CWF("stage_start_%d", mySD->getSilType())->getCString();
//	
//	hspConnector::get()->command("increaseStats", param, NULL);
	
	mySGD->resetLabels();
	myGD->resetGameData();
	
	mySGD->setGameStart();
	CCDirector::sharedDirector()->replaceScene(Maingame::scene());
}

void StageSettingPopup::cancelGame()
{
	if(!is_menu_enable)
	{
		start_loading->removeFromParent();
		
		CCLOG("Fail : user data save");
		
		deque<bool> is_using_item;
		is_using_item.push_back(false);
		for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
			is_using_item.push_back(false);
		
		for(int i=0;i<is_selected_item.size();i++)
		{
			if(is_selected_item[i])
			{
				myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i])+1);
				is_using_item[item_list[i]] = true;
			}
		}
		
		mySGD->resetUsingItem();
		
		is_menu_enable = true;
	}
}

void StageSettingPopup::finalAcceptStartAction(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		was_end_startAction = true;
		if(was_end_removeMessage)
			goToGame();
	}
	else
	{
		cancelGame();
	}
}

void StageSettingPopup::finalStartAction(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		goToGame();
	}
	else
	{
		heart_time->backHeart();
		
		cancelGame();
	}
}

void StageSettingPopup::popupClose()
{
	is_menu_enable = true;
}

void StageSettingPopup::buySuccessItem(int t_clicked_item_number, int cnt)
{
	myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number])+cnt);
	int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number]);
	is_selected_item[t_clicked_item_number] = true;
	
	myLog->addLog(kLOG_buyItem_s, -1, convertToItemCodeToItemName(item_list[t_clicked_item_number]).c_str());
	
	CCNode* item_parent = my_ilv->getChildByTag(kSSP_MT_itemBase+t_clicked_item_number);
	
	CCLabelTTF* cnt_label = (CCLabelTTF*)item_parent->getChildByTag(kSSP_MT_itemCntBase+t_clicked_item_number);
	if(cnt_label)
	{
		cnt_label->setString(CCString::createWithFormat("+%d", item_cnt)->getCString());
	}
	else
	{
		cnt_label = CCLabelTTF::create(CCString::createWithFormat("+%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
		cnt_label->setPosition(ccp(-115, -15));
		item_parent->addChild(cnt_label, kSSP_Z_content, kSSP_MT_itemCntBase + t_clicked_item_number);
	}
	
	((CCSprite*)item_parent->getChildByTag(kSSP_MT_selectedBase+t_clicked_item_number))->setVisible(true);
	
//	vector<SaveUserData_Key> save_userdata_list;
//	
//	save_userdata_list.push_back(kSaveUserData_Key_gold);
//	save_userdata_list.push_back(kSaveUserData_Key_item);
//	
//	myDSH->saveUserData(save_userdata_list, nullptr);
}

string StageSettingPopup::convertToItemCodeToItemName(ITEM_CODE t_code)
{
	string return_value;
	if(t_code == kIC_attack)				return_value = "attack";
	else if(t_code == kIC_speedUp)			return_value = "speedUp";
	else if(t_code == kIC_addTime)			return_value = "addTime";
	else if(t_code == kIC_fast)				return_value = "fast";
	else if(t_code == kIC_subOneDie)		return_value = "subOneDie";
	else if(t_code == kIC_doubleItem)		return_value = "doubleItem";
	else if(t_code == kIC_silence)			return_value = "silence";
	else if(t_code == kIC_longTime)			return_value = "longTime";
	else if(t_code == kIC_baseSpeedUp)		return_value = "baseSpeedUp";
	
	return return_value.c_str();
}