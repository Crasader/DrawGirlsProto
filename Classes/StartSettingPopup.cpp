//
//  StartSettingPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 16..
//
//

#include "StartSettingPopup.h"
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
#include "MissileUpgradePopup.h"
#include "ItemGachaPopup.h"

bool StartSettingPopup::init()
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
	
	touch_priority = -210;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kStartSettingPopupZorder_back);
	
	gray->runAction(CCFadeTo::create(0.5f, 255));
	
	setMain();
	
	is_menu_enable = true;
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_startClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_startClick);
		addChild(t_tutorial, kStartSettingPopupZorder_popup);
		
		tutorial_node = t_tutorial;
	}
	else if(recent_step == kTutorialFlowStep_emptyCardClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_emptyCardClick);
		addChild(t_tutorial, kStartSettingPopupZorder_popup);
		
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
	//		addChild(t_suction, kStartSettingPopupZorder_popup);
	//	}
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(touch_priority+10);
	t_suction->setTouchEnabled(true);
	addChild(t_suction);
	
	PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
	if(!t_history.is_open)
	{
		t_history.is_open = true;
		mySGD->setPieceHistory(t_history, nullptr);
	}
	
	return true;
}

void StartSettingPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

enum StartSettingPopupMenuTag{
	kStartSettingPopupMenuTag_back,
	kStartSettingPopupMenuTag_start,
	kStartSettingPopupMenuTag_card,
	kStartSettingPopupMenuTag_turn,
	kStartSettingPopupMenuTag_itemBuy,
	kStartSettingPopupMenuTag_itemBase = 100000
};

enum StartSettingPopupItemZorder{
	kStartSettingPopupItemZorder_base = 0,
	kStartSettingPopupItemZorder_selected,
	kStartSettingPopupItemZorder_clicked,
	kStartSettingPopupItemZorder_cntLabel
};

void StartSettingPopup::setMain()
{
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160));
	addChild(main_case, kStartSettingPopupZorder_main);
	
	main_case->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t){KS::setOpacity(main_case, 255);}));
	
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
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_back->setContentSize(CCSizeMake(310, 232));
	right_back->setPosition(main_case->getContentSize().width*0.66f, main_case->getContentSize().height*0.44f);
	main_case->addChild(right_back);
	
	
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
	
	
	CommonButton* back_button = CommonButton::createCloseButton(touch_priority);
	back_button->setPosition(ccp(main_case->getContentSize().width-28, main_case->getContentSize().height+40-65));
	back_button->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kStartSettingPopupMenuTag_back);
								 menuAction(t_node);
							 });
	main_case->addChild(back_button);
	
	
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
		ASPopupView* t_popup = ASPopupView::create(touch_priority-100);
		
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
		addChild(t_popup, kStartSettingPopupZorder_popup);
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
			main_case->addChild(item_parent, 0, kStartSettingPopupMenuTag_itemBase+i);
			
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
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingPopup::itemAction));
				item_item->setTag(i+1);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				item_menu->setTouchPriority(touch_priority);
				
				is_selected_item.push_back(true);
			}
			else
			{
				mySGD->setBeforeUseItem(t_ic, false);
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
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingPopup::itemAction));
				item_item->setTag(i+1);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				item_menu->setTouchPriority(touch_priority);
				
				is_selected_item.push_back(false);
			}
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
			cnt_label->setPosition(ccp(21, -21));
			item_parent->addChild(cnt_label, kStartSettingPopupItemZorder_cntLabel, kStartSettingPopupItemZorder_cntLabel);
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(false);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingPopupItemZorder_clicked, kStartSettingPopupItemZorder_clicked);
		}
		else
			is_selected_item.push_back(false);
	}
	
	CCRect option_size = CCRectMake(0, 0, 200, 25);
	CCPoint option_position = ccp(188, 113);
	
	//		CCSprite* option_rect = CCSprite::create("whitePaper.png", option_size);
	//		option_rect->setOpacity(100);
	//		option_rect->setAnchorPoint(ccp(0,1));
	//		option_rect->setPosition(option_position);
	//		main_case->addChild(option_rect);
	
	option_label = CCLabelTTF::create("게임에 사용할 아이템을 선택하세요.", mySGD->getFont().c_str(), 12, option_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	option_label->setAnchorPoint(ccp(0,1));
	option_label->setPosition(option_position);
	main_case->addChild(option_label);
	
	item_gacha_menu = NULL;
	is_clicked_gacha_menu = false;
	
	gacha_clicked_img = CCSprite::create("startsetting_item_clicked.png");
	gacha_clicked_img->setPosition(ccp(425,190));
	gacha_clicked_img->setVisible(false);
	main_case->addChild(gacha_clicked_img, kStartSettingPopupZorder_main+1);
	
	gachaMenuCreate();
	
	gacha_item = CCSprite::create("startsetting_item_gacha_inner.png");
	gacha_item->setPosition(ccp(425,190));
	main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
	
	
	CCScale9Sprite* script_box = CCScale9Sprite::create("startsetting_scriptbox.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	script_box->setContentSize(CCSizeMake(290, 70));
	script_box->setPosition(ccp(317,117));
	main_case->addChild(script_box);
	
	
	CCSprite* n_start = CCSprite::create("startsetting_start.png");
	CCSprite* s_start = CCSprite::create("startsetting_start.png");
	s_start->setColor(ccGRAY);
	
	CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(StartSettingPopup::menuAction));
	start_item->setTag(kStartSettingPopupMenuTag_start);
	
	CCMenu* start_menu = CCMenu::createWithItem(start_item);
	start_menu->setPosition(ccp(317, 44));
	main_case->addChild(start_menu);
	start_menu->setTouchPriority(touch_priority);
	
	
	CCSprite* level_case = CCSprite::create("startsetting_levelbox.png");
	level_case->setPosition(ccp(83,95));
	main_case->addChild(level_case);
	
	
	StoneType missile_type_code = StoneType(myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)%7);
	
	int missile_level = myDSH->getIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))+1;
	
	if(missile_type_code == kStoneType_guided)
	{
		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("jack_missile_%d.png", missile_level)->getCString(),
																														 false);
//		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("me_guide%d.ccbi", (missile_level-1)%5 + 1)->getCString());
		t_gm->setPosition(ccp(83,158));
		t_gm->beautifier((missile_level-1)/5+1, (missile_level-1)%5+1);
		main_case->addChild(t_gm);
		
		int grade = (missile_level-1)/5+1;
		
		t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
		
		missile_img = t_gm;
	}
	
	missile_data_level = CCLabelTTF::create(CCString::createWithFormat("레벨 %d", missile_level)->getCString(), mySGD->getFont().c_str(), 12);
	missile_data_level->setAnchorPoint(ccp(0,0.5f));
	missile_data_level->setPosition(ccp(30,95));
	main_case->addChild(missile_data_level);
	
	missile_data_power = CCLabelTTF::create(CCString::createWithFormat("파워 %d", StoneAttack::getPower((missile_level-1)/5+1, (missile_level-1)%5+1))->getCString(), mySGD->getFont().c_str(), 12);
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
		CCSprite* n_price_type = CCSprite::create("common_button_gold.png");
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
		CCSprite* s_price_type = CCSprite::create("common_button_gold.png");
		s_price_type->setPosition(ccp(25,22));
		s_upgrade->addChild(s_price_type);
		CCLabelTTF* s_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 12);
		s_price_label->setColor(ccBLACK);
		s_price_label->setPosition(ccp(78,22));
		s_upgrade->addChild(s_price_label);
		
		CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
		
		upgrade_menu = CCMenu::createWithItem(upgrade_item);
		upgrade_menu->setPosition(ccp(83,46));
		main_case->addChild(upgrade_menu);
		
		upgrade_menu->setTouchPriority(touch_priority);
	}
}

void StartSettingPopup::gachaMenuCreate()
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
																			   CCNode* before_item_parent = (CCNode*)main_case->getChildByTag(kStartSettingPopupMenuTag_itemBase+clicked_item_idx);
																			   CCSprite* before_clicked_img = (CCSprite*)before_item_parent->getChildByTag(kStartSettingPopupItemZorder_clicked);
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
																		   buy_button->setTouchPriority(touch_priority);
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
	
	item_gacha_menu->setTouchPriority(touch_priority);
}

void StartSettingPopup::upgradeAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	MissileUpgradePopup* t_popup = MissileUpgradePopup::create(touch_priority-100, [=](){popupClose();}, [=](){
		int missile_level = myDSH->getIntegerForKey(kDSH_Key_weaponLevelForCharacter_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))+1;
		
		missile_data_level->setString(CCString::createWithFormat("레벨 %d", missile_level)->getCString());
		missile_data_power->setString(CCString::createWithFormat("파워 %d", StoneAttack::getPower((missile_level-1)/5+1, (missile_level-1)%5+1))->getCString());
		
		CCPoint missile_position;
		if(missile_img)
		{
			missile_position = missile_img->getPosition();
			missile_img->removeFromParent();
		}
		
		StoneType missile_type_code = StoneType(myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)%7);
		
		if(missile_type_code == kStoneType_guided)
		{
			GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("jack_missile_%d.png", missile_level)->getCString(),
																															 false);
//			GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("me_guide%d.ccbi", (missile_level-1)%5 + 1)->getCString());
			t_gm->setPosition(missile_position);
			t_gm->beautifier((missile_level-1)/5+1, (missile_level-1)%5+1);
			main_case->addChild(t_gm);
			
			int grade = (missile_level-1)/5+1;
			
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
			CCSprite* n_price_type = CCSprite::create("common_button_gold.png");
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
			CCSprite* s_price_type = CCSprite::create("common_button_gold.png");
			s_price_type->setPosition(ccp(25,22));
			s_upgrade->addChild(s_price_type);
			CCLabelTTF* s_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", missile_level*1000)->getCString(), mySGD->getFont().c_str(), 12);
			s_price_label->setColor(ccBLACK);
			s_price_label->setPosition(ccp(78,22));
			s_upgrade->addChild(s_price_label);
			
			CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
			
			upgrade_menu = CCMenu::createWithItem(upgrade_item);
			upgrade_menu->setPosition(upgrade_position);
			main_case->addChild(upgrade_menu);
			upgrade_menu->setTouchPriority(touch_priority);
		}
	});
	addChild(t_popup, kStartSettingPopupZorder_popup);
}

void StartSettingPopup::startItemGacha()
{
	if(!is_menu_enable || 1000 > mySGD->getGold())
		return;
	
	is_menu_enable = false;
	
	CCLog("start item gacha");
	
	mySGD->setGold(mySGD->getGold() - 1000);
	myDSH->saveUserData({kSaveUserData_Key_gold}, json_selector(this, StartSettingPopup::goItemGacha));

	
	
	
	
	
	
	
	
//	if(selected_gacha_item > kIC_emptyBegin && selected_gacha_item < kIC_emptyEnd)
//		selected_gacha_item = kIC_emptyBegin;
//	
//	gacha_item_frame = 0;
//	schedule(schedule_selector(StartSettingPopup::itemGachaAction), 1.f/20.f);
}

void StartSettingPopup::goItemGacha(Json::Value result_data)
{
	CCLog("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLog("save userdata success!!");
	
		ItemGachaPopup* t_popup = ItemGachaPopup::create(touch_priority-100, [=](){endItemGacha();}, [=](int item_type){
			
			selected_gacha_item = (ITEM_CODE)item_type;
			
			CCPoint gacha_item_position = gacha_item->getPosition();
			gacha_item->removeFromParent();
			
			gacha_item = CCSprite::create(CCString::createWithFormat("item%d.png", selected_gacha_item)->getCString());
			gacha_item->setPosition(gacha_item_position);
			main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
			
			CCSprite* mount_img = CCSprite::create("startsetting_item_mounted_case.png");
			mount_img->setPosition(ccp(gacha_item->getContentSize().width/2.f + 37.5f - mount_img->getContentSize().width/2.f-6, gacha_item->getContentSize().width/2.f + 37.5f - mount_img->getContentSize().height/2.f-6));
			gacha_item->addChild(mount_img);
		});
		addChild(t_popup, kStartSettingPopupZorder_popup);
	}
	else
	{
		CCLog("save userdata fail!!!");
		mySGD->setGold(mySGD->getGold() + 1000);
		is_menu_enable = true;
	}
}

void StartSettingPopup::itemGachaAction()
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
	main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
	
	if(gacha_item_frame == 25)
	{
		gacha_item_cover = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 40, 40));
		gacha_item_cover->setScale(0.f);
		gacha_item_cover->setPosition(gacha_item->getPosition());
		main_case->addChild(gacha_item_cover, kStartSettingPopupZorder_main+1);
		
		CCScaleTo* t_scale = CCScaleTo::create(0.15f, 1.f);
		CCDelayTime* t_delay = CCDelayTime::create(0.25f);
		CCFadeTo* t_fade = CCFadeTo::create(0.6f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartSettingPopup::endItemGacha));
		CCSequence* t_seq = CCSequence::create(t_scale, t_delay, t_fade, t_call, NULL);
		gacha_item_cover->runAction(t_seq);
	}
	else if(gacha_item_frame >= 30)
	{
		stopItemGacha();
	}
}
void StartSettingPopup::stopItemGacha()
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
	main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
	
	CCSprite* mount_img = CCSprite::create("startsetting_item_mounted_case.png");
	mount_img->setPosition(ccp(gacha_item->getContentSize().width/2.f + 37.5f - mount_img->getContentSize().width/2.f-6, gacha_item->getContentSize().width/2.f + 37.5f - mount_img->getContentSize().height/2.f-6));
	gacha_item->addChild(mount_img);
	
	unschedule(schedule_selector(StartSettingPopup::itemGachaAction));
}

void StartSettingPopup::endItemGacha()
{
	is_menu_enable = true;
}

int StartSettingPopup::getSelectedItemCount()
{
	int selected_item_cnt = 0;
	for(int k=0;k<is_selected_item.size();k++)
		if(is_selected_item[k])
			selected_item_cnt++;
	
	return selected_item_cnt;
}

void StartSettingPopup::itemAction(CCObject *sender)
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
			CCNode* before_item_parent = (CCNode*)main_case->getChildByTag(kStartSettingPopupMenuTag_itemBase+clicked_item_idx);
			CCSprite* before_clicked_img = (CCSprite*)before_item_parent->getChildByTag(kStartSettingPopupItemZorder_clicked);
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
			CCNode* item_parent = (CCNode*)main_case->getChildByTag(kStartSettingPopupMenuTag_itemBase+tag-1);
			item_parent->removeAllChildren();
			
			
			ITEM_CODE t_ic = item_list[tag-1];
			
			mySGD->setBeforeUseItem(t_ic, false);
			
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
			
			CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingPopup::itemAction));
			item_item->setTag(tag);
			
			CCMenu* item_menu = CCMenu::createWithItem(item_item);
			item_menu->setPosition(CCPointZero);
			item_parent->addChild(item_menu);
			item_menu->setTouchPriority(touch_priority);
			
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);

			CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
			cnt_label->setPosition(ccp(21, -21));
			item_parent->addChild(cnt_label, kStartSettingPopupItemZorder_cntLabel, kStartSettingPopupItemZorder_cntLabel);
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(true);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingPopupItemZorder_clicked, kStartSettingPopupItemZorder_clicked);
			
			
			is_selected_item[tag-1] = false;
		}
		else // unselected -> selected or nothing
		{
			CCNode* item_parent = (CCNode*)main_case->getChildByTag(kStartSettingPopupMenuTag_itemBase+tag-1);
			item_parent->removeAllChildren();
			
			
			ITEM_CODE t_ic = item_list[tag-1];
			
			bool is_price_usable = false; // 소지하고 있거나 장착 가능한 가격
			is_price_usable = is_price_usable || (myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic) > 0); // 소지하고 있는지
			
			if(getSelectedItemCount() < 3 && is_price_usable)
			{
				mySGD->setBeforeUseItem(t_ic, true);
				
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
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingPopup::itemAction));
				item_item->setTag(tag);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				item_menu->setTouchPriority(touch_priority);
				
				is_selected_item[tag-1] = true;
			}
			else
			{
				mySGD->setBeforeUseItem(t_ic, false);
				
				if(getSelectedItemCount() >= 3)
					addChild(ASPopupView::getCommonNoti(-210, "아이템은 최대 3개까지\n선택이 가능합니다."), kStartSettingPopupZorder_popup);
				
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
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingPopup::itemAction));
				item_item->setTag(tag);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				item_menu->setTouchPriority(touch_priority);
				
				is_selected_item[tag-1] = false;
			}
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);

			CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 10);
			cnt_label->setPosition(ccp(21, -21));
			item_parent->addChild(cnt_label, kStartSettingPopupItemZorder_cntLabel, kStartSettingPopupItemZorder_cntLabel);
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(true);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingPopupItemZorder_clicked, kStartSettingPopupItemZorder_clicked);
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
		buy_button->setTouchPriority(touch_priority);
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
										if(mySD->getItemPrice(item_list[clicked_item_idx]) <= mySGD->getGold())
										{
											mySGD->setGold(mySGD->getGold()-mySD->getItemPrice(item_list[clicked_item_idx]));
											buySuccessItem(clicked_item_idx, 1);
										}
										else
										{
											ASPopupView* t_popup = ASPopupView::create(-500);
											
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
											addChild(t_popup, kStartSettingPopupZorder_popup);
											
											CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
											case_back->setPosition(ccp(0,0));
											t_container->addChild(case_back);
											
											case_back->setContentSize(CCSizeMake(220, 190));
											
											CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
											content_back->setPosition(ccp(0,25));
											t_container->addChild(content_back);
											
											content_back->setContentSize(CCSizeMake(200, 120));
											
											CCLabelTTF* ment1_label = CCLabelTTF::create("골드가 부족합니다.", mySGD->getFont().c_str(), 15);
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
																	   t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
																	   t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
																	   t_shop->setShopCode(kSC_gold);
																	   t_shop->setShopBeforeCode(kShopBeforeCode_startsetting);
																	   addChild(t_shop, kStartSettingPopupZorder_popup);
																	   t_popup->removeFromParent();
																   });
											t_container->addChild(ok_button);
										}
									}
									else if(item_currency == "ruby")
									{
										if(mySD->getItemPrice(item_list[clicked_item_idx]) <= mySGD->getStar())
										{
											mySGD->setStar(mySGD->getStar()-mySD->getItemPrice(item_list[clicked_item_idx]));
											buySuccessItem(clicked_item_idx, 1);
										}
										else
										{
											ASPopupView* t_popup = ASPopupView::create(-500);
											
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
											addChild(t_popup, kStartSettingPopupZorder_popup);
											
											CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
											case_back->setPosition(ccp(0,0));
											t_container->addChild(case_back);
											
											case_back->setContentSize(CCSizeMake(220, 190));
											
											CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
											content_back->setPosition(ccp(0,25));
											t_container->addChild(content_back);
											
											content_back->setContentSize(CCSizeMake(200, 120));
											
											CCLabelTTF* ment1_label = CCLabelTTF::create("루비가 부족합니다.", mySGD->getFont().c_str(), 15);
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
																	   t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
																	   t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
																	   t_shop->setShopCode(kSC_ruby);
																	   t_shop->setShopBeforeCode(kShopBeforeCode_startsetting);
																	   addChild(t_shop, kStartSettingPopupZorder_popup);
																	   t_popup->removeFromParent();
																   });
											t_container->addChild(ok_button);
										}
									}
									
								});
		buy_button->setPosition(410,117);
		main_case->addChild(buy_button);
		
		is_menu_enable = true;
	}
}

void StartSettingPopup::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_startClick)
	{
		int tag = ((CCNode*)sender)->getTag();
		if(tag == kStartSettingPopupMenuTag_start)
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
		if(tag == kStartSettingPopupMenuTag_card)
		{
			is_menu_enable = false;
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_targetCardClick);
			removeChild(tutorial_node);
			CardChangePopup* t_popup = CardChangePopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupCloseCardSetting));
			addChild(t_popup, kStartSettingPopupZorder_popup);
		}
	}
	else
	{
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kStartSettingPopupMenuTag_back)
		{
			closeAction();
		}
		else if(tag == kStartSettingPopupMenuTag_start)
		{
			callStart();
		}
		else if(tag == kStartSettingPopupMenuTag_card)
		{
			CardChangePopup* t_popup = CardChangePopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupCloseCardSetting));
			addChild(t_popup, kStartSettingPopupZorder_popup);
		}
		else if(tag == kStartSettingPopupMenuTag_turn)
		{
			is_menu_enable = true;
		}
	}
}

void StartSettingPopup::closeAction()
{
	is_menu_enable = false;
	addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
										   {
											   gray->setOpacity(255*t);
										   }, [=](float t)
										   {
											   gray->setOpacity(0);
											   if(target_final && delegate_final)
												   (target_final->*delegate_final)();
											   removeFromParent();
										   }));
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t){KS::setOpacity(main_case, 0);}));
}

void StartSettingPopup::callStart()
{
	if(((PuzzleScene*)getParent())->heart_time->isStartable())
	{
		if(((PuzzleScene*)getParent())->heart_time->startGame())
			realStartAction();
		else
		{
			is_menu_enable = true;
		}
	}
	else
	{
		ASPopupView* t_popup = ASPopupView::create(touch_priority-100);
		
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
		addChild(t_popup, kStartSettingPopupZorder_popup);
		
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
								   t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
								   t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
								   t_shop->setShopCode(kSC_heart);
								   t_shop->setShopBeforeCode(kShopBeforeCode_startsetting);
								   addChild(t_shop, kStartSettingPopupZorder_popup);
								   t_popup->removeFromParent();
							   });
		t_container->addChild(ok_button);
		
	}
}

void StartSettingPopup::realStartAction()
{
	finalSetting();
	
	vector<CommandParam> t_command_list;
	t_command_list.clear();
	t_command_list.push_back(CommandParam("updateUserData", myDSH->getSaveAllUserDataParam(), json_selector(this, StartSettingPopup::finalStartAction)));
	
	PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
	t_history.try_count++;
	
	mySGD->setPieceHistoryForNotSave(t_history);
	
	t_command_list.push_back(CommandParam("updatePieceHistory", mySGD->getSavePieceHistoryParam(t_history), nullptr));
	
	hspConnector::get()->command(t_command_list);
	
//	myDSH->saveAllUserData(json_selector(this, StartSettingPopup::finalStartAction));
}

void StartSettingPopup::acceptStartAction()
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
		
		command_list.push_back(CommandParam("updateUserData", myDSH->getSaveAllUserDataParam(), json_selector(this, StartSettingPopup::finalAcceptStartAction)));
		
		hspConnector::get()->command(command_list);
	}
	else
	{
		was_end_startAction = true;
		was_end_removeMessage = true;
		goToGame();
	}
}
void StartSettingPopup::finalSetting()
{
	start_loading = LoadingLayer::create();
	addChild(start_loading, kStartSettingPopupZorder_popup);
	
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
	
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
		mySGD->setIsUsingItem(ITEM_CODE(i), is_using_item[i]);
}

void StartSettingPopup::finalRemoveMessage(Json::Value result_data)
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

void StartSettingPopup::goToGame()
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
	
	if(mySD->getSilType() == 1 && !myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_control))
	{
		myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_control, true);
		mySGD->setNextSceneName("playtutorial");
		
		LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
		addChild(loading_tip, kStartSettingPopupZorder_popup);
	}
	else
	{
		mySGD->setNextSceneName("maingame");
		
		LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
		addChild(loading_tip, kStartSettingPopupZorder_popup);
	}
}

void StartSettingPopup::cancelGame()
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
		
		mySGD->resetUsingItem();
		
		is_menu_enable = true;
	}
}

void StartSettingPopup::finalAcceptStartAction(Json::Value result_data)
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

void StartSettingPopup::finalStartAction(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		goToGame();
	}
	else
	{
		((PuzzleScene*)getParent())->heart_time->backHeart();
		
		cancelGame();
	}
}

void StartSettingPopup::popupClose()
{
	is_menu_enable = true;
}

void StartSettingPopup::popupCloseCardSetting()
{
	//	changeCard();
	is_menu_enable = true;
}

void StartSettingPopup::buySuccessItem(int t_clicked_item_idx, int cnt)
{
	myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_idx], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_idx])+cnt);
	int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_idx]);
	
	myLog->addLog(kLOG_buyItem_s, -1, convertToItemCodeToItemName(item_list[t_clicked_item_idx]).c_str());
	
	CCNode* item_parent = main_case->getChildByTag(kStartSettingPopupMenuTag_itemBase+t_clicked_item_idx);
	
	CCLabelTTF* cnt_label = (CCLabelTTF*)item_parent->getChildByTag(kStartSettingPopupItemZorder_cntLabel);

	cnt_label->setString(CCString::createWithFormat("%d", item_cnt)->getCString());
	
	bool is_selectable = false;
	if(getSelectedItemCount() > 3)
		is_selectable = false;
	else
		is_selectable = true;
	
	if(is_selectable)
	{
		((CCSprite*)item_parent->getChildByTag(kStartSettingPopupItemZorder_clicked))->setVisible(true);
		is_selected_item[t_clicked_item_idx] = true;
	}
	
	vector<SaveUserData_Key> save_userdata_list;
	
	save_userdata_list.push_back(kSaveUserData_Key_gold);
	save_userdata_list.push_back(kSaveUserData_Key_star);
	save_userdata_list.push_back(kSaveUserData_Key_item);
	
	myDSH->saveUserData(save_userdata_list, nullptr);
	
	is_menu_enable = true;
}

string StartSettingPopup::convertToItemCodeToItemName(ITEM_CODE t_code)
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