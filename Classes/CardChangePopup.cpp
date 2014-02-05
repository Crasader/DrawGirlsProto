//
//  CardChangePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 19..
//
//

#include "CardChangePopup.h"
#include "DataStorageHub.h"
#include "CardCase.h"
#include "StageImgLoader.h"
#include "CumberShowWindow.h"
#include "TutorialFlowStep.h"
#include "JackMissileShow.h"
#include "CardStrengthPopup.h"
#include "StartSettingScene.h"
#include "TouchSuctionLayer.h"
#include "CommonButton.h"

enum CardChangePopupZorder{
	kCardChangePopupZorder_gray = 1,
	kCardChangePopupZorder_back,
	kCardChangePopupZorder_content,
	kCardChangePopupZorder_gage,
	kCardChangePopupZorder_gageTop,
	kCardChangePopupZorder_selectedCard,
	kCardChangePopupZorder_popup
};

enum CardChangePopupMenuTag{
	kCardChangePopupMenuTag_close = 1,
	kCardChangePopupMenuTag_mount,
	kCardChangePopupMenuTag_release,
	kCardChangePopupMenuTag_alignRank,
	kCardChangePopupMenuTag_alignTake,
	kCardChangePopupMenuTag_strength,
	kCardChangePopupMenuTag_tip
};

void CardChangePopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool CardChangePopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	is_menu_enable = false;
	base_touch_priority = -200;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCardChangePopupZorder_gray);
	
	main_case = CCSprite::create("cardchange_back.png");
	main_case->setAnchorPoint(ccp(0.5f,0.5f));
	main_case->setPosition(ccp(240,160-450));
	addChild(main_case, kCardChangePopupZorder_back);
	
	
	CommonButton* close_menu = CommonButton::createCloseButton(base_touch_priority);
	close_menu->setPosition(ccp(453,256));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kCardChangePopupMenuTag_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kCardChangePopupZorder_content);
	
	
	CommonButton* strength_menu = CommonButton::create("카드강화", 12, CCSizeMake(80,38), CommonButtonYellow, base_touch_priority);
	strength_menu->setTitleColor(ccBLACK);
	strength_menu->setPosition(ccp(393,256));
	main_case->addChild(strength_menu, kCardChangePopupZorder_content);
	strength_menu->setFunction([=](CCObject* sender)
						  {
							  CCNode* t_node = CCNode::create();
							  t_node->setTag(kCardChangePopupMenuTag_strength);
							  menuAction(t_node);
						  });
	
	
	CommonButton* align_rank_menu = CommonButton::create("등급순", 14, CCSizeMake(75,45), CommonButtonYellow, base_touch_priority);
	align_rank_menu->setTitleColor(ccBLACK);
	align_rank_menu->setPosition(ccp(415,75));
	main_case->addChild(align_rank_menu, kCardChangePopupZorder_content);
	align_rank_menu->setFunction([=](CCObject* sender)
							   {
								   CCNode* t_node = CCNode::create();
								   t_node->setTag(kCardChangePopupMenuTag_alignRank);
								   menuAction(t_node);
							   });
	
	
	CommonButton* align_take_menu = CommonButton::create("획득순", 14, CCSizeMake(75,45), CommonButtonYellow, base_touch_priority);
	align_take_menu->setTitleColor(ccBLACK);
	align_take_menu->setPosition(ccp(415,37));
	main_case->addChild(align_take_menu, kCardChangePopupZorder_content);
	align_take_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kCardChangePopupMenuTag_alignTake);
									 menuAction(t_node);
								 });
	
	setMonster();
	
	explosion_node = CCSpriteBatchNode::create("fx_monster_hit.png");
	main_case->addChild(explosion_node, kCardChangePopupZorder_gage);
	
	selected_card = NULL;
	mount_menu = NULL;
	release_menu = NULL;
	clicked_card_number = 0;
	setSelectedCard(myDSH->getIntegerForKey(kDSH_Key_selectedCard));
	setHaveCardList();
	
	CCSize table_size = CCSizeMake(341, 71);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setPosition(ccp(29, 26));
//	main_case->addChild(temp_back, kCardChangePopupZorder_content);
	
	have_card_table = CCTableView::create(this, table_size);
	have_card_table->setAnchorPoint(CCPointZero);
	have_card_table->setDirection(kCCScrollViewDirectionHorizontal);
	have_card_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	have_card_table->setPosition(ccp(29, 23));
	have_card_table->setDelegate(this);
	main_case->addChild(have_card_table, kCardChangePopupZorder_content);
	have_card_table->setTouchPriority(base_touch_priority);
	
	
	
	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(CardChangePopup::menuAction));
	tip_item->setTag(kCardChangePopupMenuTag_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(465,(myDSH->puzzle_ui_top-320.f)/2.f+320.f-3 -13));
	addChild(tip_menu, kCardChangePopupZorder_content);
	
	tip_menu->setTouchPriority(base_touch_priority+1);
	
	
    return true;
}

void CardChangePopup::setSelectedCard(int t_card_number)
{
	unschedule(schedule_selector(CardChangePopup::shootMissile));
	if(selected_card)
	{
		selected_card->removeFromParent();
		selected_card = NULL;
	}
	if(mount_menu)
	{
		mount_menu->removeFromParent();
		mount_menu = NULL;
	}
	if(release_menu)
	{
		release_menu->removeFromParent();
		release_menu = NULL;
	}
	
	if(t_card_number <= 0)
	{
		CCProgressFromTo* power_progress_to = CCProgressFromTo::create(0.3f, power_gage->getPercentage(), 0);
		power_gage->runAction(power_progress_to);
		
		CCProgressFromTo* dex_progress_to = CCProgressFromTo::create(0.3f, dex_gage->getPercentage(), 0);
		dex_gage->runAction(dex_progress_to);
		
		CCProgressFromTo* speed_progress_to = CCProgressFromTo::create(0.3f, speed_gage->getPercentage(), 0);
		speed_gage->runAction(speed_progress_to);

		return;
	}
	
	int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, t_card_number);
	int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
	
	selected_card = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_grade)->getCString());
	selected_card->setScale(0.29f);
	selected_card->setPosition(ccp(72,173));
	main_case->addChild(selected_card, kCardChangePopupZorder_selectedCard);
	
	if(card_grade == 3 && mySD->isAnimationStage(card_stage))
	{
		CCSize ani_size = mySD->getAnimationCutSize(card_stage);
		CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_grade)->getCString(),
											  CCRectMake(0, 0, ani_size.width, ani_size.height));
		t_ani->setPosition(mySD->getAnimationPosition(card_stage));
		selected_card->addChild(t_ani);
	}
	
	CardCase* t_case = CardCase::create(card_stage, card_grade);
	t_case->setPosition(CCPointZero);
	selected_card->addChild(t_case);
	
//	if(t_card_number == myDSH->getIntegerForKey(kDSH_Key_selectedCard)) // mounted card
//	{
//		addMountedCase();
//		
//		CCSprite* n_release = CCSprite::create("cardchange_release.png");
//		CCSprite* s_release = CCSprite::create("cardchange_release.png");
//		s_release->setColor(ccGRAY);
//		
//		CCMenuItem* release_item = CCMenuItemSprite::create(n_release, s_release, this, menu_selector(CardChangePopup::menuAction));
//		release_item->setTag(kCardChangePopupMenuTag_release);
//		
//		release_menu = CCMenu::createWithItem(release_item);
//		release_menu->setPosition(ccpAdd(selected_card->getPosition(), ccp(0,0)));
//		main_case->addChild(release_menu);
//		release_menu->setTouchPriority(base_touch_priority);
//	}
//	else if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, t_card_number) > 0) // mountable card
//	{
//		CCSprite* n_mount = CCSprite::create("cardchange_mount.png");
//		CCSprite* s_mount = CCSprite::create("cardchange_mount.png");
//		s_mount->setColor(ccGRAY);
//		
//		CCMenuItem* mount_item = CCMenuItemSprite::create(n_mount, s_mount, this, menu_selector(CardChangePopup::menuAction));
//		mount_item->setTag(kCardChangePopupMenuTag_mount);
//		
//		mount_menu = CCMenu::createWithItem(mount_item);
//		mount_menu->setPosition(ccpAdd(selected_card->getPosition(), ccp(0,0)));
//		main_case->addChild(mount_menu);
//		mount_menu->setTouchPriority(base_touch_priority);
//	}
	
	clicked_card_number = t_card_number;
	myDSH->setIntegerForKey(kDSH_Key_selectedCard, clicked_card_number);
	
	float card_power = NSDS_GI(kSDS_CI_int1_missile_power_i, clicked_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, clicked_card_number)-1)*0.1f+1.f);
	float card_dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, clicked_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, clicked_card_number)-1)*0.1f+1.f);
	float card_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, clicked_card_number);
	
	card_power = card_power < 1 ? 1 : card_power;
	card_dex = card_dex < 1 ? 1 : card_dex;
	
	float boss_hp = stage_monster->boss_hp;
	float boss_agi = stage_monster->boss_agi;
	float boss_speed = stage_monster->boss_speed;
	
	float power_percent = 100.f - ((boss_hp - card_power*20.f)/boss_hp*100.f);
	if(power_percent > 100.f)	power_percent = 100.f;
	if(power_percent < 0.f)		power_percent = 0.f;
	float dex_percent = (boss_agi-card_dex)/boss_agi;
	dex_percent = dex_percent < 0 ? 0 : dex_percent;
	dex_percent = dex_percent/100.f*85.f + 0.1f;
	dex_percent *= 100.f;
	if(dex_percent > 100.f)		dex_percent = 100.f;
	if(dex_percent < 0.f)		dex_percent = 0.f;
	dex_percent = 100.f-dex_percent;
	float speed_percent = card_speed/(boss_speed+card_speed)*100.f;
	if(speed_percent > 100.f)	speed_percent = 100.f;
	if(speed_percent < 0.f)		speed_percent = 0.f;
	
	CCProgressFromTo* power_progress_to = CCProgressFromTo::create(0.3f, power_gage->getPercentage(), power_percent);
	power_gage->runAction(power_progress_to);
	
//	power_divide_line->runAction(CCMoveTo::create(0.3f, ccp(power_gage->getPositionX()-power_gage->getContentSize().width/2.f+power_gage->getContentSize().width*power_percent/100.f,power_divide_line->getPositionY())));
	
	CCProgressFromTo* dex_progress_to = CCProgressFromTo::create(0.3f, dex_gage->getPercentage(), dex_percent);
	dex_gage->runAction(dex_progress_to);
	
//	dex_divide_line->runAction(CCMoveTo::create(0.3f, ccp(dex_gage->getPositionX()-dex_gage->getContentSize().width/2.f+dex_gage->getContentSize().width*dex_percent/100.f,dex_divide_line->getPositionY())));
	
	CCProgressFromTo* speed_progress_to = CCProgressFromTo::create(0.3f, speed_gage->getPercentage(), speed_percent);
	speed_gage->runAction(speed_progress_to);
	
//	speed_divide_line->runAction(CCMoveTo::create(0.3f, ccp(speed_gage->getPositionX()-speed_gage->getContentSize().width/2.f+speed_gage->getContentSize().width*speed_percent/100.f,speed_divide_line->getPositionY())));
	
	
	schedule(schedule_selector(CardChangePopup::shootMissile), 0.5f);
}

void CardChangePopup::shootMissile()
{
	int cmCnt = 1;
	
	string missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
	int jm_type = MissileDamageData::getMissileType(missile_code.c_str());
	float missile_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
	
	if(jm_type%10 >= 0 && jm_type%10 <= 3)
	{
		for(int i=0;i<cmCnt;i++)
		{
			JackMissileShow* t_jm = JM_BasicMissileShow::create(stage_monster, jm_type, missile_speed, ccp(72,173), stage_monster->boss_agi, this, callfuncCCpI_selector(CardChangePopup::explosion), callfuncCCpI_selector(CardChangePopup::showDamageMissile), callfuncCCpI_selector(CardChangePopup::showMissMissile));
			main_case->addChild(t_jm, kCardChangePopupZorder_gageTop);
			t_jm->startMoving();
		}
	}
	else if(jm_type%10 >= 4 && jm_type%10 <= 6)
	{
		UM_creatorShow* t_c = UM_creatorShow::create(cmCnt*2, jm_type, missile_speed, ccp(72,173), stage_monster, stage_monster->boss_agi, this, callfuncCCpI_selector(CardChangePopup::explosion), callfuncCCpI_selector(CardChangePopup::showDamageMissile), callfuncCCpI_selector(CardChangePopup::showMissMissile));
		main_case->addChild(t_c, kCardChangePopupZorder_gageTop);
		t_c->startCreate();
	}

}

void CardChangePopup::addMountedCase()
{
//	CCSprite* top_case = CCSprite::create("card_case_check_top.png");
//	top_case->setPosition(ccp(160,430));
//	selected_card->addChild(top_case, 1, kCARDCASE_Tag_mounted_top);
//	
//	CCSprite* bottom_case = CCSprite::create("card_case_check_bottom.png");
//	bottom_case->setPosition(ccp(160,0));
//	selected_card->addChild(bottom_case, 1, kCARDCASE_Tag_mounted_bottom);
//	
//	CCSprite* left_case = CCSprite::create("card_case_check_left.png");
//	left_case->setPosition(ccp(0,215));
//	selected_card->addChild(left_case, 1, kCARDCASE_Tag_mounted_left);
//	
//	CCSprite* right_case = CCSprite::create("card_case_check_right.png");
//	right_case->setPosition(ccp(320,215));
//	selected_card->addChild(right_case, 1, kCARDCASE_Tag_mounted_right);
}

void CardChangePopup::setHaveCardList()
{
	int loop_length = mySGD->getHasGottenCardsSize();
	for(int i=0;i<loop_length;i++)
	{
		int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
		if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
		{
			CardSortInfo t_info = mySGD->getHasGottenCardData(i);
			have_card_list.push_back(t_info);
		}
	}
	
	alignHaveCardList(kCardChangeAlign_takeReverse);
}

void CardChangePopup::alignHaveCardList(CardChangeAlign t_align)
{
	recent_align = t_align;
	
	if(t_align == kCardChangeAlign_rank)
	{
		struct t_CardSortGradeUp{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank > b.rank;
			}
		} pred;
		
		sort(have_card_list.begin(), have_card_list.end(), pred);
	}
	else if(t_align == kCardChangeAlign_rankReverse)
	{
		struct t_CardSortGradeDown{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank < b.rank;
			}
		} pred;
		
		sort(have_card_list.begin(), have_card_list.end(), pred);
	}
	else if(t_align == kCardChangeAlign_take)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number > b.take_number;
			}
		} pred;
		
		sort(have_card_list.begin(), have_card_list.end(), pred);
	}
	else if(t_align == kCardChangeAlign_takeReverse)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number < b.take_number;
			}
		} pred;
		
		sort(have_card_list.begin(), have_card_list.end(), pred);
	}
}

enum CardChangeTableCellZorder{
	kCardChangeTableCellZorder_img = 0,
	kCardChangeTableCellZorder_noimg,
	kCardChangeTableCellZorder_touched,
	kCardChangeTableCellZorder_mounted
};

CCTableViewCell* CardChangePopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCPoint card_center_position = ccp(30, 35);
	
	if(idx == 0)
	{
		CCSprite* release_img = CCSprite::create("cardchange_release_card.png");
		release_img->setPosition(card_center_position);
		cell->addChild(release_img, kCardChangeTableCellZorder_img);
		
		cell->setTag(-1);
	}
	else
	{
		int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, have_card_list[idx-1].card_number);
		int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, have_card_list[idx-1].card_number);
		
		CCSprite* have_card = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
		have_card->setScale(0.73f);
		have_card->setPosition(card_center_position);
		cell->addChild(have_card, kCardChangeTableCellZorder_img);
		
		CCSprite* no_img = CCSprite::create("cardchange_noimg.png");
		no_img->setPosition(card_center_position);
		cell->addChild(no_img, kCardChangeTableCellZorder_noimg);
		
		CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
		mini_rank->setPosition(ccp(17,17));
		no_img->addChild(mini_rank);
		
		CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, have_card_list[idx-1].card_number))->getCString(), mySGD->getFont().c_str(), 8);
		t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
		mini_rank->addChild(t_rank);
		
		CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, have_card_list[idx-1].card_number))->getCString(), mySGD->getFont().c_str(), 8);
		t_card_level_label->setPosition(ccp(17,60));
		cell->addChild(t_card_level_label, kCardChangeTableCellZorder_noimg);
		
		if(have_card_list[idx-1].card_number == myDSH->getIntegerForKey(kDSH_Key_selectedCard))
		{
			CCSprite* small_selected_img = CCSprite::create("cardchange_selected.png");
			small_selected_img->setPosition(card_center_position);
			cell->addChild(small_selected_img, kCardChangeTableCellZorder_mounted);
		}
		else if(have_card_list[idx-1].card_number == clicked_card_number)
		{
			CCSprite* clicked_img = CCSprite::create("cardchange_clicked.png");
			clicked_img->setPosition(card_center_position);
			cell->addChild(clicked_img, kCardChangeTableCellZorder_touched);
		}
		
		cell->setTag(have_card_list[idx-1].card_number);
	}
	
	return cell;
}
void CardChangePopup::scrollViewDidScroll(CCScrollView* view){}
void CardChangePopup::scrollViewDidZoom(CCScrollView* view){}
void CardChangePopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if(!is_menu_enable)
		return;
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	if(recent_step == kTutorialFlowStep_targetCardClick)
	{
		int touched_card_number = have_card_list[cell->getIdx()-1].card_number;
		setSelectedCard(touched_card_number);
		table->updateCellAtIndex(cell->getIdx());
		
		myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_mountClick);
		removeChild(tutorial_node);
		
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_mountClick);
		addChild(t_tutorial, kCardChangePopupZorder_popup);
		
		tutorial_node = t_tutorial;
	}
	else if(recent_step == kTutorialFlowStep_mountClick)
	{
		
	}
	else if(recent_step == kTutorialFlowStep_closeClick)
	{
		
	}
	else
	{
		if(cell->getIdx() == 0)
		{
			int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
			clicked_card_number = 0;
			int found_idx = -1;
			for(int i=0;i<numberOfCellsInTableView(have_card_table) && found_idx == -1;i++)
			{
				CCTableViewCell* t_cell = have_card_table->cellAtIndex(i);
				if(t_cell)
				{
					int cell_card_number = t_cell->getTag();
					if(cell_card_number == selected_card_number)
						found_idx = i;
				}
			}
			if(found_idx != -1)
				have_card_table->updateCellAtIndex(found_idx);
			
			setSelectedCard(clicked_card_number);
		}
		else
		{
			int touched_card_number = have_card_list[cell->getIdx()-1].card_number;
			if(touched_card_number == clicked_card_number)
				return;
			
			is_menu_enable = false;
			
			if(clicked_card_number > 0)
			{
				int found_idx = -1;
				for(int i=1;i<numberOfCellsInTableView(table) && found_idx == -1;i++)
				{
					CCTableViewCell* t_cell = table->cellAtIndex(i);
					if(t_cell)
					{
						int cell_card_number = t_cell->getTag();
						if(cell_card_number == clicked_card_number)
							found_idx = i;
					}
				}
				setSelectedCard(touched_card_number);
				if(found_idx != -1)
					table->updateCellAtIndex(found_idx);
				table->updateCellAtIndex(cell->getIdx());
			}
			else
			{
				setSelectedCard(touched_card_number);
				table->updateCellAtIndex(cell->getIdx());
			}
		}
		is_menu_enable = true;
	}
}
CCSize CardChangePopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(56, 71);
}
unsigned int CardChangePopup::numberOfCellsInTableView(CCTableView *table)
{
	return have_card_list.size() + 1;
}

void CardChangePopup::setMonster()
{
	stage_monster = CumberShowWindow::create(mySD->getSilType(), kCumberShowWindowSceneCode_cardChange);
	stage_monster->setPosition(ccp(409,175));
	main_case->addChild(stage_monster);
	
	CCSprite* boss_gage = CCSprite::create("cardchange_gage_boss.png");
	boss_gage->setPosition(ccp(240,192));
	main_case->addChild(boss_gage, kCardChangePopupZorder_content);
	
	power_gage = CCProgressTimer::create(CCSprite::create("cardchange_gage_power.png"));
	power_gage->setType(kCCProgressTimerTypeBar);
	power_gage->setMidpoint(ccp(0,0));
	power_gage->setBarChangeRate(ccp(1,0));
	power_gage->setPercentage(0);
	power_gage->setPosition(ccp(240,192));
	main_case->addChild(power_gage, kCardChangePopupZorder_gage);
	
//	power_divide_line = CCSprite::create("cardchange_gage_divide_line.png");
//	power_divide_line->setPosition(ccpAdd(power_gage->getPosition(), ccp(power_gage->getContentSize().width*power_gage->getPercentage()/100.f,0)));
//	main_case->addChild(power_divide_line, kCardChangePopupZorder_gageTop);
	
	CCSprite* power_case = CCSprite::create("cardchange_gage_case.png");
	power_case->setPosition(ccpAdd(power_gage->getPosition(), ccp(0,-0.5)));
	main_case->addChild(power_case, kCardChangePopupZorder_gageTop);
	
	CCSprite* boss_gage1 = CCSprite::create("cardchange_gage_boss.png");
	boss_gage1->setPosition(ccp(240,163));
	main_case->addChild(boss_gage1, kCardChangePopupZorder_content);
	
	dex_gage = CCProgressTimer::create(CCSprite::create("cardchange_gage_dex.png"));
	dex_gage->setType(kCCProgressTimerTypeBar);
	dex_gage->setMidpoint(ccp(0,0));
	dex_gage->setBarChangeRate(ccp(1,0));
	dex_gage->setPercentage(0);
	dex_gage->setPosition(ccp(240,163));
	main_case->addChild(dex_gage, kCardChangePopupZorder_gage);
	
//	dex_divide_line = CCSprite::create("cardchange_gage_divide_line.png");
//	dex_divide_line->setPosition(ccpAdd(dex_gage->getPosition(), ccp(dex_gage->getContentSize().width*dex_gage->getPercentage()/100.f,0)));
//	main_case->addChild(dex_divide_line, kCardChangePopupZorder_gageTop);
	
	CCSprite* dex_case = CCSprite::create("cardchange_gage_case.png");
	dex_case->setPosition(ccpAdd(dex_gage->getPosition(), ccp(0,-0.5)));
	main_case->addChild(dex_case, kCardChangePopupZorder_gageTop);
	
	CCSprite* boss_gage2 = CCSprite::create("cardchange_gage_boss.png");
	boss_gage2->setPosition(ccp(240,134));
	main_case->addChild(boss_gage2, kCardChangePopupZorder_content);
	
	speed_gage = CCProgressTimer::create(CCSprite::create("cardchange_gage_speed.png"));
	speed_gage->setType(kCCProgressTimerTypeBar);
	speed_gage->setMidpoint(ccp(0,0));
	speed_gage->setBarChangeRate(ccp(1,0));
	speed_gage->setPercentage(0);
	speed_gage->setPosition(ccp(240,134));
	main_case->addChild(speed_gage, kCardChangePopupZorder_gage);
	
//	speed_divide_line = CCSprite::create("cardchange_gage_divide_line.png");
//	speed_divide_line->setPosition(ccpAdd(speed_gage->getPosition(), ccp(speed_gage->getContentSize().width*speed_gage->getPercentage()/100.f,0)));
//	main_case->addChild(speed_divide_line, kCardChangePopupZorder_gageTop);
	
	CCSprite* speed_case = CCSprite::create("cardchange_gage_case.png");
	speed_case->setPosition(ccpAdd(speed_gage->getPosition(), ccp(0,-0.5)));
	main_case->addChild(speed_case, kCardChangePopupZorder_gageTop);
	
	
//	CCSprite* boss_gage3 = CCSprite::create("cardchange_gage_boss.png");
//	boss_gage3->setPosition(ccp(241,137));
//	main_case->addChild(boss_gage3, kCardChangePopupZorder_content);
}

void CardChangePopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void CardChangePopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardChangePopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardChangePopup::endShowPopup()
{
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_targetCardClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_targetCardClick);
		addChild(t_tutorial, kCardChangePopupZorder_popup);
		
		tutorial_node = t_tutorial;
	}
	else
	{
//		if(!myDSH->getBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardchange))
//		{
//			myDSH->setBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardchange, true);
//			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//			t_suction->target_touch_began = t_suction;
//			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//			t_suction->setTouchEnabled(true);
//			
//			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardchange.png");
//			dimed_tip->setPosition(ccp(240,160));
//			t_suction->addChild(dimed_tip);
//			
//			addChild(t_suction, kCardChangePopupZorder_popup);
//		}
	}
	
	is_menu_enable = true;
}

void CardChangePopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardChangePopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardChangePopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void CardChangePopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	if(recent_step == kTutorialFlowStep_targetCardClick)
	{
		
	}
	else if(recent_step == kTutorialFlowStep_mountClick)
	{
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCardChangePopupMenuTag_mount)
		{
			int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, clicked_card_number);
			if(selected_card_number > 0)
			{
				int found_idx = -1;
				for(int i=0;i<numberOfCellsInTableView(have_card_table) && found_idx == -1;i++)
				{
					CCTableViewCell* t_cell = have_card_table->cellAtIndex(i);
					if(t_cell)
					{
						int cell_card_number = t_cell->getTag();
						if(cell_card_number == selected_card_number)
							found_idx = i;
					}
				}
				if(found_idx != -1)
					have_card_table->updateCellAtIndex(found_idx);
			}
			
			setSelectedCard(clicked_card_number);
			int found_idx = -1;
			for(int i=0;i<numberOfCellsInTableView(have_card_table) && found_idx == -1;i++)
			{
				CCTableViewCell* t_cell = have_card_table->cellAtIndex(i);
				if(t_cell)
				{
					int cell_card_number = t_cell->getTag();
					if(cell_card_number == clicked_card_number)
						found_idx = i;
				}
			}
			if(found_idx != -1)
				have_card_table->updateCellAtIndex(found_idx);
			
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_closeClick);
			removeChild(tutorial_node);
			
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_closeClick);
			addChild(t_tutorial, kCardChangePopupZorder_popup);
			
			tutorial_node = t_tutorial;
		}
	}
	else if(recent_step == kTutorialFlowStep_closeClick)
	{
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCardChangePopupMenuTag_close)
		{
			hidePopup();
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_backClick);
			removeChild(tutorial_node);
		}
	}
	else
	{
		is_menu_enable = false;
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCardChangePopupMenuTag_close)
		{
			hidePopup();
		}
		else if(tag == kCardChangePopupMenuTag_alignRank)
		{
			if(recent_align == kCardChangeAlign_rankReverse)
				alignHaveCardList(kCardChangeAlign_rank);
			else
				alignHaveCardList(kCardChangeAlign_rankReverse);
			
			have_card_table->reloadData();
			is_menu_enable = true;
		}
		else if(tag == kCardChangePopupMenuTag_alignTake)
		{
			if(recent_align == kCardChangeAlign_takeReverse)
				alignHaveCardList(kCardChangeAlign_take);
			else
				alignHaveCardList(kCardChangeAlign_takeReverse);
			
			have_card_table->reloadData();
			is_menu_enable = true;
		}
		else if(tag == kCardChangePopupMenuTag_mount)
		{
			int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, clicked_card_number);
			if(selected_card_number > 0)
			{
				int found_idx = -1;
				for(int i=0;i<numberOfCellsInTableView(have_card_table) && found_idx == -1;i++)
				{
					CCTableViewCell* t_cell = have_card_table->cellAtIndex(i);
					if(t_cell)
					{
						int cell_card_number = t_cell->getTag();
						if(cell_card_number == selected_card_number)
							found_idx = i;
					}
				}
				if(found_idx != -1)
					have_card_table->updateCellAtIndex(found_idx);
			}
			
			setSelectedCard(clicked_card_number);
			int found_idx = -1;
			for(int i=0;i<numberOfCellsInTableView(have_card_table) && found_idx == -1;i++)
			{
				CCTableViewCell* t_cell = have_card_table->cellAtIndex(i);
				if(t_cell)
				{
					int cell_card_number = t_cell->getTag();
					if(cell_card_number == clicked_card_number)
						found_idx = i;
				}
			}
			if(found_idx != -1)
				have_card_table->updateCellAtIndex(found_idx);
			is_menu_enable = true;
		}
		else if(tag == kCardChangePopupMenuTag_release)
		{
			int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
			clicked_card_number = 0;
			int found_idx = -1;
			for(int i=0;i<numberOfCellsInTableView(have_card_table) && found_idx == -1;i++)
			{
				CCTableViewCell* t_cell = have_card_table->cellAtIndex(i);
				if(t_cell)
				{
					int cell_card_number = t_cell->getTag();
					if(cell_card_number == selected_card_number)
						found_idx = i;
				}
			}
			if(found_idx != -1)
				have_card_table->updateCellAtIndex(found_idx);
			
			setSelectedCard(clicked_card_number);
			is_menu_enable = true;
		}
		else if(tag == kCardChangePopupMenuTag_strength)
		{
			mySGD->setCardStrengthBefore(kCardStrengthBefore_cardChange);
			mySGD->setStrengthTargetCardNumber(myDSH->getIntegerForKey(kDSH_Key_selectedCard));
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kStartSettingZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
		else if(tag == kCardChangePopupMenuTag_tip)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
			t_suction->target_touch_began = t_suction;
			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			t_suction->setTouchEnabled(true);
			
			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardchange.png");
			dimed_tip->setPosition(ccp(240,160));
			t_suction->addChild(dimed_tip);
			
			addChild(t_suction, kCardChangePopupZorder_popup);
			
			is_menu_enable = true;
		}
	}
}

void CardChangePopup::explosion(CCPoint startPosition, int t_angle)
{
	AudioEngine::sharedInstance()->playEffect("sound_jack_missile_bomb.mp3",false);
	CCSprite* t_explosion = CCSprite::createWithTexture(explosion_node->getTexture(), CCRectMake(0, 0, 167, 191));
	t_explosion->setScale(0.5f);
	t_explosion->setRotation(-t_angle-90);
	t_explosion->setPosition(startPosition);
	explosion_node->addChild(t_explosion);
	
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1f);
	t_animation->addSpriteFrameWithTexture(explosion_node->getTexture(), CCRectMake(0, 0, 167, 191));
	for(int i=0;i<2;i++)
		for(int j=0;j<3;j++)
			t_animation->addSpriteFrameWithTexture(explosion_node->getTexture(), CCRectMake(j*167, i*191, 167, 191));
	
	CCAnimate* t_animate = CCAnimate::create(t_animation);
	CCFadeTo* t_fade = CCFadeTo::create(0.2f, 0);
	CCRemoveSelf* t_remove = CCRemoveSelf::create();
	CCSequence* t_seq = CCSequence::create(t_animate, t_fade, t_remove, NULL);
	t_explosion->runAction(t_seq);
}

void CardChangePopup::showDamageMissile(CCPoint t_position, int t_damage)
{
	CCNode* container = CCNode::create();
	container->setScale(0.5f);
	container->setPosition(t_position);
	main_case->addChild(container, kCardChangePopupZorder_gage);
	
	CountingBMLabel* damage_label = CountingBMLabel::create("1", "missile_damage_label.fnt", 0.3f, "%d");
	container->addChild(damage_label, kCardChangePopupZorder_gage);
	
	damage_label->setString(CCString::createWithFormat("%d", t_damage)->getCString());
	
	CCDelayTime* t_delay = CCDelayTime::create(0.5f);
	CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
	CCCallFunc* t_call = CCCallFunc::create(container, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::create(t_delay, t_fade, t_call, NULL);
	damage_label->runAction(t_seq);
}

void CardChangePopup::showMissMissile(CCPoint t_position, int t_damage)
{
	t_position.x += rand()%21 - 10;
	t_position.y += rand()%21 - 10;
	
	CCSprite* miss_label = CCSprite::create("missile_miss.png");
	miss_label->setScale(0.5f);
	miss_label->setPosition(t_position);
	main_case->addChild(miss_label, kCardChangePopupZorder_gage);
	
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(miss_label, callfunc_selector(CCSprite::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	miss_label->runAction(t_seq);
}