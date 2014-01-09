//
//  StartSettingScene.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 18..
//
//

#include "StartSettingScene.h"
#include "DataStorageHub.h"
#include "CountingBMLabel.h"
#include "StarGoldData.h"
#include "HeartTime.h"
#include "ShopPopup.h"
#include "CardChangePopup.h"
#include "ServerDataSave.h"
#include "ASPopupView.h"
#include "PuzzleScene.h"
#include "MainFlowScene.h"
#include "StageImgLoader.h"
#include "CardCase.h"
#include "ShowItemContent.h"
#include "ItemBuyPopup.h"
#include "LogData.h"
#include "KSUtil.h"
#include "LoadingLayer.h"
#include "DurabilityNoti.h"
#include "MaingameScene.h"
#include "TutorialFlowStep.h"

CCScene* StartSettingScene::scene()
{
    CCScene *scene = CCScene::create();
    
    StartSettingScene *layer = StartSettingScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    scene->addChild(layer);
	
    return scene;
}

enum StartSettingZorder{
	kStartSettingZorder_back = 1,
	kStartSettingZorder_main,
	kStartSettingZorder_top,
	kStartSettingZorder_popup
};

bool StartSettingScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	friend_point_popup = NULL;
	item_title_label = NULL;
	option_label = NULL;
	buy_menu = NULL;
	card_img = NULL;
	
	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kStartSettingZorder_back);
	
	setMain();
	
	setTop();
	
	is_menu_enable = true;
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_startClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_startClick);
		addChild(t_tutorial, kStartSettingZorder_popup);
		
		tutorial_node = t_tutorial;
	}
	else if(recent_step == kTutorialFlowStep_emptyCardClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_emptyCardClick);
		addChild(t_tutorial, kStartSettingZorder_popup);
		
		tutorial_node = t_tutorial;
	}
	
	return true;
}

enum StartSettingMenuTag{
	kStartSettingMenuTag_cancel = 0,
	kStartSettingMenuTag_rubyShop,
	kStartSettingMenuTag_goldShop,
	kStartSettingMenuTag_heartShop,
	kStartSettingMenuTag_friendPointContent,
	kStartSettingMenuTag_friendPointClose,
	kStartSettingMenuTag_back,
	kStartSettingMenuTag_start,
	kStartSettingMenuTag_card,
	kStartSettingMenuTag_itemBuy,
	kStartSettingMenuTag_itemBase = 100000
};

enum StartSettingItemZorder{
	kStartSettingItemZorder_base = 0,
	kStartSettingItemZorder_selected,
	kStartSettingItemZorder_clicked,
	kStartSettingItemZorder_cntLabel
};

void StartSettingScene::setMain()
{
	main_case = CCSprite::create("startsetting_back.png");
	main_case->setPosition(ccp(240,160));
	addChild(main_case, kStartSettingZorder_main);
	
	int stage_number;
	
	if(mySGD->is_before_selected_event_stage)
	{
		stage_number = mySD->getSilType();
		
		CCLabelTTF* stage_number_label = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(),	mySGD->getFont().c_str(), 15);
		stage_number_label->setPosition(ccp(60, main_case->getContentSize().height-62));
		main_case->addChild(stage_number_label);
		
		mySGD->is_before_selected_event_stage = false;
	}
	else
	{
		stage_number = mySD->getSilType();
		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
		int piece_number = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
		
		CCLabelTTF* piece_number_label = CCLabelTTF::create(CCString::createWithFormat("%d-%d", puzzle_number, piece_number)->getCString(),	mySGD->getFont().c_str(), 15);
		piece_number_label->setPosition(ccp(60, main_case->getContentSize().height-62));
		main_case->addChild(piece_number_label);
	}
	
	CCLabelTTF* mission_label = CCLabelTTF::create(mySD->getConditionContent(stage_number).c_str(), mySGD->getFont().c_str(), 13);
	mission_label->setPosition(ccp(main_case->getContentSize().width/2.f+62, main_case->getContentSize().height-62));
	main_case->addChild(mission_label);
	
	if(mySD->getClearCondition(stage_number) != kCLEAR_default)
	{
		CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
		CCHide* t_hide = CCHide::create();
		CCDelayTime* t_delay2 = CCDelayTime::create(0.2f);
		CCShow* t_show = CCShow::create();
		CCSequence* t_seq = CCSequence::create(t_delay1, t_hide, t_delay2, t_show, NULL);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
		mission_label->runAction(t_repeat);
	}
	
	CCSprite* n_back = CCSprite::create("item_buy_popup_close.png");
	CCSprite* s_back = CCSprite::create("item_buy_popup_close.png");
	s_back->setColor(ccGRAY);
	
	CCMenuItem* back_item = CCMenuItemSprite::create(n_back, s_back, this, menu_selector(StartSettingScene::menuAction));
	back_item->setTag(kStartSettingMenuTag_back);
	
	CCMenu* back_menu = CCMenu::createWithItem(back_item);
	back_menu->setPosition(ccp(main_case->getContentSize().width-45, main_case->getContentSize().height-59));
	main_case->addChild(back_menu);
	
	changeCard();
	
	float card_rate = 0.473f;
	
	CCSprite* n_card = CCSprite::create("cardsetting_cardmenu_big.png", CCRectMake(0, 0, 320*card_rate, 430*card_rate));
	CCSprite* s_card = CCSprite::create("cardsetting_cardmenu_big.png", CCRectMake(0, 0, 320*card_rate, 430*card_rate));
	
	CCMenuItem* card_item = CCMenuItemSprite::create(n_card, s_card, this, menu_selector(StartSettingScene::menuAction));
	card_item->setTag(kStartSettingMenuTag_card);
	
	CCMenu* card_menu = CCMenu::createWithItem(card_item);
	card_menu->setPosition(ccp(320*card_rate/2.f + 40, 430*card_rate/2.f + 28));
	main_case->addChild(card_menu);
	
	
	item_list = mySD->getStageItemList(stage_number);
	
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
	myDSH->saveUserData({kSaveUserData_Key_item}, nullptr);
	
	if(!show_item_popup.empty())
	{
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		
		ShowItemContent* t_container = ShowItemContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
															   {
																   t_popup->removeFromParent();
															   }, show_item_popup);
		t_popup->setContainerNode(t_container);
		addChild(t_popup, kStartSettingZorder_popup);
	}
	
	clicked_item_idx = -1;
	
	for(int i=0;i<item_list.size() && i < 8;i++)
	{
		ITEM_CODE t_ic = item_list[i];
		
		CCPoint item_position = ccp(239 + (i%4)*55, 205 - (i/4)*51);
		
		deque<int>::iterator iter = find(card_options.begin(), card_options.end(), t_ic);
		if(iter == card_options.end()) // not same option card // enable item
		{
			CCNode* item_parent = CCNode::create();
			item_parent->setPosition(item_position);
			main_case->addChild(item_parent, 0, kStartSettingMenuTag_itemBase+i);
			
			string item_filename = "";
//			if(t_ic == kIC_subOneDie || t_ic == kIC_longTime || t_ic == kIC_bossLittleEnergy || t_ic == kIC_smallArea || t_ic == kIC_widePerfect || t_ic == kIC_randomChange) // mission
//				item_filename = "stagesetting_item_case_mission.png";
//			else
				item_filename = "startsetting_item_case.png";
			
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
			
			CCMenuItem* select_item = CCMenuItemSprite::create(n_case, s_case, this, menu_selector(StartSettingScene::itemAction));
			select_item->setTag(i+1);
			
			CCMenu* select_menu = CCMenu::createWithItem(select_item);
			select_menu->setPosition(ccp(0,0));
			item_parent->addChild(select_menu);
			
			
			
			CCSprite* selected_img = CCSprite::create("startsetting_item_selected.png");
			selected_img->setVisible(false);
			selected_img->setPosition(ccp(0, 0));
			item_parent->addChild(selected_img, kStartSettingItemZorder_selected, kStartSettingItemZorder_selected);
			
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(false);
			clicked_img->setPosition(ccp(0, 0));
			item_parent->addChild(clicked_img, kStartSettingItemZorder_clicked, kStartSettingItemZorder_clicked);
			
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			if(item_cnt > 0)
			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("+%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setPosition(ccp(-15, -15));
				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
			}
			
			auto t_iter = find(show_item_popup.begin(), show_item_popup.end(), t_ic);
			if(t_iter != show_item_popup.end())
			{
				if(t_ic == kIC_rentCard && mySGD->getSelectedFriendCardData().card_number == 0)
				{
					is_selected_item.push_back(false);
					select_menu->setEnabled(false);
				}
				else
				{
					if(getSelectedItemCount() > 3)
					{
						is_selected_item.push_back(false);
					}
					else
					{
						is_selected_item.push_back(true);
						selected_img->setVisible(true);
					}
				}
			}
			else
			{
				if(t_ic == kIC_rentCard && mySGD->getSelectedFriendCardData().card_number == 0)
					select_menu->setEnabled(false);
				is_selected_item.push_back(false);
			}
		}
		else
			is_selected_item.push_back(false);
	}
	
	CCSprite* n_start = CCSprite::create("startsetting_start.png");
	CCSprite* s_start = CCSprite::create("startsetting_start.png");
	s_start->setColor(ccGRAY);
	
	CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(StartSettingScene::menuAction));
	start_item->setTag(kStartSettingMenuTag_start);
	
	CCMenu* start_menu = CCMenu::createWithItem(start_item);
	start_menu->setPosition(ccp(321, 45));
	main_case->addChild(start_menu);
}

int StartSettingScene::getSelectedItemCount()
{
	int selected_item_cnt = 0;
	for(int k=0;k<selected_item_cnt;k++)
		if(is_selected_item[k])
			selected_item_cnt++;
	
	return selected_item_cnt;
}

void StartSettingScene::changeCard()
{
	if(card_img)
		card_img->removeFromParent();
	
	float card_rate = 0.473f;
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard); // 1, 2, 3 / 11, 12, 13 / 14, ...
	if(selected_card_number > 0 && myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) > 0)
	{
		int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, selected_card_number);
		int card_level = NSDS_GI(kSDS_CI_int1_grade_i, selected_card_number);
		
		card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_level)->getCString());
		card_img->setScale(card_rate);
		card_img->setPosition(ccp(320*card_rate/2.f + 40, 430*card_rate/2.f + 28));
		main_case->addChild(card_img);
		
		if(card_level == 3 && mySD->isAnimationStage(card_stage))
		{
			CCSize ani_size = mySD->getAnimationCutSize(card_stage);
			CCSprite* card_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_level)->getCString(),
													 CCRectMake(0, 0, ani_size.width, ani_size.height));
			card_ani->setPosition(mySD->getAnimationPosition(card_stage));
			card_img->addChild(card_ani);
		}
		
		CardCase* t_case = CardCase::create(selected_card_number);
		t_case->setPosition(CCPointZero);
		card_img->addChild(t_case);
		
		mySD->setCardOptions(card_options, selected_card_number);
	}
	else
	{
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
	}
}

void StartSettingScene::itemAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_startClick || recent_step == kTutorialFlowStep_emptyCardClick)
	{
		
	}
	else
	{
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(clicked_item_idx == -1)
		{
			clicked_item_idx = tag-1;
			CCNode* item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+clicked_item_idx);
			CCSprite* clicked_img = (CCSprite*)item_parent->getChildByTag(kStartSettingItemZorder_clicked);
			clicked_img->setVisible(true);
		}
		else
		{
			CCNode* before_item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+clicked_item_idx);
			CCSprite* before_clicked_img = (CCSprite*)before_item_parent->getChildByTag(kStartSettingItemZorder_clicked);
			before_clicked_img->setVisible(false);
			
			clicked_item_idx = tag-1;
			
			CCNode* after_item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+clicked_item_idx);
			CCSprite* after_clicked_img = (CCSprite*)after_item_parent->getChildByTag(kStartSettingItemZorder_clicked);
			after_clicked_img->setVisible(true);
		}
		
		if(is_selected_item[tag-1]) // selected -> unselect
		{
			CCNode* item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+tag-1);
			CCSprite* selected_img = (CCSprite*)item_parent->getChildByTag(kStartSettingItemZorder_selected);
			selected_img->setVisible(false);
			is_selected_item[tag-1] = false;
		}
		else // unselected -> selected or nothing
		{
			bool is_selectable = false;
			ITEM_CODE clicked_item_code = item_list[tag-1];
			if(clicked_item_code == kIC_rentCard && mySGD->getSelectedFriendCardData().card_number == 0) // nothing friend
				is_selectable = false;
			else if(getSelectedItemCount() > 3)
				is_selectable = false;
			else
				is_selectable = true;
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[tag-1]);
			if(item_cnt > 0 && is_selectable)
			{
				CCNode* item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+tag-1);
				CCSprite* selected_img = (CCSprite*)item_parent->getChildByTag(kStartSettingItemZorder_selected);
				selected_img->setVisible(true);
				is_selected_item[tag-1] = true;
			}
		}
		
		if(item_title_label)
			item_title_label->removeFromParent();
		if(option_label)
			option_label->removeFromParent();
		if(buy_menu)
			buy_menu->removeFromParent();
		
		item_title_label = CCLabelTTF::create(convertToItemCodeToItemName(item_list[tag-1]).c_str(), mySGD->getFont().c_str(), 10, CCSizeMake(145, 23), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
		item_title_label->setAnchorPoint(ccp(0,1));
		item_title_label->setPosition(ccp(218, 120));
		main_case->addChild(item_title_label);
		
		option_label = CCLabelTTF::create(mySD->getItemScript(item_list[tag-1]).c_str(), mySGD->getFont().c_str(), 8, CCSizeMake(145, 23), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
		option_label->setAnchorPoint(ccp(0,1));
		option_label->setPosition(ccp(218, 102));
		main_case->addChild(option_label);
		
		
		string buy_type = mySD->getItemCurrency(item_list[tag-1]);
		if(buy_type == "gold")
			buy_type = "price_gold_img.png";
		else if(buy_type == "ruby")
			buy_type = "price_ruby_img.png";
		else if(buy_type == "social")
			buy_type = "price_candy_img.png";
		
		CCSprite* n_buy = CCSprite::create("startsetting_item_buy.png");
		CCSprite* n_buy_type = CCSprite::create(buy_type.c_str());
		n_buy_type->setScale(0.5f);
		n_buy_type->setPosition(ccp(17, 32));
		n_buy->addChild(n_buy_type);
		CCLabelTTF* n_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", mySD->getItemPrice(item_list[tag-1]))->getCString(), mySGD->getFont().c_str(), 8);
		n_label->setAnchorPoint(ccp(0.5f, 0.5f));
		n_label->setPosition(ccp(38, 32));
		n_buy->addChild(n_label);
		CCSprite* s_buy = CCSprite::create("startsetting_item_buy.png");
		s_buy->setColor(ccGRAY);
		CCSprite* s_buy_type = CCSprite::create(buy_type.c_str());
		s_buy_type->setScale(0.5f);
		s_buy_type->setPosition(ccp(17, 32));
		s_buy->addChild(s_buy_type);
		CCLabelTTF* s_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", mySD->getItemPrice(item_list[tag-1]))->getCString(), mySGD->getFont().c_str(), 8);
		s_label->setAnchorPoint(ccp(0.5f, 0.5f));
		s_label->setPosition(ccp(38, 32));
		s_buy->addChild(s_label);
		
		CCMenuItem* buy_item = CCMenuItemSprite::create(n_buy, s_buy, this, menu_selector(StartSettingScene::menuAction));
		buy_item->setTag(kStartSettingMenuTag_itemBuy);
		buy_menu = CCMenu::createWithItem(buy_item);
		buy_menu->setPosition(ccp(398, 102));
		main_case->addChild(buy_menu);
		
		is_menu_enable = true;
	}
}

void StartSettingScene::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_startClick)
	{
		int tag = ((CCNode*)sender)->getTag();
		if(tag == kStartSettingMenuTag_start)
		{
			is_menu_enable = false;
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_ingame);
			removeChild(tutorial_node);
			callStart();
		}
	}
	else if(recent_step == kTutorialFlowStep_emptyCardClick)
	{
		int tag = ((CCNode*)sender)->getTag();
		if(tag == kStartSettingMenuTag_card)
		{
			is_menu_enable = false;
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_targetCardClick);
			removeChild(tutorial_node);
			CardChangePopup* t_popup = CardChangePopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupCloseCardSetting));
			addChild(t_popup, kStartSettingZorder_popup);
		}
	}
	else
	{
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kStartSettingMenuTag_cancel)
		{
			CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
		}
		else if(tag == kStartSettingMenuTag_rubyShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_ruby);
			t_shop->setShopBeforeCode(kShopBeforeCode_startsetting);
			addChild(t_shop, kStartSettingZorder_popup);
		}
		else if(tag == kStartSettingMenuTag_goldShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_gold);
			t_shop->setShopBeforeCode(kShopBeforeCode_startsetting);
			addChild(t_shop, kStartSettingZorder_popup);
		}
		else if(tag == kStartSettingMenuTag_heartShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_heart);
			t_shop->setShopBeforeCode(kShopBeforeCode_startsetting);
			addChild(t_shop, kStartSettingZorder_popup);
		}
		else if(tag == kStartSettingMenuTag_friendPointContent)
		{
			if(!friend_point_popup)
			{
				CCNode* menu_node = ((CCNode*)sender)->getParent();
				CCNode* top_node = menu_node->getParent();
				friend_point_popup = CCSprite::create("candy_popup.png");
				friend_point_popup->setAnchorPoint(ccp(0.5,1.f));
				friend_point_popup->setPosition(ccp(410,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
				top_node->addChild(friend_point_popup, -1);
				
				CCSprite* n_close = CCSprite::create("candy_popup_close.png");
				CCSprite* s_close = CCSprite::create("candy_popup_close.png");
				s_close->setColor(ccGRAY);
				
				CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(StartSettingScene::menuAction));
				close_item->setTag(kStartSettingMenuTag_friendPointClose);
				
				CCMenu* close_menu = CCMenu::createWithItem(close_item);
				close_menu->setPosition(ccp(friend_point_popup->getContentSize().width/2.f, 25));
				friend_point_popup->addChild(close_menu);
				
				CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(410,menu_node->getPositionY()-12));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartSettingScene::popupClose));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
				friend_point_popup->runAction(t_seq);
			}
			else
				is_menu_enable = true;
		}
		else if(tag == kStartSettingMenuTag_friendPointClose)
		{
			CCNode* menu_node = ((CCNode*)sender)->getParent();
			CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(410,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartSettingScene::closeFriendPointPopup));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
			friend_point_popup->runAction(t_seq);
		}
		else if(tag == kStartSettingMenuTag_back)
		{
			if(mySD->getSilType() >= 10000 || mySGD->getIsAcceptHelp() || mySGD->getIsAcceptChallenge())
			{
				mySGD->setIsAcceptChallenge(false);
				mySGD->setIsAcceptHelp(false);
				CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
			}
			else
				CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
		}
		else if(tag == kStartSettingMenuTag_start)
		{
			callStart();
		}
		else if(tag == kStartSettingMenuTag_card)
		{
			CardChangePopup* t_popup = CardChangePopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupCloseCardSetting));
			addChild(t_popup, kStartSettingZorder_popup);
		}
		else if(tag == kStartSettingMenuTag_itemBuy)
		{
			ItemBuyPopup* t_ibp = ItemBuyPopup::create(item_list[clicked_item_idx], clicked_item_idx, this, callfuncII_selector(StartSettingScene::buySuccessItem));
			t_ibp->setCloseTargetDelegate(this, callfunc_selector(StartSettingScene::popupClose));
			addChild(t_ibp, kStartSettingZorder_popup);
		}
	}
}

void StartSettingScene::setTop()
{
	CCSprite* top_case = CCSprite::create("puzzle_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	addChild(top_case, kStartSettingZorder_top);
	
	CCSprite* n_cancel = CCSprite::create("puzzle_cancel.png");
	CCSprite* s_cancel = CCSprite::create("puzzle_cancel.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(StartSettingScene::menuAction));
	cancel_item->setTag(kStartSettingMenuTag_cancel);
	
	CCMenu* cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(80,top_case->getContentSize().height/2.f));
	top_case->addChild(cancel_menu);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(130,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(StartSettingScene::menuAction));
	heart_item->setTag(kStartSettingMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(243,top_case->getContentSize().height/2.f-2));
	top_case->addChild(heart_menu);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "mainflow_top_font1.png.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(302,top_case->getContentSize().height/2.f-2));
	top_case->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(StartSettingScene::menuAction));
	gold_item->setTag(kStartSettingMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(336,top_case->getContentSize().height/2.f-2));
	top_case->addChild(gold_menu);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getStar())->getCString(), "mainflow_top_font1.png.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(391,top_case->getContentSize().height/2.f-2));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(StartSettingScene::menuAction));
	ruby_item->setTag(kStartSettingMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(421,top_case->getContentSize().height/2.f-2));
	top_case->addChild(ruby_menu);
	
	friend_point_label =  CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getFriendPoint())->getCString(), "mainflow_top_font1.png.fnt", 0.3f, "%d");
	friend_point_label->setPosition(ccp(475,top_case->getContentSize().height/2.f-2));
	top_case->addChild(friend_point_label);
	
	mySGD->setFriendPointLabel(friend_point_label);
	
	CCSprite* n_friend_point = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_friend_point = CCSprite::create("mainflow_top_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(StartSettingScene::menuAction));
	friend_point_item->setTag(kStartSettingMenuTag_friendPointContent);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(505,top_case->getContentSize().height/2.f-2));
	top_case->addChild(friend_point_menu);
}

void StartSettingScene::callStart()
{
	if(mySGD->getIsAcceptChallenge() || mySGD->getIsAcceptHelp())
	{
		acceptStartAction();
	}
	else
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		int durability;
		if(selected_card_number > 0)
		{
			durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number)-1;
		}
		else
		{
			durability = -1;
		}
		
		if(heart_time->isStartable())
		{
			if(durability > 0)
			{
				if(heart_time->startGame())
					realStartAction();
				else
				{
					if(mySGD->getIsMeChallenge())
						mySGD->setIsMeChallenge(false);
					
					is_menu_enable = true;
				}
			}
			else if(durability == 0)
			{
				is_menu_enable = true;
				DurabilityNoti* t_popup = DurabilityNoti::create(this, menu_selector(StartSettingScene::durabilityCancelAction), this, menu_selector(StartSettingScene::durabilityOkAction));
				addChild(t_popup, kStartSettingZorder_popup, kStartSettingZorder_popup);
			}
			else // not selected card
			{
				if(heart_time->startGame())
					realStartAction();
				else
				{
					if(mySGD->getIsMeChallenge())
						mySGD->setIsMeChallenge(false);
					
					is_menu_enable = true;
				}
			}
		}
		else
		{
			if(mySGD->getIsMeChallenge())
				mySGD->setIsMeChallenge(false);
			
			is_menu_enable = true;
		}
	}
}

void StartSettingScene::realStartAction()
{
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	if(selected_card_number > 0)
	{
		int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) - 1;
		myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number, durability);
	}
	
	finalSetting();
	
	myDSH->saveAllUserData(json_selector(this, StartSettingScene::finalStartAction));
}

void StartSettingScene::acceptStartAction()
{
	finalSetting();
	
	was_end_startAction = false;
	was_end_removeMessage = false;
	
	vector<CommandParam> command_list;
	
	////////////////////////////// ksks
	Json::Value p;
	p["no"] = mySGD->getRemoveMessageMailNo();
	p["memberID"] = mySGD->getRemoveMessageMemberId();
	CommandParam ksooParam("removemessage", p, bind(&ThisClassType::finalRemoveMessage, this, _1));
	command_list.push_back(ksooParam);
	// create message remove command
	// command_list.push_back(message remove command);
	
	//////////////////////////////
	
	command_list.push_back(CommandParam("updateUserData", myDSH->getSaveAllUserDataParam(), json_selector(this, StartSettingScene::finalAcceptStartAction)));
	
	hspConnector::get()->command(command_list);
}
void StartSettingScene::finalSetting()
{
	start_loading = LoadingLayer::create();
	addChild(start_loading, kStartSettingZorder_popup);
	
	deque<bool> is_using_item;
	is_using_item.push_back(false);
	
	for(int i=kIC_attack;i<=kIC_rentCard;i++)
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
	
	for(int i=kIC_attack;i<=kIC_rentCard;i++)
		mySGD->setIsUsingItem(ITEM_CODE(i), is_using_item[i]);
}

void StartSettingScene::finalRemoveMessage(Json::Value result_data)
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

void StartSettingScene::goToGame()
{
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
	
	Json::Value param;
	param["key"] = CCString::createWithFormat("stage_start_%d", mySD->getSilType())->getCString();
	
	hspConnector::get()->command("increaseStats", param, NULL);
	
	mySGD->resetLabels();
	myGD->resetGameData();
	
	mySGD->setGameStart();
	CCDirector::sharedDirector()->replaceScene(Maingame::scene());
}

void StartSettingScene::cancelGame()
{
	if(!is_menu_enable)
	{
		start_loading->removeFromParent();
		
		CCLog("Fail : user data save");
		
		deque<bool> is_using_item;
		is_using_item.push_back(false);
		for(int i=kIC_attack;i<=kIC_rentCard;i++)
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
		
		if(mySGD->getIsMeChallenge())
			mySGD->setIsMeChallenge(false);
		
		is_menu_enable = true;
	}
}

void StartSettingScene::finalAcceptStartAction(Json::Value result_data)
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

void StartSettingScene::finalStartAction(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		goToGame();
	}
	else
	{
		heart_time->backHeart();
		
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(selected_card_number > 0)
		{
			int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) + 1;
			myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number, durability);
		}
		
		cancelGame();
	}
}

void StartSettingScene::durabilityCancelAction(CCObject* sender)
{
	removeChildByTag(kStartSettingZorder_popup);
	
	if(mySGD->getIsMeChallenge())
		mySGD->setIsMeChallenge(false);
}

void StartSettingScene::durabilityOkAction(CCObject *sender)
{
	removeChildByTag(kStartSettingZorder_popup);
	
	heart_time->startGame();
	realStartAction();
}

void StartSettingScene::popupClose()
{
	is_menu_enable = true;
}

void StartSettingScene::popupCloseCardSetting()
{
	changeCard();
	is_menu_enable = true;
}

void StartSettingScene::buySuccessItem(int t_clicked_item_idx, int cnt)
{
	myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_idx], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_idx])+cnt);
	int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_idx]);
	
	myLog->addLog(kLOG_buyItem_s, -1, convertToItemCodeToItemName(item_list[t_clicked_item_idx]).c_str());
	
	CCNode* item_parent = main_case->getChildByTag(kStartSettingMenuTag_itemBase+t_clicked_item_idx);
	
	CCLabelTTF* cnt_label = (CCLabelTTF*)item_parent->getChildByTag(kStartSettingItemZorder_cntLabel);
	if(cnt_label)
	{
		cnt_label->setString(CCString::createWithFormat("+%d", item_cnt)->getCString());
	}
	else
	{
		cnt_label = CCLabelTTF::create(CCString::createWithFormat("+%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
		cnt_label->setPosition(ccp(-15, -15));
		item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
	}
	
	bool is_selectable = false;
	ITEM_CODE clicked_item_code = item_list[t_clicked_item_idx];
	if(clicked_item_code == kIC_rentCard && mySGD->getSelectedFriendCardData().card_number == 0) // nothing friend
		is_selectable = false;
	else if(getSelectedItemCount() > 0)
		is_selectable = false;
	else
		is_selectable = true;
	
	if(is_selectable)
	{
		CCSprite* selected_img = (CCSprite*)item_parent->getChildByTag(kStartSettingItemZorder_selected);
		selected_img->setVisible(true);
		((CCSprite*)item_parent->getChildByTag(kStartSettingItemZorder_clicked))->setVisible(true);
		is_selected_item[t_clicked_item_idx] = true;
	}
	
	vector<SaveUserData_Key> save_userdata_list;
	
	save_userdata_list.push_back(kSaveUserData_Key_gold);
	save_userdata_list.push_back(kSaveUserData_Key_item);
	
	myDSH->saveUserData(save_userdata_list, nullptr);
	
	is_menu_enable = true;
}

void StartSettingScene::closeFriendPointPopup()
{
	friend_point_popup->removeFromParent();
	friend_point_popup = NULL;
	is_menu_enable = true;
}

string StartSettingScene::convertToItemCodeToItemName(ITEM_CODE t_code)
{
	string return_value;
	if(t_code == kIC_attack)				return_value = "Attack";
	else if(t_code == kIC_speedUp)			return_value = "SpeedUp";
	else if(t_code == kIC_addTime)			return_value = "AddTime";
	else if(t_code == kIC_fast)				return_value = "Fast";
	else if(t_code == kIC_critical)			return_value = "Critical";
	else if(t_code == kIC_subOneDie)		return_value = "SubOneDie";
	else if(t_code == kIC_doubleItem)		return_value = "DoubleItem";
	else if(t_code == kIC_silence)			return_value = "Silence";
	else if(t_code == kIC_subNothing)		return_value = "SubNothing";
	else if(t_code == kIC_longTime)			return_value = "LongTime";
	else if(t_code == kIC_bossLittleEnergy)	return_value = "BossLittleEnergy";
	else if(t_code == kIC_subSmallSize)		return_value = "SubSmallSize";
	else if(t_code == kIC_smallArea)		return_value = "SmallArea";
	else if(t_code == kIC_widePerfect)		return_value = "WidePerfect";
	else if(t_code == kIC_randomChange)		return_value = "RandomChange";
	else if(t_code == kIC_rentCard)			return_value = "RentCard";
	
	return return_value.c_str();
}