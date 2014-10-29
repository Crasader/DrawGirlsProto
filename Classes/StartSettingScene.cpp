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
#include "StoneMissile.h"
#include "KSLabelTTF.h"
#include "FormSetter.h"
#include "MailPopup.h"
#include "OptionPopup.h"
#include "AchievePopup.h"
#include "MyLocalization.h"

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
	kStartSettingMenuTag_postbox,
	kStartSettingMenuTag_achieve,
	kStartSettingMenuTag_option,
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
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160));
	addChild(main_case, kStartSettingZorder_main);
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("startsetting_left_back.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_back->setContentSize(CCSizeMake(152, 232));
	left_back->setPosition(ccp(main_case->getContentSize().width*0.174f,main_case->getContentSize().height*0.44f));
	main_case->addChild(left_back);
	
	CCScale9Sprite* left_front = CCScale9Sprite::create("startsetting_left_front.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_front->setContentSize(CCSizeMake(138, 155));
	left_front->setPosition(ccp(main_case->getContentSize().width*0.173f,main_case->getContentSize().height*0.56f));
	main_case->addChild(left_front);
	
	CCSprite* left_tab = CCSprite::create("startsetting_tab.png");
	left_tab->setPosition(ccp(55,225));
	main_case->addChild(left_tab);
	
	KSLabelTTF* tab_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_mymissile), mySGD->getFont().c_str(), 11);
	tab_label->setColor(ccc3(54, 35, 12));
	tab_label->enableOuterStroke(ccc3(54, 35, 12), 0.25f);
	tab_label->setPosition(ccp(36.5f,12.5f));
	left_tab->addChild(tab_label);
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_back->setContentSize(CCSizeMake(310, 232));
	right_back->setPosition(main_case->getContentSize().width*0.66f, main_case->getContentSize().height*0.44f);
	main_case->addChild(right_back);
	
	
//	float screen_height = myDSH->ui_top-320.f;
//	if(screen_height < 0.f)		screen_height = 0.f;
//	screen_height /= 4.f;
//	
//	
//	main_case = CCSprite::create("startsetting_back.png");
//	main_case->setAnchorPoint(ccp(0.5,0));
//	main_case->setPosition(ccp(240,3-screen_height));
//	addChild(main_case, kStartSettingZorder_main);
	
	
	int stage_number;
	
	if(mySGD->is_before_selected_event_stage)
	{
		stage_number = mySD->getSilType();
		
		KSLabelTTF* stage_number_label = KSLabelTTF::create(CCString::createWithFormat("%d 스테이지", stage_number)->getCString(),	mySGD->getFont().c_str(), 15);
		stage_number_label->setPosition(ccp(65, 256));
		main_case->addChild(stage_number_label);
		
		is_before_selected_event_stage = true;
		mySGD->is_before_selected_event_stage = false;
	}
	else
	{
		stage_number = mySD->getSilType();
//		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
//		int piece_number = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
		
		KSLabelTTF* piece_number_label = KSLabelTTF::create(CCString::createWithFormat("%d 스테이지", stage_number)->getCString(),	mySGD->getFont().c_str(), 15);
		piece_number_label->setPosition(ccp(65, 256));
		main_case->addChild(piece_number_label);
		
		is_before_selected_event_stage = false;
	}
	
	CCRect mission_size = CCRectMake(0, 0, 210, 22);
	CCPoint mission_position = ccp(main_case->getContentSize().width/2.f+90, main_case->getContentSize().height+2-28);
	
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
	
	CCSprite* mission_img = CCSprite::create("startsetting_mission.png");
	mission_img->setPosition(ccp(198,255));
	main_case->addChild(mission_img);
	
	
//	if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) > 10000)
//	{
	CommonButton* back_button = CommonButton::createCloseButton();
	back_button->setPosition(ccp(main_case->getContentSize().width-28, main_case->getContentSize().height+40-65));
	back_button->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kStartSettingMenuTag_back);
								 menuAction(t_node);
							 });
	main_case->addChild(back_button);
//	}
	
	
	selected_friend_idx = -1;
	setStageRank();
	
	
//	use_item_price_gold = KSProtectVar<int>(0);
//	use_item_price_ruby = KSProtectVar<int>(0);
	
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
		addChild(t_popup, kStartSettingZorder_popup);
	}
	
	clicked_item_idx = -1;
	
	for(int i=0;i<item_list.size() && i < 3;i++)
	{
		ITEM_CODE t_ic = item_list[i];
		
		CCPoint item_position = ccp(205.f + i*71.f, 190);
		
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
//			string item_currency = mySD->getItemCurrency(t_ic);
//			if(item_currency == "gold")
//				is_price_usable = is_price_usable || ((use_item_price_gold.getV() + mySD->getItemPrice(t_ic)) <= mySGD->getGoodsValue(kGoodsType_gold));
//			else if(item_currency == "ruby")
//				is_price_usable = is_price_usable || ((use_item_price_ruby.getV() + mySD->getItemPrice(t_ic)) <= mySGD->getGoodsValue(kGoodsType_ruby));
			
			if(getSelectedItemCount() < 3 && (is_before_used_item || is_show_item_popup) && is_price_usable)
			{
				// mount
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				CCSprite* n_mount = CCSprite::create("startsetting_item_mounted_case.png");
				n_mount->setPosition(ccp(n_item_case->getContentSize().width - n_mount->getContentSize().width/2.f-6, n_item_case->getContentSize().height - n_mount->getContentSize().height/2.f-6));
				n_item_case->addChild(n_mount);
				
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_mount = CCSprite::create("startsetting_item_mounted_case.png");
				s_mount->setPosition(ccp(s_item_case->getContentSize().width - s_mount->getContentSize().width/2.f-6, s_item_case->getContentSize().height - s_mount->getContentSize().height/2.f-6));
				s_item_case->addChild(s_mount);
				
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setColor(ccGRAY);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
				s_item_case->addChild(s_img);
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
				item_item->setTag(i+1);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				
				is_selected_item.push_back(true);
				
//				if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) <= 0)
//				{
//					if(item_currency == "gold")
//						use_item_price_gold = use_item_price_gold.getV() + mySD->getItemPrice(t_ic);
//					else if(item_currency == "ruby")
//						use_item_price_ruby = use_item_price_ruby.getV() + mySD->getItemPrice(t_ic);
//				}
			}
			else
			{
				// normal
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setColor(ccGRAY);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
				s_item_case->addChild(s_img);
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
				item_item->setTag(i+1);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				
				is_selected_item.push_back(false);
			}
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
			cnt_label->setPosition(ccp(21, -21));
			item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
//			else
//			{
//				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", mySD->getItemPrice(t_ic))->getCString(), mySGD->getFont().c_str(), 10);
//				cnt_label->setPosition(ccp(5, -21.5));
//				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
//				
//				string buy_type_filename;
//				if(item_currency == "gold")
//					buy_type_filename = "price_gold_img.png";
//				else if(item_currency == "ruby")
//					buy_type_filename = "price_ruby_img.png";
//				
//				CCSprite* buy_type = CCSprite::create(buy_type_filename.c_str());
//				buy_type->setScale(0.5f);
//				buy_type->setPosition(ccp(-13, -21.5));
//				item_parent->addChild(buy_type);
//			}
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(false);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingItemZorder_clicked, kStartSettingItemZorder_clicked);
		}
		else
			is_selected_item.push_back(false);
	}
	
	item_gacha_menu = NULL;
	is_clicked_gacha_menu = false;
	
	gacha_clicked_img = CCSprite::create("startsetting_item_clicked.png");
	gacha_clicked_img->setPosition(ccp(425,190));
	gacha_clicked_img->setVisible(false);
	main_case->addChild(gacha_clicked_img, kStartSettingZorder_main+1);
	
	gachaMenuCreate();
	
//	CCSprite* gacha_mark = CCSprite::create("startsetting_item_gacha.png");
//	gacha_mark->setPosition(ccp(425,190));
//	main_case->addChild(gacha_mark);
	
	gacha_item = CCSprite::create("startsetting_item_gacha_inner.png");
	gacha_item->setPosition(ccp(425,190));
	main_case->addChild(gacha_item, kStartSettingZorder_main);
	
	KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gacha), mySGD->getFont().c_str(), 15);
	gacha_label->enableOuterStroke(ccBLACK, 1.f);
	gacha_label->setPosition(ccp(37.5f, 23.5f));
	gacha_item->addChild(gacha_label);
	
//	CommonButton* item_gacha = CommonButton::create("", CommonButtonGreen);
//	item_gacha->setSize(CCSizeMake(65, 30));
//	item_gacha->setPrice(PriceTypeGold, 1000);
//	item_gacha->setPosition(ccp(425, 165));
//	main_case->addChild(item_gacha);
//	item_gacha->setFunction([=](CCObject* sender)
//							{
//								startItemGacha();
//							});
	
	CCScale9Sprite* script_box = CCScale9Sprite::create("startsetting_scriptbox.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	script_box->setContentSize(CCSizeMake(290, 70));
	script_box->setPosition(ccp(317,117));
	main_case->addChild(script_box);
	
	// 구입 버튼 자리는 400,115
	
	
	CCSprite* n_start = CCSprite::create("startsetting_start.png");
	KSLabelTTF* n_start_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gamestart), mySGD->getFont().c_str(), 32.5f);
	n_start_label->setColor(ccc3(47, 30, 6));
	n_start_label->enableOuterStroke(ccc3(47, 30, 6), 0.25f);
	n_start_label->setPosition(ccp(147.5f,40.5f));
	n_start->addChild(n_start_label);
	
	CCSprite* s_start = CCSprite::create("startsetting_start.png");
	s_start->setColor(ccGRAY);
	KSLabelTTF* s_start_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gamestart), mySGD->getFont().c_str(), 32.5f);
	s_start_label->setColor(ccc3(47, 30, 6));
	s_start_label->enableOuterStroke(ccc3(47, 30, 6), 0.25f);
	s_start_label->setPosition(ccp(147.5f,40.5f));
	s_start->addChild(s_start_label);
	
	CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(StartSettingScene::menuAction));
	start_item->setTag(kStartSettingMenuTag_start);
	
	CCMenu* start_menu = CCMenu::createWithItem(start_item);
	start_menu->setPosition(ccp(317, 44));
	main_case->addChild(start_menu);
	
	
	CCSprite* level_case = CCSprite::create("startsetting_levelbox.png");
	level_case->setPosition(ccp(83,95));
	main_case->addChild(level_case);
	
	
	StoneType missile_type_code = StoneType(myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)%7);
	
	int missile_level = myDSH->getIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))+1;
	
	if(missile_type_code == kStoneType_guided)
	{
		int grade = (missile_level-1)/5+1;
		bool rotation = false;
		if(grade == 1 || grade == 4)
			rotation = true;
		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("jack_missile_%d.png", missile_level)->getCString(), rotation,
																														 (missile_level-1)/5+1, (missile_level-1)%5+1);
		t_gm->setPosition(ccp(83,158));
//		t_gm->beautifier((missile_level-1)/5+1, (missile_level-1)%5+1);
		main_case->addChild(t_gm);
		
		
		t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
		
		missile_img = t_gm;
	}
	
	missile_data_level = CCLabelTTF::create(CCString::createWithFormat("레벨 %d", missile_level)->getCString(), mySGD->getFont().c_str(), 12);
	missile_data_level->setAnchorPoint(ccp(0,0.5f));
	missile_data_level->setPosition(ccp(30,95));
	main_case->addChild(missile_data_level);
	
	missile_data_power = CCLabelTTF::create(CCString::createWithFormat("파워 %d", /*mySGD->getSelectedCharacterHistory().power.getV()*/1)->getCString(), mySGD->getFont().c_str(), 12);
	missile_data_power->setAnchorPoint(ccp(0,0.5f));
	missile_data_power->setPosition(ccp(84,95));
	main_case->addChild(missile_data_power);
	
	if(missile_level >= 25)
	{
		upgrade_menu = NULL;
	}
	else
	{
		CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
		CCLabelTTF* n_level = CCLabelTTF::create(CCString::createWithFormat("업그레이드 레벨 %d", missile_level+1)->getCString(), mySGD->getFont().c_str(), 12);
		n_level->setColor(ccBLACK);
		n_level->setPosition(ccp(70,47));
		n_upgrade->addChild(n_level);
		CCSprite* n_price_type = CCSprite::create("price_gold_img.png");
		n_price_type->setPosition(ccp(25,22));
		n_upgrade->addChild(n_price_type);
		CCLabelTTF* n_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 12);
		n_price_label->setColor(ccBLACK);
		n_price_label->setPosition(ccp(78,22));
		n_upgrade->addChild(n_price_label);
		
		CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
		s_upgrade->setColor(ccGRAY);
		CCLabelTTF* s_level = CCLabelTTF::create(CCString::createWithFormat("업그레이드 레벨 %d", missile_level+1)->getCString(), mySGD->getFont().c_str(), 12);
		s_level->setColor(ccBLACK);
		s_level->setPosition(ccp(70,47));
		s_upgrade->addChild(s_level);
		CCSprite* s_price_type = CCSprite::create("price_gold_img.png");
		s_price_type->setPosition(ccp(25,22));
		s_upgrade->addChild(s_price_type);
		CCLabelTTF* s_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 12);
		s_price_label->setColor(ccBLACK);
		s_price_label->setPosition(ccp(78,22));
		s_upgrade->addChild(s_price_label);
		
		CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingScene::upgradeAction));
		
		upgrade_menu = CCMenu::createWithItem(upgrade_item);
		upgrade_menu->setPosition(ccp(83,46));
		main_case->addChild(upgrade_menu);
	}
}

void StartSettingScene::gachaMenuCreate()
{
	if(item_gacha_menu)
		item_gacha_menu->removeFromParent();
	
	CCSprite* n_gacha = CCSprite::create("startsetting_item_gacha.png");
	CCSprite* s_gacha = CCSprite::create("startsetting_item_gacha.png");
	s_gacha->setColor(ccGRAY);
	
	CCMenuItemLambda* gacha_item_item = CCMenuItemSpriteLambda::create(n_gacha, s_gacha, [=](CCObject* sender)
																	   {
																		   if(!is_menu_enable)
																			   return;
																		   
																		   is_menu_enable = false;
																		   
																		   if(clicked_item_idx != -1)
																		   {
																			   CCNode* before_item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+clicked_item_idx);
																			   CCSprite* before_clicked_img = (CCSprite*)before_item_parent->getChildByTag(kStartSettingItemZorder_clicked);
																			   before_clicked_img->setVisible(false);
																		   }
																		   
																		   is_clicked_gacha_menu = true;
																		   
																		   if(item_title_label)
																			   item_title_label->removeFromParent();
																		   if(option_label)
																			   option_label->removeFromParent();
																		   if(buy_button)
																			   buy_button->removeFromParent();
																		   
																		   CCRect title_size = CCRectMake(0, 0, 200, 20);
																		   CCPoint title_position = ccp(188, 140);
																		   
																		   item_title_label = CCLabelTTF::create(convertToItemCodeToItemName(kIC_itemGacha).c_str(), mySGD->getFont().c_str(), 14, title_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
																		   item_title_label->setAnchorPoint(ccp(0,1));
																		   item_title_label->setPosition(title_position);
																		   item_title_label->setColor(ccORANGE);
																		   main_case->addChild(item_title_label);
																		   
																		   CCRect option_size = CCRectMake(0, 0, 200, 25);
																		   CCPoint option_position = ccp(188, 113);
																		   
																		   option_label = CCLabelTTF::create(mySD->getItemScript(kIC_itemGacha).c_str(), mySGD->getFont().c_str(), 12, option_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
																		   option_label->setAnchorPoint(ccp(0,1));
																		   option_label->setPosition(option_position);
																		   main_case->addChild(option_label);
																		   
																		   buy_button = CommonButton::create("", CommonButtonYellowUp);
																		   buy_button->setSize(CCSizeMake(90, 55));
																		   buy_button->setPrice(PriceTypeGold, 1000);
																		   buy_button->setFunction([=](CCObject* sender)
																								   {
																									   this->startItemGacha();
																								   });
																		   buy_button->setPosition(410,117);
																		   main_case->addChild(buy_button);
																		   
																		   gacha_clicked_img->setVisible(true);
																		   
																		   is_menu_enable = true;
																	   });
	
	item_gacha_menu = CCMenuLambda::createWithItem(gacha_item_item);
	item_gacha_menu->setPosition(ccp(425,190));
	main_case->addChild(item_gacha_menu);
}

void StartSettingScene::upgradeAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	int upgrade_price = myDSH->getIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))+1;
	upgrade_price*=1000;
	if(mySGD->getGoodsValue(kGoodsType_gold) < upgrade_price)// + use_item_price_gold.getV())
	{
		addChild(ASPopupView::getCommonNoti(-300, "골드가 부족합니다."), kStartSettingZorder_popup);
		return;
	}
	
	is_menu_enable = false;
	
//	ASPopupView* t_popup = ASPopupView::create(-300);
//	
//	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//	if(screen_scale_x < 1.f)
//		screen_scale_x = 1.f;
//	
//	float height_value = 320.f;
//	if(myDSH->screen_convert_rate < 1.f)
//		height_value = 320.f/myDSH->screen_convert_rate;
//	
//	if(height_value < myDSH->ui_top)
//		height_value = myDSH->ui_top;
//	
//	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
//	t_popup->setDimmedPosition(ccp(240, 160));
//	t_popup->setBasePosition(ccp(240, 160));
//	
//	CCNode* t_container = CCNode::create();
//	t_popup->setContainerNode(t_container);
//	addChild(t_popup, kStartSettingZorder_popup);
//	
//	CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//	case_back->setPosition(ccp(0,0));
//	t_container->addChild(case_back);
//	
//	case_back->setContentSize(CCSizeMake(220, 190));
//	
//	CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//	content_back->setPosition(ccp(0,25));
//	t_container->addChild(content_back);
//	
//	content_back->setContentSize(CCSizeMake(200, 120));
//	
//	CCLabelTTF* ment1_label = CCLabelTTF::create("정말 업그레이드 하시겠습니까?", mySGD->getFont().c_str(), 15);
//	ment1_label->setPosition(ccp(0,25));
//	t_container->addChild(ment1_label);
//	
//	
//	
//	CommonButton* cancel_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
//	cancel_button->setPosition(ccp(100,85));
//	cancel_button->setFunction([=](CCObject* sender)
//							   {
//								   is_menu_enable = true;
//								   t_popup->removeFromParent();
//							   });
//	t_container->addChild(cancel_button);
//	
//	
//	CommonButton* ok_button = CommonButton::create("확인", 15, CCSizeMake(110, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
//	ok_button->setPosition(ccp(0,-65));
//	ok_button->setFunction([=](CCObject* sender)
//						   {
							   int missile_level = myDSH->getIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))+1;
//							   mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold)-missile_level*1000);
							   myDSH->setIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter), missile_level);
							   
//							   myDSH->saveUserData({kSaveUserData_Key_gold, kSaveUserData_Key_character}, nullptr);
				
							   missile_level++;
							   missile_data_level->setString(CCString::createWithFormat("레벨 %d", missile_level)->getCString());
							   missile_data_power->setString(CCString::createWithFormat("파워 %d", /*mySGD->getSelectedCharacterHistory().power.getV()*/1)->getCString());
							   
							   CCPoint missile_position;
							   if(missile_img)
								{
									missile_position = missile_img->getPosition();
								   missile_img->removeFromParent();
								}
							   
							   StoneType missile_type_code = StoneType(myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)%7);
							   
							   if(missile_type_code == kStoneType_guided)
							   {
								   int grade = (missile_level-1)/5+1;
									 bool rotation = false;
									 if(grade == 1 || grade == 4)
										 rotation = true;
								   GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("jack_missile_%d.png", missile_level)->getCString(), rotation,
																																						(missile_level-1)/5+1, (missile_level-1)%5+1);
								   t_gm->setPosition(missile_position);
//								   t_gm->beautifier((missile_level-1)/5+1, (missile_level-1)%5+1);
								   main_case->addChild(t_gm);
								   
								   
								   t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
								   
								   missile_img = t_gm;
							   }
							   
							   
							   
							   CCPoint upgrade_position = upgrade_menu->getPosition();
							   upgrade_menu->removeFromParent();
							   
							   if(missile_level >= 25)
							   {
								   upgrade_menu = NULL;
							   }
							   else
							   {
								   CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
								   CCLabelTTF* n_level = CCLabelTTF::create(CCString::createWithFormat("업그레이드 레벨 %d", missile_level+1)->getCString(), mySGD->getFont().c_str(), 12);
								   n_level->setColor(ccBLACK);
								   n_level->setPosition(ccp(70,47));
								   n_upgrade->addChild(n_level);
								   CCSprite* n_price_type = CCSprite::create("price_gold_img.png");
								   n_price_type->setPosition(ccp(25,22));
								   n_upgrade->addChild(n_price_type);
								   CCLabelTTF* n_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 12);
								   n_price_label->setColor(ccBLACK);
								   n_price_label->setPosition(ccp(78,22));
								   n_upgrade->addChild(n_price_label);
								   
								   CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
								   s_upgrade->setColor(ccGRAY);
								   CCLabelTTF* s_level = CCLabelTTF::create(CCString::createWithFormat("업그레이드 레벨 %d", missile_level+1)->getCString(), mySGD->getFont().c_str(), 12);
								   s_level->setColor(ccBLACK);
								   s_level->setPosition(ccp(70,47));
								   s_upgrade->addChild(s_level);
								   CCSprite* s_price_type = CCSprite::create("price_gold_img.png");
								   s_price_type->setPosition(ccp(25,22));
								   s_upgrade->addChild(s_price_type);
								   CCLabelTTF* s_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 12);
								   s_price_label->setColor(ccBLACK);
								   s_price_label->setPosition(ccp(78,22));
								   s_upgrade->addChild(s_price_label);
								   
								   CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingScene::upgradeAction));
								   
								   upgrade_menu = CCMenu::createWithItem(upgrade_item);
								   upgrade_menu->setPosition(upgrade_position);
								   main_case->addChild(upgrade_menu);
							   }
							   
							   is_menu_enable = true;
//							   t_popup->removeFromParent();
//						   });
//	t_container->addChild(ok_button);
	
}

void StartSettingScene::startItemGacha()
{
	if(!is_menu_enable || (/*use_item_price_gold.getV() + */1000) > mySGD->getGoodsValue(kGoodsType_gold))
		return;
	
	is_menu_enable = false;
	
	CCLOG("start item gacha");
	
//	mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) - 1000);
//	myDSH->saveUserData({kSaveUserData_Key_gold}, nullptr);
	
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
	gacha_item->setPosition(gacha_item_position);
	main_case->addChild(gacha_item, kStartSettingZorder_main);
	
	if(gacha_item_frame == 25)
	{
		gacha_item_cover = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 40, 40));
		gacha_item_cover->setScale(0.f);
		gacha_item_cover->setPosition(gacha_item->getPosition());
		main_case->addChild(gacha_item_cover, kStartSettingZorder_main+1);
		
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
	gacha_item->setPosition(gacha_item_position);
	main_case->addChild(gacha_item, kStartSettingZorder_main);
	
	CCSprite* mount_img = CCSprite::create("startsetting_item_mounted_case.png");
	mount_img->setPosition(ccp(gacha_item->getContentSize().width/2.f + 37.5f - mount_img->getContentSize().width/2.f-6, gacha_item->getContentSize().width/2.f + 37.5f - mount_img->getContentSize().height/2.f-6));
	gacha_item->addChild(mount_img);
	
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
//				CCLOG("not found friend memberID : %s", score_list[i]["memberID"].asString().c_str());
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
	
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
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
//	CCLOG("touched cell idx : %d", cell->getIdx());
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
		
		if(is_clicked_gacha_menu)
		{
			is_clicked_gacha_menu = false;
			gacha_clicked_img->setVisible(false);
		}
		
		clicked_item_idx = tag-1;
		
		
		if(is_selected_item[tag-1]) // selected -> unselect
		{
			CCNode* item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+tag-1);
			item_parent->removeAllChildren();
			
			
			ITEM_CODE t_ic = item_list[tag-1];
			
			CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
			CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
			n_item_case->addChild(n_img);
			
			CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
			s_item_case->setColor(ccGRAY);
			CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			s_img->setColor(ccGRAY);
			s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
			s_item_case->addChild(s_img);
			
			CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
			item_item->setTag(tag);
			
			CCMenu* item_menu = CCMenu::createWithItem(item_item);
			item_menu->setPosition(CCPointZero);
			item_parent->addChild(item_menu);
			
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
//			if(item_cnt > 0)
//			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setPosition(ccp(21, -21));
				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
//			}
//			else
//			{
//				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", mySD->getItemPrice(t_ic))->getCString(), mySGD->getFont().c_str(), 10);
//				cnt_label->setPosition(ccp(5, -21.5));
//				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
//				
//				string item_currency = mySD->getItemCurrency(t_ic);
//				string buy_type_filename;
//				if(item_currency == "gold")
//					buy_type_filename = "price_gold_img.png";
//				else if(item_currency == "ruby")
//					buy_type_filename = "price_ruby_img.png";
//				
//				CCSprite* buy_type = CCSprite::create(buy_type_filename.c_str());
//				buy_type->setScale(0.5f);
//				buy_type->setPosition(ccp(-13, -21.5));
//				item_parent->addChild(buy_type);
//			}
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(true);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingItemZorder_clicked, kStartSettingItemZorder_clicked);
			
			
//			if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) <= 0)
//			{
//				string item_currency = mySD->getItemCurrency(t_ic);
//				if(item_currency == "gold")
//					use_item_price_gold = use_item_price_gold.getV() - mySD->getItemPrice(t_ic);
//				else if(item_currency == "ruby")
//					use_item_price_ruby = use_item_price_ruby.getV() - mySD->getItemPrice(t_ic);
//			}
			
			
			is_selected_item[tag-1] = false;
		}
		else // unselected -> selected or nothing
		{
			CCNode* item_parent = (CCNode*)main_case->getChildByTag(kStartSettingMenuTag_itemBase+tag-1);
			item_parent->removeAllChildren();
			
			
			ITEM_CODE t_ic = item_list[tag-1];
			
			bool is_price_usable = false; // 소지하고 있거나 장착 가능한 가격
			is_price_usable = is_price_usable || (myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) > 0); // 소지하고 있는지
//			string item_currency = mySD->getItemCurrency(t_ic);
//			if(item_currency == "gold")
//				is_price_usable = is_price_usable || ((use_item_price_gold.getV() + mySD->getItemPrice(t_ic)) <= mySGD->getGoodsValue(kGoodsType_gold));
//			else if(item_currency == "ruby")
//				is_price_usable = is_price_usable || ((use_item_price_ruby.getV() + mySD->getItemPrice(t_ic)) <= mySGD->getGoodsValue(kGoodsType_ruby));
			
			if(getSelectedItemCount() < 3 && is_price_usable)
			{
				// mount
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				CCSprite* n_mount = CCSprite::create("startsetting_item_mounted_case.png");
				n_mount->setPosition(ccp(n_item_case->getContentSize().width - n_mount->getContentSize().width/2.f-6, n_item_case->getContentSize().height - n_mount->getContentSize().height/2.f-6));
				n_item_case->addChild(n_mount);
				
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_mount = CCSprite::create("startsetting_item_mounted_case.png");
				s_mount->setPosition(ccp(s_item_case->getContentSize().width - s_mount->getContentSize().width/2.f-6, s_item_case->getContentSize().height - s_mount->getContentSize().height/2.f-6));
				s_item_case->addChild(s_mount);
				
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setColor(ccGRAY);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
				s_item_case->addChild(s_img);
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
				item_item->setTag(tag);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				
				is_selected_item[tag-1] = true;
				
//				if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) <= 0)
//				{
//					if(item_currency == "gold")
//						use_item_price_gold = use_item_price_gold.getV() + mySD->getItemPrice(t_ic);
//					else if(item_currency == "ruby")
//						use_item_price_ruby = use_item_price_ruby.getV() + mySD->getItemPrice(t_ic);
//				}
			}
			else
			{
				if(getSelectedItemCount() >= 3)
					addChild(ASPopupView::getCommonNoti(-210, "아이템은 최대 3개까지\n선택이 가능합니다."), kStartSettingZorder_popup);
//				else if(!is_price_usable)
//					addChild(ASPopupView::getCommonNoti(-210, item_currency + "가 부족합니다."), kStartSettingZorder_popup);
				
				// normal
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setColor(ccGRAY);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
				s_item_case->addChild(s_img);
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingScene::itemAction));
				item_item->setTag(tag);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				
				is_selected_item[tag-1] = false;
			}
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
//			if(item_cnt > 0)
//			{
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
				cnt_label->setPosition(ccp(21, -21));
				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
//			}
//			else
//			{
//				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", mySD->getItemPrice(t_ic))->getCString(), mySGD->getFont().c_str(), 10);
//				cnt_label->setPosition(ccp(5, -21.5));
//				item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
//				
//				string item_currency = mySD->getItemCurrency(t_ic);
//				string buy_type_filename;
//				if(item_currency == "gold")
//					buy_type_filename = "price_gold_img.png";
//				else if(item_currency == "ruby")
//					buy_type_filename = "price_ruby_img.png";
//				
//				CCSprite* buy_type = CCSprite::create(buy_type_filename.c_str());
//				buy_type->setScale(0.5f);
//				buy_type->setPosition(ccp(-13, -21.5));
//				item_parent->addChild(buy_type);
//			}
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(true);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingItemZorder_clicked, kStartSettingItemZorder_clicked);
		}
		
		if(item_title_label)
			item_title_label->removeFromParent();
		if(option_label)
			option_label->removeFromParent();
		if(buy_button)
			buy_button->removeFromParent();
		
		CCRect title_size = CCRectMake(0, 0, 200, 20);
		CCPoint title_position = ccp(188, 140);
		
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
		
		CCRect option_size = CCRectMake(0, 0, 200, 25);
		CCPoint option_position = ccp(188, 113);
		
//		CCSprite* option_rect = CCSprite::create("whitePaper.png", option_size);
//		option_rect->setOpacity(100);
//		option_rect->setAnchorPoint(ccp(0,1));
//		option_rect->setPosition(option_position);
//		main_case->addChild(option_rect);
		
		option_label = CCLabelTTF::create(mySD->getItemScript(item_list[tag-1]).c_str(), mySGD->getFont().c_str(), 12, option_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
		option_label->setAnchorPoint(ccp(0,1));
		option_label->setPosition(option_position);
		main_case->addChild(option_label);
		
		string item_currency = mySD->getItemCurrency(item_list[tag-1]);
		string buy_type_filename;
		PriceType priceType;
		if(item_currency == "gold")
			priceType = PriceTypeGold;
		else if(item_currency == "ruby")
			priceType = PriceTypeRuby;
		
		buy_button = CommonButton::create("", CommonButtonYellowUp);
		buy_button->setSize(CCSizeMake(90, 55));
		buy_button->setPrice(priceType, mySD->getItemPrice(item_list[tag-1]));
		buy_button->setFunction([=](CCObject* sender)
								{
									if(!is_menu_enable)
										return;
									
									is_menu_enable = false;
									
									string item_currency = mySD->getItemCurrency(item_list[clicked_item_idx]);
									
									if(item_currency == "gold")
									{
//										if(mySD->getItemPrice(item_list[clicked_item_idx]) <= mySGD->getGoodsValue(kGoodsType_gold))
//										{
//											mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold)-mySD->getItemPrice(item_list[clicked_item_idx]));
											buySuccessItem(clicked_item_idx, 1);
//										}
//										else
//										{
//											ASPopupView* t_popup = ASPopupView::create(-500);
//											
//											CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//											float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//											if(screen_scale_x < 1.f)
//												screen_scale_x = 1.f;
//											
//											float height_value = 320.f;
//											if(myDSH->screen_convert_rate < 1.f)
//												height_value = 320.f/myDSH->screen_convert_rate;
//											
//											if(height_value < myDSH->ui_top)
//												height_value = myDSH->ui_top;
//											
//											t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
//											t_popup->setDimmedPosition(ccp(240, 160));
//											t_popup->setBasePosition(ccp(240, 160));
//											
//											CCNode* t_container = CCNode::create();
//											t_popup->setContainerNode(t_container);
//											addChild(t_popup, kStartSettingZorder_popup);
//											
//											CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//											case_back->setPosition(ccp(0,0));
//											t_container->addChild(case_back);
//											
//											case_back->setContentSize(CCSizeMake(220, 190));
//											
//											CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//											content_back->setPosition(ccp(0,25));
//											t_container->addChild(content_back);
//											
//											content_back->setContentSize(CCSizeMake(200, 120));
//											
//											CCLabelTTF* ment1_label = CCLabelTTF::create("골드가 부족합니다.", mySGD->getFont().c_str(), 15);
//											ment1_label->setPosition(ccp(0,35));
//											t_container->addChild(ment1_label);
//											
//											CCLabelTTF* ment2_label = CCLabelTTF::create("구매하러 가시겠습니까?", mySGD->getFont().c_str(), 15);
//											ment2_label->setPosition(ccp(0,15));
//											t_container->addChild(ment2_label);
//											
//											
//											
//											CommonButton* cancel_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
//											cancel_button->setPosition(ccp(100,85));
//											cancel_button->setFunction([=](CCObject* sender)
//																	   {
//																		   is_menu_enable = true;
//																		   t_popup->removeFromParent();
//																	   });
//											t_container->addChild(cancel_button);
//											
//											
//											CommonButton* ok_button = CommonButton::create("확인", 15, CCSizeMake(110, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
//											ok_button->setPosition(ccp(0,-65));
//											ok_button->setFunction([=](CCObject* sender)
//																   {
//																	   ShopPopup* t_shop = ShopPopup::create();
//																	   t_shop->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupClose));
//																	   t_shop->targetHeartTime(heart_time);
//																	   t_shop->setShopCode(kSC_gold);
//																	   t_shop->setShopBeforeCode(kShopBeforeCode_startsetting);
//																	   addChild(t_shop, kStartSettingZorder_popup);
//																	   t_popup->removeFromParent();
//																   });
//											t_container->addChild(ok_button);
//										}
									}
									else if(item_currency == "ruby")
									{
//										if(mySD->getItemPrice(item_list[clicked_item_idx]) <= mySGD->getGoodsValue(kGoodsType_ruby))
//										{
//											mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby)-mySD->getItemPrice(item_list[clicked_item_idx]));
											buySuccessItem(clicked_item_idx, 1);
//										}
//										else
//										{
//											ASPopupView* t_popup = ASPopupView::create(-500);
//											
//											CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//											float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//											if(screen_scale_x < 1.f)
//												screen_scale_x = 1.f;
//											
//											float height_value = 320.f;
//											if(myDSH->screen_convert_rate < 1.f)
//												height_value = 320.f/myDSH->screen_convert_rate;
//											
//											if(height_value < myDSH->ui_top)
//												height_value = myDSH->ui_top;
//											
//											t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
//											t_popup->setDimmedPosition(ccp(240, 160));
//											t_popup->setBasePosition(ccp(240, 160));
//											
//											CCNode* t_container = CCNode::create();
//											t_popup->setContainerNode(t_container);
//											addChild(t_popup, kStartSettingZorder_popup);
//											
//											CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//											case_back->setPosition(ccp(0,0));
//											t_container->addChild(case_back);
//											
//											case_back->setContentSize(CCSizeMake(220, 190));
//											
//											CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//											content_back->setPosition(ccp(0,25));
//											t_container->addChild(content_back);
//											
//											content_back->setContentSize(CCSizeMake(200, 120));
//											
//											CCLabelTTF* ment1_label = CCLabelTTF::create("젬이 부족합니다.", mySGD->getFont().c_str(), 15);
//											ment1_label->setPosition(ccp(0,35));
//											t_container->addChild(ment1_label);
//											
//											CCLabelTTF* ment2_label = CCLabelTTF::create("구매하러 가시겠습니까?", mySGD->getFont().c_str(), 15);
//											ment2_label->setPosition(ccp(0,15));
//											t_container->addChild(ment2_label);
//											
//											
//											
//											CommonButton* cancel_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
//											cancel_button->setPosition(ccp(100,85));
//											cancel_button->setFunction([=](CCObject* sender)
//																	   {
//																		   is_menu_enable = true;
//																		   t_popup->removeFromParent();
//																	   });
//											t_container->addChild(cancel_button);
//											
//											
//											CommonButton* ok_button = CommonButton::create("확인", 15, CCSizeMake(110, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
//											ok_button->setPosition(ccp(0,-65));
//											ok_button->setFunction([=](CCObject* sender)
//																   {
//																	   ShopPopup* t_shop = ShopPopup::create();
//																	   t_shop->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupClose));
//																	   t_shop->targetHeartTime(heart_time);
//																	   t_shop->setShopCode(kSC_ruby);
//																	   t_shop->setShopBeforeCode(kShopBeforeCode_startsetting);
//																	   addChild(t_shop, kStartSettingZorder_popup);
//																	   t_popup->removeFromParent();
//																   });
//											t_container->addChild(ok_button);
//										}
									}

								});
		buy_button->setPosition(410,117);
		main_case->addChild(buy_button);
		
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
//			if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) < 10000)
				CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
//			else
//				CCDirector::sharedDirector()->replaceScene(NewMainFlowScene::scene());
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
//			if(mySD->getSilType() < 10000)
//				CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
//			else
				CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
		}
		else if(tag == kStartSettingMenuTag_postbox)
		{
			MailPopup* t_pp = MailPopup::create(this, callfunc_selector(StartSettingScene::mailPopupClose), bind(&StartSettingScene::heartRefresh, this));
			addChild(t_pp, kStartSettingZorder_popup);
			
			postbox_count_case->setVisible(false);
		}
		else if(tag == kStartSettingMenuTag_option)
		{
			OptionPopup* t_popup = OptionPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupClose));
			addChild(t_popup, kStartSettingZorder_popup);
		}
		else if(tag == kStartSettingMenuTag_achieve)
		{
			AchievePopup* t_ap = AchievePopup::create();
			addChild(t_ap, kStartSettingZorder_popup);
			
			t_ap->setHideFinalAction(this, callfunc_selector(StartSettingScene::popupClose));
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
	CCSprite* n_cancel = CCSprite::create("puzzle_cancel.png");
	CCSprite* s_cancel = CCSprite::create("puzzle_cancel.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(StartSettingScene::menuAction));
	cancel_item->setTag(kStartSettingMenuTag_cancel);
	
	CCMenu* cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(25,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(cancel_menu, kStartSettingZorder_top);
	
	
	CCSprite* top_heart = CCSprite::create("mainflow_top_heart.png");
	top_heart->setAnchorPoint(ccp(0.5f,1.f));
	top_heart->setPosition(ccp(78+35,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_heart, kStartSettingZorder_top);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(15,top_heart->getContentSize().height/2.f));
	top_heart->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(StartSettingScene::menuAction));
	heart_item->setTag(kStartSettingMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(top_heart->getContentSize().width-n_heart->getContentSize().width/2.f+5,top_heart->getContentSize().height/2.f));
	top_heart->addChild(heart_menu);
	
	
	CCSprite* top_gold = CCSprite::create("mainflow_top_gold.png");
	top_gold->setAnchorPoint(ccp(0.5f,1.f));
	top_gold->setPosition(ccp(216+23,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_gold, kStartSettingZorder_top);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_gold))->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(top_gold->getContentSize().width/2.f + 1,top_gold->getContentSize().height/2.f-5));
	top_gold->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(StartSettingScene::menuAction));
	gold_item->setTag(kStartSettingMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(top_gold->getContentSize().width-n_gold->getContentSize().width/2.f+5,top_gold->getContentSize().height/2.f));
	top_gold->addChild(gold_menu);
	
	
	CCSprite* top_ruby = CCSprite::create("mainflow_top_ruby.png");
	top_ruby->setAnchorPoint(ccp(0.5f,1.f));
	top_ruby->setPosition(ccp(325+10,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_ruby, kStartSettingZorder_top);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_ruby))->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(top_ruby->getContentSize().width/2.f + 1,top_ruby->getContentSize().height/2.f-5));
	top_ruby->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(StartSettingScene::menuAction));
	ruby_item->setTag(kStartSettingMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(top_ruby->getContentSize().width-n_ruby->getContentSize().width/2.f+5,top_ruby->getContentSize().height/2.f));
	top_ruby->addChild(ruby_menu);
	
	
	
	
	
	CCSprite* n_postbox = CCSprite::create("mainflow_new_postbox.png");
	CCSprite* s_postbox = CCSprite::create("mainflow_new_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(StartSettingScene::menuAction));
	postbox_item->setTag(kStartSettingMenuTag_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccp(394,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(postbox_menu, kStartSettingZorder_top);
	
	postbox_count_case = CCSprite::create("mainflow_new.png");//"mainflow_postbox_count.png");
	postbox_count_case->setPosition(ccp(406,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-9));
	addChild(postbox_count_case, kStartSettingZorder_top);
	postbox_count_case->setVisible(false);
	
//	postbox_count_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 10);
//	postbox_count_label->setColor(ccc3(95, 60, 30));
//	postbox_count_label->setPosition(ccp(postbox_count_case->getContentSize().width/2.f-0.5f, postbox_count_case->getContentSize().height/2.f+0.5f));
//	postbox_count_case->addChild(postbox_count_label);
	
	countingMessage();
	
	
	CCSprite* n_achieve = CCSprite::create("mainflow_new_achievement.png");
	CCSprite* s_achieve = CCSprite::create("mainflow_new_achievement.png");
	s_achieve->setColor(ccGRAY);
	
	CCMenuItem* achieve_item = CCMenuItemSprite::create(n_achieve, s_achieve, this, menu_selector(StartSettingScene::menuAction));
	achieve_item->setTag(kStartSettingMenuTag_achieve);
	
	CCMenu* achieve_menu = CCMenu::createWithItem(achieve_item);
	achieve_menu->setPosition(ccp(427,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(achieve_menu, kStartSettingZorder_top);
	
	
	CCSprite* n_option = CCSprite::create("mainflow_new_option.png");
	CCSprite* s_option = CCSprite::create("mainflow_new_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(StartSettingScene::menuAction));
	option_item->setTag(kStartSettingMenuTag_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccp(460,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(option_menu, kStartSettingZorder_top);
	
}

void StartSettingScene::mailPopupClose()
{
	countingMessage();
	is_menu_enable = true;
}

void StartSettingScene::heartRefresh()
{
	CCPoint heart_position = heart_time->getPosition();
	CCNode* heart_parent = heart_time->getParent();
	
	heart_time->removeFromParent();
	
	heart_time = HeartTime::create();
	heart_time->setPosition(heart_position);
	heart_parent->addChild(heart_time);
}

void StartSettingScene::countingMessage()
{
	Json::Value p;
	p["memberID"]=hspConnector::get()->getSocialID();
	p["type"]=0; // 모든 타입의 메시지를 받겠다는 뜻.
	p["limitDay"] = mySGD->getMsgRemoveDay();
	// 0 이 아니면 해당하는 타입의 메시지가 들어옴.
	
	hspConnector::get()->command("getmessagelist",p,[this](Json::Value r)
								 {
									 GraphDogLib::JsonToLog("getmessagelist", r);
									 if(r["result"]["code"].asInt() != GDSUCCESS)
										 return;
									 Json::Value message_list = r["list"];
									 if(message_list.size() > 0)
									 {
										 postbox_count_case->setVisible(true);
										 
//										 if(message_list.size() < 10)
//										 {
//											 postbox_count_label->setFontSize(10);
//											 postbox_count_label->setString(CCString::createWithFormat("%d", message_list.size())->getCString());
//										 }
//										 else if(message_list.size() < 100)
//										 {
//											 postbox_count_label->setFontSize(7);
//											 postbox_count_label->setString(CCString::createWithFormat("%d", message_list.size())->getCString());
//										 }
//										 else
//										 {
//											 postbox_count_label->setFontSize(8);
//											 postbox_count_label->setString("...");
//										 }
									 }
									 else
									 {
										 postbox_count_case->setVisible(false);
//										 postbox_count_label->setString("0");
									 }
								 });
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
	finalSetting();
	
//	myDSH->saveAllUserData(json_selector(this, StartSettingScene::finalStartAction));
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
		
//		command_list.push_back(CommandParam("updateUserData", myDSH->getSaveAllUserDataParam(), json_selector(this, StartSettingScene::finalAcceptStartAction)));
		
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
	
//	mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) - use_item_price_gold.getV());
//	mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - use_item_price_ruby.getV());
	
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
	
//	Json::Value param;
//	param["key"] = CCString::createWithFormat("stage_start_%d", mySD->getSilType())->getCString();
//	
//	hspConnector::get()->command("increaseStats", param, NULL);
	
	mySGD->resetLabels();
	myGD->resetGameData();
	
	mySGD->setGameStart();
	
	if(mySD->getSilType() == 1 && !myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_control))
	{
		myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_control, true);
		mySGD->setNextSceneName("playtutorial");
		
		CCDirector::sharedDirector()->replaceScene(LoadingTipScene::scene());
	}
	else
	{
		mySGD->setNextSceneName("maingame");
		
		LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
		addChild(loading_tip, kStartSettingZorder_popup);
		
//		CCDirector::sharedDirector()->replaceScene(LoadingTipScene::scene());
	}
//	CCDirector::sharedDirector()->replaceScene(Maingame::scene());
}

void StartSettingScene::cancelGame()
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
				if(is_have_item[i])
					myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i])+1);
				is_using_item[item_list[i]] = true;
			}
		}
				   
//		mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) + use_item_price_gold.getV());
//		mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + use_item_price_ruby.getV());
		
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
//	if(cnt_label)
//	{
		cnt_label->setString(CCString::createWithFormat("%d", item_cnt)->getCString());
//	}
//	else
//	{
//		cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
//		cnt_label->setPosition(ccp(-15, -15));
//		item_parent->addChild(cnt_label, kStartSettingItemZorder_cntLabel, kStartSettingItemZorder_cntLabel);
//	}
	
	bool is_selectable = false;
	if(getSelectedItemCount() > 3)
		is_selectable = false;
	else
		is_selectable = true;
	
	if(is_selectable)
	{
		((CCSprite*)item_parent->getChildByTag(kStartSettingItemZorder_clicked))->setVisible(true);
		is_selected_item[t_clicked_item_idx] = true;
	}
	
//	vector<SaveUserData_Key> save_userdata_list;
//	
//	save_userdata_list.push_back(kSaveUserData_Key_gold);
//	save_userdata_list.push_back(kSaveUserData_Key_star);
//	save_userdata_list.push_back(kSaveUserData_Key_item);
//	
//	myDSH->saveUserData(save_userdata_list, nullptr);
	
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
	else if(t_code == kIC_doubleItem)		return_value = "더블아이템";
	else if(t_code == kIC_silence)			return_value = "Silence";
	else if(t_code == kIC_longTime)			return_value = "시간추가";
	else if(t_code == kIC_baseSpeedUp)		return_value = "기본속도향상";
	else if(t_code == kIC_itemGacha)		return_value = "아이템 뽑기";
	
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
	
	CommonButton::callBackKey();
	//AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(StartSettingScene::alertAction));
}
