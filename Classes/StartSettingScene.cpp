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
//#include "ItemBuyPopup.h"
#include "LogData.h"
#include "KSUtil.h"
#include "LoadingLayer.h"
//#include "DurabilityNoti.h"
#include "MaingameScene.h"
#include "TutorialFlowStep.h"
#include "AlertEngine.h"
#include "LoadingTipScene.h"
#include "LabelTTFMarquee.h"
#include "TouchSuctionLayer.h"
#include "NewMainFlowScene.h"
#include "CommonButton.h"

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

bool StartSettingScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
//	mySGD->selectFriendCard();
	
	setKeypadEnabled(true);
	
	item_title_label = NULL;
	option_label = NULL;
//	card_img = NULL;
	selected_gacha_item = kIC_emptyBegin;
	
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
	
//	if(!myDSH->getBoolForKey(kDSH_Key_was_opened_tutorial_dimed_startsetting))
//	{
//		myDSH->setBoolForKey(kDSH_Key_was_opened_tutorial_dimed_startsetting, true);
//		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//		t_suction->target_touch_began = t_suction;
//		t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//		t_suction->setTouchEnabled(true);
//		
//		CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_startsetting.png");
//		dimed_tip->setPosition(ccp(240,160));
//		t_suction->addChild(dimed_tip);
//		
//		addChild(t_suction, kStartSettingZorder_popup);
//	}
	
	return true;
}

enum StartSettingMenuTag{
	kStartSettingMenuTag_cancel = 0,
	kStartSettingMenuTag_rubyShop,
	kStartSettingMenuTag_goldShop,
	kStartSettingMenuTag_heartShop,
	kStartSettingMenuTag_tip,
	kStartSettingMenuTag_back,
	kStartSettingMenuTag_start,
	kStartSettingMenuTag_card,
	kStartSettingMenuTag_turn,
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
	float screen_height = myDSH->ui_top-320.f;
	if(screen_height < 0.f)		screen_height = 0.f;
	screen_height /= 4.f;
	
	
	main_case = CCSprite::create("startsetting_back.png");
	main_case->setAnchorPoint(ccp(0.5,0));
	main_case->setPosition(ccp(240,3-screen_height));
	addChild(main_case, kStartSettingZorder_main);
	
	
	int stage_number;
	
	if(mySGD->is_before_selected_event_stage)
	{
		stage_number = mySD->getSilType();
		
		CCLabelTTF* stage_number_label = CCLabelTTF::create(CCString::createWithFormat("%d 스테이지", stage_number)->getCString(),	mySGD->getFont().c_str(), 15);
		stage_number_label->setPosition(ccp(65, main_case->getContentSize().height+40-65));
		main_case->addChild(stage_number_label);
		
		is_before_selected_event_stage = true;
		mySGD->is_before_selected_event_stage = false;
	}
	else
	{
		stage_number = mySD->getSilType();
//		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
//		int piece_number = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
		
		CCLabelTTF* piece_number_label = CCLabelTTF::create(CCString::createWithFormat("%d 스테이지", stage_number)->getCString(),	mySGD->getFont().c_str(), 15);
		piece_number_label->setPosition(ccp(65, main_case->getContentSize().height+40-65));
		main_case->addChild(piece_number_label);
		
		is_before_selected_event_stage = false;
	}
	
	CCRect mission_size = CCRectMake(0, 0, 225, 22);
	CCPoint mission_position = ccp(main_case->getContentSize().width/2.f+90, main_case->getContentSize().height+2-65);
	
//	CCSprite* temp_mission = CCSprite::create("whitePaper.png", mission_size);
//	temp_mission->setOpacity(100);
//	temp_mission->setPosition(mission_position);
//	main_case->addChild(temp_mission);
	
	LabelTTFMarquee* mission_label = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), mission_size.size.width, mission_size.size.height, mySD->getConditionContent(stage_number).c_str());
	mission_label->setAnchorPoint(ccp(0.5,0.5));
	mission_label->setPosition(mission_position);
	main_case->addChild(mission_label);
	mission_label->setFontSize(13);
	mission_label->startMarquee();
	
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
	
	if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) > 10000)
	{
		CCSprite* n_back = CCSprite::create("cardchange_cancel.png");
		CCSprite* s_back = CCSprite::create("cardchange_cancel.png");
		s_back->setColor(ccGRAY);
		
		CCMenuItem* back_item = CCMenuItemSprite::create(n_back, s_back, this, menu_selector(StartSettingScene::menuAction));
		back_item->setTag(kStartSettingMenuTag_back);
		
		CCMenu* back_menu = CCMenu::createWithItem(back_item);
		back_menu->setPosition(ccp(main_case->getContentSize().width-28, main_case->getContentSize().height+40-65));
		main_case->addChild(back_menu);
	}
	
	
	selected_friend_idx = -1;
	setStageRank();
	
	
	use_item_price_gold = KSProtectVar<int>(0);
	use_item_price_ruby = KSProtectVar<int>(0);
	
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
		addChild(t_popup, kStartSettingZorder_popup);
	}
	
	clicked_item_idx = -1;
	
	for(int i=0;i<item_list.size() && i < 3;i++)
	{
		ITEM_CODE t_ic = item_list[i];
		
		CCPoint item_position = ccp(212.f + i*58.f, 163.5);
		
		deque<int>::iterator iter = find(card_options.begin(), card_options.end(), t_ic);
		if(iter == card_options.end()) // not same option card // enable item
		{
			CCNode* item_parent = CCNode::create();
			item_parent->setPosition(item_position);
			main_case->addChild(item_parent, 0, kStartSettingMenuTag_itemBase+i);
			
			
			bool is_before_used_item = mySGD->isBeforeUseItem(t_ic); // 이전에 사용했는지
			bool is_show_item_popup = false; // 처음 등장한 아이템인지
			for(int j=0;j<show_item_popup.size();j++)
			{
				if(show_item_popup[j] == t_ic)
				{
					is_show_item_popup = true;
					break;
				}
			}
			bool is_price_usable = false; // 소지하고 있거나 장착 가능한 가격
			is_price_usable = is_price_usable || (myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) > 0); // 소지하고 있는지
			string item_currency = mySD->getItemCurrency(t_ic);
			if(item_currency == "gold")
				is_price_usable = is_price_usable || ((use_item_price_gold.getV() + mySD->getItemPrice(t_ic)) <= mySGD->getGold());
			else if(item_currency == "ruby")
				is_price_usable = is_price_usable || ((use_item_price_ruby.getV() + mySD->getItemPrice(t_ic)) <= mySGD->getStar());
			
			if(getSelectedItemCount() < 3 && (is_before_used_item || is_show_item_popup) && is_price_usable)
			{
				// mount
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				CCSprite* n_mount = CCSprite::create("startsetting_item_mounted_case.png");
				n_mount->setBlendFunc(ccBlendFunc{GL_ONE, GL_ONE});
				n_mount->setPosition(ccp(n_item_case->getContentSize().width/2.f, n_item_case->getContentSize().height/2.f));
				n_item_case->addChild(n_mount);
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setScale(0.8);
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f+6));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_mount = CCSprite::create("startsetting_item_mounted_case.png");
				s_mount->setBlendFunc(ccBlendFunc{GL_ONE, GL_ONE});
				s_mount->setPosition(ccp(s_item_case->getContentSize().width/2.f, s_item_case->getContentSize().height/2.f));
				s_item_case->addChild(s_mount);
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setScale(0.8);
				s_img->setColor(ccGRAY);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f+6));
				s_item_case->addChild(s_img);
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
				item_item->setTag(i+1);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				
				is_selected_item.push_back(true);
				
				if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) <= 0)
				{
					if(item_currency == "gold")
						use_item_price_gold = use_item_price_gold.getV() + mySD->getItemPrice(t_ic);
					else if(item_currency == "ruby")
						use_item_price_ruby = use_item_price_ruby.getV() + mySD->getItemPrice(t_ic);
				}
			}
			else
			{
				// normal
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setScale(0.8);
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f+6));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setScale(0.8);
				s_img->setColor(ccGRAY);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f+6));
				s_item_case->addChild(s_img);
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
				item_item->setTag(i+1);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				
				is_selected_item.push_back(false);
			}
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			if(item_cnt > 0)
			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("소지 %d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setColor(ccBLACK);
				cnt_label->setPosition(ccp(0, -21.5));
				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
			}
			else
			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", mySD->getItemPrice(t_ic))->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setColor(ccBLACK);
				cnt_label->setPosition(ccp(5, -21.5));
				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
				
				string buy_type_filename;
				if(item_currency == "gold")
					buy_type_filename = "price_gold_img.png";
				else if(item_currency == "ruby")
					buy_type_filename = "price_ruby_img.png";
				
				CCSprite* buy_type = CCSprite::create(buy_type_filename.c_str());
				buy_type->setScale(0.5f);
				buy_type->setPosition(ccp(-13, -21.5));
				item_parent->addChild(buy_type);
			}
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(false);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingItemZorder_clicked, kStartSettingItemZorder_clicked);
		}
		else
			is_selected_item.push_back(false);
	}
	
	gacha_item = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 35, 35));
	gacha_item->setPosition(ccp(414,164));
	main_case->addChild(gacha_item);
	
	
	CommonButton* item_gacha = CommonButton::create("", CommonButtonGreen);
	item_gacha->setSize(CCSizeMake(65, 30));
	item_gacha->setPrice(PriceTypeGold, 1000);
	item_gacha->setPosition(ccp(414, 138));
	main_case->addChild(item_gacha);
	item_gacha->setFunction([=](CCObject* sender)
							{
								startItemGacha();
							});
	
	
	CCSprite* n_start = CCSprite::create("startsetting_start.png");
	CCSprite* s_start = CCSprite::create("startsetting_start.png");
	s_start->setColor(ccGRAY);
	
	CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(StartSettingScene::menuAction));
	start_item->setTag(kStartSettingMenuTag_start);
	
	CCMenu* start_menu = CCMenu::createWithItem(start_item);
	start_menu->setPosition(ccp(317, 44));
	main_case->addChild(start_menu);
	
	
	StoneType missile_type_code = StoneType(myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)%7);
	string missile_type_str;
	if(missile_type_code == kStoneType_guided)
		missile_type_str = "유도형";
	else if(missile_type_code == kStoneType_spread)
		missile_type_str = "방사형";
	else if(missile_type_code == kStoneType_range)
		missile_type_str = "범위형";
	else if(missile_type_code == kStoneType_mine)
		missile_type_str = "지뢰형";
	else if(missile_type_code == kStoneType_laser)
		missile_type_str = "레이저형";
	else if(missile_type_code == kStoneType_spirit)
		missile_type_str = "소환형";
	else if(missile_type_code == kStoneType_global)
		missile_type_str = "전체공격형";
	
	CCLabelTTF* missile_type = CCLabelTTF::create(missile_type_str.c_str(), mySGD->getFont().c_str(), 12);
	missile_type->setPosition(ccp(94,157));
	main_case->addChild(missile_type);
	
	int missile_level = myDSH->getIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))+1;
	missile_data = CCLabelTTF::create(CCString::createWithFormat("레벨 %d    파워 %d(임시)", missile_level, 999)->getCString(), mySGD->getFont().c_str(), 12);
	missile_data->setPosition(ccp(94,95));
	main_case->addChild(missile_data);
	
	
	if(missile_level >= 25)
	{
		upgrade_menu = NULL;
	}
	else
	{
		CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
		CCLabelTTF* n_level = CCLabelTTF::create(CCString::createWithFormat("레벨%d", missile_level+1)->getCString(), mySGD->getFont().c_str(), 10);
		n_level->setAnchorPoint(ccp(1,0.5));
		n_level->setPosition(ccp(60,40));
		n_upgrade->addChild(n_level);
		CCSprite* n_price_type = CCSprite::create("common_button_gold.png");
		n_price_type->setPosition(ccp(33,21));
		n_upgrade->addChild(n_price_type);
		CCLabelTTF* n_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 10);
		n_price_label->setPosition(ccp(78,21));
		n_upgrade->addChild(n_price_label);
		
		CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
		s_upgrade->setColor(ccGRAY);
		CCLabelTTF* s_level = CCLabelTTF::create(CCString::createWithFormat("레벨%d", missile_level+1)->getCString(), mySGD->getFont().c_str(), 10);
		s_level->setAnchorPoint(ccp(1,0.5));
		s_level->setPosition(ccp(60,40));
		s_upgrade->addChild(s_level);
		CCSprite* s_price_type = CCSprite::create("common_button_gold.png");
		s_price_type->setPosition(ccp(33,21));
		s_upgrade->addChild(s_price_type);
		CCLabelTTF* s_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 10);
		s_price_label->setPosition(ccp(78,21));
		s_upgrade->addChild(s_price_label);
		
		CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingScene::upgradeAction));
		
		upgrade_menu = CCMenu::createWithItem(upgrade_item);
		upgrade_menu->setPosition(ccp(93,46));
		main_case->addChild(upgrade_menu);
	}
}

void StartSettingScene::upgradeAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	int upgrade_price = myDSH->getIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))+1;
	upgrade_price*=1000;
	if(mySGD->getGold() < upgrade_price)
		return;
	
	is_menu_enable = false;
	
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
	addChild(t_popup, kStartSettingZorder_popup);
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	case_back->setPosition(ccp(0,0));
	t_container->addChild(case_back);
	
	case_back->setContentSize(CCSizeMake(220, 190));
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	content_back->setPosition(ccp(0,25));
	t_container->addChild(content_back);
	
	content_back->setContentSize(CCSizeMake(200, 120));
	
	CCLabelTTF* ment1_label = CCLabelTTF::create("정말 업그레이드 하시겠습니까?", mySGD->getFont().c_str(), 15);
	ment1_label->setPosition(ccp(0,25));
	t_container->addChild(ment1_label);
	
	
	
	CommonButton* cancel_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
	cancel_button->setPosition(ccp(100,85));
	cancel_button->setFunction([=](CCObject* sender)
							   {
								   is_menu_enable = true;
								   t_popup->removeFromParent();
							   });
	t_container->addChild(cancel_button);
	
	
	CommonButton* ok_button = CommonButton::create("확인", 15, CCSizeMake(110, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
	ok_button->setPosition(ccp(0,-65));
	ok_button->setFunction([=](CCObject* sender)
						   {
							   int missile_level = myDSH->getIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))+1;
							   mySGD->setGold(mySGD->getGold()-missile_level*1000);
							   myDSH->setIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter), missile_level);
							   
							   myDSH->saveUserData({kSaveUserData_Key_gold, kSaveUserData_Key_character}, nullptr);
							   
							   missile_level++;
							   missile_data->setString(CCString::createWithFormat("레벨 %d    파워 %d(임시)", missile_level, 999)->getCString());
							   
							   
							   CCPoint upgrade_position = upgrade_menu->getPosition();
							   upgrade_menu->removeFromParent();
							   
							   if(missile_level >= 25)
							   {
								   upgrade_menu = NULL;
							   }
							   else
							   {
								   CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
								   CCLabelTTF* n_level = CCLabelTTF::create(CCString::createWithFormat("레벨%d", missile_level+1)->getCString(), mySGD->getFont().c_str(), 10);
								   n_level->setAnchorPoint(ccp(1,0.5));
								   n_level->setPosition(ccp(60,40));
								   n_upgrade->addChild(n_level);
								   CCSprite* n_price_type = CCSprite::create("common_button_gold.png");
								   n_price_type->setPosition(ccp(33,21));
								   n_upgrade->addChild(n_price_type);
								   CCLabelTTF* n_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 10);
								   n_price_label->setPosition(ccp(78,21));
								   n_upgrade->addChild(n_price_label);
								   
								   CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
								   s_upgrade->setColor(ccGRAY);
								   CCLabelTTF* s_level = CCLabelTTF::create(CCString::createWithFormat("레벨%d", missile_level+1)->getCString(), mySGD->getFont().c_str(), 10);
								   s_level->setAnchorPoint(ccp(1,0.5));
								   s_level->setPosition(ccp(60,40));
								   s_upgrade->addChild(s_level);
								   CCSprite* s_price_type = CCSprite::create("common_button_gold.png");
								   s_price_type->setPosition(ccp(33,21));
								   s_upgrade->addChild(s_price_type);
								   CCLabelTTF* s_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 10);
								   s_price_label->setPosition(ccp(78,21));
								   s_upgrade->addChild(s_price_label);
								   
								   CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingScene::upgradeAction));
								   
								   upgrade_menu = CCMenu::createWithItem(upgrade_item);
								   upgrade_menu->setPosition(upgrade_position);
								   main_case->addChild(upgrade_menu);
							   }
							   
							   is_menu_enable = true;
							   t_popup->removeFromParent();
						   });
	t_container->addChild(ok_button);
	
}

void StartSettingScene::startItemGacha()
{
	if(!is_menu_enable || mySGD->getGold() < 1000)
		return;
	
	is_menu_enable = false;
	
	CCLog("start item gacha");
	
	mySGD->setGold(mySGD->getGold() - 1000);
	myDSH->saveUserData({kSaveUserData_Key_gold}, nullptr);
	
	if(selected_gacha_item > kIC_emptyBegin && selected_gacha_item < kIC_emptyEnd)
		selected_gacha_item = kIC_emptyBegin;
	
	gacha_item_frame = 0;
	schedule(schedule_selector(StartSettingScene::itemGachaAction), 1.f/20.f);
}
void StartSettingScene::itemGachaAction()
{
	gacha_item_frame++;
	
	CCPoint gacha_item_position = gacha_item->getPosition();
	gacha_item->removeFromParent();
	
	int t_item_code = 4;
	
	if(gacha_item_frame%4 == 0)
		t_item_code = 4;
	else if(gacha_item_frame%4 == 1)
		t_item_code = 5;
	else if(gacha_item_frame%4 == 2)
		t_item_code = 7;
	else if(gacha_item_frame%4 == 3)
		t_item_code = 10;
	
	gacha_item = CCSprite::create(CCString::createWithFormat("item%d.png", t_item_code)->getCString());
	gacha_item->setScale(0.8f);
	gacha_item->setPosition(gacha_item_position);
	main_case->addChild(gacha_item);
	
	if(gacha_item_frame == 25)
	{
		gacha_item_cover = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 40, 40));
		gacha_item_cover->setScale(0.f);
		gacha_item_cover->setPosition(gacha_item->getPosition());
		main_case->addChild(gacha_item_cover, 1);
		
		CCScaleTo* t_scale = CCScaleTo::create(0.15f, 1.f);
		CCDelayTime* t_delay = CCDelayTime::create(0.25f);
		CCFadeTo* t_fade = CCFadeTo::create(0.6f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartSettingScene::endItemGacha));
		CCSequence* t_seq = CCSequence::create(t_scale, t_delay, t_fade, t_call, NULL);
		gacha_item_cover->runAction(t_seq);
	}
	else if(gacha_item_frame >= 30)
	{
		stopItemGacha();
	}
}
void StartSettingScene::stopItemGacha()
{
	int random_value = rand()%1000;
	if(random_value < 250)
		selected_gacha_item = kIC_fast;
	else if(random_value < 500)
		selected_gacha_item = kIC_subOneDie;
	else if(random_value < 800)
		selected_gacha_item = kIC_silence;
	else
		selected_gacha_item = kIC_heartUp;
	
	CCPoint gacha_item_position = gacha_item->getPosition();
	gacha_item->removeFromParent();
	
	gacha_item = CCSprite::create(CCString::createWithFormat("item%d.png", selected_gacha_item)->getCString());
	gacha_item->setScale(0.8f);
	gacha_item->setPosition(gacha_item_position);
	main_case->addChild(gacha_item);
	
	unschedule(schedule_selector(StartSettingScene::itemGachaAction));
}

void StartSettingScene::endItemGacha()
{
	is_menu_enable = true;
}

int StartSettingScene::getSelectedItemCount()
{
	int selected_item_cnt = 0;
	for(int k=0;k<is_selected_item.size();k++)
		if(is_selected_item[k])
			selected_item_cnt++;
	
	return selected_item_cnt;
}

void StartSettingScene::setStageRank()
{
//	friend_list.clear();
//	rank_table = NULL;
//	
//	Json::Value p;
//	Json::Value my_info = hspConnector::get()->myKakaoInfo;
//	
//	RankFriendInfo t_my_info;
//	t_my_info.nickname = my_info["nickname"].asString();
//	t_my_info.img_url = my_info["profile_image_url"].asString();
//	t_my_info.user_id = my_info["user_id"].asString();
//	t_my_info.score = 0;
//	t_my_info.is_play = false;
//	t_my_info.is_message_blocked = my_info["message_blocked"].asBool();
//	friend_list.push_back(t_my_info);
//	
//	p["memberIDList"].append(t_my_info.user_id);
//	
//	for(auto i : KnownFriends::getInstance()->getFriends())
//	{
//		RankFriendInfo t_friend_info;
//		t_friend_info.nickname = i.nick;
//		t_friend_info.img_url = i.profileUrl;
//		t_friend_info.user_id = i.userId;
//		t_friend_info.score = 0;
//		t_friend_info.is_play = false;
//		t_friend_info.is_message_blocked = i.messageBlocked;
//		friend_list.push_back(t_friend_info);
//		
//		p["memberIDList"].append(i.userId);
//	}
//	
//	for(auto i : UnknownFriends::getInstance()->getFriends())
//	{
//		RankFriendInfo fInfo;
//		fInfo.nickname = i.nick;
//		fInfo.img_url = "";
//		fInfo.user_id = i.userId;
//		fInfo.score = 0;
//		fInfo.is_play = false;
//		fInfo.is_message_blocked = false;
//		friend_list.push_back(fInfo);
//		
//		p["memberIDList"].append(i.userId);
//	}
//	
//	p["stageNo"] = mySD->getSilType();
//	hspConnector::get()->command("getstagescorelist",p,json_selector(this, StartSettingScene::resultGetStageScoreList));
}

//void StartSettingScene::resultGetStageScoreList(Json::Value result_data)
//{
//	if(result_data["result"]["code"].asInt() == GDSUCCESS && result_data["param"]["stageNo"].asInt() == mySD->getSilType() && !rank_table)
//	{
//		Json::Value score_list = result_data["list"];
//		for(int i=0;i<score_list.size();i++)
//		{
//			vector<RankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asString());
//			if(iter != friend_list.end())
//			{
//				(*iter).score = score_list[i]["score"].asFloat();
//				(*iter).is_play = true;
//			}
//			else
//				CCLog("not found friend memberID : %s", score_list[i]["memberID"].asString().c_str());
//		}
//		
//		auto beginIter = std::remove_if(friend_list.begin(), friend_list.end(), [=](RankFriendInfo t_info)
//										{
//											return !t_info.is_play;
//										});
//		friend_list.erase(beginIter, friend_list.end());
//		
//		struct t_FriendSort{
//			bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
//			{
//				return a.score > b.score;
//			}
//		} pred;
//		
//		sort(friend_list.begin(), friend_list.end(), pred);
//		
//		for(int i=0;i<friend_list.size();i++)
//			friend_list[i].rank = i+1;
//		
//		mySGD->save_stage_rank_stageNumber = mySD->getSilType();
//		mySGD->save_stage_rank_list = friend_list;
//		
//		selected_friend_idx = -1;
//		
//		CCSize table_size = CCSizeMake(198, 175);
//		CCPoint table_position = ccp(30,20);
//		
////		CCSprite* temp_table = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
////		temp_table->setAnchorPoint(CCPointZero);
////		temp_table->setPosition(table_position);
////		temp_table->setOpacity(100);
////		main_case->addChild(temp_table);
//		
//		rank_table = CCTableView::create(this, table_size);
//		rank_table->setAnchorPoint(CCPointZero);
//		rank_table->setDirection(kCCScrollViewDirectionVertical);
//		rank_table->setVerticalFillOrder(kCCTableViewFillTopDown);
//		rank_table->setPosition(table_position);
//		
//		rank_table->setDelegate(this);
//		main_case->addChild(rank_table);
////		rank_table->setTouchPriority(touch_priority-1);
//	}
//}

CCTableViewCell* StartSettingScene::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
//	RankFriendInfo* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
//	string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
//	string cell_id = (*member).user_id;
//	
//	CCSprite* back_img;
//	if(my_id == cell_id)
//		back_img = CCSprite::create("puzzle_right_ranklist_me.png");
//	else if(idx == 0)
//		back_img = CCSprite::create("puzzle_right_ranklist_gold.png");
//	else if(idx == 1)
//		back_img = CCSprite::create("puzzle_right_ranklist_silver.png");
//	else if(idx == 2)
//		back_img = CCSprite::create("puzzle_right_ranklist_bronze.png");
//	else
//		back_img = CCSprite::create("puzzle_right_ranklist_normal.png");
//	back_img->setPosition(CCPointZero);
//	back_img->setAnchorPoint(CCPointZero);
//	cell->addChild(back_img);
//	
//	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
//	profileImg->setAnchorPoint(ccp(0.5, 0.5));
//	profileImg->setPosition(ccp(21, 21));
//	cell->addChild(profileImg);
//	
//	if(my_id != cell_id && KnownFriends::getInstance()->findById(cell_id) != nullptr)
//	{
//		CCSprite* kakao_sign = CCSprite::create("puzzle_right_rank_kakao.png");
//		kakao_sign->setPosition(ccp(10,28));
//		cell->addChild(kakao_sign);
//	}
//	
//	CCLabelTTF* nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 10);
//	nickname_label->setPosition(ccp(89,27));
//	cell->addChild(nickname_label);
//	
//	CCLabelTTF* score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 8);
//	score_label->setColor(ccBLACK);
//	score_label->setPosition(ccp(89,12));
//	cell->addChild(score_label);
//	
//	if(idx == 0)
//	{
//		CCSprite* medal_img = CCSprite::create("puzzle_right_rank_gold.png");
//		medal_img->setPosition(ccp(50,20));
//		cell->addChild(medal_img);
//	}
//	else if(idx == 1)
//	{
//		CCSprite* medal_img = CCSprite::create("puzzle_right_rank_silver.png");
//		medal_img->setPosition(ccp(50,20));
//		cell->addChild(medal_img);
//	}
//	else if(idx == 2)
//	{
//		CCSprite* medal_img = CCSprite::create("puzzle_right_rank_bronze.png");
//		medal_img->setPosition(ccp(50,20));
//		cell->addChild(medal_img);
//	}
//	else
//	{
//		CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", (*member).rank)->getCString(), mySGD->getFont().c_str(), 14);
//		rank_label->setPosition(ccp(50,20));
//		cell->addChild(rank_label);
//	}
//	
//	if(selected_friend_idx == idx)
//	{
//		CCSprite* selected_img = CCSprite::create("puzzle_right_rank_selected.png");
//		selected_img->setPosition(CCPointZero);
//		selected_img->setAnchorPoint(CCPointZero);
//		cell->addChild(selected_img);
//	}
	
	return cell;
}

void StartSettingScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
//	CCLog("touched cell idx : %d", cell->getIdx());
//	
//	string touched_id = friend_list[cell->getIdx()].user_id;
//	string touched_nick = friend_list[cell->getIdx()].nickname;
//	float touched_score = friend_list[cell->getIdx()].score;
//	string touched_profile = friend_list[cell->getIdx()].img_url;
//	
//	string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
//	
//	if(touched_id != my_id)
//	{
//		if(selected_friend_idx == -1)
//		{
//			selected_friend_idx = cell->getIdx();
//			table->updateCellAtIndex(selected_friend_idx);
//			
//			mySGD->setMeChallengeTarget(touched_id, touched_nick, touched_score, touched_profile);
//		}
//		else if (cell->getIdx() != selected_friend_idx)
//		{
//			int keep_idx = selected_friend_idx;
//			selected_friend_idx = cell->getIdx();
//			table->updateCellAtIndex(keep_idx);
//			table->updateCellAtIndex(selected_friend_idx);
//			
//			mySGD->setMeChallengeTarget(touched_id, touched_nick, touched_score, touched_profile);
//		}
//		else
//		{
//			int keep_idx = selected_friend_idx;
//			selected_friend_idx = -1;
//			table->updateCellAtIndex(keep_idx);
//			
//			mySGD->setIsMeChallenge(false);
//		}
//	}
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
		
		if(clicked_item_idx != -1)
		{
			CCNode* before_item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+clicked_item_idx);
			CCSprite* before_clicked_img = (CCSprite*)before_item_parent->getChildByTag(kStartSettingItemZorder_clicked);
			before_clicked_img->setVisible(false);
		}
		
		clicked_item_idx = tag-1;
		
		
		if(is_selected_item[tag-1]) // selected -> unselect
		{
			CCNode* item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+tag-1);
			item_parent->removeAllChildren();
			
			
			ITEM_CODE t_ic = item_list[tag-1];
			
			CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
			CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			n_img->setScale(0.8);
			n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f+6));
			n_item_case->addChild(n_img);
			
			CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
			s_item_case->setColor(ccGRAY);
			CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			s_img->setScale(0.8);
			s_img->setColor(ccGRAY);
			s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f+6));
			s_item_case->addChild(s_img);
			
			CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
			item_item->setTag(tag);
			
			CCMenu* item_menu = CCMenu::createWithItem(item_item);
			item_menu->setPosition(CCPointZero);
			item_parent->addChild(item_menu);
			
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			if(item_cnt > 0)
			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("소지 %d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setColor(ccBLACK);
				cnt_label->setPosition(ccp(0, -21.5));
				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
			}
			else
			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", mySD->getItemPrice(t_ic))->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setColor(ccBLACK);
				cnt_label->setPosition(ccp(5, -21.5));
				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
				
				string item_currency = mySD->getItemCurrency(t_ic);
				string buy_type_filename;
				if(item_currency == "gold")
					buy_type_filename = "price_gold_img.png";
				else if(item_currency == "ruby")
					buy_type_filename = "price_ruby_img.png";
				
				CCSprite* buy_type = CCSprite::create(buy_type_filename.c_str());
				buy_type->setScale(0.5f);
				buy_type->setPosition(ccp(-13, -21.5));
				item_parent->addChild(buy_type);
			}
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(true);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingItemZorder_clicked, kStartSettingItemZorder_clicked);
			
			
			if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) <= 0)
			{
				string item_currency = mySD->getItemCurrency(t_ic);
				if(item_currency == "gold")
					use_item_price_gold = use_item_price_gold.getV() - mySD->getItemPrice(t_ic);
				else if(item_currency == "ruby")
					use_item_price_ruby = use_item_price_ruby.getV() - mySD->getItemPrice(t_ic);
			}
			
			
			is_selected_item[tag-1] = false;
		}
		else // unselected -> selected or nothing
		{
			CCNode* item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+tag-1);
			item_parent->removeAllChildren();
			
			
			ITEM_CODE t_ic = item_list[tag-1];
			
			bool is_price_usable = false; // 소지하고 있거나 장착 가능한 가격
			is_price_usable = is_price_usable || (myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) > 0); // 소지하고 있는지
			string item_currency = mySD->getItemCurrency(t_ic);
			if(item_currency == "gold")
				is_price_usable = is_price_usable || ((use_item_price_gold.getV() + mySD->getItemPrice(t_ic)) <= mySGD->getGold());
			else if(item_currency == "ruby")
				is_price_usable = is_price_usable || ((use_item_price_ruby.getV() + mySD->getItemPrice(t_ic)) <= mySGD->getStar());
			
			if(getSelectedItemCount() < 3 && is_price_usable)
			{
				// mount
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				CCSprite* n_mount = CCSprite::create("startsetting_item_mounted_case.png");
				n_mount->setBlendFunc(ccBlendFunc{GL_ONE, GL_ONE});
				n_mount->setPosition(ccp(n_item_case->getContentSize().width/2.f, n_item_case->getContentSize().height/2.f));
				n_item_case->addChild(n_mount);
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setScale(0.8);
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f+6));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_mount = CCSprite::create("startsetting_item_mounted_case.png");
				s_mount->setBlendFunc(ccBlendFunc{GL_ONE, GL_ONE});
				s_mount->setPosition(ccp(s_item_case->getContentSize().width/2.f, s_item_case->getContentSize().height/2.f));
				s_item_case->addChild(s_mount);
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setScale(0.8);
				s_img->setColor(ccGRAY);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f+6));
				s_item_case->addChild(s_img);
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
				item_item->setTag(tag);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				
				is_selected_item[tag-1] = true;
				
				if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) <= 0)
				{
					if(item_currency == "gold")
						use_item_price_gold = use_item_price_gold.getV() + mySD->getItemPrice(t_ic);
					else if(item_currency == "ruby")
						use_item_price_ruby = use_item_price_ruby.getV() + mySD->getItemPrice(t_ic);
				}
			}
			else
			{
				if(getSelectedItemCount() >= 3)
					addChild(ASPopupView::getCommonNoti(-210, "아이템은 최대 3개까지\n선택이 가능합니다."), kStartSettingZorder_popup);
				else if(!is_price_usable)
					addChild(ASPopupView::getCommonNoti(-210, item_currency + "가 부족합니다."), kStartSettingZorder_popup);
				
				// normal
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setScale(0.8);
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f+6));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setScale(0.8);
				s_img->setColor(ccGRAY);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f+6));
				s_item_case->addChild(s_img);
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
				item_item->setTag(tag);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				
				is_selected_item[tag-1] = false;
			}
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			if(item_cnt > 0)
			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("소지 %d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setColor(ccBLACK);
				cnt_label->setPosition(ccp(0, -21.5));
				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
			}
			else
			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", mySD->getItemPrice(t_ic))->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setColor(ccBLACK);
				cnt_label->setPosition(ccp(5, -21.5));
				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
				
				string item_currency = mySD->getItemCurrency(t_ic);
				string buy_type_filename;
				if(item_currency == "gold")
					buy_type_filename = "price_gold_img.png";
				else if(item_currency == "ruby")
					buy_type_filename = "price_ruby_img.png";
				
				CCSprite* buy_type = CCSprite::create(buy_type_filename.c_str());
				buy_type->setScale(0.5f);
				buy_type->setPosition(ccp(-13, -21.5));
				item_parent->addChild(buy_type);
			}
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(true);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingItemZorder_clicked, kStartSettingItemZorder_clicked);
		}
		
		if(item_title_label)
			item_title_label->removeFromParent();
		if(option_label)
			option_label->removeFromParent();
		
		CCRect title_size = CCRectMake(0, 0, 250, 20);
		CCPoint title_position = ccp(190, 115);
		
//		CCSprite* title_rect = CCSprite::create("whitePaper.png", title_size);
//		title_rect->setOpacity(100);
//		title_rect->setAnchorPoint(ccp(0,1));
//		title_rect->setPosition(title_position);
//		main_case->addChild(title_rect);
		
		item_title_label = CCLabelTTF::create(convertToItemCodeToItemName(item_list[tag-1]).c_str(), mySGD->getFont().c_str(), 14, title_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
		item_title_label->setAnchorPoint(ccp(0,1));
		item_title_label->setPosition(title_position);
		item_title_label->setColor(ccORANGE);
		main_case->addChild(item_title_label);
		
		CCRect option_size = CCRectMake(0, 0, 250, 23);
		CCPoint option_position = ccp(191, 96);
		
//		CCSprite* option_rect = CCSprite::create("whitePaper.png", option_size);
//		option_rect->setOpacity(100);
//		option_rect->setAnchorPoint(ccp(0,1));
//		option_rect->setPosition(option_position);
//		main_case->addChild(option_rect);
		
		option_label = CCLabelTTF::create(mySD->getItemScript(item_list[tag-1]).c_str(), mySGD->getFont().c_str(), 10, option_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
		option_label->setAnchorPoint(ccp(0,1));
		option_label->setPosition(option_position);
		main_case->addChild(option_label);
		
		
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
			if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) < 10000)
				CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
			else
				CCDirector::sharedDirector()->replaceScene(NewMainFlowScene::scene());
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
		else if(tag == kStartSettingMenuTag_back)
		{
			if(mySD->getSilType() < 10000)
				CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
			else
				CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
		}
		else if(tag == kStartSettingMenuTag_tip)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
			t_suction->target_touch_began = t_suction;
			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			t_suction->setTouchEnabled(true);
			
			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_startsetting.png");
			dimed_tip->setPosition(ccp(240,160));
			t_suction->addChild(dimed_tip);
			
			addChild(t_suction, kStartSettingZorder_popup);
			
			is_menu_enable = true;
		}
		else if(tag == kStartSettingMenuTag_start)
		{
//			bool is_have_usable_card = false;
//			int card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
//			for(int i=1;i<=card_take_cnt && !is_have_usable_card;i++)
//			{
//				int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
//				int card_durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number);
//				if(card_durability > 0)
//					is_have_usable_card = true;
//			}
//			
//			if(is_have_usable_card && myDSH->getIntegerForKey(kDSH_Key_selectedCard) == 0)
//			{
//				ASPopupView* t_popup = ASPopupView::create(-300);
//				
//				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//				if(screen_scale_x < 1.f)
//					screen_scale_x = 1.f;
//				
//				float height_value = 320.f;
//				if(myDSH->screen_convert_rate < 1.f)
//					height_value = 320.f/myDSH->screen_convert_rate;
//				
//				if(height_value < myDSH->ui_top)
//					height_value = myDSH->ui_top;
//				
//				t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
//				t_popup->setDimmedPosition(ccp(240, 160));
//				t_popup->setBasePosition(ccp(240, 160));
//				
//				CCNode* t_container = CCNode::create();
//				t_popup->setContainerNode(t_container);
//				addChild(t_popup, kStartSettingZorder_popup);
//				
//				CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//				case_back->setPosition(ccp(0,0));
//				t_container->addChild(case_back);
//				
//				case_back->setContentSize(CCSizeMake(260, 170));
//				
//				CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//				content_back->setPosition(ccp(0,25));
//				t_container->addChild(content_back);
//				
//				content_back->setContentSize(CCSizeMake(240, 100));
//				
//				CCLabelTTF* ment1_label = CCLabelTTF::create("카드가 장착되어있지 않습니다.",	mySGD->getFont().c_str(), 15);
//				ment1_label->setPosition(ccp(0,40));
//				t_container->addChild(ment1_label);
//				
//				CCLabelTTF* ment2_label = CCLabelTTF::create("계속하시겠습니까?", mySGD->getFont().c_str(), 15);
//				ment2_label->setPosition(ccp(0,10));
//				t_container->addChild(ment2_label);
//				
//				
//				CommonButton* cancel_button = CommonButton::create("장착하기", 15, CCSizeMake(100, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
//				cancel_button->setPosition(ccp(60,-55));
//				cancel_button->setFunction([=](CCObject* sender)
//										   {
//											   CardChangePopup* change_popup = CardChangePopup::create();
//											   change_popup->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupCloseCardSetting));
//											   addChild(change_popup, kStartSettingZorder_popup);
//
//											   t_popup->removeFromParent();
//										   });
//				t_container->addChild(cancel_button);
//				
//				
//				CommonButton* ok_button = CommonButton::create("그냥하기", 15, CCSizeMake(110, 50), CommonButtonGreen, t_popup->getTouchPriority()-5);
//				ok_button->setPosition(ccp(-60,-55));
//				ok_button->setFunction([=](CCObject* sender)
//									   {
//										   callStart();
//										   t_popup->removeFromParent();
//									   });
//				t_container->addChild(ok_button);
//			}
//			else
//			{
				callStart();
//			}
		}
		else if(tag == kStartSettingMenuTag_card)
		{
			CardChangePopup* t_popup = CardChangePopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupCloseCardSetting));
			addChild(t_popup, kStartSettingZorder_popup);
		}
		else if(tag == kStartSettingMenuTag_turn)
		{
			is_menu_enable = true;
		}
		else if(tag == kStartSettingMenuTag_itemBuy)
		{
//			ItemBuyPopup* t_ibp = ItemBuyPopup::create(item_list[clicked_item_idx], clicked_item_idx, this, callfuncII_selector(StartSettingScene::buySuccessItem));
//			t_ibp->setCloseTargetDelegate(this, callfunc_selector(StartSettingScene::popupClose));
//			addChild(t_ibp, kStartSettingZorder_popup);
		}
	}
}

void StartSettingScene::setTop()
{
	CCSprite* top_case = CCSprite::create("mainflow_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(258,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_case, kStartSettingZorder_top);
	
	CCSprite* n_cancel = CCSprite::create("startsetting_home.png");
	CCSprite* s_cancel = CCSprite::create("startsetting_home.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(StartSettingScene::menuAction));
	cancel_item->setTag(kStartSettingMenuTag_cancel);
	
	CCMenu* cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(-32,top_case->getContentSize().height/2.f));
	top_case->addChild(cancel_menu);
	
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(16,top_case->getContentSize().height/2.f-0.5f));
	top_case->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(StartSettingScene::menuAction));
	heart_item->setTag(kStartSettingMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(120,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_menu);
	
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(178,top_case->getContentSize().height/2.f-5));
	top_case->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(StartSettingScene::menuAction));
	gold_item->setTag(kStartSettingMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(212,top_case->getContentSize().height/2.f));
	top_case->addChild(gold_menu);
	
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getStar())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(261,top_case->getContentSize().height/2.f-5));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(StartSettingScene::menuAction));
	ruby_item->setTag(kStartSettingMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(287,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_menu);

	
	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(StartSettingScene::menuAction));
	tip_item->setTag(kStartSettingMenuTag_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(394, top_case->getContentSize().height/2.f));
	top_case->addChild(tip_menu);
}

void StartSettingScene::callStart()
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
		addChild(t_popup, kStartSettingZorder_popup);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setPosition(ccp(0,0));
		t_container->addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(220, 190));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		content_back->setPosition(ccp(0,25));
		t_container->addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(200, 120));
		
		CCLabelTTF* ment1_label = CCLabelTTF::create("하트가 부족합니다.", mySGD->getFont().c_str(), 15);
		ment1_label->setPosition(ccp(0,35));
		t_container->addChild(ment1_label);
		
		CCLabelTTF* ment2_label = CCLabelTTF::create("구매하러 가시겠습니까?", mySGD->getFont().c_str(), 15);
		ment2_label->setPosition(ccp(0,15));
		t_container->addChild(ment2_label);
		
		
		
		CommonButton* cancel_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
		cancel_button->setPosition(ccp(100,85));
		cancel_button->setFunction([=](CCObject* sender)
								   {
									   is_menu_enable = true;
									   t_popup->removeFromParent();
								   });
		t_container->addChild(cancel_button);
		
		
		CommonButton* ok_button = CommonButton::create("확인", 15, CCSizeMake(110, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
		ok_button->setPosition(ccp(0,-65));
		ok_button->setFunction([=](CCObject* sender)
							   {
								   ShopPopup* t_shop = ShopPopup::create();
								   t_shop->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupClose));
								   t_shop->targetHeartTime(heart_time);
								   t_shop->setShopCode(kSC_heart);
								   t_shop->setShopBeforeCode(kShopBeforeCode_startsetting);
								   addChild(t_shop, kStartSettingZorder_popup);
								   t_popup->removeFromParent();
							   });
		t_container->addChild(ok_button);
		
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
	
	
	if(mySGD->getRemoveMessageMailNo() != 0 && mySGD->getRemoveMessageMemberId() != 0)
	{
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
	else
	{
		was_end_startAction = true;
		was_end_removeMessage = true;
		goToGame();
	}
}
void StartSettingScene::finalSetting()
{
	start_loading = LoadingLayer::create();
	addChild(start_loading, kStartSettingZorder_popup);
	
	is_have_item.clear();
	is_have_item.push_back(false);
	
	deque<bool> is_using_item;
	is_using_item.push_back(false);
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
	{
		is_using_item.push_back(false);
		is_have_item.push_back(false);
	}
	
	for(int i=0;i<is_selected_item.size();i++)
	{
		if(is_selected_item[i])
		{
			if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i]) > 0)
			{
				myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i])-1);
				is_have_item[i] = true;
			}
			myLog->addLog(kLOG_useItem_s, -1, convertToItemCodeToItemName(item_list[i]).c_str());
			is_using_item[item_list[i]] = true;
		}
	}
	
	if(selected_gacha_item > kIC_emptyBegin && selected_gacha_item < kIC_emptyEnd)
	{
		is_using_item[selected_gacha_item] = true;
	}
	
	mySGD->setGold(mySGD->getGold() - use_item_price_gold.getV());
	mySGD->setStar(mySGD->getStar() - use_item_price_ruby.getV());
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
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
	mySGD->setRemoveMessageMailNo(0);
	mySGD->setRemoveMessageMemberId(0);
	
	mySGD->is_before_selected_event_stage = is_before_selected_event_stage;
	
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
	
	Json::Value param;
	param["key"] = CCString::createWithFormat("stage_start_%d", mySD->getSilType())->getCString();
	
	hspConnector::get()->command("increaseStats", param, NULL);
	
	mySGD->resetLabels();
	myGD->resetGameData();
	
	mySGD->setGameStart();
	
	if(mySD->getSilType() == 1)
	{
		mySGD->setNextSceneName("playtutorial");
		CCDirector::sharedDirector()->replaceScene(LoadingTipScene::scene());
	}
	else
	{
		mySGD->setNextSceneName("maingame");
		CCDirector::sharedDirector()->replaceScene(LoadingTipScene::scene());
	}
//	CCDirector::sharedDirector()->replaceScene(Maingame::scene());
}

void StartSettingScene::cancelGame()
{
	if(!is_menu_enable)
	{
		start_loading->removeFromParent();
		
		CCLog("Fail : user data save");
		
		deque<bool> is_using_item;
		is_using_item.push_back(false);
		for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
			is_using_item.push_back(false);
		
		for(int i=0;i<is_selected_item.size();i++)
		{
			if(is_selected_item[i])
			{
				if(is_have_item[i])
					myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i])+1);
				is_using_item[item_list[i]] = true;
			}
		}
				   
		mySGD->setGold(mySGD->getGold() + use_item_price_gold.getV());
		mySGD->setStar(mySGD->getStar() + use_item_price_ruby.getV());
		
		mySGD->resetUsingItem();
		
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
//	removeChildByTag(kStartSettingZorder_popup);
	
}

void StartSettingScene::durabilityOkAction(CCObject *sender)
{
//	removeChildByTag(kStartSettingZorder_popup);
	
	heart_time->startGame();
	realStartAction();
}

void StartSettingScene::popupClose()
{
	is_menu_enable = true;
}

void StartSettingScene::popupCloseCardSetting()
{
//	changeCard();
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
	if(getSelectedItemCount() > 0)
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

string StartSettingScene::convertToItemCodeToItemName(ITEM_CODE t_code)
{
	string return_value;
	if(t_code == kIC_attack)				return_value = "Attack";
	else if(t_code == kIC_speedUp)			return_value = "SpeedUp";
	else if(t_code == kIC_addTime)			return_value = "AddTime";
	else if(t_code == kIC_fast)				return_value = "Fast";
	else if(t_code == kIC_subOneDie)		return_value = "SubOneDie";
	else if(t_code == kIC_doubleItem)		return_value = "DoubleItem";
	else if(t_code == kIC_silence)			return_value = "Silence";
	else if(t_code == kIC_longTime)			return_value = "LongTime";
	else if(t_code == kIC_baseSpeedUp)		return_value = "BaseSpeedUp";
	
	return return_value.c_str();
}

void StartSettingScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void StartSettingScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(StartSettingScene::alertAction));
}