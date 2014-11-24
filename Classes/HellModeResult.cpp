//
//  HellModeResult.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 9. 23..
//
//

#include "HellModeResult.h"
#include "StageImgLoader.h"
#include "SilhouetteData.h"
#include "CountingBMLabel.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "KSAlertView.h"
#include "ASPopupView.h"
#include "AchieveNoti.h"
#include "GDWebSprite.h"
#include "KSLabelTTF.h"
#include "TakeCardToDiary.h"
#include "LoadingLayer.h"
#include "FlagSelector.h"
#include "TodayMissionPopup.h"
#include "StyledLabelTTF.h"
#include "TouchSuctionLayer.h"
#include "LabelTTFMarquee.h"
#include "CharacterExpUp.h"

typedef enum tMenuTagHellModeResult{
	kMT_HMR_ok = 1,
	kMT_HMR_replay
}MenuTagHellModeResult;

typedef enum tZorderHellModeResult{
	kZ_HMR_gray = 1,
	kZ_HMR_back,
	kZ_HMR_img,
	kZ_HMR_table,
	kZ_HMR_menu,
	kZ_HMR_popup
}ZorderHellModeResult;

void HellModeResult::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool HellModeResult::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	AudioEngine::sharedInstance()->preloadEffectScene("Ending");
	
	is_menu_enable = false;
	
	is_end_popup_animation = false;
	is_end_take_card = false;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kZ_HMR_gray);
	
	send_command_list.clear();
	
	is_rank_changed = false;
    
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f-450));
	addChild(main_case, kZ_HMR_back);
	
	inner_left = CCScale9Sprite::create("common_doubleblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	inner_left->setContentSize(CCSizeMake(209, 177));
	inner_left->setPosition(132, main_case->getContentSize().height*0.58f+3);
	main_case->addChild(inner_left);
	
	
	CCScale9Sprite* inner_right = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	inner_right->setContentSize(CCSizeMake(209, 177));
	inner_right->setPosition(347, main_case->getContentSize().height*0.58f+3);
	main_case->addChild(inner_right);
	
	CCScale9Sprite* star_back = CCScale9Sprite::create("common_deepgray.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	star_back->setContentSize(CCSizeMake(193, 80));
	star_back->setPosition(ccp(inner_left->getContentSize().width/2.f,100));
	inner_left->addChild(star_back);
	
	string title_string;
	
	bool is_clear = mySGD->getIsCleared();
	if(is_clear)
	{
		title_string = myLoc->getLocalForKey(LK::kMyLocalKey_clearTitle);
		if(!mySGD->isTimeEvent(kTimeEventType_heart))
		{
			mySGD->addChangeGoods("clearHeartUp", kGoodsType_begin, 0, "", ccsf("%d", mySGD->getGameUseTime()), "클리어하트충전");
		}
		
		int hell_count = NSDS_GI(kSDS_GI_hellMode_listCount_i);
		int character_no = -1;
		for(int i=0;character_no == -1 && i<hell_count;i++)
		{
			if(NSDS_GI(kSDS_GI_hellMode_int1_pieceNo_i, i+1) == mySD->getSilType())
				character_no = NSDS_GI(kSDS_GI_hellMode_int1_characterNo_i, i+1);
		}
		if(character_no == -1)
			character_no = 1;
		
		bool is_found = false;
		CharacterHistory found_history;
		int found_index;
		int history_size = mySGD->getCharacterHistorySize();
		for(int i=0;!is_found && i<history_size;i++)
		{
			CharacterHistory t_history = mySGD->getCharacterHistory(i);
			if(t_history.characterNo.getV() == character_no)
			{
				is_found = true;
				found_history = t_history;
				found_index = i;
			}
		}
		
		PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
		bool is_change_history = false;
		
		if(!mySGD->isClearPiece(mySD->getSilType()))
		{
			t_history.is_clear[0] = true;
			t_history.clear_count = t_history.try_count.getV();
			
			is_change_history = true;
		}
		
		if(is_change_history)
		{
			if(is_found)
			{
				CharacterHistory keep_history = found_history;
				CharacterHistory t_history = found_history;
				t_history.characterExp = mySGD->getGachaCharExp();
				send_command_list.push_back(mySGD->getUpdateCharacterHistoryParam(t_history, [=](Json::Value result_data)
																				  {
																					  if(result_data["result"]["code"].asInt() == GDSUCCESS)
																					  {
																						  float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
																						  CharacterExpUp* t_exp_up = CharacterExpUp::create(keep_history, mySGD->getCharacterHistory(found_index), ccp(240,160+160*screen_scale_y));
																						  CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(t_exp_up, 99999998);
																					  }
																				  }));
			}
			
			send_command_list.push_back(mySGD->getUpdatePieceHistoryParam(t_history, [=](Json::Value result_data)
																		   {
																			   TRACE();
																			   if(result_data["result"]["code"] == GDSUCCESS)
																			   {
																				   TRACE();
																			   }
																			   TRACE();
																		   }));
		}
		
		if(!is_found)
		{
			// 카드 획득 통신
			int card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1);

//			mySGD->addHasGottenCardNumber(card_number);
//			
//			Json::Value card_param;
//			card_param["memberID"] = hspConnector::get()->getSocialID();
//			card_param["cardNo"] = card_number;
//			card_param["addCount"] = 1;
//			
//			send_command_list.push_back(CommandParam("updateCardHistory", card_param, [=](Json::Value result_data)
//													  {
//														  TRACE();
//														  if(result_data["result"]["code"].asInt() == GDSUCCESS)
//														  {
//															  TRACE();
//															  for(int i=kAchievementCode_cardCollection1;i<=kAchievementCode_cardCollection3;i++)
//															  {
//																  if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
//																	 mySGD->getHasGottenCardsSize() >= myAchieve->getCondition((AchievementCode)i))
//																  {
//																	  myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
//																	  AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
//																	  CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
//																  }
//															  }
//														  }
//													  }));
//			
//			PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
//			bool is_change_history = false;
//			
//			if(!mySGD->isClearPiece(mySD->getSilType()))
//			{
//				t_history.is_clear[0] = true;
//				t_history.clear_count = t_history.try_count.getV();
//				
//				is_change_history = true;
//			}
//			
//			if(is_change_history)
//			{
//				send_command_list.push_back(mySGD->getUpdatePieceHistoryParam(t_history, nullptr));
//			}
			
			// 캐릭터 획득 이미지
			
			int character_index = -1;
			int character_count = NSDS_GI(kSDS_GI_characterCount_i);
			for(int i=0;character_index == -1 && i<character_count;i++)
			{
				int t_no = NSDS_GI(kSDS_GI_characterInfo_int1_no_i, i+1);
				if(character_no == t_no)
					character_index = i+1;
			}
			
			CharacterHistory tt_history;
			tt_history.characterNo = character_no;
			
			send_command_list.push_back(mySGD->getUpdateCharacterHistoryParam(tt_history, nullptr));
			
			CCNode* character_node = CCNode::create();
			character_node->setScale(0.7f);
			character_node->setPosition(ccpFromSize(star_back->getContentSize()/2.f) + ccp(0,-17));
			star_back->addChild(character_node);
			
			CCSprite* light_back = KS::loadCCBI<CCSprite*>(this, "hell_cha_back.ccbi").first;
			light_back->setPosition(ccp(0,0));
			character_node->addChild(light_back);
			
			CCSprite* character_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, character_index) + ".ccbi").first;
			character_img->setPosition(ccp(0,0));
			character_node->addChild(character_img);
			
			KSLabelTTF* take_character_card = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_takeCharacterCard), mySGD->getFont().c_str(), 15);
			take_character_card->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
			take_character_card->enableOuterStroke(ccc3(60, 20, 0), 1.f, 255, true);
			take_character_card->setPosition(ccpFromSize(star_back->getContentSize()/2.f) + ccp(0,25));
			star_back->addChild(take_character_card);
			
			
//			CCClippingNode* t_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
//			t_clipping->setAlphaThreshold(0.1f);
//			
//			CCSprite* t_card = mySIL->getLoadedImg(ccsf("card%d_visible.png", card_number));
//			t_clipping->addChild(t_card);
//			t_card->setScale(0.2f);
//			
//			t_clipping->setPosition(ccpFromSize(star_back->getContentSize()/2.f) + ccp(60,0));
//			t_clipping->setScale(1.f/0.2f*0.1f);
//			star_back->addChild(t_clipping);
//			
//			CCSprite* t_frame = CCSprite::create("hell_frame.png");
//			t_frame->setPosition(ccpFromSize(star_back->getContentSize()/2.f) + ccp(60,0));
//			star_back->addChild(t_frame);
			
//			CCSprite* card_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", card_number));
//			card_img->setScale(0.15f);
//			card_img->setPosition(ccpFromSize(star_back->getContentSize()/2.f) + ccp(60,0));
//			star_back->addChild(card_img);
		}
		else
		{
			// 이미 획득한 카드입니다.
			StyledLabelTTF* have_character_card = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_haveCharacterCard), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
			have_character_card->setAnchorPoint(ccp(0.5f,0.5f));
			have_character_card->setPosition(ccpFromSize(star_back->getContentSize()/2.f));
			star_back->addChild(have_character_card);
		}
	}
	else
	{
		// 실패 디자인
		title_string = myLoc->getLocalForKey(LK::kMyLocalKey_failTitleGameover);
		
		StyledLabelTTF* have_character_card = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failHellMode), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
		have_character_card->setAnchorPoint(ccp(0.5f,0.5f));
		have_character_card->setPosition(ccpFromSize(star_back->getContentSize()/2.f));
		star_back->addChild(have_character_card);
	}
	
	CCScale9Sprite* gold_time_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	gold_time_back->setContentSize(CCSizeMake(193, 46));
	gold_time_back->setPosition(ccp(inner_left->getContentSize().width/2.f,30));
	inner_left->addChild(gold_time_back);
	
	KSLabelTTF* gold_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_hellTitleGold), mySGD->getFont().c_str(), 12);
	gold_title->setAnchorPoint(ccp(0,0.5f));
	gold_title->setPosition(ccp(8, gold_time_back->getContentSize().height/2.f+12));
	gold_time_back->addChild(gold_title);
	
	
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	double gold_bonus = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_gold_d, t_history.characterIndex.getV(), t_history.characterLevel.getV());
	
	CCPoint base_position = ccp(gold_title->getContentSize().width, gold_title->getContentSize().height/2.f);
	if(gold_bonus > 1.0)
	{
		CCSprite* gold_bonus_back = CCSprite::create("ending_cha_bonus.png");
		gold_bonus_back->setAnchorPoint(ccp(0,0.5f));
		gold_bonus_back->setPosition(base_position);
		gold_title->addChild(gold_bonus_back);
		
		KSLabelTTF* character_bonus = KSLabelTTF::create(getLocal(LK::kMyLocalKey_characterBonus), mySGD->getFont().c_str(), 7);
		character_bonus->enableOuterStroke(ccBLACK, 1, 127, true);
		character_bonus->setPosition(ccpFromSize(gold_bonus_back->getContentSize()/2.f) + ccp(2,5));
		gold_bonus_back->addChild(character_bonus);
		
		KSLabelTTF* bonus_value = KSLabelTTF::create(ccsf("+%0.f%%", float((gold_bonus-1.0)*100.0)), mySGD->getFont().c_str(), 12);
		bonus_value->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
		bonus_value->enableOuterStroke(ccBLACK, 1, 127, true);
		bonus_value->setPosition(ccpFromSize(gold_bonus_back->getContentSize()/2.f) + ccp(2,-4));
		gold_bonus_back->addChild(bonus_value);
		
		base_position = base_position + ccp(gold_bonus_back->getContentSize().width,0);
	}
	
	if(mySGD->isTimeEvent(kTimeEventType_gold))
	{
		CCSprite* event_back = CCSprite::create("ending_event.png");
		event_back->setAnchorPoint(ccp(0,0.5f));
		event_back->setPosition(base_position);
		gold_title->addChild(event_back);
		
		KSLabelTTF* event_label = KSLabelTTF::create("EVENT", mySGD->getFont().c_str(), 7);
		event_label->enableOuterStroke(ccBLACK, 1, 127, true);
		event_label->setPosition(ccpFromSize(event_back->getContentSize()/2.f) + ccp(2,5));
		event_back->addChild(event_label);
		
		KSLabelTTF* gold_event = KSLabelTTF::create(CCString::createWithFormat("X%.1f", mySGD->getTimeEventFloatValue(kTimeEventType_gold))->getCString(), mySGD->getFont().c_str(), 12);
		gold_event->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
		gold_event->enableOuterStroke(ccBLACK, 1, 127, true);
		gold_event->setPosition(ccpFromSize(event_back->getContentSize()/2.f) + ccp(2,-4));
		event_back->addChild(gold_event);
	}
	
	KSLabelTTF* gold_content = KSLabelTTF::create(KS::insert_separator(mySGD->getStageGold()).c_str(), mySGD->getFont().c_str(), 13);
	gold_content->setAnchorPoint(ccp(1,0.5f));
	gold_content->setPosition(ccp(gold_time_back->getContentSize().width-8, gold_time_back->getContentSize().height/2.f+12));
	gold_time_back->addChild(gold_content);
	
	
	CCSprite* t_line = CCSprite::create("common_line.png");
	t_line->setScaleX(183.f/t_line->getContentSize().width);
	t_line->setPosition(ccpFromSize(gold_time_back->getContentSize()/2.f));
	gold_time_back->addChild(t_line);
	
	
	
	KSLabelTTF* time_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_hellTitleTime), mySGD->getFont().c_str(), 12);
	time_title->setAnchorPoint(ccp(0,0.5f));
	time_title->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	time_title->setPosition(ccp(8, gold_time_back->getContentSize().height/2.f-12));
	gold_time_back->addChild(time_title);
	
	KSLabelTTF* time_content = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_hellContentTime), KS::insert_separator(mySGD->hell_play_time.getV(), "%.1f").c_str()), mySGD->getFont().c_str(), 18);
	time_content->setAnchorPoint(ccp(1,0.5f));
	time_content->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	time_content->enableOuterStroke(ccBLACK, 1, 255, true);
	time_content->setPosition(ccp(gold_time_back->getContentSize().width-8, gold_time_back->getContentSize().height/2.f-12));
	gold_time_back->addChild(time_content);
	
	
	title = KSLabelTTF::create(title_string.c_str(), mySGD->getFont().c_str(), 21.5f);
//	title->setScale(0);
	if(is_clear)
		title->setGradientColor(ccc4(255, 230, 10, 255), ccc4(255, 130, 7, 255), ccp(0,-1));
	else
		title->setColor(ccc3(255, 30, 30));
	title->setPosition(ccp(inner_left->getContentSize().width/2.f,154));
	inner_left->addChild(title, kZ_HMR_menu);
	
	CCSprite* stage_tab = CCSprite::create("title_tab.png");
	stage_tab->setPosition(ccp(76,253));
	main_case->addChild(stage_tab);
	
	int stage_number = mySD->getSilType();
	
	
	KSLabelTTF* piece_number_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_hellMode), mySGD->getFont().c_str(), 11);
	piece_number_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	piece_number_label->setPosition(ccpFromSize(stage_tab->getContentSize()/2.f) + ccp(0,1));
	stage_tab->addChild(piece_number_label);
	
	is_saved_user_data = false;
	
//	if(mySGD->is_clear_diary)
//	{
//		TakeCardToDiary* t_take_card_popup = TakeCardToDiary::create(NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, 1), [=](){endTakeCard();});
//		addChild(t_take_card_popup, kZ_HMR_popup+5);
//	}
//	else
		endTakeCard();
	
//	CCScale9Sprite* left_total_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
//	left_total_back->setContentSize(CCSizeMake(193, 20));
//	left_total_back->setPosition(ccp(inner_left->getContentSize().width/2.f, 15));
//	inner_left->addChild(left_total_back);
//	
//	KSLabelTTF* left_total_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_totalScore), mySGD->getFont().c_str(), 13);
//	left_total_title->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
//	left_total_title->setAnchorPoint(ccp(0,0.5f));
//	left_total_title->setPosition(ccp(8, left_total_back->getContentSize().height/2.f));
//	left_total_back->addChild(left_total_title);
//	
//	left_total_content = KSLabelTTF::create(start_total_left_content.c_str(), mySGD->getFont().c_str(), 15);
//	left_total_content->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
//	left_total_content->setAnchorPoint(ccp(1,0.5f));
//	left_total_content->setPosition(ccp(left_total_back->getContentSize().width-8, left_total_back->getContentSize().height/2.f));
//	left_total_back->addChild(left_total_content);
	
	CCLabelTTF* t_ok_node = CCLabelTTF::create();
	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_toMain), mySGD->getFont().c_str(), 27.5f);
//	ok_label->enableOuterStroke(ccBLACK, 1.f, 255, true);
	t_ok_node->addChild(ok_label);
	
	ok_menu = CCControlButton::create(t_ok_node, CCScale9Sprite::create("mainbutton_purple.png", CCRectMake(0, 0, 215, 65), CCRectMake(107, 32, 1, 1)));
	ok_menu->setPreferredSize(CCSizeMake(215, 65));
	ok_menu->setTag(kMT_HMR_ok);
	ok_menu->addTargetWithActionForControlEvents(this, cccontrol_selector(HellModeResult::controlButtonAction), CCControlEventTouchUpInside);
	ok_menu->setPosition(ccp(347,45));
//	ok_menu->setVisible(false);
//	ok_menu->setEnabled(false);
	main_case->addChild(ok_menu, kZ_HMR_menu);
	ok_menu->setTouchPriority(-400);
	
	
	CCLabelTTF* t_replay_node = CCLabelTTF::create();
	KSLabelTTF* replay_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_regame), mySGD->getFont().c_str(), 27.5f);
//	replay_label->enableOuterStroke(ccBLACK, 1.f, 255, true);
	t_replay_node->addChild(replay_label);
	
	replay_menu = CCControlButton::create(t_replay_node, CCScale9Sprite::create("mainbutton_green.png", CCRectMake(0, 0, 215, 65), CCRectMake(107, 32, 1, 1)));
	replay_menu->setPreferredSize(CCSizeMake(215, 65));
	replay_menu->setTag(kMT_HMR_replay);
	replay_menu->addTargetWithActionForControlEvents(this, cccontrol_selector(HellModeResult::controlButtonAction), CCControlEventTouchUpInside);
	replay_menu->setPosition(ccp(132,45));
//	replay_menu->setVisible(false);
//	replay_menu->setEnabled(false);
	main_case->addChild(replay_menu, kZ_HMR_menu);
	replay_menu->setTouchPriority(-400);
	
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	loading_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
	loading_img = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_img->setPosition(ccp(347,150));
	main_case->addChild(loading_img, kZ_HMR_img);
//	reader->release();
	

	Json::Value param2;
	param2["myScore"]=mySGD->hell_play_time.getV()*100.f;
	param2["stageNo"]=mySD->getSilType();
	param2["memberID"] = hspConnector::get()->getSocialID();
	param2["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	param2["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	Json::Value p2_data;
	p2_data["character"] = myDSH->getIntegerForKey(kDSH_Key_selectedCharacter);
	p2_data["highstage"] = mySGD->getUserdataHighPiece();
	param2["data"] = p2_data;
	
	send_command_list.push_back(CommandParam("getstagerankbyalluser", param2, json_selector(this, HellModeResult::resultGetRank)));
	mySGD->keep_time_info.is_loaded = false;
	send_command_list.push_back(CommandParam("gettimeinfo", Json::Value(), json_selector(this, HellModeResult::resultGetTime)));
	
    return true;
}

void HellModeResult::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
	LoadingLayer* t_loading = LoadingLayer::create(-9999);
	addChild(t_loading, 9999);
	t_loading->startLoading();
	tryTransaction(t_loading);
}

void HellModeResult::controlButtonAction(CCObject* sender, CCControlEvent t_event)
{
	menuAction(sender);
}

void HellModeResult::tryTransaction(CCNode* t_loading)
{
	TRACE();
	mySGD->changeGoodsTransaction(send_command_list, [=](Json::Value result_data)
								  {
									  TRACE();
									  if(result_data["result"]["code"].asInt() == GDSUCCESS)
									  {
										  TRACE();
										  CCLOG("HellModeResult transaction success");
										  
										  mySGD->network_check_cnt = 0;
										  
										  t_loading->removeFromParent();
									  }
									  else
									  {
										  TRACE();
										  CCLOG("HellModeResult transaction fail");
										  
										  mySGD->network_check_cnt++;
										  
										  if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
										  {
											  mySGD->network_check_cnt = 0;
											  
											  ASPopupView* alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4), [=](){
												  tryTransaction(t_loading);
											  }, 1);
											  
											  if(alert)
												  ((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
										  }
										  else
										  {
											  addChild(KSTimer::create(0.5f, [=]()
																	   {
																		   tryTransaction(t_loading);
																	   }));
										  }
									  }
									  TRACE();
								  });
}

HellModeResult::~HellModeResult()
{
	hspConnector::get()->removeTarget(this);
}

void HellModeResult::resultGetTime(Json::Value result_data)
{
	TRACE();
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		TRACE();
		mySGD->keep_time_info.timestamp = result_data["timestamp"].asUInt();
		mySGD->keep_time_info.weekNo = result_data["weekNo"].asUInt();
		mySGD->keep_time_info.weekday = result_data["weekday"].asInt();
		mySGD->keep_time_info.date = result_data["date"].asInt64();
		mySGD->keep_time_info.hour = result_data["hour"].asInt();
		mySGD->keep_time_info.is_loaded = true;
	}
}

void HellModeResult::resultGetRank(Json::Value result_data)
{
	TRACE();
	cell_action_list.clear();
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
//		CCLog("resultGetRank : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
		TRACE();
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		
		CCSprite* graph_back = CCSprite::create("ending_graph.png");
		graph_back->setPosition(ccp(347,240));
		main_case->addChild(graph_back, kZ_HMR_img);
		
		//		KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
		//		t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		//		t_rank_a->setPosition(ccp(25,8));
		//		graph_back->addChild(t_rank_a);
		//
		//		KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
		//		t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		//		t_rank_b->setPosition(ccp(25+49,8));
		//		graph_back->addChild(t_rank_b);
		//
		//		KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
		//		t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		//		t_rank_c->setPosition(ccp(25+98,8));
		//		graph_back->addChild(t_rank_c);
		//
		//		KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
		//		t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		//		t_rank_d->setPosition(ccp(25+147,8));
		//		graph_back->addChild(t_rank_d);
		
		int alluser = result_data["alluser"].asInt();
		int myrank = result_data["myrank"].asInt();
		
		int before_stage_high_rank = myDSH->getIntegerForKey(kDSH_Key_stageHighRank_int1, mySD->getSilType()); // 이전 기록
		
		myDSH->setIntegerForKey(kDSH_Key_stageHighRank_int1, mySD->getSilType(), myrank);
		//		CCLabelTTF* all_user_label = CCLabelTTF::create(CCString::createWithFormat("/%d", alluser)->getCString(), mySGD->getFont().c_str(), 10);
		//		all_user_label->setColor(ccc3(255, 170, 20));
		//		all_user_label->setAnchorPoint(ccp(1,0.5));
		//		all_user_label->setPosition(ccp(main_case->getContentSize().width-30, 210));
		//		main_case->addChild(all_user_label, kZ_CP_img);
		//
		//		CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_myrankValue), myrank)->getCString(), mySGD->getFont().c_str(), 10);
		//		my_rank_label->setAnchorPoint(ccp(1,0.5));
		//		my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
		//		main_case->addChild(my_rank_label, kZ_CP_img);
		//		my_rank_label->setOpacity(0);
		
		float rank_percent = 1.f*myrank/alluser;
		
		CCSprite* rank_percent_case = CCSprite::create("gameresult_rank_percent.png");
		rank_percent_case->setAnchorPoint(ccp(0.5,1));
		rank_percent_case->setPosition(ccp(249+195,240));
		main_case->addChild(rank_percent_case, kZ_HMR_img);
		
		KSLabelTTF* percent_label = KSLabelTTF::create("100%", mySGD->getFont().c_str(), 12);
		//		percent_label->setColor(ccc3(255, 170, 20));
		percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
		percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f-3));
		rank_percent_case->addChild(percent_label, kZ_HMR_img);
		//		percent_label->setOpacity(0);
		
		// 이까지 그래프 표시하는 부분 코드.
		
		
		cell_action_list.push_back([=](){
			CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(249 + 195.f*rank_percent,240));
			rank_percent_case->runAction(t_move);
			
			CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
			//			CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 255);
			CCSequence* t_seq2 = CCSequence::create(t_delay2, /*t_fade2,*/ NULL);
			percent_label->runAction(t_seq2);
			addChild(KSGradualValue<float>::create(100.f, rank_percent*100.f,
												   2.f * (1.f - rank_percent), [=](float t){
													   percent_label->setString(ccsf("%.0f%%", t));
												   }, [=](float t){
													   percent_label->setString(ccsf("%.0f%%", t));
												   }));
		});
		
		Json::Value user_list = result_data["list"];
		
		delay_index = 0;
		int limit_count = 3;
		auto cellSize = CCSizeMake(204, 31);
		
		float spinDuration = 0.3f;
		CCScale9Sprite* t_list_cell_case_back = nullptr;
		CCScale9Sprite* t_list_cell_case = nullptr;
		for(int i=0;i<user_list.size() && i<limit_count;i++)
		{
			string case_name;
			bool isMe = false;
			if(myrank == i+1)
			{
				case_name = "mainpopup_purple.png";
				limit_count++;
				isMe = true;
			}
			else
			{
				case_name = "rank_normal2.png";
			}
			
			
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			auto cellPosition = ccp(347, 191 - i*32);
			list_cell_case->setContentSize(cellSize);
			list_cell_case->setPosition(cellPosition);
			main_case->addChild(list_cell_case, kZ_HMR_img);
			
			if(isMe)
			{
				CCScale9Sprite* list_cell_case_back = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
				list_cell_case_back->setContentSize(cellSize);
				list_cell_case_back->setPosition(cellPosition);
				t_list_cell_case_back = list_cell_case_back;
				t_list_cell_case = list_cell_case;
				main_case->addChild(list_cell_case_back, kZ_HMR_img);
				
			}
			CCPoint rank_position = ccp(17,15.5f);
			if(i == 0)
			{
				CCSprite* gold_medal = CCSprite::create("rank_gold.png");
				gold_medal->setPosition(rank_position);
				list_cell_case->addChild(gold_medal);
			}
			else if(i == 1)
			{
				CCSprite* silver_medal = CCSprite::create("rank_silver.png");
				silver_medal->setPosition(rank_position);
				list_cell_case->addChild(silver_medal);
			}
			else if(i == 2)
			{
				CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
				bronze_medal->setPosition(rank_position);
				list_cell_case->addChild(bronze_medal);
			}
			else
			{
				KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 10);
				rank_label->setPosition(ccp(33,rank_position.y+8));
				list_cell_case->addChild(rank_label);
			}
			
			Json::Reader reader;
			Json::Value read_data;
			reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
			
			string flag = read_data.get("flag", "kr").asString().c_str();
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			if(i >= 3)
				selectedFlagSpr->setPosition(ccp(33,rank_position.y-5));
			else
				selectedFlagSpr->setPosition(ccp(49,15.5f));
			selectedFlagSpr->setScale(0.8);
			list_cell_case->addChild(selectedFlagSpr);
			
			CCLabelTTF* t_nick_size = CCLabelTTF::create(user_list[i].get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12.5f);
			if(t_nick_size->getContentSize().width > 70)
			{
				LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 70, 15, "");
				nick_marquee->setSpace(30);
				nick_marquee->addText(("<font strokecolor=000 strokesize=0.3f strokeopacity=50>" + user_list[i].get("nick", Json::Value()).asString() + "</font>").c_str());
				nick_marquee->startMarquee();
				nick_marquee->setFontSize(12.5f);
				nick_marquee->setAnchorPoint(ccp(0,0.5f));
				nick_marquee->setPosition(ccp(64,15.5f));
				list_cell_case->addChild(nick_marquee);
			}
			else
			{
				KSLabelTTF* nick_label = KSLabelTTF::create(user_list[i].get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12.5f); // user_list[i]["nick"].asString().c_str()
				nick_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				nick_label->setAnchorPoint(ccp(0,0.5f));
				nick_label->setPosition(ccp(64,15.5f));
				list_cell_case->addChild(nick_label);
			}
			
			KSLabelTTF* score_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_hellContentTime), KS::insert_separator(user_list[i]["score"].asInt64()/100.f, "%.1f").c_str()), mySGD->getFont().c_str(), 13);
			score_label->disableOuterStroke();
			score_label->setAnchorPoint(ccp(1,0.5));
			score_label->setColor(ccc3(55, 35, 150));
			score_label->setPosition(ccp(198,15.5f));
			list_cell_case->addChild(score_label);
			
			CCPoint original_position = list_cell_case->getPosition();
			list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
			
			cell_action_list.push_back([=](){
				CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
				CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
				CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
				list_cell_case->runAction(t_seq);
				delay_index++;
			});
		}
		
		if(myrank > 3)
		{
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_purple.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			list_cell_case->setContentSize(CCSizeMake(204, 31));
			list_cell_case->setPosition(ccp(347,191-3*32));
			t_list_cell_case = list_cell_case;
			main_case->addChild(list_cell_case, kZ_HMR_img);
			
			KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", myrank)->getCString(), mySGD->getFont().c_str(), 10);
			rank_label->setPosition(ccp(33,15.5f+8));
			list_cell_case->addChild(rank_label);
			
			string flag = myDSH->getStringForKey(kDSH_Key_flag);
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			selectedFlagSpr->setPosition(ccp(33,15.5f-5));
			selectedFlagSpr->setScale(0.8);
			list_cell_case->addChild(selectedFlagSpr);
			
			CCLabelTTF* t_nick_size = CCLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12.5f);
			if(t_nick_size->getContentSize().width > 70)
			{
				LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 70, 15, "");
				nick_marquee->setSpace(30);
				nick_marquee->addText(("<font strokecolor=000 strokesize=0.3f strokeopacity=50>" + myDSH->getStringForKey(kDSH_Key_nick) + "</font>").c_str());
				nick_marquee->startMarquee();
				nick_marquee->setFontSize(12.5f);
				nick_marquee->setAnchorPoint(ccp(0,0.5f));
				nick_marquee->setPosition(ccp(64,15.5f));
				list_cell_case->addChild(nick_marquee);
			}
			else
			{
				KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12.5f); // user_list[i]["nick"].asString().c_str()
				nick_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				nick_label->setAnchorPoint(ccp(0,0.5f));
				nick_label->setPosition(ccp(64,15.5f));
				list_cell_case->addChild(nick_label);
			}
			
			KSLabelTTF* score_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_hellContentTime), KS::insert_separator(mySGD->hell_play_time.getV(), "%.1f").c_str()), mySGD->getFont().c_str(), 13);
			score_label->disableOuterStroke();
			score_label->setAnchorPoint(ccp(1,0.5));
			score_label->setColor(ccc3(55, 35, 150));
			score_label->setPosition(ccp(198,15.5f));
			list_cell_case->addChild(score_label);
			
			CCPoint original_position = list_cell_case->getPosition();
			list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
			
			cell_action_list.push_back([=](){
				CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
				CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
				CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
				list_cell_case->runAction(t_seq);
			});
			
			if(!(before_stage_high_rank == 0 || before_stage_high_rank == myrank))
			{
				CCScale9Sprite* list_cell_case_back = CCScale9Sprite::create("mainpopup_purple.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
				
				list_cell_case_back->setContentSize(cellSize);
				list_cell_case_back->setPosition(original_position);
				t_list_cell_case_back = list_cell_case_back;
				main_case->addChild(list_cell_case_back, kZ_HMR_img);
			}
		}
		if(t_list_cell_case_back)
		{
			std::string changedRankLbl;
			if(before_stage_high_rank == 0 || before_stage_high_rank == myrank)
			{
				// nothing to do
				t_list_cell_case_back->setVisible(false);
			}
			else
			{
				
				if(before_stage_high_rank - myrank >= 0)
				{
					changedRankLbl = ccsf("▲   %d", before_stage_high_rank - myrank);
				}
				else
				{
					changedRankLbl = ccsf("▼   %d", before_stage_high_rank - myrank);
				}
				KSLabelTTF* changedRank = KSLabelTTF::create(changedRankLbl.c_str(), mySGD->getFont().c_str(), 12);
				changedRank->setPosition(ccpFromSize(t_list_cell_case_back->getContentSize()) / 2.f);
				changedRank->setColor(ccc3(255, 179, 0));
				t_list_cell_case_back->addChild(changedRank);
				
				float flipDelay = 3.f;
				t_list_cell_case_back->setScaleY(0);
				frontFlip = [=](){
					addChild(KSTimer::create(flipDelay, [=](){
						addChild(KSGradualValue<float>::create(1.f, 0, spinDuration, [=](float t){
							t_list_cell_case->setScaleY(t);
						}, [=](float t){
							t_list_cell_case->setScaleY(t);
							
							addChild(KSGradualValue<float>::create(0.f, 1.f, spinDuration, [=](float t){
								t_list_cell_case_back->setScaleY(t);
							}, [=](float t){
								t_list_cell_case_back->setScaleY(t);
								backFlip();
							}));
						}));
					}));
				};
				backFlip = [=](){
					addChild(KSTimer::create(flipDelay, [=](){
						addChild(KSGradualValue<float>::create(1.f, 0, spinDuration, [=](float t){
							t_list_cell_case_back->setScaleY(t);
						}, [=](float t){
							t_list_cell_case_back->setScaleY(t);
							
							addChild(KSGradualValue<float>::create(0.f, 1.f, spinDuration, [=](float t){
								t_list_cell_case->setScaleY(t);
							}, [=](float t){
								t_list_cell_case->setScaleY(t);
								frontFlip();
							}));
						}));
					}));
				};
				addChild(KSTimer::create(5.f, [=](){
					frontFlip();
				}));
			}
		}
		
		is_saved_user_data = true;
		endLoad();
		
		if(is_end_popup_animation)
		{
			for(int i=0;i<cell_action_list.size();i++)
				cell_action_list[i]();
		}
		
        if(loading_img)
        {
            loading_img->removeFromParent();
            loading_img = NULL;
        }
	}
	else
	{
		TRACE();
		//		hspConnector::get()->command(send_command_list, -1); // 987987987
		
		//		CCLabelTTF* fail_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failCheckRanking), mySGD->getFont().c_str(), 12);
		//		fail_label->setPosition(loading_img->getPosition());
		//		main_case->addChild(fail_label, kZ_CP_img);
	}
	TRACE();
}

void HellModeResult::showPopup()
{
	gray->setOpacity(255);
	//	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	//	gray->runAction(gray_fade);
	
	main_case->setPosition(ccp(240,160-14.f));
	
	
	
	endShowPopup();
	//	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
	//	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ClearPopup::endShowPopup));
	//	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	//	main_case->runAction(main_seq);
}

void HellModeResult::endShowPopup()
{
	
}

void HellModeResult::hidePopup()
{
	is_menu_enable = false;
	//	rankTableView->setTouchEnabled(false);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-14.f-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(HellModeResult::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void HellModeResult::endHidePopup()
{
	AudioEngine::sharedInstance()->unloadEffectScene("Ending");
	
	if(target_final)
		(target_final->*delegate_final)();
	
	removeFromParent();
}

void HellModeResult::endTakeCard()
{
	AudioEngine::sharedInstance()->playEffect("bgm_result.mp3");
	is_end_take_card = true;
	
	CCScaleTo* title_scale1 = CCScaleTo::create(1.f/6.f, 1.1f);
	CCScaleTo* title_scale2 = CCScaleTo::create(1.f/12.f, 1.f);
	CCSequence* title_seq = CCSequence::create(title_scale1, title_scale2, NULL);
	title->runAction(title_seq);
	
	closePopup();
}

void HellModeResult::closePopup()
{
	is_end_popup_animation = true;
	if(is_end_popup_animation && is_saved_user_data)// && is_loaded_list)
	{
		is_menu_enable = true;
	}
}

void HellModeResult::resultSavedUserData(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		is_saved_user_data = true;
		endLoad();
	}
	else
	{
		
	}
}

void HellModeResult::endLoad()
{
	if(is_end_popup_animation && is_saved_user_data)// && is_loaded_list)
	{
		is_menu_enable = true;
	}
}

void HellModeResult::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void HellModeResult::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	AudioEngine::sharedInstance()->stopAllEffects();
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kMT_HMR_ok)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		AudioEngine::sharedInstance()->playSound("bgm_ui.mp3", true);
		//		mySGD->resetLabels();
		mySGD->is_hell_mode = false;
		hidePopup();
	}
	else if(tag == kMT_HMR_replay)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		
		addChild(KSGradualValue<float>::create(1.f, 0.f, 0.5f, [=](float t)
											   {
												   gray->setOpacity(t*255);
												   main_case->setPosition(ccp(240,160-(450-450*t)));
											   }, [=](float t)
											   {
												   gray->setOpacity(0);
												   main_case->setPosition(ccp(240,160-450));
												   replay_func();
												   removeFromParent();
											   }));
	}
}