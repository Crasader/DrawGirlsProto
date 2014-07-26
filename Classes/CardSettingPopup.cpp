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
	
	recent_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
	
	is_take_reverse = recent_sort_type == kCST_takeReverse;
	is_rank_reverse = recent_sort_type == kCST_gradeDown;
	
	for(int i=0;i<mySGD->getHasGottenCardsSize();i++)
	{
		card_list.push_back(mySGD->getHasGottenCardData(i));
	}
	
	changeSortType(CardSortType(recent_sort_type));
	
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
	
	title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mycard), mySGD->getFont().c_str(), 15);
	title_label->disableOuterStroke();
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f));
	title_back->addChild(title_label);
	
	KSLabelTTF* title_label_shadow = KSLabelTTF::create(title_label->getString(), mySGD->getFont().c_str(), 15);
	title_label_shadow->setColor(ccBLACK);
	title_label_shadow->enableOuterStroke(ccBLACK, 1, 255 * 0.6f, true);
	title_label_shadow->setPosition(ccpFromSize(title_label->getContentSize()/2.f) + ccp(0,-1));
	title_label->addChild(title_label_shadow, -1);
	
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
//		button_label_string = myLoc->getLocalForKey(kMyLocalKey_defaultSort);
//	else if(recent_sort_type == kCST_gradeDown)
//		button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▲";
//	else if(recent_sort_type == kCST_gradeUp)
//		button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_gradeOrder)) + "▼";
//	else if(recent_sort_type == kCST_take)
//		button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▲";
//	else if(recent_sort_type == kCST_takeReverse)
//		button_label_string = string(myLoc->getLocalForKey(kMyLocalKey_takeOrder)) + "▼";
	
	align_default_menu = NULL;
	defaultMenuSet();
	
	align_take_menu = NULL;
	takeMenuSet();
	
	align_rank_menu = NULL;
	rankMenuSet();
	
	
	
	take_count_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	take_count_back->setAnchorPoint(ccp(0,0.5f));
	take_count_back->setPosition(ccp(28,240));
	main_case->addChild(take_count_back, kCSS_Z_content);
	
	KSLabelTTF* mycard_count = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_myCardCount), mySGD->getFont().c_str(), 10);
	mycard_count->disableOuterStroke();
	mycard_count->setAnchorPoint(ccp(0.5f,0.5f));
	take_count_back->addChild(mycard_count);
	
	KSLabelTTF* total_card_count = KSLabelTTF::create(ccsf("/%d", mySGD->total_card_cnt), mySGD->getFont().c_str(), 10);
	total_card_count->disableOuterStroke();
	total_card_count->setAnchorPoint(ccp(0.5f,0.5f));
	take_count_back->addChild(total_card_count);
	
	KSLabelTTF* take_card_count = KSLabelTTF::create(ccsf("%d", mySGD->getHasGottenCardsSize()), mySGD->getFont().c_str(), 10);
	take_card_count->setColor(ccc3(255, 170, 20));
	take_card_count->disableOuterStroke();
	take_card_count->setAnchorPoint(ccp(0.5f,0.5f));
	take_count_back->addChild(take_card_count);
	
	take_count_back->setContentSize(CCSizeMake(5 + mycard_count->getContentSize().width + total_card_count->getContentSize().width + take_card_count->getContentSize().width + 5, 18));
	
	mycard_count->setPosition(ccp(5 + mycard_count->getContentSize().width/2.f , take_count_back->getContentSize().height/2.f));
	take_card_count->setPosition(ccp(mycard_count->getPositionX() + mycard_count->getContentSize().width/2.f + take_card_count->getContentSize().width/2.f, take_count_back->getContentSize().height/2.f));
	total_card_count->setPosition(ccp(take_card_count->getPositionX() + take_card_count->getContentSize().width/2.f + total_card_count->getContentSize().width/2.f, take_count_back->getContentSize().height/2.f));
	
	
	CCSprite* n_diary_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_diary_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_diaryView), mySGD->getFont().c_str(), 12.5f);
	n_diary_label->disableOuterStroke();
	n_diary_label->setPosition(ccpFromSize(n_diary_img->getContentSize()/2.f) + ccp(0,-1));
	n_diary_img->addChild(n_diary_label);
	
	CCSprite* s_diary_img = CCSprite::create("subbutton_pink.png");
	s_diary_img->setColor(ccGRAY);
	KSLabelTTF* s_diary_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_diaryView), mySGD->getFont().c_str(), 12.5f);
	s_diary_label->setColor(ccGRAY);
	s_diary_label->disableOuterStroke();
	s_diary_label->setPosition(ccpFromSize(s_diary_img->getContentSize()/2.f) + ccp(0,-1));
	s_diary_img->addChild(s_diary_label);
	
	CCSprite* d_diary_img = CCSprite::create("subbutton_pink.png");
	d_diary_img->setColor(ccGRAY);
	KSLabelTTF* d_diary_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_diaryView), mySGD->getFont().c_str(), 12.5f);
	d_diary_label->disableOuterStroke();
	d_diary_label->setPosition(ccpFromSize(d_diary_img->getContentSize()/2.f) + ccp(0,-1));
	d_diary_img->addChild(d_diary_label);
	
	
	diary_menu = CCMenuItemSprite::create(n_diary_img, s_diary_img, d_diary_img, this, menu_selector(CardSettingPopup::menuAction));
	diary_menu->setTag(kCSS_MT_diary);
	diary_menu->setPosition(ccp(395,16));
	tab_menu->addChild(diary_menu);
	
	diary_menu->setEnabled(mySGD->getHasGottenCardsSize() > 0);
	
	
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
	KSLabelTTF* n_default_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_defaultSort), mySGD->getFont().c_str(), 12.5f);
	n_default_label->disableOuterStroke();
	n_default_label->setPosition(ccpFromSize(n_default_img->getContentSize()/2.f) + ccp(0,2));
	n_default_img->addChild(n_default_label);
	
	CCSprite* s_default_img = CCSprite::create(filename.c_str());
	s_default_img->setColor(ccGRAY);
	KSLabelTTF* s_default_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_defaultSort), mySGD->getFont().c_str(), 12.5f);
	s_default_label->disableOuterStroke();
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
	n_take_label = KSLabelTTF::create((myLoc->getLocalForKey(kMyLocalKey_takeOrder) + sign_str).c_str(), mySGD->getFont().c_str(), 12.5f);
	n_take_label->disableOuterStroke();
	n_take_label->setPosition(ccpFromSize(n_take_img->getContentSize()/2.f) + ccp(0,2));
	n_take_img->addChild(n_take_label);
	
	CCSprite* s_take_img = CCSprite::create(filename.c_str());
	s_take_img->setColor(ccGRAY);
	s_take_label = KSLabelTTF::create((myLoc->getLocalForKey(kMyLocalKey_takeOrder) + sign_str).c_str(), mySGD->getFont().c_str(), 12.5f);
	s_take_label->disableOuterStroke();
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
	n_rank_label = KSLabelTTF::create((myLoc->getLocalForKey(kMyLocalKey_gradeOrder) + sign_str).c_str(), mySGD->getFont().c_str(), 12.5f);
	n_rank_label->disableOuterStroke();
	n_rank_label->setPosition(ccpFromSize(n_rank_img->getContentSize()/2.f) + ccp(0,2));
	n_rank_img->addChild(n_rank_label);
	
	CCSprite* s_rank_img = CCSprite::create(filename.c_str());
	s_rank_img->setColor(ccGRAY);
	s_rank_label = KSLabelTTF::create((myLoc->getLocalForKey(kMyLocalKey_gradeOrder) + sign_str).c_str(), mySGD->getFont().c_str(), 12.5f);
	s_rank_label->disableOuterStroke();
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
}

void CardSettingPopup::changeSortType( CardSortType t_type )
{
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
			
			myDSH->saveUserData(save_userdata_list, nullptr);
			
			
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
			mySGD->setStrengthTargetCardNumber(myDSH->getIntegerForKey(kDSH_Key_selectedCard));
			mySGD->setCardStrengthBefore(kCardStrengthBefore_cardSetting);
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
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
}

void CardSettingPopup::cellAction( CCObject* sender )
{
	
}

CCTableViewCell* CardSettingPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CardSortType sort_type = CardSortType(recent_sort_type);
	
	if(sort_type == kCST_default)
	{
		for(int i=idx*6;i<idx*6+6 && i<mySGD->total_card_cnt;i++)
		{
			int card_number = i+1;
			CCPoint card_position = ccp(35.f + (i-idx*6)*(70.f), cellSizeForTable(table).height/2.f);
			
			if(mySGD->isHasGottenCards(card_number))
			{
				CCClippingNode* n_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
				n_clipping->setAlphaThreshold(0.1f);
				
				GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																											  card_number)->getCString()));
				n_clipping->addChild(n_card);
				
				CCSprite* n_frame = CCSprite::create("cardsetting_on.png");
				n_clipping->addChild(n_frame);
				//			n_card->setScale(0.6f);
				
				CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card->getContentSize().width*n_card->getScale(), n_card->getContentSize().height*n_card->getScale()));
				n_node->setOpacity(0);
				n_clipping->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
				n_node->addChild(n_clipping);
				
				CCClippingNode* s_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
				s_clipping->setAlphaThreshold(0.1f);
				
				GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																											  card_number)->getCString()));
				//			s_card->setScale(0.6f);
				s_card->setColor(ccGRAY);
				s_clipping->addChild(s_card);
				
				CCSprite* s_frame = CCSprite::create("cardsetting_on.png");
				s_clipping->addChild(s_frame);
				
				CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card->getContentSize().width*s_card->getScale(), s_card->getContentSize().height*s_card->getScale()));
				s_node->setOpacity(0);
				s_clipping->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
				s_node->addChild(s_clipping);
				
				CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
				t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
				
				ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
				t_card_menu->setPosition(card_position);
				cell->addChild(t_card_menu);
				t_card_menu->setTouchPriority(-180-3);
			}
			else
			{
				CCSprite* empty_back = CCSprite::create("cardsetting_off.png");
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
					number_label->disableOuterStroke();
					number_label->setPosition(ccpFromSize(empty_back->getContentSize()/2.f) + ccp(0,10));
					empty_back->addChild(number_label);
					
					KSLabelTTF* stage_label = KSLabelTTF::create("STAGE", mySGD->getFont().c_str(), 12);
					stage_label->disableOuterStroke();
					stage_label->setPosition(ccpFromSize(empty_back->getContentSize()/2.f) + ccp(0,-10));
					empty_back->addChild(stage_label);
				}
			}
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
			
			CCClippingNode* n_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
			n_clipping->setAlphaThreshold(0.1f);
			
			GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  card_number)->getCString()));
			n_clipping->addChild(n_card);
			
			CCSprite* n_frame = CCSprite::create("cardsetting_on.png");
			n_clipping->addChild(n_frame);
			//			n_card->setScale(0.6f);
			
			CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card->getContentSize().width*n_card->getScale(), n_card->getContentSize().height*n_card->getScale()));
			n_node->setOpacity(0);
			n_clipping->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
			n_node->addChild(n_clipping);
			
			CCClippingNode* s_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
			s_clipping->setAlphaThreshold(0.1f);
			
			GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  card_number)->getCString()));
			//			s_card->setScale(0.6f);
			s_card->setColor(ccGRAY);
			s_clipping->addChild(s_card);
			
			CCSprite* s_frame = CCSprite::create("cardsetting_on.png");
			s_clipping->addChild(s_frame);
			
			CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card->getContentSize().width*s_card->getScale(), s_card->getContentSize().height*s_card->getScale()));
			s_node->setOpacity(0);
			s_clipping->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
			s_node->addChild(s_clipping);
			
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
