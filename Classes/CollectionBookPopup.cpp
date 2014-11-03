//
//  CollectionBookPopupPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#include "CollectionBookPopup.h"
#include "MyLocalization.h"
#include "CardSettingPopup.h"
#include "utf8.h"
//#include "DiaryZoom.h"
#include "LogData.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "PuzzleMapScene.h"
#include "DiaryZoomPopup.h"
#include "CommonButton.h"
#include "CardStrengthPopup.h"
#include "MainFlowScene.h"
#include "StageImgLoader.h"
#include <random>
#include "KSLabelTTF.h"
#include "CardViewScene.h"
#include "styledLabelTTF.h"
#include "CommonAnimation.h"
#include "FormSetter.h"
#include "TouchSuctionLayer.h"
#include "CardGiftPopup.h"
#include "ASPopupView.h"
#include "LoadingLayer.h"
#include "Diary19Popup.h"

enum CBP_Zorder{
	kCBP_Z_gray = 1,
	kCBP_Z_after,
	kCBP_Z_recent,
	kCBP_Z_cover
};

enum CBP_MenuTag{
	kCBP_MT_close = 1,
	kCBP_MT_zoom,
	kCBP_MT_pre,
	kCBP_MT_next,
	kCBP_MT_gift,
	kCBP_MT_diary19,
	kCBP_MT_takeCnt,
//	kCBP_MT_inputText,
	kCBP_MT_strength,
//	kCBP_MT_second,
//	kCBP_MT_third,
//	kCBP_MT_forth,
	kCBP_MT_cardBase = 10000
};

void CollectionBookPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

CCPoint CollectionBookPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kCBP_MT_zoom)		return_value = ccp(57, 57);
	else if(t_tag == kCBP_MT_pre)	return_value = ccp(30, 160);
	else if(t_tag == kCBP_MT_next)	return_value = ccp(210, 160);
//	else if(t_tag == kCBP_MT_second)	return_value = ccp(50,225);
//	else if(t_tag == kCBP_MT_third)	return_value = ccp(110,225);
//	else if(t_tag == kCBP_MT_forth)	return_value = ccp(170,225);
	else if(t_tag == kCBP_MT_close)	return_value = ccp(195, 282);
//	else if(t_tag == kCBP_MT_inputText)	return_value = ccp(112,95);
	else if(t_tag == kCBP_MT_strength)	return_value = ccp(55,40);
	
	return return_value;
}

void CollectionBookPopup::setRightPage(CCNode *target, int card_number)
{
	KSLabelTTF* r_stage_name = KSLabelTTF::create(NSDS_GS(kSDS_CI_int1_name_s, card_number).c_str(), mySGD->getFont().c_str(), 10);
	r_stage_name->disableOuterStroke();
	r_stage_name->setPosition(ccp(32,273));
	r_stage_name->setAnchorPoint(ccp(0,0.5f));
	target->addChild(r_stage_name);
	
//	CCSprite* script_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 175, 35));
//	script_back->setColor(ccGRAY);
//	script_back->setOpacity(100);
//	script_back->setAnchorPoint(ccp(0,1));
//	script_back->setPosition(ccp(25,261));
//	target->addChild(script_back);
	
	CCLabelTTF* r_stage_script = CCLabelTTF::create(NSDS_GS(kSDS_CI_int1_script_s, card_number).c_str(), mySGD->getFont().c_str(), 10, CCSizeMake(175, 35), kCCTextAlignmentLeft);
	r_stage_script->setPosition(ccp(25,253));
	r_stage_script->setColor(ccBLACK);
	r_stage_script->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	r_stage_script->setAnchorPoint(ccp(0,1));
	target->addChild(r_stage_script);
	
	
	KSLabelTTF* r_profile_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_profile), mySGD->getFont().c_str(), 10);
	r_profile_label->disableOuterStroke();
	r_profile_label->setPosition(ccp(32,208));
	r_profile_label->setAnchorPoint(ccp(0,0.5f));
	target->addChild(r_profile_label);
	
//	CCSprite* profile_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 175, 50));
//	profile_back->setColor(ccGRAY);
//	profile_back->setOpacity(100);
//	profile_back->setAnchorPoint(ccp(0,1));
//	profile_back->setPosition(ccp(25,157));
//	target->addChild(profile_back);
	
	CCLabelTTF* r_stage_profile = CCLabelTTF::create(NSDS_GS(kSDS_CI_int1_profile_s, card_number).c_str(), mySGD->getFont().c_str(), 10, CCSizeMake(175, 50), kCCTextAlignmentLeft);
	r_stage_profile->setPosition(ccp(25,189));
	r_stage_profile->setColor(ccBLACK);
	r_stage_profile->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	r_stage_profile->setAnchorPoint(ccp(0,1));
	target->addChild(r_stage_profile);
	
	
	KSLabelTTF* r_card_elemental_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardElemental), mySGD->getFont().c_str(), 10);
	r_card_elemental_label->disableOuterStroke();
	r_card_elemental_label->setPosition(ccp(32,111));
	r_card_elemental_label->setAnchorPoint(ccp(0,0.5f));
	target->addChild(r_card_elemental_label);
	
	string elemental_str, elemental_filename;
	int card_type = NSDS_GI(kSDS_CI_int1_type_i, card_number);
	
	if(card_type == 0)
	{
		elemental_str = "음";
		elemental_filename = "diary_icon_light.png";
	}
	else if(card_type == 1)
	{
		elemental_str = "양";
		elemental_filename = "diary_icon_shaded.png";
	}
	else if(card_type == 2)
	{
		elemental_str = "섬";
		elemental_filename = "diary_icon_sun.png";
	}
	else if(card_type == 3)
	{
		elemental_str = "음란";
		elemental_filename = "diary_icon_sun.png";
	}
	else if(card_type == 4)
	{
		elemental_str = "양란";
		elemental_filename = "diary_icon_sun.png";
	}
	else if(card_type == 5)
	{
		elemental_str = "섬란";
		elemental_filename = "diary_icon_sun.png";
	}
	
	KSLabelTTF* r_elemental = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_elementalValue), elemental_str.c_str()), mySGD->getFont().c_str(), 9);
	r_elemental->setColor(ccBLACK);
	r_elemental->setPosition(ccp(45,95));
	target->addChild(r_elemental);
	
	CCSprite* elemental_img = CCSprite::create(elemental_filename.c_str());
	elemental_img->setPosition(ccp(45,74));
	target->addChild(elemental_img);
	
	KSLabelTTF* r_touch = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_touch), mySGD->getFont().c_str(), 9);
	r_touch->setColor(ccBLACK);
	r_touch->setPosition(ccp(90,95));
	target->addChild(r_touch);
	
	if(mySGD->isCardMorphing(card_number))
	{
		CCSprite* touch_img = CCSprite::create("diary_icon_touch.png");
		touch_img->setPosition(ccp(90,74));
		target->addChild(touch_img);
	}
	else
	{
		CCSprite* not_touch_img = CCSprite::create("diary_icon_lock.png");
		not_touch_img->setPosition(ccp(90,74));
		target->addChild(not_touch_img);
	}
	
	if(NSDS_GI(kSDS_CI_int1_soundCnt_i, card_number) > 0)
	{
		KSLabelTTF* r_sound = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_sound), mySGD->getFont().c_str(), 9);
		r_sound->setColor(ccBLACK);
		r_sound->setPosition(ccp(135,95));
		target->addChild(r_sound);
		
		if(mySGD->isCardMorphing(card_number))
		{
			CCSprite* sound_img = CCSprite::create("diary_icon_sound.png");
			sound_img->setPosition(ccp(135,74));
			target->addChild(sound_img);
		}
		else
		{
			CCSprite* not_sound_img = CCSprite::create("diary_icon_lock.png");
			not_sound_img->setPosition(ccp(135,74));
			target->addChild(not_sound_img);
		}
	}
	
//	if(NSDS_GB(kSDS_CI_int1_haveAdult_b, card_number))
//	{
//		CCSprite* n_diary_19 = CCSprite::create("diary_19.png");
//		CCSprite* s_diary_19 = CCSprite::create("diary_19.png");
//		s_diary_19->setColor(ccGRAY);
//		
//		CCMenuItem* diary_19_item = CCMenuItemSprite::create(n_diary_19, s_diary_19, this, menu_selector(CollectionBookPopup::menuAction));
//		diary_19_item->setTag(kCBP_MT_diary19);
//		
//		CCMenu* diary_19_menu = CCMenu::createWithItem(diary_19_item);
//		diary_19_menu->setPosition(ccp(185,84));
//		target->addChild(diary_19_menu, 1, kCBP_MT_diary19);
//		diary_19_menu->setTouchPriority(-191);
//	}
	
	CommonButton* close = CommonButton::createCloseButton(-191);
	close->setFunction([=](CCObject* sender)
					   {
						   CCNode* t_node = CCNode::create();
						   t_node->setTag(kCBP_MT_close);
						   menuAction(t_node);
					   });
	close->setPosition(getContentPosition(kCBP_MT_close));
	target->addChild(close, 1, kCBP_MT_close);
	
	CommonButton* gift = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_cardGiftTitle), 9, CCSizeMake(80,32), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), -191);
	KSLabelTTF* gift_label = gift->getTitleLabel();
	gift_label->disableOuterStroke();
	gift->setFunction([=](CCObject* sender)
					  {
						  if(!is_menu_enable)
							  return;
						  
						  is_menu_enable = false;
						  
//						  addChild(ASPopupView::getCommonNoti(-300, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_afterOpenCBT), [=](){is_menu_enable = true;}), 999);;
						  
						  CardGiftPopup* t_popup = CardGiftPopup::create(-300, card_number, [=]()
							{
								is_menu_enable = true;
								CardSortInfo t_card_info = mySGD->getHasGottenCardDataForCardNumber(card_number);
								gift->setVisible(t_card_info.count.getV() >= 2);
								((KSLabelTTF*)recent_left_img->getChildByTag(kCBP_MT_takeCnt))->setString(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_cardTakeCnt), t_card_info.count.getV()));
							}, [=]()
						  {
							  is_menu_enable = true;
							  CCNode* t_node = CCNode::create();
							  t_node->setTag(kCBP_MT_close);
							  menuAction(t_node);
						  });
						  addChild(t_popup, 999);
					  });
	gift->setPosition(ccp(115,37));
	target->addChild(gift, 1, kCBP_MT_gift);
	
	CardSortInfo t_card_info = mySGD->getHasGottenCardDataForCardNumber(card_number);
	gift->setVisible(t_card_info.count.getV() >= 2);
	
//	CCSprite* n_zoom = CCSprite::create("diary_zoom.png");
//	CCSprite* s_zoom = CCSprite::create("diary_zoom.png");
//	s_zoom->setColor(ccGRAY);
//	
//	CCMenuItem* zoom_item = CCMenuItemSprite::create(n_zoom, s_zoom, this, menu_selector(CollectionBookPopup::menuAction));
//	zoom_item->setTag(kCBP_MT_zoom);
//	
//	CCMenu* zoom_menu = CCMenu::createWithItem(zoom_item);
//	zoom_menu->setPosition(getContentPosition(kCBP_MT_zoom));
//	target->addChild(zoom_menu, 1, kCBP_MT_zoom);
//	zoom_menu->setTouchPriority(-191);
	
	
//	float mul_value = 0.88f;
//	
//	int position_index = 1;
//	
//	random_device rd;
//	default_random_engine e1(rd());
//	uniform_int_distribution<int> uniform_dist(-10, 10);
//	uniform_int_distribution<int> uniform_dist_x(8, 82);
//	uniform_int_distribution<int> uniform_dist_y(8, 112);
//	uniform_int_distribution<int> uniform_dist_cnt(1, 2);
//	uniform_int_distribution<int> uniform_dist_type(1, 3);
//	uniform_int_distribution<int> uniform_dist_rotate(-90,90);
//	
//	vector<int> card_set = mySGD->getCollectionCardSet(card_number);
//	
//	for(int j=0;j<card_set.size();j++)
//	{
//		if(card_set[j] == card_number)
//			continue;
//		
//		int rotation_value;
//		CCPoint position_value;
//		
//		if(position_index == 1)
//		{
//			rotation_value = uniform_dist(e1);
//			position_value = ccpAdd(getContentPosition(kCBP_MT_second), ccp(0,uniform_dist(e1)));
//		}
//		else if(position_index == 2)
//		{
//			rotation_value = uniform_dist(e1);
//			position_value = ccpAdd(getContentPosition(kCBP_MT_third), ccp(0,uniform_dist(e1)));
//		}
//		else
//		{
//			rotation_value = uniform_dist(e1);
//			position_value = ccpAdd(getContentPosition(kCBP_MT_forth), ccp(0,uniform_dist(e1)));
//		}
//		
//		if(card_set[j] > 0)
//		{
//			CCSprite* second_img = mySIL->getLoadedImg(CCString::createWithFormat("card%d_thumbnail.png", card_set[j])->getCString());
//            second_img->setScale(mul_value);
//			second_img->setRotation(rotation_value);
//            second_img->setPosition(position_value);
//            target->addChild(second_img);
//			
//			CCSprite* img_case = CCSprite::create("diary_frame.png");
//			img_case->setPosition(ccp(second_img->getContentSize().width/2.f, second_img->getContentSize().height/2.f-4.f));
//			second_img->addChild(img_case);
//			
//			int tape_cnt = uniform_dist_cnt(e1);
//			for(int j=0;j<tape_cnt;j++)
//			{
//				int tape_type = uniform_dist_type(e1);
//				CCSprite* tape_img = CCSprite::create(CCString::createWithFormat("diary_tape%d.png", tape_type)->getCString());
//				bool is_x = uniform_dist_cnt(e1) == 1;
//				bool is_first = uniform_dist_cnt(e1) == 1;
//				if(is_x)
//				{
//					if(is_first)
//						tape_img->setPosition(ccp(uniform_dist_x(e1), 8));
//					else
//						tape_img->setPosition(ccp(uniform_dist_x(e1), 112));
//				}
//				else
//				{
//					if(is_first)
//						tape_img->setPosition(ccp(8, uniform_dist_y(e1)));
//					else
//						tape_img->setPosition(ccp(82, uniform_dist_y(e1)));
//				}
//				img_case->addChild(tape_img);
//				tape_img->setRotation(uniform_dist_rotate(e1));
//			}
//			
//			CCMenuItem* second_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionBookPopup::menuAction));
//			second_item->setTag(kCBP_MT_cardBase + card_set[j]);
//			second_item->setRotation(rotation_value);
//			
//			CCMenu* second_menu = CCMenu::createWithItem(second_item);
//			second_menu->setPosition(second_img->getPosition());
//			target->addChild(second_menu, 1, kCBP_MT_second+position_index-1);
//			second_menu->setTouchPriority(-191);
//		}
//		else if(card_set[j] == -1)
//		{
//			CCSprite* no_img = CCSprite::create("diary_nophoto.png");
//			
//			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_diaryNoImg), mySGD->getFont().c_str(), 8);
//			no_label->disableOuterStroke();
//			no_label->setColor(ccc3(60, 60, 60));
//			no_label->setPosition(ccp(no_img->getContentSize().width/2.f, no_img->getContentSize().height/2.f));
//			no_img->addChild(no_label);
//			
//			no_img->setScale(mul_value);
//			no_img->setRotation(rotation_value);
//            no_img->setPosition(position_value);
//            target->addChild(no_img);
//			
//			CCSprite* img_case = CCSprite::create("diary_frame.png");
//			img_case->setPosition(ccp(no_img->getContentSize().width/2.f, no_img->getContentSize().height/2.f-4.f));
//			no_img->addChild(img_case);
//			
//			int tape_cnt = uniform_dist_cnt(e1);
//			for(int j=0;j<tape_cnt;j++)
//			{
//				int tape_type = uniform_dist_type(e1);
//				CCSprite* tape_img = CCSprite::create(CCString::createWithFormat("diary_tape%d.png", tape_type)->getCString());
//				bool is_x = uniform_dist_cnt(e1) == 1;
//				bool is_first = uniform_dist_cnt(e1) == 1;
//				if(is_x)
//				{
//					if(is_first)
//						tape_img->setPosition(ccp(uniform_dist_x(e1), 8));
//					else
//						tape_img->setPosition(ccp(uniform_dist_x(e1), 112));
//				}
//				else
//				{
//					if(is_first)
//						tape_img->setPosition(ccp(8, uniform_dist_y(e1)));
//					else
//						tape_img->setPosition(ccp(82, uniform_dist_y(e1)));
//				}
//				img_case->addChild(tape_img);
//				tape_img->setRotation(uniform_dist_rotate(e1));
//			}
//		}
//		position_index++;
//	}
	
//	if(card_set[0] == -2)
//	{
//		int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
//		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
//		
//		CCLabelTTF* r_stage_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_puzzleValue), puzzle_number)->getCString(), mySGD->getFont().c_str(), 8);
//		r_stage_label->setAnchorPoint(ccp(0,0.5f));
//		r_stage_label->setPosition(ccp(138, 287));
//		r_stage_label->setColor(ccBLACK);
//		r_stage_label->setHorizontalAlignment(kCCTextAlignmentCenter);
//		r_stage_label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
//		target->addChild(r_stage_label);
//	}
//	else
//	{
	
	string card_category = NSDS_GS(kSDS_CI_int1_category_s, card_number);
	if(card_category == "")
		card_category = "normal";
	if(card_category == "nPuzzle" || card_category == "ePuzzle")
	{
//		int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
//		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
//		
//		CCLabelTTF* r_stage_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_puzzleValue), puzzle_number)->getCString(), mySGD->getFont().c_str(), 8);
//		r_stage_label->setAnchorPoint(ccp(1,0.5f));
//		r_stage_label->setPosition(ccp(167, 288));
//		r_stage_label->setColor(ccBLACK);
//		target->addChild(r_stage_label);
	}
	else
	{
		int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
		
		CCLabelTTF* r_stage_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_stageValue), stage_number)->getCString(), mySGD->getFont().c_str(), 8);
		r_stage_label->setAnchorPoint(ccp(1,0.5f));
		r_stage_label->setPosition(ccp(167, 288));
		r_stage_label->setColor(ccBLACK);
		target->addChild(r_stage_label);
	}
//	}
}

void CollectionBookPopup::setLeftPage(CCNode *target, int card_number)
{
	KSLabelTTF* take_cnt_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_cardTakeCnt), mySGD->getHasGottenCardDataForCardNumber(card_number).count.getV()), mySGD->getFont().c_str(), 9);
	take_cnt_label->setColor(ccBLACK);
	take_cnt_label->disableOuterStroke();
	take_cnt_label->setAnchorPoint(ccp(0,0.5f));
	take_cnt_label->setPosition(ccp(38,28));
	target->addChild(take_cnt_label, 0, kCBP_MT_takeCnt);
	
	CCSprite* r_card_img = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png", card_number)->getCString());
	r_card_img->setScale(0.57);
	r_card_img->setPosition(ccp(129,161));
	target->addChild(r_card_img);
	
	if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, card_number))
	{
		CollectionAnimation* t_ca = CollectionAnimation::create(card_number);
		r_card_img->addChild(t_ca);
	}
	
//	if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, card_number))
//	{
//		CCSprite* ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, card_number)).first;
//		ccb_img->setPosition(ccp(160,215));
//		r_card_img->addChild(ccb_img);
//	}
	
	CCSprite* left_top_paper = CCSprite::create("diary_paper.png");
	left_top_paper->setPosition(ccp(44,276));
	target->addChild(left_top_paper);
	
	CCSprite* right_bottom_paper = CCSprite::create("diary_paper.png");
	right_bottom_paper->setRotation(180);
	right_bottom_paper->setPosition(ccp(213,44));
	target->addChild(right_bottom_paper);
	
	KSLabelTTF* card_number_label = KSLabelTTF::create(ccsf("No.%d", card_number), mySGD->getFont().c_str(), 9);
	card_number_label->setColor(ccBLACK);
	card_number_label->disableOuterStroke();
	card_number_label->setAnchorPoint(ccp(0,0.5f));
	card_number_label->setPosition(ccp(38,294));
	target->addChild(card_number_label);
	
	CCSprite* n_zoom = CCSprite::create("diary_zoom.png");
	CCSprite* s_zoom = CCSprite::create("diary_zoom.png");
	s_zoom->setColor(ccGRAY);
	
	CCMenuItem* zoom_item = CCMenuItemSprite::create(n_zoom, s_zoom, this, menu_selector(CollectionBookPopup::menuAction));
	zoom_item->setTag(kCBP_MT_zoom);
	
	CCMenu* zoom_menu = CCMenu::createWithItem(zoom_item);
	zoom_menu->setPosition(getContentPosition(kCBP_MT_zoom));
	target->addChild(zoom_menu, 1, kCBP_MT_zoom);
	zoom_menu->setTouchPriority(-191);
	
	page_cnt++;
	if(page_cnt > 8)
	{
		CCTextureCache::sharedTextureCache()->removeUnusedTextures();
		page_cnt = 0;
	}
}

// on "init" you need to initialize your instance
bool CollectionBookPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	startFormSetter(this);
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	page_cnt = 0;
	
	is_menu_enable = false;
	
	mySGD->initCollectionBook();
	
//	mySGD->changeSortType(kCST_default);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCBP_Z_gray);
	
	main_case = CCNode::create();
	main_case->setPosition(ccp(240, 160));
	setFormSetter(main_case);
	addChild(main_case, kCBP_Z_gray);
	
	CCSprite* diary_case = CCSprite::create("diary_back_cover.png");
	diary_case->setPosition(ccp(0, 0));
	main_case->addChild(diary_case);
	
	
	recent_card_number = mySGD->selected_collectionbook;
	
	int next_number = mySGD->getNextStageCardNumber(recent_card_number);
	int pre_number = mySGD->getPreStageCardNumber(recent_card_number);
	
	if(next_number < 0 || pre_number < 0)
		is_enable_pageturn = false;
	else
		is_enable_pageturn = true;
	
	recent_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	recent_left_img->setAnchorPoint(ccp(1.f,0.5f));
	recent_left_img->setPosition(ccp(0, 0));
	main_case->addChild(recent_left_img, kCBP_Z_recent);
	
	setLeftPage(recent_left_img, recent_card_number);
	
	
	recent_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	recent_right_img->setAnchorPoint(ccp(0.f,0.5f));
	recent_right_img->setPosition(ccp(0, 0));
	main_case->addChild(recent_right_img, kCBP_Z_recent);
	
	setRightPage(recent_right_img, recent_card_number);
	
//	string input_data = mySGD->getCardComment(recent_card_number);
//    if(input_data == "")
//        input_data = myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease);
//    
//    input_text = CCEditBox::create(CCSizeMake(190, 50), CCScale9Sprite::create("diary_text_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1)));
//	input_text->setPosition(getContentPosition(kCBP_MT_inputText));
//	input_text->setPlaceHolder(input_data.c_str());
//	input_text->setReturnType(kKeyboardReturnTypeDone);
//	input_text->setFont(mySGD->getFont().c_str(), 12);
//	input_text->setFontColor(ccBLACK);
//	input_text->setDelegate(this);
//	input_text->setTouchPriority(-191);
//	
//    recent_right_img->addChild(input_text, kCBP_Z_cover);
//	
//    if(input_data != myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease))
//        input_text->setText(input_data.c_str());
	
	if(is_enable_pageturn)
	{
//		CCSprite* n_pre = CCSprite::create("diary_left.png");
//		CCSprite* s_pre = CCSprite::create("diary_left.png");
//		s_pre->setColor(ccGRAY);
//		
//		CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBookPopup::menuAction));
//		r_pre_item->setTag(kCBP_MT_pre);
//		
//		CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
//		r_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
//		recent_right_img->addChild(r_pre_menu, 1, kCBP_MT_pre);
//		r_pre_menu->setTouchPriority(-191);
		
		CCSprite* n_pre = CCSprite::create("diary_left.png");
		CCSprite* s_pre = CCSprite::create("diary_left.png");
		s_pre->setColor(ccGRAY);
		
		CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBookPopup::menuAction));
		r_pre_item->setTag(kCBP_MT_pre);
		
		CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
		r_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
		recent_left_img->addChild(r_pre_menu, 1, kCBP_MT_pre);
		r_pre_menu->setTouchPriority(-191);
		
		CCSprite* n_next = CCSprite::create("diary_right.png");
		CCSprite* s_next = CCSprite::create("diary_right.png");
		s_next->setColor(ccGRAY);
		
		CCMenuItem* r_next_item = CCMenuItemSprite::create(n_next, s_next, this, menu_selector(CollectionBookPopup::menuAction));
		r_next_item->setTag(kCBP_MT_next);
		
		CCMenu* r_next_menu = CCMenu::createWithItem(r_next_item);
		r_next_menu->setPosition(getContentPosition(kCBP_MT_next));
		recent_right_img->addChild(r_next_menu, 1, kCBP_MT_next);
		r_next_menu->setTouchPriority(-191);
		
		
		
		after_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
		after_left_img->setAnchorPoint(ccp(1.f,0.5f));
		after_left_img->setPosition(ccp(0, 0));
		main_case->addChild(after_left_img, kCBP_Z_after);
		
		setLeftPage(after_left_img, pre_number);
		
		((CCMenu*)after_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
		
		
		after_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
		after_right_img->setAnchorPoint(ccp(0.f,0.5f));
		after_right_img->setPosition(ccp(0, 0));
		main_case->addChild(after_right_img, kCBP_Z_after);
		
		setRightPage(after_right_img, next_number);
		
		((CommonButton*)after_right_img->getChildByTag(kCBP_MT_close))->setEnabled(false);
		((CommonButton*)after_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(false);
		if((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))
			((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(false);
//		((CCMenu*)after_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
		
//		int sub_count = after_right_img->getTag();
//		for(int i=0;i<sub_count;i++)
//		{
//			if(after_right_img->getChildByTag(kCBP_MT_second+i))
//				((CCMenu*)after_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(false);
//		}
		
//		CCSprite* a_n_pre = CCSprite::create("diary_left.png");
//		CCSprite* a_s_pre = CCSprite::create("diary_left.png");
//		a_s_pre->setColor(ccGRAY);
//		
//		CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBookPopup::menuAction));
//		a_pre_item->setTag(kCBP_MT_pre);
//		
//		CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
//		a_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
//		a_pre_menu->setEnabled(false);
//		after_right_img->addChild(a_pre_menu, 1, kCBP_MT_pre);
//		a_pre_menu->setTouchPriority(-191);
		
		CCSprite* a_n_pre = CCSprite::create("diary_left.png");
		CCSprite* a_s_pre = CCSprite::create("diary_left.png");
		a_s_pre->setColor(ccGRAY);
		
		CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBookPopup::menuAction));
		a_pre_item->setTag(kCBP_MT_pre);
		
		CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
		a_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
		a_pre_menu->setEnabled(false);
		after_left_img->addChild(a_pre_menu, 1, kCBP_MT_pre);
		a_pre_menu->setTouchPriority(-191);
		
		
		CCSprite* a_n_next = CCSprite::create("diary_right.png");
		CCSprite* a_s_next = CCSprite::create("diary_right.png");
		a_s_next->setColor(ccGRAY);
		
		CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBookPopup::menuAction));
		a_next_item->setTag(kCBP_MT_next);
		
		CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
		a_next_menu->setPosition(getContentPosition(kCBP_MT_next));
		a_next_menu->setEnabled(false);
		after_right_img->addChild(a_next_menu, 1, kCBP_MT_next);
		a_next_menu->setTouchPriority(-191);
	}
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-189);
	addChild(t_suction);
	t_suction->setTouchEnabled(true);
	
    return true;
}

void CollectionBookPopup::editBoxEditingDidBegin(CCEditBox* editBox)
{
	CCLOG("edit begin");
}
void CollectionBookPopup::editBoxEditingDidEnd(CCEditBox* editBox)
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["cardNo"] = recent_card_number;
	param["comment"] = editBox->getText();
	
	hspConnector::get()->command("updateCardHistory", param, nullptr);
	mySGD->setCardComment(recent_card_number, editBox->getText());
	CCLOG("edit end");
}
void CollectionBookPopup::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	CCLOG("edit changed : %s", text.c_str());
}
void CollectionBookPopup::editBoxReturn(CCEditBox* editBox)
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["cardNo"] = recent_card_number;
	param["comment"] = editBox->getText();
	
	hspConnector::get()->command("updateCardHistory", param, nullptr);
	mySGD->setCardComment(recent_card_number, editBox->getText());
	CCLOG("edit return");
}

void CollectionBookPopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void CollectionBookPopup::showPopup()
{
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, bind(&CollectionBookPopup::endShowPopup, this));
}

void CollectionBookPopup::endShowPopup()
{
	is_menu_enable = true;
	
	if(is_enable_pageturn)
	{
		is_touch_enable = true;
		setTouchEnabled(true);
	}
	else
	{
		is_touch_enable = false;
		setTouchEnabled(true);
	}
}

void CollectionBookPopup::hidePopup()
{
	is_menu_enable = false;
	
	
	CommonAnimation::closePopup(this, main_case, gray, [=](){
		
	}, bind(&CollectionBookPopup::endHidePopup, this));
}

void CollectionBookPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

bool CollectionBookPopup::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
	location = ccpSub(location, myDSH->ui_touch_convert);
	
	if(!is_touch_enable)
		return true;
	
	begin_point = location;
	
	if(begin_point.x > 240)
	{
		touch_direction = 1;
	}
	else
	{
		touch_direction = -1;
	}
	
	is_menu_enable = false;
	
	return true;
}

void CollectionBookPopup::startNextPageFull()
{
	setTouchEnabled(false);
	is_touch_enable = false;
	animation_angle = 0;
	animation_img = recent_right_img;
	touch_direction = 1;
	touch_end_direction = -1;
	end_animation_delegate = callfunc_selector(CollectionBookPopup::startNextPage);
	
	schedule(schedule_selector(CollectionBookPopup::ingPageFull));
}

void CollectionBookPopup::startPrePageFull()
{
	setTouchEnabled(false);
	is_touch_enable = false;
	animation_angle = 0;
	animation_img = recent_left_img;
	touch_direction = -1;
	touch_end_direction = -1;
	end_animation_delegate = callfunc_selector(CollectionBookPopup::startPrePage);
	
	schedule(schedule_selector(CollectionBookPopup::ingPageFull));
}

void CollectionBookPopup::ingPageFull()
{
	animation_angle += 3.f;
	
	animation_img->setSkewY(-touch_direction*touch_end_direction*animation_angle/4.f);
	animation_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	if(animation_angle >= 90.f)
	{
		unschedule(schedule_selector(CollectionBookPopup::ingPageFull));
		animation_img = NULL;
		(this->*end_animation_delegate)();
	}
}

void CollectionBookPopup::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touch_enable)
		return;
	
	//	if(was_open_text)	return;
	
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
	location = ccpSub(location, myDSH->ui_touch_convert);
	
	float x_distance = begin_point.x - location.x;
	
	if(touch_direction == 1)
	{
		if(x_distance < 0)
			return;
		
		animation_angle = 9.f/10.f*x_distance;
		
		if(animation_angle > 90)
		{
			setTouchEnabled(false);
			is_touch_enable = false;
			startNextPage();
			return;
		}
		
		float skewY_value = animation_angle / 4.f;
		float scaleX_value = cosf(animation_angle/180.f*M_PI);
		
		recent_right_img->setSkewY(skewY_value);
		recent_right_img->setScaleX(scaleX_value);
		
	}
	else if(touch_direction == -1)
	{
		if(x_distance > 0)
			return;
		x_distance *= -1.f;
		
		animation_angle = 9.f/10.f*x_distance;
		
		if(animation_angle > 90)
		{
			setTouchEnabled(false);
			is_touch_enable = false;
			startPrePage();
			return;
		}
		
		float skewY_value = -animation_angle / 4.f;
		float scaleX_value = cosf(animation_angle/180.f*M_PI);
		
		recent_left_img->setSkewY(skewY_value);
		recent_left_img->setScaleX(scaleX_value);
	}
}

void CollectionBookPopup::startNextPage()
{
	recent_right_img->removeFromParent();
	recent_right_img = after_right_img;
	after_right_img = NULL;
	
	reorderChild(recent_right_img, kCBP_Z_recent);
	((CommonButton*)recent_right_img->getChildByTag(kCBP_MT_close))->setEnabled(true);
	((CommonButton*)recent_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(true);
	if((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(true);
//	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(true);
//	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_pre))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_next))->setEnabled(true);
	
//	int sub_count = recent_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(recent_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(true);
//	}
	
	((CCMenu*)recent_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	((CCMenu*)recent_left_img->getChildByTag(kCBP_MT_pre))->setEnabled(false);
	
	covered_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	covered_left_img->setAnchorPoint(ccp(1.f,0.5f));
	covered_left_img->setPosition(ccp(0, 0));
	main_case->addChild(covered_left_img, kCBP_Z_cover);
	setFormSetter(main_case);
	int next_number = mySGD->getNextStageCardNumber(recent_card_number);
	recent_card_number = next_number;
	
//	string input_data = mySGD->getCardComment(recent_card_number);
//    if(input_data == "")
//        input_data = myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease);
//    
//    input_text = CCEditBox::create(CCSizeMake(190, 50), CCScale9Sprite::create("diary_text_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1)));
//	input_text->setPosition(getContentPosition(kCBP_MT_inputText));
//	input_text->setPlaceHolder(input_data.c_str());
//	input_text->setReturnType(kKeyboardReturnTypeDone);
//	input_text->setFont(mySGD->getFont().c_str(), 12);
//	input_text->setFontColor(ccBLACK);
//	input_text->setDelegate(this);
//	recent_right_img->addChild(input_text, kCBP_Z_cover);
//	input_text->setTouchPriority(-191);
//    
//    if(input_data != myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease))
//        input_text->setText(input_data.c_str());
	
	setLeftPage(covered_left_img, next_number);
	
	CCSprite* n_pre = CCSprite::create("diary_left.png");
	CCSprite* s_pre = CCSprite::create("diary_left.png");
	s_pre->setColor(ccGRAY);
	
	CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBookPopup::menuAction));
	r_pre_item->setTag(kCBP_MT_pre);
	
	CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
	r_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
	covered_left_img->addChild(r_pre_menu, 1, kCBP_MT_pre);
	r_pre_menu->setTouchPriority(-191);
	
	
	animation_angle = 90.f;
	
	covered_left_img->setSkewY(animation_angle/4.f);
	covered_left_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	animation_img = covered_left_img;
	end_animation_delegate = callfunc_selector(CollectionBookPopup::endNextPage);
	
	touch_end_direction = 1;
	
	schedule(schedule_selector(CollectionBookPopup::ingPage));
}

void CollectionBookPopup::startNextFullSelectedPage()
{
	setTouchEnabled(false);
	is_touch_enable = false;
	
	after_right_img->removeFromParent();
	after_right_img = NULL;
	
	after_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	after_right_img->setAnchorPoint(ccp(0.f,0.5f));
	after_right_img->setPosition(ccp(0,0 ));
	main_case->addChild(after_right_img, kCBP_Z_after);
	
	setRightPage(after_right_img, mySGD->selected_collectionbook);
	
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_close))->setEnabled(false);
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(false);
	if((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(false);
//	((CCMenu*)after_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	
//	int sub_count = after_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(after_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)after_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(false);
//	}
	
//	CCSprite* a_n_pre = CCSprite::create("diary_left.png");
//	CCSprite* a_s_pre = CCSprite::create("diary_left.png");
//	a_s_pre->setColor(ccGRAY);
//	
//	CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBookPopup::menuAction));
//	a_pre_item->setTag(kCBP_MT_pre);
//	
//	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
//	a_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
//	a_pre_menu->setEnabled(false);
//	after_right_img->addChild(a_pre_menu, 1, kCBP_MT_pre);
//	a_pre_menu->setTouchPriority(-191);
	
	CCSprite* a_n_next = CCSprite::create("diary_right.png");
	CCSprite* a_s_next = CCSprite::create("diary_right.png");
	a_s_next->setColor(ccGRAY);
	
	CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBookPopup::menuAction));
	a_next_item->setTag(kCBP_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(getContentPosition(kCBP_MT_next));
	a_next_menu->setEnabled(false);
	after_right_img->addChild(a_next_menu, 1, kCBP_MT_next);
	a_next_menu->setTouchPriority(-191);
	
	
	animation_angle = 0;
	animation_img = recent_right_img;
	touch_direction = 1;
	touch_end_direction = -1;
	end_animation_delegate = callfunc_selector(CollectionBookPopup::startNextSelectedPage);
	
	schedule(schedule_selector(CollectionBookPopup::ingPageFull));
}

void CollectionBookPopup::startPreFullSelectedPage()
{
	setTouchEnabled(false);
	is_touch_enable = false;
	
	after_left_img->removeFromParent();
	after_left_img = NULL;
	
	after_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	after_left_img->setAnchorPoint(ccp(1.f,0.5f));
	after_left_img->setPosition(ccp(0,0));
	main_case->addChild(after_left_img, kCBP_Z_after);
	
	setLeftPage(after_left_img, mySGD->selected_collectionbook);
	
	CCSprite* n_pre = CCSprite::create("diary_left.png");
	CCSprite* s_pre = CCSprite::create("diary_left.png");
	s_pre->setColor(ccGRAY);
	
	CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBookPopup::menuAction));
	r_pre_item->setTag(kCBP_MT_pre);
	
	CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
	r_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
	after_left_img->addChild(r_pre_menu, 1, kCBP_MT_pre);
	r_pre_menu->setTouchPriority(-191);
	
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_pre))->setEnabled(false);
	
	animation_angle = 0;
	animation_img = recent_left_img;
	touch_direction = -1;
	touch_end_direction = -1;
	end_animation_delegate = callfunc_selector(CollectionBookPopup::startPreSelectedPage);
	
	schedule(schedule_selector(CollectionBookPopup::ingPageFull));
}

void CollectionBookPopup::startPreSelectedPage()
{
	recent_left_img->removeFromParent();
	recent_left_img = after_left_img;
	after_left_img = NULL;
	
	reorderChild(recent_left_img, kCBP_Z_recent);
	((CCMenu*)recent_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(true);
	((CCMenu*)recent_left_img->getChildByTag(kCBP_MT_pre))->setEnabled(true);
	
	recent_card_number = mySGD->selected_collectionbook;
	
	covered_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	covered_right_img->setAnchorPoint(ccp(0.f,0.5f));
	covered_right_img->setPosition(ccp(0,0));
	main_case->addChild(covered_right_img, kCBP_Z_cover);
	
	setRightPage(covered_right_img, recent_card_number);
	
//	string input_data = mySGD->getCardComment(recent_card_number);
//    if(input_data == "")
//        input_data = myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease);
//    
//    input_text = CCEditBox::create(CCSizeMake(190, 50), CCScale9Sprite::create("diary_text_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1)));
//	input_text->setPosition(getContentPosition(kCBP_MT_inputText));
//	input_text->setPlaceHolder(input_data.c_str());
//	input_text->setReturnType(kKeyboardReturnTypeDone);
//	input_text->setFont(mySGD->getFont().c_str(), 12);
//	input_text->setFontColor(ccBLACK);
//	input_text->setDelegate(this);
//	covered_right_img->addChild(input_text, kCBP_Z_cover);
//	input_text->setTouchPriority(-191);
//    
//    if(input_data != myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease))
//        input_text->setText(input_data.c_str());
	
	((CommonButton*)covered_right_img->getChildByTag(kCBP_MT_close))->setEnabled(false);
	((CommonButton*)covered_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(false);
	if((CCMenu*)covered_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)covered_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(false);
//	((CCMenu*)covered_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	
//	int sub_count = covered_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(covered_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)covered_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(false);
//	}
	
//	CCSprite* a_n_pre = CCSprite::create("diary_left.png");
//	CCSprite* a_s_pre = CCSprite::create("diary_left.png");
//	a_s_pre->setColor(ccGRAY);
//	
//	CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBookPopup::menuAction));
//	a_pre_item->setTag(kCBP_MT_pre);
//	
//	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
//	a_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
//	a_pre_menu->setEnabled(false);
//	covered_right_img->addChild(a_pre_menu, 1, kCBP_MT_pre);
//	a_pre_menu->setTouchPriority(-191);
	
	CCSprite* a_n_next = CCSprite::create("diary_right.png");
	CCSprite* a_s_next = CCSprite::create("diary_right.png");
	a_s_next->setColor(ccGRAY);
	
	CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBookPopup::menuAction));
	a_next_item->setTag(kCBP_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(getContentPosition(kCBP_MT_next));
	a_next_menu->setEnabled(false);
	covered_right_img->addChild(a_next_menu, 1, kCBP_MT_next);
	a_next_menu->setTouchPriority(-191);
	
	
	animation_angle = 90.f;
	
	covered_right_img->setSkewY(animation_angle/4.f);
	covered_right_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	animation_img = covered_right_img;
	end_animation_delegate = callfunc_selector(CollectionBookPopup::endPreSelectedPage);
	
	touch_end_direction = 1;
	
	schedule(schedule_selector(CollectionBookPopup::ingPage));
}

void CollectionBookPopup::startNextSelectedPage()
{
	recent_right_img->removeFromParent();
	recent_right_img = after_right_img;
	after_right_img = NULL;
	
	reorderChild(recent_right_img, kCBP_Z_recent);
	((CommonButton*)recent_right_img->getChildByTag(kCBP_MT_close))->setEnabled(true);
	((CommonButton*)recent_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(true);
	if((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(true);
//	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(true);
//	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_pre))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_next))->setEnabled(true);
	
//	int sub_count = recent_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(recent_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(true);
//	}
	
	((CCMenu*)recent_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	((CCMenu*)recent_left_img->getChildByTag(kCBP_MT_pre))->setEnabled(false);
	
	covered_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	covered_left_img->setAnchorPoint(ccp(1.f,0.5f));
	covered_left_img->setPosition(ccp(0,0));
	main_case->addChild(covered_left_img, kCBP_Z_cover);
	
	recent_card_number = mySGD->selected_collectionbook;
	
//	string input_data = mySGD->getCardComment(recent_card_number);
//    if(input_data == "")
//        input_data = myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease);
//    
//    input_text = CCEditBox::create(CCSizeMake(190, 50), CCScale9Sprite::create("diary_text_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1)));
//	input_text->setPosition(getContentPosition(kCBP_MT_inputText));
//	input_text->setPlaceHolder(input_data.c_str());
//	input_text->setReturnType(kKeyboardReturnTypeDone);
//	input_text->setFont(mySGD->getFont().c_str(), 12);
//	input_text->setFontColor(ccBLACK);
//	input_text->setDelegate(this);
//	recent_right_img->addChild(input_text, kCBP_Z_cover);
//	input_text->setTouchPriority(-191);
//    
//    if(input_data != myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease))
//        input_text->setText(input_data.c_str());
	
	setLeftPage(covered_left_img, recent_card_number);
	
	CCSprite* n_pre = CCSprite::create("diary_left.png");
	CCSprite* s_pre = CCSprite::create("diary_left.png");
	s_pre->setColor(ccGRAY);
	
	CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBookPopup::menuAction));
	r_pre_item->setTag(kCBP_MT_pre);
	
	CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
	r_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
	covered_left_img->addChild(r_pre_menu, 1, kCBP_MT_pre);
	r_pre_menu->setTouchPriority(-191);
	
	
	animation_angle = 90.f;
	
	covered_left_img->setSkewY(animation_angle/4.f);
	covered_left_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	animation_img = covered_left_img;
	end_animation_delegate = callfunc_selector(CollectionBookPopup::endNextSelectedPage);
	
	touch_end_direction = 1;
	
	schedule(schedule_selector(CollectionBookPopup::ingPage));
}

void CollectionBookPopup::ingPage()
{
	animation_angle -= 3.f;
	
	if(animation_angle < 0.f)
		animation_angle = 0.f;
	
	animation_img->setSkewY(-touch_direction*touch_end_direction*animation_angle/4.f);
	animation_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	if(animation_angle <= 0.f)
	{
		unschedule(schedule_selector(CollectionBookPopup::ingPage));
		animation_img = NULL;
		(this->*end_animation_delegate)();
	}
}

void CollectionBookPopup::endNextPage()
{
	after_left_img->removeFromParent();
	after_left_img = recent_left_img;
	recent_left_img = covered_left_img;
	covered_left_img = NULL;
	
	int pre_number = mySGD->getPreStageCardNumber(recent_card_number);
	after_left_img->removeFromParent();
	
	after_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	after_left_img->setAnchorPoint(ccp(1.f,0.5f));
	after_left_img->setPosition(ccp(0,0));
	main_case->addChild(after_left_img, kCBP_Z_after);
	
	setLeftPage(after_left_img, pre_number);
	
	CCSprite* n_pre = CCSprite::create("diary_left.png");
	CCSprite* s_pre = CCSprite::create("diary_left.png");
	s_pre->setColor(ccGRAY);
	
	CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBookPopup::menuAction));
	r_pre_item->setTag(kCBP_MT_pre);
	
	CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
	r_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
	after_left_img->addChild(r_pre_menu, 1, kCBP_MT_pre);
	r_pre_menu->setTouchPriority(-191);
	
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_pre))->setEnabled(false);
	
	
	reorderChild(recent_left_img, kCBP_Z_recent);
	
	after_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	after_right_img->setAnchorPoint(ccp(0.f, 0.5f));
	after_right_img->setPosition(ccp(0,0));
	main_case->addChild(after_right_img, kCBP_Z_after);
	
	setRightPage(after_right_img, mySGD->getNextStageCardNumber(recent_card_number));
	
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_close))->setEnabled(false);
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(false);
	if((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(false);
//	((CCMenu*)after_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	
//	int sub_count = after_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(after_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)after_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(false);
//	}
	
	
//	CCSprite* a_n_pre = CCSprite::create("diary_left.png");
//	CCSprite* a_s_pre = CCSprite::create("diary_left.png");
//	a_s_pre->setColor(ccGRAY);
//	
//	CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBookPopup::menuAction));
//	a_pre_item->setTag(kCBP_MT_pre);
//	
//	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
//	a_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
//	a_pre_menu->setEnabled(false);
//	after_right_img->addChild(a_pre_menu, 1, kCBP_MT_pre);
//	a_pre_menu->setTouchPriority(-191);
	
	CCSprite* a_n_next = CCSprite::create("diary_right.png");
	CCSprite* a_s_next = CCSprite::create("diary_right.png");
	a_s_next->setColor(ccGRAY);
	
	CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBookPopup::menuAction));
	a_next_item->setTag(kCBP_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(getContentPosition(kCBP_MT_next));
	a_next_menu->setEnabled(false);
	after_right_img->addChild(a_next_menu, 1, kCBP_MT_next);
	a_next_menu->setTouchPriority(-191);
	
	endPage();
}

void CollectionBookPopup::endNextSelectedPage()
{
	after_left_img->removeFromParent();
	after_left_img = recent_left_img;
	recent_left_img = covered_left_img;
	covered_left_img = NULL;
	
	int pre_number = mySGD->getPreStageCardNumber(recent_card_number);
	after_left_img->removeFromParent();
	
	after_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	after_left_img->setAnchorPoint(ccp(1.f,0.5f));
	after_left_img->setPosition(ccp(0,0));
	main_case->addChild(after_left_img, kCBP_Z_after);
	
	setLeftPage(after_left_img, pre_number);
	
	CCSprite* n_pre = CCSprite::create("diary_left.png");
	CCSprite* s_pre = CCSprite::create("diary_left.png");
	s_pre->setColor(ccGRAY);
	
	CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBookPopup::menuAction));
	r_pre_item->setTag(kCBP_MT_pre);
	
	CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
	r_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
	after_left_img->addChild(r_pre_menu, 1, kCBP_MT_pre);
	r_pre_menu->setTouchPriority(-191);
	
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_pre))->setEnabled(false);
	
	reorderChild(recent_left_img, kCBP_Z_recent);
	
	after_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	after_right_img->setAnchorPoint(ccp(0.f, 0.5f));
	after_right_img->setPosition(ccp(0,0));
	main_case->addChild(after_right_img, kCBP_Z_after);
	
	setRightPage(after_right_img, mySGD->getNextStageCardNumber(recent_card_number));
	
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_close))->setEnabled(false);
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(false);
	if((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(false);
//	((CCMenu*)after_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	
//	int sub_count = after_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(after_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)after_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(false);
//	}
	
//	CCSprite* a_n_pre = CCSprite::create("diary_left.png");
//	CCSprite* a_s_pre = CCSprite::create("diary_left.png");
//	a_s_pre->setColor(ccGRAY);
//	
//	CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBookPopup::menuAction));
//	a_pre_item->setTag(kCBP_MT_pre);
//	
//	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
//	a_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
//	a_pre_menu->setEnabled(false);
//	after_right_img->addChild(a_pre_menu, 1, kCBP_MT_pre);
//	a_pre_menu->setTouchPriority(-191);
	
	CCSprite* a_n_next = CCSprite::create("diary_right.png");
	CCSprite* a_s_next = CCSprite::create("diary_right.png");
	a_s_next->setColor(ccGRAY);
	
	CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBookPopup::menuAction));
	a_next_item->setTag(kCBP_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(getContentPosition(kCBP_MT_next));
	a_next_menu->setEnabled(false);
	after_right_img->addChild(a_next_menu, 1, kCBP_MT_next);
	a_next_menu->setTouchPriority(-191);
	
	endPage();
}

void CollectionBookPopup::startPrePage()
{
	recent_left_img->removeFromParent();
	recent_left_img = after_left_img;
	after_left_img = NULL;
	
	reorderChild(recent_left_img, kCBP_Z_recent);
	((CCMenu*)recent_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(true);
	((CCMenu*)recent_left_img->getChildByTag(kCBP_MT_pre))->setEnabled(true);
	
	recent_card_number = mySGD->getPreStageCardNumber(recent_card_number);
	
	covered_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	covered_right_img->setAnchorPoint(ccp(0.f,0.5f));
	covered_right_img->setPosition(ccp(0,0));
	main_case->addChild(covered_right_img, kCBP_Z_cover);
	
	setRightPage(covered_right_img, recent_card_number);
	
//	string input_data = mySGD->getCardComment(recent_card_number);
//    if(input_data == "")
//        input_data = myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease);
//    
//    input_text = CCEditBox::create(CCSizeMake(190, 50), CCScale9Sprite::create("diary_text_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1)));
//	input_text->setPosition(getContentPosition(kCBP_MT_inputText));
//	input_text->setPlaceHolder(input_data.c_str());
//	input_text->setReturnType(kKeyboardReturnTypeDone);
//	input_text->setFont(mySGD->getFont().c_str(), 12);
//	input_text->setFontColor(ccBLACK);
//	input_text->setDelegate(this);
//	covered_right_img->addChild(input_text, kCBP_Z_cover);
//	input_text->setTouchPriority(-191);
//    
//    if(input_data != myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease))
//        input_text->setText(input_data.c_str());
	
	((CommonButton*)covered_right_img->getChildByTag(kCBP_MT_close))->setEnabled(false);
	((CommonButton*)covered_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(false);
	if((CCMenu*)covered_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)covered_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(false);
//	((CCMenu*)covered_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	
//	int sub_count = covered_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(covered_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)covered_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(false);
//	}
	
//	CCSprite* a_n_pre = CCSprite::create("diary_left.png");
//	CCSprite* a_s_pre = CCSprite::create("diary_left.png");
//	a_s_pre->setColor(ccGRAY);
//	
//	CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBookPopup::menuAction));
//	a_pre_item->setTag(kCBP_MT_pre);
//	
//	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
//	a_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
//	a_pre_menu->setEnabled(false);
//	covered_right_img->addChild(a_pre_menu, 1, kCBP_MT_pre);
//	a_pre_menu->setTouchPriority(-191);
	
	CCSprite* a_n_next = CCSprite::create("diary_right.png");
	CCSprite* a_s_next = CCSprite::create("diary_right.png");
	a_s_next->setColor(ccGRAY);
	
	CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBookPopup::menuAction));
	a_next_item->setTag(kCBP_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(getContentPosition(kCBP_MT_next));
	a_next_menu->setEnabled(false);
	covered_right_img->addChild(a_next_menu, 1, kCBP_MT_next);
	a_next_menu->setTouchPriority(-191);
	
	
	animation_angle = 90.f;
	
	covered_right_img->setSkewY(animation_angle/4.f);
	covered_right_img->setScaleX(cosf(animation_angle/180.f*M_PI));
	
	animation_img = covered_right_img;
	end_animation_delegate = callfunc_selector(CollectionBookPopup::endPrePage);
	
	touch_end_direction = 1;
	
	schedule(schedule_selector(CollectionBookPopup::ingPage));
}

void CollectionBookPopup::endPrePage()
{
	after_right_img->removeFromParent();
	after_right_img = recent_right_img;
	recent_right_img = covered_right_img;
	covered_right_img = NULL;
	
	reorderChild(recent_right_img, kCBP_Z_recent);
	((CommonButton*)recent_right_img->getChildByTag(kCBP_MT_close))->setEnabled(true);
	((CommonButton*)recent_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(true);
	if((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(true);
//	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(true);
//	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_pre))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_next))->setEnabled(true);

//	int sub_count = recent_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(recent_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(true);
//	}
	
	after_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	after_left_img->setAnchorPoint(ccp(1.f, 0.5f));
	after_left_img->setPosition(ccp(0,0));
	main_case->addChild(after_left_img, kCBP_Z_after);
	
	int pre_number = mySGD->getPreStageCardNumber(recent_card_number);
	
	setLeftPage(after_left_img, pre_number);
	
	CCSprite* n_pre = CCSprite::create("diary_left.png");
	CCSprite* s_pre = CCSprite::create("diary_left.png");
	s_pre->setColor(ccGRAY);
	
	CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBookPopup::menuAction));
	r_pre_item->setTag(kCBP_MT_pre);
	
	CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
	r_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
	after_left_img->addChild(r_pre_menu, 1, kCBP_MT_pre);
	r_pre_menu->setTouchPriority(-191);
	
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_pre))->setEnabled(false);
	
	
	after_right_img->removeFromParent();
	after_right_img = NULL;
	
	after_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	after_right_img->setAnchorPoint(ccp(0.f,0.5f));
	after_right_img->setPosition(ccp(0,0));
	main_case->addChild(after_right_img, kCBP_Z_after);
	
	setRightPage(after_right_img, mySGD->getNextStageCardNumber(recent_card_number));
	
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_close))->setEnabled(false);
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(false);
	if((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(false);
//	((CCMenu*)after_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	
//	sub_count = after_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(after_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)after_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(false);
//	}
	
//	CCSprite* a_n_pre = CCSprite::create("diary_left.png");
//	CCSprite* a_s_pre = CCSprite::create("diary_left.png");
//	a_s_pre->setColor(ccGRAY);
//	
//	CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBookPopup::menuAction));
//	a_pre_item->setTag(kCBP_MT_pre);
//	
//	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
//	a_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
//	a_pre_menu->setEnabled(false);
//	after_right_img->addChild(a_pre_menu, 1, kCBP_MT_pre);
//	a_pre_menu->setTouchPriority(-191);
	
	CCSprite* a_n_next = CCSprite::create("diary_right.png");
	CCSprite* a_s_next = CCSprite::create("diary_right.png");
	a_s_next->setColor(ccGRAY);
	
	CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBookPopup::menuAction));
	a_next_item->setTag(kCBP_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(getContentPosition(kCBP_MT_next));
	a_next_menu->setEnabled(false);
	after_right_img->addChild(a_next_menu, 1, kCBP_MT_next);
	a_next_menu->setTouchPriority(-191);
	
	endPage();
}

void CollectionBookPopup::endPreSelectedPage()
{
	after_right_img->removeFromParent();
	after_right_img = recent_right_img;
	recent_right_img = covered_right_img;
	covered_right_img = NULL;
	
	reorderChild(recent_right_img, kCBP_Z_recent);
	((CommonButton*)recent_right_img->getChildByTag(kCBP_MT_close))->setEnabled(true);
	((CommonButton*)recent_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(true);
	if((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(true);
//	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(true);
//	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_pre))->setEnabled(true);
	((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_next))->setEnabled(true);
	
//	int sub_count = recent_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(recent_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)recent_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(true);
//	}
	
	after_left_img = CCSprite::create("diary_back.png", CCRectMake(0, 0, 240, 320));
	after_left_img->setAnchorPoint(ccp(1.f, 0.5f));
	after_left_img->setPosition(ccp(0,0));
	main_case->addChild(after_left_img, kCBP_Z_after);
	
	int pre_number = mySGD->getPreStageCardNumber(recent_card_number);
	
	setLeftPage(after_left_img, pre_number);
	
	CCSprite* n_pre = CCSprite::create("diary_left.png");
	CCSprite* s_pre = CCSprite::create("diary_left.png");
	s_pre->setColor(ccGRAY);
	
	CCMenuItem* r_pre_item = CCMenuItemSprite::create(n_pre, s_pre, this, menu_selector(CollectionBookPopup::menuAction));
	r_pre_item->setTag(kCBP_MT_pre);
	
	CCMenu* r_pre_menu = CCMenu::createWithItem(r_pre_item);
	r_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
	after_left_img->addChild(r_pre_menu, 1, kCBP_MT_pre);
	r_pre_menu->setTouchPriority(-191);
	
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	((CCMenu*)after_left_img->getChildByTag(kCBP_MT_pre))->setEnabled(false);
	
	
	after_right_img->removeFromParent();
	after_right_img = NULL;
	
	after_right_img = CCSprite::create("diary_back.png", CCRectMake(240, 0, 240, 320));
	after_right_img->setAnchorPoint(ccp(0.f,0.5f));
	after_right_img->setPosition(ccp(0,0));
	main_case->addChild(after_right_img, kCBP_Z_after);
	
	setRightPage(after_right_img, mySGD->getNextStageCardNumber(recent_card_number));
	
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_close))->setEnabled(false);
	((CommonButton*)after_right_img->getChildByTag(kCBP_MT_gift))->setEnabled(false);
	if((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))
		((CCMenu*)after_right_img->getChildByTag(kCBP_MT_diary19))->setTouchEnabled(false);
//	((CCMenu*)after_right_img->getChildByTag(kCBP_MT_zoom))->setEnabled(false);
	
//	sub_count = after_right_img->getTag();
//	for(int i=0;i<sub_count;i++)
//	{
//		if(after_right_img->getChildByTag(kCBP_MT_second+i))
//			((CCMenu*)after_right_img->getChildByTag(kCBP_MT_second+i))->setEnabled(false);
//	}
	
//	CCSprite* a_n_pre = CCSprite::create("diary_left.png");
//	CCSprite* a_s_pre = CCSprite::create("diary_left.png");
//	a_s_pre->setColor(ccGRAY);
//	
//	CCMenuItem* a_pre_item = CCMenuItemSprite::create(a_n_pre, a_s_pre, this, menu_selector(CollectionBookPopup::menuAction));
//	a_pre_item->setTag(kCBP_MT_pre);
//	
//	CCMenu* a_pre_menu = CCMenu::createWithItem(a_pre_item);
//	a_pre_menu->setPosition(getContentPosition(kCBP_MT_pre));
//	a_pre_menu->setEnabled(false);
//	after_right_img->addChild(a_pre_menu, 1, kCBP_MT_pre);
//	a_pre_menu->setTouchPriority(-191);
	
	CCSprite* a_n_next = CCSprite::create("diary_right.png");
	CCSprite* a_s_next = CCSprite::create("diary_right.png");
	a_s_next->setColor(ccGRAY);
	
	CCMenuItem* a_next_item = CCMenuItemSprite::create(a_n_next, a_s_next, this, menu_selector(CollectionBookPopup::menuAction));
	a_next_item->setTag(kCBP_MT_next);
	
	CCMenu* a_next_menu = CCMenu::createWithItem(a_next_item);
	a_next_menu->setPosition(getContentPosition(kCBP_MT_next));
	a_next_menu->setEnabled(false);
	after_right_img->addChild(a_next_menu, 1, kCBP_MT_next);
	a_next_menu->setTouchPriority(-191);
	
	endPage();
}

void CollectionBookPopup::endPage()
{
	is_touch_enable = is_enable_pageturn;
	setTouchEnabled(true);
	is_menu_enable = true;
}

void CollectionBookPopup::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touch_enable)
		return;
	
	is_touch_enable = false;
	setTouchEnabled(false);
	
	if(touch_direction == 1)
		animation_img = recent_right_img;
	else if(touch_direction == -1)
		animation_img = recent_left_img;
	
	end_animation_delegate = callfunc_selector(CollectionBookPopup::endPage);
	
	touch_end_direction = -1;
	
	schedule(schedule_selector(CollectionBookPopup::ingPage));
}
void CollectionBookPopup::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touch_enable)
		return;
	
	is_touch_enable = false;
	setTouchEnabled(false);
	
	if(touch_direction == 1)
		animation_img = recent_right_img;
	else if(touch_direction == -1)
		animation_img = recent_left_img;
	
	end_animation_delegate = callfunc_selector(CollectionBookPopup::endPage);
	
	touch_end_direction = -1;
	
	schedule(schedule_selector(CollectionBookPopup::ingPage));
}

void CollectionBookPopup::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -190, true);
}

void CollectionBookPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kCBP_MT_close)
	{
		mySGD->changeSortType((CardSortType)myDSH->getIntegerForKey(kDSH_Key_cardSortType));
		
		CardSettingPopup* t_popup = CardSettingPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, kPMS_Z_popup);
		
		target_final = NULL;
		hidePopup();
	}
	else if(tag == kCBP_MT_zoom)
	{
		mySGD->selected_collectionbook = recent_card_number;

//		is_menu_enable = true;
		CCLog("why two?");
//		CCTransitionFadeTR* t_trans = CCTransitionFadeTR::create(1.f, CardViewScene::scene());
		CCDirector::sharedDirector()->pushScene(CardViewScene::scene([=](){is_menu_enable = true;}));
		
//		DiaryZoomPopup* t_popup = DiaryZoomPopup::create();
//		t_popup->setHideFinalAction(target_final, delegate_final);
//		getParent()->addChild(t_popup, kPMS_Z_popup);
//		
//		target_final = NULL;
//		hidePopup();
	}
	else if(tag == kCBP_MT_diary19)
	{
		if(graphdog->isExistApp())
		{
			// 다이어리 앱 설치되어 있음
			
			LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
			addChild(t_loading, 9999);
			t_loading->startLoading();
			
			Json::Value t_param;
			t_param["memberID"] = myHSP->getMemberID();
			
			myHSP->command("makediarycode", t_param, [=](Json::Value result_data)
						   {
							   if(result_data["result"]["code"].asInt() == GDSUCCESS)
							   {
								   graphdog->openDiaryApp(t_param["memberID"].asString(), result_data["diaryCode"].asString(), recent_card_number); // 다이어리 앱 실행 result_data["diaryCode"].asString() 과 myHSP->getMemberID() 를 보내줌
							   }
							   else
							   {
								   CCLOG("failed makediarycode");
							   }
							   
							   t_loading->removeFromParent();
							   is_menu_enable = true;
						   });
		}
		else
		{
			Diary19Popup* t_popup = Diary19Popup::create(-999, [=]()
														 {
															 is_menu_enable = true;
														 });
			addChild(t_popup, 999);
		}
	}
	else if(tag == kCBP_MT_pre)
	{
		AudioEngine::sharedInstance()->playEffect("se_pageturn.mp3", false);
		startPrePageFull();
	}
	else if(tag == kCBP_MT_next)
	{
		AudioEngine::sharedInstance()->playEffect("se_pageturn.mp3", false);
		startNextPageFull();
	}
	else if(tag >= kCBP_MT_cardBase)
	{
		int t_tag = tag - kCBP_MT_cardBase;
		
		mySGD->selected_collectionbook = t_tag;
		
		if(t_tag > recent_card_number)
			startNextFullSelectedPage();
		else if(t_tag < recent_card_number)
			startPreFullSelectedPage();
	}
	else if(tag == kCBP_MT_strength)
	{
		mySGD->setCardStrengthBefore(kCardStrengthBefore_diary);
		
		if(mySGD->isHasGottenCards(recent_card_number) > 0)
			mySGD->setStrengthTargetCardNumber(recent_card_number);
		else
			mySGD->setStrengthTargetCardNumber(myDSH->getIntegerForKey(kDSH_Key_selectedCard));
		
		CardStrengthPopup* t_popup = CardStrengthPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, kMainFlowZorder_popup);
		
		target_final = NULL;
		hidePopup();
	}
}

CollectionAnimation* CollectionAnimation::create( int t_card_number )
{
	CollectionAnimation* t_ca = new CollectionAnimation();
	t_ca->setAnimation(t_card_number);
	t_ca->initWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_animation.png", t_card_number)->getCString()), CCRectMake(0, 0, t_ca->ani_size.width, t_ca->ani_size.height));
	t_ca->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card_number)));
	t_ca->autorelease();
	t_ca->startAnimation();
	return t_ca;
}

void CollectionAnimation::setAnimation( int t_card_number )
{
	stage_number = NSDS_GI(kSDS_CI_int1_stage_i, t_card_number);
	level_number = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
	ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card_number));

	loop_length = NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card_number);

	for(int i=0;i<loop_length;i++)
		animation_frame.push_back(NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card_number, i));
}

void CollectionAnimation::startAnimation()
{
	startStageAnimation();
}

void CollectionAnimation::startStageAnimation()
{
	ing_animation_frame = 0;

	int random_value = rand()%16;

	if(random_value >= 2 && random_value <= 4)
		random_value = 7;
	else if(random_value >= 5 && random_value <= 10)
		random_value = 10;
	else if(random_value >= 11 && random_value <= 13)
		random_value = 13;
	else if(random_value == 14 || random_value == 15)
		random_value = 16;
	else
		random_value++;

	CCDelayTime* t_delay = CCDelayTime::create(random_value*0.1f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CollectionAnimation::frameAnimation));
	CCAction* t_seq = CCSequence::create(t_delay, t_call, NULL);
	runAction(t_seq);
}

void CollectionAnimation::frameAnimation()
{
	int loop_point = animation_frame[ing_animation_frame];
	setTextureRect(CCRectMake(loop_point*ani_size.width, 0, ani_size.width, ani_size.height));

	ing_animation_frame++;
	if(ing_animation_frame >= loop_length)
		startStageAnimation();
	else
	{
		CCDelayTime* t_delay = CCDelayTime::create(0.1f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CollectionAnimation::frameAnimation));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
}
