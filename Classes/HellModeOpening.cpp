//
//  HellModeOpening.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 9. 20..
//
//

#include "HellModeOpening.h"
#include "DataStorageHub.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "TouchSuctionLayer.h"
#include "FlagSelector.h"
#include "FormSetter.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"
#include "StageImgLoader.h"
#include "StartSettingPopup.h"
#include "MainFlowScene.h"
#include "StyledLabelTTF.h"
#include "MyLocalization.h"
#include "CommonAnimation.h"
#include "LabelTTFMarquee.h"
#include "CCMenuLambda.h"
#include "TypingBox.h"
#include "ScrollMenu.h"
#include "StoryLayer.h"

enum HellModeOpeningZorder
{
	kHellModeOpeningZorder_gray = 0,
	kHellModeOpeningZorder_back,
	kHellModeOpeningZorder_content
};

bool HellModeOpening::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	touch_priority = -300;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kHellModeOpeningZorder_gray);
	
	gray->runAction(CCFadeTo::create(0.5f, 255));
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	suction->setTouchEnabled(true);
	addChild(suction);
	
	is_menu_enable = false;
	
	setMain();
	
	if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_hellmode))
	{
		myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_hellmode, true);
		StoryLayer::startStory(this, "menu_hellmode");
	}
	
	return true;
}

void HellModeOpening::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

void HellModeOpening::setMain()
{
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kHellModeOpeningZorder_back);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-10));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_hellMode), mySGD->getFont().c_str(), 14);
	title_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,1.5f));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(main_case->getContentSize().width-20, main_case->getContentSize().height-12));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
								  CommonAnimation::closePopup(this, main_case, gray, [=](){
									  
								  }, [=](){
									  mySGD->is_hell_mode = false;
									  if(target_final && delegate_final)
										  (target_final->*delegate_final)();
									  removeFromParent();
								  });
							  });
	main_case->addChild(close_button, 2);
	
	
	CCScale9Sprite* tip_marquee_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	tip_marquee_back->setContentSize(CCSizeMake(278, 26));
	tip_marquee_back->setPosition(ccp(main_case->getContentSize().width*0.655f, main_case->getContentSize().height+2-23.5f));
	main_case->addChild(tip_marquee_back);
	
	LabelTTFMarquee* tipMaquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 278, 22, "");
	tipMaquee->addText(getLocal(LK::kMyLocalKey_hellMarquee2));
	tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_hellMarquee));
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
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	left_back->setContentSize(CCSizeMake(270, 212));
	left_back->setPosition(ccp(25+left_back->getContentSize().width/2.f, main_case->getContentSize().height*0.44f));
	main_case->addChild(left_back);
	
	loading_right_circle = NULL;
	
	hell_list.clear();
	int list_cnt = NSDS_GI(kSDS_GI_hellMode_listCount_i);
	int not_played_stage = -1;
	int not_take_stage = -1;
	int first_stage = -1;
	for(int i=0;i<list_cnt;i++)
	{
		HellInfo t_info;
		t_info.title = NSDS_GS(kSDS_GI_hellMode_int1_title_s, i+1);
		t_info.content = NSDS_GS(kSDS_GI_hellMode_int1_content_s, i+1);
		t_info.is_take = false;
		
		int piece_number = NSDS_GI(kSDS_GI_hellMode_int1_pieceNo_i, i+1);
		int card_number = NSDS_GI(piece_number, kSDS_SI_level_int1_card_i, 1);
		
		int character_no = NSDS_GI(kSDS_GI_hellMode_int1_characterNo_i, i+1);
		int character_count = NSDS_GI(kSDS_GI_characterCount_i);
		for(int j=1;j<=character_count;j++)
		{
			if(NSDS_GI(kSDS_GI_characterInfo_int1_no_i, j) == character_no)
			{
				t_info.character_name = NSDS_GS(kSDS_GI_characterInfo_int1_name_s, j);
				break;
			}
		}
		
		int history_size = mySGD->getCharacterHistorySize();
		for(int j=0;!t_info.is_take && j<history_size;j++)
		{
			CharacterHistory t_history = mySGD->getCharacterHistory(j);
			if(t_history.characterNo.getV() == character_no)
				t_info.is_take = true;
		}
		
		int condition_stage = NSDS_GI(kSDS_GI_hellMode_int1_openPieceNo_i, i+1);
		t_info.condition_stage = condition_stage;
		
		PieceHistory t_history = mySGD->getPieceHistory(condition_stage);
		
		if(t_history.is_clear[0].getV() || t_history.is_clear[1].getV() || t_history.is_clear[2].getV() || t_history.is_clear[3].getV())
			t_info.is_open = true;
		else
			t_info.is_open = false;
		
		if(t_info.is_open && not_played_stage == -1)
		{
			PieceHistory tt_history = mySGD->getPieceHistory(piece_number);
			if(tt_history.try_count.getV() <= 0)
			{
				not_played_stage = piece_number;
			}
		}
		
		if(t_info.is_open && not_take_stage == -1)
		{
			if(!t_info.is_take)
			{
				not_take_stage = piece_number;
			}
		}
		
		if(t_info.is_open && first_stage == -1)
			first_stage = piece_number;
		
		t_info.stage_number = piece_number;
		
		hell_list.push_back(t_info);
	}
	
	clicked_stage = -1;
	
	if(not_played_stage != -1)
		clicked_stage = not_played_stage;
	else if(not_take_stage != -1)
		clicked_stage = not_take_stage;
	else if(first_stage != -1)
		clicked_stage = first_stage;
	
	
	CCRect table_rect = CCRectMake(3, left_back->getContentSize().height/2.f - 150/2.f - 30, 265, 210);
	
//	CCSprite* table_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_rect.size.width, table_rect.size.height));
//	table_back->setOpacity(100);
//	table_back->setAnchorPoint(ccp(0,0));
//	table_back->setPosition(table_rect.origin);
//	left_back->addChild(table_back);
	
	hell_table = CCTableView::create(this, table_rect.size);
	hell_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	hell_table->setPosition(table_rect.origin);
	left_back->addChild(hell_table);
	hell_table->setTouchPriority(touch_priority);
	hell_table->setDelegate(this);
	
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	right_back->setContentSize(CCSizeMake(150, 160));
	right_back->setPosition(ccp(main_case->getContentSize().width-25-right_back->getContentSize().width/2.f, 149));
	main_case->addChild(right_back);
	
	right_info_node = CCNode::create();
	right_info_node->setPosition(ccpFromSize(right_back->getContentSize()/2.f));
	right_back->addChild(right_info_node);
	
	if(myDSH->getMainFlowSceneShowType() == kMainFlowSceneShowType_hellReplay)
		clicked_stage = mySD->getSilType();
	
	if(clicked_stage != -1)
	{
		setRight(clicked_stage);
	}
	
	ready_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ready), 18.5f, CCSizeMake(156, 50), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), touch_priority);
	ready_button->setPosition(ccp(right_back->getContentSize().width/2.f,-28));
	right_back->addChild(ready_button);
	ready_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
								  if(clicked_stage != -1)
									{
										CommonAnimation::closePopup(this, main_case, gray, [=](){
											
										}, [=](){
											mySGD->is_hell_mode = true;
											mySD->setSilType(clicked_stage);
											
											StartSettingPopup* t_popup = StartSettingPopup::create();
											t_popup->setHideFinalAction(getParent(), callfunc_selector(MainFlowScene::showHellOpening));
											getParent()->addChild(t_popup, kMainFlowZorder_popup);
											removeFromParent();
										});
									}
								  else
									{
										is_menu_enable = true;
									}
							  });
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, [=]()
	{
		if(myDSH->getMainFlowSceneShowType() == kMainFlowSceneShowType_hellReplay)
		{
			CommonAnimation::closePopup(this, main_case, gray, [=](){
				
			}, [=](){
				mySGD->is_hell_mode = true;
				mySD->setSilType(clicked_stage);
				
				StartSettingPopup* t_popup = StartSettingPopup::create();
				t_popup->setHideFinalAction(getParent(), callfunc_selector(MainFlowScene::showHellOpening));
				getParent()->addChild(t_popup, kMainFlowZorder_popup);
				myDSH->setMainFlowSceneShowType(kMainFlowSceneShowType_init);
				removeFromParent();
			});
		}
		else
			is_menu_enable = true;
	});
}

void HellModeOpening::setRight(int t_stage_number)
{
	if(called_stage != t_stage_number)
	{
		right_info_node->removeAllChildren();
		
		loading_right_circle = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
		loading_right_circle->setPosition(ccp(0,0));
		right_info_node->addChild(loading_right_circle);
		
		called_stage = t_stage_number;
		
		Json::Value t_param;
		t_param["memberID"] = myHSP->getSocialID();
		t_param["stageNo"] = called_stage;
		
		myHSP->command("getstagerankbyalluser", t_param, this, json_selector(this, HellModeOpening::resultGetRank));
	}
	else if(selected_stage == t_stage_number)
	{
		right_info_node->removeAllChildren();
		
		resultGetRank(saved_stage_rank);
	}
	else
	{
		
	}
}

void HellModeOpening::resultGetRank(Json::Value result_data)
{
	if(loading_right_circle)
	{
		loading_right_circle->removeFromParent();
		loading_right_circle = NULL;
	}
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		saved_stage_rank = result_data;
		selected_stage = result_data["stageNo"].asInt();
		
		if(selected_stage != called_stage)
			return;
		
		Json::Value user_list = result_data["list"];
		
		CCSprite* graph_back = CCSprite::create("endless_graph.png");
		graph_back->setPosition(ccp(0,67));
		right_info_node->addChild(graph_back);
		
		int alluser = result_data["alluser"].asInt();
		int myrank = result_data["myrank"].asInt();
		
		float rank_percent = alluser == 0 ? 1.f : 1.f * myrank/alluser;
		
		CCSprite* rank_percent_case = CCSprite::create("gameresult_rank_percent.png");//"puzzle_rank_percent.png");
		rank_percent_case->setAnchorPoint(ccp(0.5,1));
		rank_percent_case->setPosition(ccp(graph_back->getContentSize().width,7));
		graph_back->addChild(rank_percent_case);
		
		KSLabelTTF* percent_label = KSLabelTTF::create("100%", mySGD->getFont().c_str(), 12);
		percent_label->addChild(KSGradualValue<float>::create(100.f, rank_percent*100.f,
															  2.f * (1.f - rank_percent), [=](float t){
																  percent_label->setString(ccsf("%.0f%%", t));
															  }, [=](float t){
																  percent_label->setString(ccsf("%.0f%%", t));
															  }));
		percent_label->enableOuterStroke(ccc3(50, 25, 0), 0.5f);
		percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f-3));
		rank_percent_case->addChild(percent_label);
		
		CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(graph_back->getContentSize().width - graph_back->getContentSize().width*(1.f-rank_percent),7));
		rank_percent_case->runAction(t_move);
		
		int limit_count = 3;
		for(int i=0;i<user_list.size() && i<limit_count;i++)
		{
			string case_name;
			if(myrank == i+1)
			{
				case_name = "mainpopup_purple_s.png";
				limit_count++;
			}
			else
			{
				case_name = "rank_normal2_s.png";
			}
			
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
			list_cell_case->setContentSize(CCSizeMake(145, 28.5f));
			list_cell_case->setPosition(ccp(0,22-i*29.f));
			right_info_node->addChild(list_cell_case);
			
			CCPoint rank_position = ccp(14,list_cell_case->getContentSize().height/2.f);
			if(i == 0)
			{
				CCSprite* gold_medal = CCSprite::create("rank_gold.png");
				gold_medal->setPosition(rank_position);
				gold_medal->setScale(0.85f);
				list_cell_case->addChild(gold_medal);
			}
			else if(i == 1)
			{
				CCSprite* silver_medal = CCSprite::create("rank_silver.png");
				silver_medal->setPosition(rank_position);
				silver_medal->setScale(0.85f);
				list_cell_case->addChild(silver_medal);
			}
			else if(i == 2)
			{
				CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
				bronze_medal->setPosition(rank_position);
				bronze_medal->setScale(0.85f);
				list_cell_case->addChild(bronze_medal);
			}
			else
			{
				KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 12);
				rank_label->disableOuterStroke();
				rank_label->setPosition(rank_position);//ccp(27.f, rank_position.y+8));
				list_cell_case->addChild(rank_label);
			}
			
			Json::Reader reader;
			Json::Value read_data;
			reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
			
			string flag = read_data.get("flag", "kr").asString().c_str();
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
//			if(i >= 3)
//				selectedFlagSpr->setPosition(ccp(27,list_cell_case->getContentSize().height/2.f-5));
//			else
				selectedFlagSpr->setPosition(ccp(40,list_cell_case->getContentSize().height/2.f));
			selectedFlagSpr->setScale(0.7);
			list_cell_case->addChild(selectedFlagSpr);
			
			CCLabelTTF* t_nick_size = CCLabelTTF::create(user_list[i].get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12.5f);
			if(t_nick_size->getContentSize().width > 70)
			{
				LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 70, 15, "");
				nick_marquee->setSpace(30);
				nick_marquee->addText(("<font strokecolor=000 strokesize=0.3 strokeopacity=50>" + user_list[i].get("nick", Json::Value()).asString() + "</font>").c_str());
				nick_marquee->startMarquee();
				nick_marquee->setFontSize(12.5f);
				nick_marquee->setAnchorPoint(ccp(0.5f,0.5f));
				nick_marquee->setPosition(ccp(93,list_cell_case->getContentSize().height/2.f + 5));
				list_cell_case->addChild(nick_marquee);
			}
			else
			{
				KSLabelTTF* nick_label = KSLabelTTF::create(user_list[i].get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12.5f); // user_list[i]["nick"].asString().c_str()
				nick_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				nick_label->setPosition(ccp(93,list_cell_case->getContentSize().height/2.f + 5));
				list_cell_case->addChild(nick_label);
			}
			
			KSLabelTTF* score_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_hellContentTime), KS::insert_separator(user_list[i]["score"].asInt64()/100.f, "%.1f").c_str()), mySGD->getFont().c_str(), 12);
			score_label->setColor(ccc3(54, 36, 148));
			score_label->disableOuterStroke();
			score_label->setAnchorPoint(ccp(1,0.5f));
			score_label->setPosition(ccp(135,list_cell_case->getContentSize().height/2.f - 7));
			list_cell_case->addChild(score_label);
			
			//			CCPoint original_position = list_cell_case->getPosition();
			//			list_cell_case->setPosition(ccpAdd(original_position, ccp(0, -500)));
			//
			//			cell_action_list.push_back([=](){
			//				CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
			//				CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
			//				CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
			//				list_cell_case->runAction(t_seq);
			//				delay_index++;
			//			});
		}
		
		if(myrank > 3)
		{
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_purple_s.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
			list_cell_case->setContentSize(CCSizeMake(145, 28.5f));
			list_cell_case->setPosition(ccp(0,22-3*29.f));
			right_info_node->addChild(list_cell_case);
			
			KSLabelTTF* rank_label = KSLabelTTF::create(KS::insert_separator(myrank).c_str(), mySGD->getFont().c_str(), 12);
			rank_label->disableOuterStroke();
			rank_label->setPosition(ccp(14,list_cell_case->getContentSize().height/2.f));//ccp(27,list_cell_case->getContentSize().height/2.f+8));
			list_cell_case->addChild(rank_label);
			
			
			string flag = myDSH->getStringForKey(kDSH_Key_flag);
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			selectedFlagSpr->setPosition(ccp(40,list_cell_case->getContentSize().height/2.f));//ccp(27,list_cell_case->getContentSize().height/2.f-5));
			selectedFlagSpr->setScale(0.7);
			list_cell_case->addChild(selectedFlagSpr);
			
			CCLabelTTF* t_nick_size = CCLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12.5f);
			if(t_nick_size->getContentSize().width > 70)
			{
				LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 70, 15, "");
				nick_marquee->setSpace(30);
				nick_marquee->addText(("<font strokecolor=000 strokesize=0.3f strokeopacity=50>" + myDSH->getStringForKey(kDSH_Key_nick) + "</font>").c_str());
				nick_marquee->startMarquee();
				nick_marquee->setFontSize(12.5f);
				nick_marquee->setAnchorPoint(ccp(0.5f,0.5f));
				nick_marquee->setPosition(ccp(93,list_cell_case->getContentSize().height/2.f + 5));
				list_cell_case->addChild(nick_marquee);
			}
			else
			{
				KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12.5f); // user_list[i]["nick"].asString().c_str()
				nick_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				nick_label->setPosition(ccp(93,list_cell_case->getContentSize().height/2.f + 5));
				list_cell_case->addChild(nick_label);
			}
			
			KSLabelTTF* score_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_hellContentTime), KS::insert_separator(result_data["myscore"].asInt64()/100.f, "%.1f").c_str()), mySGD->getFont().c_str(), 12);
			score_label->setColor(ccc3(54, 36, 148));
			score_label->disableOuterStroke();
			score_label->setAnchorPoint(ccp(1,0.5f));
			score_label->setPosition(ccp(135,list_cell_case->getContentSize().height/2.f - 7));
			list_cell_case->addChild(score_label);
		}
	}
	else
	{
		KSLabelTTF* fail_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failCheckRanking), mySGD->getFont().c_str(), 12);
		fail_label->setPosition(ccp(0,0));
		right_info_node->addChild(fail_label);
		
		called_stage = -1;
	}
}

unsigned int HellModeOpening::numberOfCellsInTableView(CCTableView *table)
{
	return hell_list.size();
}

void HellModeOpening::menuAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int t_tag = ((CCNode*)sender)->getTag();
	
	if(clicked_stage == t_tag)
	{
		CommonAnimation::closePopup(this, main_case, gray, [=](){
			
		}, [=](){
			mySGD->is_hell_mode = true;
			mySD->setSilType(clicked_stage);
			
			StartSettingPopup* t_popup = StartSettingPopup::create();
			t_popup->setHideFinalAction(getParent(), callfunc_selector(MainFlowScene::showHellOpening));
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			removeFromParent();
		});
	}
	else
	{
		clicked_stage = t_tag;
		
		setRight(t_tag);
		
		CCPoint t_offset = hell_table->getContentOffset();
		hell_table->reloadData();
		hell_table->setContentOffset(t_offset);
		
		is_menu_enable = true;
	}
}

CCTableViewCell* HellModeOpening::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	if(hell_list[idx].is_open)
	{
		CCSprite* n_back_img = mySIL->getLoadedImg(ccsf("hell_cell%d_img.png", idx+1));
		CCSprite* s_back_img = mySIL->getLoadedImg(ccsf("hell_cell%d_img.png", idx+1));
		s_back_img->setColor(ccGRAY);
		
		CCMenuItem* back_item = CCMenuItemSprite::create(n_back_img, s_back_img, this, menu_selector(HellModeOpening::menuAction));
		back_item->setTag(hell_list[idx].stage_number);
		
		ScrollMenu* back_menu = ScrollMenu::create(back_item, NULL);
		back_menu->setPosition(ccpFromSize(n_back_img->getContentSize()/2.f));
		cell->addChild(back_menu);
		
		back_menu->setTouchPriority(touch_priority-1);
		
		KSLabelTTF* reward_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_reward), mySGD->getFont().c_str(), 12);
		reward_label->enableOuterStroke(ccBLACK, 0.5f, 255, true);
		reward_label->setPosition(ccp(28,40));
		cell->addChild(reward_label);
		
		KSLabelTTF* character_name = KSLabelTTF::create(hell_list[idx].character_name.c_str(), mySGD->getFont().c_str(), 12);
		character_name->enableOuterStroke(ccBLACK, 0.5f, 255, true);
		character_name->setPosition(ccp(28,12));
		cell->addChild(character_name);
		
		KSLabelTTF* title_label = KSLabelTTF::create(hell_list[idx].title.c_str(), mySGD->getFont().c_str(), 18);
		title_label->setAnchorPoint(ccp(0,0.5f));
		title_label->setPosition(ccp(55,33));
		title_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
		title_label->enableOuterStroke(ccc3(60, 20, 0), 1.f, 255, true);
		cell->addChild(title_label);
		
		KSLabelTTF* content_label = KSLabelTTF::create(hell_list[idx].content.c_str(), mySGD->getFont().c_str(), 12);
		content_label->setAnchorPoint(ccp(0,0.5f));
		content_label->enableOuterStroke(ccBLACK, 0.5f, 255, true);
		content_label->setPosition(ccp(55,15));
		cell->addChild(content_label);
		
		if(clicked_stage == hell_list[idx].stage_number)
		{
			CCScale9Sprite* clicked_img = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
			clicked_img->setContentSize(CCSizeMake(268, 59));
			clicked_img->setPosition(ccpFromSize(n_back_img->getContentSize()/2.f));
			cell->addChild(clicked_img);
		}
		
		if(hell_list[idx].is_take)
		{
			CCScale9Sprite* take_back = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
			take_back->setContentSize(CCSizeMake(50, 25));
			take_back->setPosition(ccp(30,27));
			take_back->setRotation(-20);
			cell->addChild(take_back);
			
			KSLabelTTF* take_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_hellTakeCharacter), mySGD->getFont().c_str(), 12);
			take_label->setColor(ccc3(255, 255, 30));
			take_label->enableOuterStroke(ccBLACK, 0.3f, 60, true);
			take_label->setPosition(ccpFromSize(take_back->getContentSize()/2.f));
			take_back->addChild(take_label);
		}
	}
	else
	{
		CCSprite* n_back_img = mySIL->getLoadedImg(ccsf("hell_cell%d_img.png", idx+1));
		n_back_img->setPosition(ccpFromSize(n_back_img->getContentSize()/2.f));
		cell->addChild(n_back_img);
		
		KSLabelTTF* reward_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_reward), mySGD->getFont().c_str(), 12);
		reward_label->enableOuterStroke(ccBLACK, 0.5f, 255, true);
		reward_label->setPosition(ccp(28,40));
		cell->addChild(reward_label);
		
		KSLabelTTF* character_name = KSLabelTTF::create(hell_list[idx].character_name.c_str(), mySGD->getFont().c_str(), 12);
		character_name->enableOuterStroke(ccBLACK, 0.5f, 255, true);
		character_name->setPosition(ccp(28,12));
		cell->addChild(character_name);
		
		KSLabelTTF* title_label = KSLabelTTF::create(hell_list[idx].title.c_str(), mySGD->getFont().c_str(), 18);
		title_label->setAnchorPoint(ccp(0,0.5f));
		title_label->setPosition(ccp(55,33));
		title_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
		title_label->enableOuterStroke(ccc3(60, 20, 0), 1.f, 255, true);
		cell->addChild(title_label);
		
		KSLabelTTF* content_label = KSLabelTTF::create(hell_list[idx].content.c_str(), mySGD->getFont().c_str(), 12);
		content_label->setAnchorPoint(ccp(0,0.5f));
		content_label->enableOuterStroke(ccBLACK, 0.5f, 255, true);
		content_label->setPosition(ccp(55,15));
		cell->addChild(content_label);
		
		if(clicked_stage == hell_list[idx].stage_number)
		{
			CCScale9Sprite* clicked_img = CCScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
			clicked_img->setContentSize(CCSizeMake(268, 59));
			clicked_img->setPosition(ccpFromSize(n_back_img->getContentSize()/2.f));
			cell->addChild(clicked_img);
		}
		
		if(hell_list[idx].is_take)
		{
			CCScale9Sprite* take_back = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
			take_back->setContentSize(CCSizeMake(50, 25));
			take_back->setPosition(ccp(30,27));
			take_back->setRotation(-20);
			cell->addChild(take_back);
			
			KSLabelTTF* take_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_hellTakeCharacter), mySGD->getFont().c_str(), 12);
			take_label->setColor(ccc3(255, 255, 30));
			take_label->enableOuterStroke(ccBLACK, 0.3f, 60, true);
			take_label->setPosition(ccpFromSize(take_back->getContentSize()/2.f));
			take_back->addChild(take_label);
		}
		
		CCSprite* back_img = CCSprite::create("hell_lock.png");
		back_img->setPosition(ccpFromSize(back_img->getContentSize()/2.f));
		cell->addChild(back_img);
		
		StyledLabelTTF* lock_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_hellNotOpen), hell_list[idx].condition_stage), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
		lock_label->setAnchorPoint(ccp(0,0.5f));
		lock_label->setPosition(ccp(100, back_img->getContentSize().height/2.f));
		back_img->addChild(lock_label);
	}
	
	return cell;
}

void HellModeOpening::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	
}