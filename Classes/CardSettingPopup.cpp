//
//  CardSettingPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#include "CardSettingPopup.h"
#include "MyLocalization.h"
#include "StageImgLoader.h"
//#include "CollectionBook.h"
#include "CollectionBookPopup.h"
#include "PuzzleMapScene.h"
#include "MainFlowScene.h"
#include "CardCase.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "CardListViewer.h"
#include "StageSettingPopup.h"
#include "CardStrengthPopup.h"
#include "TutorialFlowStep.h"
#include "TouchSuctionLayer.h"
#include "CommonButton.h"
#include "KSLabelTTF.h"
#include "CommonAnimation.h"
#include "CCMenuLambda.h"
#include "TypingBox.h"
#include "ASPopupView.h"
#include "AsyncImage.h"
#include "StoryLayer.h"
#include "CharacterSelectPopup.h"
#include "CardGachaPopup.h"
#include "LoadingLayer.h"
#include "ConvexGraph.h"
#include "CardComposePopup.h"

void CardSettingPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool CardSettingPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	is_downloading = false;
	
//	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	table_update_cnt = 0;
	
	CCSprite* t_caching = CCSprite::create("loading_card.png");
	t_caching->setScale(0.2f);
	t_caching->setPosition(ccp(-500,-500));
	t_caching->setVisible(false);
	addChild(t_caching);
	
	recent_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
	
	is_take_reverse = recent_sort_type == kCST_takeReverse;
	is_rank_reverse = recent_sort_type == kCST_gradeDown;
	
	card_list.clear();
	event_card_list.clear();
	
	for(int i=0;i<mySGD->getHasGottenCardsSize();i++)
	{
		CardSortInfo t_info = mySGD->getHasGottenCardData(i);
		card_list.push_back(t_info);
		
		string t_category = NSDS_GS(kSDS_CI_int1_category_s, t_info.card_number.getV());
		if(t_category == "event" || t_category == "ePuzzle")
		{
			event_card_list.push_back(t_info);
		}
	}
	
	changeSortType(CardSortType(recent_sort_type), true);
	
	recent_selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	is_menu_enable = false;
	
	setTouchEnabled(true);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCSS_Z_gray);
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kCSS_Z_back);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);
	
	title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_mycard), mySGD->getFont().c_str(), 15);
	title_label->disableOuterStroke();
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);

	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(424, 194));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f + 5);
	main_case->addChild(main_inner);
	
	
	CCSize table_size = CCSizeMake(430, 194);
	CCPoint table_position = ccp(2, 0);
	
//	CCSprite* temp_table = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_table->setAnchorPoint(CCPointZero);
//	temp_table->setOpacity(100);
//	temp_table->setPosition(table_position);
//	main_inner->addChild(temp_table);
	
	
	card_table = CCTableView::create(this, table_size);
	card_table->setAnchorPoint(CCPointZero);
	card_table->setDirection(kCCScrollViewDirectionVertical);
	card_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	card_table->setPosition(table_position);
	card_table->setDelegate(this);
	main_inner->addChild(card_table);
	card_table->setTouchPriority(-180-5);
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-184);
	t_suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	t_suction->setTouchEnabled(true);
	main_inner->addChild(t_suction);
	
	
	CommonButton* close_menu = CommonButton::createCloseButton(-200);
	close_menu->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kCSS_MT_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kCSS_Z_content);
	
	
	tab_menu = CCMenu::create();
	tab_menu->setPosition(CCPointZero);
	main_case->addChild(tab_menu, kCSS_Z_content);
	tab_menu->setTouchPriority(-185);
	
	
//	string button_label_string;
//	if(recent_sort_type == kCST_default)
//		button_label_string = myLoc->getLocalForKey(LK::kMyLocalKey_defaultSort);
//	else if(recent_sort_type == kCST_gradeDown)
//		button_label_string = string(myLoc->getLocalForKey(LK::kMyLocalKey_gradeOrder)) + "▲";
//	else if(recent_sort_type == kCST_gradeUp)
//		button_label_string = string(myLoc->getLocalForKey(LK::kMyLocalKey_gradeOrder)) + "▼";
//	else if(recent_sort_type == kCST_take)
//		button_label_string = string(myLoc->getLocalForKey(LK::kMyLocalKey_takeOrder)) + "▲";
//	else if(recent_sort_type == kCST_takeReverse)
//		button_label_string = string(myLoc->getLocalForKey(LK::kMyLocalKey_takeOrder)) + "▼";
	
	align_default_menu = NULL;
	defaultMenuSet();
	
	align_take_menu = NULL;
	takeMenuSet();
	
	align_rank_menu = NULL;
	rankMenuSet();
	
	event_card_menu = NULL;
	eventMenuSet();
	
	
	take_count_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	take_count_back->setAnchorPoint(ccp(0,0.5f));
	take_count_back->setPosition(ccp(28,240));
	main_case->addChild(take_count_back, kCSS_Z_content);
	
	KSLabelTTF* mycard_count = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_myCardCount), mySGD->getFont().c_str(), 10);
	mycard_count->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	mycard_count->setAnchorPoint(ccp(0.5f,0.5f));
	take_count_back->addChild(mycard_count);
	
//	KSLabelTTF* total_card_count = KSLabelTTF::create(ccsf("/%d", mySGD->total_card_cnt), mySGD->getFont().c_str(), 10);
//	total_card_count->enableOuterStroke(ccBLACK, 0.3f, 50, true);
//	total_card_count->setAnchorPoint(ccp(0.5f,0.5f));
//	take_count_back->addChild(total_card_count);
	
	KSLabelTTF* take_card_count = KSLabelTTF::create(ccsf("%d", mySGD->getHasGottenCardsSize()), mySGD->getFont().c_str(), 10);
	take_card_count->setColor(ccc3(255, 170, 20));
	take_card_count->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	take_card_count->setAnchorPoint(ccp(0.5f,0.5f));
	take_count_back->addChild(take_card_count);
	
	take_count_back->setContentSize(CCSizeMake(5 + mycard_count->getContentSize().width/* + total_card_count->getContentSize().width*/ + take_card_count->getContentSize().width + 5, 18));
	
	mycard_count->setPosition(ccp(5 + mycard_count->getContentSize().width/2.f , take_count_back->getContentSize().height/2.f));
	take_card_count->setPosition(ccp(mycard_count->getPositionX() + mycard_count->getContentSize().width/2.f + take_card_count->getContentSize().width/2.f, take_count_back->getContentSize().height/2.f));
//	total_card_count->setPosition(ccp(take_card_count->getPositionX() + take_card_count->getContentSize().width/2.f + total_card_count->getContentSize().width/2.f, take_count_back->getContentSize().height/2.f));
	
	
	CCSprite* n_diary_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_diary_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_diaryView), mySGD->getFont().c_str(), 12.5f);
	n_diary_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_diary_label->setPosition(ccpFromSize(n_diary_img->getContentSize()/2.f) + ccp(0,-1));
	n_diary_img->addChild(n_diary_label);
	
	CCSprite* s_diary_img = CCSprite::create("subbutton_pink.png");
	s_diary_img->setColor(ccGRAY);
	KSLabelTTF* s_diary_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_diaryView), mySGD->getFont().c_str(), 12.5f);
	s_diary_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_diary_label->setPosition(ccpFromSize(s_diary_img->getContentSize()/2.f) + ccp(0,-1));
	s_diary_img->addChild(s_diary_label);
	
	CCSprite* d_diary_img = GraySprite::create("subbutton_pink.png");
	((GraySprite*)d_diary_img)->setGray(true);
	KSLabelTTF* d_diary_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_diaryView), mySGD->getFont().c_str(), 12.5f);
	d_diary_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_diary_label->setPosition(ccpFromSize(d_diary_img->getContentSize()/2.f) + ccp(0,-1));
	d_diary_img->addChild(d_diary_label);
	
	
	diary_menu = CCMenuItemSprite::create(n_diary_img, s_diary_img, d_diary_img, this, menu_selector(CardSettingPopup::menuAction));
	diary_menu->setTag(kCSS_MT_diary);
	diary_menu->setPosition(ccp(395,16));
	tab_menu->addChild(diary_menu);
	
	diary_menu->setEnabled(mySGD->getHasGottenCardsSize() > 0);
	
	
	CCSprite* n_char_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_char_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_character), mySGD->getFont().c_str(), 12.5f);
	n_char_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_char_label->setPosition(ccpFromSize(n_char_img->getContentSize()/2.f) + ccp(0,-1));
	n_char_img->addChild(n_char_label);
	
	CCSprite* s_char_img = CCSprite::create("subbutton_pink.png");
	s_char_img->setColor(ccGRAY);
	KSLabelTTF* s_char_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_character), mySGD->getFont().c_str(), 12.5f);
	s_char_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_char_label->setPosition(ccpFromSize(s_char_img->getContentSize()/2.f) + ccp(0,-1));
	s_char_img->addChild(s_char_label);
	
	
	CCMenuItem* char_menu = CCMenuItemSprite::create(n_char_img, s_char_img, this, menu_selector(CardSettingPopup::menuAction));
	char_menu->setTag(kCSS_MT_character);
	char_menu->setPosition(ccp(290,16));
	tab_menu->addChild(char_menu);
	
	
	
	CCSprite* n_card_gacha_img = CCSprite::create("subbutton_violet.png");
	KSLabelTTF* n_card_gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardGacha), mySGD->getFont().c_str(), 12.5f);
	n_card_gacha_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_card_gacha_label->setPosition(ccpFromSize(n_card_gacha_img->getContentSize()/2.f) + ccp(0,-1));
	n_card_gacha_img->addChild(n_card_gacha_label);
	
	CCSprite* s_card_gacha_img = CCSprite::create("subbutton_violet.png");
	s_card_gacha_img->setColor(ccGRAY);
	KSLabelTTF* s_card_gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardGacha), mySGD->getFont().c_str(), 12.5f);
	s_card_gacha_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_card_gacha_label->setPosition(ccpFromSize(s_card_gacha_img->getContentSize()/2.f) + ccp(0,-1));
	s_card_gacha_img->addChild(s_card_gacha_label);
	
	
	CCMenuItem* card_gacha_menu = CCMenuItemSprite::create(n_card_gacha_img, s_card_gacha_img, this, menu_selector(CardSettingPopup::menuAction));
	card_gacha_menu->setTag(kCSS_MT_cardGacha);
	card_gacha_menu->setPosition(ccp(80,16));
	tab_menu->addChild(card_gacha_menu);
	
	
	CCSprite* n_card_compose_img = CCSprite::create("subbutton_violet.png");
	KSLabelTTF* n_card_compose_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardCompose), mySGD->getFont().c_str(), 12.5f);
	n_card_compose_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_card_compose_label->setPosition(ccpFromSize(n_card_compose_img->getContentSize()/2.f) + ccp(0,-1));
	n_card_compose_img->addChild(n_card_compose_label);
	
	CCSprite* s_card_compose_img = CCSprite::create("subbutton_violet.png");
	s_card_compose_img->setColor(ccGRAY);
	KSLabelTTF* s_card_compose_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardCompose), mySGD->getFont().c_str(), 12.5f);
	s_card_compose_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_card_compose_label->setPosition(ccpFromSize(s_card_compose_img->getContentSize()/2.f) + ccp(0,-1));
	s_card_compose_img->addChild(s_card_compose_label);
	
	
	CCMenuItem* card_compose_menu = CCMenuItemSprite::create(n_card_compose_img, s_card_compose_img, this, menu_selector(CardSettingPopup::menuAction));
	card_compose_menu->setTag(kCSS_MT_cardCompose);
	card_compose_menu->setPosition(ccp(185,16));
	tab_menu->addChild(card_compose_menu);
	
	
	/*
	CCSprite* n_strength_img = GraySprite::create("subbutton_pink.png");
	((GraySprite*)n_strength_img)->setGray(true);
	KSLabelTTF* n_strength_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardStrength), mySGD->getFont().c_str(), 12.5f);
	n_strength_label->disableOuterStroke();//ccBLACK, 0.5f, 150, true);
	n_strength_label->setPosition(ccpFromSize(n_strength_img->getContentSize()/2.f) + ccp(0,-1));
	n_strength_label->setColor(ccc3(70, 70, 70));
	n_strength_img->addChild(n_strength_label);
	
	CCSprite* s_strength_img = GraySprite::create("subbutton_pink.png");
	((GraySprite*)s_strength_img)->setDeepGray(true);
	KSLabelTTF* s_strength_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardStrength), mySGD->getFont().c_str(), 12.5f);
	s_strength_label->setColor(ccc3(0, 0, 0));
	s_strength_label->disableOuterStroke();
	s_strength_label->setPosition(ccpFromSize(s_strength_img->getContentSize()/2.f) + ccp(0,-1));
	s_strength_img->addChild(s_strength_label);
	
	CCSprite* d_strength_img = GraySprite::create("subbutton_pink.png");
	((GraySprite*)d_strength_img)->setGray(true);
	KSLabelTTF* d_strength_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardStrength), mySGD->getFont().c_str(), 12.5f);
	d_strength_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	d_strength_label->setPosition(ccpFromSize(d_strength_img->getContentSize()/2.f) + ccp(0,-1));
	d_strength_label->setColor(ccGRAY);
	d_strength_img->addChild(d_strength_label);
	
	
	CCMenuItem* strength_menu = CCMenuItemSprite::create(n_strength_img, s_strength_img, d_strength_img, this, menu_selector(CardSettingPopup::menuAction));
	strength_menu->setTag(kCSS_MT_strength);
	strength_menu->setPosition(ccp(290,16));
	tab_menu->addChild(strength_menu);
	*/
	
	if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_card))
	{
		myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_card, true);
		
//		CCNode* scenario_node = CCNode::create();
//		addChild(scenario_node, 9999);
//		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//		
//		
//		CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//		ikaruga->setAnchorPoint(ccp(0,0));
//		ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
//		scenario_node->addChild(ikaruga, 1);
//		
//		TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
//		typing_box->setHide();
//		scenario_node->addChild(typing_box, 2);
//		
//		CCSprite* n_skip = CCSprite::create("kt_skip.png");
//		CCSprite* s_skip = CCSprite::create("kt_skip.png");
//		s_skip->setColor(ccGRAY);
//		
//		CCMenuLambda* skip_menu = CCMenuLambda::create();
//		skip_menu->setPosition(ccp(240-240*screen_scale_x + 35, 160+160*screen_scale_y - 25 + 150));
//		scenario_node->addChild(skip_menu, 3);
//		skip_menu->setTouchPriority(-19999);
//		skip_menu->setEnabled(false);
//		
//		CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [=](CCObject* sender)
//																	 {
//																		 skip_menu->setEnabled(false);
//																		 
//																		 addChild(KSTimer::create(0.1f, [=]()
//																								  {
//																									  scenario_node->removeFromParent();
//																								  }));
//																	 });
//		skip_menu->addChild(skip_item);
//		
//		typing_box->showAnimation(0.3f);
//		
//		function<void()> end_func3 = [=]()
//		{
//			skip_menu->setEnabled(false);
//			
//			addChild(KSTimer::create(0.1f, [=]()
//									 {
//										 scenario_node->removeFromParent();
//									 }));
//		};
//		
//		function<void()> end_func2 = [=]()
//		{
//			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial5), end_func3);
//		};
//		
//		function<void()> end_func1 = [=]()
//		{
//			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial4), end_func2);
//		};
//		
//		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//															  {
//																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
//															  }, [=](float t)
//															  {
//																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
//																  skip_menu->setEnabled(true);
//																  
//																  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial3), end_func1);
//															  }));
		StoryLayer::startStory(this, "menu_mycard");
	}
	
//	{
//		CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(ccsf("card%d_visible.png", 1)));
//		target_img->setAnchorPoint(ccp(0,0));
//		
//		target_img->setScale(0.2f);
//		
//		CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
//		t_texture->setSprite(target_img);
//		t_texture->beginWithClear(0, 0, 0, 0);
//		t_texture->getSprite()->visit();
//		t_texture->end();
//		
//		t_texture->saveToFile("test_save_card1.png", kCCImageFormatPNG);
//		
//		CCSprite* test_img2 = mySIL->getLoadedImg("test_save_card1.png");
////		test_img2->setScale(0.2f/0.4f);
//		test_img2->setPosition(ccp(130,160));
//		addChild(test_img2, 9999);
//	}
//	
//	{
//		CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(ccsf("card%d_visible.png", 1)));
//		target_img->setAnchorPoint(ccp(0,0));
//		
//		target_img->setScale(0.3f);
//		
//		CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
//		t_texture->setSprite(target_img);
//		t_texture->beginWithClear(0, 0, 0, 0);
//		t_texture->getSprite()->visit();
//		t_texture->end();
//		
//		t_texture->saveToFile("test_save_card2.png", kCCImageFormatPNG);
//		
//		CCSprite* test_img2 = mySIL->getLoadedImg("test_save_card2.png");
//		test_img2->setScale(0.2f/0.3f);
//		test_img2->setPosition(ccp(200,160));
//		addChild(test_img2, 9999);
//	}
//	
//	{
//		CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(ccsf("card%d_visible.png", 1)));
//		target_img->setAnchorPoint(ccp(0,0));
//		
//		target_img->setScale(0.4f);
//		
//		CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
//		t_texture->setSprite(target_img);
//		t_texture->beginWithClear(0, 0, 0, 0);
//		t_texture->getSprite()->visit();
//		t_texture->end();
//		
//		t_texture->saveToFile("test_save_card3.png", kCCImageFormatPNG);
//		
//		CCSprite* test_img2 = mySIL->getLoadedImg("test_save_card3.png");
//		test_img2->setScale(0.2f/0.4f);
//		test_img2->setPosition(ccp(270,160));
//		addChild(test_img2, 9999);
//	}
//	
//	{
//		CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(ccsf("card%d_visible.png", 1)));
//		target_img->setAnchorPoint(ccp(0,0));
//		
//		target_img->setScale(0.5f);
//		
//		CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
//		t_texture->setSprite(target_img);
//		t_texture->beginWithClear(0, 0, 0, 0);
//		t_texture->getSprite()->visit();
//		t_texture->end();
//		
//		t_texture->saveToFile("test_save_card4.png", kCCImageFormatPNG);
//		
//		CCSprite* test_img2 = mySIL->getLoadedImg("test_save_card4.png");
//		test_img2->setScale(0.2f/0.5f);
//		test_img2->setPosition(ccp(340,160));
//		addChild(test_img2, 9999);
//	}
//	
//	CCSprite* test_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", 1));
//	test_img->setPosition(ccp(410,160));
//	test_img->setScale(0.2f);
//	addChild(test_img, 9999);
	
    return true;
}

void CardSettingPopup::defaultMenuSet()
{
	string filename;
	
	if(recent_sort_type == kCST_default)
	{
		filename = "tabbutton_up.png";
	}
	else
	{
		filename = "tabbutton_down.png";
	}
	
	CCSprite* n_default_img = CCSprite::create(filename.c_str());
	KSLabelTTF* n_default_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_defaultSort), mySGD->getFont().c_str(), 12.5f);
	n_default_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_default_label->setPosition(ccpFromSize(n_default_img->getContentSize()/2.f) + ccp(0,2));
	n_default_img->addChild(n_default_label);
	
	CCSprite* s_default_img = CCSprite::create(filename.c_str());
	s_default_img->setColor(ccGRAY);
	KSLabelTTF* s_default_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_defaultSort), mySGD->getFont().c_str(), 12.5f);
	s_default_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_default_label->setPosition(ccpFromSize(s_default_img->getContentSize()/2.f) + ccp(0,2));
	s_default_img->addChild(s_default_label);
	
	if(!align_default_menu)
	{
		align_default_menu = CCMenuItemSprite::create(n_default_img, s_default_img, this, menu_selector(CardSettingPopup::menuAction));
		align_default_menu->setTag(kCSS_MT_alignDefault);
		align_default_menu->setPosition(ccp(244,256.5f));
		tab_menu->addChild(align_default_menu);
	}
	else
	{
		align_default_menu->setNormalImage(n_default_img);
		align_default_menu->setSelectedImage(s_default_img);
	}
}

void CardSettingPopup::eventMenuSet()
{
	string filename;
	
	if(recent_sort_type == kCST_event)
	{
		filename = "tabbutton_up.png";
	}
	else
	{
		filename = "tabbutton_down.png";
	}
	
	CCSprite* n_event_img = CCSprite::create(filename.c_str());
	KSLabelTTF* n_event_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_eventCard), mySGD->getFont().c_str(), 12.5f);
	n_event_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_event_label->setPosition(ccpFromSize(n_event_img->getContentSize()/2.f) + ccp(0,2));
	n_event_img->addChild(n_event_label);
	
	CCSprite* s_event_img = CCSprite::create(filename.c_str());
	s_event_img->setColor(ccGRAY);
	KSLabelTTF* s_event_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_eventCard), mySGD->getFont().c_str(), 12.5f);
	s_event_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_event_label->setPosition(ccpFromSize(s_event_img->getContentSize()/2.f) + ccp(0,2));
	s_event_img->addChild(s_event_label);
	
	if(!event_card_menu)
	{
		event_card_menu = CCMenuItemSprite::create(n_event_img, s_event_img, this, menu_selector(CardSettingPopup::menuAction));
		event_card_menu->setTag(kCSS_MT_eventCard);
		event_card_menu->setPosition(ccp(169,256.5f));
		tab_menu->addChild(event_card_menu);
	}
	else
	{
		event_card_menu->setNormalImage(n_event_img);
		event_card_menu->setSelectedImage(s_event_img);
	}
}

void CardSettingPopup::takeMenuSet()
{
	string filename;
	
	if(recent_sort_type == kCST_take || recent_sort_type == kCST_takeReverse)
	{
		filename = "tabbutton_up.png";
	}
	else
	{
		filename = "tabbutton_down.png";
	}
	
	string sign_str;
	
	if(is_take_reverse)
		sign_str = "▼";
	else
		sign_str = "▲";
	
	CCSprite* n_take_img = CCSprite::create(filename.c_str());
	n_take_label = KSLabelTTF::create((myLoc->getLocalForKey(LK::kMyLocalKey_takeOrder) + sign_str).c_str(), mySGD->getFont().c_str(), 12.5f);
	n_take_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_take_label->setPosition(ccpFromSize(n_take_img->getContentSize()/2.f) + ccp(0,2));
	n_take_img->addChild(n_take_label);
	
	CCSprite* s_take_img = CCSprite::create(filename.c_str());
	s_take_img->setColor(ccGRAY);
	s_take_label = KSLabelTTF::create((myLoc->getLocalForKey(LK::kMyLocalKey_takeOrder) + sign_str).c_str(), mySGD->getFont().c_str(), 12.5f);
	s_take_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_take_label->setPosition(ccpFromSize(s_take_img->getContentSize()/2.f) + ccp(0,2));
	s_take_img->addChild(s_take_label);
	
	if(!align_take_menu)
	{
		align_take_menu = CCMenuItemSprite::create(n_take_img, s_take_img, this, menu_selector(CardSettingPopup::menuAction));
		align_take_menu->setTag(kCSS_MT_alignTake);
		align_take_menu->setPosition(ccp(319,256.5f));
		tab_menu->addChild(align_take_menu);
	}
	else
	{
		align_take_menu->setNormalImage(n_take_img);
		align_take_menu->setSelectedImage(s_take_img);
	}
}
void CardSettingPopup::rankMenuSet()
{
	string filename;
	
	if(recent_sort_type == kCST_gradeUp || recent_sort_type == kCST_gradeDown)
	{
		filename = "tabbutton_up.png";
	}
	else
	{
		filename = "tabbutton_down.png";
	}
	
	string sign_str;
	
	if(is_rank_reverse)
		sign_str = "▼";
	else
		sign_str = "▲";
	
	CCSprite* n_rank_img = CCSprite::create(filename.c_str());
	n_rank_label = KSLabelTTF::create((myLoc->getLocalForKey(LK::kMyLocalKey_gradeOrder) + sign_str).c_str(), mySGD->getFont().c_str(), 12.5f);
	n_rank_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_rank_label->setPosition(ccpFromSize(n_rank_img->getContentSize()/2.f) + ccp(0,2));
	n_rank_img->addChild(n_rank_label);
	
	CCSprite* s_rank_img = CCSprite::create(filename.c_str());
	s_rank_img->setColor(ccGRAY);
	s_rank_label = KSLabelTTF::create((myLoc->getLocalForKey(LK::kMyLocalKey_gradeOrder) + sign_str).c_str(), mySGD->getFont().c_str(), 12.5f);
	s_rank_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_rank_label->setPosition(ccpFromSize(s_rank_img->getContentSize()/2.f) + ccp(0,2));
	s_rank_img->addChild(s_rank_label);
	
	if(!align_rank_menu)
	{
		align_rank_menu = CCMenuItemSprite::create(n_rank_img, s_rank_img, this, menu_selector(CardSettingPopup::menuAction));
		align_rank_menu->setTag(kCSS_MT_alignRank);
		align_rank_menu->setPosition(ccp(394,256.5f));
		tab_menu->addChild(align_rank_menu);
	}
	else
	{
		align_rank_menu->setNormalImage(n_rank_img);
		align_rank_menu->setSelectedImage(s_rank_img);
	}
}

void CardSettingPopup::beforeMenuReset(int keep_type)
{
	if(keep_type == kCST_default)
	{
		defaultMenuSet();
	}
	else if(keep_type == kCST_take || keep_type == kCST_takeReverse)
	{
		takeMenuSet();
	}
	else if(keep_type == kCST_gradeUp || keep_type == kCST_gradeDown)
	{
		rankMenuSet();
	}
	else if(keep_type == kCST_event)
	{
		eventMenuSet();
	}
}

void CardSettingPopup::changeSortType( CardSortType t_type, bool is_init )
{
	if(!is_init)
	{
		if(recent_sort_type == kCST_default)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetDefault, card_table->getContentOffset().y);
		}
		else if(recent_sort_type == kCST_take)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetTake, card_table->getContentOffset().y);
		}
		else if(recent_sort_type == kCST_takeReverse)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetTakeReverse, card_table->getContentOffset().y);
		}
		else if(recent_sort_type == kCST_gradeUp)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetGrade, card_table->getContentOffset().y);
		}
		else if(recent_sort_type == kCST_gradeDown)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetGradeReverse, card_table->getContentOffset().y);
		}
		else if(recent_sort_type == kCST_event)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetEvent, card_table->getContentOffset().y);
		}
	}
	
	recent_sort_type = t_type;
	myDSH->setIntegerForKey(kDSH_Key_cardSortType, t_type);
	mySGD->changeSortType(t_type);
	
	if(t_type == kCST_default)
	{
		struct t_CardSortDefault{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.card_number.getV() < b.card_number.getV();
			}
		} pred;
		
		stable_sort(card_list.begin(), card_list.end(), pred);
	}
	else if(t_type == kCST_take)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() > b.take_number.getV();
			}
		} pred;
		
		stable_sort(card_list.begin(), card_list.end(), pred);
	}
	else if(t_type == kCST_takeReverse)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() < b.take_number.getV();
			}
		} pred;
		
		stable_sort(card_list.begin(), card_list.end(), pred);
	}
	else if(t_type == kCST_gradeUp) // rank
	{
		struct t_CardSortGradeUp{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank.getV() > b.rank.getV();
			}
		} pred;
		
		stable_sort(card_list.begin(), card_list.end(), pred);
	}
	else if(t_type == kCST_gradeDown) // rank
	{
		struct t_CardSortGradeDown{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank.getV() < b.rank.getV();
			}
		} pred;
		
		stable_sort(card_list.begin(), card_list.end(), pred);
	}
}

void CardSettingPopup::onEnter()
{
	CCLayer::onEnter();
	
	if(recent_sort_type == kCST_default && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetDefault) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetDefault)));
	}
	else if(recent_sort_type == kCST_take && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetTake) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetTake)));
	}
	else if(recent_sort_type == kCST_takeReverse && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetTakeReverse) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetTakeReverse)));
	}
	else if(recent_sort_type == kCST_gradeUp && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetGrade) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetGrade)));
	}
	else if(recent_sort_type == kCST_gradeDown && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetGradeReverse) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetGradeReverse)));
	}
	else if(recent_sort_type == kCST_event && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetEvent) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetEvent)));
	}
	
	showPopup();
}

void CardSettingPopup::showPopup()
{
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, bind(&CardSettingPopup::endShowPopup, this));
//	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
//	gray->runAction(gray_fade);
//	
//	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-14.f));
//	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardSettingPopup::endShowPopup));
//	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
//	main_case->runAction(main_seq);
}

void CardSettingPopup::endShowPopup()
{
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_upgradeClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_upgradeClick);
		addChild(t_tutorial, kCSS_Z_popup);
		
		tutorial_node = t_tutorial;
		
		card_table->setTouchEnabled(false);
	}
	else
	{
//		if(!myDSH->getBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardsetting))
//		{
//			myDSH->setBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardsetting, true);
//			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//			t_suction->target_touch_began = t_suction;
//			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//			t_suction->setTouchEnabled(true);
//			
//			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardsetting.png");
//			dimed_tip->setPosition(ccp(240,160));
//			t_suction->addChild(dimed_tip);
//			
//			addChild(t_suction, kCSS_Z_popup);
//		}
	}
	
	is_menu_enable = true;
}

void CardSettingPopup::hidePopup()
{
	is_menu_enable = false;
	
	if(recent_sort_type == kCST_default)
	{
		myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetDefault, card_table->getContentOffset().y);
	}
	else if(recent_sort_type == kCST_take)
	{
		myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetTake, card_table->getContentOffset().y);
	}
	else if(recent_sort_type == kCST_takeReverse)
	{
		myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetTakeReverse, card_table->getContentOffset().y);
	}
	else if(recent_sort_type == kCST_gradeUp)
	{
		myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetGrade, card_table->getContentOffset().y);
	}
	else if(recent_sort_type == kCST_gradeDown)
	{
		myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetGradeReverse, card_table->getContentOffset().y);
	}
	else if(recent_sort_type == kCST_event)
	{
		myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetEvent, card_table->getContentOffset().y);
	}
	
	CommonAnimation::closePopup(this, main_case, gray, [=](){
		
	}, bind(&CardSettingPopup::endHidePopup, this));
}

void CardSettingPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

CCPoint CardSettingPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kCSS_MT_close)			return_value = ccp(452,256);
	else if(t_tag == kCSS_MT_cardBase)	return_value = ccp(245,210);
	
	return return_value;
}

void CardSettingPopup::characterClose()
{
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

void CardSettingPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_upgradeClick)
	{
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCSS_MT_strength)
		{
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_upgradeScript);
			is_menu_enable = false;
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
	}
	else
	{
		is_menu_enable = false;
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCSS_MT_close)
		{
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_cardsInfo);
			save_userdata_list.push_back(kSaveUserData_Key_selectedCard);
			
//			myDSH->saveUserData(save_userdata_list, nullptr);
			
			
			if(mySGD->before_cardsetting == kSceneCode_PuzzleMapScene)
				hidePopup();
			else if(mySGD->before_cardsetting == kSceneCode_StageSetting)
			{
				StageSettingPopup* t_popup = StageSettingPopup::create();
				t_popup->setHideFinalAction(target_final, delegate_final);
				getParent()->addChild(t_popup, kMainFlowZorder_popup);
				
				target_final = NULL;
				hidePopup();
			}
		}
		else if(tag == kCSS_MT_alignDefault)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_default)
			{
				int keep_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
				
				changeSortType(kCST_default);
				alignChange();
				defaultMenuSet();
				
				beforeMenuReset(keep_sort_type);
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_eventCard)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_event)
			{
				int keep_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
				
				changeSortType(kCST_event);
				alignChange();
				eventMenuSet();
				
				beforeMenuReset(keep_sort_type);
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_alignRank)
		{
			int keep_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
			
			if(keep_sort_type == kCST_gradeDown || keep_sort_type == kCST_gradeUp)
				is_rank_reverse = !is_rank_reverse;
			
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_gradeDown)
			{
				changeSortType(kCST_gradeDown);
				alignChange();
				rankMenuSet();
				
				beforeMenuReset(keep_sort_type);
			}
			else
			{
				changeSortType(kCST_gradeUp);
				alignChange();
				rankMenuSet();
				
				beforeMenuReset(keep_sort_type);
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_alignTake)
		{
			int keep_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
			
			if(keep_sort_type == kCST_take || keep_sort_type == kCST_takeReverse)
				is_take_reverse = !is_take_reverse;
			
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_take)
			{
				changeSortType(kCST_take);
				alignChange();
				takeMenuSet();
				
				beforeMenuReset(keep_sort_type);
			}
			else
			{
				changeSortType(kCST_takeReverse);
				alignChange();
				takeMenuSet();
				
				beforeMenuReset(keep_sort_type);
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_diary)
		{
			if(recent_sort_type == kCST_default)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetDefault, card_table->getContentOffset().y);
			}
			else if(recent_sort_type == kCST_take)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetTake, card_table->getContentOffset().y);
			}
			else if(recent_sort_type == kCST_takeReverse)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetTakeReverse, card_table->getContentOffset().y);
			}
			else if(recent_sort_type == kCST_gradeUp)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetGrade, card_table->getContentOffset().y);
			}
			else if(recent_sort_type == kCST_gradeDown)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetGradeReverse, card_table->getContentOffset().y);
			}
			else if(recent_sort_type == kCST_event)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSettingTableOffsetEvent, card_table->getContentOffset().y);
			}
			
			mySGD->selected_collectionbook = mySGD->getHasGottenCardsDataCardNumber(mySGD->getHasGottenCardsSize()-1);
			
			CollectionBookPopup* t_popup = CollectionBookPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
		else if(tag == kCSS_MT_character)
		{
			addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){
				main_case->setScaleY(t);
			}, [=](float t){
				main_case->setScaleY(1.2f);
				addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){
					main_case->setScaleY(t);
				}, [=](float t){
					main_case->setScaleY(0.f);
					CharacterSelectPopup* t_popup = CharacterSelectPopup::create();
					t_popup->setHideFinalAction(this, callfunc_selector(CardSettingPopup::characterClose));
					addChild(t_popup, kCSS_Z_popup);
				}));
			}));
		}
		else if(tag == kCSS_MT_cardGacha)
		{
			card_loading = LoadingLayer::create(-9999);
			addChild(card_loading, 9999);
			card_loading->startLoading();
			
			myHSP->command("getcardgachalist", Json::Value(), [=](Json::Value result_data)
						   {
							   if(result_data["result"]["code"].asInt() == GDSUCCESS)
								{
									mySGD->card_gacha_no = result_data["no"].asInt();
									mySGD->card_gacha_msg = result_data["msg"].asString();
									mySGD->card_gacha_list.clear();
									card_download_list.clear();
									
									Json::Value cards = result_data["cardlist"];
									for(int i=0;i<cards.size();i++)
									{
										Json::Value t_card = cards[i];
										KSProtectVar<int> t_gacha_card_no;
										t_gacha_card_no = t_card["no"].asInt();
										mySGD->card_gacha_list.push_back(t_gacha_card_no);
										if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
										   continue;
										NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
										NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
										NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
										NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
										NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
										NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
										
										NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
										
										NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
										NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
										
										Json::Value t_imgInfo = t_card["imgInfo"];
										
										//			bool is_add_cf = false;
										
										if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
										{
											// check, after download ----------
											DownloadFile t_df;
											t_df.size = t_imgInfo["size"].asInt();
											t_df.img = t_imgInfo["img"].asString().c_str();
											t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
											t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
											
											auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
											if(iter == card_download_list.end())
											{
												card_download_list.push_back(t_df);
												// ================================
											}
										}
										
										Json::Value t_aniInfo = t_card["aniInfo"];
										NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
										if(t_aniInfo["isAni"].asBool())
										{
											Json::Value t_detail = t_aniInfo["detail"];
											NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
											
											Json::Value t_loopSeq = t_detail["loopSeq"];
											for(int j=0;j<t_loopSeq.size();j++)
												NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
											
											NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
											NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
											NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
											NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
											NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
											
											if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
											{
												// check, after download ----------
												DownloadFile t_df;
												t_df.size = t_detail["size"].asInt();
												t_df.img = t_detail["img"].asString().c_str();
												t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
												t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
												
												auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
												if(iter == card_download_list.end())
													card_download_list.push_back(t_df);
												// ================================
											}
										}
										
										NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
										NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
										NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
										NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
										NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
										
										NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
										NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
										NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
										
										int sound_cnt = t_card["sound"].size();
										NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
										for(int j=1;j<=sound_cnt;j++)
										{
											NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
										}
										
										NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
										
										Json::Value t_silImgInfo = t_card["silImgInfo"];
										NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
										if(t_silImgInfo["isSil"].asBool())
										{
											if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
											{
												// check, after download ----------
												DownloadFile t_df;
												t_df.size = t_silImgInfo["size"].asInt();
												t_df.img = t_silImgInfo["img"].asString().c_str();
												t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
												t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
												
												auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
												if(iter == card_download_list.end())
													card_download_list.push_back(t_df);
												// ================================
											}
										}
										
										Json::Value t_faceInfo = t_card["faceInfo"];
										if(!t_faceInfo.isNull() && t_faceInfo.asString() != "")
										{
											NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
											NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
											
											if(NSDS_GS(kSDS_CI_int1_faceInfoCcbi_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
											{
												DownloadFile t_df1;
												t_df1.size = t_faceInfo["size"].asInt();
												t_df1.img = t_faceInfo["ccbi"].asString().c_str();
												t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
												t_df1.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str(), t_card["no"].asInt());
												card_download_list.push_back(t_df1);
											}
											
											if(NSDS_GS(kSDS_CI_int1_faceInfoPlist_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".plist"))
											{
												DownloadFile t_df2;
												t_df2.size = t_faceInfo["size"].asInt();
												t_df2.img = t_faceInfo["plist"].asString().c_str();
												t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
												t_df2.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str(), t_card["no"].asInt());
												card_download_list.push_back(t_df2);
											}
											
											if(NSDS_GS(kSDS_CI_int1_faceInfoPvrccz_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".pvr.ccz"))
											{
												DownloadFile t_df3;
												t_df3.size = t_faceInfo["size"].asInt();
												t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
												t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
												t_df3.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str(), t_card["no"].asInt());
												card_download_list.push_back(t_df3);
											}
										}
										else
										{
											NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), false, false);
										}
										mySDS->fFlush(t_card["piece"].asInt(), kSDS_SI_base);
									}
									
									mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
									
									if(card_download_list.size() > 0)
									{
										card_progress_timer = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(201, 13), ConvexGraphType::width);
										card_progress_timer->setPosition(ccp(240,100));
										card_loading->addChild(card_progress_timer);
										
										card_progress_timer->setCover("loading_progress_front1.png", "loading_progress_mask.png");
										card_progress_timer->setBack("loading_progress_back.png");
										
										ing_download_cnt = 1;
										success_download_cnt = 0;
										
										download_set.clear();
										is_enable_index.clear();
										for(int i=0;i<5;i++)
										{
											is_enable_index.push_back(i);
										}
										
										download_type = kCSS_MT_cardGacha;
										startFileDownloadSet();
									}
									else
									{
										card_loading->removeFromParent();
										
										CardGachaPopup* t_popup = CardGachaPopup::create(-200);
										t_popup->setHideFinalAction(target_final, delegate_final);
										getParent()->addChild(t_popup, getZOrder());
										
										target_final = NULL;
										hidePopup();
									}
								}
							   else
								{
									is_menu_enable = true;
									card_loading->removeFromParent();
									addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_endlessServerError)), 999);
								}
						   });
		}
		else if(tag == kCSS_MT_cardCompose)
		{
			card_loading = LoadingLayer::create(-9999);
			addChild(card_loading, 9999);
			card_loading->startLoading();
			
			myHSP->command("getcardcomposelist", Json::Value(), [=](Json::Value result_data)
						   {
							   if(result_data["result"]["code"].asInt() == GDSUCCESS)
							   {
								   mySGD->card_compose_list.clear();
								   card_download_list.clear();
								   
								   Json::Value t_list = result_data["list"];
								   for(int i=0;i<t_list.size();i++)
									{
										Json::Value t_info_json = t_list[i];
										CardComposeInfo t_info;
										t_info.compose_no = t_info_json["no"].asInt();
										t_info.title = t_info_json["title"].asString();
										t_info.msg = t_info_json["msg"].asString();
										t_info.need_exp = t_info_json["needExp"].asInt();
										t_info.compose_card_number = t_info_json["cardNo"].asInt();
										Json::Value material = t_info_json["materialCards"];
										t_info.material_card_list.clear();
										for(int j=0;j<material.size();j++)
										{
											KSProtectVar<int> t_p;
											t_p = material[j].asInt();
											t_info.material_card_list.push_back(t_p);
										}
										
										mySGD->card_compose_list.push_back(t_info);
										
										Json::Value t_card = t_info_json["cardInfo"];
										if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
											continue;
										NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
										NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
										NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
										NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
										NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
										NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
										
										NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
										
										NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
										NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
										
										Json::Value t_imgInfo = t_card["imgInfo"];
										
										//			bool is_add_cf = false;
										
										if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
										{
											// check, after download ----------
											DownloadFile t_df;
											t_df.size = t_imgInfo["size"].asInt();
											t_df.img = t_imgInfo["img"].asString().c_str();
											t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
											t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
											
											auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
											if(iter == card_download_list.end())
											{
												card_download_list.push_back(t_df);
												// ================================
											}
										}
										
										Json::Value t_aniInfo = t_card["aniInfo"];
										NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
										if(t_aniInfo["isAni"].asBool())
										{
											Json::Value t_detail = t_aniInfo["detail"];
											NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
											
											Json::Value t_loopSeq = t_detail["loopSeq"];
											for(int j=0;j<t_loopSeq.size();j++)
												NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
											
											NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
											NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
											NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
											NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
											NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
											
											if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
											{
												// check, after download ----------
												DownloadFile t_df;
												t_df.size = t_detail["size"].asInt();
												t_df.img = t_detail["img"].asString().c_str();
												t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
												t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
												
												auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
												if(iter == card_download_list.end())
													card_download_list.push_back(t_df);
												// ================================
											}
										}
										
										NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
										NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
										NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
										NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
										NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
										
										NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
										NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
										NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
										
										int sound_cnt = t_card["sound"].size();
										NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
										for(int j=1;j<=sound_cnt;j++)
										{
											NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
										}
										
										NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
										
										Json::Value t_silImgInfo = t_card["silImgInfo"];
										NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
										if(t_silImgInfo["isSil"].asBool())
										{
											if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
											{
												// check, after download ----------
												DownloadFile t_df;
												t_df.size = t_silImgInfo["size"].asInt();
												t_df.img = t_silImgInfo["img"].asString().c_str();
												t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
												t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
												
												auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
												if(iter == card_download_list.end())
													card_download_list.push_back(t_df);
												// ================================
											}
										}
										
										Json::Value t_faceInfo = t_card["faceInfo"];
										if(!t_faceInfo.isNull() && t_faceInfo.asString() != "")
										{
											NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
											NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
											
											if(NSDS_GS(kSDS_CI_int1_faceInfoCcbi_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
											{
												DownloadFile t_df1;
												t_df1.size = t_faceInfo["size"].asInt();
												t_df1.img = t_faceInfo["ccbi"].asString().c_str();
												t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
												t_df1.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str(), t_card["no"].asInt());
												card_download_list.push_back(t_df1);
											}
											
											if(NSDS_GS(kSDS_CI_int1_faceInfoPlist_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".plist"))
											{
												DownloadFile t_df2;
												t_df2.size = t_faceInfo["size"].asInt();
												t_df2.img = t_faceInfo["plist"].asString().c_str();
												t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
												t_df2.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str(), t_card["no"].asInt());
												card_download_list.push_back(t_df2);
											}
											
											if(NSDS_GS(kSDS_CI_int1_faceInfoPvrccz_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".pvr.ccz"))
											{
												DownloadFile t_df3;
												t_df3.size = t_faceInfo["size"].asInt();
												t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
												t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
												t_df3.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str(), t_card["no"].asInt());
												card_download_list.push_back(t_df3);
											}
										}
										else
										{
											NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), false, false);
										}
										mySDS->fFlush(t_card["piece"].asInt(), kSDS_SI_base);
									}
								   
								   mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
								   
								   if(card_download_list.size() > 0)
								   {
									   card_progress_timer = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(201, 13), ConvexGraphType::width);
									   card_progress_timer->setPosition(ccp(240,100));
									   card_loading->addChild(card_progress_timer);
									   
									   card_progress_timer->setCover("loading_progress_front1.png", "loading_progress_mask.png");
									   card_progress_timer->setBack("loading_progress_back.png");
									   
									   ing_download_cnt = 1;
									   success_download_cnt = 0;
									   
									   download_set.clear();
									   is_enable_index.clear();
									   for(int i=0;i<5;i++)
									   {
										   is_enable_index.push_back(i);
									   }
									   
									   download_type = kCSS_MT_cardCompose;
									   startFileDownloadSet();
								   }
								   else
								   {
									   card_loading->removeFromParent();
									   
									   CardComposePopup* t_popup = CardComposePopup::create(-200);
									   t_popup->setHideFinalAction(target_final, delegate_final);
									   getParent()->addChild(t_popup, getZOrder());
									   
									   target_final = NULL;
									   hidePopup();
								   }
							   }
							   else
							   {
								   is_menu_enable = true;
								   card_loading->removeFromParent();
								   addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_endlessServerError)), 999);
							   }
						   });
		}
		else if(tag == kCSS_MT_event)
		{
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_strength)
		{
			addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_updateTitle), myLoc->getLocalForKey(LK::kMyLocalKey_updateContent)), 999);
			
			is_menu_enable = true;
			
//			mySGD->setStrengthTargetCardNumber(myDSH->getIntegerForKey(kDSH_Key_selectedCard));
//			mySGD->setCardStrengthBefore(kCardStrengthBefore_cardSetting);
//			CardStrengthPopup* t_popup = CardStrengthPopup::create();
//			t_popup->setHideFinalAction(target_final, delegate_final);
//			getParent()->addChild(t_popup, kMainFlowZorder_popup);
//			
//			target_final = NULL;
//			hidePopup();
		}
		else if(tag == kCSS_MT_tip)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
			t_suction->target_touch_began = t_suction;
			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			t_suction->setTouchEnabled(true);
			
			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardsetting.png");
			dimed_tip->setPosition(ccp(240,160));
			t_suction->addChild(dimed_tip);
			
			addChild(t_suction, kCSS_Z_popup);
			
			is_menu_enable = true;
		}
		else if(tag >= kCSS_MT_cardMenuBase && tag < kCSS_MT_noCardBase)
		{
			int clicked_card_number = tag-kCSS_MT_cardMenuBase;
			
			if(clicked_card_number != 0)
			{
				mySGD->selected_collectionbook = clicked_card_number;
				
				CollectionBookPopup* t_popup = CollectionBookPopup::create();
				t_popup->setHideFinalAction(target_final, delegate_final);
				getParent()->addChild(t_popup, kMainFlowZorder_popup);
				
				target_final = NULL;
				hidePopup();
				
				//			CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
			}
			else
			{
				is_menu_enable = true;
			}
		}
		else if(tag >= kCSS_MT_noCardBase)
		{
			is_menu_enable = true;
		}
	}
}

void CardSettingPopup::startFileDownloadSet()
{
	int max_thread_cnt = 5;
	int total_download_cnt = card_download_list.size();
	int before_download_size = download_set.size();
	
	for(int i=download_set.size();ing_download_cnt <= total_download_cnt && i<max_thread_cnt;i++)
	{
		DownloadImgInfo t_info;
		if(card_download_list.size() > 0 && ing_download_cnt <= card_download_list.size())
		{
			t_info.download_url = card_download_list[ing_download_cnt-1].img;
			t_info.download_size = card_download_list[ing_download_cnt-1].size;
			t_info.download_filename = card_download_list[ing_download_cnt-1].filename;
		}
		else
		{
			break;
		}
		
		t_info.is_fail = false;
		t_info.is_end = false;
		download_set.push_back(t_info);
		
		ing_download_cnt++;
	}
	
	if(download_set.size() > 0)
	{
		for(int i=before_download_size;i<download_set.size();i++)
		{
			int will_download_index = is_enable_index.back();
			CCLOG("start download idx : %d / filename : %s", will_download_index, download_set[i].download_filename.c_str());
			mySIL->downloadImg(download_set[i], will_download_index);
			is_enable_index.pop_back();
		}
		if(!is_downloading)
		{
			is_downloading = true;
			schedule(schedule_selector(CardSettingPopup::checkDownloading));
		}
	}
	else
	{
		unschedule(schedule_selector(CardSettingPopup::checkDownloading));
		
		
		for(int i=0;i<card_download_list.size();i++)
		{
			SDS_SS(kSDF_cardInfo, card_download_list[i].key,
				   card_download_list[i].img, false);
		}
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		card_loading->removeFromParent();
		
		if(download_type == kCSS_MT_cardGacha)
		{
			CardGachaPopup* t_popup = CardGachaPopup::create(-200);
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, getZOrder());
		}
		else if(download_type == kCSS_MT_cardCompose)
		{
			CardComposePopup* t_popup = CardComposePopup::create(-200);
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, getZOrder());
		}
		
		target_final = NULL;
		hidePopup();
	}
}

void CardSettingPopup::checkDownloading()
{
	int max_thread_cnt = 5;
	bool is_successed = false;
	for(int i=0;i<max_thread_cnt;i++)
	{
		if(mySIL->downloading_list[i].is_end)
		{
			if(mySIL->downloading_list[i].is_fail)
			{
				CCLOG("fail download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					CCLOG("start download idx : %d / filename : %s", i, iter->download_filename.c_str());
					mySIL->downloadImg((*iter), i);
				}
				else
				{
					CCLOG("not found fail set");
				}
			}
			else
			{
				CCLOG("success download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					download_set.erase(iter);
				}
				else
				{
					CCLOG("not found success set");
				}
				is_enable_index.push_back(i);
				success_download_cnt++;
				is_successed = true;
			}
		}
	}
	
	float download_percent = 100.f*success_download_cnt/int(card_download_list.size());
	
	card_progress_timer->setPercentage(download_percent);
	
	if(is_successed || success_download_cnt >= card_download_list.size())
	{
		startFileDownloadSet();
	}
}

void CardSettingPopup::alignChange()
{
	card_table->reloadData();
	
	if(recent_sort_type == kCST_default && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetDefault) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetDefault)));
	}
	else if(recent_sort_type == kCST_take && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetTake) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetTake)));
	}
	else if(recent_sort_type == kCST_takeReverse && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetTakeReverse) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetTakeReverse)));
	}
	else if(recent_sort_type == kCST_gradeUp && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetGrade) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetGrade)));
	}
	else if(recent_sort_type == kCST_gradeDown && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetGradeReverse) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetGradeReverse)));
	}
	else if(recent_sort_type == kCST_event && myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetEvent) != 0)
	{
		card_table->setContentOffset(ccp(card_table->getContentOffset().x, myDSH->getIntegerForKey(kDSH_Key_cardSettingTableOffsetEvent)));
	}
}

void CardSettingPopup::cellAction( CCObject* sender )
{
	
}

CCTableViewCell* CardSettingPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CardSortType sort_type = CardSortType(recent_sort_type);
	
	if(sort_type == kCST_default)
	{
		for(int i=idx*6;i<idx*6+6 && i<mySGD->total_card_cnt;i++)
		{
			int card_number = NSDS_GI(kSDS_GI_serial_int1_cardNumber_i, i+1);
			CCPoint card_position = ccp(35.f + (i-idx*6)*(70.f), cellSizeForTable(table).height/2.f);
			
			if(mySGD->isHasGottenCards(card_number))
			{
				CardSortInfo t_info = mySGD->getHasGottenCardDataForCardNumber(card_number);
				string case_filename;
				CCPoint add_position = CCPointZero;
				int c_count = t_info.count.getV();
				if(c_count == 1)
				{
//					if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
//						case_filename = "cardsetting_on19.png";
//					else
						case_filename = "cardsetting_on.png";
				}
				else
				{
//					if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
//						case_filename = "cardsetting_on_many19.png";
//					else
						case_filename = "cardsetting_on_many.png";
					add_position = ccp(-2,3);
				}
				
				CCClippingNode* n_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
				n_clipping->setAlphaThreshold(0.1f);
				
				AsyncImage* n_card = AsyncImage::create(mySIL->getDocumentPath() + ccsf("card%d_visible.png", card_number), "loading_card.png");
//				GraySprite* n_card = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("card%d_thumbnail.png",
//																											  card_number)->getCString()));
				n_clipping->addChild(n_card);
//				n_card->setScale(0.5f);
				n_card->setScale(0.2f);
				
				CCSprite* n_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, n_card->getContentSize().width*n_card->getScale(), n_card->getContentSize().height*n_card->getScale()));
				n_clipping->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f) + add_position);
				n_node->addChild(n_clipping);
				
				CCSprite* n_frame = CCSprite::create(case_filename.c_str());
				n_frame->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
				n_node->addChild(n_frame);
				
				KSLabelTTF* n_label = KSLabelTTF::create(ccsf("No.%d", i+1), mySGD->getFont().c_str(), 9);
				n_label->setPosition(ccp(n_node->getContentSize().width-16, 12) + add_position);
				n_frame->addChild(n_label);
				
				CCClippingNode* s_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
				s_clipping->setAlphaThreshold(0.1f);
				
				AsyncImage* s_card = AsyncImage::create(mySIL->getDocumentPath() + ccsf("card%d_visible.png", card_number), "loading_card.png");
//				GraySprite* s_card = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("card%d_thumbnail.png",
//																											  card_number)->getCString()));
//				s_card->setScale(0.5f);
				s_card->setScale(0.2f);
				s_card->setColor(ccGRAY);
				s_clipping->addChild(s_card);
				
				CCSprite* s_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, s_card->getContentSize().width*s_card->getScale(), s_card->getContentSize().height*s_card->getScale()));
				s_clipping->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f) + add_position);
				s_node->addChild(s_clipping);
				
				CCSprite* s_frame = CCSprite::create(case_filename.c_str());
				s_frame->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
				s_node->addChild(s_frame);
				
				KSLabelTTF* s_label = KSLabelTTF::create(ccsf("No.%d", i+1), mySGD->getFont().c_str(), 9);
				s_label->setPosition(ccp(s_node->getContentSize().width-16, 12) + add_position);
				s_frame->addChild(s_label);
				
				CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
				t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
				
				ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
				t_card_menu->setPosition(card_position);
				cell->addChild(t_card_menu);
				t_card_menu->setTouchPriority(-180-3);
			}
			else
			{
				string empty_back_filename;
//				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
//					empty_back_filename = "cardsetting_off19.png";
//				else
					empty_back_filename = "cardsetting_off.png";
				
				CCSprite* empty_back = CCSprite::create(empty_back_filename.c_str());
				empty_back->setPosition(card_position);
				cell->addChild(empty_back);
				
				int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
				
				if(stage_number == 0)
				{
					KSLabelTTF* number_label = KSLabelTTF::create("?", mySGD->getFont().c_str(), 17);
					number_label->disableOuterStroke();
					number_label->setPosition(ccpFromSize(empty_back->getContentSize()/2.f) + ccp(0,0));
					empty_back->addChild(number_label);
				}
				else
				{
					KSLabelTTF* number_label = KSLabelTTF::create(ccsf("%d", NSDS_GI(kSDS_CI_int1_stage_i, card_number)), mySGD->getFont().c_str(), 17);
					number_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
					number_label->setPosition(ccpFromSize(empty_back->getContentSize()/2.f) + ccp(0,10));
					empty_back->addChild(number_label);
					
					KSLabelTTF* stage_label = KSLabelTTF::create("STAGE", mySGD->getFont().c_str(), 12);
					stage_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
					stage_label->setPosition(ccpFromSize(empty_back->getContentSize()/2.f) + ccp(0,-10));
					empty_back->addChild(stage_label);
				}
			}
		}
	}
	else if(sort_type == kCST_event)
	{
		for(int i=idx*6;i<idx*6+6 && i<event_card_list.size();i++)
		{
			int card_number = event_card_list[i].card_number.getV();
			CCPoint card_position = ccp(35.f + (i-idx*6)*(70.f), cellSizeForTable(table).height/2.f);
			
			CardSortInfo t_info = mySGD->getHasGottenCardDataForCardNumber(card_number);
			string case_filename;
			CCPoint add_position = CCPointZero;
			int c_count = t_info.count.getV();
			if(c_count == 1)
			{
//				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
//					case_filename = "cardsetting_on19.png";
//				else
					case_filename = "cardsetting_on.png";
			}
			else
			{
//				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
//					case_filename = "cardsetting_on_many19.png";
//				else
					case_filename = "cardsetting_on_many.png";
				add_position = ccp(-2,3);
			}
			
			CCClippingNode* n_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
			n_clipping->setAlphaThreshold(0.1f);
			
			AsyncImage* n_card = AsyncImage::create(mySIL->getDocumentPath() + ccsf("card%d_visible.png", card_number), "loading_card.png");
//			GraySprite* n_card = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("card%d_thumbnail.png",
//																											   card_number)->getCString()));
//			n_card->setScale(0.5f);
			n_card->setScale(0.2f);
			n_clipping->addChild(n_card);
			
			CCSprite* n_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, n_card->getContentSize().width*n_card->getScale(), n_card->getContentSize().height*n_card->getScale()));
			n_clipping->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f) + add_position);
			n_node->addChild(n_clipping);
			
			CCSprite* n_frame = CCSprite::create(case_filename.c_str());
			n_frame->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
			n_node->addChild(n_frame);
			
			KSLabelTTF* n_label = KSLabelTTF::create(ccsf("No.%d", NSDS_GI(kSDS_CI_int1_serial_i, card_number)), mySGD->getFont().c_str(), 9);
			n_label->setPosition(ccp(n_node->getContentSize().width-16, 12) + add_position);
			n_frame->addChild(n_label);
			
			
			CCClippingNode* s_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
			s_clipping->setAlphaThreshold(0.1f);
			
			AsyncImage* s_card = AsyncImage::create(mySIL->getDocumentPath() + ccsf("card%d_visible.png", card_number), "loading_card.png");
//			GraySprite* s_card = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("card%d_thumbnail.png",
//																											   card_number)->getCString()));
//			s_card->setScale(0.5f);
			s_card->setScale(0.2f);
			s_card->setColor(ccGRAY);
			s_clipping->addChild(s_card);
			
			CCSprite* s_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, s_card->getContentSize().width*s_card->getScale(), s_card->getContentSize().height*s_card->getScale()));
			s_clipping->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f) + add_position);
			s_node->addChild(s_clipping);
			
			CCSprite* s_frame = CCSprite::create(case_filename.c_str());
			s_frame->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
			s_node->addChild(s_frame);
			
			KSLabelTTF* s_label = KSLabelTTF::create(ccsf("No.%d", NSDS_GI(kSDS_CI_int1_serial_i, card_number)), mySGD->getFont().c_str(), 9);
			s_label->setPosition(ccp(s_node->getContentSize().width-16, 12) + add_position);
			s_frame->addChild(s_label);
			
			CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
			t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
			
			ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
			t_card_menu->setPosition(card_position);
			cell->addChild(t_card_menu);
			t_card_menu->setTouchPriority(-180-3);
		}
	}
	else
	{
		for(int i=idx*6;i<idx*6+6 && i<card_list.size();i++)
		{
			int card_number = card_list[i].card_number.getV();
			CCPoint card_position = ccp(35.f + (i-idx*6)*(70.f), cellSizeForTable(table).height/2.f);
			
			//		string card_type = NSDS_GS(kSDS_CI_int1_category_s, card_number);
			//		if(card_type == "")
			//			card_type = "normal";
			
			CardSortInfo t_info = mySGD->getHasGottenCardDataForCardNumber(card_number);
			string case_filename;
			CCPoint add_position = CCPointZero;
			int c_count = t_info.count.getV();
			if(c_count == 1)
			{
//				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
//					case_filename = "cardsetting_on19.png";
//				else
					case_filename = "cardsetting_on.png";
			}
			else
			{
//				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
//					case_filename = "cardsetting_on_many19.png";
//				else
					case_filename = "cardsetting_on_many.png";
				add_position = ccp(-2,3);
			}
			
			CCClippingNode* n_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
			n_clipping->setAlphaThreshold(0.1f);
			
			AsyncImage* n_card = AsyncImage::create(mySIL->getDocumentPath() + ccsf("card%d_visible.png", card_number), "loading_card.png");
//			GraySprite* n_card = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("card%d_thumbnail.png",
//																										  card_number)->getCString()));
//			n_card->setScale(0.5f);
			n_card->setScale(0.2f);
			n_clipping->addChild(n_card);
			
			CCSprite* n_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, n_card->getContentSize().width*n_card->getScale(), n_card->getContentSize().height*n_card->getScale()));
			n_clipping->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f) + add_position);
			n_node->addChild(n_clipping);
			
			CCSprite* n_frame = CCSprite::create(case_filename.c_str());
			n_frame->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
			n_node->addChild(n_frame);
			
			KSLabelTTF* n_label = KSLabelTTF::create(ccsf("No.%d", NSDS_GI(kSDS_CI_int1_serial_i, card_number)), mySGD->getFont().c_str(), 9);
			n_label->setPosition(ccp(n_node->getContentSize().width-16, 12) + add_position);
			n_frame->addChild(n_label);
			
			
			CCClippingNode* s_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
			s_clipping->setAlphaThreshold(0.1f);
			
			AsyncImage* s_card = AsyncImage::create(mySIL->getDocumentPath() + ccsf("card%d_visible.png", card_number), "loading_card.png");
//			GraySprite* s_card = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("card%d_thumbnail.png",
//																										  card_number)->getCString()));
//			s_card->setScale(0.5f);
			s_card->setScale(0.2f);
			s_card->setColor(ccGRAY);
			s_clipping->addChild(s_card);
			
			CCSprite* s_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, s_card->getContentSize().width*s_card->getScale(), s_card->getContentSize().height*s_card->getScale()));
			s_clipping->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f) + add_position);
			s_node->addChild(s_clipping);
			
			CCSprite* s_frame = CCSprite::create(case_filename.c_str());
			s_frame->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
			s_node->addChild(s_frame);
			
			KSLabelTTF* s_label = KSLabelTTF::create(ccsf("No.%d", NSDS_GI(kSDS_CI_int1_serial_i, card_number)), mySGD->getFont().c_str(), 9);
			s_label->setPosition(ccp(s_node->getContentSize().width-16, 12) + add_position);
			s_frame->addChild(s_label);
			
			CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
			t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
			
			ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
			t_card_menu->setPosition(card_position);
			cell->addChild(t_card_menu);
			t_card_menu->setTouchPriority(-180-3);
			
			//		if(mySGD->isCardMorphing(card_number))
			//		{
			//			CCSprite* morphing_mark = KS::loadCCBI<CCSprite*>(this, "morphing_card.ccbi").first;
			//			morphing_mark->setPosition(card_position + ccp(n_card->getContentSize().width/2.f*n_card->getScale()-10, -n_card->getContentSize().height/2.f*n_card->getScale()+10));
			//			cell->addChild(morphing_mark);
			//		}
			//
			//		if(recent_selected_card_number == card_number)
			//		{
			//			CCSprite* select_img = CCSprite::create("card_check.png");
			//			select_img->setPosition(card_position);
			//			cell->addChild(select_img);
			//		}
		}
	}
	
//	table_update_cnt++;
//	if(table_update_cnt > 3)
//	{
//		CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//		table_update_cnt = 0;
//	}
	
	return cell;
}

void CardSettingPopup::scrollViewDidScroll( CCScrollView* view )
{
//	if(m_scrollBar)
//	{
//		m_scrollBar->setBarRefresh();
//	}
}

void CardSettingPopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void CardSettingPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	
}

CCSize CardSettingPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(430, 95);
}

CCSize CardSettingPopup::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return cellSizeForTable(table);
}

unsigned int CardSettingPopup::numberOfCellsInTableView( CCTableView *table )
{
	CardSortType sort_type = CardSortType(recent_sort_type);
	
	if(sort_type == kCST_default)
	{
		if(mySGD->total_card_cnt == 0)
			return 0;
		else
			return (mySGD->total_card_cnt-1)/6+1;
	}
	else if(sort_type == kCST_event)
	{
		if(event_card_list.empty())
			return 0;
		else
			return (event_card_list.size()-1)/6+1;
	}
	else
	{
		if(card_list.empty())
			return 0;
		else
			return (card_list.size()-1)/6+1;
	}
}

bool CardSettingPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLOG("touch swallow!!");
	
	return true;
}
void CardSettingPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
