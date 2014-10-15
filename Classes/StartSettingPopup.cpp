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
#include "MyLocalization.h"
#include "LevelupGuidePopup.h"
#include "CommonAnimation.h"
#include "AchieveNoti.h"
#include "TypingBox.h"
#include "FiveRocksCpp.h"
#include "CharacterSelectPopup.h"
#include <boost/lexical_cast.hpp>

bool StartSettingPopup::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	//	mySGD->selectFriendCard();
	
	item_title_label = NULL;
	option_label = NULL;
	//	card_img = NULL;
	buy_button = NULL;
	selected_gacha_item = (ITEM_CODE)mySGD->gacha_item.getV();
	
	is_first_gacha = false;
	
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
	
	is_menu_enable = false;
	
	setMain();
	
	startFormSetter(this);
	setFormSetterGuide("guide1.png");
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
	if(!t_history.is_open.getV())
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
	bool is_tutorial = false;
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-22.f+8));
	addChild(main_case, kStartSettingPopupZorder_main);
	
	if(mySGD->is_hell_mode_enabled)
	{
		left_back = CCScale9Sprite::create("common_doubleblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		left_back->setContentSize(CCSizeMake(146, 205));
		left_back->setPosition(ccp(main_case->getContentSize().width*0.2f-1,main_case->getContentSize().height*0.44f));
		main_case->addChild(left_back);
		
		CCSprite* left_tab = CCSprite::create("startsetting_tab.png");
		left_tab->setPosition(ccp(27,left_back->getContentSize().height-15));
		left_back->addChild(left_tab);
		
		KSLabelTTF* tab_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_mycharacter), mySGD->getFont().c_str(), 12.f);
		tab_label->setColor(ccc3(255, 255, 255));
		tab_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		tab_label->setPosition(ccp(left_tab->getContentSize().width/2.f-3,left_tab->getContentSize().height/2.f-1.f));
		left_tab->addChild(tab_label);
	}
	else
	{
		left_back = CCScale9Sprite::create("common_doubleblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		left_back->setContentSize(CCSizeMake(142, 205));
		left_back->setPosition(ccp(main_case->getContentSize().width*0.2f,main_case->getContentSize().height*0.44f));
		main_case->addChild(left_back);
		
		CCSprite* left_tab = CCSprite::create("title_tab.png");
		left_tab->setPosition(ccp(left_back->getContentSize().width/2.f,left_back->getContentSize().height-7));
		left_back->addChild(left_tab);
		
		KSLabelTTF* tab_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_mymissile), mySGD->getFont().c_str(), 12.5f);
		tab_label->setColor(ccc3(20, 50, 70));
		tab_label->disableOuterStroke();
		tab_label->setPosition(ccp(left_tab->getContentSize().width/2.f,left_tab->getContentSize().height/2.f+1.5f));
		left_tab->addChild(tab_label);
	}
	
//	CommonAnimation::applyShadow(tab_label);
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	right_back->setContentSize(CCSizeMake(280, 145));
	right_back->setPosition(main_case->getContentSize().width*0.655f, main_case->getContentSize().height*0.545f);
	main_case->addChild(right_back);
	
	
	int stage_number = mySD->getSilType();
	
	if(mySGD->is_hell_mode)
	{
		KSLabelTTF* stage_number_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_hellMode), mySGD->getFont().c_str(), 17.5f);
		stage_number_label->setColor(ccc3(255, 255, 255));
		stage_number_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		stage_number_label->setAnchorPoint(ccp(0,0.5f));
		stage_number_label->setPosition(ccp(30, 250));
		setFormSetter(stage_number_label);
		main_case->addChild(stage_number_label);
		
		mySGD->is_before_selected_event_stage = false;
		is_before_selected_event_stage = false;
	}
	else if(mySGD->is_endless_mode)
	{
		KSLabelTTF* stage_number_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessStartSettingTitle), mySGD->getFont().c_str(), 17.5f);
		stage_number_label->setColor(ccc3(255, 255, 255));
		stage_number_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		stage_number_label->setAnchorPoint(ccp(0,0.5f));
		stage_number_label->setPosition(ccp(30, 250));
		setFormSetter(stage_number_label);
		main_case->addChild(stage_number_label);
		
		mySGD->is_before_selected_event_stage = false;
		is_before_selected_event_stage = false;
	}
	else
	{
		if(mySGD->is_before_selected_event_stage)
		{
			KSLabelTTF* stage_number_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_stageValue), stage_number)->getCString(),	mySGD->getFont().c_str(), 17.5f);
			stage_number_label->setColor(ccc3(255, 255, 255));
			stage_number_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
			stage_number_label->setAnchorPoint(ccp(0,0.5f));
			stage_number_label->setPosition(ccp(30, 250));
			setFormSetter(stage_number_label);
			main_case->addChild(stage_number_label);
			
			is_before_selected_event_stage = true;
			mySGD->is_before_selected_event_stage = false;
		}
		else
		{
			KSLabelTTF* piece_number_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_stageValue), stage_number)->getCString(),	mySGD->getFont().c_str(), 17.5f);
			setFormSetter(piece_number_label);
			piece_number_label->setColor(ccc3(255, 255, 255));
			piece_number_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
			piece_number_label->setAnchorPoint(ccp(0,0.5f));
			piece_number_label->setPosition(ccp(30, 250));
			main_case->addChild(piece_number_label);
			
			is_before_selected_event_stage = false;
		}
	}
	
//	if(!mySGD->is_endless_mode)
//	{
		CCScale9Sprite* mission_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		mission_back->setContentSize(CCSizeMake(278, 26));
		mission_back->setPosition(ccp(main_case->getContentSize().width*0.655f, main_case->getContentSize().height+2-23.5f));
		main_case->addChild(mission_back);
		
		CCRect mission_size = CCRectMake(0, 0, 225, 22);
		CCPoint mission_position = ccp(main_case->getContentSize().width/2.f+97, main_case->getContentSize().height+2-23.5f);
		
//		CCSprite* temp_mission = CCSprite::create("whitePaper.png", mission_size);
//		temp_mission->setOpacity(100);
//		temp_mission->setPosition(mission_position);
//		main_case->addChild(temp_mission);
	
		LabelTTFMarquee* mission_label = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), mission_size.size.width, mission_size.size.height, mySD->getConditionContent(stage_number).c_str());
		setFormSetter(mission_label);
		mission_label->setAnchorPoint(ccp(0.5,0.5));
		mission_label->setPosition(mission_position);
		main_case->addChild(mission_label);
		mission_label->setFontSize(12);
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
		
		CCSprite* mission_title_back = CCSprite::create("tabbutton_up.png");
		mission_title_back->setPosition(ccp(main_case->getContentSize().width*0.417f+9, main_case->getContentSize().height+2-25));
		main_case->addChild(mission_title_back);
		
		KSLabelTTF* mission_title_label = KSLabelTTF::create("MISSION", mySGD->getFont().c_str(), 12.5f);
		mission_title_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		mission_title_label->setPosition(ccpFromSize(mission_title_back->getContentSize()/2.f) + ccp(0,1));
		mission_title_back->addChild(mission_title_label);
		
//		CCScale9Sprite* mission_img = CCScale9Sprite::create("common_time.png", CCRectMake(0, 0, 22, 22), CCRectMake(10, 10, 2, 2));
//		setFormSetter(mission_img);
//		mission_img->setContentSize(CCSizeMake(65, 22));
//		mission_img->setPosition(ccp(198,255));
//		main_case->addChild(mission_img);
//		
//		KSLabelTTF* mission_img_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_mission), mySGD->getFont().c_str(), 12);
//		mission_img_label->setColor(ccc3(255, 170, 20));
//		mission_img_label->setPosition(ccp(mission_img->getContentSize().width/2.f, mission_img->getContentSize().height/2.f));
//		mission_img->addChild(mission_img_label);
//	}
	
	
	CommonButton* back_button = CommonButton::createCloseButton(touch_priority);
	setFormSetter(back_button);
	back_button->setPosition(ccp(main_case->getContentSize().width-20, main_case->getContentSize().height-12));
	back_button->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kStartSettingPopupMenuTag_back);
								 menuAction(t_node);
							 });
	main_case->addChild(back_button);
	
	
	item_list = mySD->getStageItemList(stage_number);
	
	if(myDSH->getIntegerForKey(kDSH_Key_showedScenario) != 2)
	{
		for(int i=0;i<item_list.size();i++)
		{
			ITEM_CODE t_code = item_list[i];
			if(t_code == kIC_baseSpeedUp && mySGD->getItem9OpenStage() <= mySGD->getUserdataHighPiece() && mySGD->isClearPiece(mySGD->getItem9OpenStage()) && !myDSH->getBoolForKey(kDSH_Key_isShowItem_int1, int(t_code)))
			{
				show_item_popup.push_back(t_code);
				myDSH->setBoolForKey(kDSH_Key_isShowItem_int1, int(t_code), true);
				
//				mySGD->addChangeGoods(CCString::createWithFormat("b_i_%d", t_code)->getCString());
			}
			else if(t_code == kIC_doubleItem && mySGD->getItem6OpenStage() <= mySGD->getUserdataHighPiece() && mySGD->isClearPiece(mySGD->getItem6OpenStage()) && !myDSH->getBoolForKey(kDSH_Key_isShowItem_int1, t_code))
			{
				show_item_popup.push_back(t_code);
				myDSH->setBoolForKey(kDSH_Key_isShowItem_int1, int(t_code), true);
				
//				mySGD->addChangeGoods(CCString::createWithFormat("b_i_%d", t_code)->getCString());
			}
			else if(t_code == kIC_magnet && mySGD->getItem11OpenStage() <= mySGD->getUserdataHighPiece() && mySGD->isClearPiece(mySGD->getItem11OpenStage()) && !myDSH->getBoolForKey(kDSH_Key_isShowItem_int1, t_code))
			{
				show_item_popup.push_back(t_code);
				myDSH->setBoolForKey(kDSH_Key_isShowItem_int1, int(t_code), true);
				
//				mySGD->addChangeGoods(CCString::createWithFormat("b_i_%d", t_code)->getCString());
			}
		}
//		mySGD->changeGoods([=](Json::Value result_data)
//						   {
//							   if(result_data["result"]["code"].asInt() == GDSUCCESS)
//							   {
////								   myDSH->saveUserData({kSaveUserData_Key_item}, nullptr);
//							   }
//						   });
		
		
		
		if(!show_item_popup.empty() && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) != 1)
		{
			is_tutorial = true;
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
			
			t_container->hide_func = [=](function<void()> end_func)
			{
				CommonAnimation::closePopup(t_popup, t_container, t_popup->getDimmedSprite(), [=](){
					
				}, [=](){
					end_func();
				});
			};
			
			CommonAnimation::openPopup(t_popup, t_container, t_popup->getDimmedSprite(), [=](){
				
			}, [=](){
				t_container->endShowAnimation();
			});
		}
	}
	else
	{
		is_tutorial = true;
		myDSH->setIntegerForKey(kDSH_Key_showedScenario, 3);
		
		CCNode* scenario_node = CCNode::create();
		addChild(scenario_node, 9999);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		
		CCNode* t_stencil_node = CCNode::create();
		
		
		CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
		t_clipping->setAlphaThreshold(0.1f);
		
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
		t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		
		
		CCSprite* t_gray = CCSprite::create("back_gray.png");
		t_gray->setScaleX(screen_scale_x);
		t_gray->setScaleY(myDSH->ui_top/myDSH->screen_convert_rate/320.f);
		t_gray->setOpacity(0);
		t_gray->setPosition(ccp(240,160));
		t_clipping->addChild(t_gray);
		
		t_clipping->setInverted(true);
		scenario_node->addChild(t_clipping, 0);
		
		CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
		ikaruga->setAnchorPoint(ccp(0,0));
		ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
		scenario_node->addChild(ikaruga, 1);
		
		TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
		typing_box->setHide();
		scenario_node->addChild(typing_box, 2);
		
		CCSprite* n_skip = CCSprite::create("kt_skip.png");
		CCSprite* s_skip = CCSprite::create("kt_skip.png");
		s_skip->setColor(ccGRAY);
		
		CCMenuLambda* skip_menu = CCMenuLambda::create();
		skip_menu->setPosition(ccp(240-240*screen_scale_x + 35, 160+160*screen_scale_y - 25 + 150));
		scenario_node->addChild(skip_menu, 3);
		skip_menu->setTouchPriority(-19999);
		skip_menu->setEnabled(false);
		
		CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [=](CCObject* sender)
																	 {
																		 skip_menu->setEnabled(false);
																		 
																		 is_menu_enable = false;
																		 callStart();
																		 
																		 addChild(KSTimer::create(0.1f, [=]()
																								  {
																									  scenario_node->removeFromParent();
																								  }));
																	 });
		skip_menu->addChild(skip_item);
		
		typing_box->showAnimation(0.3f);
		
		
		function<void()> end_func4 = [=]()
		{
			skip_menu->setEnabled(false);
			
			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																  {
																	  t_gray->setOpacity(255-255*t);
																  }, [=](float t)
																  {
																	  t_gray->setOpacity(0);
																	  is_menu_enable = false;
																	  callStart();
																	  scenario_node->removeFromParent();
																  }));
		};
		
		function<void()> end_func3 = [=]()
		{
			skip_menu->setVisible(true);
			
			t_clipping->removeChildByTag(1);
			t_stencil_node->removeChildByTag(2);
			
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent17), end_func4);
		};
		
		function<void()> end_func2 = [=]()
		{
			skip_menu->setVisible(false);
			ikaruga->setVisible(true);
			
			typing_box->setVisible(true);
			typing_box->setTouchSuction(true);
			
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent16), end_func3);
			
			CCSprite* t_arrow1 = CCSprite::create("kt_arrow_big.png");
			t_arrow1->setScale(0.6f);
			t_arrow1->setPosition(ccp(215, 160+160*screen_scale_y - 20));
			t_arrow1->setRotation(0);
			t_clipping->addChild(t_arrow1, 0, 1);
			
			CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			t_stencil1->setContentSize(CCSizeMake(140, 31));
			t_stencil1->setPosition(ccp(115, 160+160*screen_scale_y - 20));
			t_stencil_node->addChild(t_stencil1, 0, 2);
		};
		
		function<void()> end_func1 = [=]()
		{
			skip_menu->setVisible(false);
			
			ikaruga->setVisible(false);
			
			typing_box->setTouchOffScrollAndButton();
			typing_box->setVisible(false);
			
			CCSprite* t_arrow1 = CCSprite::create("kt_arrow_big.png");
			t_arrow1->setScale(0.6f);
			t_arrow1->setPosition(ccp(314, 221));
			t_arrow1->setRotation(90);
			t_clipping->addChild(t_arrow1);
			
			StyledLabelTTF* t_ment1 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_stageSettingDimmed1), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_ment1->setAnchorPoint(ccp(0.5f,1));
			t_ment1->setPosition(t_arrow1->getPosition() + ccp(0, -t_arrow1->getContentSize().height/2.f*t_arrow1->getScale() - 3));
			t_clipping->addChild(t_ment1);
			
			CCSprite* t_arrow2 = CCSprite::create("kt_arrow_big.png");
			t_arrow2->setScale(0.6f);
			t_arrow2->setPosition(ccp(314, 105));
			t_arrow2->setRotation(-90);
			t_clipping->addChild(t_arrow2);
			
			StyledLabelTTF* t_ment2 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_stageSettingDimmed2), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_ment2->setAnchorPoint(ccp(0.5f,0));
			t_ment2->setPosition(t_arrow2->getPosition() + ccp(0, t_arrow2->getContentSize().height/2.f*t_arrow2->getScale() + 3));
			t_clipping->addChild(t_ment2);
			
			CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			t_stencil1->setContentSize(CCSizeMake(282, 37));
			t_stencil1->setPosition(ccp(314, 265));
			t_stencil_node->addChild(t_stencil1);
			
			CCScale9Sprite* t_stencil2 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			t_stencil2->setContentSize(CCSizeMake(282, 56));
			t_stencil2->setPosition(ccp(314, 54));
			t_stencil_node->addChild(t_stencil2);
			
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
			scenario_node->addChild(t_suction);
			t_suction->setTouchEnabled(true);
			t_suction->touch_began_func = [=]()
			{
				skip_menu->setVisible(true);
				t_suction->is_on_touch_began_func = false;
				t_stencil_node->removeAllChildren();
				t_arrow1->removeFromParent();
				t_arrow2->removeFromParent();
				t_ment1->removeFromParent();
				t_ment2->removeFromParent();
				end_func2();
				t_suction->removeFromParent();
			};
			t_suction->is_on_touch_began_func = true;
			
			typing_box->setTouchSuction(false);
		};
		
		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
															  {
																  t_gray->setOpacity(t*255);
																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
															  }, [=](float t)
															  {
																  t_gray->setOpacity(255);
																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
																  skip_menu->setEnabled(true);
																  
																  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent15), end_func1);
															  }));
	}
	
	clicked_item_idx = -1;
	
	bool is_have_unlock_item = false;
	
	for(int i=0;i<item_list.size() && i < 3;i++)
	{
		ITEM_CODE t_ic = item_list[i];
		
		CCPoint item_position = ccp(213.f + i*65.f, 185);
		
		bool is_unlocked = true;
		if(t_ic == kIC_baseSpeedUp && (mySGD->getItem9OpenStage() > mySGD->getUserdataHighPiece() || !mySGD->isClearPiece(mySGD->getItem9OpenStage())))
			is_unlocked = false;
		else if(t_ic == kIC_doubleItem && (mySGD->getItem6OpenStage() > mySGD->getUserdataHighPiece() || !mySGD->isClearPiece(mySGD->getItem6OpenStage())))
			is_unlocked = false;
		else if(t_ic == kIC_magnet && (mySGD->getItem11OpenStage() > mySGD->getUserdataHighPiece() || !mySGD->isClearPiece(mySGD->getItem11OpenStage())))
			is_unlocked = false;
		
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
			is_price_usable = is_price_usable || (mySGD->getGoodsValue(mySGD->getItemCodeToGoodsType(t_ic)) > 0); // 소지하고 있는지
			
            bool is_endless_tutorial = (mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1);
            
			if(getSelectedItemCount() < 3 && (is_before_used_item || is_show_item_popup) && is_price_usable && is_unlocked && !is_endless_tutorial)
			{
				// mount
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
				n_img->setScale(0.7f);
				n_item_case->addChild(n_img);
				
				CCSprite* n_mount = CCSprite::create("startsetting_item_mounted_case.png");
				n_mount->setPosition(ccpFromSize(n_img->getContentSize()/2.f));
				n_mount->setScale(1.f/n_img->getScale());
				n_img->addChild(n_mount);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setColor(ccGRAY);
				s_img->setScale(0.7f);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
				s_item_case->addChild(s_img);
				
				CCSprite* s_mount = CCSprite::create("startsetting_item_mounted_case.png");
				s_mount->setPosition(ccpFromSize(s_img->getContentSize()/2.f));
				s_mount->setScale(1.f/s_img->getScale());
				s_img->addChild(s_mount);
				
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
				n_img->setScale(0.7f);
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setColor(ccGRAY);
				s_img->setScale(0.7f);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
				s_item_case->addChild(s_img);
				
				CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingPopup::itemAction));
				item_item->setTag(i+1);
				
				CCMenu* item_menu = CCMenu::createWithItem(item_item);
				item_menu->setPosition(CCPointZero);
				item_parent->addChild(item_menu);
				item_menu->setTouchPriority(touch_priority);
				
				item_menu->setEnabled(is_unlocked);
				
				is_selected_item.push_back(false);
			}
			
			int item_cnt = mySGD->getGoodsValue(mySGD->getItemCodeToGoodsType(t_ic));
//			if(is_show_item_popup)
//				item_cnt += mySGD->getBonusItemCnt(t_ic);
			CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 11);
			cnt_label->setPosition(ccp(16.5f, -18));
			item_parent->addChild(cnt_label, kStartSettingPopupItemZorder_cntLabel, kStartSettingPopupItemZorder_cntLabel);
			
			if(!is_unlocked)
				cnt_label->setVisible(false);
			
			CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
			clicked_img->setVisible(false);
			clicked_img->setPosition(CCPointZero);
			item_parent->addChild(clicked_img, kStartSettingPopupItemZorder_clicked, kStartSettingPopupItemZorder_clicked);
		}
		else
			is_selected_item.push_back(false);
		
		if(!is_unlocked)
		{
			CCSprite* locked_img = CCSprite::create("startsetting_item_locked.png");
			locked_img->setPosition(item_position);
			main_case->addChild(locked_img, 1);
		}
		else
		{
			is_have_unlock_item = true;
		}
	}
	
	item_gacha_menu = NULL;
	is_clicked_gacha_menu = false;
	
	gacha_clicked_img = CCSprite::create("startsetting_item_clicked.png");
	gacha_clicked_img->setPosition(ccp(410,185));
	gacha_clicked_img->setVisible(false);
	main_case->addChild(gacha_clicked_img, kStartSettingPopupZorder_main+1);
	
	gachaMenuCreate();
	
	if(selected_gacha_item > kIC_emptyBegin && selected_gacha_item < kIC_emptyEnd)
	{
		gacha_item = CCSprite::create(CCString::createWithFormat("item%d.png", selected_gacha_item)->getCString());
		gacha_item->setScale(0.7f);
		gacha_item->setPosition(ccp(410,185));
		main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
		
		CCSprite* mount_img = CCSprite::create("startsetting_item_mounted_case.png");
		mount_img->setPosition(ccpFromSize(gacha_item->getContentSize()/2.f));
		mount_img->setScale(1.f/gacha_item->getScale());
		gacha_item->addChild(mount_img, 1);
	}
	else
	{
        auto t_ccb = KS::loadCCBI<CCSprite*>(this, "startsetting_question2.ccbi");
		gacha_item = t_ccb.first;//CCSprite::create("startsetting_item_gacha_inner.png");
		gacha_item->setPosition(ccp(410,193));
		main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
        t_ccb.second->runAnimationsForSequenceNamed("Default Timeline");
		
		KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gacha), mySGD->getFont().c_str(), 12.5f);
		gacha_label->enableOuterStroke(ccBLACK, 1.f);
		gacha_label->setPosition(ccp(gacha_item->getContentSize().width/2.f, gacha_item->getContentSize().height/2.f - 23.f));
		gacha_item->addChild(gacha_label);
	}
	
	if(mySGD->getItemGachaOpenStage() > mySD->getSilType())
	{
		item_gacha_menu->setEnabled(false);
		CCSprite* locked_img = CCSprite::create("startsetting_item_locked.png");

		locked_img->setPosition(ccp(410,185));
		setFormSetter(locked_img);
		main_case->addChild(locked_img, kStartSettingPopupZorder_main);
	}
	else
	{
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, int(KindTutorialType::kNewItem_gacha)) && !mySGD->is_endless_mode)
		{
			is_tutorial = true;
			myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, int(KindTutorialType::kNewItem_gacha), true);
			
			CCNode* scenario_node = CCNode::create();
			addChild(scenario_node, 9999);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
			
			
			CCNode* t_stencil_node = CCNode::create();
			
			CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			t_stencil1->setContentSize(CCSizeMake(64, 64));
			t_stencil1->setPosition(ccp(410, 191));
			t_stencil_node->addChild(t_stencil1);
			
			CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
			t_clipping->setAlphaThreshold(0.1f);
			
			float change_scale = 1.f;
			CCPoint change_origin = ccp(0,0);
			if(screen_scale_x > 1.f)
			{
				change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
				change_scale = screen_scale_x;
			}
			if(screen_scale_y > 1.f)
				change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
			CCSize win_size = CCDirector::sharedDirector()->getWinSize();
			t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
			
			
			CCSprite* t_gray = CCSprite::create("back_gray.png");
			t_gray->setScaleX(screen_scale_x);
			t_gray->setScaleY(myDSH->ui_top/myDSH->screen_convert_rate/320.f);
			t_gray->setOpacity(0);
			t_gray->setPosition(ccp(240,160));
			t_clipping->addChild(t_gray);
			
			t_clipping->setInverted(true);
			scenario_node->addChild(t_clipping, 0);
			
			
			CCSprite* asuka = CCSprite::create("kt_cha_asuka_1.png");
			asuka->setAnchorPoint(ccp(1,0));
			asuka->setPosition(ccp(240+240*screen_scale_x+asuka->getContentSize().width, 160-160*screen_scale_y));
			asuka->setVisible(false);
			scenario_node->addChild(asuka, 1);
			
			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
			ikaruga->setAnchorPoint(ccp(0,0));
			ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
			scenario_node->addChild(ikaruga, 1);
			
			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
			typing_box->setHide();
			scenario_node->addChild(typing_box, 2);
			
			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 60), ccp(255, 60));
			scenario_node->addChild(typing_box2, 2);
			
			typing_box2->setTouchOffScrollAndButton();
			typing_box2->setVisible(false);
			typing_box2->setTouchSuction(false);
			
			typing_box->showAnimation(0.3f);
			
			function<void()> end_func3 = [=]()
			{
				addChild(KSTimer::create(0.1f, [=]()
										 {
											 scenario_node->removeFromParent();
										 }));
			};
			
			function<void()> end_func2 = [=]()
			{
				TypingBox::changeTypingBox(typing_box2, typing_box, asuka, ikaruga);
				typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial10), end_func3);
			};
			
			function<void()> end_func1 = [=]()
			{
				ikaruga->setVisible(false);
				asuka->setVisible(true);
				
				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
																	  {
																		  asuka->setPositionX(240+240*screen_scale_x+asuka->getContentSize().width - asuka->getContentSize().width*2.f/3.f*t);
																	  }, [=](float t)
																	  {
																		  asuka->setPositionX(240+240*screen_scale_x+asuka->getContentSize().width - asuka->getContentSize().width*2.f/3.f*t);
																		  
																		  typing_box2->setVisible(true);
																		  typing_box2->setTouchSuction(true);
																		  
																		  typing_box->setTouchSuction(false);
																		  
																		  typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial9), end_func2);
																	  }));
				typing_box->setTouchOffScrollAndButton();
				typing_box->setVisible(false);
			};
			
			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																  {
																	  t_gray->setOpacity(t*255);
																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																  }, [=](float t)
																  {
																	  t_gray->setOpacity(255);
																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																	  
																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial8), end_func1);
																  }));
		}
	}
	
	CCScale9Sprite* script_box = CCScale9Sprite::create("startsetting_scriptbox.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	script_box->setContentSize(CCSizeMake(276, 63));
	
	script_box->setPosition(ccp(314,114));
	setFormSetter(script_box);
	main_case->addChild(script_box);
	
	const char* readyLbl;
	if(mySGD->is_endless_mode)
	{
		readyLbl = myLoc->getLocalForKey(LK::kMyLocalKey_battleStart);
	}
	else
	{
		readyLbl = myLoc->getLocalForKey(LK::kMyLocalKey_gamestart);
	}
	
	CCLabelTTF* t_label = CCLabelTTF::create();
	KSLabelTTF* start_label = KSLabelTTF::create(readyLbl, mySGD->getFont().c_str(), 30.f);
	start_label->disableOuterStroke();
	t_label->addChild(start_label);
	
	if(mySGD->isTimeEvent(kTimeEventType_heart))
	{
		CCSprite* time_event_back = CCSprite::create("startsetting_event.png");
		time_event_back->setPosition(ccp(0,-20) + ccp(88.5f, 20.5f));
		t_label->addChild(time_event_back);
		setFormSetter(time_event_back);
		KSLabelTTF* time_event_back_lbl = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_heartFree), mySGD->getFont().c_str(), 10.f);
		time_event_back_lbl->disableOuterStroke();
		time_event_back->addChild(time_event_back_lbl);
		time_event_back_lbl->setPosition(ccpFromSize(time_event_back->getContentSize()) / 2.f + ccp(3, -5.f));
	}
	
	CCScale9Sprite* start_back = CCScale9Sprite::create("mainbutton_purple.png", CCRectMake(0, 0, 215, 65), CCRectMake(107, 32, 1, 1));
	
	CCControlButton* start_button = CCControlButton::create(t_label, start_back);
	start_button->setPreferredSize(CCSizeMake(285, 65));
	start_button->addTargetWithActionForControlEvents(this, cccontrol_selector(StartSettingPopup::startButtonAction), CCControlEventTouchUpInside);
	start_button->setPosition(ccp(314,49));
	main_case->addChild(start_button);
	start_button->setTouchPriority(touch_priority);
	
	
	if(mySGD->is_hell_mode_enabled)
	{
		cumber_node = CCNode::create();
		setFormSetter(cumber_node);
		cumber_node->setPosition(ccp(left_back->getPositionX(),158));
		main_case->addChild(cumber_node);
		
		t_cumber = CumberShowWindow::create(mySD->getSilType(), kCumberShowWindowSceneCode_cardChange);
		//	t_cumber->setPosition(ccp(83,158));
		cumber_node->addChild(t_cumber);
		
		int character_no = mySGD->getSelectedCharacterHistory().characterNo.getV();
		
		auto t_char = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, character_no) + ".ccbi");
		character_img = t_char.first;
		character_manager = t_char.second;
		
		character_img->setPosition(ccp(left_back->getPositionX()-60, 100));
		main_case->addChild(character_img);
		
		repeat_character_action = [=]()
		{
			//		if(character_no == 1)
			//		{
			//			character_manager->runAnimationsForSequenceNamed("move");
			//			character_img->setScaleX(1.f);
			//		}
			//		else if(character_no == 2)
			//		{
			character_manager->runAnimationsForSequenceNamed("move_right");
			//		}
			character_img->addChild(KSGradualValue<float>::create(left_back->getPositionX()-60, left_back->getPositionX()+60, 0.8f, [=](float t)
																  {
																	  character_img->setPositionX(t);
																  }, [=](float t)
																  {
																	  character_img->setPositionX(t);
																	  //										  if(character_no == 1)
																	  //											{
																	  //												character_manager->runAnimationsForSequenceNamed("move");
																	  //												character_img->setScaleX(-1.f);
																	  //											}
																	  //										  else if(character_no == 2)
																	  //											{
																	  character_manager->runAnimationsForSequenceNamed("move_left");
																	  //											}
																	  character_img->addChild(KSGradualValue<float>::create(left_back->getPositionX()+60, left_back->getPositionX()-60, 0.8f, [=](float t)
																															{
																																character_img->setPositionX(t);
																															}, [=](float t)
																															{
																																character_img->setPositionX(t);
																																this->repeat_character_action();
																															}));
																  }));
			
		};
		
		repeat_character_action();
		
		CCSprite* level_case = CCSprite::create("startsetting_levelbox.png");
		setFormSetter(level_case);
		level_case->setPosition(ccp(left_back->getPositionX(),75));
		main_case->addChild(level_case);
		
		StoneType missile_type_code = StoneType(mySGD->getUserdataSelectedCharNO()-1);
		missile_type_code = kStoneType_guided; // 임시
		
		int missile_level = mySGD->getSelectedCharacterHistory().level.getV();
		
		if(missile_type_code == kStoneType_guided)
		{
			int grade = (missile_level-1)/5+1;
			bool rotation = false;
			if(grade == 1 || grade == 4)
				rotation = true;
			GuidedMissile* t_gm = GuidedMissile::createForShowStartSettingPopup(character_img, CCString::createWithFormat("jack_missile_%02d_%02d.png", mySGD->getUserdataSelectedCharNO(), missile_level)->getCString(),
																				rotation, (missile_level-1)/5+1, (missile_level-1)%5+1);
			//		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("me_guide%d.ccbi", (missile_level-1)%5 + 1)->getCString());
			t_gm->setFunctionForCrash([=](){
				cumber_node->stopAllActions();
				cumber_node->runAction(CCSequence::create(CCScaleBy::create(0.06f,0.9),CCScaleTo::create(0.1,1), NULL));
			});
			t_gm->setPosition(ccp(left_back->getPositionX(),158));
			main_case->addChild(t_gm);
			
			
			
			t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
			
			missile_img = t_gm;
		}
		
		missile_data_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_levelValue), missile_level)->getCString(), mySGD->getFont().c_str(), 12);
		setFormSetter(missile_data_level);
		missile_data_level->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		missile_data_level->setPosition(ccp(left_back->getPositionX()-29,75));
		main_case->addChild(missile_data_level);
		
		missile_data_power = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_powerValue), KS::insert_separator(mySGD->getSelectedCharacterHistory().power.getV()).c_str())->getCString(), mySGD->getFont().c_str(), 12);
		setFormSetter(missile_data_power);
		missile_data_power->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		missile_data_power->setPosition(ccp(left_back->getPositionX()+29,75));
		main_case->addChild(missile_data_power);
		
		if(mySGD->getSelectedCharacterHistory().isMaxLevel.getV())
		{
			CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
			//		setFormSetter(n_upgrade);
			//		KSLabelTTF* n_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_maxLevel), missile_level)->getCString(), mySGD->getFont().c_str(), 14);
			//		setFormSetter(n_level);
			//		n_level->enableOuterStroke(ccBLACK, 1, int(255*0.5), true);
			//		n_level->setPosition(ccp(70,46));
			//		n_upgrade->addChild(n_level);
			KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missileUpgrade2), mySGD->getFont().c_str(), 12);
			n_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
			n_label->setPosition(ccpFromSize(n_upgrade->getContentSize()/2.f));
			n_upgrade->addChild(n_label);
			
			CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
			s_upgrade->setColor(ccGRAY);
			//		setFormSetter(s_upgrade);
			//		KSLabelTTF* s_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_maxLevel), missile_level)->getCString(), mySGD->getFont().c_str(), 14);
			//		setFormSetter(s_level);
			//		s_level->enableOuterStroke(ccBLACK, 1, int(255*0.5), true);
			//		s_level->setPosition(ccp(70,46));
			//		s_upgrade->addChild(s_level);
			KSLabelTTF* s_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missileUpgrade2), mySGD->getFont().c_str(), 12);
			s_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
			s_label->setPosition(ccpFromSize(s_upgrade->getContentSize()/2.f));
			s_upgrade->addChild(s_label);
			
			//		KSLabelTTF* n_price_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endUpgrade), mySGD->getFont().c_str(), 17.5f);
			//		setFormSetter(n_price_label);
			//		n_price_label->disableOuterStroke();
			//		n_price_label->setPosition(ccp(70,20));
			//		n_upgrade->addChild(n_price_label);
			//
			//		KSLabelTTF* s_price_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endUpgrade), mySGD->getFont().c_str(), 17.5f);
			//		setFormSetter(s_price_label);
			//		s_price_label->disableOuterStroke();
			//		s_price_label->setPosition(ccp(70,20));
			//		s_upgrade->addChild(s_price_label);
			
			
			CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
			setFormSetter(upgrade_item);
			upgrade_menu = CCMenu::createWithItem(upgrade_item);
			upgrade_menu->setPosition(ccp(left_back->getPositionX()+36,43));
			main_case->addChild(upgrade_menu);
			
			upgrade_menu->setTouchPriority(touch_priority);
			upgrade_menu->setEnabled(false);
		}
		else
		{
			CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
			KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missileUpgrade2), mySGD->getFont().c_str(), 12);
			n_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
			n_label->setPosition(ccpFromSize(n_upgrade->getContentSize()/2.f));
			n_upgrade->addChild(n_label);
			
			CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
			s_upgrade->setColor(ccGRAY);
			KSLabelTTF* s_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missileUpgrade2), mySGD->getFont().c_str(), 12);
			s_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
			s_label->setPosition(ccpFromSize(s_upgrade->getContentSize()/2.f));
			s_upgrade->addChild(s_label);
			
			//		CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
			//		setFormSetter(n_upgrade);
			//		KSLabelTTF* n_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_upgradeLevelValue), missile_level+1)->getCString(), mySGD->getFont().c_str(), 14);
			//		setFormSetter(n_level);
			//		n_level->enableOuterStroke(ccBLACK, 1, int(255*0.5), true);
			//		n_level->setPosition(ccp(70,46));
			//		n_upgrade->addChild(n_level);
			//
			//		CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
			//		setFormSetter(s_upgrade);
			//		s_upgrade->setColor(ccGRAY);
			//		KSLabelTTF* s_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_upgradeLevelValue), missile_level+1)->getCString(), mySGD->getFont().c_str(), 14);
			//		setFormSetter(s_level);
			//		s_level->enableOuterStroke(ccBLACK, 1, int(255*0.5), true);
			//		s_level->setPosition(ccp(70,46));
			//		s_upgrade->addChild(s_level);
			//
			//
			//		if(mySGD->getGoodsValue(kGoodsType_pass3) > 0)
			//		{
			//			CCSprite* n_pass_ticket = CCSprite::create("pass_ticket3.png");
			//
			//			n_pass_ticket->setPosition(ccp(28,21.f));
			//			n_upgrade->addChild(n_pass_ticket);
			//
			//			{
			//				CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
			//				pass_count_case->setPosition(ccpFromSize(n_pass_ticket->getContentSize()/2.f) + ccp(9,6));
			//				n_pass_ticket->addChild(pass_count_case);
			//
			//				CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass3)), mySGD->getFont().c_str(), 8);
			//				pass_count_label->setColor(ccc3(255, 255, 255));
			//				float min_width = 10+pass_count_label->getContentSize().width;
			//				if(min_width < 20)
			//					min_width = 20;
			//				pass_count_case->setContentSize(CCSizeMake(min_width, 20));
			//				pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
			//				pass_count_case->addChild(pass_count_label);
			//			}
			//
			//			KSLabelTTF* n_free_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 17.5f);
			//			n_free_label->disableOuterStroke();
			//			n_free_label->setPosition(ccp(78,20));
			//			n_upgrade->addChild(n_free_label);
			//
			//			CCSprite* s_pass_ticket = CCSprite::create("pass_ticket3.png");
			//			s_pass_ticket->setPosition(ccp(28,21.f));
			//			s_upgrade->addChild(s_pass_ticket);
			//
			//			{
			//				CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
			//				pass_count_case->setPosition(ccpFromSize(s_pass_ticket->getContentSize()/2.f) + ccp(9,6));
			//				s_pass_ticket->addChild(pass_count_case);
			//
			//				CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass3)), mySGD->getFont().c_str(), 8);
			//				pass_count_label->setColor(ccc3(255, 255, 255));
			//				float min_width = 10+pass_count_label->getContentSize().width;
			//				if(min_width < 20)
			//					min_width = 20;
			//				pass_count_case->setContentSize(CCSizeMake(min_width, 20));
			//				pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
			//				pass_count_case->addChild(pass_count_label);
			//			}
			//
			//			KSLabelTTF* s_free_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 17.5f);
			//			s_free_label->disableOuterStroke();
			//			s_free_label->setPosition(ccp(78,20));
			//			s_upgrade->addChild(s_free_label);
			//		}
			//		else
			//		{
			//			CCSprite* n_price_type = CCSprite::create("price_gold_img.png");
			//			n_price_type->setPosition(ccp(25,20));
			//			n_upgrade->addChild(n_price_type);
			//			KSLabelTTF* n_price_label = KSLabelTTF::create(KS::insert_separator(
			//												ccsf("%d", mySGD->getSelectedCharacterHistory().nextPrice.getV())).c_str(), mySGD->getFont().c_str(), 17.5f);
			//			n_price_label->disableOuterStroke();
			//			n_price_label->setPosition(ccp(78,20));
			//			n_upgrade->addChild(n_price_label);
			//
			//			CCSprite* s_price_type = CCSprite::create("price_gold_img.png");
			//			s_price_type->setPosition(ccp(25,20));
			//			s_upgrade->addChild(s_price_type);
			//			KSLabelTTF* s_price_label = KSLabelTTF::create(KS::insert_separator(
			//												ccsf("%d", mySGD->getSelectedCharacterHistory().nextPrice.getV())).c_str(), mySGD->getFont().c_str(), 17.5f);
			//			s_price_label->disableOuterStroke();
			//			s_price_label->setPosition(ccp(78,20));
			//			s_upgrade->addChild(s_price_label);
			//		}
			
			
			CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
			
			upgrade_menu = CCMenu::createWithItem(upgrade_item);
			upgrade_menu->setPosition(ccp(left_back->getPositionX()+36,43));
			main_case->addChild(upgrade_menu);
			
			upgrade_menu->setTouchPriority(touch_priority);
		}
		
		CCSprite* n_changeCharacter = CCSprite::create("startsetting_upgrade.png");
		KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_changeCharacter), mySGD->getFont().c_str(), 12);
		n_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
		n_label->setPosition(ccpFromSize(n_changeCharacter->getContentSize()/2.f));
		n_changeCharacter->addChild(n_label);
		
		
		CCSprite* s_changeCharacter = CCSprite::create("startsetting_upgrade.png");
		s_changeCharacter->setColor(ccGRAY);
		KSLabelTTF* s_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_changeCharacter), mySGD->getFont().c_str(), 12);
		s_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
		s_label->setPosition(ccpFromSize(s_changeCharacter->getContentSize()/2.f));
		s_changeCharacter->addChild(s_label);
		
		
		CCMenuItem* changeCharacter_item = CCMenuItemSprite::create(n_changeCharacter, s_changeCharacter, this, menu_selector(StartSettingPopup::changeCharacterAction));
		
		CCMenu* changeCharacter_menu = CCMenu::createWithItem(changeCharacter_item);
		changeCharacter_menu->setPosition(ccp(left_back->getPositionX()-36,43));
		main_case->addChild(changeCharacter_menu);
		
		changeCharacter_menu->setTouchPriority(touch_priority);
	}
	else
	{
		cumber_node = CCNode::create();
		setFormSetter(cumber_node);
		cumber_node->setPosition(ccp(left_back->getPositionX(),158));
		main_case->addChild(cumber_node);
		
		t_cumber = CumberShowWindow::create(mySD->getSilType(), kCumberShowWindowSceneCode_cardChange);
		//	t_cumber->setPosition(ccp(83,158));
		cumber_node->addChild(t_cumber);
		
		
		CCSprite* level_case = CCSprite::create("startsetting_levelbox.png");
		setFormSetter(level_case);
		level_case->setPosition(ccp(left_back->getPositionX(),95));
		main_case->addChild(level_case);
		
		StoneType missile_type_code = StoneType(mySGD->getSelectedCharacterHistory().characterNo.getV()-1);
		missile_type_code = kStoneType_guided; // 임시
		
		int missile_level = mySGD->getSelectedCharacterHistory().level.getV();
		
		if(missile_type_code == kStoneType_guided)
		{
			int grade = (missile_level-1)/5+1;
			bool rotation = false;
			if(grade == 1 || grade == 4)
				rotation = true;
			GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("jack_missile_%d.png", missile_level)->getCString(),
																	 rotation, (missile_level-1)/5+1, (missile_level-1)%5+1);
			//		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("me_guide%d.ccbi", (missile_level-1)%5 + 1)->getCString());
			t_gm->setFunctionForCrash([=](){
				cumber_node->stopAllActions();
				cumber_node->runAction(CCSequence::create(CCScaleBy::create(0.06f,0.9),CCScaleTo::create(0.1,1), NULL));
			});
			t_gm->setPosition(ccp(left_back->getPositionX(),158));
			main_case->addChild(t_gm);
			
			
			
			t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
			
			missile_img = t_gm;
		}
		
		missile_data_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_levelValue), missile_level)->getCString(), mySGD->getFont().c_str(), 12);
		setFormSetter(missile_data_level);
		missile_data_level->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		missile_data_level->setPosition(ccp(left_back->getPositionX()-29,95));
		main_case->addChild(missile_data_level);
		
		missile_data_power = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_powerValue), KS::insert_separator(mySGD->getSelectedCharacterHistory().power.getV()).c_str())->getCString(), mySGD->getFont().c_str(), 12);
		setFormSetter(missile_data_power);
		missile_data_power->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		missile_data_power->setPosition(ccp(left_back->getPositionX()+29,95));
		main_case->addChild(missile_data_power);
		
		if(mySGD->getSelectedCharacterHistory().isMaxLevel.getV())
		{
			CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
			setFormSetter(n_upgrade);
			KSLabelTTF* n_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_maxLevel), missile_level)->getCString(), mySGD->getFont().c_str(), 14);
			setFormSetter(n_level);
			n_level->enableOuterStroke(ccBLACK, 1, int(255*0.5), true);
			n_level->setPosition(ccp(70,46));
			n_upgrade->addChild(n_level);
			
			CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
			s_upgrade->setColor(ccGRAY);
			setFormSetter(s_upgrade);
			KSLabelTTF* s_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_maxLevel), missile_level)->getCString(), mySGD->getFont().c_str(), 14);
			setFormSetter(s_level);
			s_level->enableOuterStroke(ccBLACK, 1, int(255*0.5), true);
			s_level->setPosition(ccp(70,46));
			s_upgrade->addChild(s_level);
			
			
			KSLabelTTF* n_price_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endUpgrade), mySGD->getFont().c_str(), 17.5f);
			setFormSetter(n_price_label);
			n_price_label->disableOuterStroke();
			n_price_label->setPosition(ccp(70,20));
			n_upgrade->addChild(n_price_label);
			
			KSLabelTTF* s_price_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endUpgrade), mySGD->getFont().c_str(), 17.5f);
			setFormSetter(s_price_label);
			s_price_label->disableOuterStroke();
			s_price_label->setPosition(ccp(70,20));
			s_upgrade->addChild(s_price_label);
			
			
			CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
			setFormSetter(upgrade_item);
			upgrade_menu = CCMenu::createWithItem(upgrade_item);
			upgrade_menu->setPosition(ccp(left_back->getPositionX(),51));
			main_case->addChild(upgrade_menu);
			
			upgrade_menu->setTouchPriority(touch_priority);
			upgrade_menu->setEnabled(false);
		}
		else
		{
			CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
			setFormSetter(n_upgrade);
			KSLabelTTF* n_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_upgradeLevelValue), missile_level+1)->getCString(), mySGD->getFont().c_str(), 14);
			setFormSetter(n_level);
			n_level->enableOuterStroke(ccBLACK, 1, int(255*0.5), true);
			n_level->setPosition(ccp(70,46));
			n_upgrade->addChild(n_level);
			
			CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
			setFormSetter(s_upgrade);
			s_upgrade->setColor(ccGRAY);
			KSLabelTTF* s_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_upgradeLevelValue), missile_level+1)->getCString(), mySGD->getFont().c_str(), 14);
			setFormSetter(s_level);
			s_level->enableOuterStroke(ccBLACK, 1, int(255*0.5), true);
			s_level->setPosition(ccp(70,46));
			s_upgrade->addChild(s_level);
			
			
			if(mySGD->getGoodsValue(kGoodsType_pass3) > 0)
			{
				CCSprite* n_pass_ticket = CCSprite::create("pass_ticket3.png");
				
				n_pass_ticket->setPosition(ccp(28,21.f));
				n_upgrade->addChild(n_pass_ticket);
				
				{
					CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
					pass_count_case->setPosition(ccpFromSize(n_pass_ticket->getContentSize()/2.f) + ccp(9,6));
					n_pass_ticket->addChild(pass_count_case);
					
					CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass3)), mySGD->getFont().c_str(), 8);
					pass_count_label->setColor(ccc3(255, 255, 255));
					float min_width = 10+pass_count_label->getContentSize().width;
					if(min_width < 20)
						min_width = 20;
					pass_count_case->setContentSize(CCSizeMake(min_width, 20));
					pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
					pass_count_case->addChild(pass_count_label);
				}
				
				KSLabelTTF* n_free_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 17.5f);
				n_free_label->disableOuterStroke();
				n_free_label->setPosition(ccp(78,20));
				n_upgrade->addChild(n_free_label);
				
				CCSprite* s_pass_ticket = CCSprite::create("pass_ticket3.png");
				s_pass_ticket->setPosition(ccp(28,21.f));
				s_upgrade->addChild(s_pass_ticket);
				
				{
					CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
					pass_count_case->setPosition(ccpFromSize(s_pass_ticket->getContentSize()/2.f) + ccp(9,6));
					s_pass_ticket->addChild(pass_count_case);
					
					CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass3)), mySGD->getFont().c_str(), 8);
					pass_count_label->setColor(ccc3(255, 255, 255));
					float min_width = 10+pass_count_label->getContentSize().width;
					if(min_width < 20)
						min_width = 20;
					pass_count_case->setContentSize(CCSizeMake(min_width, 20));
					pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
					pass_count_case->addChild(pass_count_label);
				}
				
				KSLabelTTF* s_free_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 17.5f);
				s_free_label->disableOuterStroke();
				s_free_label->setPosition(ccp(78,20));
				s_upgrade->addChild(s_free_label);
			}
			else
			{
				CCSprite* n_price_type = CCSprite::create("price_gold_img.png");
				n_price_type->setPosition(ccp(25,20));
				n_upgrade->addChild(n_price_type);
				KSLabelTTF* n_price_label = KSLabelTTF::create(KS::insert_separator(
																					ccsf("%d", mySGD->getSelectedCharacterHistory().nextPrice.getV())).c_str(), mySGD->getFont().c_str(), 17.5f);
				n_price_label->disableOuterStroke();
				n_price_label->setPosition(ccp(78,20));
				n_upgrade->addChild(n_price_label);
				
				CCSprite* s_price_type = CCSprite::create("price_gold_img.png");
				s_price_type->setPosition(ccp(25,20));
				s_upgrade->addChild(s_price_type);
				KSLabelTTF* s_price_label = KSLabelTTF::create(KS::insert_separator(
																					ccsf("%d", mySGD->getSelectedCharacterHistory().nextPrice.getV())).c_str(), mySGD->getFont().c_str(), 17.5f);
				s_price_label->disableOuterStroke();
				s_price_label->setPosition(ccp(78,20));
				s_upgrade->addChild(s_price_label);
			}
			
			
			CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
			
			upgrade_menu = CCMenu::createWithItem(upgrade_item);
			upgrade_menu->setPosition(ccp(left_back->getPositionX(),51));
			main_case->addChild(upgrade_menu);
			
			upgrade_menu->setTouchPriority(touch_priority);
		}
	}
	
	
	CCRect option_size = CCRectMake(0, 0, 240, 25);
	CCPoint option_position = ccp(314,114);
	
//	CCSprite* option_rect = CCSprite::create("whitePaper.png", option_size);
//	option_rect->setOpacity(100);
//	option_rect->setAnchorPoint(ccp(0.5f,0.5f));
//	option_rect->setPosition(option_position);
//	main_case->addChild(option_rect);
	
	string option_ment;
	if(is_have_unlock_item)
		option_ment = myLoc->getLocalForKey(LK::kMyLocalKey_selectUseItem);
	else
		option_ment = myLoc->getLocalForKey(LK::kMyLocalKey_notUseItem);
	
	option_label = CCLabelTTF::create(option_ment.c_str(), mySGD->getFont().c_str(), 11, option_size.size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
	setFormSetter(option_label);
	option_label->setAnchorPoint(ccp(0.5,0.5));
	option_label->setPosition(ccp(314,114));
	main_case->addChild(option_label);

	
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, [=](){
		
		if(mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
		{
			is_menu_enable = false;
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			CCNode* t_stencil_node = CCNode::create();
			CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			t_stencil1->setContentSize(start_button->getContentSize() + CCSizeMake(5, 5));
			t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + start_button->getPosition());
			t_stencil_node->addChild(t_stencil1);
			
			CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
			t_clipping->setAlphaThreshold(0.1f);
			
			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
			
			float change_scale = 1.f;
			CCPoint change_origin = ccp(0,0);
			if(screen_scale_x > 1.f)
			{
				change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
				change_scale = screen_scale_x;
			}
			if(screen_scale_y > 1.f)
				change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
			CCSize win_size = CCDirector::sharedDirector()->getWinSize();
			t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
			
			
			CCSprite* t_gray = CCSprite::create("back_gray.png");
			t_gray->setScaleX(screen_scale_x);
			t_gray->setScaleY(myDSH->ui_top/myDSH->screen_convert_rate/320.f);
			t_gray->setOpacity(0);
			t_gray->setPosition(ccp(240,160));
			t_clipping->addChild(t_gray);
			
			CCSprite* t_arrow1 = CCSprite::create("kt_arrow_big.png");
			t_arrow1->setRotation(-90);
			t_arrow1->setPosition(t_stencil1->getPosition() + ccp(0,t_stencil1->getContentSize().height/2.f + 30));
			t_clipping->addChild(t_arrow1);
			
			CCMoveTo* t_up = CCMoveTo::create(0.4f, t_arrow1->getPosition() + ccp(0,30));
			CCMoveTo* t_down = CCMoveTo::create(0.4f, t_arrow1->getPosition());
			CCSequence* t_seq = CCSequence::create(t_up, t_down, NULL);
			
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
			
			t_arrow1->runAction(t_repeat);
			
			TouchSuctionLayer* t_suction_temp = TouchSuctionLayer::create(-9998);
			addChild(t_suction_temp);
			t_suction_temp->setTouchEnabled(true);
			
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
			addChild(t_suction);
			
			t_suction->setSwallowRect(CCRectMake(t_stencil1->getPosition().x-t_stencil1->getContentSize().width/2.f, t_stencil1->getPosition().y-t_stencil1->getContentSize().height/2.f, t_stencil1->getContentSize().width, t_stencil1->getContentSize().height));
			
			t_suction->setTouchEnabled(true);
			
			t_clipping->setInverted(true);
			addChild(t_clipping, 9999);
			
			addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
												   {
													   t_gray->setOpacity(t*255);
												   }, [=](float t)
												   {
													   t_gray->setOpacity(255);
													   is_menu_enable = true;
													   
													   t_suction->touch_began_func = [=]()
													   {
														   t_suction->is_on_touch_began_func = false;
														   if(!is_menu_enable)
															   return;
														   
														   is_menu_enable = false;
														   
														   tutorial_success_func = [=]()
														   {
															   t_arrow1->removeFromParent();
															   
															   t_suction_temp->removeFromParent();
															   t_suction->removeFromParent();
															   
															   addChild(KSGradualValue<float>::create(1.f, 0.f, 0.5f, [=](float t)
																									  {
																										  t_gray->setOpacity(255*t);
																									  }, [=](float t)
																									  {
																										  t_gray->setOpacity(255*t);
																										  
																										  t_clipping->removeFromParent();
																									  }));
														   };
														   
														   tutorial_fail_func = [=]()
														   {
															   t_suction->is_on_touch_began_func = true;
															   is_menu_enable = true;
														   };
														   
														   callStart();
													   };
													   t_suction->is_on_touch_began_func = true;
													   
												   }));
		}
		else if(!is_tutorial && mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_levelupGuide) && mySGD->getUserdataTotalPlayCount() >= mySGD->getLevelupGuidePlayCount() &&
				mySGD->getSelectedCharacterHistory().level.getV() <= mySGD->getLevelupGuideConditionLevel())
		{
			is_menu_enable = false;
			LevelupGuidePopup* t_popup = LevelupGuidePopup::create(-300, [=](){is_menu_enable = true;}, [=]()
																														 {
																															 is_menu_enable = true;
																															 upgradeAction(NULL);
																														 });
			addChild(t_popup, kStartSettingPopupZorder_popup);
		}
		else
		{
			is_menu_enable = true;
		}
	});
}

void StartSettingPopup::changeCharacterAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){
		main_case->setScaleY(t);
	}, [=](float t){
		main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){
			main_case->setScaleY(t);
		}, [=](float t){
			main_case->setScaleY(0.f);
			CharacterSelectPopup* t_popup = CharacterSelectPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(StartSettingPopup::characterClose));
			addChild(t_popup, kStartSettingPopupZorder_popup);
		}));
	}));
}

void StartSettingPopup::characterClose()
{
	if(character_img)
	{
		character_img->removeFromParent();
		character_img = NULL;
	}
	
	int character_no = mySGD->getSelectedCharacterHistory().characterNo.getV();
	
	auto t_char = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, character_no) + ".ccbi");
	character_img = t_char.first;
	character_manager = t_char.second;
	
	character_img->setPosition(ccp(left_back->getPositionX()-60, 100));
	character_img->setScaleX(1.f);
	main_case->addChild(character_img);
	
	repeat_character_action = [=]()
	{
		//		if(character_no == 1)
		//		{
		//			character_manager->runAnimationsForSequenceNamed("move");
		//			character_img->setScaleX(1.f);
		//		}
		//		else if(character_no == 2)
		//		{
		character_manager->runAnimationsForSequenceNamed("move_right");
		//		}
		character_img->addChild(KSGradualValue<float>::create(left_back->getPositionX()-60, left_back->getPositionX()+60, 0.8f, [=](float t)
															  {
																  character_img->setPositionX(t);
															  }, [=](float t)
															  {
																  character_img->setPositionX(t);
																  //																  if(character_no == 1)
																  //																  {
																  //																	  character_manager->runAnimationsForSequenceNamed("move");
																  //																	  character_img->setScaleX(-1.f);
																  //																  }
																  //																  else if(character_no == 2)
																  //																  {
																  character_manager->runAnimationsForSequenceNamed("move_left");
																  //																  }
																  character_img->addChild(KSGradualValue<float>::create(left_back->getPositionX()+60, left_back->getPositionX()-60, 0.8f, [=](float t)
																														{
																															character_img->setPositionX(t);
																														}, [=](float t)
																														{
																															character_img->setPositionX(t);
																															this->repeat_character_action();
																														}));
															  }));
		
	};
	
	repeat_character_action();
	
	CCPoint keep_position = missile_img->getPosition();
	missile_img->removeFromParent();
	
	StoneType missile_type_code = StoneType(mySGD->getUserdataSelectedCharNO()-1);
	missile_type_code = kStoneType_guided; // 임시
	
	int missile_level = mySGD->getSelectedCharacterHistory().level.getV();
	
	if(missile_type_code == kStoneType_guided)
	{
		int grade = (missile_level-1)/5+1;
		bool rotation = false;
		if(grade == 1 || grade == 4)
			rotation = true;
		GuidedMissile* t_gm = GuidedMissile::createForShowStartSettingPopup(character_img, CCString::createWithFormat("jack_missile_%02d_%02d.png", mySGD->getUserdataSelectedCharNO(), missile_level)->getCString(),
																			rotation, (missile_level-1)/5+1, (missile_level-1)%5+1);
		//		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("me_guide%d.ccbi", (missile_level-1)%5 + 1)->getCString());
		t_gm->setFunctionForCrash([=](){
			cumber_node->stopAllActions();
			cumber_node->runAction(CCSequence::create(CCScaleBy::create(0.06f,0.9),CCScaleTo::create(0.1,1), NULL));
		});
		t_gm->setPosition(keep_position);
		main_case->addChild(t_gm);
		
		
		
		t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
		
		missile_img = t_gm;
	}
	
	main_case->setScaleY(0.f);
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){
		main_case->setScaleY(t);
	}, [=](float t){ // finish
		main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){
			main_case->setScaleY(t);
		}, [=](float t){ // finish
			main_case->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){
				main_case->setScaleY(t);
			}, [=](float t){ // finish
				main_case->setScaleY(1.f);
				is_menu_enable = true;
			}));}));}));
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
																		   
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   
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
//																		   if(buy_button)
//																			   buy_button->removeFromParent();
																		   
																		   CCRect title_size = CCRectMake(0, 0, 200, 20);
																		   CCPoint title_position = ccp(188, 133);
																		   
																		   item_title_label = CCLabelTTF::create(convertToItemCodeToItemName(kIC_itemGacha).c_str(), mySGD->getFont().c_str(), 15, title_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
																			 setFormSetter(item_title_label);
																		   item_title_label->setColor(ccc3(255, 170, 20));
																		   item_title_label->setAnchorPoint(ccp(0,1));
																		   item_title_label->setPosition(title_position);
																		   main_case->addChild(item_title_label);
																		   
																		   CCRect option_size = CCRectMake(0, 0, 160, 25);
																		   CCPoint option_position = ccp(188, 115);
																		   
																		   option_label = CCLabelTTF::create(mySD->getItemScript(kIC_itemGacha).c_str(), mySGD->getFont().c_str(), 10.f, option_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
																			 setFormSetter(option_label);
																		   option_label->setAnchorPoint(ccp(0,1));
																		   option_label->setPosition(option_position);
																		   main_case->addChild(option_label);
																		   
																		   if(selected_gacha_item > kIC_emptyBegin && selected_gacha_item < kIC_emptyEnd)
																			{
																				item_title_label->setString(convertToItemCodeToItemName(selected_gacha_item).c_str());
																				option_label->setString(mySD->getItemScript(selected_gacha_item).c_str());
																			}
																		   else
																			{
//																				CCPoint keep_position = gacha_item->getPosition();
																				gacha_item->removeFromParent();
																				
																				gacha_item = KS::loadCCBI<CCSprite*>(this, "randomitem.ccbi").first;//CCSprite::create("startsetting_item_gacha_inner.png");
																				gacha_item->setPosition(ccp(410,185));
																				main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
																				
																				KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gacha), mySGD->getFont().c_str(), 12.5f);
																				gacha_label->enableOuterStroke(ccBLACK, 1.f);
																				gacha_label->setPosition(ccp(gacha_item->getContentSize().width/2.f, gacha_item->getContentSize().height/2.f - 15.f));
																				gacha_item->addChild(gacha_label);
																			}
																		   
																		   if(!buy_button)
																			{
																				buy_button = CommonButton::create("", 15, CCSizeMake(100, 55), CCScale9Sprite::create("startsetting_item_buy.png", CCRectMake(0, 0, 100, 55), CCRectMake(49, 27, 2, 1)), touch_priority);
																				buy_button->setPrice(PriceTypeGold, 0);
																				setFormSetter(buy_button);
																				buy_button->setPosition(ccp(402,114));
																				main_case->addChild(buy_button);
																			}
																		   
																		   if(selected_gacha_item > kIC_emptyBegin && selected_gacha_item < kIC_emptyEnd)
																			{
																				if(mySGD->getGoodsValue(kGoodsType_pass4) > 0)
																					buy_button->setPrice(PriceTypePass4, 0);
																				else
																					buy_button->setPrice(PriceTypeGold, mySGD->getItemGachaReplayGoldFee());
//																				buy_button->setTitle(myLoc->getLocalForKey(LK::kMyLocalKey_itemRegacha));
																			}
																		   else
																			{
																				if(mySGD->getGoodsValue(kGoodsType_pass4) > 0)
																					buy_button->setPrice(PriceTypePass4, 0);
																				else
																					buy_button->setPrice(PriceTypeGold, mySGD->getItemGachaGoldFee());
//																				buy_button->setTitle(myLoc->getLocalForKey(LK::kMyLocalKey_buy));
																			}
																		   buy_button->setFunction([=](CCObject* sender)
																								   {
																									   this->startItemGacha();
																								   });
																		   
																		   gacha_clicked_img->setVisible(true);
																		   
																		   is_menu_enable = true;
																	   });
	
	item_gacha_menu = CCMenuLambda::createWithItem(gacha_item_item);
	item_gacha_menu->setPosition(ccp(410,185));
	setFormSetter(item_gacha_menu);
	main_case->addChild(item_gacha_menu);
	
	item_gacha_menu->setTouchPriority(touch_priority);
}

void StartSettingPopup::upgradeAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	if(mySGD->is_hell_mode_enabled)
	{
		MissileUpgradePopup* t_popup = MissileUpgradePopup::create(touch_priority-100, [=](){popupClose();}, [=](){
			int missile_level = mySGD->getSelectedCharacterHistory().level.getV();
			
			missile_data_level->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_levelValue), missile_level)->getCString());
			missile_data_power->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_powerValue), KS::insert_separator(mySGD->getSelectedCharacterHistory().power.getV()).c_str())->getCString());
			
			CCPoint keep_position = missile_img->getPosition();
			missile_img->removeFromParent();
			
			StoneType missile_type_code = StoneType(mySGD->getUserdataSelectedCharNO()-1);
			missile_type_code = kStoneType_guided; // 임시
			
			if(missile_type_code == kStoneType_guided)
			{
				int grade = (missile_level-1)/5+1;
				bool rotation = false;
				if(grade == 1 || grade == 4)
					rotation = true;
				GuidedMissile* t_gm = GuidedMissile::createForShowStartSettingPopup(character_img, CCString::createWithFormat("jack_missile_%02d_%02d.png", mySGD->getUserdataSelectedCharNO(), missile_level)->getCString(),
																					rotation, (missile_level-1)/5+1, (missile_level-1)%5+1);
				//		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("me_guide%d.ccbi", (missile_level-1)%5 + 1)->getCString());
				t_gm->setFunctionForCrash([=](){
					cumber_node->stopAllActions();
					cumber_node->runAction(CCSequence::create(CCScaleBy::create(0.06f,0.9),CCScaleTo::create(0.1,1), NULL));
				});
				t_gm->setPosition(keep_position);
				main_case->addChild(t_gm);
				
				
				
				t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
				
				missile_img = t_gm;
			}
			
			
			
			CCPoint upgrade_position = upgrade_menu->getPosition();
			upgrade_menu->removeFromParent();
			
			if(mySGD->getSelectedCharacterHistory().isMaxLevel.getV())
			{
				CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
				KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missileUpgrade2), mySGD->getFont().c_str(), 12);
				n_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
				n_label->setPosition(ccpFromSize(n_upgrade->getContentSize()/2.f));
				n_upgrade->addChild(n_label);
				
				CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
				s_upgrade->setColor(ccGRAY);
				KSLabelTTF* s_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missileUpgrade2), mySGD->getFont().c_str(), 12);
				s_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
				s_label->setPosition(ccpFromSize(s_upgrade->getContentSize()/2.f));
				s_upgrade->addChild(s_label);
				
				
				CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
				setFormSetter(upgrade_item);
				upgrade_menu = CCMenu::createWithItem(upgrade_item);
				upgrade_menu->setPosition(upgrade_position);
				main_case->addChild(upgrade_menu);
				
				upgrade_menu->setTouchPriority(touch_priority);
				upgrade_menu->setEnabled(false);
			}
			else
			{
				CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
				KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missileUpgrade2), mySGD->getFont().c_str(), 12);
				n_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
				n_label->setPosition(ccpFromSize(n_upgrade->getContentSize()/2.f));
				n_upgrade->addChild(n_label);
				
				CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
				s_upgrade->setColor(ccGRAY);
				KSLabelTTF* s_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missileUpgrade2), mySGD->getFont().c_str(), 12);
				s_label->enableOuterStroke(ccBLACK, 1, int(255*0.5f), true);
				s_label->setPosition(ccpFromSize(s_upgrade->getContentSize()/2.f));
				s_upgrade->addChild(s_label);
				
				
				CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
				
				upgrade_menu = CCMenu::createWithItem(upgrade_item);
				upgrade_menu->setPosition(upgrade_position);
				main_case->addChild(upgrade_menu);
				
				upgrade_menu->setTouchPriority(touch_priority);
			}
		});
		addChild(t_popup, kStartSettingPopupZorder_popup);
	}
	else
	{
		MissileUpgradePopup* t_popup = MissileUpgradePopup::create(touch_priority-100, [=](){popupClose();}, [=](){
			int missile_level = mySGD->getSelectedCharacterHistory().level.getV();
			
			missile_data_level->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_levelValue), missile_level)->getCString());
			missile_data_power->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_powerValue), KS::insert_separator(mySGD->getSelectedCharacterHistory().power.getV()).c_str())->getCString());
			
			CCPoint missile_position;
			if(missile_img)
			{
				missile_position = missile_img->getPosition();
				missile_img->removeFromParent();
				missile_img = NULL;
			}
			
			StoneType missile_type_code = StoneType(mySGD->getSelectedCharacterHistory().characterNo.getV()-1);
			missile_type_code = kStoneType_guided;
			
			if(missile_type_code == kStoneType_guided)
			{
				int grade = (missile_level-1)/5+1;
				bool rotation = false;
				if(grade == 1 || grade == 4)
					rotation = true;
				GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("jack_missile_%d.png", missile_level)->getCString(),
																		 rotation, (missile_level-1)/5+1, (missile_level-1)%5+1);
				//			GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("me_guide%d.ccbi", (missile_level-1)%5 + 1)->getCString());
				
				t_gm->setFunctionForCrash([=](){
					cumber_node->stopAllActions();
					cumber_node->runAction(CCSequence::create(CCScaleBy::create(0.06f,0.9),CCScaleTo::create(0.1,1), NULL));
				});
				t_gm->setPosition(missile_position);
				//			t_gm->beautifier((missile_level-1)/5+1, (missile_level-1)%5+1);
				main_case->addChild(t_gm);
				
				
				
				t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
				
				missile_img = t_gm;
			}
			
			
			
			CCPoint upgrade_position = upgrade_menu->getPosition();
			upgrade_menu->removeFromParent();
			
			if(mySGD->getSelectedCharacterHistory().isMaxLevel.getV())
			{
				CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
				KSLabelTTF* n_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_maxLevel), missile_level)->getCString(), mySGD->getFont().c_str(), 14);
				n_level->setPosition(ccp(70,46));
				n_upgrade->addChild(n_level);
				
				CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
				s_upgrade->setColor(ccGRAY);
				KSLabelTTF* s_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_maxLevel), missile_level)->getCString(), mySGD->getFont().c_str(), 14);
				s_level->setPosition(ccp(70,46));
				s_upgrade->addChild(s_level);
				
				
				KSLabelTTF* n_price_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endUpgrade), mySGD->getFont().c_str(), 17.5f);
				n_price_label->setPosition(ccp(70,20));
				n_upgrade->addChild(n_price_label);
				
				KSLabelTTF* s_price_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endUpgrade), mySGD->getFont().c_str(), 17.5f);
				s_price_label->setPosition(ccp(70,20));
				s_upgrade->addChild(s_price_label);
				
				
				CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
				
				upgrade_menu = CCMenu::createWithItem(upgrade_item);
				upgrade_menu->setPosition(upgrade_position);
				main_case->addChild(upgrade_menu);
				
				upgrade_menu->setTouchPriority(touch_priority);
				upgrade_menu->setEnabled(false);
			}
			else
			{
				CCSprite* n_upgrade = CCSprite::create("startsetting_upgrade.png");
				KSLabelTTF* n_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_upgradeLevelValue), missile_level+1)->getCString(), mySGD->getFont().c_str(), 14);
				n_level->setPosition(ccp(70,46));
				n_upgrade->addChild(n_level);
				
				CCSprite* s_upgrade = CCSprite::create("startsetting_upgrade.png");
				s_upgrade->setColor(ccGRAY);
				KSLabelTTF* s_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_upgradeLevelValue), missile_level+1)->getCString(), mySGD->getFont().c_str(), 14);
				s_level->setPosition(ccp(70,46));
				s_upgrade->addChild(s_level);
				
				if(mySGD->getGoodsValue(kGoodsType_pass3) > 0)
				{
					CCSprite* n_pass_ticket = CCSprite::create("pass_ticket3.png");
					n_pass_ticket->setPosition(ccp(28,21.f));
					n_upgrade->addChild(n_pass_ticket);
					KSLabelTTF* n_free_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 17.5f);
					n_free_label->setPosition(ccp(78,20));
					n_upgrade->addChild(n_free_label);
					
					{
						CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
						pass_count_case->setPosition(ccpFromSize(n_pass_ticket->getContentSize()/2.f) + ccp(9,6));
						n_pass_ticket->addChild(pass_count_case);
						
						CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass3)), mySGD->getFont().c_str(), 8);
						pass_count_label->setColor(ccc3(255, 255, 255));
						float min_width = 10+pass_count_label->getContentSize().width;
						if(min_width < 20)
							min_width = 20;
						pass_count_case->setContentSize(CCSizeMake(min_width, 20));
						pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
						pass_count_case->addChild(pass_count_label);
					}
					
					CCSprite* s_pass_ticket = CCSprite::create("pass_ticket3.png");
					s_pass_ticket->setPosition(ccp(28,21.f));
					s_upgrade->addChild(s_pass_ticket);
					KSLabelTTF* s_free_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 17.5f);
					s_free_label->setPosition(ccp(78,20));
					s_upgrade->addChild(s_free_label);
					
					{
						CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
						pass_count_case->setPosition(ccpFromSize(s_pass_ticket->getContentSize()/2.f) + ccp(9,6));
						s_pass_ticket->addChild(pass_count_case);
						
						CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass3)), mySGD->getFont().c_str(), 8);
						pass_count_label->setColor(ccc3(255, 255, 255));
						float min_width = 10+pass_count_label->getContentSize().width;
						if(min_width < 20)
							min_width = 20;
						pass_count_case->setContentSize(CCSizeMake(min_width, 20));
						pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
						pass_count_case->addChild(pass_count_label);
					}
				}
				else
				{
					CCSprite* n_price_type = CCSprite::create("price_gold_img.png");
					n_price_type->setPosition(ccp(25,20));
					n_upgrade->addChild(n_price_type);
					KSLabelTTF* n_price_label = KSLabelTTF::create(KS::insert_separator(
																						ccsf("%d", mySGD->getSelectedCharacterHistory().nextPrice.getV())).c_str(), mySGD->getFont().c_str(), 17.5f);
					n_price_label->setPosition(ccp(78,20));
					n_upgrade->addChild(n_price_label);
					
					CCSprite* s_price_type = CCSprite::create("price_gold_img.png");
					s_price_type->setPosition(ccp(25,20));
					s_upgrade->addChild(s_price_type);
					KSLabelTTF* s_price_label = KSLabelTTF::create(KS::insert_separator(
																						ccsf("%d", mySGD->getSelectedCharacterHistory().nextPrice.getV())).c_str(), mySGD->getFont().c_str(), 17.5f);
					s_price_label->setPosition(ccp(78,20));
					s_upgrade->addChild(s_price_label);
				}
				
				CCMenuItem* upgrade_item = CCMenuItemSprite::create(n_upgrade, s_upgrade, this, menu_selector(StartSettingPopup::upgradeAction));
				
				upgrade_menu = CCMenu::createWithItem(upgrade_item);
				upgrade_menu->setPosition(upgrade_position);
				main_case->addChild(upgrade_menu);
				upgrade_menu->setTouchPriority(touch_priority);
			}
		});
		addChild(t_popup, kStartSettingPopupZorder_popup);
	}
	
}

void StartSettingPopup::startItemGacha()
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int random_value;
	
	if(!myDSH->getBoolForKey(kDSH_Key_isNotFirstItemGacha))
	{
		is_first_gacha = true;
		myDSH->setBoolForKey(kDSH_Key_isNotFirstItemGacha, true);
		random_value = 999;
	}
	else
	{
		random_value = rand()%1000;
	}
	
	if(random_value < 200) // 200
		gacha_item_type = kIC_fast; // 4
	else if(random_value < 400) // 200
		gacha_item_type = kIC_subOneDie; // 5
	else if(random_value < 650) // 250
		gacha_item_type = kIC_silence; // 7
	else if(random_value < 900) // 250
		gacha_item_type = kIC_longTime; // 8
	else // 100
		gacha_item_type = kIC_heartUp; // 10
	
	if(mySGD->getGoodsValue(kGoodsType_pass4) > 0)
	{
		CCLOG("start item gacha");
		
		if(is_first_gacha)
			mySGD->addChangeGoods("g_i_p", kGoodsType_begin, 0, "", ccsf("%d", -1), "아이템가챠(패스권)");
		else
			mySGD->addChangeGoods("g_i_p", kGoodsType_begin, 0, "", ccsf("%d", gacha_item_type), "아이템가챠(패스권)");
		
		mySGD->changeGoods(json_selector(this, StartSettingPopup::goItemGacha));
	}
	else
	{
		if(selected_gacha_item > kIC_emptyBegin && selected_gacha_item < kIC_emptyEnd)
		{
			if(mySGD->getItemGachaReplayGoldFee() > mySGD->getGoodsValue(kGoodsType_gold))
			{
				addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-500, kGoodsType_gold, [=]()
																	{
																		ShopPopup* t_shop = ShopPopup::create();
																		t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
																		if(mySGD->is_endless_mode)
																			t_shop->targetHeartTime(((MainFlowScene*)getParent())->heart_time);
																		else
																			t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
																		t_shop->setShopCode(kSC_gold);
																		t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
																		addChild(t_shop, kStartSettingPopupZorder_popup);
																	}, [=](){is_menu_enable = true;}), 9999);
				return;
			}
			
			if(is_first_gacha)
				mySGD->addChangeGoods("g_i_gr", kGoodsType_begin, 0, "", ccsf("%d", -1), "아이템가챠(다시뽑기)");
			else
				mySGD->addChangeGoods("g_i_gr", kGoodsType_begin, 0, "", ccsf("%d", gacha_item_type), "아이템가챠(다시뽑기)");
		}
		else
		{
			if(mySGD->getItemGachaGoldFee() > mySGD->getGoodsValue(kGoodsType_gold))
			{
				addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-500, kGoodsType_gold, [=]()
																	{
																		ShopPopup* t_shop = ShopPopup::create();
																		t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
																		if(mySGD->is_endless_mode)
																			t_shop->targetHeartTime(((MainFlowScene*)getParent())->heart_time);
																		else
																			t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
																		t_shop->setShopCode(kSC_gold);
																		t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
																		addChild(t_shop, kStartSettingPopupZorder_popup);
																	}, [=](){is_menu_enable = true;}), 9999);
				return;
			}
			
			if(is_first_gacha)
				mySGD->addChangeGoods("g_i_g", kGoodsType_begin, 0, "", ccsf("%d", -1), "아이템가챠");
			else
				mySGD->addChangeGoods("g_i_g", kGoodsType_begin, 0, "", ccsf("%d", gacha_item_type), "아이템가챠");
		}
		CCLOG("start item gacha");
		
		mySGD->changeGoods(json_selector(this, StartSettingPopup::goItemGacha));
	}
	
	
	
	
	
//	if(selected_gacha_item > kIC_emptyBegin && selected_gacha_item < kIC_emptyEnd)
//		selected_gacha_item = kIC_emptyBegin;
//	
//	gacha_item_frame = 0;
//	schedule(schedule_selector(StartSettingPopup::itemGachaAction), 1.f/20.f);
}

void StartSettingPopup::goItemGacha(Json::Value result_data)
{
	CCLOG("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLOG("save userdata success!!");
		
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
		if(mySGD->getGoodsValue(kGoodsType_pass4) > 0)
			buy_button->setPrice(PriceTypePass4, 0);
		else
			buy_button->setPrice(PriceTypeGold, mySGD->getItemGachaReplayGoldFee());
		
		ItemGachaPopup* t_popup = ItemGachaPopup::create(touch_priority-100, [=]()
		{
			if(mySGD->getGoodsValue(kGoodsType_pass4) > 0)
				buy_button->setPrice(PriceTypePass4, 0);
			else
				buy_button->setPrice(PriceTypeGold, mySGD->getItemGachaReplayGoldFee());
			endItemGacha();
		}, [=](int item_type){
			
			selected_gacha_item = (ITEM_CODE)item_type;
			mySGD->gacha_item = selected_gacha_item;
			
			item_title_label->setString(convertToItemCodeToItemName(selected_gacha_item).c_str());
			option_label->setString(mySD->getItemScript(selected_gacha_item).c_str());
			
			CCPoint gacha_item_position = gacha_item->getPosition();
			gacha_item->removeFromParent();
			
			gacha_item = CCSprite::create(CCString::createWithFormat("item%d.png", selected_gacha_item)->getCString());
			gacha_item->setScale(0.7f);
			gacha_item->setPosition(gacha_item_position);
			main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
			
			CCSprite* mount_img = CCSprite::create("startsetting_item_mounted_case.png");
			mount_img->setPosition(ccpFromSize(gacha_item->getContentSize()/2.f));
			mount_img->setScale(1.f/gacha_item->getScale());
			gacha_item->addChild(mount_img, 1);
			
			if(mySGD->getGoodsValue(kGoodsType_pass4) > 0)
				buy_button->setPrice(PriceTypePass4, 0);
			else
				buy_button->setPrice(PriceTypeGold, mySGD->getItemGachaReplayGoldFee());
//			buy_button->setTitle(myLoc->getLocalForKey(LK::kMyLocalKey_itemRegacha));
		}, gacha_item_type);
		addChild(t_popup, kStartSettingPopupZorder_popup);
	}
	else
	{
		if(is_first_gacha)
		{
			myDSH->setBoolForKey(kDSH_Key_isNotFirstItemGacha, false);
		}
		
		CCLOG("save userdata fail!!!");
		mySGD->clearChangeGoods();
		
		addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
		
		is_menu_enable = true;
	}
	
	is_first_gacha = false;
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
	gacha_item->setScale(0.7f);
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
	gacha_item->setScale(0.7f);
	gacha_item->setPosition(gacha_item_position);
	main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
	
	CCSprite* mount_img = CCSprite::create("startsetting_item_mounted_case.png");
	mount_img->setPosition(ccpFromSize(gacha_item->getContentSize()/2.f));
	mount_img->setScale(1.f/gacha_item->getScale());
	gacha_item->addChild(mount_img, 1);
	
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
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		
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
			
			if(selected_gacha_item > kIC_emptyBegin && selected_gacha_item < kIC_emptyEnd)
			{
				
			}
			else
			{
				CCPoint keep_position = gacha_item->getPosition();
				gacha_item->removeFromParent();
				
                auto t_ccb = KS::loadCCBI<CCSprite*>(this, "startsetting_question2.ccbi");
				gacha_item = t_ccb.first;//CCSprite::create("startsetting_item_gacha_inner.png");
				gacha_item->setPosition(keep_position);
				main_case->addChild(gacha_item, kStartSettingPopupZorder_main);
                t_ccb.second->runAnimationsForSequenceNamed("Default Timeline");
				
				KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gacha), mySGD->getFont().c_str(), 12.5f);
				gacha_label->enableOuterStroke(ccBLACK, 1.f);
				gacha_label->setPosition(ccp(gacha_item->getContentSize().width/2.f, gacha_item->getContentSize().height/2.f - 23.f));
				gacha_item->addChild(gacha_label);
			}
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
			n_img->setScale(0.7f);
			n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
			n_item_case->addChild(n_img);
			
			CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
			s_item_case->setColor(ccGRAY);
			CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			s_img->setColor(ccGRAY);
			s_img->setScale(0.7f);
			s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
			s_item_case->addChild(s_img);
			
			CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingPopup::itemAction));
			item_item->setTag(tag);
			
			CCMenu* item_menu = CCMenu::createWithItem(item_item);
			item_menu->setPosition(CCPointZero);
			item_parent->addChild(item_menu);
			item_menu->setTouchPriority(touch_priority);
			
			
			int item_cnt = mySGD->getGoodsValue(mySGD->getItemCodeToGoodsType(t_ic));

			CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 11);
			cnt_label->setPosition(ccp(16.5f, -18));
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
			is_price_usable = is_price_usable || (mySGD->getGoodsValue(mySGD->getItemCodeToGoodsType(t_ic)) > 0); // 소지하고 있는지
			
			if(getSelectedItemCount() < 3 && is_price_usable)
			{
				mySGD->setBeforeUseItem(t_ic, true);
				
				// mount
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setScale(0.7f);
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
				n_item_case->addChild(n_img);
				
				CCSprite* n_mount = CCSprite::create("startsetting_item_mounted_case.png");
				n_mount->setPosition(ccpFromSize(n_img->getContentSize()/2.f));
				n_mount->setScale(1.f/n_img->getScale());
				n_img->addChild(n_mount, 1);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setColor(ccGRAY);
				s_img->setScale(0.7f);
				s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
				s_item_case->addChild(s_img);
				
				CCSprite* s_mount = CCSprite::create("startsetting_item_mounted_case.png");
				s_mount->setPosition(ccpFromSize(s_img->getContentSize()/2.f));
				s_mount->setScale(1.f/s_img->getScale());
				s_img->addChild(s_mount, 1);
				
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
					addChild(ASPopupView::getCommonNoti(-210, myLoc->getLocalForKey(LK::kMyLocalKey_totalItemSelectCount3)), kStartSettingPopupZorder_popup);
				
				// normal
				CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
				CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				n_img->setScale(0.7f);
				n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
				n_item_case->addChild(n_img);
				
				CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
				s_item_case->setColor(ccGRAY);
				CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
				s_img->setColor(ccGRAY);
				s_img->setScale(0.7f);
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
			
			int item_cnt = mySGD->getGoodsValue(mySGD->getItemCodeToGoodsType(t_ic));

			CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 11);
			cnt_label->setPosition(ccp(16.5f, -18));
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
//		if(buy_button)
//			buy_button->removeFromParent();
		
		CCRect title_size = CCRectMake(0, 0, 200, 20);
		CCPoint title_position = ccp(188, 133);
		
		//		CCSprite* title_rect = CCSprite::create("whitePaper.png", title_size);
		//		title_rect->setOpacity(100);
		//		title_rect->setAnchorPoint(ccp(0,1));
		//		title_rect->setPosition(title_position);
		//		main_case->addChild(title_rect);
		
		item_title_label = CCLabelTTF::create(convertToItemCodeToItemName(item_list[tag-1]).c_str(), mySGD->getFont().c_str(), 15, title_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
		setFormSetter(item_title_label);
		item_title_label->setColor(ccc3(255, 170, 20));
		item_title_label->setAnchorPoint(ccp(0,1));
		item_title_label->setPosition(title_position);
		main_case->addChild(item_title_label);
		
		CCRect option_size = CCRectMake(0, 0, 160, 25);
		CCPoint option_position = ccp(188, 115);
		
//		CCSprite* option_rect = CCSprite::create("whitePaper.png", option_size);
//		option_rect->setOpacity(100);
//		option_rect->setAnchorPoint(ccp(0,1));
//		option_rect->setPosition(option_position);
//		main_case->addChild(option_rect);
		
		option_label = CCLabelTTF::create(mySD->getItemScript(item_list[tag-1]).c_str(), mySGD->getFont().c_str(), 10.f, option_size.size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		setFormSetter(option_label);
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
		
		if(!buy_button)
		{
			buy_button = CommonButton::create("", 15, CCSizeMake(100, 55), CCScale9Sprite::create("startsetting_item_buy.png", CCRectMake(0, 0, 100, 55), CCRectMake(49, 27, 2, 1)), touch_priority);
			setFormSetter(buy_button);
			buy_button->setPrice(PriceTypeGold, 0);
			buy_button->setPosition(ccp(402,114));
			main_case->addChild(buy_button);
			
		}
		
		buy_button->setPrice(priceType, mySD->getItemPrice(item_list[tag-1]));
//		buy_button->setTitle(myLoc->getLocalForKey(LK::kMyLocalKey_buy));
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
											AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
											
											LoadingLayer* t_loading = LoadingLayer::create(-9999);
											addChild(t_loading, 9999);
											
											mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopItem_int1_exchangeID_s, item_list[clicked_item_idx]), kGoodsType_begin, 0, "", ccsf("%d", mySD->getSilType()), "아이템구입");
											
											mySGD->changeGoods([=](Json::Value result_data){
												t_loading->removeFromParent();
												if(result_data["result"]["code"] == GDSUCCESS)
												{
													buySuccessItem(clicked_item_idx, 1);
												}
												else if(result_data["result"]["code"] == GDPROPERTYISMINUS)
												{
													mySGD->clearChangeGoods();
													addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-200, kGoodsType_gold, [=]()
																										{
																											ShopPopup* t_shop = ShopPopup::create();
																											t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
																											if(mySGD->is_endless_mode)
																												t_shop->targetHeartTime(((MainFlowScene*)getParent())->heart_time);
																											else
																												t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
																											t_shop->setShopCode(kSC_gold);
																											t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
																											addChild(t_shop, kStartSettingPopupZorder_popup);
																										}, [=](){is_menu_enable = true;}), 9999);
												}
												else
												{
													mySGD->clearChangeGoods();
													addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
													is_menu_enable = true;
												}
											});
//										}
//										else
//										{
//											addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-500, kGoodsType_gold, [=]()
//																								{
//																									ShopPopup* t_shop = ShopPopup::create();
//																									t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
//																									if(mySGD->is_endless_mode)
//																										t_shop->targetHeartTime(((MainFlowScene*)getParent())->heart_time);
//																									else
//																										t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
//																									t_shop->setShopCode(kSC_gold);
//																									t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
//																									addChild(t_shop, kStartSettingPopupZorder_popup);
//																								}, [=](){is_menu_enable = true;}), 9999);
//											
//											is_menu_enable = true;
//										}
									}
									else if(item_currency == "ruby")
									{
//										if(mySD->getItemPrice(item_list[clicked_item_idx]) <= mySGD->getGoodsValue(kGoodsType_ruby))
//										{
											AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
											
											LoadingLayer* t_loading = LoadingLayer::create(-9999);
											addChild(t_loading, 9999);
											
											mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopItem_int1_exchangeID_s, item_list[clicked_item_idx]), kGoodsType_begin, 0, "", ccsf("%d", mySD->getSilType()), "아이템구입");
											
											mySGD->changeGoods([=](Json::Value result_data){
												t_loading->removeFromParent();
												if(result_data["result"]["code"] == GDSUCCESS)
												{
													buySuccessItem(clicked_item_idx, 1);
												}
												else if(result_data["result"]["code"] == GDPROPERTYISMINUS)
												{
													mySGD->clearChangeGoods();
													addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought), [=](){is_menu_enable = true;}), 9999);
//													addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-200, kGoodsType_ruby, [=]()
//																										{
//																											ShopPopup* t_shop = ShopPopup::create();
//																											t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
//																											if(mySGD->is_endless_mode)
//																												t_shop->targetHeartTime(((MainFlowScene*)getParent())->heart_time);
//																											else
//																												t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
//																											t_shop->setShopCode(kSC_ruby);
//																											t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
//																											addChild(t_shop, kStartSettingPopupZorder_popup);
//																										}, [=](){is_menu_enable = true;}), 9999);
												}
												else
												{
													mySGD->clearChangeGoods();
													addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
													is_menu_enable = true;
												}
											});
//										}
//										else
//										{
//											addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-500, kGoodsType_ruby, [=]()
//																								{
//																									ShopPopup* t_shop = ShopPopup::create();
//																									t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
//																									if(mySGD->is_endless_mode)
//																										t_shop->targetHeartTime(((MainFlowScene*)getParent())->heart_time);
//																									else
//																										t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
//																									t_shop->setShopCode(kSC_ruby);
//																									t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
//																									addChild(t_shop, kStartSettingPopupZorder_popup);
//																								}, [=](){is_menu_enable = true;}), 9999);
//											
//											is_menu_enable = true;
//										}
									}
									
								});
		
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
		
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		
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

void StartSettingPopup::startButtonAction(CCObject *sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	callStart();
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
											   if(is_go_to_mainflow)
												   goToMainFlow_func();
											   removeFromParent();
										   }));
	
	CommonAnimation::closePopup(this, main_case, nullptr, [=](){
		
	}, [=](){
//		end_func(); removeFromParent();
	});
}

void StartSettingPopup::callStart()
{
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	bool is_open = mySGD->getPuzzleHistory(puzzle_number).is_open.getV();
	is_open = is_open | mySGD->is_endless_mode;
	
	is_go_to_mainflow = false;
	
	if(!is_open)
	{
		string puzzle_condition = NSDS_GS(puzzle_number, kSDS_PZ_condition_s);
		
		Json::Value condition_list;
		Json::Reader reader;
		reader.parse(puzzle_condition, condition_list);
		
		if(condition_list.size() <= 0)
			is_open = true;
		
		bool is_base_condition_success = true;
		
		for(int i=0;!is_open && i<condition_list.size();i++)
		{
			Json::Value t_condition_and = condition_list[i];
			
			bool and_open = true;
			//				bool is_time_condition = false;
			
			for(int j=0;and_open && j<t_condition_and.size();j++)
			{
				Json::Value t_condition = t_condition_and[j];
				string t_type = t_condition["type"].asString();
				if(t_type == "p")
				{
					if(!mySGD->getPuzzleHistory(t_condition["value"].asInt()).is_clear.getV())
					{
						and_open = false;
						is_base_condition_success = false;
					}
				}
				else if(t_type == "s")
				{
					if(mySGD->getClearStarCount() < t_condition["value"].asInt())
					{
						and_open = false;
						is_base_condition_success = false;
					}
				}
				else if(t_type == "r")
				{
					and_open = false;
				}
				else if(t_type == "w")
				{
					//						is_time_condition = true;
					if(!mySGD->keep_time_info.is_loaded)
						and_open = false;
					else
					{
						int weekday = t_condition["weekday"].asInt();
						if(mySGD->keep_time_info.weekday.getV() != -1 && mySGD->keep_time_info.weekday.getV() != weekday)
							and_open = false;
						if(mySGD->keep_time_info.hour.getV() < t_condition["s"].asInt() || mySGD->keep_time_info.hour.getV() >= t_condition["e"].asInt())
							and_open = false;
					}
				}
				else if(t_type == "d")
				{
					//						is_time_condition = true;
					if(mySGD->keep_time_info.date.getV() < t_condition["s"].asInt64() || mySGD->keep_time_info.date.getV() >= t_condition["e"].asInt64())
						and_open = false;
				}
			}
			
			if(and_open)
			{
				is_open = true;
				//					if(!is_time_condition)
				//					{
				//						PuzzleHistory t_history = mySGD->getPuzzleHistory(puzzle_number);
				//						t_history.is_open = true;
				//						t_history.open_type = "무료";
				//						mySGD->setPuzzleHistory(t_history, nullptr);
				//					}
			}
		}
		
		if(!is_open)
		{
			if(mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
				tutorial_fail_func();
			is_go_to_mainflow = true;
			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_timeOutFrame)), 9999);
			closeAction();
			
			return;
		}
	}
	
//	bool is_startable = false;
//	if(mySGD->is_endless_mode)
//	{
//		if(mySGD->endless_my_victory.getV() > 0)
//			is_startable = true;
//		else
//			is_startable = ((MainFlowScene*)getParent())->heart_time->isStartable();
//	}
//	else
//	{
//		is_startable = ((PuzzleScene*)getParent())->heart_time->isStartable();
//	}
//	
//	if(mySGD->isTimeEvent(kTimeEventType_heart))
//	{
//		is_startable = true;
//	}
//	
//	if(is_startable)
//	{
		bool is_startGame = false;
		
		if(mySGD->isTimeEvent(kTimeEventType_heart))
			is_startGame = true;
		
		if(mySGD->is_endless_mode)
		{
			if(mySGD->endless_my_victory.getV() > 0 || myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
				is_startGame = true;
		}
		
//		if(!is_startGame)
//		{
//			if(mySGD->is_endless_mode)
//				is_startGame = ((MainFlowScene*)getParent())->heart_time->startGame();
//			else
//				is_startGame = ((PuzzleScene*)getParent())->heart_time->startGame();
//		}
		
//		if(is_startGame)
//		{
			realStartAction(!is_startGame);
//		}
//		else
//		{
//			if(mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
//				tutorial_fail_func();
//			is_menu_enable = true;
//		}
//	}
//	else
//	{
//		addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-500, kGoodsType_money, [=]()
//															{
//																ShopPopup* t_shop = ShopPopup::create();
//																t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
//																if(mySGD->is_endless_mode)
//																	t_shop->targetHeartTime(((MainFlowScene*)getParent())->heart_time);
//																else
//																	t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
//																t_shop->setShopCode(kSC_heart);
//																t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
//																addChild(t_shop, kStartSettingPopupZorder_popup);
//															}, [=]()
//		{
//			if(mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
//				tutorial_fail_func();
//			is_menu_enable = true;
//		}), 9999);
//	}
}

void StartSettingPopup::realStartAction(bool is_use_heart)
{
	finalSetting();
	
	vector<CommandParam> t_command_list;
	t_command_list.clear();
//	t_command_list.push_back(CommandParam("updateUserData", myDSH->getSaveAllUserDataParam(), nullptr));
	
	if(!mySGD->is_endless_mode && !mySGD->is_hell_mode && mySD->getSilType() > mySGD->getUserdataHighPiece())
	{
		mySGD->setUserdataHighPiece(mySD->getSilType());
	}
	
	mySGD->setUserdataTotalPlayCount(mySGD->getUserdataTotalPlayCount()+1);
	t_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
	t_history.try_count = t_history.try_count.getV()+1;
	
	mySGD->setPieceHistoryForNotSave(t_history);
	
	t_command_list.push_back(CommandParam("updatePieceHistory", mySGD->getSavePieceHistoryParam(t_history), nullptr));
	
	start_loading = LoadingLayer::create();
	addChild(start_loading, kStartSettingPopupZorder_popup);
	
	Json::Value heart_param;
	heart_param["memberID"] = myHSP->getMemberID();
	if(is_use_heart)
		heart_param["use"] = is_use_heart;
	t_command_list.push_back(CommandParam("getheart", heart_param, [=](Json::Value result_data)
										  {
											  if(result_data["result"]["code"].asInt() == GDSUCCESS)
											  {
												  mySGD->heartRefreshSuccess(result_data);
												  if(mySGD->is_endless_mode || mySGD->is_hell_mode)
												  {
													  ((MainFlowScene*)getParent())->heart_time->refreshHeartTime();
												  }
												  else
												  {
													  ((PuzzleScene*)getParent())->heart_time->refreshHeartTime();
												  }
											  }
											  else if(result_data["isMinus"].asBool())
											  {
												  mySGD->heartRefreshSuccess(result_data);
												  if(mySGD->is_endless_mode || mySGD->is_hell_mode)
												  {
													  ((MainFlowScene*)getParent())->heart_time->refreshHeartTime();
												  }
												  else
												  {
													  ((PuzzleScene*)getParent())->heart_time->refreshHeartTime();
												  }
												  
												  addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-500, kGoodsType_money, [=]()
																									  {
																										  ShopPopup* t_shop = ShopPopup::create();
																										  t_shop->setHideFinalAction(this, callfunc_selector(StartSettingPopup::popupClose));
																										  if(mySGD->is_endless_mode)
																											  t_shop->targetHeartTime(((MainFlowScene*)getParent())->heart_time);
																										  else
																											  t_shop->targetHeartTime(((PuzzleScene*)getParent())->heart_time);
																										  t_shop->setShopCode(kSC_heart);
																										  t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
																										  addChild(t_shop, kStartSettingPopupZorder_popup);
																									  }, [=]()
																									  {
																										  if(mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
																											  tutorial_fail_func();
																										  is_menu_enable = true;
																									  }), 9999);

											  }
										  }));
	
	Json::Value cle_param;
	cle_param["memberID"] = myHSP->getMemberID();
	t_command_list.push_back(CommandParam("checkLoginEvent", cle_param, nullptr));
	
	t_command_list.push_back(CommandParam("gettimeevent", Json::Value(), [=](Json::Value result_data)
										  {
											  if(result_data["result"]["code"].asInt() == GDSUCCESS)
											  {
												  mySGD->initTimeEventList(result_data["list"]);
											  }
										  }));
	
	if(mySGD->is_endless_mode)
	{
		Json::Value endless_param;
		endless_param["memberID"] = myHSP->getMemberID();
		endless_param["autoLevel"] = mySGD->getUserdataAutoLevel();
		t_command_list.push_back(CommandParam("startendlessplay", endless_param, nullptr));
	}
	
	mySGD->changeGoodsTransaction(t_command_list, json_selector(this, StartSettingPopup::finalStartAction));
}

void StartSettingPopup::acceptStartAction()
{
	finalSetting();
	
	was_end_startAction = false;
	was_end_removeMessage = false;
	
	
	if(mySGD->getRemoveMessageMailNo() != 0 && mySGD->getRemoveMessageMemberId() != 0)
	{
//		vector<CommandParam> command_list;
//		
//		////////////////////////////// ksks
//		Json::Value p;
//		p["no"] = mySGD->getRemoveMessageMailNo();
//		p["memberID"] = mySGD->getRemoveMessageMemberId();
//		CommandParam ksooParam("removemessage", p, bind(&ThisClassType::finalRemoveMessage, this, _1));
//		command_list.push_back(ksooParam);
//		// create message remove command
//		// command_list.push_back(message remove command);
//		
//		//////////////////////////////
//		
//		command_list.push_back(CommandParam("updateUserData", myDSH->getSaveAllUserDataParam(), json_selector(this, StartSettingPopup::finalAcceptStartAction)));
//		
//		hspConnector::get()->command(command_list);
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
			if(mySGD->getGoodsValue(mySGD->getItemCodeToGoodsType(item_list[i])) > 0)
			{
				mySGD->addChangeGoods(CCString::createWithFormat("u_i_%d", item_list[i])->getCString(), kGoodsType_begin, 0, "", ccsf("%d", mySD->getSilType()), "아이템사용");
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
	
//	Json::Value param;
//	param["key"] = CCString::createWithFormat("stage_start_%d", mySD->getSilType())->getCString();
//	
//	hspConnector::get()->command("increaseStats", param, nullptr);
	
	mySGD->resetLabels();
	myGD->resetGameData();
	
	mySGD->setGameStart();
	
	AudioEngine::sharedInstance()->playEffect("se_heartuse.mp3", false);
	
	AudioEngine::sharedInstance()->stopSound();
	
	if(myDSH->getIntegerForKey(kDSH_Key_showedScenario) == 3)
	{
		mySGD->setNextSceneName("playtutorial");
		
		AudioEngine::sharedInstance()->preloadEffectScene("playtutorial");
		
		addChild(KSTimer::create(0.3f, [=]()
		{
			LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
			addChild(loading_tip, kStartSettingPopupZorder_popup);
		}));
	}
	else
	{
		
		if(!mySGD->is_endless_mode && !mySGD->is_hell_mode){

			std::string playcountKey = std::string("playcount_") + boost::lexical_cast<std::string>(mySD->getSilType());
			
			myDSH->setUserIntForStr(playcountKey, myDSH->getUserIntForStr(playcountKey, 0) + 1);
		}
		mySGD->setNextSceneName("maingame");
		
		addChild(KSTimer::create(0.3f, [=]()
								 {
									 LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
									 addChild(loading_tip, kStartSettingPopupZorder_popup);
								 }));
	}
}

void StartSettingPopup::cancelGame()
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
				mySGD->clearChangeGoods();
				is_using_item[item_list[i]] = true;
			}
		}
		
		mySGD->resetUsingItem();
		
		if(mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
			tutorial_fail_func();
		
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
		if(mySGD->is_endless_mode && mySGD->endless_my_victory.getV() == 0)
			mySGD->pvp_continue_cnt = 0;
		mySGD->ingame_continue_cnt = 0;
		start_loading->removeFromParent();
		
		if(mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
			tutorial_success_func();
		goToGame();
	}
	else
	{
		if(mySGD->is_endless_mode)
			((MainFlowScene*)getParent())->heart_time->backHeart();
		else
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
	bool is_it_possible_achieve = false;
	
	for(int i=kAchievementCode_hidden_shopper2;i<=kAchievementCode_hidden_shopper3;i++)
	{
		if(!myAchieve->isCompleted((AchievementCode)i) && !myAchieve->isAchieve((AchievementCode)i))
			is_it_possible_achieve = true;
		
		if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) && !myAchieve->isAchieve((AchievementCode)i) &&
		   mySGD->getUserdataAchieveItemBuyCount() + cnt >= myAchieve->getCondition((AchievementCode)i))
		{
			myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
			AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			myAchieve->updateAchieve(nullptr);
		}
	}
	
	if(is_it_possible_achieve)
	{
		mySGD->setUserdataAchieveItemBuyCount(mySGD->getUserdataAchieveItemBuyCount() + cnt);
		mySGD->changeUserdata(nullptr);
	}
	
	string fiverocks_param1;
	if(item_list[t_clicked_item_idx] == kIC_baseSpeedUp)
		fiverocks_param1 = "Speed";
	else if(item_list[t_clicked_item_idx] == kIC_doubleItem)
		fiverocks_param1 = "Double";
	else if(item_list[t_clicked_item_idx] == kIC_magnet)
		fiverocks_param1 = "Magnet";
	
	fiverocks::FiveRocksBridge::trackEvent("UseGold", "BuyItem", fiverocks_param1.c_str(), ccsf("Stage %03d", mySD->getSilType()));
	
	int item_cnt = mySGD->getGoodsValue(mySGD->getItemCodeToGoodsType(item_list[t_clicked_item_idx]));
	
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
		item_parent->removeAllChildren();
		
		ITEM_CODE t_ic = item_list[t_clicked_item_idx];
		
		CCSprite* n_item_case = CCSprite::create("startsetting_item_normal_case.png");
		
		CCSprite* n_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
		n_img->setScale(0.7f);
		n_img->setPosition(ccp(n_item_case->getContentSize().width/2.f,n_item_case->getContentSize().height/2.f));
		n_item_case->addChild(n_img);
		
		CCSprite* n_mount = CCSprite::create("startsetting_item_mounted_case.png");
		n_mount->setPosition(ccpFromSize(n_img->getContentSize()/2.f));
		n_mount->setScale(1.f/n_img->getScale());
		n_img->addChild(n_mount, 1);
		
		CCSprite* s_item_case = CCSprite::create("startsetting_item_normal_case.png");
		s_item_case->setColor(ccGRAY);
		
		CCSprite* s_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
		s_img->setColor(ccGRAY);
		s_img->setScale(0.7f);
		s_img->setPosition(ccp(s_item_case->getContentSize().width/2.f,s_item_case->getContentSize().height/2.f));
		s_item_case->addChild(s_img);
		
		CCSprite* s_mount = CCSprite::create("startsetting_item_mounted_case.png");
		s_mount->setPosition(ccpFromSize(s_img->getContentSize()/2.f));
		s_mount->setScale(1.f/s_img->getScale());
		s_img->addChild(s_mount, 1);
		
		CCMenuItem* item_item = CCMenuItemSprite::create(n_item_case, s_item_case, this, menu_selector(StartSettingPopup::itemAction));
		item_item->setTag(t_clicked_item_idx+1);
		
		CCMenu* item_menu = CCMenu::createWithItem(item_item);
		item_menu->setPosition(CCPointZero);
		item_parent->addChild(item_menu);
		item_menu->setTouchPriority(touch_priority);
		
		int item_cnt = mySGD->getGoodsValue(mySGD->getItemCodeToGoodsType(t_ic));
		
		CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 11);
		cnt_label->setPosition(ccp(18, -18));
		item_parent->addChild(cnt_label, kStartSettingPopupItemZorder_cntLabel, kStartSettingPopupItemZorder_cntLabel);
		
		CCSprite* clicked_img = CCSprite::create("startsetting_item_clicked.png");
		clicked_img->setVisible(true);
		clicked_img->setPosition(CCPointZero);
		item_parent->addChild(clicked_img, kStartSettingPopupItemZorder_clicked, kStartSettingPopupItemZorder_clicked);
		
		
		
		
		
//		((CCSprite*)item_parent->getChildByTag(kStartSettingPopupItemZorder_clicked))->setVisible(true);
		is_selected_item[t_clicked_item_idx] = true;
	}
	
	is_menu_enable = true;
}

string StartSettingPopup::convertToItemCodeToItemName(ITEM_CODE t_code)
{
	string return_value;
	if(t_code == kIC_attack)				return_value = "Attack";
	else if(t_code == kIC_speedUp)			return_value = "SpeedUp";
	else if(t_code == kIC_addTime)			return_value = "AddTime";
	else if(t_code == kIC_fast)				return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item4title);
	else if(t_code == kIC_subOneDie)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item5title);
	else if(t_code == kIC_doubleItem)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_doubleItemTitle);
	else if(t_code == kIC_silence)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item7title);
	else if(t_code == kIC_longTime)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item8title);
	else if(t_code == kIC_baseSpeedUp)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_baseSpeedUpTitle);
	else if(t_code == kIC_magnet)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_magnetTitle);
	else if(t_code == kIC_itemGacha)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_itemGachaTitle);
	else if(t_code == kIC_heartUp)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_item10title);
	
	return return_value.c_str();
}
