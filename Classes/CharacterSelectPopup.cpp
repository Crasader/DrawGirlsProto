//
//  CharacterSelectPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 9. 21..
//
//

#include "CharacterSelectPopup.h"
#include "StarGoldData.h"
#include "TouchSuctionLayer.h"
#include "StageSettingPopup.h"
#include "MainFlowScene.h"
#include "PuzzleScene.h"
#include "KSUtil.h"
#include "StageImgLoader.h"
#include "CommonButton.h"
#include "ScrollMenu.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "CommonAnimation.h"
#include "StyledLabelTTF.h"
#include "LabelTTFMarquee.h"
#include "hspConnector.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"
#include "StoneMissile.h"

enum CharacterSelectPopup_Zorder{
	kCSP_Z_gray = 0,
	kCSP_Z_back,
	kCSP_Z_content,
	kCSP_Z_popup
};

bool CharacterSelectPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	touch_priority = -300;
	is_menu_enable = false;
	
	change_loading = NULL;
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	suction->setTouchEnabled(true);
	addChild(suction);
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240, 160 - 14));
	addChild(main_case, kCSP_Z_back);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_selectCharacter), mySGD->getFont().c_str(), 15);
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(424, 204));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f);
	main_case->addChild(main_inner);
	
	CCScale9Sprite* tip_marquee_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	tip_marquee_back->setContentSize(CCSizeMake(278, 26));
	tip_marquee_back->setPosition(ccp(main_case->getContentSize().width*0.655f, main_case->getContentSize().height+2-23.5f));
	main_case->addChild(tip_marquee_back);
	
	LabelTTFMarquee* tipMaquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 278, 22, "");
	tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_characterMarquee));
	tipMaquee->setPosition(ccpFromSize(tip_marquee_back->getContentSize()/2.f));
	tipMaquee->startMarquee();
	tipMaquee->setAnchorPoint(ccp(0.5f,0.5f));
	tip_marquee_back->addChild(tipMaquee);
	
	CCSprite* tipBack = CCSprite::create("tabbutton_up.png");
	tipBack->setPosition(ccp(main_case->getContentSize().width*0.417f, main_case->getContentSize().height+2-25));
	main_case->addChild(tipBack);
	KSLabelTTF* tipLbl = KSLabelTTF::create("TIP", mySGD->getFont().c_str(), 14.f);
	tipLbl->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	//	tipLbl->disableOuterStroke();
	tipLbl->setPosition(ccpFromSize(tipBack->getContentSize()) / 2.f + ccp(0,1));
	tipBack->addChild(tipLbl);
	
	CommonButton* close_menu = CommonButton::createCloseButton(touch_priority-4);
	close_menu->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
							{
								if(!is_menu_enable)
									return;
								
								is_menu_enable = false;
								
								AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								
								CommonAnimation::closePopup(this, main_case, NULL, [=](){
									
								}, [=](){
									if(target_final && delegate_final)
										(target_final->*delegate_final)();
									removeFromParent();
								});
							});
	main_case->addChild(close_menu, kCSP_Z_content);
	
	
	list_cnt = NSDS_GI(kSDS_GI_characterCount_i);
	for(int i=0;i<list_cnt;i++)
	{
		CharacterInfo t_info;
		t_info.m_number = NSDS_GI(kSDS_GI_characterInfo_int1_no_i, i+1);
		t_info.m_index = i;
		t_info.m_name = NSDS_GS(kSDS_GI_characterInfo_int1_name_s, i+1);
		t_info.m_comment = NSDS_GS(kSDS_GI_characterInfo_int1_comment_s, i+1);
		t_info.m_character = NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, i+1);
		t_info.is_have = (i == 0);
		t_info.m_card = -1;
		
		bool is_found = false;
		int history_size = mySGD->getCharacterHistorySize();
		for(int j=0;!is_found && j<history_size;j++)
		{
			CharacterHistory t_history = mySGD->getCharacterHistory(j);
			if(t_history.characterNo.getV() == t_info.m_number)
			{
				t_info.m_damage = t_history.power.getV();
				t_info.m_level = t_history.level.getV();
				t_info.is_have = true;
				is_found = true;
			}
		}
		
		history_list.push_back(t_info);
	}
	
	int hell_list_cnt = NSDS_GI(kSDS_GI_hellMode_listCount_i);
	for(int i=0;i<hell_list_cnt;i++)
	{
		int stage_number = NSDS_GI(kSDS_GI_hellMode_int1_pieceNo_i, i+1);
		int card_number = NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, 1);
		int character_number = NSDS_GI(kSDS_CI_int1_characterNo_i, card_number);
		
		bool is_found = false;
		int history_size = mySGD->getCharacterHistorySize();
		for(int j=0;!is_found && j<history_size;j++)
		{
			CharacterHistory t_history = mySGD->getCharacterHistory(j);
			if(t_history.characterNo.getV() == character_number)
			{
				if(t_history.level.getV() > 0)
				{
					for(int j=0;j<list_cnt;j++)
					{
						if(history_list[j].m_number == character_number)
						{
							history_list[j].is_have = true;
							history_list[j].m_card = card_number;
							break;
						}
					}
				}
			}
		}
		
		
//		if(mySGD->isHasGottenCards(card_number))
//		{
//			int character_number = NSDS_GI(kSDS_CI_int1_characterNo_i, card_number);
//			
//			for(int j=0;j<list_cnt;j++)
//			{
//				if(history_list[j].m_number == character_number)
//				{
//					history_list[j].is_have = true;
//					history_list[j].m_card = card_number;
//					break;
//				}
//			}
//		}
	}
	
	selected_character_number = mySGD->getSelectedCharacterHistory().characterNo.getV();
	
	CCRect table_rect = CCRectMake(1, 3, 422, 198);
	
//	CCSprite* table_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_rect.size.width, table_rect.size.height));
//	table_back->setOpacity(100);
//	table_back->setAnchorPoint(ccp(0,0));
//	table_back->setPosition(table_rect.origin);
//	main_inner->addChild(table_back);
	
	character_table = CCTableView::create(this, table_rect.size);
	character_table->setDirection(CCScrollViewDirection::kCCScrollViewDirectionHorizontal);
	character_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	character_table->setPosition(table_rect.origin);
	main_inner->addChild(character_table);
	character_table->setTouchPriority(touch_priority);
	character_table->setDelegate(this);
	
	
	CommonAnimation::openPopup(this, main_case, NULL, [=](){
		
	}, [=]()
							   {
								   is_menu_enable = true;
							   });
	
	return true;
}

void CharacterSelectPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

CCTableViewCell* CharacterSelectPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	
	if(idx < list_cnt && history_list[idx].is_have.getV())
	{
		CCScale9Sprite* back_img;
		
		if(history_list[idx].m_number == selected_character_number)
		{
			back_img = CCScale9Sprite::create("cha_select.png", CCRectMake(0, 0, 60, 60), CCRectMake(29, 29, 2, 2));
			back_img->setContentSize(CCSizeMake(150, 198));
			back_img->setPosition(ccpFromSize(CCSizeMake(150, 198)/2.f));
			cell->addChild(back_img);
		}
		else
		{
			back_img = CCScale9Sprite::create("cha_unselect.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
			back_img->setContentSize(CCSizeMake(142, 190));
			back_img->setPosition(ccpFromSize(CCSizeMake(150, 198)/2.f));
			cell->addChild(back_img);
		}
		
		CCScale9Sprite* inner_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		inner_back->setContentSize(CCSizeMake(131, 120));
		inner_back->setPosition(ccpFromSize(back_img->getContentSize()/2.f) + ccp(0,10));
		back_img->addChild(inner_back);
		
		CCSprite* light_back = KS::loadCCBI<CCSprite*>(this, "hell_cha_back.ccbi").first;
		light_back->setPosition(ccpFromSize(inner_back->getContentSize()/2.f) + ccp(0,5));
		inner_back->addChild(light_back);
		
		CCSprite* character_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + history_list[idx].m_character + ".ccbi").first;
		character_img->setPosition(ccpFromSize(inner_back->getContentSize()/2.f) + ccp(0,5));
		inner_back->addChild(character_img);
		
		StoneType missile_type_code = StoneType(history_list[idx].m_number-1);
		missile_type_code = kStoneType_guided;
		
		if(missile_type_code == kStoneType_guided)
		{
			int grade = (history_list[idx].m_level-1)/5+1;
			bool rotation = false;
			if(grade == 1 || grade == 4)
				rotation = true;
			
			GuidedMissileForUpgradeWindow* t_gm = GuidedMissileForUpgradeWindow::createForShowWindow(CCString::createWithFormat("jack_missile_%02d_%02d.png", history_list[idx].m_number, history_list[idx].m_level)->getCString(),
																									 rotation);
			t_gm->beautifier((history_list[idx].m_level-1)/5+1, (history_list[idx].m_level-1)%5);
			//		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("me_guide%d.ccbi", (missile_level-1)%5 + 1)->getCString());
			t_gm->setPosition(character_img->getPosition());
			//		t_gm->beautifier((missile_level-1)/5+1, (missile_level-1)%5+1);
			t_gm->setScale(0.6f);
			inner_back->addChild(t_gm);
			
			
			t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
		}
		
		CCSprite* missile_back = CCSprite::create("cha_level.png");
		missile_back->setPosition(ccp(31, 101));
		inner_back->addChild(missile_back);
		
		KSLabelTTF* level_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_levelValue), history_list[idx].m_level), mySGD->getFont().c_str(), 11);
		level_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		level_label->setPosition(ccpFromSize(missile_back->getContentSize()/2.f) + ccp(0,7));
		missile_back->addChild(level_label);
		
		KSLabelTTF* damage_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_powerValue), KS::insert_separator(history_list[idx].m_damage).c_str()), mySGD->getFont().c_str(), 11);
		damage_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		damage_label->setPosition(ccpFromSize(missile_back->getContentSize()/2.f) + ccp(0,-9));
		missile_back->addChild(damage_label);
		
		CCSprite* name_back = CCSprite::create("cha_tab.png");
		name_back->setPosition(ccpFromSize(CCSizeMake(150, 198)/2.f) + ccp(0, 86));
		cell->addChild(name_back);
		
		KSLabelTTF* name_label = KSLabelTTF::create(history_list[idx].m_name.c_str(), mySGD->getFont().c_str(), 14);
		name_label->setPosition(ccpFromSize(name_back->getContentSize()/2.f) + ccp(0,0));
		name_back->addChild(name_label);
		
		if(history_list[idx].m_card != -1)
		{
//			CCClippingNode* t_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
//			t_clipping->setAlphaThreshold(0.1f);
//			
//			CCSprite* t_card = mySIL->getLoadedImg(ccsf("card%d_visible.png", history_list[idx].m_card));
//			t_clipping->addChild(t_card);
//			t_card->setScale(0.2f);
//			
//			t_clipping->setPosition(ccp(25,27));
//			t_clipping->setScale(0.5f);
//			inner_back->addChild(t_clipping);
//			
//			CCSprite* t_frame = CCSprite::create("hell_frame.png");
//			t_frame->setPosition(ccp(25,27));
//			inner_back->addChild(t_frame);
			
			StyledLabelTTF* comment_label = StyledLabelTTF::create(history_list[idx].m_comment.c_str(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
			comment_label->setAnchorPoint(ccp(0.5f,0.5f));
			comment_label->setPosition(ccp(inner_back->getContentSize().width/2.f,27));
			inner_back->addChild(comment_label);
		}
		
		if(history_list[idx].m_number == selected_character_number)
		{
			CCSprite* button_img = CCSprite::create("subbutton_purple3.png");
			button_img->setPosition(ccpFromSize(back_img->getContentSize()/2.f) + ccp(0,-71));
			back_img->addChild(button_img);
			
			KSLabelTTF* button_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_characterSelected), mySGD->getFont().c_str(), 14);
			button_label->enableOuterStroke(ccBLACK, 0.5f, 50, true);
			button_label->setPosition(ccpFromSize(button_img->getContentSize()/2.f));
			button_img->addChild(button_label);
		}
		else
		{
			CCSprite* n_button_img = CCSprite::create("subbutton_purple3.png");
			KSLabelTTF* n_button_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_characterSelect), mySGD->getFont().c_str(), 14);
			n_button_label->enableOuterStroke(ccBLACK, 0.5f, 50, true);
			n_button_label->setPosition(ccpFromSize(n_button_img->getContentSize()/2.f));
			n_button_img->addChild(n_button_label);
			
			CCSprite* s_button_img = CCSprite::create("subbutton_purple3.png");
			s_button_img->setColor(ccGRAY);
			KSLabelTTF* s_button_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_characterSelect), mySGD->getFont().c_str(), 14);
			s_button_label->enableOuterStroke(ccBLACK, 0.5f, 50, true);
			s_button_label->setPosition(ccpFromSize(s_button_img->getContentSize()/2.f));
			s_button_img->addChild(s_button_label);
			
			CCMenuItem* button_item = CCMenuItemSprite::create(n_button_img, s_button_img, this, menu_selector(CharacterSelectPopup::characterChangeAction));
			button_item->setTag(history_list[idx].m_number);
			
			ScrollMenu* button_menu = ScrollMenu::create(button_item, NULL);
			button_menu->setPosition(ccpFromSize(back_img->getContentSize()/2.f) + ccp(0,-71));
			back_img->addChild(button_menu);
			button_menu->setTouchPriority(touch_priority-1);
		}
	}
	else
	{
		CCSprite* not_have_img = CCSprite::create("cha_lock.png");
		not_have_img->setPosition(ccpFromSize(CCSizeMake(150, 198)/2.f));
		cell->addChild(not_have_img);
		
		StyledLabelTTF* ment_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_needHaveCharacterCard), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
		ment_label->setPosition(ccpFromSize(not_have_img->getContentSize()/2.f) + ccp(0,-5));
		not_have_img->addChild(ment_label);
		
//		back_img = CCScale9Sprite::create("cha_unselect.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
//		back_img->setContentSize(CCSizeMake(142, 190));
//		back_img->setPosition(ccpFromSize(CCSizeMake(150, 198)/2.f));
//		cell->addChild(back_img);
//		
//		CCScale9Sprite* inner_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
//		inner_back->setContentSize(CCSizeMake(131, 120));
//		inner_back->setPosition(ccpFromSize(back_img->getContentSize()/2.f) + ccp(0,10));
//		back_img->addChild(inner_back);
//		
//		KSLabelTTF* question_label = KSLabelTTF::create("?", mySGD->getFont().c_str(), 50);
//		question_label->setPosition(ccpFromSize(inner_back->getContentSize()/2.f));
//		inner_back->addChild(question_label);
	}
	
	return cell;
}

void CharacterSelectPopup::characterChangeAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	change_loading = LoadingLayer::create(touch_priority-100);
	addChild(change_loading);
	change_loading->startLoading();
	
	int t_tag = ((CCNode*)sender)->getTag();
	
	send_character_number = t_tag;
	
	mySGD->setUserdataSelectedCharNO(send_character_number);
	
	mySGD->changeUserdata(json_selector(this, CharacterSelectPopup::resultUpdateCharacterHistory));
	
//	mySGD->keep_character_history_callback = nullptr;
//	
//	Json::Value param;
//	param["memberID"] = hspConnector::get()->getSocialID();
//	param["characterNo"] = send_character_number;
//	
//	myHSP->command("updatecharacterhistory", param, json_selector(this, CharacterSelectPopup::resultUpdateCharacterHistory));
}

void CharacterSelectPopup::resultUpdateCharacterHistory(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
//		mySGD->resultUpdateCharacterHistory(result_data);
		
		selected_character_number = mySGD->getSelectedCharacterHistory().characterNo.getV();
		
		CCPoint t_offset = character_table->getContentOffset();
		character_table->reloadData();
		character_table->setContentOffset(t_offset);
		
		change_loading->removeFromParent();
		change_loading = NULL;
		
		is_menu_enable = true;
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
				mySGD->changeUserdata(json_selector(this, CharacterSelectPopup::resultUpdateCharacterHistory));
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 mySGD->changeUserdata(json_selector(this, CharacterSelectPopup::resultUpdateCharacterHistory));
									 }));
		}
	}
}

void CharacterSelectPopup::scrollViewDidScroll(CCScrollView* view)
{
	
}
void CharacterSelectPopup::scrollViewDidZoom(CCScrollView* view)
{
	
}
void CharacterSelectPopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLOG("cell touched!!");
}
CCSize CharacterSelectPopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(150, 198);
}
unsigned int CharacterSelectPopup::numberOfCellsInTableView(CCTableView *table)
{
	return list_cnt+5;
}