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
#include "CardCase.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "CardListViewer.h"
#include "StageSettingPopup.h"
#include "CardStrengthPopup.h"

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
	
	is_menu_enable = false;
	mount_menu = NULL;
	
	inner_card_distance = ccp(72,92);
	
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
	
	main_case = CCSprite::create("cardsetting_back.png");
	main_case->setAnchorPoint(ccp(0,0));
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kCSS_Z_back);
	
	my_clv = CardListViewer::create();
	main_case->addChild(my_clv, kCSS_Z_content);
	
	createCardList();
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	if(selected_card_number > 0 && myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) > 0)
	{
		int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, selected_card_number);
		int card_level = NSDS_GI(kSDS_CI_int1_grade_i, selected_card_number);
		
		recent_mounted_number = selected_card_number;
		
		mountingCard(card_stage, card_level);
	}
	else
	{
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
		selected_card_img = NULL;
		selected_img = NULL;
		star_parent = NULL;
		card_option_case = NULL;
		card_option_script = NULL;
		selected_card_menu = NULL;
		check_img = NULL;
		
		recent_mounted_number = selected_card_number;
	}
	
	my_clv->setMaxPositionY();
	
	t_lvs = ListViewerScroll::create(CCRectMake(428, 27, 27, 232), my_clv, "card_scroll.png", ccp(441,49), ccp(441,233));
	main_case->addChild(t_lvs, kCSS_Z_content);
	
	my_clv->setScroll(t_lvs);
	my_clv->setTouchEnabled(true);
	
	CCSprite* n_close = CCSprite::create("cardsetting_close.png");
	CCSprite* s_close = CCSprite::create("cardsetting_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(CardSettingPopup::menuAction));
	close_item->setTag(kCSS_MT_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kCSS_MT_close));
	main_case->addChild(close_menu, kCSS_Z_content);
	
	
	CCSprite* n_align = CCSprite::create("cardsetting_align.png");
	CCSprite* s_align = CCSprite::create("cardsetting_align.png");
	s_align->setColor(ccGRAY);
	
	CCMenuItem* align_item = CCMenuItemSprite::create(n_align, s_align, this, menu_selector(CardSettingPopup::menuAction));
	align_item->setTag(kCSS_MT_align);
	
	CCMenu* align_menu = CCMenu::createWithItem(align_item);
	align_menu->setPosition(getContentPosition(kCSS_MT_align));
	main_case->addChild(align_menu, kCSS_Z_alignButton);
	
	
	CCSprite* n_diary = CCSprite::create("cardsetting_diary.png");
	CCSprite* s_diary = CCSprite::create("cardsetting_diary.png");
	s_diary->setColor(ccGRAY);
	
	CCMenuItem* diary_item = CCMenuItemSprite::create(n_diary, s_diary, this, menu_selector(CardSettingPopup::menuAction));
	diary_item->setTag(kCSS_MT_diary);
	
	CCMenu* diary_menu = CCMenu::createWithItem(diary_item);
	diary_menu->setPosition(getContentPosition(kCSS_MT_diary));
	main_case->addChild(diary_menu, kCSS_Z_content);
	
	align_list_img = NULL;
	
    return true;
}

void CardSettingPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void CardSettingPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardSettingPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardSettingPopup::endShowPopup()
{
	is_menu_enable = true;
	t_lvs->setTouchEnabled(true);
	my_clv->setTouchEnabled(true);
}

void CardSettingPopup::hidePopup()
{
	is_menu_enable = false;
	t_lvs->setTouchEnabled(false);
	my_clv->setTouchEnabled(false);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardSettingPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardSettingPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void CardSettingPopup::createCardList()
{
	CardSortType sort_type = CardSortType(myDSH->getIntegerForKey(kDSH_Key_cardSortType));
	
	if(sort_type == kCST_default)
	{
		align_default_position_list.clear();
		
		int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
		
		int t_stage_y_base = 0;
		for(int i=1;i<=puzzle_count;i++)
		{
			int t_start_stage = NSDS_GI(i, kSDS_PZ_startStage_i);
			int t_stage_count = NSDS_GI(i, kSDS_PZ_stageCount_i);
			for(int j=t_start_stage;j<t_start_stage+t_stage_count;j++)
			{
				for(int k=1;k<=3;k++)
				{
					int t_card_stage = j;
					int t_card_grade = k;
					
					int t_card_number = mySGD->isHasGottenCards(t_card_stage, t_card_grade); // isHasGottenCards == card number call ok
					if(t_card_number > 0)
					{
						CLV_Node* t_node = CLV_Node::create(t_card_number, this, menu_selector(CardSettingPopup::menuAction),
															ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((t_card_grade-1)*inner_card_distance.x, -(t_stage_y_base+t_card_stage-t_start_stage)*inner_card_distance.y)), my_clv->getViewRect());
						my_clv->addChild(t_node, kCSS_Z_content, t_node->getMyTag());
						
						align_default_position_list[t_card_number] = t_node->getPosition();
					}
					else
					{
						CLV_Node* t_node = CLV_Node::create(t_card_stage, t_card_grade, this, menu_selector(CardSettingPopup::menuAction),
															ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((t_card_grade-1)*inner_card_distance.x, -(t_stage_y_base+t_card_stage-t_start_stage)*inner_card_distance.y)), my_clv->getViewRect());
						my_clv->addChild(t_node, kCSS_Z_content, t_node->getMyTag());
						
						align_default_position_list[NSDS_GI(t_card_stage, kSDS_SI_level_int1_card_i, t_card_grade)] = t_node->getPosition();
					}
				}
			}
			t_stage_y_base += t_stage_count;
		}
		int event_stage_count = NSDS_GI(kSDS_GI_eventCount_i);
		for(int i=10001;i<10001+event_stage_count;i++)
		{
			for(int k=1;k<=3;k++)
			{
				int t_card_stage = i;
				int t_card_grade = k;
				
				int t_card_number = mySGD->isHasGottenCards(t_card_stage, t_card_grade); // isHasGottenCards == card number call ok
				if(t_card_number > 0)
				{
					CLV_Node* t_node = CLV_Node::create(t_card_number, this, menu_selector(CardSettingPopup::menuAction),
														ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((t_card_grade-1)*inner_card_distance.x, -(t_stage_y_base+t_card_stage-10001)*inner_card_distance.y)), my_clv->getViewRect());
					my_clv->addChild(t_node, kCSS_Z_content, t_node->getMyTag());
					
					align_default_position_list[t_card_number] = t_node->getPosition();
				}
				else
				{
					CLV_Node* t_node = CLV_Node::create(t_card_stage, t_card_grade, this, menu_selector(CardSettingPopup::menuAction),
														ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((t_card_grade-1)*inner_card_distance.x, -(t_stage_y_base+t_card_stage-10001)*inner_card_distance.y)), my_clv->getViewRect());
					my_clv->addChild(t_node, kCSS_Z_content, t_node->getMyTag());
					
					align_default_position_list[NSDS_GI(t_card_stage, kSDS_SI_level_int1_card_i, t_card_grade)] = t_node->getPosition();
				}
			}
		}
		
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		check_img = CCSprite::create("card_check.png");
		check_img->setPosition(align_default_position_list[selected_card_number]);
		my_clv->addChild(check_img, kCSS_Z_check, kCSS_MT_checkMark);
	}
	else
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		int loop_length = mySGD->getHasGottenCardsSize();
		
		for(int i=0;i<loop_length;i++)
		{
			int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
			
			CLV_Node* t_node = CLV_Node::create(card_number, this, menu_selector(CardSettingPopup::menuAction),
												ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((i%3)*inner_card_distance.x, -(i/3)*inner_card_distance.y)), my_clv->getViewRect());
			my_clv->addChild(t_node, kCSS_Z_content, t_node->getMyTag());
			
			if(selected_card_number == mySGD->getHasGottenCardsDataCardNumber(i))
			{
				check_img = CCSprite::create("card_check.png");
				check_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((i%3)*inner_card_distance.x, -(i/3)*inner_card_distance.y)));
				my_clv->addChild(check_img, kCSS_Z_check, kCSS_MT_checkMark);
			}
		}
	}
}

CCPoint CardSettingPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kCSS_MT_close)			return_value = ccp(440,290);
	else if(t_tag == kCSS_MT_align)		return_value = ccp(265,290);
	else if(t_tag == kCSS_MT_diary)		return_value = ccp(373,290);
	else if(t_tag == kCSS_MT_cardBase)	return_value = ccp(245,213);
	
	return return_value;
}

void CardSettingPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
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
			getParent()->addChild(t_popup, kPMS_Z_popup);
			
			target_final = NULL;
			hidePopup();
		}
	}
	else if(tag == kCSS_MT_align)
	{
		if(align_list_img)
		{
			align_list_img->removeFromParent();
			align_list_img = NULL;
		}
		else
		{
			align_list_img = CCSprite::create("cardsetting_alignback.png");
			align_list_img->setAnchorPoint(ccp(0.5f,1.f));
			align_list_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_align), ccp(0,0)));
			main_case->addChild(align_list_img, kCSS_Z_alignList);
			
			CCMenuItem* align_close_item = CCMenuItemImage::create("cardsetting_align_close.png", "cardsetting_align_close.png", this, menu_selector(CardSettingPopup::menuAction));
			align_close_item->setTag(kCSS_MT_align);
			
			CCMenu* align_close_menu = CCMenu::createWithItem(align_close_item);
			align_close_menu->setPosition(ccp(52,21));
			align_list_img->addChild(align_close_menu);
			align_close_menu->setTouchPriority(kCCMenuHandlerPriority-3);
			
			CCMenuItem* align_default_item = CCMenuItemImage::create("cardsetting_align_click.png", "cardsetting_align_click.png", this, menu_selector(CardSettingPopup::menuAction));
			align_default_item->setTag(kCSS_MT_alignDefault);
			
			CCMenu* align_default_menu = CCMenu::createWithItem(align_default_item);
			align_default_menu->setPosition(ccp(52,53));
			align_list_img->addChild(align_default_menu);
			align_default_menu->setTouchPriority(kCCMenuHandlerPriority-3);
			
			CCMenuItem* align_gradedown_item = CCMenuItemImage::create("cardsetting_align_click.png", "cardsetting_align_click.png", this, menu_selector(CardSettingPopup::menuAction));
			align_gradedown_item->setTag(kCSS_MT_alignGradeDown);
			
			CCMenu* align_gradedown_menu = CCMenu::createWithItem(align_gradedown_item);
			align_gradedown_menu->setPosition(ccp(52,53+27));
			align_list_img->addChild(align_gradedown_menu);
			align_gradedown_menu->setTouchPriority(kCCMenuHandlerPriority-3);
			
			CCMenuItem* align_gradeup_item = CCMenuItemImage::create("cardsetting_align_click.png", "cardsetting_align_click.png", this, menu_selector(CardSettingPopup::menuAction));
			align_gradeup_item->setTag(kCSS_MT_alignGradeUp);
			
			CCMenu* align_gradeup_menu = CCMenu::createWithItem(align_gradeup_item);
			align_gradeup_menu->setPosition(ccp(52,53+27+27));
			align_list_img->addChild(align_gradeup_menu);
			align_gradeup_menu->setTouchPriority(kCCMenuHandlerPriority-3);
			
			CCMenuItem* align_take_item = CCMenuItemImage::create("cardsetting_align_click.png", "cardsetting_align_click.png", this, menu_selector(CardSettingPopup::menuAction));
			align_take_item->setTag(kCSS_MT_alignTake);
			
			CCMenu* align_take_menu = CCMenu::createWithItem(align_take_item);
			align_take_menu->setPosition(ccp(52,53+27+27+27));
			align_list_img->addChild(align_take_menu);
			align_take_menu->setTouchPriority(kCCMenuHandlerPriority-3);
			
			CCSprite* sort_check = CCSprite::create("cardsetting_check.png");
			sort_check->setScale(0.5f);
			align_list_img->addChild(sort_check);
			
			CardSortType sort_type = CardSortType(myDSH->getIntegerForKey(kDSH_Key_cardSortType));
			if(sort_type == kCST_default)			sort_check->setPosition(ccp(18,57));
			else if(sort_type == kCST_gradeDown)	sort_check->setPosition(ccp(18,57+27));
			else if(sort_type == kCST_gradeUp)		sort_check->setPosition(ccp(18,57+27+27));
			else if(sort_type == kCST_take)			sort_check->setPosition(ccp(18,57+27+27+27));
		}
		
		is_menu_enable = true;
	}
	else if(tag == kCSS_MT_alignDefault)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_default)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_default);
			mySGD->changeSortType(kCST_default);
			align_list_img->removeFromParent();
			align_list_img = NULL;
			alignChange();
		}
		
		is_menu_enable = true;
	}
	else if(tag == kCSS_MT_alignGradeDown)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_gradeDown)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_gradeDown);
			mySGD->changeSortType(kCST_gradeDown);
			align_list_img->removeFromParent();
			align_list_img = NULL;
			alignChange();
		}
		
		is_menu_enable = true;
	}
	else if(tag == kCSS_MT_alignGradeUp)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_gradeUp)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_gradeUp);
			mySGD->changeSortType(kCST_gradeUp);
			align_list_img->removeFromParent();
			align_list_img = NULL;
			alignChange();
		}
		
		is_menu_enable = true;
	}
	else if(tag == kCSS_MT_alignTake)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_take)
		{
			myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_take);
			mySGD->changeSortType(kCST_take);
			align_list_img->removeFromParent();
			align_list_img = NULL;
			alignChange();
		}
		
		is_menu_enable = true;
	}
	else if(tag == kCSS_MT_diary)
	{
		if(recent_mounted_number != 0)
		{
			mySGD->selected_collectionbook = recent_mounted_number;
			
			CollectionBookPopup* t_popup = CollectionBookPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kPMS_Z_popup);
			
			target_final = NULL;
			hidePopup();
			
//			CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
		}
		else
		{
			is_menu_enable = true;
		}
	}
	else if(tag == kCSS_MT_selectedCard)
	{
		if(recent_mounted_number != 0)
		{
			mySGD->selected_collectionbook = recent_mounted_number;
			
			CollectionBookPopup* t_popup = CollectionBookPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kPMS_Z_popup);
			
			target_final = NULL;
			hidePopup();
			
//			CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
		}
		else
		{
			is_menu_enable = true;
		}
	}
	else if(tag == kCSS_MT_releaseCard)
	{
		mount_menu->removeFromParent();
		mount_menu = NULL;
		
		removeMountedCase();
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
		
		if(check_img)
		{
			check_img->removeFromParent();
			check_img = NULL;
		}
		
		CCSprite* n_mount = CCSprite::create("card_mount.png");
		CCSprite* s_mount = CCSprite::create("card_mount.png");
		s_mount->setColor(ccGRAY);
		
		CCMenuItem* mount_item = CCMenuItemSprite::create(n_mount, s_mount, this, menu_selector(CardSettingPopup::menuAction));
		mount_item->setTag(kCSS_MT_mountCard);
		
		mount_menu = CCMenu::createWithItem(mount_item);
		mount_menu->setPosition(ccpAdd(selected_card_img->getPosition(), ccp(0,-112)));
		main_case->addChild(mount_menu, kCSS_Z_content);
		
		is_menu_enable = true;
	}
	else if(tag == kCSS_MT_strength)
	{
		CardStrengthPopup* t_popup = CardStrengthPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, kPMS_Z_popup);
		
		target_final = NULL;
		hidePopup();
	}
	else if(tag == kCSS_MT_mountCard)
	{
		mount_menu->removeFromParent();
		mount_menu = NULL;
		
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, recent_mounted_number);
		
		if(check_img)
		{
			check_img->removeFromParent();
			check_img = NULL;
		}
		
		if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) == kCST_default)
		{
			check_img = CCSprite::create("card_check.png");
			check_img->setPosition(align_default_position_list[recent_mounted_number]);
			my_clv->addChild(check_img, kCSS_Z_check, kCSS_MT_checkMark);
		}
		else
		{
			int loop_length = mySGD->getHasGottenCardsSize();
			for(int i=0;i<loop_length;i++)
			{
				if(recent_mounted_number == mySGD->getHasGottenCardsDataCardNumber(i))
				{
					check_img = CCSprite::create("card_check.png");
					check_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((i%3)*inner_card_distance.x, -(i/3)*inner_card_distance.y)));
					my_clv->addChild(check_img, kCSS_Z_check, kCSS_MT_checkMark);
					break;
				}
			}
		}
		
		addMountedCase();
		
		CCSprite* n_release = CCSprite::create("card_release.png");
		CCSprite* s_release = CCSprite::create("card_release.png");
		s_release->setColor(ccGRAY);
		
		CCMenuItem* release_item = CCMenuItemSprite::create(n_release, s_release, this, menu_selector(CardSettingPopup::menuAction));
		release_item->setPosition(ccp(40,0));
		release_item->setTag(kCSS_MT_releaseCard);
		
		mount_menu = CCMenu::createWithItem(release_item);
		mount_menu->setPosition(ccpAdd(selected_card_img->getPosition(), ccp(0,-112)));
		main_case->addChild(mount_menu, kCSS_Z_content);
		
		bool is_strength_enable = true;
		
		// check is strength enable
		
		if(is_strength_enable)
		{
			CCSprite* n_strength = CCSprite::create("cardsetting_strength_on.png");
			CCSprite* s_strength = CCSprite::create("cardsetting_strength_on.png");
			s_strength->setColor(ccGRAY);
			
			CCMenuItem* strength_item = CCMenuItemSprite::create(n_strength, s_strength, this, menu_selector(CardSettingPopup::menuAction));
			strength_item->setPosition(ccp(-40,0));
			strength_item->setTag(kCSS_MT_strength);
			
			mount_menu->addChild(strength_item);
		}
		else
		{
			CCMenuItem* strength_item = CCMenuItemImage::create("cardsetting_strength_off.png", "cardsetting_strength_off.png", this, menu_selector(CardSettingPopup::menuAction));
			strength_item->setPosition(ccp(-40,0));
			strength_item->setEnabled(false);
			
			mount_menu->addChild(strength_item);
		}
		
		is_menu_enable = true;
	}
	else if(tag >= kCSS_MT_cardMenuBase && tag < kCSS_MT_noCardBase)
	{
		int clicked_card_number = tag-kCSS_MT_cardMenuBase;
		
		if(clicked_card_number != recent_mounted_number)
		{
			removeMountingCard();
			recent_mounted_number = clicked_card_number;
			int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, recent_mounted_number);
			int card_level = NSDS_GI(kSDS_CI_int1_grade_i, recent_mounted_number);
			mountingCard(card_stage, card_level);
		}
		else if(myDSH->getIntegerForKey(kDSH_Key_selectedCard) != clicked_card_number && myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, clicked_card_number) > 0)
		{
			mount_menu->removeFromParent();
			mount_menu = NULL;
			
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, clicked_card_number);
			
			if(check_img)
			{
				check_img->removeFromParent();
				check_img = NULL;
			}
			
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) == kCST_default)
			{
				check_img = CCSprite::create("card_check.png");
				check_img->setPosition(align_default_position_list[clicked_card_number]);
				my_clv->addChild(check_img, kCSS_Z_check, kCSS_MT_checkMark);
			}
			else
			{
				int loop_length = mySGD->getHasGottenCardsSize();
				for(int i=0;i<loop_length;i++)
				{
					if(clicked_card_number == mySGD->getHasGottenCardsDataCardNumber(i))
					{
						check_img = CCSprite::create("card_check.png");
						check_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((i%3)*inner_card_distance.x, -(i/3)*inner_card_distance.y)));
						my_clv->addChild(check_img, kCSS_Z_check, kCSS_MT_checkMark);
						break;
					}
				}
			}
			
			addMountedCase();
			
			CCSprite* n_release = CCSprite::create("card_release.png");
			CCSprite* s_release = CCSprite::create("card_release.png");
			s_release->setColor(ccGRAY);
			
			CCMenuItem* release_item = CCMenuItemSprite::create(n_release, s_release, this, menu_selector(CardSettingPopup::menuAction));
			release_item->setPosition(ccp(40,0));
			release_item->setTag(kCSS_MT_releaseCard);
			
			mount_menu = CCMenu::createWithItem(release_item);
			mount_menu->setPosition(ccpAdd(selected_card_img->getPosition(), ccp(0,-112)));
			main_case->addChild(mount_menu, kCSS_Z_content);
			
			bool is_strength_enable = true;
			
			// check is strength enable
			
			if(is_strength_enable)
			{
				CCSprite* n_strength = CCSprite::create("cardsetting_strength_on.png");
				CCSprite* s_strength = CCSprite::create("cardsetting_strength_on.png");
				s_strength->setColor(ccGRAY);
				
				CCMenuItem* strength_item = CCMenuItemSprite::create(n_strength, s_strength, this, menu_selector(CardSettingPopup::menuAction));
				strength_item->setPosition(ccp(-40,0));
				strength_item->setTag(kCSS_MT_strength);
				
				mount_menu->addChild(strength_item);
			}
			else
			{
				CCMenuItem* strength_item = CCMenuItemImage::create("cardsetting_strength_off.png", "cardsetting_strength_off.png", this, menu_selector(CardSettingPopup::menuAction));
				strength_item->setPosition(ccp(-40,0));
				strength_item->setEnabled(false);
				
				mount_menu->addChild(strength_item);
			}
		}
		
		is_menu_enable = true;
	}
	else if(tag >= kCSS_MT_noCardBase)
	{
		removeMountingCard();

		is_menu_enable = true;
	}
}

void CardSettingPopup::removeMountedCase()
{
	selected_card_img->removeChildByTag(kCARDCASE_Tag_mounted_top);
	selected_card_img->removeChildByTag(kCARDCASE_Tag_mounted_bottom);
	selected_card_img->removeChildByTag(kCARDCASE_Tag_mounted_left);
	selected_card_img->removeChildByTag(kCARDCASE_Tag_mounted_right);
}

void CardSettingPopup::addMountedCase()
{
	CCSprite* top_case = CCSprite::create("card_case_check_top.png");
	top_case->setPosition(ccp(160,430));
	selected_card_img->addChild(top_case, 1, kCARDCASE_Tag_mounted_top);
	
	CCSprite* bottom_case = CCSprite::create("card_case_check_bottom.png");
	bottom_case->setPosition(ccp(160,0));
	selected_card_img->addChild(bottom_case, 1, kCARDCASE_Tag_mounted_bottom);
	
	CCSprite* left_case = CCSprite::create("card_case_check_left.png");
	left_case->setPosition(ccp(0,215));
	selected_card_img->addChild(left_case, 1, kCARDCASE_Tag_mounted_left);
	
	CCSprite* right_case = CCSprite::create("card_case_check_right.png");
	right_case->setPosition(ccp(320,215));
	selected_card_img->addChild(right_case, 1, kCARDCASE_Tag_mounted_right);
}

void CardSettingPopup::alignChange()
{
	my_clv->removeAllChildren();
	
	int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, recent_mounted_number);
	int card_level = NSDS_GI(kSDS_CI_int1_grade_i, recent_mounted_number);
	
	if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) == kCST_default)
	{
		selected_img = CCSprite::create("card_selected.png");
		selected_img->setPosition(align_default_position_list[recent_mounted_number]);
		my_clv->addChild(selected_img, kCSS_Z_select, kCSS_MT_selectedCheck);
	}
	else
	{
		int loop_length = mySGD->getHasGottenCardsSize();
		int card_number = NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level);
		for(int i=0;i<loop_length;i++)
		{
			if(card_number == mySGD->getHasGottenCardsDataCardNumber(i))
			{
				selected_img = CCSprite::create("card_selected.png");
				selected_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((i%3)*inner_card_distance.x, -(i/3)*inner_card_distance.y)));
				my_clv->addChild(selected_img, kCSS_Z_select, kCSS_MT_selectedCheck);
				break;
			}
		}
	}
	
	createCardList();
	my_clv->setMaxPositionY();
	my_clv->setPosition(CCPointZero);
}

void CardSettingPopup::removeMountingCard()
{
	if(selected_card_img)		selected_card_img->removeFromParent();
	selected_card_img = NULL;
	if(star_parent)				star_parent->removeFromParent();
	star_parent = NULL;
	
	if(card_option_case)		card_option_case->removeFromParent();
	card_option_case = NULL;
	if(card_option_script)		card_option_script->removeFromParent();
	card_option_script = NULL;
	
	if(selected_card_menu)		selected_card_menu->removeFromParent();
	selected_card_menu = NULL;
	
	if(selected_img)			selected_img->removeFromParent();
	selected_img = NULL;
	
	if(mount_menu)
		mount_menu->removeFromParent();
	mount_menu = NULL;
	
	recent_mounted_number = 0;
}

void CardSettingPopup::mountingCard(int card_stage, int card_level)
{
	selected_card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_level)->getCString());
	selected_card_img->setScale(0.43f);
	selected_card_img->setPosition(ccp(112,159));
	main_case->addChild(selected_card_img, kCSS_Z_content);
	
	if(card_level == 3 && mySD->isAnimationStage(card_stage))
	{
		CCSize ani_size = mySD->getAnimationCutSize(card_stage);
		CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_level)->getCString(),
											  CCRectMake(0, 0, ani_size.width, ani_size.height));
		t_ani->setPosition(mySD->getAnimationPosition(card_stage));
		selected_card_img->addChild(t_ani);
	}
	
	CCSprite* n_cardmenu = CCSprite::create("cardsetting_cardmenu_big.png", CCRectMake(0, 0, 320*selected_card_img->getScale(), 430*selected_card_img->getScale()));
	CCSprite* s_cardmenu = CCSprite::create("cardsetting_cardmenu_big.png", CCRectMake(0, 0, 320*selected_card_img->getScale(), 430*selected_card_img->getScale()));
	
	CCMenuItem* selected_card_item = CCMenuItemSprite::create(n_cardmenu, s_cardmenu, this, menu_selector(CardSettingPopup::menuAction));
	selected_card_item->setTag(kCSS_MT_selectedCard);
	
	selected_card_menu = CCMenu::createWithItem(selected_card_item);
	selected_card_menu->setPosition(selected_card_img->getPosition());
	main_case->addChild(selected_card_menu, kCSS_Z_selectedImg);
	
	CardCase* t_case = CardCase::create(card_stage, card_level);
	t_case->setPosition(CCPointZero);
	selected_card_img->addChild(t_case);
	
	if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) == kCST_default)
	{
		selected_img = CCSprite::create("card_selected.png");
		selected_img->setPosition(align_default_position_list[recent_mounted_number]);
		my_clv->addChild(selected_img, kCSS_Z_select, kCSS_MT_selectedCheck);
	}
	else
	{
		int loop_length = mySGD->getHasGottenCardsSize();
		int card_number = NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, card_level);
		for(int i=0;i<loop_length;i++)
		{
			if(card_number == mySGD->getHasGottenCardsDataCardNumber(i))
			{
				selected_img = CCSprite::create("card_selected.png");
				selected_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((i%3)*inner_card_distance.x, -(i/3)*inner_card_distance.y)));
				my_clv->addChild(selected_img, kCSS_Z_select, kCSS_MT_selectedCheck);
				break;
			}
		}
	}
	
	if(recent_mounted_number == myDSH->getIntegerForKey(kDSH_Key_selectedCard))
	{
		addMountedCase();
		
		CCSprite* n_release = CCSprite::create("card_release.png");
		CCSprite* s_release = CCSprite::create("card_release.png");
		s_release->setColor(ccGRAY);
		
		CCMenuItem* release_item = CCMenuItemSprite::create(n_release, s_release, this, menu_selector(CardSettingPopup::menuAction));
		release_item->setPosition(ccp(40,0));
		release_item->setTag(kCSS_MT_releaseCard);
		
		mount_menu = CCMenu::createWithItem(release_item);
		mount_menu->setPosition(ccpAdd(selected_card_img->getPosition(), ccp(0,-112)));
		main_case->addChild(mount_menu, kCSS_Z_content);
		
		bool is_strength_enable = true;
		
		// check is strength enable
		
		if(is_strength_enable)
		{
			CCSprite* n_strength = CCSprite::create("cardsetting_strength_on.png");
			CCSprite* s_strength = CCSprite::create("cardsetting_strength_on.png");
			s_strength->setColor(ccGRAY);
			
			CCMenuItem* strength_item = CCMenuItemSprite::create(n_strength, s_strength, this, menu_selector(CardSettingPopup::menuAction));
			strength_item->setPosition(ccp(-40,0));
			strength_item->setTag(kCSS_MT_strength);
			
			mount_menu->addChild(strength_item);
		}
		else
		{
			CCMenuItem* strength_item = CCMenuItemImage::create("cardsetting_strength_off.png", "cardsetting_strength_off.png", this, menu_selector(CardSettingPopup::menuAction));
			strength_item->setPosition(ccp(-40,0));
			strength_item->setEnabled(false);
			
			mount_menu->addChild(strength_item);
		}
	}
	else if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, recent_mounted_number) > 0)
	{
		CCSprite* n_mount = CCSprite::create("card_mount.png");
		CCSprite* s_mount = CCSprite::create("card_mount.png");
		s_mount->setColor(ccGRAY);
		
		CCMenuItem* mount_item = CCMenuItemSprite::create(n_mount, s_mount, this, menu_selector(CardSettingPopup::menuAction));
		mount_item->setTag(kCSS_MT_mountCard);
		
		mount_menu = CCMenu::createWithItem(mount_item);
		mount_menu->setPosition(ccpAdd(selected_card_img->getPosition(), ccp(0,-112)));
		main_case->addChild(mount_menu, kCSS_Z_content);
	}
	else
	{
		GraySprite* n_mount = GraySprite::create("card_mount.png");
		n_mount->setGray(true);
		GraySprite* s_mount = GraySprite::create("card_mount.png");
		s_mount->setGray(true);
		
		CCMenuItem* mount_item = CCMenuItemSprite::create(n_mount, s_mount, this, menu_selector(CardSettingPopup::menuAction));
		mount_item->setTag(kCSS_MT_mountCard);
		
		mount_menu = CCMenu::createWithItem(mount_item);
		mount_menu->setEnabled(false);
		mount_menu->setPosition(ccpAdd(selected_card_img->getPosition(), ccp(0,-112)));
		main_case->addChild(mount_menu, kCSS_Z_content);
	}
}