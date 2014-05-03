//
//  MainFlowScene.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 16..
//
//

#include "MainFlowScene.h"
#include "DataStorageHub.h"
#include "CountingBMLabel.h"
#include "StarGoldData.h"
#include "HeartTime.h"
#include "ShopPopup.h"
#include "OptionPopup.h"
#include "RankPopup.h"
#include "CardSettingPopup.h"
#include "GachaPurchase.h"
#include "EventPopup.h"
#include "ServerDataSave.h"
#include "ASPopupView.h"
#include "NoticeContent.h"
#include "TicketRequestContent.h"
#include "PuzzleScene.h"
#include "StageListDown.h"
#include "AchievePopup.h"
#include "TutorialFlowStep.h"
#include "ClearPopup.h"
#include "FailPopup.h"
#include "AlertEngine.h"
#include "PuzzleListShadow.h"
#include "InviteEventPopup.h"
#include "TouchSuctionLayer.h"
#include "ScrollMenu.h"
#include "MyLocalization.h"
#include "NewMainFlowScene.h"
#include "RankNewPopup.h"
#include "SumranMailPopup.h"
#include "LoadingLayer.h"
#include "KSLabelTTF.h"
#include "DetailConditionPopup.h"
#include "GoodsLight.h"
#include "TodayMissionPopup.h"
#include "FormSetter.h"

CCScene* MainFlowScene::scene()
{
    CCScene *scene = CCScene::create();
    
    MainFlowScene *layer = MainFlowScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    scene->addChild(layer);
	
    return scene;
}

bool MainFlowScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
	
	is_table_openning = false;
	callTimeInfo();
	
	have_card_count_for_puzzle_index.clear();
	
	int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
	for(int i=1;i<=puzzle_count;i++)
	{
		int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
		int have_card_cnt = 0;
		int start_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_startStage_i);
		int stage_count = NSDS_GI(t_puzzle_number, kSDS_PZ_stageCount_i);
		
		int card_take_cnt = mySGD->getHasGottenCardsSize();
		for(int i=0;i<card_take_cnt;i++)
		{
			int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
			int card_stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
			if(card_stage_number >= start_stage && card_stage_number < start_stage+stage_count)
				have_card_cnt += NSDS_GI(kSDS_CI_int1_grade_i, card_number);
		}
		
		have_card_count_for_puzzle_index.push_back(have_card_cnt);
	}
	
	is_puzzle_enter_list.clear();
	int total_puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
	
	int locked_puzzle_count = 0;
	
	for(int i=1;i<=total_puzzle_count;i++)
	{
		int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
		
		PuzzleOpenInfo t_info;
		t_info.is_open = mySGD->getPuzzleHistory(t_puzzle_number).is_open;
		
		string puzzle_condition = NSDS_GS(t_puzzle_number, kSDS_PZ_condition_s);
		
		Json::Value condition_list;
		Json::Reader reader;
		reader.parse(puzzle_condition, condition_list);
		
		if(condition_list.size() <= 0)
			t_info.is_open = true;
		
		t_info.is_base_condition_success = true;
		t_info.is_have_week_condition = false;
		t_info.is_have_date_condition = false;
		t_info.is_have_ruby_condition = false;
		t_info.need_star_count = 0;
		
		for(int i=0;!t_info.is_open && i<condition_list.size();i++)
		{
			Json::Value t_condition_and = condition_list[i];
			
			bool and_open = true;
			bool is_time_condition = false;
			
			for(int j=0;j<t_condition_and.size();j++)
			{
				Json::Value t_condition = t_condition_and[j];
				string t_type = t_condition["type"].asString();
				if(t_type == "p")
				{
					if(!mySGD->getPuzzleHistory(t_condition["value"].asInt()).is_clear)
					{
						and_open = false;
						t_info.is_base_condition_success = false;
					}
				}
				else if(t_type == "s")
				{
					t_info.need_star_count = t_condition["value"].asInt();
					if(mySGD->getClearStarCount() < t_info.need_star_count)
					{
						and_open = false;
						t_info.is_base_condition_success = false;
					}
				}
				else if(t_type == "r")
				{
					t_info.need_ruby_value = t_condition["value"].asInt();
					and_open = false;
					t_info.is_have_ruby_condition = true;
				}
				else if(t_type == "w")
				{
					is_time_condition = true;
					t_info.is_have_week_condition = true;
					if(!mySGD->keep_time_info.is_loaded)
						and_open = false;
					else
					{
						int weekday = t_condition["weekday"].asInt();
						t_info.keep_weekday = weekday;
						if(mySGD->keep_time_info.weekday.getV() != -1 && mySGD->keep_time_info.weekday.getV() != weekday)
							and_open = false;
						t_info.keep_week_start = t_condition["s"].asInt();
						t_info.keep_week_end = t_condition["e"].asInt();
						if(mySGD->keep_time_info.hour.getV() < t_condition["s"].asInt() || mySGD->keep_time_info.hour.getV() >= t_condition["e"].asInt())
							and_open = false;
					}
				}
				else if(t_type == "d")
				{
					is_time_condition = true;
					t_info.is_have_date_condition = true;
					t_info.keep_date_start = t_condition["s"].asString();
					if(mySGD->keep_time_info.date.getV() < t_condition["s"].asInt64() || mySGD->keep_time_info.date.getV() >= t_condition["e"].asInt64())
						and_open = false;
				}
			}
			
			if(and_open)
			{
				t_info.is_open = true;
				if(!is_time_condition)
				{
					PuzzleHistory t_history = mySGD->getPuzzleHistory(t_puzzle_number);
					t_history.is_open = true;
					t_history.open_type = "무료";
					mySGD->setPuzzleHistory(t_history, nullptr);
				}
			}
		}
		
		t_info.before_locked_puzzle_count = locked_puzzle_count;
		is_puzzle_enter_list.push_back(t_info);
		
		if(!t_info.is_open)
			locked_puzzle_count++;
	}
	
	
	CCSpriteBatchNode* back_batch = CCSpriteBatchNode::create("ingame_side_pattern.png");
	back_batch->setPosition(ccp(240,160));
	addChild(back_batch, kMainFlowZorder_back);
	
	CCPoint base_position = ccp(-284,-180);
	
	for(int i=0;i*26 < 360;i++)
	{
		for(int j=0;j*48 < 568;j++)
		{
			CCSprite* t_back = CCSprite::createWithTexture(back_batch->getTexture());
			t_back->setAnchorPoint(ccp(0,0));
			t_back->setPosition(base_position + ccp(j*48, i*26));
			back_batch->addChild(t_back);
		}
	}
	
//	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
//	back_img->setPosition(ccp(240,160));
//	addChild(back_img, kMainFlowZorder_back);
	
	is_unlock_puzzle = mySGD->getIsUnlockPuzzle();
	mySGD->setIsUnlockPuzzle(0);
	
	setTable();
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;

//	CCSprite* back_shadow_left = CCSprite::create("mainflow_back_shadow_left.png");
//	back_shadow_left->setAnchorPoint(ccp(0.f,0.5f));
//	back_shadow_left->setPosition(ccp(-(screen_scale_x-1.f)*480.f/2.f,160));
//	addChild(back_shadow_left, kMainFlowZorder_top);
//	
//	CCSprite* back_shadow_right = CCSprite::create("mainflow_back_shadow_right.png");
//	back_shadow_right->setAnchorPoint(ccp(1.f,0.5f));
//	back_shadow_right->setPosition(ccp(480+(screen_scale_x-1.f)*480.f/2.f,160));
//	addChild(back_shadow_right, kMainFlowZorder_top);
	
	setTop();
	setBottom();
	
	bool is_openning = false;
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_init) // 실행 후 첫 접근시
	{
		AudioEngine::sharedInstance()->playSound("bgm_ui.mp3", true);
		
		is_openning = true;
		topOpenning();
		bottomOpenning();
		
		if(mySGD->getMustBeShowNotice())
		{
			ASPopupView* t_popup = ASPopupView::create(-200);
			
			float height_value = 320.f;
			if(myDSH->screen_convert_rate < 1.f)
				height_value = 320.f/myDSH->screen_convert_rate;
			
			if(height_value < myDSH->ui_top)
				height_value = myDSH->ui_top;
			
			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
			t_popup->setDimmedPosition(ccp(240, 160));
			t_popup->setBasePosition(ccp(240, 160));
			
			NoticeContent* t_container = NoticeContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
															   {
																   t_popup->removeFromParent();
															   }, mySGD->getNoticeList());
			t_popup->setContainerNode(t_container);
			addChild(t_popup, kMainFlowZorder_popup);
		}
		
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
	}
	else if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_stage)
	{
		bottomOpenning();
		topOnLight();
	}
	
	is_menu_enable = true;
	
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
	{
//		myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
		
		int take_level;
		if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 4;
		else if(mySGD->is_showtime)							take_level = 3;
		else if(mySGD->is_exchanged)						take_level = 2;
		else												take_level = 1;
		
		if(mySGD->isHasGottenCards(mySD->getSilType(), take_level) == 0)
		{
			mySGD->setClearRewardGold(NSDS_GI(kSDS_CI_int1_reward_i, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)));
			
			mySGD->addHasGottenCardNumber(NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
			
			keep_card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level);
			
			LoadingLayer* t_loading = LoadingLayer::create(-900);
			addChild(t_loading, kMainFlowZorder_popup+1);
			
			updateCardHistory(t_loading);
		}
		else
		{
			
		}
		
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		showClearPopup();
	}
	else if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_fail)
	{
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		showFailPopup();
	}
	else
	{
		TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
		
		if(recent_step == kTutorialFlowStep_puzzleClick)
		{
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_puzzleClick);
			addChild(t_tutorial, kMainFlowZorder_popup);
			
			tutorial_node = t_tutorial;
			
			puzzle_table->setTouchEnabled(false);
		}
		else if(recent_step == kTutorialFlowStep_cardCollectionClick)
		{
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_cardCollectionClick);
			addChild(t_tutorial, kMainFlowZorder_popup);
			
			tutorial_node = t_tutorial;
			
			puzzle_table->setTouchEnabled(false);
		}
		
		
		if(is_unlock_puzzle > 0)
		{
			is_menu_enable = false;
			puzzle_table->setTouchEnabled(false);
		}
		
		tableOpenning();
		if(!is_openning)
			topReturnMode();
	}
	
//	if(!myDSH->getBoolForKey(kDSH_Key_was_opened_tutorial_dimed_main))
//	{
//		myDSH->setBoolForKey(kDSH_Key_was_opened_tutorial_dimed_main, true);
//		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//		t_suction->target_touch_began = t_suction;
//		t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//		t_suction->setTouchEnabled(true);
//		
//		CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_main.png");
//		dimed_tip->setPosition(ccp(240,160));
//		t_suction->addChild(dimed_tip);
//		
//		addChild(t_suction, kMainFlowZorder_popup);
//	}
	
	return true;
}

void MainFlowScene::updateCardHistory(CCNode *t_loading)
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["cardNo"] = keep_card_number;
	
	hspConnector::get()->command("updateCardHistory", param, [=](Json::Value result_data)
								 {
									 if(result_data["result"]["code"].asInt() == GDSUCCESS)
									 {
										 t_loading->removeFromParent();
									 }
									 else
									 {
										 updateCardHistory(t_loading);
									 }
								 });
}

void MainFlowScene::tableOpenning()
{
	is_table_openning = true;
	
	int cell_cnt = NSDS_GI(kSDS_GI_puzzleListCount_i);
	int action_cell_count = 0;
	
	for(int i=0;i<cell_cnt;i++)
	{
		CCTableViewCell* t_cell = puzzle_table->cellAtIndex(i);
		
		if(t_cell)
		{
			CCNode* t_node = t_cell->getChildByTag(1);
			
			CCNode* t_rate = t_node->getChildByTag(999);
			if(t_rate)
				t_rate->setVisible(false);
			
			float origin_x = t_node->getPositionX();
			
			t_node->setPositionX(origin_x + 70);
			KS::setOpacity(t_node, 0);
			
			float delay_value = action_cell_count*0.1f;
			
			t_node->addChild(KSTimer::create(delay_value, [=]()
											 {
												 t_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t)
																								{
																									t_node->setPositionX(origin_x + 70-70.f*t);
																									KS::setOpacity(t_node, 255*t);
																								}, [=](float t)
																								{
																									t_node->setPositionX(origin_x);
																									KS::setOpacity(t_node, 255);
																									
																									CCNode* t_rate = t_node->getChildByTag(999);
																									if(t_rate)
																										t_rate->setVisible(true);
																								}));
											 }));
			
			action_cell_count++;
		}
	}
	
	addChild(KSTimer::create(action_cell_count*0.1f+0.2f, [=]()
	{
		is_table_openning = false;
		if(mySGD->keep_time_info.is_loaded)
			tableRefresh();
	}));
}

void MainFlowScene::showClearPopup()
{
	is_menu_enable = false;
	
	ClearPopup* t_popup = ClearPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::hideClearPopup));
	addChild(t_popup, kPuzzleZorder_popup);
}

void MainFlowScene::hideClearPopup()
{
	is_menu_enable = true;
}

void MainFlowScene::showFailPopup()
{
	is_menu_enable = false;
	
	FailPopup* t_popup = FailPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::hideFailPopup));
	addChild(t_popup, kPuzzleZorder_popup);
}

void MainFlowScene::hideFailPopup()
{
	is_menu_enable = true;
}

void MainFlowScene::setTable()
{
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	CCSize table_size = CCSizeMake(480*screen_scale_x, 245);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setPosition(ccp(myDSH->ui_zero_point.x, 160-table_size.height/2.f));
//	addChild(temp_back, kMainFlowZorder_table);
	
	puzzle_table = CCTableView::create(this, table_size);
	puzzle_table->setAnchorPoint(CCPointZero);
	puzzle_table->setDirection(kCCScrollViewDirectionHorizontal);
	puzzle_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	puzzle_table->setPosition(ccp((-480.f*screen_scale_x+480.f)/2.f, 160-table_size.height/2.f-5.f));
	puzzle_table->setDelegate(this);
	addChild(puzzle_table, kMainFlowZorder_table);
	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
	
	int puzzle_number;
	
	if(is_unlock_puzzle > 0)
		puzzle_number = is_unlock_puzzle;
	else
		puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	if(puzzle_number == 0 || puzzle_number > 10000)
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, NSDS_GI(kSDS_GI_puzzleList_int1_no_i, 1));
	
	int myPosition = puzzle_table->minContainerOffset().x;
	for(int i=0; i<numberOfCellsInTableView(puzzle_table); i++)
	{
		if(puzzle_number == NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1))
		{
			myPosition = i+1;
			break;
		}
	}
	float xInitPosition = MAX(puzzle_table->minContainerOffset().x, -cellSizeForTable(puzzle_table).width*myPosition + puzzle_table->getViewSize().width / 2.f);
	xInitPosition = MIN(0, xInitPosition);
//	puzzle_table->setContentOffsetInDuration(ccp(xInitPosition, 0), 0.3f);
	puzzle_table->setContentOffset(ccp(xInitPosition, 0));
}

enum MainFlowTableCellTag{
	kMainFlowTableCellTag_openBase = 0,
	kMainFlowTableCellTag_buyBase = 10000,
	kMainFlowTableCellTag_ticketBase = 20000
};

void MainFlowScene::basicEnter()
{
	topPuzzleMode();
	bottomPuzzleMode();
	tableEnter([=](){puzzleLoadSuccess();});
}

void MainFlowScene::loadTempFunc(CCObject* sender)
{
	
}

void MainFlowScene::tableEnter(function<void()> end_func)
{
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
//	mySIL->addImageAsync(CCString::createWithFormat("puzzle%d_original_top.png", puzzle_number)->getCString(), this, callfuncO_selector(MainFlowScene::loadTempFunc));
//	mySIL->addImageAsync(CCString::createWithFormat("puzzle%d_original_botton.png", puzzle_number)->getCString(), this, callfuncO_selector(MainFlowScene::loadTempFunc));
//	mySIL->addImageAsync(CCString::createWithFormat("puzzle%d_original_left.png", puzzle_number)->getCString(), this, callfuncO_selector(MainFlowScene::loadTempFunc));
//	mySIL->addImageAsync(CCString::createWithFormat("puzzle%d_original_right.png", puzzle_number)->getCString(), this, callfuncO_selector(MainFlowScene::loadTempFunc));
//	
//	int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
//	int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
//	
//	for(int i=start_stage;i<start_stage+stage_count;i++)
//	{
//		mySIL->addImageAsync(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number, NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, i)-1)->getCString(), this, callfuncO_selector(MainFlowScene::loadTempFunc));
//	}
	
	
	int cell_cnt = NSDS_GI(kSDS_GI_puzzleListCount_i);
	bool is_found = false;
	
	for(int i=0;i<cell_cnt;i++)
	{
		int index_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
		CCTableViewCell* t_cell = puzzle_table->cellAtIndex(i);
		
		if(is_found)
		{
			if(t_cell)
			{
				CCNode* t_node = t_cell->getChildByTag(1);
				float origin_x = t_node->getPositionX();
				t_node->addChild(KSGradualValue<float>::create(0.f, 800.f, 1.f, [=](float t)
															   {
																   t_node->setPositionX(origin_x + t);
															   }, [=](float t)
															   {
																   t_node->setPositionX(origin_x + 800.f);
															   }));
			}
		}
		else
		{
			if(index_puzzle_number == puzzle_number)
			{
				is_found = true;
				if(t_cell)
				{
					CCNode* t_node = t_cell->getChildByTag(1);
					
					CCNode* t_rate = t_node->getChildByTag(999);
					if(t_rate)
						t_rate->setVisible(false);
					
					t_node->addChild(KSGradualValue<float>::create(1.f, -0.2f, 0.6f, [=](float t)
																   {
																	   if(t >= 0.f)
																		   KS::setOpacity(t_node, 255*t);
																	   else
																		   KS::setOpacity(t_node, 0);
																   }, [=](float t)
																   {
																	   end_func();
																   }));
				}
			}
			else
			{
				if(t_cell)
				{
					CCNode* t_node = t_cell->getChildByTag(1);
					float origin_x = t_node->getPositionX();
					t_node->addChild(KSGradualValue<float>::create(0.f, -800.f, 1.f, [=](float t)
																   {
																	   t_node->setPositionX(origin_x + t);
																   }, [=](float t)
																   {
																	   t_node->setPositionX(origin_x - 800.f);
																   }));
				}
			}
		}
	}
}

void MainFlowScene::puzzleLoadSuccess()
{
	CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
}

void MainFlowScene::cellAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int tag = ((CCNode*)sender)->getTag();
//	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
//	
//	if(recent_step == kTutorialFlowStep_puzzleClick)
//	{
//		if(tag < kMainFlowTableCellTag_buyBase && tag - kMainFlowTableCellTag_openBase == 1)
//		{
//			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_pieceClick);
//			is_menu_enable = false;
//			
//			int puzzle_number = tag - kMainFlowTableCellTag_openBase;
//			myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, puzzle_number);
//			
//			StageListDown* t_sld = StageListDown::create(this, callfunc_selector(MainFlowScene::puzzleLoadSuccess), puzzle_number);
//			addChild(t_sld, kMainFlowZorder_popup);
//		}
//	}
//	else if(recent_step == kTutorialFlowStep_cardCollectionClick)
//	{
//		
//	}
//	else
//	{
		is_menu_enable = false;
		
		if(tag < kMainFlowTableCellTag_buyBase)
		{
			CCSprite* black_img = CCSprite::create("mainflow_puzzle_lock_black.png");
			black_img->setPosition(CCPointZero);
			black_img->setOpacity(100);
			CCNode* cell_node = ((CCNode*)sender)->getParent()->getParent();
			cell_node->addChild(black_img);
			
			CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
			CCBReader* reader = new CCBReader(nodeLoader);
			CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
			loading_progress_img->setPosition(ccp(0,30));
			cell_node->addChild(loading_progress_img);
			reader->release();
			
			
			int puzzle_number = tag - kMainFlowTableCellTag_openBase;
			myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, puzzle_number);
			
			StageListDown* t_sld = StageListDown::create(this, callfunc_selector(MainFlowScene::basicEnter), puzzle_number, [=](function<void()> t_func)
			{
				mySGD->is_before_stage_img_download = true;
				topOuting();
				bottomPuzzleMode();
				tableEnter(t_func);
			}, [=](){puzzleLoadSuccess();});
			addChild(t_sld, kMainFlowZorder_popup);
		}
		else if(tag < kMainFlowTableCellTag_ticketBase) // buyBase
		{
			AudioEngine::sharedInstance()->playEffect("se_lock.mp3", false);
			
			int puzzle_number = tag - kMainFlowTableCellTag_buyBase;
			CCLOG("puzzle_number : %d", puzzle_number);
			
			ASPopupView* t_popup = ASPopupView::create(-200);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top/myDSH->screen_convert_rate));
			
			CCNode* t_container = CCNode::create();
			t_popup->setContainerNode(t_container);
			addChild(t_popup);
			
			CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
			case_back->setPosition(CCPointZero);
			t_container->addChild(case_back);
			
			case_back->setContentSize(CCSizeMake(230, 250));
			
			CCScale9Sprite* content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
			content_back->setPosition(ccp(0,2));
			t_container->addChild(content_back);
			
			content_back->setContentSize(CCSizeMake(202, 146));
			
			CCLabelTTF* title_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_openRightNow), mySGD->getFont().c_str(), 20);
			title_label->setPosition(ccp(0, 102));
			t_container->addChild(title_label);
			
			CCLabelTTF* content_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_openRubyValue), NSDS_GI(puzzle_number, kSDS_PZ_point_i))->getCString(), mySGD->getFont().c_str(), 18);
			content_label->setPosition(CCPointZero);
			t_container->addChild(content_label);
			
			CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
			CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
			s_close->setColor(ccGRAY);
			
			CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
																				{
																					is_menu_enable = true;
																					t_popup->removeFromParent();
																				});
			
			CCMenuLambda* close_menu = CCMenuLambda::createWithItem(close_item);
			close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
			close_menu->setPosition(ccp(92,105));
			t_container->addChild(close_menu);
			
			if(mySGD->getGoodsValue(kGoodsType_ruby) >= NSDS_GI(puzzle_number, kSDS_PZ_point_i))
			{
				CCSprite* n_buy = CCSprite::create("popup2_buy.png");
				CCSprite* s_buy = CCSprite::create("popup2_buy.png");
				s_buy->setColor(ccGRAY);
				
				CCMenuItemSpriteLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender){
					
					AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
					
					LoadingLayer* t_loading = LoadingLayer::create(-9999);
					addChild(t_loading, 9999);
					
					mySGD->addChangeGoods(kGoodsType_ruby, -NSDS_GI(puzzle_number, kSDS_PZ_point_i), "퍼즐오픈", CCString::createWithFormat("%d", puzzle_number)->getCString());
					
					int open_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, mySGD->getOpenPuzzleCount()+1);
					PuzzleHistory t_history = mySGD->getPuzzleHistory(open_puzzle_number);
					t_history.is_open = true;
					t_history.open_type = "루비소모";
					
					mySGD->changeGoodsTransaction({mySGD->getUpdatePuzzleHistoryParam(t_history, [=](Json::Value result_data)
																					  {
																						  if(result_data["result"]["code"].asInt() != GDSUCCESS)
																						  {
																							  PuzzleHistory r_history = mySGD->getPuzzleHistory(open_puzzle_number);
																							  r_history.is_open = false;
																							  r_history.open_type = "";
																							  mySGD->setPuzzleHistoryForNotSave(r_history);
																						  }
																					  })}, [=](Json::Value result_data)
												  {
													  t_loading->removeFromParent();
													  if(result_data["result"]["code"].asInt() != GDSUCCESS)
														{
															mySGD->clearChangeGoods();
															addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
														}
													  else
														{
															int found_idx = -1;
															for(int i=0;i<numberOfCellsInTableView(puzzle_table) && found_idx == -1;i++)
															{
																CCTableViewCell* t_cell = puzzle_table->cellAtIndex(i);
																if(t_cell)
																{
																	int cell_card_number = t_cell->getTag();
																	if(cell_card_number == puzzle_number)
																		found_idx = i;
																}
															}
															if(found_idx != -1)
																puzzle_table->updateCellAtIndex(found_idx);
															
															is_menu_enable = true;
															t_popup->removeFromParent();
														}
												  });
				});
				
				CCMenuLambda* buy_menu = CCMenuLambda::createWithItem(buy_item);
				buy_menu->setTouchPriority(t_popup->getTouchPriority()-1);
				buy_menu->setPosition(ccp(0,-95));
				t_container->addChild(buy_menu);
			}
			else
			{
				CCSprite* buy_img = CCSprite::create("popup2_buy.png");
				buy_img->setColor(ccc3(100, 100, 100));
				buy_img->setPosition(ccp(0,-95));
				t_container->addChild(buy_img);
			}
		}
		else // ticketBase
		{
			AudioEngine::sharedInstance()->playEffect("se_lock.mp3", false);
			
			int puzzle_number = tag - kMainFlowTableCellTag_ticketBase;
			
			ASPopupView* t_popup = ASPopupView::create(-200);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top/myDSH->screen_convert_rate));
			
			TicketRequestContent* t_container = TicketRequestContent::create(t_popup->getTouchPriority(), puzzle_number);
			t_popup->setContainerNode(t_container);
			addChild(t_popup);
			
			t_container->setRemoveAction([=](){
				is_menu_enable = true;
				t_popup->removeFromParent();
			});
		}
//	}
}
CCTableViewCell* MainFlowScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCNode* cell_node = CCNode::create();
	cell_node->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f));
	cell->addChild(cell_node, 1, 1);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
//	CCSize table_size = CCSizeMake(480*screen_scale_x, 245);
	
	int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx+1);
	cell->setTag(puzzle_number);
	
//	if(puzzle_number == is_unlock_puzzle)
//	{
//		if(NSDS_GI(puzzle_number, kSDS_PZ_point_i) <= 0 || NSDS_GI(puzzle_number, kSDS_PZ_ticket_i) <= 0)
//		{
//			int open_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, mySGD->getOpenPuzzleCount()+1);
//			PuzzleHistory t_history = mySGD->getPuzzleHistory(open_puzzle_number);
//			t_history.is_open = true;
//			t_history.open_type = "무료(이전퍼즐완료)";
//			mySGD->setPuzzleHistory(t_history, nullptr);
//			
//			is_open = true;
//		}
//	}
	
	if(is_puzzle_enter_list[idx].is_open)
//	if(puzzle_number == 1 || mySGD->getPuzzleHistory(puzzle_number).is_open || (mySGD->getPuzzleHistory(puzzle_number-1).is_clear && NSDS_GI(puzzle_number, kSDS_PZ_point_i) == 0))
//	if(puzzle_number == 1 || 9999+1 >= puzzle_number)
	{
		
		addChild(KSTimer::create(rand()%10*0.2f, [=](){
			CCSprite* mainTwinkle = KS::loadCCBI<CCSprite*>(this, "main_twinkle.ccbi").first;
			mainTwinkle->setAnchorPoint(ccp(0,0));
			mainTwinkle->setPosition(ccp(mainTwinkle->getContentSize().width/2,mainTwinkle->getContentSize().height/2));
			cell_node->addChild(mainTwinkle,100);
		}));
		
		CCSprite* n_open_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", puzzle_number)->getCString());//CCSprite::create("mainflow_puzzle_open_back.png");
		CCSprite* s_open_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", puzzle_number)->getCString());//CCSprite::create("mainflow_puzzle_open_back.png");
		s_open_back->setColor(ccGRAY);
		
		
		CCMenuItem* open_item = CCMenuItemSprite::create(n_open_back, s_open_back, this, menu_selector(MainFlowScene::cellAction));
		open_item->setTag(kMainFlowTableCellTag_openBase + puzzle_number);
		
		ScrollMenu* open_menu = ScrollMenu::create(open_item, NULL);
		open_menu->setPosition(CCPointZero);
		cell_node->addChild(open_menu);
		
		int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
		int total_card_cnt = stage_count*10;
		
		CCLabelTTF* rate_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", have_card_count_for_puzzle_index[idx], total_card_cnt)->getCString(), mySGD->getFont().c_str(), 10);
		rate_label->setPosition(ccp(-25, -81));
		cell_node->addChild(rate_label);
		
		CCProgressTimer* rate_timer = CCProgressTimer::create(CCSprite::create("mainflow_puzzle_bar.png"));
		rate_timer->setType(kCCProgressTimerTypeBar);
		rate_timer->setMidpoint(ccp(0,0));
		rate_timer->setBarChangeRate(ccp(1,0));
		rate_timer->setPercentage(100.f*have_card_count_for_puzzle_index[idx]/total_card_cnt);
		rate_timer->setPosition(ccp(22, -80));
		cell_node->addChild(rate_timer, 0, 999);

		
//		PuzzleListShadow* shadow_node = PuzzleListShadow::create(this, cell, ccpAdd(ccp((-480.f*screen_scale_x+480.f)/2.f, 160-table_size.height/2.f), ccp(table_size.width/2.f, table_size.height/2.f)), ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f), ccp(1.f,0), ccp(0.2f,0));
//		cell->addChild(shadow_node, -1);
//		shadow_node->startAction();
//		
//		CCSprite* shadow_img = CCSprite::create("mainflow_puzzle_shadow.png");
//		shadow_node->addChild(shadow_img, -1);
	}
	else
	{
		CCSprite* close_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", puzzle_number)->getCString());
		close_back->setPosition(CCPointZero);
		cell_node->addChild(close_back);
		
		CCLabelTTF* rate_label = CCLabelTTF::create("0/50", mySGD->getFont().c_str(), 10);
		rate_label->setPosition(ccp(-25, -81));
		cell_node->addChild(rate_label);
		
		if(is_puzzle_enter_list[idx].is_base_condition_success)//mySGD->getPuzzleHistory(puzzle_number-1).is_clear) // 기본조건 충족 했는가
		{
			// 루비 구매 혹은 시간이 되야 열림
			
//			CCSprite* n_buy = CCSprite::create("mainflow_puzzle_open_buy.png");
//			CCSprite* s_buy = CCSprite::create("mainflow_puzzle_open_buy.png");
//			s_buy->setColor(ccGRAY);
//			
//			CCMenuItem* buy_item = CCMenuItemSprite::create(n_buy, s_buy, this, menu_selector(MainFlowScene::cellAction));
//			buy_item->setTag(kMainFlowTableCellTag_buyBase + puzzle_number);
//			
//			ScrollMenu* buy_menu = ScrollMenu::create(buy_item, NULL);
//			buy_menu->setPosition(ccp(-24, -16));
//			cell_node->addChild(buy_menu);
			
			int lock_img_number = is_puzzle_enter_list[idx].before_locked_puzzle_count;
			if(lock_img_number > 1)
				lock_img_number = 1;
			lock_img_number++;
			
			CCSprite* not_clear_img = CCSprite::create(CCString::createWithFormat("mainflow_puzzle_lock_base%d.png", lock_img_number)->getCString());
			not_clear_img->setPosition(close_back->getPosition());
			cell_node->addChild(not_clear_img);
			
			if(is_puzzle_enter_list[idx].before_locked_puzzle_count == 0)
			{
				KSLabelTTF* condition_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_frameOpenConditionTitle), mySGD->getFont().c_str(), 10);
				condition_title->setColor(ccc3(255, 177, 38));
				condition_title->setPosition(ccp(67.5f, 143.5f));
				not_clear_img->addChild(condition_title);
				
				if(is_puzzle_enter_list[idx].is_have_week_condition)
				{
					string weekday_string;
					if(is_puzzle_enter_list[idx].keep_weekday == -1)
						weekday_string = "매일";
					else if(is_puzzle_enter_list[idx].keep_weekday == 0)
						weekday_string = "일요일";
					else if(is_puzzle_enter_list[idx].keep_weekday == 1)
						weekday_string = "월요일";
					else if(is_puzzle_enter_list[idx].keep_weekday == 2)
						weekday_string = "화요일";
					else if(is_puzzle_enter_list[idx].keep_weekday == 3)
						weekday_string = "수요일";
					else if(is_puzzle_enter_list[idx].keep_weekday == 4)
						weekday_string = "목요일";
					else if(is_puzzle_enter_list[idx].keep_weekday == 5)
						weekday_string = "금요일";
					else if(is_puzzle_enter_list[idx].keep_weekday == 6)
						weekday_string = "토요일";
					
					KSLabelTTF* condition_content = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_frameOpenConditionContentTimeWeek), weekday_string.c_str(), is_puzzle_enter_list[idx].keep_week_start, is_puzzle_enter_list[idx].keep_week_end)->getCString(), mySGD->getFont().c_str(), 9);
					condition_content->setPosition(ccp(67.5f, 122.5f));
					not_clear_img->addChild(condition_content);
				}
				else if(is_puzzle_enter_list[idx].is_have_date_condition)
				{
					KSLabelTTF* condition_content = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_frameOpenConditionContentTimeDate), is_puzzle_enter_list[idx].keep_date_start.substr(4,2).c_str(), is_puzzle_enter_list[idx].keep_date_start.substr(6,2).c_str(), is_puzzle_enter_list[idx].keep_date_start.substr(8,2).c_str(), is_puzzle_enter_list[idx].keep_date_start.substr(10,2).c_str())->getCString(), mySGD->getFont().c_str(), 9);
					condition_content->setPosition(ccp(67.5f, 122.5f));
					not_clear_img->addChild(condition_content);
				}
				else
				{
					KSLabelTTF* condition_content = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_frameOpenConditionContentRuby), mySGD->getFont().c_str(), 9);
					condition_content->setPosition(ccp(67.5f, 122.5f));
					not_clear_img->addChild(condition_content);
				}
				
				if(is_puzzle_enter_list[idx].is_have_ruby_condition)
				{
					CCLabelTTF* c_label = CCLabelTTF::create();
					
					CCSprite* price_type = CCSprite::create("price_ruby_img.png");
					price_type->setScale(0.7f);
					c_label->addChild(price_type);
					
					KSLabelTTF* price_value_label = KSLabelTTF::create(CCString::createWithFormat("%d  ", is_puzzle_enter_list[idx].need_ruby_value)->getCString(), mySGD->getFont().c_str(), 10);
					c_label->addChild(price_value_label);
					
					KSLabelTTF* detail_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_directEnter), mySGD->getFont().c_str(), 10);
					c_label->addChild(detail_label);
					
					float t_width = price_type->getContentSize().width*price_type->getScale();
					float v_width = price_value_label->getContentSize().width;
					float d_width = detail_label->getContentSize().width;
					
					price_type->setPositionX(-(v_width+d_width - (t_width+v_width+d_width)/2.f + t_width/2.f));
					price_value_label->setPositionX(-(d_width-(t_width+v_width+d_width)/2.f + v_width/2.f));
					detail_label->setPositionX(price_value_label->getPositionX() + v_width/2.f + d_width/2.f);
					
					CCScale9Sprite* detail_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
					
					CCControlButton* detail_button = CCControlButton::create(c_label, detail_back);
					detail_button->addTargetWithActionForControlEvents(this, cccontrol_selector(MainFlowScene::detailCondition), CCControlEventTouchUpInside);
					detail_button->setTag(10000000 + idx*10000 + is_puzzle_enter_list[idx].need_ruby_value);
					detail_button->setPreferredSize(CCSizeMake(90,35));
					detail_button->setPosition(ccp(67.5f,94.5f));
					not_clear_img->addChild(detail_button);
				}
				else
				{
					CCLabelTTF* c_label = CCLabelTTF::create();
					KSLabelTTF* detail_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailView), mySGD->getFont().c_str(), 10);
					detail_label->setPosition(ccp(0,0));
					c_label->addChild(detail_label);
					
					CCScale9Sprite* detail_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
					
					CCControlButton* detail_button = CCControlButton::create(c_label, detail_back);
					detail_button->addTargetWithActionForControlEvents(this, cccontrol_selector(MainFlowScene::detailCondition), CCControlEventTouchUpInside);
					detail_button->setTag(0);
					detail_button->setPreferredSize(CCSizeMake(75,35));
					detail_button->setPosition(ccp(67.5f,94.5f));
					not_clear_img->addChild(detail_button);
				}
			}
			else
			{
				KSLabelTTF* not_clear_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_beforeNotClearPuzzle), mySGD->getFont().c_str(), 12);
				not_clear_label->enableOuterStroke(ccBLACK, 1.f);
				not_clear_label->setPosition(ccp(67.5f,138.5f));
				not_clear_img->addChild(not_clear_label);
			}
		}
		else
		{
			int lock_img_number = is_puzzle_enter_list[idx].before_locked_puzzle_count;
			if(lock_img_number > 1)
				lock_img_number = 1;
			lock_img_number++;
			
			CCSprite* not_clear_img = CCSprite::create(CCString::createWithFormat("mainflow_puzzle_lock_base%d.png", lock_img_number)->getCString());
			not_clear_img->setPosition(close_back->getPosition());
			cell_node->addChild(not_clear_img);
			
			if(is_puzzle_enter_list[idx].before_locked_puzzle_count == 0)
			{
				KSLabelTTF* condition_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_frameOpenConditionTitle), mySGD->getFont().c_str(), 10);
				condition_title->setColor(ccc3(255, 177, 38));
				condition_title->setPosition(ccp(67.5f, 143.5f));
				not_clear_img->addChild(condition_title);
				
				KSLabelTTF* condition_content = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_frameOpenConditionContent), is_puzzle_enter_list[idx].need_star_count)->getCString(), mySGD->getFont().c_str(), 9);
				condition_content->setPosition(ccp(67.5f, 122.5f));
				not_clear_img->addChild(condition_content);
				
				CCLabelTTF* c_label = CCLabelTTF::create();
				KSLabelTTF* detail_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailView), mySGD->getFont().c_str(), 10);
				detail_label->setPosition(ccp(0,0));
				c_label->addChild(detail_label);
				
				CCScale9Sprite* detail_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
				
				CCControlButton* detail_button = CCControlButton::create(c_label, detail_back);
				detail_button->addTargetWithActionForControlEvents(this, cccontrol_selector(MainFlowScene::detailCondition), CCControlEventTouchUpInside);
				detail_button->setTag(0);
				detail_button->setPreferredSize(CCSizeMake(75,35));
				detail_button->setPosition(ccp(67.5f,94.5f));
				not_clear_img->addChild(detail_button);
			}
			else
			{
				KSLabelTTF* not_clear_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_beforeNotClearPuzzle), mySGD->getFont().c_str(), 10);
				not_clear_label->enableOuterStroke(ccBLACK, 1.f);
				not_clear_label->setPosition(ccp(67.5f,138.5f));
				not_clear_img->addChild(not_clear_label);
			}
		}
		
//		PuzzleListShadow* shadow_node = PuzzleListShadow::create(this, cell, ccpAdd(ccp((-480.f*screen_scale_x+480.f)/2.f, 160-table_size.height/2.f), ccp(table_size.width/2.f, table_size.height/2.f)), ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f), ccp(1.f,0), ccp(0.2f,0));
//		cell->addChild(shadow_node, -1);
//		shadow_node->startAction();
//		
//		CCSprite* shadow_img = CCSprite::create("mainflow_puzzle_shadow.png");
//		shadow_node->addChild(shadow_img, -1);
	}
	
	if(puzzle_number == is_unlock_puzzle)
	{
		CCSprite* puzzle_unlock_beam = CCSprite::create("puzzle_unlock_beam.png");
		puzzle_unlock_beam->setPosition(CCPointZero);
		cell_node->addChild(puzzle_unlock_beam, -1);
		puzzle_unlock_beam->setOpacity(0);
		
		CCScaleTo* beam_scale1 = CCScaleTo::create(0.2f, 1.4f);
		CCDelayTime* beam_scale_delay = CCDelayTime::create(0.5f);
		CCScaleTo* beam_scale2 = CCScaleTo::create(0.2f, 1.f);
		CCSequence* beam_scale_seq = CCSequence::create(beam_scale1, beam_scale_delay, beam_scale2, NULL);
		
		CCRotateBy* beam_rotate = CCRotateBy::create(0.9f, 80);
		
		CCFadeTo* beam_fade1 = CCFadeTo::create(0.2f, 200);
		CCDelayTime* beam_fade_delay = CCDelayTime::create(0.5f);
		CCFadeTo* beam_fade2 = CCFadeTo::create(0.2f, 0);
		CCSequence* beam_fade_seq = CCSequence::create(beam_fade1, beam_fade_delay, beam_fade2, NULL);
		
		CCDelayTime* beam_delay = CCDelayTime::create(0.5f);
		CCSpawn* beam_spawn = CCSpawn::create(beam_scale_seq, beam_rotate, beam_fade_seq, NULL);
		CCCallFunc* beam_remove = CCCallFunc::create(puzzle_unlock_beam, callfunc_selector(CCSprite::removeFromParent));
		CCSequence* beam_seq = CCSequence::create(beam_delay, beam_spawn, beam_remove, NULL);
		
		puzzle_unlock_beam->runAction(beam_seq);
		
		CCSprite* not_clear_img = CCSprite::create("mainflow_puzzle_lock_base1.png");
		not_clear_img->setPosition(CCPointZero);
		cell_node->addChild(not_clear_img);
		
		KSLabelTTF* not_clear_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_beforeNotClearPuzzle), mySGD->getFont().c_str(), 12);
		not_clear_label->enableOuterStroke(ccBLACK, 1.f);
		not_clear_label->setPosition(ccp(67.5f,138.5f));
		not_clear_img->addChild(not_clear_label);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
		CCFadeTo* t_fade1 = CCFadeTo::create(0.2f, 0);
		CCCallFunc* t_remove_self1 = CCCallFunc::create(not_clear_img, callfunc_selector(CCNode::removeFromParent));
		CCSequence* t_seq1 = CCSequence::create(t_delay1, t_fade1, t_remove_self1, NULL);
		not_clear_img->runAction(t_seq1);
		
		is_unlock_puzzle = 0;
		
		cell_node->addChild(KSTimer::create(0.7f, [=]()
											{
												AudioEngine::sharedInstance()->playEffect("se_puzzleopen_2.mp3", false);
												cell_node->addChild(KSGradualValue<float>::create(0.f, 3.f, 0.5f, [=](float t)
																			  {
																				  if(t < 1.f)
																				  {
																					  cell_node->setScale(1.f+0.1f*t);
																				  }
																				  else if(t < 2.f)
																				  {
																					  cell_node->setScale(1.1f);
																				  }
																				  else
																				  {
																					  cell_node->setScale(1.1f-0.1f*(t-2.f));
																				  }
																			  }, [=](float t)
																			  {
																				  cell_node->setScale(1.f);
																				  endUnlockAnimation();
																			  }));
											}));
	}
	
	return cell;
}

void MainFlowScene::detailCondition(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == 0)
	{
		DetailConditionPopup* t_popup = DetailConditionPopup::create(-800, [=](){is_menu_enable = true;});
		addChild(t_popup, kMainFlowZorder_popup);
	}
	else
	{
		int t_index = (tag-10000000)/10000;
		int t_need_ruby = tag%10000;
		
		LoadingLayer* t_loading = LoadingLayer::create(-9999);
		addChild(t_loading, 9999);
		
		int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, t_index+1);
		PuzzleHistory t_history = mySGD->getPuzzleHistory(puzzle_number);
		t_history.is_open = true;
		t_history.open_type = "루비결재";
		
		mySGD->addChangeGoods(kGoodsType_ruby, -t_need_ruby, "퍼즐오픈", CCString::createWithFormat("%d", puzzle_number)->getCString());
		
		vector<CommandParam> command_list;
		
		command_list.push_back(mySGD->getUpdatePuzzleHistoryParam(t_history, nullptr));
		
		mySGD->changeGoodsTransaction(command_list, [=](Json::Value result_data)
									  {
										  t_loading->removeFromParent();
										  if(result_data["result"]["code"].asInt() == GDSUCCESS)
										  {
											  is_menu_enable = true;
											  puzzle_table->updateCellAtIndex(t_index);
										  }
										  else
										  {
											  mySGD->clearChangeGoods();
											  PuzzleHistory t_history = mySGD->getPuzzleHistory(puzzle_number);
											  t_history.is_open = false;
											  t_history.open_type = "";
											  mySGD->setPuzzleHistoryForNotSave(t_history);
											  
											  addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-9999, kGoodsType_ruby, [=]()
											  {
												  is_menu_enable = false;
												  ShopPopup* t_shop = ShopPopup::create();
												  t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
												  t_shop->targetHeartTime(heart_time);
												  t_shop->setShopCode(kSC_ruby);
												  t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
												  addChild(t_shop, kMainFlowZorder_popup);
											  }), 9999);
											  
											  is_menu_enable = true;
										  }
									  });
	}
}

void MainFlowScene::endUnlockAnimation()
{
	puzzle_table->setTouchEnabled(true);
	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
	is_menu_enable = true;
}

void MainFlowScene::scrollViewDidScroll(CCScrollView* view){}
void MainFlowScene::scrollViewDidZoom(CCScrollView* view){}
void MainFlowScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell){}
CCSize MainFlowScene::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(132, 245);//135, 245);
}
unsigned int MainFlowScene::numberOfCellsInTableView(CCTableView *table)
{
	int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
	return puzzle_count;
	
//	return NSDS_GI(kSDS_GI_puzzleListCount_i);// eventListCount_i);
}

enum MainFlowMenuTag{
	kMainFlowMenuTag_rubyShop = 1,
	kMainFlowMenuTag_goldShop,
	kMainFlowMenuTag_heartShop,
	kMainFlowMenuTag_friendPointContent,
	kMainFlowMenuTag_postbox,
	kMainFlowMenuTag_option,
	kMainFlowMenuTag_tip,
	kMainFlowMenuTag_rank,
	kMainFlowMenuTag_shop,
	kMainFlowMenuTag_cardSetting,
	kMainFlowMenuTag_mission,
	kMainFlowMenuTag_friendManagement,
	kMainFlowMenuTag_gacha,
	kMainFlowMenuTag_achievement,
	kMainFlowMenuTag_event
};

void MainFlowScene::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int tag = ((CCNode*)sender)->getTag();
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_puzzleClick)
	{
		return;
	}
	else if(recent_step == kTutorialFlowStep_cardCollectionClick)
	{
		if(tag == kMainFlowMenuTag_cardSetting)
		{
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_upgradeClick);
			is_menu_enable = false;
			
			mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
			CardSettingPopup* t_popup = CardSettingPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::tutorialCardSettingClose));
			addChild(t_popup, kMainFlowZorder_popup);
			
			removeChild(tutorial_node);
		}
	}
	else
	{
		is_menu_enable = false;
		
		if(tag == kMainFlowMenuTag_rubyShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_ruby);
			t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
			addChild(t_shop, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_goldShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_gold);
			t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
			addChild(t_shop, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_heartShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_heart);
			t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
			addChild(t_shop, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_friendPointContent)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-300);
			t_suction->setTouchEnabled(true);
			t_suction->target_touch_began = this;
			t_suction->delegate_touch_began = callfunc_selector(MainFlowScene::closeFriendPoint);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
			CCSprite* stencil_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 78, 150));
			stencil_node->setPosition(ccp(336,229+160.f*(screen_scale_y-1.f)));
			CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
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
			cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
			cliping_node->setAlphaThreshold(0.05f);
			cliping_node->setPosition(CCPointZero);
			t_suction->addChild(cliping_node);
			
			CCSprite* inner_img = CCSprite::create("candy_popup.png");
			inner_img->setPosition(ccp(336,229+160.f*(screen_scale_y-1.f)+150));
			cliping_node->addChild(inner_img);
			
			CCMoveTo* t_move_down = CCMoveTo::create(0.3f, ccp(336,229+160.f*(screen_scale_y-1.f)));
			inner_img->runAction(t_move_down);
			
			close_friend_point_action = [=](){
				t_suction->target_touch_began = NULL;
				t_suction->delegate_touch_began = NULL;
				
				CCMoveTo* t_move_up = CCMoveTo::create(0.3f, ccp(336,229+160.f*(screen_scale_y-1.f)+150));
				CCCallFunc* t_call = CCCallFunc::create(t_suction, callfunc_selector(CCLayer::removeFromParent));
				CCSequence* t_seq = CCSequence::create(t_move_up, t_call, NULL);
				inner_img->runAction(t_seq);
			};
			addChild(t_suction, kMainFlowZorder_top-1);
			
			is_menu_enable = true;
		}
		else if(tag == kMainFlowMenuTag_postbox)
		{
			SumranMailPopup* t_pp = SumranMailPopup::create(this, callfunc_selector(MainFlowScene::mailPopupClose), bind(&MainFlowScene::heartRefresh, this));
			addChild(t_pp, kMainFlowZorder_popup);
			
			postbox_count_case->setVisible(false);
		}
		else if(tag == kMainFlowMenuTag_option)
		{
			OptionPopup* t_popup = OptionPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			addChild(t_popup, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_tip)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
			t_suction->target_touch_began = t_suction;
			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			t_suction->setTouchEnabled(true);
			
			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_main.png");
			dimed_tip->setPosition(ccp(240,160));
			t_suction->addChild(dimed_tip);
			
			addChild(t_suction, kMainFlowZorder_popup);
			
			is_menu_enable = true;
		}
		else if(tag == kMainFlowMenuTag_rank)
		{
			RankNewPopup* t_popup = RankNewPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			addChild(t_popup, kMainFlowZorder_popup);
			
//			RankPopup* t_rp = RankPopup::create(this, callfunc_selector(MainFlowScene::popupClose));
//			addChild(t_rp, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_shop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_ruby);
			t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
			addChild(t_shop, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_cardSetting)
		{
			puzzle_table->setTouchEnabled(false);
			mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
			CardSettingPopup* t_popup = CardSettingPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::tutorialCardSettingClose));
			addChild(t_popup, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_mission)
		{
			TodayMissionPopup* t_popup = TodayMissionPopup::create(-300, [=](){is_menu_enable = true;});
			addChild(t_popup, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_friendManagement)
		{
			is_menu_enable = true;

			InviteEventPopup* t_rp = InviteEventPopup::create(this, callfunc_selector(MainFlowScene::tutorialCardSettingClose));

			addChild(t_rp, kMainFlowZorder_popup);

		}
		else if(tag == kMainFlowMenuTag_gacha)
		{
			GachaPurchase* t_gp = GachaPurchase::create();
			addChild(t_gp, kMainFlowZorder_popup);
			
			t_gp->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_gp->setOutAllObjectAction(NULL, NULL);
			t_gp->setInAllObjectAction(this, callfunc_selector(MainFlowScene::popupClose));
		}
		else if(tag == kMainFlowMenuTag_achievement)
		{
			AchievePopup* t_ap = AchievePopup::create();
			addChild(t_ap, kMainFlowZorder_popup);
			
			t_ap->setHideFinalAction(this, callfunc_selector(MainFlowScene::achievePopupClose));
		}
		else if(tag == kMainFlowMenuTag_event)
		{
			CCDirector::sharedDirector()->replaceScene(NewMainFlowScene::scene());
			
//			is_menu_enable = true; // 임시
//			EventPopup* t_popup = EventPopup::create();
//			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
//			addChild(t_popup, kMainFlowZorder_popup);
		}
	}
}

void MainFlowScene::bottomOpenning()
{
	for(int i=0;i<bottom_list.size();i++)
	{
		CCPoint original_position = bottom_list[i]->getPosition();
		bottom_list[i]->setPositionY(original_position.y-100);
		
		CCDelayTime* t_delay = CCDelayTime::create(i*0.1f);
		CCMoveTo* t_move = CCMoveTo::create(0.2f, original_position);
		CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
		
		bottom_list[i]->runAction(t_seq);
	}
}

void MainFlowScene::bottomPuzzleMode()
{
	for(int i=0;i<bottom_list.size();i++)
	{
		CCDelayTime* t_delay = CCDelayTime::create(i*0.1f);
		CCMoveTo* t_move = CCMoveTo::create(0.2f, ccp(bottom_list[i]->getPositionX(), bottom_list[i]->getPositionY()-100));
		CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
		
		bottom_list[i]->runAction(t_seq);
	}
}

void MainFlowScene::setBottom()
{
	bottom_list.clear();
	
	CCNode* bottom_case = CCNode::create();
	bottom_case->setAnchorPoint(ccp(0.5f,0.5f));
	bottom_case->setPosition(ccp(240,-(myDSH->puzzle_ui_top-320.f)/2.f+10));
	addChild(bottom_case, kMainFlowZorder_uiButton);
	
	bottom_list.push_back(bottom_case);
	
	
	CCSprite* n_rank = CCSprite::create("mainflow_rank.png");
	KSLabelTTF* n_rank_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ranking), mySGD->getFont().c_str(), 12);
	n_rank_label->enableOuterStroke(ccBLACK, 1.f);
	n_rank_label->setPosition(ccp(n_rank->getContentSize().width/2.f, 7));
	n_rank->addChild(n_rank_label);
	CCSprite* s_rank = CCSprite::create("mainflow_rank.png");
	s_rank->setColor(ccGRAY);
	KSLabelTTF* s_rank_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ranking), mySGD->getFont().c_str(), 12);
	s_rank_label->enableOuterStroke(ccBLACK, 1.f);
	s_rank_label->setPosition(ccp(s_rank->getContentSize().width/2.f, 7));
	s_rank->addChild(s_rank_label);
	
	CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(MainFlowScene::menuAction));
	rank_item->setTag(kMainFlowMenuTag_rank);
	
	CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
	rank_menu->setPosition(ccp(43-240, n_rank->getContentSize().height/2.f+8));//ccp(-205, n_rank->getContentSize().height/2.f));
	bottom_case->addChild(rank_menu);
	rank_menu->setTouchPriority(kCCMenuHandlerPriority-1);
	
	{
		CCSprite* t_bar = CCSprite::create("mainflow_bottom_case_bar.png");
		t_bar->setPosition(ccp(43-240+214.f/8.f, n_rank->getContentSize().height/2.f-4));
		bottom_case->addChild(t_bar, -1);
	}
	
	
	CCSprite* n_shop = CCSprite::create("mainflow_shop.png");
	KSLabelTTF* n_shop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_shop), mySGD->getFont().c_str(), 12);
	n_shop_label->enableOuterStroke(ccBLACK, 1.f);
	n_shop_label->setPosition(ccp(n_shop->getContentSize().width/2.f, 7));
	n_shop->addChild(n_shop_label);
	CCSprite* s_shop = CCSprite::create("mainflow_shop.png");
	s_shop->setColor(ccGRAY);
	KSLabelTTF* s_shop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_shop), mySGD->getFont().c_str(), 12);
	s_shop_label->enableOuterStroke(ccBLACK, 1.f);
	s_shop_label->setPosition(ccp(s_shop->getContentSize().width/2.f, 7));
	s_shop->addChild(s_shop_label);
	
	CCMenuItem* shop_item = CCMenuItemSprite::create(n_shop, s_shop, this, menu_selector(MainFlowScene::menuAction));
	shop_item->setTag(kMainFlowMenuTag_shop);
	
	CCMenu* shop_menu = CCMenu::createWithItem(shop_item);
	shop_menu->setPosition(ccp(43-240+214.f/4.f, n_shop->getContentSize().height/2.f+8));//ccp(-73, n_shop->getContentSize().height/2.f));
	bottom_case->addChild(shop_menu);
	shop_menu->setTouchPriority(kCCMenuHandlerPriority-1);
	
	{
		CCSprite* t_bar = CCSprite::create("mainflow_bottom_case_bar.png");
		t_bar->setPosition(ccp(43-240+214.f/8.f*3.f, n_rank->getContentSize().height/2.f-4));
		bottom_case->addChild(t_bar, -1);
	}
	
//	CCSprite* n_friendmanagement = CCSprite::create("mainflow_friendmanagement.png");
//	CCSprite* s_friendmanagement = CCSprite::create("mainflow_friendmanagement.png");
//	s_friendmanagement->setColor(ccGRAY);
//	
//	CCMenuItem* friendmanagement_item = CCMenuItemSprite::create(n_friendmanagement, s_friendmanagement, this, menu_selector(MainFlowScene::menuAction));
//	friendmanagement_item->setTag(kMainFlowMenuTag_friendManagement);
//	
//	CCMenu* friendmanagement_menu = CCMenu::createWithItem(friendmanagement_item);
//	friendmanagement_menu->setPosition(ccp(-139, n_friendmanagement->getContentSize().height/2.f));
//	bottom_case->addChild(friendmanagement_menu);
	
//	CCSprite* n_achievement = CCSprite::create("mainflow_achievement.png");
//	CCSprite* s_achievement = CCSprite::create("mainflow_achievement.png");
//	s_achievement->setColor(ccGRAY);
//	
//	CCMenuItem* achievement_item = CCMenuItemSprite::create(n_achievement, s_achievement, this, menu_selector(MainFlowScene::menuAction));
//	achievement_item->setTag(kMainFlowMenuTag_achievement);
//	
//	CCMenu* achievement_menu = CCMenu::createWithItem(achievement_item);
//	achievement_menu->setPosition(ccp(-130, n_achievement->getContentSize().height/2.f));//ccp(-139, n_achievement->getContentSize().height/2.f));//ccp(125, n_achievement->getContentSize().height/2.f));
//	bottom_case->addChild(achievement_menu);
	
	CCSprite* n_cardsetting = CCSprite::create("mainflow_cardsetting.png");
	KSLabelTTF* n_cardsetting_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mycard), mySGD->getFont().c_str(), 12);
	n_cardsetting_label->enableOuterStroke(ccBLACK, 1.f);
	n_cardsetting_label->setPosition(ccp(n_cardsetting->getContentSize().width/2.f, 7));
	n_cardsetting->addChild(n_cardsetting_label);
	CCSprite* s_cardsetting = CCSprite::create("mainflow_cardsetting.png");
	s_cardsetting->setColor(ccGRAY);
	KSLabelTTF* s_cardsetting_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mycard), mySGD->getFont().c_str(), 12);
	s_cardsetting_label->enableOuterStroke(ccBLACK, 1.f);
	s_cardsetting_label->setPosition(ccp(s_cardsetting->getContentSize().width/2.f, 7));
	s_cardsetting->addChild(s_cardsetting_label);
	
	CCMenuItem* cardsetting_item = CCMenuItemSprite::create(n_cardsetting, s_cardsetting, this, menu_selector(MainFlowScene::menuAction));
	cardsetting_item->setTag(kMainFlowMenuTag_cardSetting);
	
	CCMenu* cardsetting_menu = CCMenu::createWithItem(cardsetting_item);
	cardsetting_menu->setPosition(ccp(43-240+214.f/4.f*2.f, n_cardsetting->getContentSize().height/2.f+8));//ccp(-7, n_cardsetting->getContentSize().height/2.f));
	bottom_case->addChild(cardsetting_menu);
	cardsetting_menu->setTouchPriority(kCCMenuHandlerPriority-1);
	
	{
		CCSprite* t_bar = CCSprite::create("mainflow_bottom_case_bar.png");
		t_bar->setPosition(ccp(43-240+214.f/8.f*5.f, n_rank->getContentSize().height/2.f-4));
		bottom_case->addChild(t_bar, -1);
	}
	
	
	CCSprite* n_mission = CCSprite::create("mainflow_mission.png");
	KSLabelTTF* n_mission_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_todaymission), mySGD->getFont().c_str(), 10);
	n_mission_label->enableOuterStroke(ccBLACK, 1.f);
	n_mission_label->setPosition(ccp(n_mission->getContentSize().width/2.f, 7));
	n_mission->addChild(n_mission_label);
	CCSprite* s_mission = CCSprite::create("mainflow_mission.png");
	s_mission->setColor(ccGRAY);
	KSLabelTTF* s_mission_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_todaymission), mySGD->getFont().c_str(), 10);
	s_mission_label->enableOuterStroke(ccBLACK, 1.f);
	s_mission_label->setPosition(ccp(s_mission->getContentSize().width/2.f, 7));
	s_mission->addChild(s_mission_label);
	
	CCMenuItem* mission_item = CCMenuItemSprite::create(n_mission, s_mission, this, menu_selector(MainFlowScene::menuAction));
	mission_item->setTag(kMainFlowMenuTag_mission);
	
	CCMenu* mission_menu = CCMenu::createWithItem(mission_item);
	mission_menu->setPosition(ccp(43-240+214.f/4.f*3.f, n_mission->getContentSize().height/2.f+8));
	bottom_case->addChild(mission_menu);
	mission_menu->setTouchPriority(kCCMenuHandlerPriority-1);

	
//	CCSprite* n_gacha = CCSprite::create("mainflow_gacha.png");
//	CCSprite* s_gacha = CCSprite::create("mainflow_gacha.png");
//	s_gacha->setColor(ccGRAY);
//	
//	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(MainFlowScene::menuAction));
//	gacha_item->setTag(kMainFlowMenuTag_gacha);
//	
//	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
//	gacha_menu->setPosition(ccp(59, n_gacha->getContentSize().height/2.f));
//	bottom_case->addChild(gacha_menu);
	
	
//	CCSprite* n_event = CCSprite::create("mainflow_event.png");
//	CCSprite* s_event = CCSprite::create("mainflow_event.png");
//	s_event->setColor(ccGRAY);
//	
//	CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(MainFlowScene::menuAction));
//	event_item->setTag(kMainFlowMenuTag_event);
//	
//	CCMenu* event_menu = CCMenu::createWithItem(event_item);
//	event_menu->setPosition(ccp(201, n_event->getContentSize().height/2.f-3));
//	bottom_case->addChild(event_menu);
	
	CCSprite* left_back = CCSprite::create("mainflow_bottom_case_left.png");
	left_back->setPosition(ccp(39-240,left_back->getContentSize().height/2.f));
	bottom_case->addChild(left_back, -1);
	
	CCSprite* right_back = CCSprite::create("mainflow_bottom_case_right.png");
	right_back->setPosition(ccp(207.5f-240,right_back->getContentSize().height/2.f)); // 207.5 // 262
	bottom_case->addChild(right_back, -1);
	
	CCSprite* center_back = CCSprite::create("mainflow_bottom_case_center.png");
	center_back->setAnchorPoint(ccp(0,0.5));
	center_back->setPosition(ccp(39-240+left_back->getContentSize().width/2.f,center_back->getContentSize().height/2.f));
	center_back->setScaleX(0.662f);
	bottom_case->addChild(center_back, -1);
	
	Json::Value v = mySGD->cgp_data;
	std::string pState = v["promotionstate"].asString();
	
	// 아무것도 하지마!!
	if(pState == "CGP_NONE")
	{
		/* 출력값
		 {
		 "callback" : null,
		 "param" : null,
		 "promotionstate" : "CGP_NONE"
		 }
		 
		 */
	}
	// 홍보해야 될 것이 존재
	else if(pState == "CGP_PROMOTION_EXISTS")
	{
		/* 출력값
		 {
		 "bubbletext" : "",
		 "buttonurl" : "http://images.hangame.co.kr/mobile/cgp/10150_wara/wara_cgp.png",
		 "callback" : null,
		 "eventurl" : "",
		 "param" : null,
		 "promotionstate" : "CGP_PROMOTION_EXISTS",
		 "typecode" : 1
		 }
		 
		 */
		
		CCSprite* t_bar = CCSprite::create("mainflow_bottom_case_bar.png");
		t_bar->setPosition(ccp(43-240+214.f/8.f*7.f, n_rank->getContentSize().height/2.f-4));
		bottom_case->addChild(t_bar, -1);
		
		center_back->setScaleX(1.f);
		right_back->setPosition(ccp(262-240,right_back->getContentSize().height/2.f));
		
		CCSprite* n_cgp = CCSprite::create("mainflow_event.png");
		KSLabelTTF* n_cgp_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_event), mySGD->getFont().c_str(), 12);
		n_cgp_label->enableOuterStroke(ccBLACK, 1.f);
		n_cgp_label->setPosition(ccp(n_cgp->getContentSize().width/2.f, 7));
		n_cgp->addChild(n_cgp_label);
		CCSprite* s_cgp = CCSprite::create("mainflow_event.png");
		s_cgp->setColor(ccGRAY);
		KSLabelTTF* s_cgp_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_event), mySGD->getFont().c_str(), 12);
		s_cgp_label->enableOuterStroke(ccBLACK, 1.f);
		s_cgp_label->setPosition(ccp(s_cgp->getContentSize().width/2.f, 7));
		s_cgp->addChild(s_cgp_label);
		
		CCMenuLambda* cgp_menu = CCMenuLambda::create();
		cgp_menu->setPosition(ccp(43-240+214.f, n_cgp->getContentSize().height/2.f+8));
		bottom_case->addChild(cgp_menu);
		cgp_menu->setTouchPriority(kCCMenuHandlerPriority-1);
		
		CCMenuItemLambda* cgp_item = CCMenuItemSpriteLambda::create(n_cgp, s_cgp, [=](CCObject* sender){
			if(!is_menu_enable)
				return;
			
			is_menu_enable = false;
			
			hspConnector::get()->launchPromotion();
			
			t_bar->removeFromParent();
			center_back->setScaleX(0.662f);
			right_back->setPosition(ccp(207.5f-240,right_back->getContentSize().height/2.f));
			
			cgp_menu->removeFromParent();
		});
		
		cgp_menu->addChild(cgp_item);
	}
	// 일반 보상
	else if(pState == "CGP_REWARD_REQUIRED")
	{
		/* 출력값
		 {
		 "callback" : null,
		 "param" : null,
		 "promotionstate" : "CGP_REWARD_REQUIRED",
		 "rewards" :
		 [
		 {
		 "promotiontype" : 2,
		 "rewardcode" : "10289_test",
		 "rewardvalue" : 1
		 }
		 ]
		 }
		 */
		
		ASPopupView* t_popup = ASPopupView::create(-999);
		
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
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		addChild(t_popup, kMainFlowZorder_popup);
		
		t_popup->setBasePosition(ccp(240, 160));
		
		CCScale9Sprite* back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
		back_case->setContentSize(CCSizeMake(350,280));
		back_case->setPosition(ccp(0,0));
		t_container->addChild(back_case);
		
		KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_cgpNormalTitle), mySGD->getFont().c_str(), 16);;
		title_label->setPosition(ccp(0,100));
		t_container->addChild(title_label);
		
		KSLabelTTF* ment_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_cgpNormalContent), mySGD->getFont().c_str(), 12);
		ment_label->setPosition(ccp(0,0));
		t_container->addChild(ment_label);
		
		CCLabelTTF* t_label = CCLabelTTF::create();
		KSLabelTTF* take_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont().c_str(), 13);
		take_label->setPosition(ccp(0,0));
		t_label->addChild(take_label);
		
		CCScale9Sprite* take_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
		
		CCControlButton* take_button = CCControlButton::create(t_label, take_back);
		take_button->addTargetWithActionForControlEvents(this, cccontrol_selector(MainFlowScene::cgpReward), CCControlEventTouchUpInside);
		take_button->setPreferredSize(CCSizeMake(150,65));
		take_button->setPosition(ccp(0,-85));
		t_container->addChild(take_button);
		
		take_button->setTouchPriority(t_popup->getTouchPriority()-5);
		
		t_container->setScaleY(0.f);
		
		t_popup->addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
			t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.8f);
				t_popup->addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.f);}));}));}));
		
		t_popup->addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(t_container, t);}, [=](int t){KS::setOpacity(t_container, 255);}));
		
//		Json::Value rewards = v["rewards"];
//		for(int i=0; i<rewards.size(); i++)
//		{
//			int rewardValue = rewards[i]["rewardvalue"].asInt();
//			CCLOG("reward !!! : %d", rewardValue);
//		}
		
		//			hspConnector::get()->checkCGP(param, Json::Value(), this, pf);
	}
	// 전체 팝업보상
	else if(pState == "CGP_PROMOTION_REWARD_EXISTS")
	{
		ASPopupView* t_popup = ASPopupView::create(-999);
		
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
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		addChild(t_popup, kMainFlowZorder_popup);
		
		t_popup->setBasePosition(ccp(240, 160));
		
		CCScale9Sprite* back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
		back_case->setContentSize(CCSizeMake(350,280));
		back_case->setPosition(ccp(0,0));
		t_container->addChild(back_case);
		
		KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_cgpAllPopupTitle), mySGD->getFont().c_str(), 16);;
		title_label->setPosition(ccp(0,100));
		t_container->addChild(title_label);
		
		KSLabelTTF* ment_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_cgpNormalContent), mySGD->getFont().c_str(), 12);
		ment_label->setPosition(ccp(0,0));
		t_container->addChild(ment_label);
		
		CCLabelTTF* t_label = CCLabelTTF::create();
		KSLabelTTF* take_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont().c_str(), 13);
		take_label->setPosition(ccp(0,0));
		t_label->addChild(take_label);
		
		CCScale9Sprite* take_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
		
		CCControlButton* take_button = CCControlButton::create(t_label, take_back);
		take_button->addTargetWithActionForControlEvents(this, cccontrol_selector(MainFlowScene::cgpAllReward), CCControlEventTouchUpInside);
		take_button->setPreferredSize(CCSizeMake(150,65));
		take_button->setPosition(ccp(0,-85));
		t_container->addChild(take_button);
		
		take_button->setTouchPriority(t_popup->getTouchPriority()-5);
		
		t_container->setScaleY(0.f);
		
		t_popup->addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
			t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.8f);
				t_popup->addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.f);}));}));}));
		
		t_popup->addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(t_container, t);}, [=](int t){KS::setOpacity(t_container, 255);}));
		
		
		
		
//		Json::Value rewards = v["rewards"];
//		for(int i=0; i<rewards.size(); i++)
//		{
//			int rewardValue = rewards[i]["rewardvalue"].asInt();
//			CCLOG("reward !!! : %d", rewardValue);
//		}
		
		//			hspConnector::get()->checkCGP(param, Json::Value(), this, pf);
	}
	
	CCSprite* etc_event = CCSprite::create("mainflow_etc_event.png");
	etc_event->setAnchorPoint(ccp(0.5,0));
	etc_event->setPosition(ccp(385,-(myDSH->puzzle_ui_top-320.f)/2.f+11));
	addChild(etc_event, kMainFlowZorder_uiButton);
	
	bottom_list.push_back(etc_event);
}

void MainFlowScene::cgpReward(CCObject* sender, CCControlEvent t_event)
{
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	Json::Value v = mySGD->cgp_data;
	
	Json::Value rewards = v["rewards"];
	for(int i=0; i<rewards.size(); i++)
	{
		int rewardValue = rewards[i]["rewardvalue"].asInt();
		CCLOG("reward !!! : %d", rewardValue);
	}
	
	LoadingLayer* t_loading = LoadingLayer::create(-9999);
	addChild(t_loading, 9999);
	
	mySGD->addChangeGoods(kGoodsType_gold, 10000, "CGP일반보상");
	mySGD->changeGoods([=](Json::Value result_data){
		t_loading->removeFromParent();
		if(result_data["result"]["code"].asInt() == GDSUCCESS)
		{
			hspConnector::get()->completePromotion();
			
			AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
			
			mySGD->cgp_data.clear();
			mySGD->cgp_data["promotionstate"] = "CGP_NONE";
			
			((CCNode*)sender)->getParent()->getParent()->removeFromParent();
		}
		else
		{
			
		}
	});
}

void MainFlowScene::cgpAllReward(CCObject* sender, CCControlEvent t_event)
{
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	Json::Value v = mySGD->cgp_data;
	
	Json::Value rewards = v["rewards"];
	for(int i=0; i<rewards.size(); i++)
	{
		int rewardValue = rewards[i]["rewardvalue"].asInt();
		CCLOG("reward !!! : %d", rewardValue);
	}
	
	LoadingLayer* t_loading = LoadingLayer::create(-9999);
	addChild(t_loading, 9999);
	
	mySGD->addChangeGoods(kGoodsType_gold, 10000, "CGP전체팝업보상");
	mySGD->changeGoods([=](Json::Value result_data){
		t_loading->removeFromParent();
		if(result_data["result"]["code"].asInt() == GDSUCCESS)
		{
			hspConnector::get()->completeInstallPromotion();
			
			AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
			
			mySGD->cgp_data.clear();
			mySGD->cgp_data["promotionstate"] = "CGP_NONE";
			
			((CCNode*)sender)->getParent()->getParent()->removeFromParent();
		}
		else
		{
			
		}
	});
}

void MainFlowScene::heartRefresh()
{
	CCPoint heart_position = heart_time->getPosition();
	CCNode* heart_parent = heart_time->getParent();
	
	heart_time->removeFromParent();
	
	heart_time = HeartTime::create();
	heart_time->setPosition(heart_position);
	heart_parent->addChild(heart_time);
}

void MainFlowScene::topOpenning()
{
	for(int i=0;i<top_list.size();i++)
	{
		CCPoint original_position = top_list[i]->getPosition();
		top_list[i]->setPositionY(original_position.y+200);
		if(i == 0)
		{
			top_list[i]->addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
																{
																	top_list[i]->setPositionY(original_position.y+200.f*t);
																}, [=](float t)
																{
																	top_list[i]->setPositionY(original_position.y);
																}));
		}
		else
		{
			CCDelayTime* t_delay = CCDelayTime::create(i*0.1f);
			CCMoveTo* t_move = CCMoveTo::create(0.2f, original_position);
			CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
			
			top_list[i]->runAction(t_seq);
		}
	}
	
	CCDelayTime* t_delay = CCDelayTime::create(top_list.size()*0.1f + 0.1f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainFlowScene::topOnLight));
	CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
	runAction(t_seq);
}

void MainFlowScene::topOnLight()
{
	GoodsLight* gold_light = GoodsLight::create(CCSprite::create("price_gold_img_mask.png"));
	gold_light->setPosition(ccp(gold_img->getContentSize().width/2.f, gold_img->getContentSize().height/2.f));
	gold_img->addChild(gold_light);
	
	GoodsLight* ruby_light = GoodsLight::create(CCSprite::create("price_ruby_img_mask.png"));
	ruby_light->setPosition(ccp(ruby_img->getContentSize().width/2.f, ruby_img->getContentSize().height/2.f));
	ruby_img->addChild(ruby_light);
}

void MainFlowScene::topOuting()
{
	for(int i=0;i<top_list.size();i++)
	{
		if(i == 0)
		{
			CCPoint original_position = top_list[i]->getPosition();
			
			top_list[i]->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t)
																{
																	top_list[i]->setPositionY(original_position.y+200.f*t);
																}, [=](float t)
																{
																	top_list[i]->setPositionY(original_position.y+200.f);
																}));
		}
		else
		{
			CCDelayTime* t_delay = CCDelayTime::create(i*0.1f);
			CCMoveTo* t_move = CCMoveTo::create(0.2f, top_list[i]->getPosition() + ccp(0,200));
			CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
			
			top_list[i]->runAction(t_seq);
		}
	}
}

void MainFlowScene::topReturnMode()
{
	CCPoint original_position = top_list[0]->getPosition();
	top_list[0]->setPosition(original_position + ccp(-150,0));
	top_list[0]->addChild(KSGradualValue<float>::create(1.f, 0.f, 0.3f, [=](float t)
														{
															top_list[0]->setPositionX(original_position.x-150.f*t);
														}, [=](float t)
														{
															top_list[0]->setPositionX(original_position.x);
														}));
}

void MainFlowScene::topPuzzleMode()
{
	CCPoint original_position = top_list[0]->getPosition();
	
	top_list[0]->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
														{
															top_list[0]->setPositionX(original_position.x-150.f*t);
														}, [=](float t)
														{
															top_list[0]->setPositionX(original_position.x-150.f);
														}));
}

void MainFlowScene::setTop()
{
	top_list.clear();
	
	CCNode* t_star_node = CCNode::create();
	t_star_node->setPosition(ccp(25,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-22));
	addChild(t_star_node, kMainFlowZorder_top);
	
	total_star = KS::loadCCBI<CCSprite*>(this, "main_star.ccbi").first;
	total_star->setPosition(ccp(0,0));
	t_star_node->addChild(total_star);
	
	KSLabelTTF* star_count = KSLabelTTF::create(CCString::createWithFormat("%d", mySGD->getClearStarCount())->getCString(), mySGD->getFont().c_str(), 12);
	star_count->enableOuterStroke(ccBLACK, 0.8f);
	star_count->setPosition(ccp(0,0));
	t_star_node->addChild(star_count);
	
	top_list.push_back(t_star_node);
	
	CCSprite* top_heart = CCSprite::create("mainflow_top_heart.png");
	top_heart->setAnchorPoint(ccp(0.5f,1.f));
	top_heart->setPosition(ccp(78+41,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-10));
	addChild(top_heart, kMainFlowZorder_top);
	
	top_list.push_back(top_heart);
	
//	CCSprite* top_case = CCSprite::create("mainflow_top.png");
//	top_case->setAnchorPoint(ccp(0.f,1.f));
//	top_case->setPosition(ccp(0,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
//	addChild(top_case, kMainFlowZorder_top);
//	
//	CCSprite* top_case2 = CCSprite::create("mainflow_top2.png");
//	top_case2->setAnchorPoint(ccp(0.f,1.f));
//	top_case2->setPosition(ccp(top_case->getContentSize().width,top_case->getContentSize().height));
//	top_case->addChild(top_case2);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(15,top_heart->getContentSize().height/2.f));
	top_heart->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(MainFlowScene::menuAction));
	heart_item->setTag(kMainFlowMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(top_heart->getContentSize().width-n_heart->getContentSize().width/2.f+5,top_heart->getContentSize().height/2.f));
	top_heart->addChild(heart_menu);
	
	
	CCSprite* top_gold = CCSprite::create("mainflow_top_gold.png");
	top_gold->setAnchorPoint(ccp(0.5f,1.f));
	top_gold->setPosition(ccp(216+23,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-10));
	addChild(top_gold, kMainFlowZorder_top);
	
	top_list.push_back(top_gold);
	
	gold_img = CCSprite::create("price_gold_img.png");
	gold_img->setPosition(ccp(gold_img->getContentSize().width/2.f-1, top_gold->getContentSize().height/2.f+1));
	top_gold->addChild(gold_img);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_gold))->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(top_gold->getContentSize().width/2.f + 1,top_gold->getContentSize().height/2.f-6));
	top_gold->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(MainFlowScene::menuAction));
	gold_item->setTag(kMainFlowMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(top_gold->getContentSize().width-n_gold->getContentSize().width/2.f+5,top_gold->getContentSize().height/2.f));
	top_gold->addChild(gold_menu);

	
	CCSprite* top_ruby = CCSprite::create("mainflow_top_ruby.png");
	top_ruby->setAnchorPoint(ccp(0.5f,1.f));
	top_ruby->setPosition(ccp(325+5,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-10));
	addChild(top_ruby, kMainFlowZorder_top);
	
	top_list.push_back(top_ruby);
	
	ruby_img = CCSprite::create("price_ruby_img.png");
	ruby_img->setPosition(ccp(ruby_img->getContentSize().width/2.f-1, top_gold->getContentSize().height/2.f));
	top_ruby->addChild(ruby_img);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_ruby))->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(top_ruby->getContentSize().width/2.f + 1,top_ruby->getContentSize().height/2.f-6));
	top_ruby->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(MainFlowScene::menuAction));
	ruby_item->setTag(kMainFlowMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(top_ruby->getContentSize().width-n_ruby->getContentSize().width/2.f+5,top_ruby->getContentSize().height/2.f));
	top_ruby->addChild(ruby_menu);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	friend_point_label =  CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getFriendPoint())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
//	friend_point_label->setPosition(ccp(338,top_case->getContentSize().height/2.f-5));
//	top_case->addChild(friend_point_label);
//	
//	mySGD->setFriendPointLabel(friend_point_label);
	
//	CCSprite* n_friend_point = CCSprite::create("mainflow_top_shop.png");
//	CCSprite* s_friend_point = CCSprite::create("mainflow_top_shop.png");
//	s_friend_point->setColor(ccGRAY);
//	
//	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(MainFlowScene::menuAction));
//	friend_point_item->setTag(kMainFlowMenuTag_friendPointContent);
//	
//	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
//	friend_point_menu->setPosition(ccp(362,top_case->getContentSize().height/2.f));
//	top_case->addChild(friend_point_menu);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	CCSprite* n_postbox = CCSprite::create("mainflow_new_postbox.png");
	CCSprite* s_postbox = CCSprite::create("mainflow_new_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(MainFlowScene::menuAction));
	postbox_item->setTag(kMainFlowMenuTag_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccp(397,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-22));
	addChild(postbox_menu, kMainFlowZorder_top);
	
	top_list.push_back(postbox_menu);
	
	postbox_count_case = CCSprite::create("mainflow_new.png");//"mainflow_postbox_count.png");
	postbox_count_case->setPosition(ccp(409,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-16));
	addChild(postbox_count_case, kMainFlowZorder_top);
	
	CCScaleTo* t_scale1 = CCScaleTo::create(0.1f, 1.3f);
	CCScaleTo* t_scale2 = CCScaleTo::create(0.2f, 1.f);
	CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
	CCScaleTo* t_scale3 = CCScaleTo::create(0.1f, 1.3f);
	CCScaleTo* t_scale4 = CCScaleTo::create(0.2f, 1.f);
	CCDelayTime* t_delay2 = CCDelayTime::create(5.f);
	CCSequence* t_seq1 = CCSequence::create(t_scale1, t_scale2, t_delay1, t_scale3, t_scale4, t_delay2, NULL);
	CCRepeatForever* t_repeat1 = CCRepeatForever::create(t_seq1);
	postbox_count_case->runAction(t_repeat1);
	
	postbox_count_case->setVisible(false);
	
	postbox_count_case->addChild(KSSchedule::create([=](float dt)
					   {
						   if(postbox_menu->getPositionY() >= (myDSH->puzzle_ui_top-320.f)/2.f + 320.f-20)
							   postbox_count_case->setOpacity(0);
						   else
							   postbox_count_case->setOpacity(255);
						   return true;
					   }));
	
//	postbox_count_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 10);
//	postbox_count_label->setColor(ccc3(95, 60, 30));
//	postbox_count_label->setPosition(ccp(postbox_count_case->getContentSize().width/2.f-0.5f, postbox_count_case->getContentSize().height/2.f+0.5f));
//	postbox_count_case->addChild(postbox_count_label);
	
	countingMessage();

	
	CCSprite* n_achieve = CCSprite::create("mainflow_new_achievement.png");
	CCSprite* s_achieve = CCSprite::create("mainflow_new_achievement.png");
	s_achieve->setColor(ccGRAY);
	
	CCMenuItem* achieve_item = CCMenuItemSprite::create(n_achieve, s_achieve, this, menu_selector(MainFlowScene::menuAction));
	achieve_item->setTag(kMainFlowMenuTag_achievement);
	
	CCMenu* achieve_menu = CCMenu::createWithItem(achieve_item);
	achieve_menu->setPosition(ccp(429,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-22));
	addChild(achieve_menu, kMainFlowZorder_top);
	
	top_list.push_back(achieve_menu);

	achievement_count_case = CCSprite::create("mainflow_new.png");
	achievement_count_case->setPosition(ccp(441,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-16));
	addChild(achievement_count_case, kMainFlowZorder_top);
	
	CCScaleTo* t_scale5 = CCScaleTo::create(0.1f, 1.3f);
	CCScaleTo* t_scale6 = CCScaleTo::create(0.2f, 1.f);
	CCDelayTime* t_delay3 = CCDelayTime::create(0.5f);
	CCScaleTo* t_scale7 = CCScaleTo::create(0.1f, 1.3f);
	CCScaleTo* t_scale8 = CCScaleTo::create(0.2f, 1.f);
	CCDelayTime* t_delay4 = CCDelayTime::create(5.f);
	CCSequence* t_seq2 = CCSequence::create(t_scale5, t_scale6, t_delay3, t_scale7, t_scale8, t_delay4, NULL);
	CCRepeatForever* t_repeat2 = CCRepeatForever::create(t_seq2);
	achievement_count_case->runAction(t_repeat2);
	
	achievement_count_case->setVisible(false);
	
	achievement_count_case->addChild(KSSchedule::create([=](float dt)
													{
														if(achieve_menu->getPositionY() >= (myDSH->puzzle_ui_top-320.f)/2.f + 320.f-20)
															achievement_count_case->setOpacity(0);
														else
															achievement_count_case->setOpacity(255);
														return true;
													}));
	
	countingAchievement();
	
	CCSprite* n_option = CCSprite::create("mainflow_new_option.png");
	CCSprite* s_option = CCSprite::create("mainflow_new_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(MainFlowScene::menuAction));
	option_item->setTag(kMainFlowMenuTag_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccp(461,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-22));
	addChild(option_menu, kMainFlowZorder_top);
	
	top_list.push_back(option_menu);

//	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
//	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
//	s_tip->setColor(ccGRAY);
//	
//	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(MainFlowScene::menuAction));
//	tip_item->setTag(kMainFlowMenuTag_tip);
//	
//	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
//	tip_menu->setPosition(ccp(465,top_case->getContentSize().height/2.f));
//	top_case->addChild(tip_menu);
}

void MainFlowScene::countingMessage()
{
	postbox_count_case->setVisible(false);
	Json::Value p;
	p["memberID"]=hspConnector::get()->getSocialID();
	p["type"]=0; // 모든 타입의 메시지를 받겠다는 뜻.
	p["limitDay"] = mySGD->getMsgRemoveDay();
	// 0 이 아니면 해당하는 타입의 메시지가 들어옴.
	
	hspConnector::get()->command("getmessagelist",p,[this](Json::Value r)
								 {
									 GraphDogLib::JsonToLog("getmessagelist", r);
									 if(r["result"]["code"].asInt() != GDSUCCESS)
										 return;
									 Json::Value message_list = r["list"];
									 if(message_list.size() > 0)
									 {
										 postbox_count_case->setVisible(true);
										 
//										 if(message_list.size() < 10)
//										 {
//											 postbox_count_label->setFontSize(10);
//											 postbox_count_label->setString(CCString::createWithFormat("%d", message_list.size())->getCString());
//										 }
//										 else if(message_list.size() < 100)
//										 {
//											 postbox_count_label->setFontSize(7);
//											 postbox_count_label->setString(CCString::createWithFormat("%d", message_list.size())->getCString());
//										 }
//										 else
//										 {
//											 postbox_count_label->setFontSize(8);
//											 postbox_count_label->setString("...");
//										 }
									 }
									 else
									 {
										 postbox_count_case->setVisible(false);
//										 postbox_count_label->setString("0");
									 }
								 });
}

void MainFlowScene::countingAchievement()
{
	achievement_count_case->setVisible(false);
	int reward_count = 0;
	
	for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) != -1 &&
		   AchieveConditionReward::sharedInstance()->isAchieve((AchievementCode)i))
			reward_count++; // achieve_list.push_back((AchievementCode)i);
	}
	
	for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) != -1 &&
		   AchieveConditionReward::sharedInstance()->isAchieve((AchievementCode)i))
			reward_count++; // achieve_list.push_back((AchievementCode)i);
	}
	
	if(reward_count > 0)
	{
		achievement_count_case->setVisible(true);
	}
}

void MainFlowScene::popupClose()
{
	is_menu_enable = true;
}

void MainFlowScene::achievePopupClose()
{
	countingAchievement();
	is_menu_enable = true;
}

void MainFlowScene::mailPopupClose()
{
	countingMessage();
	is_menu_enable = true;
}

void MainFlowScene::tutorialCardSettingClose()
{
	is_menu_enable = true;
	puzzle_table->setTouchEnabled(true);
	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
}

void MainFlowScene::closeFriendPoint()
{
	close_friend_point_action();
}

void MainFlowScene::callTimeInfo()
{
	mySGD->keep_time_info.is_loaded = false;
	hspConnector::get()->command("gettimeinfo", Json::Value(), [=](Json::Value result_data)
								 {
									if(result_data["result"]["code"].asInt() == GDSUCCESS)
									{
										mySGD->keep_time_info.timestamp = result_data["timestamp"].asUInt();
										mySGD->keep_time_info.weekNo = result_data["weekNo"].asUInt();
										mySGD->keep_time_info.weekday = result_data["weekday"].asInt();
										mySGD->keep_time_info.date = result_data["date"].asInt64();
										mySGD->keep_time_info.hour = result_data["hour"].asInt();
										mySGD->keep_time_info.is_loaded = true;
										
										if(!is_table_openning)
											tableRefresh();
									}
									else
									{
										addChild(KSTimer::create(1.f, [=](){callTimeInfo();}));
									}
								 });
}

void MainFlowScene::tableRefresh()
{
	if(!is_menu_enable)
	{
		addChild(KSTimer::create(0.1f, [=](){tableRefresh();}));
		return;
	}
	
	CCPoint origin_offset = puzzle_table->getContentOffset();
	puzzle_table->reloadData();
	puzzle_table->setContentOffset(origin_offset);
}

MainFlowScene::~MainFlowScene()
{
	hspConnector::get()->removeTarget(this);
}

void MainFlowScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void MainFlowScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(MainFlowScene::alertAction));
}
