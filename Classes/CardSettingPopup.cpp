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
		
		CCNode* scenario_node = CCNode::create();
		addChild(scenario_node, 9999);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		
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
																		 
																		 addChild(KSTimer::create(0.1f, [=]()
																								  {
																									  scenario_node->removeFromParent();
																								  }));
																	 });
		skip_menu->addChild(skip_item);
		
		typing_box->showAnimation(0.3f);
		
		function<void()> end_func3 = [=]()
		{
			skip_menu->setEnabled(false);
			
			addChild(KSTimer::create(0.1f, [=]()
									 {
										 scenario_node->removeFromParent();
									 }));
		};
		
		function<void()> end_func2 = [=]()
		{
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial5), end_func3);
		};
		
		function<void()> end_func1 = [=]()
		{
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial4), end_func2);
		};
		
		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
															  {
																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
															  }, [=](float t)
															  {
																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
																  skip_menu->setEnabled(true);
																  
																  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial3), end_func1);
															  }));
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
		
		sort(card_list.begin(), card_list.end(), pred);
	}
	else if(t_type == kCST_take)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() > b.take_number.getV();
			}
		} pred;
		
		sort(card_list.begin(), card_list.end(), pred);
	}
	else if(t_type == kCST_takeReverse)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() < b.take_number.getV();
			}
		} pred;
		
		sort(card_list.begin(), card_list.end(), pred);
	}
	else if(t_type == kCST_gradeUp) // rank
	{
		struct t_CardSortGradeUp{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank.getV() > b.rank.getV();
			}
		} pred;
		
		sort(card_list.begin(), card_list.end(), pred);
	}
	else if(t_type == kCST_gradeDown) // rank
	{
		struct t_CardSortGradeDown{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank.getV() < b.rank.getV();
			}
		} pred;
		
		sort(card_list.begin(), card_list.end(), pred);
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
					if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
						case_filename = "cardsetting_on19.png";
					else
						case_filename = "cardsetting_on.png";
				}
				else
				{
					if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
						case_filename = "cardsetting_on_many19.png";
					else
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
				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
					empty_back_filename = "cardsetting_off19.png";
				else
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
				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
					case_filename = "cardsetting_on19.png";
				else
					case_filename = "cardsetting_on.png";
			}
			else
			{
				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
					case_filename = "cardsetting_on_many19.png";
				else
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
				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
					case_filename = "cardsetting_on19.png";
				else
					case_filename = "cardsetting_on.png";
			}
			else
			{
				if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
					case_filename = "cardsetting_on_many19.png";
				else
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
