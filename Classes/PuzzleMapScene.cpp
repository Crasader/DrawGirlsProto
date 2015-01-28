//
//  PuzzleMapScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 19..
//
//

#include "PuzzleMapScene.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "StageInfoDown.h"
#include "EventPopup.h"
//#include "CardSettingScene.h"
#include "CardSettingPopup.h"
//#include "GachaPopup.h"
#include "RankPopup.h"
#include "MailPopup.h"
#include "TutorialScene.h"
#include "HeartTime.h"
#include "StageSettingPopup.h"
#include "ClearPopup.h"
#include "FailPopup.h"
#include "OptionPopup.h"
#include "ShopPopup.h"
#include <random>
#include "ASPopupView.h"
#include "TicketRequestContent.h"
#include "GachaPurchase.h"
#include "CollectionBookPopup.h"
#include "NoticeContent.h"

CCScene* PuzzleMapScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    PuzzleMapScene *layer = PuzzleMapScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PuzzleMapScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
	srand(time(NULL));
	////////////////////////////////////////////////////
	
	friend_point_popup = NULL;
	
	is_creating_puzzle = false;
	
	recent_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	if(recent_puzzle_number == 0)
	{
		recent_puzzle_number = 1;
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, recent_puzzle_number);
	}
	
	StageListDown* t_sld = StageListDown::create(this, callfunc_selector(PuzzleMapScene::loadSuccess), recent_puzzle_number);
	addChild(t_sld, kPMS_Z_popup);
	
    return true;
}

void PuzzleMapScene::loadSuccess()
{
	CCDelayTime* t_delay = CCDelayTime::create(0.2f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::startSceneSetting));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	runAction(t_seq);
}

void PuzzleMapScene::resultLogin(Json::Value result_data)
{
	recent_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	if(recent_puzzle_number == 0)
	{
		recent_puzzle_number = 1;
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, recent_puzzle_number);
	}
	
	CCLOG("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	
	//	hspConnector::get()->kLoadFriends(std::bind(&WorldMapScene::resultFriendList, this, std::placeholders::_1));
	
	StageListDown* t_sld = StageListDown::create(this, callfunc_selector(PuzzleMapScene::startSceneSetting), recent_puzzle_number);
	addChild(t_sld, kPMS_Z_popup);
}

void PuzzleMapScene::startSceneSetting()
{
	is_gesturable_map_mode = false;
	map_mode_state = kMMS_default;
	ing_caching_cnt = 0;
	
	my_puzzle_mode = (PuzzleMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode);
	
	CCSprite* ui_wall = CCSprite::create("test_ui_back_wall.png");
	ui_wall->setPosition(ccp(240,160));
	addChild(ui_wall, kPMS_Z_wall);
	
	main_node = CCNode::create();
	addChild(main_node, kPMS_Z_main);
	
	after_map_node = NULL;
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
	{
//		myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
		
		int take_level;
		if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 3;
		else if(mySGD->is_exchanged || mySGD->is_showtime)	take_level = 2;
		else												take_level = 1;
		
		if(mySGD->isHasGottenCards(mySD->getSilType(), take_level) == 0)
		{
//			mySGD->setClearRewardGold(NSDS_GI(kSDS_CI_int1_reward_i, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)));
			
			mySGD->addHasGottenCardNumber(NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
		}
		else
		{
			
		}
	}
	
	setMapNode();
	setUIs();
	
	is_menu_enable = false;
	setTouchEnabled(true);
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_init)
	{
		startChangeUiMode();
		if(mySGD->getMustBeShowNotice())
		{
			ASPopupView* t_popup = ASPopupView::create(-200);
			
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
			t_popup->setBasePosition(ccp(240, 160));
			
			NoticeContent* t_container = NoticeContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																				 {
																					 t_popup->removeFromParent();
																				 }, mySGD->getNoticeList());
			t_popup->setContainerNode(t_container);
			addChild(t_popup, kPMS_Z_popup);
		}
	}
	else if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_stage)
	{
		is_from_call_map_mode = true;
		((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setTouchEnabled(false);
		
		((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setEnabled(false);
		
		((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
		
		for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
		{
			StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
			if(t_sp)
				t_sp->mySetTouchEnable(true);
		}
		
		is_gesturable_map_mode = true;
		map_mode_state = kMMS_default;
		is_menu_enable = true;
	}
	else if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
	{
		is_from_call_map_mode = true;
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setTouchEnabled(false);
		
		((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setEnabled(false);
		
		((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
		
		for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
		{
			StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
			if(t_sp)
				t_sp->mySetTouchEnable(true);
		}
		
		is_gesturable_map_mode = true;
		map_mode_state = kMMS_default;
		is_menu_enable = true;
		
		showClearPopup();
	}
	else if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_fail)
	{
		is_from_call_map_mode = true;
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setTouchEnabled(false);
		
		((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setEnabled(false);
		
		((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
		
		for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
		{
			StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
			if(t_sp)
				t_sp->mySetTouchEnable(true);
		}
		
		is_gesturable_map_mode = true;
		map_mode_state = kMMS_default;
		is_menu_enable = true;
		
		showFailPopup();
	}
	else if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_stageSetting)
	{
		is_from_call_map_mode = true;
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setTouchEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setTouchEnabled(false);
		
		((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setEnabled(false);
		((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setEnabled(false);
		
		((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
		
		for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
		{
			StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
			if(t_sp)
				t_sp->mySetTouchEnable(true);
		}
		
		is_gesturable_map_mode = true;
		map_mode_state = kMMS_default;
		is_menu_enable = true;
		
		showStageSettingPopup();
	}
}

void PuzzleMapScene::resetPuzzle()
{
	setPuzzle(recent_puzzle_number);
}

void PuzzleMapScene::setPuzzle(int t_puzzle_number)
{
	if(getChildByTag(kPMS_MT_loadPuzzleInfo))
		removeChildByTag(kPMS_MT_loadPuzzleInfo);
	removeChildByTag(kPMS_MT_puzzleOpenTitle);
	removeChildByTag(kPMS_MT_ticketCnt);
	removeChildByTag(kPMS_MT_callTicket);
	removeChildByTag(kPMS_MT_buyPuzzle);
	
	recent_puzzle_number = t_puzzle_number;
	
	map_mode_state = kMMS_changeMode;
	is_menu_enable = false;
	
	after_map_node = createMapNode();
	after_map_node->setPosition(ccp(240,180));
	main_node->addChild(after_map_node, kPMS_Z_puzzle_back_side);
	
	if(after_map_node->getTag() == kPMS_MT_loaded)
	{
		after_map_node->setTag(-1);
		
		cachingPuzzleImg2();
		
		map_node->removeFromParent();
		map_node = after_map_node;
		after_map_node = NULL;
		
		if(map_mode_state == kMMS_changeMode)
			map_mode_state = kMMS_notLoadMode;
		else
			map_mode_state = kMMS_uiMode;
		is_menu_enable = true;
		
//		endLoadedMovingMapNode();
	}
	else if(after_map_node->getTag() == kPMS_MT_notOpenedPuzzle)
	{
		after_map_node->setTag(-1);
		
		endMovingMapNodeNotOpenPuzzle();
	}
	else if(after_map_node->getTag() == kPMS_MT_notClearBeforePuzzle)
	{
		after_map_node->setTag(-1);
		
		endMovingMapNodeNotClearPuzzle();
	}
	else
	{
		after_map_node->setTag(-1);
		
		endMovingMapNode();
	}
	
	is_gesturable_map_mode = false;
}

void PuzzleMapScene::setMapNode()
{
	maximum_scale = 2.f;
	minimum_scale = 1.f;
	
	map_node = CCNode::create();
	map_node->setPosition(ccp(240,160));
	main_node->addChild(map_node, kPMS_Z_puzzle_back_side);
	
	map_node->getCamera()->setEyeXYZ(0, 0, 2.f);
	
	is_puzzle_clear = true;
	
	PuzzleCache* puzzle_cache = PuzzleCache::getInstance();
	
	puzzle_cache->loadImage(recent_puzzle_number);
	
	CCSprite* map_back_center = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_center.png", recent_puzzle_number)->getCString());
	map_back_center->setPosition(CCPointZero);
	map_node->addChild(map_back_center, kPMS_Z_puzzle_back_side);
	
//	CCSize center_size = CCSizeMake(520.f, 340.f);
	CCPoint original_point = ccp(-260.f, -170.f);
	
	PuzzleImage* left_img = puzzle_cache->getImage(CCString::createWithFormat("puzzle%d_left", recent_puzzle_number)->getCString());
	CCSprite* map_back_left = CCSprite::createWithTexture(left_img->makeTexture());
	map_back_left->setPosition(ccpAdd(original_point, left_img->getCutPoint()));
	map_node->addChild(map_back_left, kPMS_Z_puzzle_back);
	
	CCSprite* shadow_left = CCSprite::create("puzzle_shadow_1_left.png");
	shadow_left->setPosition(map_back_left->getPosition());
	map_node->addChild(shadow_left, kPMS_Z_puzzle_shadow);
	

	PuzzleImage* right_img = puzzle_cache->getImage(CCString::createWithFormat("puzzle%d_right", recent_puzzle_number)->getCString());
	CCSprite* map_back_right = CCSprite::createWithTexture(right_img->makeTexture());
	map_back_right->setPosition(ccpAdd(original_point, right_img->getCutPoint()));
	map_node->addChild(map_back_right, kPMS_Z_puzzle_back);
	
	CCSprite* shadow_right = CCSprite::create("puzzle_shadow_1_right.png");
	shadow_right->setPosition(map_back_right->getPosition());
	map_node->addChild(shadow_right, kPMS_Z_puzzle_shadow);
	
	
	PuzzleImage* top_img = puzzle_cache->getImage(CCString::createWithFormat("puzzle%d_top", recent_puzzle_number)->getCString());
	CCSprite* map_back_top = CCSprite::createWithTexture(top_img->makeTexture());
	map_back_top->setPosition(ccpAdd(original_point, top_img->getCutPoint()));
	map_node->addChild(map_back_top, kPMS_Z_puzzle_back);
	
	CCSprite* shadow_top = CCSprite::create("puzzle_shadow_1_top.png");
	shadow_top->setPosition(map_back_top->getPosition());
	map_node->addChild(shadow_top, kPMS_Z_puzzle_shadow);
	
	
	PuzzleImage* bottom_img = puzzle_cache->getImage(CCString::createWithFormat("puzzle%d_bottom", recent_puzzle_number)->getCString());
	CCSprite* map_back_bottom = CCSprite::createWithTexture(bottom_img->makeTexture());
	map_back_bottom->setPosition(ccpAdd(original_point, bottom_img->getCutPoint()));
	map_node->addChild(map_back_bottom, kPMS_Z_puzzle_back);
	
	CCSprite* shadow_bottom = CCSprite::create("puzzle_shadow_1_bottom.png");
	shadow_bottom->setPosition(map_back_bottom->getPosition());
	map_node->addChild(shadow_bottom, kPMS_Z_puzzle_shadow);
	
	
//	CCSprite* map_back_left = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_left.png", recent_puzzle_number)->getCString());
//	map_back_left->setAnchorPoint(ccp(0.f,0.5f));
//	map_back_left->setPosition(ccp(-center_size.width/2.f, 0));
//	map_node->addChild(map_back_left, kPMS_Z_puzzle_back);
//	
//	CCSprite* map_back_right = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_right.png", recent_puzzle_number)->getCString());
//	map_back_right->setAnchorPoint(ccp(1.f,0.5f));
//	map_back_right->setPosition(ccp(center_size.width/2.f, 0));
//	map_node->addChild(map_back_right, kPMS_Z_puzzle_back);
//	
//	CCSprite* map_back_top = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_top.png", recent_puzzle_number)->getCString());
//	map_back_top->setAnchorPoint(ccp(0.5f,1.f));
//	map_back_top->setPosition(ccp(0, center_size.height/2.f));
//	map_node->addChild(map_back_top, kPMS_Z_puzzle_back);
//	
//	CCSprite* map_back_bottom = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_bottom.png", recent_puzzle_number)->getCString());
//	map_back_bottom->setAnchorPoint(ccp(0.5f,0.f));
//	map_back_bottom->setPosition(ccp(0,-center_size.height/2.f));
//	map_node->addChild(map_back_bottom, kPMS_Z_puzzle_back);
	
	shadow_batchnode = CCSpriteBatchNode::create("puzzle_shadow_1_piece.png");
	shadow_batchnode->setPosition(CCPointZero);
	map_node->addChild(shadow_batchnode, kPMS_Z_puzzle_shadow);
	
	CCRect stage_rect = CCRectMake(-30.f, -30.f, 60.f, 60.f);
	
	start_stage_number = NSDS_GI(recent_puzzle_number, kSDS_PZ_startStage_i);
	stage_count = NSDS_GI(recent_puzzle_number, kSDS_PZ_stageCount_i);
	
	for(int i=1;i<=24;i++)
	{
		int stage_count = NSDS_GI(recent_puzzle_number, kSDS_PZ_stageCount_i);
		int start_stage = NSDS_GI(recent_puzzle_number, kSDS_PZ_startStage_i);
		
		int stage_number = 0;
		
		for(int j=start_stage;j<start_stage+stage_count && stage_number == 0;j++)
		{
			if(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, j) == i)
				stage_number = j;
		}
		
		CCPoint sp_position = ccpAdd(original_point, PuzzleCache::getInstance()->getImage(CCString::createWithFormat("puzzle%d_piece%d", recent_puzzle_number, i)->getCString())->getCutPoint());
		string piece_type = "w";
		if((i-1)/6%2 == 0)
		{
			if(i%2 == 0)
				piece_type = "w";
			else
				piece_type = "h";
		}
		else
		{
			if(i%2 == 0)
				piece_type = "h";
			else
				piece_type = "w";
		}
		
		if(stage_number != 0)
		{
			int stage_level = SDS_GI(kSDF_puzzleInfo, recent_puzzle_number, CCSTR_CWF("stage%d_level", stage_number)->getCString());
			if(stage_number == 1 || mySGD->isClearPiece(stage_number) ||
			   (NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
				(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 || mySGD->isClearPiece(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))))
			{
				bool is_found = false;
				int found_number = 0;
				bool is_have_card[3] = {0,};
				for(int k=3;k>=1;k--)
				{
					int card_number = SDS_GI(kSDF_stageInfo, stage_number, CCSTR_CWF("level%d_card", k)->getCString());
					if(mySGD->isHasGottenCards(card_number) > 0)
					{
						if(!is_found)
						{
							is_found = true;
							found_number = k;
						}
						is_have_card[k-1] = true;
					}
				}
				
				if(found_number == 3)
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_piece%d", recent_puzzle_number, i)->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, false, false, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		map_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														map_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
					
					t_sp->setChangable(CCSTR_CWF("puzzle%d_thumbnail%d", recent_puzzle_number, i)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
					t_sp->setPuzzleMode(my_puzzle_mode);
					t_sp->shadow_node = addShadow(stage_number, piece_type.c_str(), t_sp->getPosition());
				}
				else if(found_number == 2)
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_piece%d", recent_puzzle_number, i)->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, false, true, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		map_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														map_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
					
					t_sp->setChangable(CCSTR_CWF("puzzle%d_thumbnail%d", recent_puzzle_number, i)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
					t_sp->setPuzzleMode(my_puzzle_mode);
					t_sp->shadow_node = addShadow(stage_number, piece_type.c_str(), t_sp->getPosition());
				}
				else if(found_number == 1)
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_piece%d", recent_puzzle_number, i)->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, true, true, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		map_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														map_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
					
					t_sp->setChangable(CCSTR_CWF("puzzle%d_thumbnail%d", recent_puzzle_number, i)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
					t_sp->setPuzzleMode(my_puzzle_mode);
					t_sp->shadow_node = addShadow(stage_number, piece_type.c_str(), t_sp->getPosition());
				}
				else
				{
					is_puzzle_clear = false;
					StagePiece* t_sp = StagePiece::create("test_puzzle_empty.png",
														  stage_number, stage_level, sp_position, stage_rect, false, false, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		map_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														map_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
				}
			}
			else
			{
				is_puzzle_clear = false;
				if(mySGD->isClearPiece(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("piece_buy_%s.png", piece_type.c_str())->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, false, false, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		map_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														map_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
				}
				else
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("piece_lock_%s.png", piece_type.c_str())->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, false, false, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		map_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														map_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
				}
			}
		}
		else
		{
			StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_piece%d", recent_puzzle_number, i)->getCString(),
												  stage_number, 0, sp_position, stage_rect, false, false, piece_type.c_str(),
												  this, menu_selector(PuzzleMapScene::stageAction));
			t_sp->mySetTouchEnable(false);
			
			map_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber());
			
			t_sp->setBackPuzzle();
			t_sp->shadow_node = addShadow(stage_number, piece_type.c_str(), t_sp->getPosition());
		}
	}
	
	if(is_puzzle_clear)
	{
		PuzzleHistory t_history = mySGD->getPuzzleHistory(recent_puzzle_number);
		t_history.is_clear = true;
		mySGD->setPuzzleHistory(t_history, nullptr);
	}
	
	touched_stage_number = 0;
}

CCSprite* PuzzleMapScene::addShadow(int i, string t_type, CCPoint sp_position)
{
	bool is_long = (t_type == "h");
	
	CCSprite* t_shadow = CCSprite::create("puzzle_shadow_1_piece.png", CCRectMake(is_long ? 0 : 105, 0, 105, 105));
	t_shadow->setPosition(sp_position);
	shadow_batchnode->addChild(t_shadow);
	
	return t_shadow;
}

void PuzzleMapScene::puzzleAction(CCObject *sender)
{
	if(!puzzle_list_view->isTouchEnabled())		return;
	
	puzzle_list_view->setTouchEnabled(false);
	puzzle_list_view->touchCancel();
	
	int tag = ((CCNode*)sender)->getTag();
	
	CCLOG("touched puzzle number : %d", tag);
	
	setPuzzle(tag);
	
	startReturnUiMode();
	
//	showEventButton();
//	
//	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(false);
//	
//	CCScaleTo* t_scale = CCScaleTo::create(0.45f, map_node->getScaleX(), map_node->getScaleX());
//	map_node->runAction(t_scale);
//	
//	CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(0,0));
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::stopReturnUiMode));
//	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
//	main_node->runAction(t_seq);
}

void PuzzleMapScene::setUIs()
{
	puzzle_list_view = PuzzleListView::create();
	puzzle_list_view->setPosition(ccp(0,340));
	main_node->addChild(puzzle_list_view, kPMS_Z_main);
	
	int puzzle_cnt = NSDS_GI(kSDS_GI_puzzleListCount_i);
	for(int i=1;i<=puzzle_cnt;i++)
	{
		CCPoint positioning_data = ccp(-140.f, 57.5f);
		if(i%2 == 0)
			positioning_data.y *= -1.f;
		positioning_data.x += 140.f*((i-1)/2);
		
		PLV_Node* t_node = PLV_Node::create(i, this, menu_selector(PuzzleMapScene::puzzleAction), ccpAdd(ccp(240, 160), positioning_data), puzzle_list_view->getViewRect());
		puzzle_list_view->addChild(t_node, 0, t_node->getPuzzleNumber());
	}
	
	puzzle_list_view->setMinPositionX();
	puzzle_list_view->setTouchEnabled(false);
	
//	CCSprite* ui_frame = CCSprite::create("test_ui_back_frame.png");
//	ui_frame->setPosition(ccp(240,160+340));
//	main_node->addChild(ui_frame, kPMS_Z_main);
	
	CCSprite* ui_back = CCSprite::create("test_ui_back_table.png");
	ui_back->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
	ui_back->setPosition(ccp(240,160));
	main_node->addChild(ui_back, kPMS_Z_main);
	
	
	CCSprite* top_case = CCSprite::create("test_ui_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(getUiButtonPosition(kPMS_MT_top));
	addChild(top_case, kPMS_Z_ui_button, kPMS_MT_top);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_ruby))->getCString(), "allfont.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(94,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_gold))->getCString(), "allfont.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(185,top_case->getContentSize().height/2.f));
	top_case->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	HeartTime* heart_time = HeartTime::create();
	heart_time->setPosition(ccp(250,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time, 0, kPMS_MT_heartTime);
	
	CCSprite* n_ruby = CCSprite::create("test_ui_shop.png");
	CCSprite* s_ruby = CCSprite::create("test_ui_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(PuzzleMapScene::menuAction));
	ruby_item->setTag(kPMS_MT_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(124,top_case->getContentSize().height/2.f-2));
	top_case->addChild(ruby_menu);
	
	CCSprite* n_gold = CCSprite::create("test_ui_shop.png");
	CCSprite* s_gold = CCSprite::create("test_ui_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(PuzzleMapScene::menuAction));
	gold_item->setTag(kPMS_MT_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(220,top_case->getContentSize().height/2.f-2));
	top_case->addChild(gold_menu);
	
	CCSprite* n_heart = CCSprite::create("test_ui_shop.png");
	CCSprite* s_heart = CCSprite::create("test_ui_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(PuzzleMapScene::menuAction));
	heart_item->setTag(kPMS_MT_lifeShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(369,top_case->getContentSize().height/2.f-2));
	top_case->addChild(heart_menu);
	
	CCSprite* n_friend_point = CCSprite::create("test_ui_shop.png");
	CCSprite* s_friend_point = CCSprite::create("test_ui_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(PuzzleMapScene::menuAction));
	friend_point_item->setTag(kPMS_MT_friendPoint);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(460,top_case->getContentSize().height/2.f-2));
	top_case->addChild(friend_point_menu);
	
	
	CCSprite* bottom_case = CCSprite::create("test_ui_bottom.png");
	bottom_case->setOpacity(0);
	bottom_case->setAnchorPoint(ccp(0.5f,0.f));
	bottom_case->setPosition(getUiButtonPosition(kPMS_MT_bottom));
	main_node->addChild(bottom_case, kPMS_Z_ui_button, kPMS_MT_bottom);
	
	CCSize bottom_size = bottom_case->getContentSize();
	
	CCSprite* n_postbox = CCSprite::create("test_ui_postbox.png");
	CCSprite* s_postbox = CCSprite::create("test_ui_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(PuzzleMapScene::menuAction));
	postbox_item->setTag(kPMS_MT_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2-56.f*2.f,0)));
	bottom_case->addChild(postbox_menu);
	
	
	CCSprite* n_cardsetting = CCSprite::create("test_ui_cardsetting.png");
	CCSprite* s_cardsetting = CCSprite::create("test_ui_cardsetting.png");
	s_cardsetting->setColor(ccGRAY);
	
	CCMenuItem* diary_item = CCMenuItemSprite::create(n_cardsetting, s_cardsetting, this, menu_selector(PuzzleMapScene::menuAction));
	diary_item->setTag(kPMS_MT_diary);
	
	CCMenu* diary_menu = CCMenu::createWithItem(diary_item);
	diary_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2-56.f,0)));
	bottom_case->addChild(diary_menu);
	
	
	CCSprite* n_rank = CCSprite::create("test_ui_rank.png");
	CCSprite* s_rank = CCSprite::create("test_ui_rank.png");
	s_rank->setColor(ccGRAY);
	
	CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(PuzzleMapScene::menuAction));
	rank_item->setTag(kPMS_MT_rank);
	
	CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
	rank_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2,0)));
	bottom_case->addChild(rank_menu);
	
	
	CCSprite* n_gacha = CCSprite::create("test_ui_gacha.png");
	CCSprite* s_gacha = CCSprite::create("test_ui_gacha.png");
	s_gacha->setColor(ccGRAY);
	
	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(PuzzleMapScene::menuAction));
	gacha_item->setTag(kPMS_MT_gacha);
	
	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
	gacha_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2+56.f,0)));
	bottom_case->addChild(gacha_menu);
	
	
	CCSprite* n_option = CCSprite::create("test_ui_option.png");
	CCSprite* s_option = CCSprite::create("test_ui_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(PuzzleMapScene::menuAction));
	option_item->setTag(kPMS_MT_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2+56.f*2.f,0)));
	bottom_case->addChild(option_menu);
	
	
	CCSprite* n_left = CCSprite::create("test_ui_left.png");
	CCSprite* s_left = CCSprite::create("test_ui_left.png");
	s_left->setColor(ccGRAY);
	
	CCMenuItem* left_item = CCMenuItemSprite::create(n_left, s_left, this, menu_selector(PuzzleMapScene::menuAction));
	left_item->setTag(kPMS_MT_left);
	
	CCMenu* left_menu = CCMenu::createWithItem(left_item);
	left_menu->setPosition(getUiButtonPosition(kPMS_MT_left));
	main_node->addChild(left_menu, kPMS_Z_arrows, kPMS_MT_left);
	
	
	CCSprite* n_right = CCSprite::create("test_ui_right.png");
	CCSprite* s_right = CCSprite::create("test_ui_right.png");
	s_right->setColor(ccGRAY);
	
	CCMenuItem* right_item = CCMenuItemSprite::create(n_right, s_right, this, menu_selector(PuzzleMapScene::menuAction));
	right_item->setTag(kPMS_MT_right);
	
	CCMenu* right_menu = CCMenu::createWithItem(right_item);
	right_menu->setPosition(getUiButtonPosition(kPMS_MT_right));
	main_node->addChild(right_menu, kPMS_Z_arrows, kPMS_MT_right);
	
	
	CCSprite* n_up = CCSprite::create("test_ui_up.png");
	CCSprite* s_up = CCSprite::create("test_ui_up.png");
	s_up->setColor(ccGRAY);
	
	CCMenuItem* up_item = CCMenuItemSprite::create(n_up, s_up, this, menu_selector(PuzzleMapScene::menuAction));
	up_item->setTag(kPMS_MT_up);
	
	CCMenu* up_menu = CCMenu::createWithItem(up_item);
	up_menu->setPosition(getUiButtonPosition(kPMS_MT_up));
	main_node->addChild(up_menu, kPMS_Z_arrows, kPMS_MT_up);
	
	
	CCSprite* n_selected_card = CCSprite::create("test_ui_selected_card.png");
	CCSprite* s_selected_card = CCSprite::create("test_ui_selected_card.png");
	s_selected_card->setColor(ccGRAY);
	
	CCMenuItem* selected_card_item = CCMenuItemSprite::create(n_selected_card, s_selected_card, this, menu_selector(PuzzleMapScene::menuAction));
	selected_card_item->setTag(kPMS_MT_selectedCard);
	
	CCMenu* selected_card_menu = CCMenu::createWithItem(selected_card_item);
	selected_card_menu->setPosition(getUiButtonPosition(kPMS_MT_selectedCard));
	main_node->addChild(selected_card_menu, kPMS_Z_arrows, kPMS_MT_selectedCard);
	
	
	CCSprite* n_trophy = CCSprite::create("test_ui_trophy.png");
	CCSprite* s_trophy = CCSprite::create("test_ui_trophy.png");
	s_trophy->setColor(ccGRAY);
	
	CCMenuItem* trophy_item = CCMenuItemSprite::create(n_trophy, s_trophy, this, menu_selector(PuzzleMapScene::menuAction));
	trophy_item->setTag(kPMS_MT_trophy);
	
	CCMenu* trophy_menu = CCMenu::createWithItem(trophy_item);
	trophy_menu->setPosition(getUiButtonPosition(kPMS_MT_trophy));
	main_node->addChild(trophy_menu, kPMS_Z_arrows, kPMS_MT_trophy);
	
	
	CCSprite* n_memo = CCSprite::create("test_ui_memo.png");
	CCSprite* s_memo = CCSprite::create("test_ui_memo.png");
	s_memo->setColor(ccGRAY);
	
	CCMenuItem* memo_item = CCMenuItemSprite::create(n_memo, s_memo, this, menu_selector(PuzzleMapScene::menuAction));
	memo_item->setTag(kPMS_MT_memo);
	
	CCMenu* memo_menu = CCMenu::createWithItem(memo_item);
	memo_menu->setPosition(getUiButtonPosition(kPMS_MT_memo));
	main_node->addChild(memo_menu, kPMS_Z_arrows, kPMS_MT_memo);
	
	
	CCSprite* title_box = CCSprite::create("test_ui_titlebox.png");
	title_box->setPosition(getUiButtonPosition(kPMS_MT_titleBox));
	main_node->addChild(title_box, kPMS_Z_arrows, kPMS_MT_titleBox);
	
	int t_puzzle_cnt = NSDS_GI(kSDS_GI_puzzleListCount_i);
	int t_found_index = -1;
	for(int i=0;i<t_puzzle_cnt && t_found_index == -1;i++)
	{
		if(recent_puzzle_number == NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1))
			t_found_index = i+1;
	}
	CCLabelTTF* title_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_puzzleList_int1_title_s, t_found_index).c_str(), mySGD->getFont().c_str(), 17);
	title_label->setPosition(ccp(title_box->getContentSize().width/2.f, title_box->getContentSize().height/2.f));
	title_box->addChild(title_label, 1, 1);
	
	
	CCSprite* n_screen = CCSprite::create("test_ui_screen.png");
	CCSprite* s_screen = CCSprite::create("test_ui_screen.png");
	s_screen->setColor(ccGRAY);
	
	CCMenuItem* screen_item = CCMenuItemSprite::create(n_screen, s_screen, this, menu_selector(PuzzleMapScene::menuAction));
	screen_item->setTag(kPMS_MT_screen);
	
	CCMenu* screen_menu = CCMenu::createWithItem(screen_item);
	screen_menu->setPosition(getUiButtonPosition(kPMS_MT_screen));
	addChild(screen_menu, kPMS_Z_ui_button, kPMS_MT_screen);
	
	
	CCSprite* n_showui = CCSprite::create("test_ui_showui.png");
	CCSprite* s_showui = CCSprite::create("test_ui_showui.png");
	s_showui->setColor(ccGRAY);
	
	CCMenuItem* showui_item = CCMenuItemSprite::create(n_showui, s_showui, this, menu_selector(PuzzleMapScene::menuAction));
	showui_item->setTag(kPMS_MT_showui);
	
	CCMenu* showui_menu = CCMenu::createWithItem(showui_item);
	showui_menu->setPosition(getUiButtonPosition(kPMS_MT_showui));
	addChild(showui_menu, kPMS_Z_ui_button, kPMS_MT_showui);
	
	
//	int event_length = NSDS_GI(kSDS_GI_eventCount_i);
//	if(event_length > 0)
//	{
		CCSprite* n_event = CCSprite::create("test_ui_event.png");
		CCSprite* s_event = CCSprite::create("test_ui_event.png");
		s_event->setColor(ccGRAY);
		
		CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(PuzzleMapScene::menuAction));
		event_item->setTag(kPMS_MT_event);
		
		CCMenu* event_menu = CCMenu::createWithItem(event_item);
		event_menu->setEnabled(false);
		event_menu->setPosition(getUiButtonPosition(kPMS_MT_event));
		main_node->addChild(event_menu, kPMS_Z_ui_button, kPMS_MT_event);
//		
//		
//		CCSprite* n_event_close = CCSprite::create("test_ui_event_close.png");
//		CCSprite* s_event_close = CCSprite::create("test_ui_event_close.png");
//		s_event_close->setColor(ccGRAY);
//		
//		CCMenuItem* event_close_item = CCMenuItemSprite::create(n_event_close, s_event_close, this, menu_selector(PuzzleMapScene::menuAction));
//		event_close_item->setTag(kPMS_MT_eventClose);
//		
//		CCMenu* event_close_menu = CCMenu::createWithItem(event_close_item);
//		event_close_menu->setEnabled(false);
//		event_close_menu->setPosition(getUiButtonPosition(kPMS_MT_eventClose));
//		main_node->addChild(event_close_menu, kPMS_Z_ui_button, kPMS_MT_eventClose);
//		
//		showEventButton();
//	}
}

void PuzzleMapScene::outAllObject()
{
	getChildByTag(kPMS_MT_top)->runAction(CCMoveBy::create(0.3f, ccp(0, 50)));
	getChildByTag(kPMS_MT_showui)->runAction(CCMoveBy::create(0.3f, ccp(0, -50)));
	getChildByTag(kPMS_MT_screen)->runAction(CCMoveBy::create(0.3f, ccp(50, 0)));
	main_node->getChildByTag(kPMS_MT_left)->runAction(CCMoveBy::create(0.3f, ccp(-100, 0)));
	main_node->getChildByTag(kPMS_MT_right)->runAction(CCMoveBy::create(0.3f, ccp(100, 0)));
	main_node->getChildByTag(kPMS_MT_up)->runAction(CCMoveBy::create(0.3f, ccp(0, 200)));
	main_node->getChildByTag(kPMS_MT_selectedCard)->runAction(CCMoveBy::create(0.3f, ccp(-150, 0)));
	main_node->getChildByTag(kPMS_MT_trophy)->runAction(CCMoveBy::create(0.3f, ccp(-150, 0)));
	main_node->getChildByTag(kPMS_MT_memo)->runAction(CCMoveBy::create(0.3f, ccp(150,0)));
	main_node->getChildByTag(kPMS_MT_event)->runAction(CCMoveBy::create(0.3f, ccp(0, -100)));
	main_node->getChildByTag(kPMS_MT_bottom)->runAction(CCMoveBy::create(0.3f, ccp(0, -100)));
	map_node->runAction(CCMoveBy::create(0.3f, ccp(-480,0)));
}

void PuzzleMapScene::inAllObject()
{
	getChildByTag(kPMS_MT_top)->runAction(CCMoveBy::create(0.3f, ccp(0, -50)));
	getChildByTag(kPMS_MT_showui)->runAction(CCMoveBy::create(0.3f, ccp(0, 50)));
	getChildByTag(kPMS_MT_screen)->runAction(CCMoveBy::create(0.3f, ccp(-50, 0)));
	main_node->getChildByTag(kPMS_MT_left)->runAction(CCMoveBy::create(0.3f, ccp(100, 0)));
	main_node->getChildByTag(kPMS_MT_right)->runAction(CCMoveBy::create(0.3f, ccp(-100, 0)));
	main_node->getChildByTag(kPMS_MT_up)->runAction(CCMoveBy::create(0.3f, ccp(0, -200)));
	main_node->getChildByTag(kPMS_MT_selectedCard)->runAction(CCMoveBy::create(0.3f, ccp(150, 0)));
	main_node->getChildByTag(kPMS_MT_trophy)->runAction(CCMoveBy::create(0.3f, ccp(150, 0)));
	main_node->getChildByTag(kPMS_MT_memo)->runAction(CCMoveBy::create(0.3f, ccp(-150, 0)));
	main_node->getChildByTag(kPMS_MT_event)->runAction(CCMoveBy::create(0.3f, ccp(0, 100)));
	main_node->getChildByTag(kPMS_MT_bottom)->runAction(CCMoveBy::create(0.3f, ccp(0, 100)));
	map_node->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(0.3f, ccp(480,0)), CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::popupClose))));
}

void PuzzleMapScene::showEventButton()
{
	CCMenu* event_menu = (CCMenu*)main_node->getChildByTag(kPMS_MT_event);
//	CCMenu* event_close_menu = (CCMenu*)main_node->getChildByTag(kPMS_MT_eventClose);
	
	CCMoveTo* t_move1 = CCMoveTo::create(0.5f, ccp(420,-(myDSH->puzzle_ui_top-320.f)/2.f + 52.f));
//	CCMoveTo* t_move2 = CCMoveTo::create(0.5f, ccp(450,-(myDSH->puzzle_ui_top-320.f)/2.f + 102.f));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::onEventButton));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move1, t_call);
	
	event_menu->runAction(t_seq);
//	event_close_menu->runAction(t_seq);
}
void PuzzleMapScene::onEventButton()
{
	((CCMenu*)main_node->getChildByTag(kPMS_MT_event))->setEnabled(true);
//	((CCMenu*)main_node->getChildByTag(kPMS_MT_eventClose))->setEnabled(true);
}
void PuzzleMapScene::hideEventButton()
{
	((CCMenu*)main_node->getChildByTag(kPMS_MT_event))->setEnabled(false);
//	((CCMenu*)main_node->getChildByTag(kPMS_MT_eventClose))->setEnabled(false);
	
	CCMenu* event_menu = (CCMenu*)main_node->getChildByTag(kPMS_MT_event);
//	CCMenu* event_close_menu = (CCMenu*)main_node->getChildByTag(kPMS_MT_eventClose);
	
	CCMoveTo* t_move1 = CCMoveTo::create(0.5f, getUiButtonPosition(kPMS_MT_event));
//	CCMoveTo* t_move2 = CCMoveTo::create(0.5f, getUiButtonPosition(kPMS_MT_eventClose));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::onEventButton));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move1, t_call);
	
	event_menu->runAction(t_seq);
//	event_close_menu->runAction(t_seq);
}

CCPoint PuzzleMapScene::getUiButtonPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kPMS_MT_event)				return_value = ccp(420,-(myDSH->puzzle_ui_top-320.f)/2.f - 100.f); // after move animation
	else if(t_tag == kPMS_MT_screen)		return_value = ccp(455,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f - 19.f);
	else if(t_tag == kPMS_MT_showui)		return_value = ccp(240,-(myDSH->puzzle_ui_top-320.f)/2.f + 10.f);
	else if(t_tag == kPMS_MT_top)			return_value = ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f + 33.f); // after_move_animation
	else if(t_tag == kPMS_MT_bottom)		return_value = ccp(145,-(myDSH->puzzle_ui_top-320.f)/2.f - 65.f); // after_move_animation
	else if(t_tag == kPMS_MT_left)			return_value = ccp(75, 180.f);
	else if(t_tag == kPMS_MT_right)			return_value = ccp(405, 180.f);
	else if(t_tag == kPMS_MT_up)			return_value = ccp(395, 250);
	else if(t_tag == kPMS_MT_selectedCard)	return_value = ccp(35, 120.f);
	else if(t_tag == kPMS_MT_trophy)		return_value = ccp(80, 240.f);
	else if(t_tag == kPMS_MT_memo)			return_value = ccp(430, 120.f);
	else if(t_tag == kPMS_MT_titleBox)		return_value = ccp(240, 265);
	
	return return_value;
}

void PuzzleMapScene::stageAction(CCObject* sender)
{
	if(!is_menu_enable)		return;
	
	unschedule(schedule_selector(PuzzleMapScene::moveAnimation));
	isAnimated = false;
	
	is_menu_enable = false;
	resetStagePiece();
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == 1 || mySGD->isClearPiece(tag) ||
	   (NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, tag) == 0 &&
		(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, tag) == 0 || mySGD->isClearPiece(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, tag)))))
	{
		mySD->setSilType(tag);
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, recent_puzzle_number);
		
		StageInfoDown* t_sid = StageInfoDown::create(this, callfunc_selector(PuzzleMapScene::showStageSettingPopup), this, callfunc_selector(PuzzleMapScene::stageCancel));
		addChild(t_sid, kPMS_Z_popup);
	}
	else if(mySGD->isClearPiece(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, tag)))
	{
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));
		
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
		
		CCLabelTTF* title_label = CCLabelTTF::create("스테이지 열기", mySGD->getFont().c_str(), 20);
		title_label->setPosition(ccp(0, 102));
		t_container->addChild(title_label);
		
		CCLabelTTF* content_label = CCLabelTTF::create(CCString::createWithFormat("%d gold 로 오픈", NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, tag))->getCString(), mySGD->getFont().c_str(), 18);
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
		
		if(mySGD->getGoodsValue(kGoodsType_gold) >= NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, tag))
		{
			CCSprite* n_buy = CCSprite::create("popup2_buy.png");
			CCSprite* s_buy = CCSprite::create("popup2_buy.png");
			s_buy->setColor(ccGRAY);
			
			CCMenuItemSpriteLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender){
//				mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) - NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, tag));
//				myDSH->setIntegerForKey(kDSH_Key_openStageCnt, myDSH->getIntegerForKey(kDSH_Key_openStageCnt)+1);
//				myDSH->setIntegerForKey(kDSH_Key_openStageNumber_int1, myDSH->getIntegerForKey(kDSH_Key_openStageCnt), tag);
//				myDSH->setBoolForKey(kDSH_Key_isOpenStage_int1, tag, true);
				
				PieceHistory t_history = mySGD->getPieceHistory(tag);
				t_history.is_open = true;
				mySGD->setPieceHistory(t_history, nullptr);
				
//				vector<SaveUserData_Key> save_userdata_list;
//				
//				save_userdata_list.push_back(kSaveUserData_Key_gold);
//				save_userdata_list.push_back(kSaveUserData_Key_openStage);
				
//				myDSH->saveUserData(save_userdata_list, nullptr);
				
				
				CCNode* open_puzzle_container = CCNode::create();
				t_popup->setContainerNode(open_puzzle_container);
				
				CCScale9Sprite* open_puzzle_case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
				open_puzzle_case_back->setPosition(CCPointZero);
				open_puzzle_container->addChild(open_puzzle_case_back);
				
				open_puzzle_case_back->setContentSize(CCSizeMake(230, 250));
				
				CCScale9Sprite* open_puzzle_content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
				open_puzzle_content_back->setPosition(ccp(0,2));
				open_puzzle_container->addChild(open_puzzle_content_back);
				
				open_puzzle_content_back->setContentSize(CCSizeMake(202, 146));
				
				CCLabelTTF* open_puzzle_title_label = CCLabelTTF::create("스테이지 오픈", mySGD->getFont().c_str(), 20);
				open_puzzle_title_label->setPosition(ccp(0, 102));
				open_puzzle_container->addChild(open_puzzle_title_label);
				
				CCLabelTTF* open_puzzle_content_label = CCLabelTTF::create("새로운 스테이지가\n오픈 되었습니다.", mySGD->getFont().c_str(), 18);
				open_puzzle_content_label->setPosition(CCPointZero);
				open_puzzle_container->addChild(open_puzzle_content_label);
				
				CCSprite* n_op_ok = CCSprite::create("popup2_ok.png");
				CCSprite* s_op_ok = CCSprite::create("popup2_ok.png");
				s_op_ok->setColor(ccGRAY);
				
				CCMenuItemSpriteLambda* op_ok_item = CCMenuItemSpriteLambda::create(n_op_ok, s_op_ok, [=](CCObject* sender){
					is_menu_enable = true;
					t_popup->removeFromParent();
				});
				
				CCMenuLambda* op_ok_menu = CCMenuLambda::createWithItem(op_ok_item);
				op_ok_menu->setTouchPriority(t_popup->getTouchPriority()-1);
				op_ok_menu->setPosition(ccp(0,-95));
				open_puzzle_container->addChild(op_ok_menu);
				
				StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(tag);
				CCNode* t_parent = t_sp->getParent();
				int stage_number = t_sp->getStageNumber();
				int stage_level = t_sp->getStageLevel();
				CCPoint sp_position = t_sp->getPosition();
				CCRect touch_rect = t_sp->getTouchRect();
				string piece_type = t_sp->getPieceType().c_str();
				
				t_sp->removeFromParent();
				
				t_sp = StagePiece::create("test_puzzle_empty.png",
													  stage_number, stage_level, sp_position, touch_rect, false, false, piece_type.c_str(),
													  this, menu_selector(PuzzleMapScene::stageAction));
				t_sp->mySetTouchEnable(false);
				
				if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		t_parent->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
				else														t_parent->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
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
	else
	{
		is_menu_enable = true;
	}
}

void PuzzleMapScene::showStageSettingPopup()
{
	setTouchEnabled(false);
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(false);
	
	for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
		if(t_sp)
			t_sp->mySetTouchEnable(false);
	}
	
	is_gesturable_map_mode = false;
	map_mode_state = kMMS_default;
	is_menu_enable = false;
	
	StageSettingPopup* t_popup = StageSettingPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::hideStageSettingPopup));
	addChild(t_popup, kPMS_Z_popup);
}

void PuzzleMapScene::hideStageSettingPopup()
{
	mySGD->setStarLabel(ruby_label);
	mySGD->setGoldLabel(gold_label);
	
	is_gesturable_map_mode = true;
	map_mode_state = kMMS_default;
	is_menu_enable = true;
	
	for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
		if(t_sp)
			t_sp->mySetTouchEnable(true);
	}
	
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
	setTouchEnabled(true);
}

void PuzzleMapScene::showClearPopup()
{
	setTouchEnabled(false);
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(false);
	
	for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
		if(t_sp)
			t_sp->mySetTouchEnable(false);
	}
	
	is_gesturable_map_mode = false;
	is_menu_enable = false;
	
	ClearPopup* t_popup = ClearPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::hideClearPopup));
	addChild(t_popup, kPMS_Z_popup);
}

void PuzzleMapScene::hideClearPopup()
{
	if(mySD->getSilType() >= 10000)
	{
		map_mode_state = kMMS_uiMode;
		mySGD->setStarLabel(ruby_label);
		mySGD->setGoldLabel(gold_label);
		
		is_menu_enable = true;
		
		setTouchEnabled(true);
	}
	else
	{
		map_mode_state = kMMS_default;
		showGetPuzzle();
	}
}

void PuzzleMapScene::showGetPuzzle()
{
	CCSprite* get_piece_title = CCSprite::create(CCString::createWithFormat("get_piece_title_%s.png", myLoc->getSupportLocalCode())->getCString());
	StagePiece* new_piece = (StagePiece*)map_node->getChildByTag(mySD->getSilType());
	get_piece_title->setPosition(ccpAdd(new_piece->getPosition(), ccp(0, 45)));
	map_node->addChild(get_piece_title, kPMS_Z_popup);
	
	new_piece->startGetPieceAnimation(this, callfuncCCp_selector(PuzzleMapScene::createGetPuzzleParticle));
	
	CCDelayTime* t_delay = CCDelayTime::create(1.f);
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endGetPuzzle));
	CCCallFunc* t_call2 = CCCallFunc::create(get_piece_title, callfunc_selector(CCSprite::removeFromParent));
	CCSequence* t_seq = CCSequence::create(t_delay, t_fade, t_call1, t_call2, NULL);
	get_piece_title->runAction(t_seq);
}

void PuzzleMapScene::createGetPuzzleParticle(CCPoint t_point)
{
	random_device rd;
	default_random_engine e1(rd());
	uniform_real_distribution<float> uniform_dist(-50, 50);
	
	CCPoint random_value;
	random_value.x = uniform_dist(e1);
	random_value.y = uniform_dist(e1);
	
	CCParticleSystemQuad* t_particle = CCParticleSystemQuad::createWithTotalParticles(150);
	t_particle->setPositionType(kCCPositionTypeRelative);
	t_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("get_piece_particle.png"));
	t_particle->setEmissionRate(400);
	t_particle->setAngle(90.0);
	t_particle->setAngleVar(45.0);
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	t_particle->setBlendFunc(blendFunc);
	t_particle->setDuration(0.1);
	t_particle->setEmitterMode(kCCParticleModeGravity);
	t_particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 1.f));
	t_particle->setStartColorVar(ccc4f(0.57f, 0.57f, 0.54f, 0.f));
	t_particle->setEndColor(ccc4f(1.f, 1.f, 1.f, 0.f));
	t_particle->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
	t_particle->setStartSize(10.0);
	t_particle->setStartSizeVar(5.0);
	t_particle->setEndSize(20.0);
	t_particle->setEndSizeVar(5.0);
	t_particle->setGravity(ccp(0,-400));
	t_particle->setRadialAccel(0.0);
	t_particle->setRadialAccelVar(0.0);
	t_particle->setSpeed(150);
	t_particle->setSpeedVar(70.0);
	t_particle->setTangentialAccel(0);
	t_particle->setTangentialAccelVar(0);
	t_particle->setTotalParticles(150);
	t_particle->setLife(0.40);
	t_particle->setLifeVar(0.5);
	t_particle->setStartSpin(0);
	t_particle->setStartSpinVar(180);
	t_particle->setEndSpin(0);
	t_particle->setEndSpinVar(180);
	t_particle->setPosVar(ccp(10,10));
	t_particle->setPosition(ccpAdd(t_point, random_value));
	t_particle->setAutoRemoveOnFinish(true);
	map_node->addChild(t_particle, kPMS_Z_popup);
}

void PuzzleMapScene::endGetPuzzle()
{
	mySGD->setStarLabel(ruby_label);
	mySGD->setGoldLabel(gold_label);
	
	is_gesturable_map_mode = true;
	map_mode_state = kMMS_default;
	is_menu_enable = true;
	
	for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
		if(t_sp)
			t_sp->mySetTouchEnable(true);
	}
	
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
	setTouchEnabled(true);
}

void PuzzleMapScene::showFailPopup()
{
	setTouchEnabled(false);
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(false);
	
	for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
		if(t_sp)
			t_sp->mySetTouchEnable(false);
	}
	
	is_gesturable_map_mode = false;
	is_menu_enable = false;
	
	FailPopup* t_popup = FailPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::hideFailPopup));
	addChild(t_popup, kPMS_Z_popup);
}

void PuzzleMapScene::hideFailPopup()
{
	if(mySD->getSilType() >= 10000)
	{
		map_mode_state = kMMS_uiMode;
		mySGD->setStarLabel(ruby_label);
		mySGD->setGoldLabel(gold_label);
		
		is_menu_enable = true;
		
		setTouchEnabled(true);
	}
	else
	{
		map_mode_state = kMMS_default;
	
		mySGD->setStarLabel(ruby_label);
		mySGD->setGoldLabel(gold_label);
		
		is_gesturable_map_mode = true;
		map_mode_state = kMMS_default;
		is_menu_enable = true;
		
		for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
		{
			StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
			if(t_sp)
				t_sp->mySetTouchEnable(true);
		}
		
		((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
		setTouchEnabled(true);
	}
}

void PuzzleMapScene::showCardSettingPopup()
{
	puzzleUiDisable();
	
	CardSettingPopup* t_popup = CardSettingPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::hideCardSettingPopup));
	addChild(t_popup, kPMS_Z_popup);
}

void PuzzleMapScene::hideCardSettingPopup()
{
	puzzleUiEnable();
}

void PuzzleMapScene::showDiaryPopup()
{
	puzzleUiDisable();
	
	CollectionBookPopup* t_popup = CollectionBookPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::hideDiaryPopup));
	addChild(t_popup, kPMS_Z_popup);
}
void PuzzleMapScene::hideDiaryPopup()
{
	puzzleUiEnable();
}

void PuzzleMapScene::showOptionPopup()
{
	puzzleUiDisable();
	
	OptionPopup* t_popup = OptionPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::hideOptionPopup));
	addChild(t_popup, kPMS_Z_popup);
}

void PuzzleMapScene::hideOptionPopup()
{
	puzzleUiEnable();
}

void PuzzleMapScene::puzzleUiDisable()
{
	setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_event))->setTouchEnabled(false);
	
	((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_event))->setEnabled(false);
	
	is_menu_enable = false;
	
	
	CCSprite* top_case = (CCSprite*)getChildByTag(kPMS_MT_top);
	CCMoveTo* top_move = CCMoveTo::create(0.3f, ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f + 33.f));
	top_case->runAction(top_move);
	
	CCSprite* bottom_case = (CCSprite*)main_node->getChildByTag(kPMS_MT_bottom);
	CCMoveTo* bottom_move = CCMoveTo::create(0.4f, ccp(145,-(myDSH->puzzle_ui_top-320.f)/2.f - 65.f));
	bottom_case->runAction(bottom_move);
}

void PuzzleMapScene::puzzleUiEnable()
{
	mySGD->setStarLabel(ruby_label);
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* top_case = (CCSprite*)getChildByTag(kPMS_MT_top);
	CCMoveTo* top_move = CCMoveTo::create(0.3f, ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	top_case->runAction(top_move);
	
	top_case->removeChildByTag(kPMS_MT_heartTime);
	HeartTime* heart_time = HeartTime::create();
	heart_time->setPosition(ccp(250,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time, 0, kPMS_MT_heartTime);
	
	CCSprite* bottom_case = (CCSprite*)main_node->getChildByTag(kPMS_MT_bottom);
	CCMoveTo* bottom_move = CCMoveTo::create(0.4f, ccp(145,-(myDSH->puzzle_ui_top-320.f)/2.f));
	bottom_case->runAction(bottom_move);
	
	is_menu_enable = true;
	
	
	((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_event))->setTouchEnabled(true);
	
	((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_event))->setEnabled(true);
	
	setTouchEnabled(true);
}

void PuzzleMapScene::showEventPopup()
{
	puzzleUiDisable();
	
//	EventPopup* t_popup = EventPopup::create();
//	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::hideEventPopup));
//	addChild(t_popup, kPMS_Z_popup);
}

void PuzzleMapScene::hideEventPopup()
{
	puzzleUiEnable();
}

void PuzzleMapScene::showAcceptStageSettingPopup()
{
	puzzleUiDisable();
	
	StageSettingPopup* t_popup = StageSettingPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::puzzleUiEnable));
	addChild(t_popup, kPMS_Z_popup);
}

void PuzzleMapScene::startChangeUiMode()
{
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_init);
	
	CCSprite* top_case = (CCSprite*)getChildByTag(kPMS_MT_top);
	top_case->removeChildByTag(kPMS_MT_heartTime);
	HeartTime* heart_time = HeartTime::create();
	heart_time->setPosition(ccp(250,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time, 0, kPMS_MT_heartTime);
	
	is_gesturable_map_mode = false;
	map_mode_state = kMMS_changeMode;
	is_from_call_map_mode = false;
	
	resetStagePiece();
	
	for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
		if(t_sp)
			t_sp->mySetTouchEnable(false);
	}
	
	this->unschedule(schedule_selector(PuzzleMapScene::moveAnimation));
	isAnimated=false;
	
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(false);
//	((CCMenu*)main_node->getChildByTag(kPMS_MT_eventClose))->setVisible(false);
	
	if(((CCMenu*)main_node->getChildByTag(kPMS_MT_event))->getPositionY() < -(myDSH->puzzle_ui_top-320.f)/2.f - 24.f)
		showEventButton();
	
	change_frame = 0;
	change_dy = -1.f/30.f;
	change_dscale = (0.53f-map_node->getScale())/30.f;
	change_dpoint = ccpMult(ccpSub(ccp(240,180), map_node->getPosition()), 1.f/30.f);
	change_top_dy = -33.f/30.f;
	change_bottom_dy = 65.f/30.f;
	schedule(schedule_selector(PuzzleMapScene::changeUiMode));
}
void PuzzleMapScene::changeUiMode()
{
	change_frame++;
	
	float t_x, t_y, t_z;
	map_node->getCamera()->getEyeXYZ(&t_x, &t_y, &t_z);
	map_node->getCamera()->setEyeXYZ(t_x, t_y+change_dy, t_z);
	map_node->setScale(map_node->getScale()+change_dscale);
	map_node->setPosition(ccpAdd(map_node->getPosition(), change_dpoint));
	
	CCSprite* top_case = (CCSprite*)getChildByTag(kPMS_MT_top);
	top_case->setPositionY(top_case->getPositionY() + change_top_dy);
	
	CCSprite* bottom_case = (CCSprite*)main_node->getChildByTag(kPMS_MT_bottom);
	bottom_case->setPositionY(bottom_case->getPositionY() + change_bottom_dy);
	
	if(change_frame >= 30)
		stopChangeUiMode();
}
void PuzzleMapScene::stopChangeUiMode()
{
	unschedule(schedule_selector(PuzzleMapScene::changeUiMode));
	
	((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setTouchEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setTouchEnabled(true);
	
	((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setEnabled(true);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setEnabled(true);
	
	map_mode_state = kMMS_uiMode;
	is_menu_enable = true;
}

void PuzzleMapScene::startChangeMapMode()
{
	is_menu_enable = false;
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
	map_mode_state = kMMS_changeMode;
	
	is_from_call_map_mode = true;
	
	((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setTouchEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setTouchEnabled(false);
	
	((CCMenu*)main_node->getChildByTag(kPMS_MT_left))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_right))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_up))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_selectedCard))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_trophy))->setEnabled(false);
	((CCMenu*)main_node->getChildByTag(kPMS_MT_memo))->setEnabled(false);
	
	change_frame = 0;
	change_dy = 1.f/30.f;
	change_dscale = (1.f-map_node->getScale())/30.f;
	change_dpoint = ccpMult(ccpSub(ccp(240,160), map_node->getPosition()), 1.f/30.f);
	change_top_dy = 33.f/30.f;
	change_bottom_dy = -65.f/30.f;
	schedule(schedule_selector(PuzzleMapScene::changeMapMode));
	hideEventButton();
}
void PuzzleMapScene::changeMapMode()
{
	change_frame++;
	
	float t_x, t_y, t_z;
	map_node->getCamera()->getEyeXYZ(&t_x, &t_y, &t_z);
	map_node->getCamera()->setEyeXYZ(t_x, t_y+change_dy, t_z);
	map_node->setScale(map_node->getScale()+change_dscale);
	map_node->setPosition(ccpAdd(map_node->getPosition(), change_dpoint));
	
	CCSprite* top_case = (CCSprite*)getChildByTag(kPMS_MT_top);
	top_case->setPositionY(top_case->getPositionY() + change_top_dy);
	
	CCSprite* bottom_case = (CCSprite*)main_node->getChildByTag(kPMS_MT_bottom);
	bottom_case->setPositionY(bottom_case->getPositionY() + change_bottom_dy);
	
	if(change_frame >= 30)
		stopChangeMapMode();
}
void PuzzleMapScene::stopChangeMapMode()
{
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
	
	for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
		if(t_sp)
			t_sp->mySetTouchEnable(true);
	}
	
	unschedule(schedule_selector(PuzzleMapScene::changeMapMode));
	is_gesturable_map_mode = true;
	map_mode_state = kMMS_default;
	is_menu_enable = true;
}

void PuzzleMapScene::startChangeFrameMode()
{
	if(getChildByTag(kPMS_MT_loadPuzzleInfo))
		removeChildByTag(kPMS_MT_loadPuzzleInfo);
	
	is_gesturable_map_mode = false;
	map_mode_state = kMMS_changeMode;
	is_menu_enable = false;
	
	puzzle_list_view->startViewCheck();
	
	
	CCScaleTo* t_scale = CCScaleTo::create(0.45f, map_node->getScaleX(), map_node->getScaleY()-0.65f);
	map_node->runAction(t_scale);
	
	CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(0,-340));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::stopChangeFrameMode));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
	main_node->runAction(t_seq);
	
	hideEventButton();
}
void PuzzleMapScene::stopChangeFrameMode()
{
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
	
	puzzle_list_view->stopViewCheck();
	puzzle_list_view->setTouchEnabled(true);
	
	map_mode_state = kMMS_frameMode;
	is_menu_enable = true;
}

void PuzzleMapScene::startReturnUiMode()
{
	puzzle_list_view->setTouchEnabled(false);
	puzzle_list_view->touchCancel();
	
	is_gesturable_map_mode = false;
	map_mode_state = kMMS_changeMode;
	is_menu_enable = false;
	
	resetStagePiece();
	
	showEventButton();
	
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(false);
	
	CCScaleTo* t_scale = CCScaleTo::create(0.45f, map_node->getScaleX(), map_node->getScaleX());
	map_node->runAction(t_scale);
	
	CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(0,0));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::stopReturnUiMode));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
	main_node->runAction(t_seq);
}
void PuzzleMapScene::stopReturnUiMode()
{
	
	if(map_mode_state != kMMS_notLoadMode)
		map_mode_state = kMMS_uiMode;
	is_menu_enable = true;
}

void PuzzleMapScene::changePiece(CCObject* sender)
{
	StagePiece* t_sp = (StagePiece*)sender;
	t_sp->setPuzzleMode(my_puzzle_mode);
	if(my_puzzle_mode == kPM_default && t_sp->isBoarder())
		map_node->reorderChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber());
	else
		map_node->reorderChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber());
}

void PuzzleMapScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	CCLOG("menu %d", tag);
	
	if(tag == kPMS_MT_event)
	{
		resetStagePiece();
		
		showEventPopup();
	}
	else if(tag == kPMS_MT_screen)
	{
		if(my_puzzle_mode == kPM_default)
		{
			startPuzzleModeChange(kPM_thumb);
		}
		else if(my_puzzle_mode == kPM_thumb)
		{
			startPuzzleModeChange(kPM_default);
		}
	}
	else if(tag == kPMS_MT_showui)
	{
		if(map_mode_state == kMMS_frameMode)
			startReturnUiMode();
		else
			startChangeUiMode();
	}
	else if(tag == kPMS_MT_diary)
	{
		mySGD->resetLabels();
		mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
		showCardSettingPopup();
	}
	else if(tag == kPMS_MT_option)
	{
		mySGD->resetLabels();
		showOptionPopup();
	}
	else if(tag == kPMS_MT_gacha)
	{
		GachaPurchase* t_gp = GachaPurchase::create();
		addChild(t_gp, kPMS_Z_popup);
		
		t_gp->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::popupClose));
		t_gp->setOutAllObjectAction(this, callfunc_selector(PuzzleMapScene::outAllObject));
		t_gp->setInAllObjectAction(this, callfunc_selector(PuzzleMapScene::inAllObject));
	}
	else if(tag == kPMS_MT_rank)
	{
		RankPopup* t_rp = RankPopup::create(this, callfunc_selector(PuzzleMapScene::popupClose));
		addChild(t_rp, kPMS_Z_popup);
	}
	else if(tag == kPMS_MT_postbox)
	{
		MailPopup* t_pp = MailPopup::create(this, callfunc_selector(PuzzleMapScene::popupClose), nullptr);
		addChild(t_pp, kPMS_Z_popup);
	}
	else if(tag == kPMS_MT_selectedCard)
	{
		mySGD->resetLabels();
		mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
		showCardSettingPopup();
	}
	else if(tag == kPMS_MT_trophy)
	{
		RankPopup* t_rp = RankPopup::create(this, callfunc_selector(PuzzleMapScene::popupClose));
		addChild(t_rp, kPMS_Z_popup);
	}
	else if(tag == kPMS_MT_memo)
	{
		if(mySGD->getHasGottenCardsDataCardNumber(0) <= 0)
		{
			is_menu_enable = true;
		}
		else
		{
			mySGD->resetLabels();
			mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
			mySGD->selected_collectionbook = mySGD->getHasGottenCardsDataCardNumber(0);
			showDiaryPopup();
		}
	}
	else if(tag == kPMS_MT_left)
	{
		int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
		bool is_found = false;
		int found_index = -1;
		for(int i=0;i<puzzle_count && !is_found;i++)
		{
			int t_puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
			if(recent_puzzle_number-1 == t_puzzle_no)
			{
				is_found = true;
				found_index = i+1;
				break;
			}
		}
		
		if(is_found)
		{
			if(map_mode_state == kMMS_notLoadMode)
			{
				if(getChildByTag(kPMS_MT_loadPuzzleInfo))
					removeChildByTag(kPMS_MT_loadPuzzleInfo);
				removeChildByTag(kPMS_MT_puzzleOpenTitle);
				removeChildByTag(kPMS_MT_ticketCnt);
				removeChildByTag(kPMS_MT_callTicket);
				removeChildByTag(kPMS_MT_buyPuzzle);
			}
			
			recent_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, found_index);
			
			map_mode_state = kMMS_changeMode;
			is_menu_enable = false;
			
			after_map_node = createMapNode();
			after_map_node->setPosition(ccp(240-480,180));
			main_node->addChild(after_map_node, kPMS_Z_puzzle_back_side);
			
			if(after_map_node->getTag() == kPMS_MT_loaded)
			{
				after_map_node->setTag(-1);
				
				cachingPuzzleImg2();
				//								switchMapNode(after_map_node);
				
				CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
				CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endLoadedMovingMapNode));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
				
				map_node->runAction(t_move1);
				after_map_node->runAction(t_seq);
			}
			else if(after_map_node->getTag() == kPMS_MT_notOpenedPuzzle)
			{
				after_map_node->setTag(-1);
				
				CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
				CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotOpenPuzzle));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
				
				map_node->runAction(t_move1);
				after_map_node->runAction(t_seq);
			}
			else if(after_map_node->getTag() == kPMS_MT_notClearBeforePuzzle)
			{
				after_map_node->setTag(-1);
				
				CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
				CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotClearPuzzle));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
				
				map_node->runAction(t_move1);
				after_map_node->runAction(t_seq);
			}
			else
			{
				after_map_node->setTag(-1);
				
				CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
				CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNode));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
				
				map_node->runAction(t_move1);
				after_map_node->runAction(t_seq);
			}
		}
		else
		{
			CCLOG("nothing pre puzzle!!!");
			
			original_mms = map_mode_state;
			
			map_mode_state = kMMS_changeMode;
			is_menu_enable = false;
			
			CCMoveBy* t_move1 = CCMoveBy::create(0.2f, ccp(40,0));
			CCMoveBy* t_move2 = CCMoveBy::create(0.1f, ccp(-40,0));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endTingMapNode));
			CCSequence* t_seq = CCSequence::create(t_move1, t_move2, t_call, NULL);
			map_node->runAction(t_seq);
		}
	}
	else if(tag == kPMS_MT_right)
	{
		int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
		bool is_found = false;
		int found_index = -1;
		for(int i=0;i<puzzle_count && !is_found;i++)
		{
			int t_puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
			if(recent_puzzle_number+1 == t_puzzle_no)
			{
				is_found = true;
				found_index = i+1;
				break;
			}
		}
		
		if(is_found && found_index <= puzzle_count)
		{
			if(map_mode_state == kMMS_notLoadMode)
			{
				if(getChildByTag(kPMS_MT_loadPuzzleInfo))
					removeChildByTag(kPMS_MT_loadPuzzleInfo);
				removeChildByTag(kPMS_MT_puzzleOpenTitle);
				removeChildByTag(kPMS_MT_ticketCnt);
				removeChildByTag(kPMS_MT_callTicket);
				removeChildByTag(kPMS_MT_buyPuzzle);
			}
			
			recent_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, found_index);
			
			map_mode_state = kMMS_changeMode;
			is_menu_enable = false;
			
			after_map_node = createMapNode();
			after_map_node->setPosition(ccp(240+480,180));
			main_node->addChild(after_map_node, kPMS_Z_puzzle_back_side);
			
			if(after_map_node->getTag() == kPMS_MT_loaded)
			{
				after_map_node->setTag(-1);
				
				cachingPuzzleImg2();
				//								switchMapNode(after_map_node);
				
				CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
				CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endLoadedMovingMapNode));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
				
				map_node->runAction(t_move1);
				after_map_node->runAction(t_seq);
			}
			else if(after_map_node->getTag() == kPMS_MT_notOpenedPuzzle)
			{
				after_map_node->setTag(-1);
				
				CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
				CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotOpenPuzzle));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
				
				map_node->runAction(t_move1);
				after_map_node->runAction(t_seq);
			}
			else if(after_map_node->getTag() == kPMS_MT_notClearBeforePuzzle)
			{
				after_map_node->setTag(-1);
				
				CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
				CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotClearPuzzle));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
				
				map_node->runAction(t_move1);
				after_map_node->runAction(t_seq);
			}
			else
			{
				after_map_node->setTag(-1);
				
				CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
				CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNode));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
				
				map_node->runAction(t_move1);
				after_map_node->runAction(t_seq);
			}
		}
		else
		{
			CCLOG("nothing next puzzle!!!");
			
			original_mms = map_mode_state;
			
			map_mode_state = kMMS_changeMode;
			is_menu_enable = false;
			
			CCMoveBy* t_move1 = CCMoveBy::create(0.2f, ccp(-40,0));
			CCMoveBy* t_move2 = CCMoveBy::create(0.1f, ccp(40,0));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endTingMapNode));
			CCSequence* t_seq = CCSequence::create(t_move1, t_move2, t_call, NULL);
			map_node->runAction(t_seq);
		}
	}
	else if(tag == kPMS_MT_up)
	{
		if(map_mode_state == kMMS_uiMode)
			startChangeFrameMode();
		else if(map_mode_state == kMMS_notLoadMode)
		{
			if(getChildByTag(kPMS_MT_loadPuzzleInfo))
				removeChildByTag(kPMS_MT_loadPuzzleInfo);
			removeChildByTag(kPMS_MT_puzzleOpenTitle);
			removeChildByTag(kPMS_MT_ticketCnt);
			removeChildByTag(kPMS_MT_callTicket);
			removeChildByTag(kPMS_MT_buyPuzzle);
			
			startChangeFrameMode();
		}
	}
	else if(tag == kPMS_MT_rubyShop)
	{
//		ShopPopup* t_shop = ShopPopup::create();
//		t_shop->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::popupClose));
//		t_shop->targetHeartTime((HeartTime*)(getChildByTag(kPMS_MT_top)->getChildByTag(kPMS_MT_heartTime)));
//		t_shop->setShopCode(kSC_ruby);
//		addChild(t_shop, kPMS_Z_popup);
	}
	else if(tag == kPMS_MT_goldShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::popupClose));
		t_shop->targetHeartTime((HeartTime*)(getChildByTag(kPMS_MT_top)->getChildByTag(kPMS_MT_heartTime)));
		t_shop->setShopCode(kSC_gold);
		addChild(t_shop, kPMS_Z_popup);
	}
	else if(tag == kPMS_MT_lifeShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(PuzzleMapScene::popupClose));
		t_shop->targetHeartTime((HeartTime*)(getChildByTag(kPMS_MT_top)->getChildByTag(kPMS_MT_heartTime)));
		t_shop->setShopCode(kSC_heart);
		addChild(t_shop, kPMS_Z_popup);
	}
	else if(tag == kPMS_MT_friendPoint)
	{
		if(!friend_point_popup)
		{
			CCNode* menu_node = ((CCNode*)pSender)->getParent();
			CCNode* top_node = menu_node->getParent();
			friend_point_popup = CCSprite::create("candy_popup.png");
			friend_point_popup->setAnchorPoint(ccp(0.5,1.f));
			friend_point_popup->setPosition(ccp(427,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
			top_node->addChild(friend_point_popup, -1);
			
			CCSprite* n_close = CCSprite::create("candy_popup_close.png");
			CCSprite* s_close = CCSprite::create("candy_popup_close.png");
			s_close->setColor(ccGRAY);
			
			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(PuzzleMapScene::menuAction));
			close_item->setTag(kPMS_MT_friendPointClose);
			
			CCMenu* close_menu = CCMenu::createWithItem(close_item);
			close_menu->setPosition(ccp(friend_point_popup->getContentSize().width/2.f, 25));
			friend_point_popup->addChild(close_menu);
			
			CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(427,menu_node->getPositionY()-12));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::popupClose));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
			friend_point_popup->runAction(t_seq);
		}
		else
			is_menu_enable = true;
	}
	else if(tag == kPMS_MT_friendPointClose)
	{
		CCNode* menu_node = ((CCNode*)pSender)->getParent();
		CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(427,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::closeFriendPointPopup));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
		friend_point_popup->runAction(t_seq);
	}
}

void PuzzleMapScene::closeFriendPointPopup()
{
	friend_point_popup->removeFromParent();
	friend_point_popup = NULL;
	is_menu_enable = true;
}

void PuzzleMapScene::popupClose()
{
	is_menu_enable = true;
}

void PuzzleMapScene::stageCancel()
{
	is_menu_enable = true;
}

CCPoint PuzzleMapScene::getStagePosition(int stage)
{
	CCPoint return_value;
	
//	return_value.x = SDS_GI(kSDF_gameInfo, CCSTR_CWF("stage%d_x", stage)->getCString());
//	return_value.y = SDS_GI(kSDF_gameInfo, CCSTR_CWF("stage%d_y", stage)->getCString());
//	
//	return_value = ccpMult(return_value, 40.f);
//	return_value = ccpAdd(return_value, ccp(20.f,20.f));
	
	return return_value;
}

void PuzzleMapScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if(map_mode_state == kMMS_changeMode || map_mode_state == kMMS_loadChangingMode)
		return;
	else if(map_mode_state == kMMS_uiMode)
	{
		CCSetIterator iter;
		CCTouch *touch;
		
		for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			multiTouchData[(int)touch] = location;
			
			if(multiTouchData.size() == 1)
			{
				touchStart_p = location;
			}
		}
	}
	else if(map_mode_state == kMMS_frameMode)
	{
		CCSetIterator iter;
		CCTouch *touch;
		
		for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			multiTouchData[(int)touch] = location;
			
			if(multiTouchData.size() == 1)
			{
				touchStart_p = location;
			}
		}
	}
	else if(map_mode_state == kMMS_notLoadMode)
	{
		CCSetIterator iter;
		CCTouch *touch;
		
		for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			multiTouchData[(int)touch] = location;
			
			if(multiTouchData.size() == 1)
			{
				touchStart_p = location;
			}
		}
	}
	else
	{
		CCSetIterator iter;
		CCTouch *touch;
		
		for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			multiTouchData[(int)touch] = location;
			
			touch_p = location;
			
			this->unschedule(schedule_selector(PuzzleMapScene::moveAnimation));
			isAnimated=false;
			
			if(multiTouchData.size() == 1)
			{
				timeval time;
				gettimeofday(&time, NULL);
				touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
				touchStart_p = location;
				
				map_mode_state = kMMS_firstTouchDefault;
				
				//					CCPoint convert_location = convertToWorldSpace(location);
				
				for(int i=start_stage_number;i<start_stage_number+stage_count && touched_stage_number == 0;i++)
				{
					StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
					if(t_sp && t_sp->touchBegan(touch, pEvent))
						//						if(t_sp->isInnerRect(convert_location))
					{
						touched_stage_number = t_sp->getStageNumber();
						t_sp->setTouchBegin();
						map_mode_state = kMMS_firstTouchStage;
					}
				}
			}
			else if(multiTouchData.size() == 2)
			{
				is_gesturable_map_mode = false;
				if(map_mode_state == kMMS_firstTouchStage)
					resetStagePiece();
				
				map_mode_state = kMMS_multiTouchZoom;
				CCPoint sub_point = CCPointZero;
				map<int, CCPoint>::iterator it;
				for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
				{
					sub_point = ccpMult(sub_point, -1);
					sub_point = ccpAdd(sub_point, it->second);
				}
				
				zoom_base_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
			}
			else
			{
				is_gesturable_map_mode = false;
			}
		}
	}
}

void PuzzleMapScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if(map_mode_state == kMMS_changeMode || map_mode_state == kMMS_loadChangingMode)
		return;
	else if(map_mode_state == kMMS_uiMode)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				o_it->second = location;
				if(multiTouchData.size() == 1)
				{
					if(location.y < touchStart_p.y - 50.f)
					{
						touchStart_p = location;
						startChangeFrameMode();
						multiTouchData.erase((int)touch);
					}
					else if(location.x < touchStart_p.x - 50.f) // next
					{
						int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
						bool is_found = false;
						int found_index = -1;
						for(int i=0;i<puzzle_count && !is_found;i++)
						{
							int t_puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
							if(recent_puzzle_number+1 == t_puzzle_no)
							{
								is_found = true;
								found_index = i+1;
								break;
							}
						}
						
						touchStart_p = location;
						
						if(is_found && found_index <= puzzle_count)
						{
							if(getChildByTag(kPMS_MT_loadPuzzleInfo))
								removeChildByTag(kPMS_MT_loadPuzzleInfo);
							recent_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, found_index);
							
							map_mode_state = kMMS_changeMode;
							is_menu_enable = false;
							
							after_map_node = createMapNode();
							after_map_node->setPosition(ccp(240+480,180));
							main_node->addChild(after_map_node, kPMS_Z_puzzle_back_side);
							
							if(after_map_node->getTag() == kPMS_MT_loaded)
							{
								after_map_node->setTag(-1);
								
								cachingPuzzleImg2();
//								switchMapNode(after_map_node);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endLoadedMovingMapNode));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else if(after_map_node->getTag() == kPMS_MT_notOpenedPuzzle)
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotOpenPuzzle));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else if(after_map_node->getTag() == kPMS_MT_notClearBeforePuzzle)
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotClearPuzzle));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNode));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
						}
						else
						{
							CCLOG("nothing next puzzle!!!");
							
							original_mms = kMMS_uiMode;
							
							map_mode_state = kMMS_changeMode;
							is_menu_enable = false;
							
							CCMoveBy* t_move1 = CCMoveBy::create(0.2f, ccp(-40,0));
							CCMoveBy* t_move2 = CCMoveBy::create(0.1f, ccp(40,0));
							CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endTingMapNode));
							CCSequence* t_seq = CCSequence::create(t_move1, t_move2, t_call, NULL);
							map_node->runAction(t_seq);
						}
					}
					else if(location.x > touchStart_p.x + 50.f) // pre
					{
						int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
						bool is_found = false;
						int found_index = -1;
						for(int i=0;i<puzzle_count && !is_found;i++)
						{
							int t_puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
							if(recent_puzzle_number-1 == t_puzzle_no)
							{
								is_found = true;
								found_index = i+1;
								break;
							}
						}
						
						touchStart_p = location;
						
						if(is_found)
						{
							if(getChildByTag(kPMS_MT_loadPuzzleInfo))
								removeChildByTag(kPMS_MT_loadPuzzleInfo);
							recent_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, found_index);
							
							map_mode_state = kMMS_changeMode;
							is_menu_enable = false;
							
							after_map_node = createMapNode();
							after_map_node->setPosition(ccp(240-480,180));
							main_node->addChild(after_map_node, kPMS_Z_puzzle_back_side);
							
							if(after_map_node->getTag() == kPMS_MT_loaded)
							{
								after_map_node->setTag(-1);
								
								cachingPuzzleImg2();
//								switchMapNode(after_map_node);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endLoadedMovingMapNode));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else if(after_map_node->getTag() == kPMS_MT_notOpenedPuzzle)
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotOpenPuzzle));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else if(after_map_node->getTag() == kPMS_MT_notClearBeforePuzzle)
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotClearPuzzle));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNode));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
						}
						else
						{
							CCLOG("nothing pre puzzle!!!");
							
							original_mms = kMMS_uiMode;
							
							map_mode_state = kMMS_changeMode;
							is_menu_enable = false;
							
							CCMoveBy* t_move1 = CCMoveBy::create(0.2f, ccp(40,0));
							CCMoveBy* t_move2 = CCMoveBy::create(0.1f, ccp(-40,0));
							CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endTingMapNode));
							CCSequence* t_seq = CCSequence::create(t_move1, t_move2, t_call, NULL);
							map_node->runAction(t_seq);
						}
					}
				}
			}
		}
	}
	else if(map_mode_state == kMMS_frameMode)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				o_it->second = location;
				if(multiTouchData.size() == 1)
				{
					if(location.y > touchStart_p.y + 50.f)
					{
						touchStart_p = location;
						
						int puzzle_cnt = NSDS_GI(kSDS_GI_puzzleListCount_i);
						int found_index = -1;
						for(int i=0;i<puzzle_cnt && found_index == -1;i++)
						{
							if(recent_puzzle_number == NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1))
								found_index = i+1;
						}
						
						if(NSDS_GI(kSDS_GI_puzzleList_int1_version_i, found_index) > NSDS_GI(recent_puzzle_number, kSDS_PZ_version_i))
						{
							CCNode* t_node = CCNode::create();
							t_node->setTag(recent_puzzle_number);
							puzzleAction(t_node);
						}
						else
							startReturnUiMode();
						
						multiTouchData.erase((int)touch);
					}
				}
			}
		}
	}
	else if(map_mode_state == kMMS_notLoadMode)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				o_it->second = location;
				if(multiTouchData.size() == 1)
				{
					if(location.y < touchStart_p.y - 50.f)
					{
						if(getChildByTag(kPMS_MT_loadPuzzleInfo))
							removeChildByTag(kPMS_MT_loadPuzzleInfo);
						removeChildByTag(kPMS_MT_puzzleOpenTitle);
						removeChildByTag(kPMS_MT_ticketCnt);
						removeChildByTag(kPMS_MT_callTicket);
						removeChildByTag(kPMS_MT_buyPuzzle);
						
						touchStart_p = location;
						startChangeFrameMode();
						multiTouchData.erase((int)touch);
					}
					else if(location.x < touchStart_p.x - 50.f) // next
					{
						int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
						bool is_found = false;
						int found_index = -1;
						for(int i=0;i<puzzle_count && !is_found;i++)
						{
							int t_puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
							if(recent_puzzle_number+1 == t_puzzle_no)
							{
								is_found = true;
								found_index = i+1;
								break;
							}
						}
						
						touchStart_p = location;
						
						if(is_found && found_index <= puzzle_count)
						{
							if(getChildByTag(kPMS_MT_loadPuzzleInfo))
								removeChildByTag(kPMS_MT_loadPuzzleInfo);
							removeChildByTag(kPMS_MT_puzzleOpenTitle);
							removeChildByTag(kPMS_MT_ticketCnt);
							removeChildByTag(kPMS_MT_callTicket);
							removeChildByTag(kPMS_MT_buyPuzzle);
							
							recent_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, found_index);
							
							map_mode_state = kMMS_changeMode;
							is_menu_enable = false;
							
							after_map_node = createMapNode();
							after_map_node->setPosition(ccp(240+480,180));
							main_node->addChild(after_map_node, kPMS_Z_puzzle_back_side);
							
							if(after_map_node->getTag() == kPMS_MT_loaded)
							{
								after_map_node->setTag(-1);
								
								cachingPuzzleImg2();
//								switchMapNode(after_map_node);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endLoadedMovingMapNode));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else if(after_map_node->getTag() == kPMS_MT_notOpenedPuzzle)
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotOpenPuzzle));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else if(after_map_node->getTag() == kPMS_MT_notClearBeforePuzzle)
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotClearPuzzle));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNode));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
						}
						else
						{
							CCLOG("nothing next puzzle!!!");
							
							original_mms = kMMS_notLoadMode;
							
							map_mode_state = kMMS_changeMode;
							is_menu_enable = false;
							
							CCMoveBy* t_move1 = CCMoveBy::create(0.2f, ccp(-40,0));
							CCMoveBy* t_move2 = CCMoveBy::create(0.1f, ccp(40,0));
							CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endTingMapNode));
							CCSequence* t_seq = CCSequence::create(t_move1, t_move2, t_call, NULL);
							map_node->runAction(t_seq);
						}
					}
					else if(location.x > touchStart_p.x + 50.f) // pre
					{
						int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
						bool is_found = false;
						int found_index = -1;
						for(int i=0;i<puzzle_count && !is_found;i++)
						{
							int t_puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
							if(recent_puzzle_number-1 == t_puzzle_no)
							{
								is_found = true;
								found_index = i+1;
								break;
							}
						}
						
						touchStart_p = location;
						
						if(is_found)
						{
							if(getChildByTag(kPMS_MT_loadPuzzleInfo))
								removeChildByTag(kPMS_MT_loadPuzzleInfo);
							removeChildByTag(kPMS_MT_puzzleOpenTitle);
							removeChildByTag(kPMS_MT_ticketCnt);
							removeChildByTag(kPMS_MT_callTicket);
							removeChildByTag(kPMS_MT_buyPuzzle);
							
							recent_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, found_index);
							
							map_mode_state = kMMS_changeMode;
							is_menu_enable = false;
							
							after_map_node = createMapNode();
							after_map_node->setPosition(ccp(240-480,180));
							main_node->addChild(after_map_node, kPMS_Z_puzzle_back_side);
							
							if(after_map_node->getTag() == kPMS_MT_loaded)
							{
								after_map_node->setTag(-1);
								
								cachingPuzzleImg2();
//								switchMapNode(after_map_node);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endLoadedMovingMapNode));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else if(after_map_node->getTag() == kPMS_MT_notOpenedPuzzle)
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotOpenPuzzle));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else if(after_map_node->getTag() == kPMS_MT_notClearBeforePuzzle)
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotClearPuzzle));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
							else
							{
								after_map_node->setTag(-1);
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNode));
								CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
								
								map_node->runAction(t_move1);
								after_map_node->runAction(t_seq);
							}
						}
						else
						{
							CCLOG("nothing pre puzzle!!!");
							
							original_mms = kMMS_notLoadMode;
							
							map_mode_state = kMMS_changeMode;
							is_menu_enable = false;
							
							CCMoveBy* t_move1 = CCMoveBy::create(0.2f, ccp(40,0));
							CCMoveBy* t_move2 = CCMoveBy::create(0.1f, ccp(-40,0));
							CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endTingMapNode));
							CCSequence* t_seq = CCSequence::create(t_move1, t_move2, t_call, NULL);
							map_node->runAction(t_seq);
						}
					}
				}
			}
		}
	}
	else
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				o_it->second = location;
				if(multiTouchData.size() == 1)
				{
					this->moveListXY(ccpSub(touch_p, location));
					touch_p = location;
					
					if(map_mode_state == kMMS_firstTouchStage)
					{
						CCPoint sub_point = ccpSub(location, touchStart_p);
						float sub_value = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
						if(sub_value > 15.f)
						{
							resetStagePiece();
							map_mode_state = kMMS_firstTouchDefault;
						}
					}
					else if(((CCMenu*)getChildByTag(kPMS_MT_showui))->isVisible() && is_gesturable_map_mode && location.y < touchStart_p.y - 50.f)
					{
						touchStart_p = location;
						startChangeUiMode();
						return;
					}
					else if(map_node->getScale() <= minimum_scale)
					{
						if(location.x < touchStart_p.x - 50.f)
						{
							((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(false);
							
							int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
							bool is_found = false;
							int found_index = -1;
							for(int i=0;i<puzzle_count && !is_found;i++)
							{
								int t_puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
								if(recent_puzzle_number+1 == t_puzzle_no)
								{
									is_found = true;
									found_index = i+1;
									break;
								}
							}
							
							touchStart_p = location;
							
							if(is_found && found_index <= puzzle_count)
							{
								recent_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, found_index);
								
								if(getChildByTag(kPMS_MT_loadPuzzleInfo))
									removeChildByTag(kPMS_MT_loadPuzzleInfo);
								removeChildByTag(kPMS_MT_puzzleOpenTitle);
								removeChildByTag(kPMS_MT_ticketCnt);
								removeChildByTag(kPMS_MT_callTicket);
								removeChildByTag(kPMS_MT_buyPuzzle);
								
								map_mode_state = kMMS_changeMode;
								is_menu_enable = false;
								
								after_map_node = createMapNode();
								after_map_node->getCamera()->setEyeXYZ(0, 0.f, 2.f);
								after_map_node->setScale(1.f);
								after_map_node->setPosition(ccp(240+480,160));
								main_node->addChild(after_map_node, kPMS_Z_puzzle_back_side);
								
								if(after_map_node->getTag() == kPMS_MT_loaded)
								{
									after_map_node->setTag(-1);
									
									cachingPuzzleImg2();
									//								switchMapNode(after_map_node);
									
									CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
									CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
									CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endLoadedMovingMapNode));
									CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
									
									map_node->runAction(t_move1);
									after_map_node->runAction(t_seq);
								}
								else if(after_map_node->getTag() == kPMS_MT_notOpenedPuzzle)
								{
									after_map_node->setTag(-1);
									
									CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
									CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
									CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotOpenPuzzle));
									CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
									
									map_node->runAction(t_move1);
									after_map_node->runAction(t_seq);
								}
								else if(after_map_node->getTag() == kPMS_MT_notClearBeforePuzzle)
								{
									after_map_node->setTag(-1);
									
									CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
									CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
									CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotClearPuzzle));
									CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
									
									map_node->runAction(t_move1);
									after_map_node->runAction(t_seq);
								}
								else
								{
									after_map_node->setTag(-1);
									
									CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(-480,0));
									CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(-480,0));
									CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNode));
									CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
									
									map_node->runAction(t_move1);
									after_map_node->runAction(t_seq);
								}
							}
							else
							{
								((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
								
								CCLOG("nothing next puzzle!!!");
								
								original_mms = kMMS_firstTouchDefault;
								
								map_mode_state = kMMS_changeMode;
								is_menu_enable = false;
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.2f, ccp(-40,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.1f, ccp(40,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endTingMapNode));
								CCSequence* t_seq = CCSequence::create(t_move1, t_move2, t_call, NULL);
								map_node->runAction(t_seq);
							}
						}
						else if(location.x > touchStart_p.x + 50.f)
						{
							((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(false);
							
							int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
							bool is_found = false;
							int found_index = -1;
							for(int i=0;i<puzzle_count && !is_found;i++)
							{
								int t_puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
								if(recent_puzzle_number-1 == t_puzzle_no)
								{
									is_found = true;
									found_index = i+1;
									break;
								}
							}
							
							touchStart_p = location;
							
							if(is_found)
							{
								recent_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, found_index);
								
								if(getChildByTag(kPMS_MT_loadPuzzleInfo))
									removeChildByTag(kPMS_MT_loadPuzzleInfo);
								removeChildByTag(kPMS_MT_puzzleOpenTitle);
								removeChildByTag(kPMS_MT_ticketCnt);
								removeChildByTag(kPMS_MT_callTicket);
								removeChildByTag(kPMS_MT_buyPuzzle);
								
								map_mode_state = kMMS_changeMode;
								is_menu_enable = false;
								
								after_map_node = createMapNode();
								after_map_node->getCamera()->setEyeXYZ(0, 0.f, 2.f);
								after_map_node->setScale(1.f);
								after_map_node->setPosition(ccp(240-480,160));
								main_node->addChild(after_map_node, kPMS_Z_puzzle_back_side);
								
								if(after_map_node->getTag() == kPMS_MT_loaded)
								{
									after_map_node->setTag(-1);
									
									cachingPuzzleImg2();
									//								switchMapNode(after_map_node);
									
									CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
									CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
									CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endLoadedMovingMapNode));
									CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
									
									map_node->runAction(t_move1);
									after_map_node->runAction(t_seq);
								}
								else if(after_map_node->getTag() == kPMS_MT_notOpenedPuzzle)
								{
									after_map_node->setTag(-1);
									
									CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
									CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
									CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotOpenPuzzle));
									CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
									
									map_node->runAction(t_move1);
									after_map_node->runAction(t_seq);
								}
								else if(after_map_node->getTag() == kPMS_MT_notClearBeforePuzzle)
								{
									after_map_node->setTag(-1);
									
									CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
									CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
									CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNodeNotClearPuzzle));
									CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
									
									map_node->runAction(t_move1);
									after_map_node->runAction(t_seq);
								}
								else
								{
									after_map_node->setTag(-1);
									
									CCMoveBy* t_move1 = CCMoveBy::create(0.4f, ccp(480,0));
									CCMoveBy* t_move2 = CCMoveBy::create(0.5f, ccp(480,0));
									CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endMovingMapNode));
									CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
									
									map_node->runAction(t_move1);
									after_map_node->runAction(t_seq);
								}
							}
							else
							{
								((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
								
								CCLOG("nothing pre puzzle!!!");
								
								original_mms = kMMS_firstTouchDefault;
								
								map_mode_state = kMMS_changeMode;
								is_menu_enable = false;
								
								CCMoveBy* t_move1 = CCMoveBy::create(0.2f, ccp(40,0));
								CCMoveBy* t_move2 = CCMoveBy::create(0.1f, ccp(-40,0));
								CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::endTingMapNode));
								CCSequence* t_seq = CCSequence::create(t_move1, t_move2, t_call, NULL);
								map_node->runAction(t_seq);
							}
						}
					}
				}
				else if(multiTouchData.size() == 2)
				{
					CCPoint sub_point = CCPointZero;
					map<int, CCPoint>::iterator it;
					for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
					{
						sub_point = ccpMult(sub_point, -1);
						sub_point = ccpAdd(sub_point, it->second);
					}
					
					//						float before_scale = map_node->getScale();
					
					float changed_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
					float after_scale = map_node->getScale()*changed_distance/zoom_base_distance;
					if(after_scale > maximum_scale)				after_scale = maximum_scale;
					else if(after_scale < minimum_scale)		after_scale = minimum_scale;
					zoom_base_distance = changed_distance;
					map_node->setScale(after_scale);
					
					if(map_node->getPositionX() < 240-(map_node->getScale()-1.f)*240.f)
						map_node->setPositionX(240-(map_node->getScale()-1.f)*240.f);
					else if(map_node->getPositionX() > 240+(map_node->getScale()-1.f)*240.f)
						map_node->setPositionX(240+(map_node->getScale()-1.f)*240.f);
					
					if(map_node->getPositionY() < 160-(map_node->getScale()-1.f)*160.f)
						map_node->setPositionY(160-(map_node->getScale()-1.f)*160.f);
					else if(map_node->getPositionY() > 160+(map_node->getScale()-1.f)*160.f)
						map_node->setPositionY(160+(map_node->getScale()-1.f)*160.f);
				}
			}
		}
	}
}

void PuzzleMapScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if(map_mode_state == kMMS_changeMode || map_mode_state == kMMS_loadChangingMode)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				multiTouchData.erase((int)touch);
			}
		}
	}
	else if(map_mode_state == kMMS_uiMode)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				multiTouchData.erase((int)touch);
			}
		}
		
		if(is_menu_enable)
			startChangeMapMode();
	}
	else if(map_mode_state == kMMS_frameMode)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				multiTouchData.erase((int)touch);
			}
		}
	}
	else if(map_mode_state == kMMS_notLoadMode)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				multiTouchData.erase((int)touch);
			}
		}
		
		if(is_menu_enable)
			startChangeMapMode();
	}
	else
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
			location = ccpSub(location, myDSH->ui_touch_convert);
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				multiTouchData.erase((int)touch);
				
				if(multiTouchData.size() == 0)
				{
					timeval time;
					gettimeofday(&time, NULL);
					long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
					CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
					
					float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
					if(isAnimated == false && fabsf(spd_value) > 2)
					{
						moveSpeed_p = _spd;
						this->schedule(schedule_selector(PuzzleMapScene::moveAnimation));
					}
					
					if(map_mode_state == kMMS_firstTouchStage || map_mode_state == kMMS_firstTouchDefault)
					{
						if(touched_stage_number != 0)
						{
							StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(touched_stage_number);
							if(t_sp)
							{
								t_sp->touchEnded(touch, pEvent);
								t_sp->setTouchCancel();
							}
						}
					}
					
					touched_stage_number = 0;
					map_mode_state = kMMS_default;
				}
				else if(multiTouchData.size() == 1)
				{
					map_mode_state = kMMS_firstTouchDefault;
					if(map_node->getScale() == 1.f)
					{
						is_gesturable_map_mode = true;
					}
				}
			}
		}
	}
}

void PuzzleMapScene::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	ccTouchesEnded(pTouches, pEvent);
}

void PuzzleMapScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, kCCMenuHandlerPriority+1);
}

#define PMS_SCROLL_SPEED_MAX_BASE	20
#define PMS_SCROLL_SPEED_DECEASE_BASE	0.2f

void PuzzleMapScene::moveListXY(CCPoint t_p)
{
	if(t_p.x > PMS_SCROLL_SPEED_MAX_BASE/map_node->getScale())		t_p.x = PMS_SCROLL_SPEED_MAX_BASE/map_node->getScale();
	if(t_p.x < -PMS_SCROLL_SPEED_MAX_BASE/map_node->getScale())		t_p.x = -PMS_SCROLL_SPEED_MAX_BASE/map_node->getScale();
	
	if(t_p.y > PMS_SCROLL_SPEED_MAX_BASE/map_node->getScale())		t_p.y = PMS_SCROLL_SPEED_MAX_BASE/map_node->getScale();
	if(t_p.y < -PMS_SCROLL_SPEED_MAX_BASE/map_node->getScale())		t_p.y = -PMS_SCROLL_SPEED_MAX_BASE/map_node->getScale();
	
	CCPoint a_p = ccpSub(map_node->getPosition(), t_p);
	
	//	if(map_node->getScale() <= maximum_scale)
	//		a_p.x = (480.f*maximum_scale-480.f*map_node->getScale())/2.f;
	//	else
	//	{
	if(a_p.x < 240-(map_node->getScale()-1.f)*240.f)
		a_p.x = 240-(map_node->getScale()-1.f)*240.f;
	else if(a_p.x > 240+(map_node->getScale()-1.f)*240.f)
		a_p.x = 240+(map_node->getScale()-1.f)*240.f;
	//	}
	
	
	if(a_p.y < 160-(map_node->getScale()-1.f)*160.f)
		a_p.y = 160-(map_node->getScale()-1.f)*160.f;
	if(a_p.y > 160+(map_node->getScale()-1.f)*160.f)
		a_p.y = 160+(map_node->getScale()-1.f)*160.f;
	
	map_node->setPosition(a_p);
}

void PuzzleMapScene::moveAnimation()
{
	isAnimated = true;
	
	if(moveSpeed_p.x >= PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale())			moveSpeed_p.x -= PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale();
	else if(moveSpeed_p.x <= -PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale())	moveSpeed_p.x += PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale();
	else							moveSpeed_p.x = 0;
	
	if(moveSpeed_p.y >= PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale())			moveSpeed_p.y -= PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale();
	else if(moveSpeed_p.y <= PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale())	moveSpeed_p.y += PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale();
	else							moveSpeed_p.y = 0;
	
	if((moveSpeed_p.x < PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale() && moveSpeed_p.x > -PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale() &&
		moveSpeed_p.y < PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale() && moveSpeed_p.y > -PMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale()) || isAnimated == false)
	{
		this->unschedule(schedule_selector(PuzzleMapScene::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}

void PuzzleMapScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void PuzzleMapScene::keyBackClicked()
{
	
	CommonButton::callBackKey();
	//AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(PuzzleMapScene::alertAction));
}

void PuzzleMapScene::visit()
{
	glEnable(GL_SCISSOR_TEST);

	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();

	float x = 0;
	float y = 0;
	float w = frame_size.width;
	float h = frame_size.height;

	glScissor(x,y,w,h);

	CCLayer::visit();

	glDisable(GL_SCISSOR_TEST);
}

void PuzzleMapScene::endMovingMapNode()
{
	if(!is_from_call_map_mode)
		map_mode_state = kMMS_notLoadMode;
	else
		map_mode_state = kMMS_firstTouchDefault;
	map_node->removeFromParent();
	map_node = after_map_node;
	after_map_node = NULL;

	CCSprite* n_load = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 70, 50));
	CCLabelTTF* n_label = CCLabelTTF::create("퍼즐 정보 로드 하기", mySGD->getFont().c_str(), 18);
	n_label->setColor(ccBLACK);
	n_label->setPosition(ccp(35,25));
	n_load->addChild(n_label);

	CCSprite* s_load = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 70, 50));
	s_load->setColor(ccGRAY);
	CCLabelTTF* s_label = CCLabelTTF::create("퍼즐 정보 로드 하기", mySGD->getFont().c_str(), 18);
	s_label->setColor(ccBLACK);
	s_label->setPosition(ccp(35,25));
	s_load->addChild(s_label);

	CCMenuItem* load_item = CCMenuItemSprite::create(n_load, s_load, this, menu_selector(PuzzleMapScene::loadPuzzleInfo));

	CCMenu* load_menu = CCMenu::createWithItem(load_item);
	load_menu->setPosition(ccp(240,160));
	addChild(load_menu, kPMS_Z_popup, kPMS_MT_loadPuzzleInfo);
	
	if(map_node->getPositionY() < 165.f)
		((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);

	is_menu_enable = true;
}

void PuzzleMapScene::loadPuzzleInfo( CCObject* sender )
{
	map_mode_state = kMMS_changeMode;
	removeChildByTag(kPMS_MT_loadPuzzleInfo);

	StageListDown* t_sld = StageListDown::create(this, callfunc_selector(PuzzleMapScene::endLoadPuzzleInfo), recent_puzzle_number);
	addChild(t_sld, kPMS_Z_popup);
}

void PuzzleMapScene::endMovingMapNodeNotOpenPuzzle()
{
	if(!is_from_call_map_mode)
		map_mode_state = kMMS_notLoadMode;
	else
		map_mode_state = kMMS_firstTouchDefault;
	map_node->removeFromParent();
	map_node = after_map_node;
	after_map_node = NULL;
	
	CCSprite* puzzle_open_title = CCSprite::create("puzzle_open_title.png");
	puzzle_open_title->setPosition(ccp(240,180));
	addChild(puzzle_open_title, kPMS_Z_popup, kPMS_MT_puzzleOpenTitle);
	
	CCLabelTTF* ticket_cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt),
																				 NSDS_GI(recent_puzzle_number, kSDS_PZ_ticket_i))->getCString(), mySGD->getFont().c_str(), 18);
	ticket_cnt_label->setPosition(ccp(270,150));
	addChild(ticket_cnt_label, kPMS_Z_popup, kPMS_MT_ticketCnt);
	
	
	CCSprite* n_buy = CCSprite::create("puzzle_open_buy.png");
	CCSprite* n_buy_type = CCSprite::create("price_ruby_img.png");
	n_buy_type->setPosition(ccp(n_buy->getContentSize().width/2.f-20, n_buy->getContentSize().height/2.f));
	n_buy->addChild(n_buy_type);
	CCLabelTTF* n_price = CCLabelTTF::create(CCString::createWithFormat("젬 %d", NSDS_GI(recent_puzzle_number, kSDS_PZ_point_i))->getCString(), mySGD->getFont().c_str(), 17);
	n_price->setPosition(ccp(n_buy->getContentSize().width/2.f+10, n_buy->getContentSize().height/2.f));
	n_buy->addChild(n_price);
	
	CCSprite* s_buy = CCSprite::create("puzzle_open_buy.png");
	s_buy->setColor(ccGRAY);
	CCSprite* s_buy_type = CCSprite::create("price_ruby_img.png");
	s_buy_type->setPosition(ccp(s_buy->getContentSize().width/2.f-20, s_buy->getContentSize().height/2.f));
	s_buy->addChild(s_buy_type);
	CCLabelTTF* s_price = CCLabelTTF::create(CCString::createWithFormat("젬 %d", NSDS_GI(recent_puzzle_number, kSDS_PZ_point_i))->getCString(), mySGD->getFont().c_str(), 17);
	s_price->setPosition(ccp(s_buy->getContentSize().width/2.f+10, s_buy->getContentSize().height/2.f));
	s_buy->addChild(s_price);
	
	CCMenuItem* buy_item = CCMenuItemSprite::create(n_buy, s_buy, this, menu_selector(PuzzleMapScene::notOpenPuzzleAction));
	buy_item->setTag(kPMS_MT_buyPuzzle);
	
	CCMenu* buy_menu = CCMenu::createWithItem(buy_item);
	buy_menu->setPosition(ccp(170,80));
	addChild(buy_menu, kPMS_Z_popup, kPMS_MT_buyPuzzle);
	
	
	CCSprite* n_ticket = CCSprite::create("puzzle_open_ticket.png");
	CCSprite* s_ticket = CCSprite::create("puzzle_open_ticket.png");
	s_ticket->setColor(ccGRAY);
	
	CCMenuItem* ticket_item = CCMenuItemSprite::create(n_ticket, s_ticket, this, menu_selector(PuzzleMapScene::notOpenPuzzleAction));
	ticket_item->setTag(kPMS_MT_callTicket);
	
	CCMenu* ticket_menu = CCMenu::createWithItem(ticket_item);
	ticket_menu->setPosition(ccp(310,80));
	addChild(ticket_menu, kPMS_Z_popup, kPMS_MT_callTicket);
	
	if(map_node->getPositionY() < 165.f)
		((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
	
	
	is_menu_enable = true;
}

void PuzzleMapScene::openPuzzleAction(int t_puzzle_number)
{
	StageListDown* t_sld = StageListDown::create(this, callfunc_selector(PuzzleMapScene::endLoadPuzzleInfo), t_puzzle_number);
	addChild(t_sld, kPMS_Z_popup);
}

void PuzzleMapScene::notOpenPuzzleAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kPMS_MT_buyPuzzle)
	{
		setTouchEnabled(false);
		
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));
		
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
		
		CCLabelTTF* title_label = CCLabelTTF::create("지금 열기", mySGD->getFont().c_str(), 20);
		title_label->setPosition(ccp(0, 102));
		t_container->addChild(title_label);
		
		CCLabelTTF* content_label = CCLabelTTF::create(CCString::createWithFormat("%d Ruby 로 오픈", NSDS_GI(recent_puzzle_number, kSDS_PZ_point_i))->getCString(), mySGD->getFont().c_str(), 18);
		content_label->setPosition(CCPointZero);
		t_container->addChild(content_label);
		
		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
		{
			setTouchEnabled(true);
			is_menu_enable = true;
			t_popup->removeFromParent();
		});
		
		CCMenuLambda* close_menu = CCMenuLambda::createWithItem(close_item);
		close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
		close_menu->setPosition(ccp(92,105));
		t_container->addChild(close_menu);
		
		if(mySGD->getGoodsValue(kGoodsType_ruby) >= NSDS_GI(recent_puzzle_number, kSDS_PZ_point_i))
		{
			CCSprite* n_buy = CCSprite::create("popup2_buy.png");
			CCSprite* s_buy = CCSprite::create("popup2_buy.png");
			s_buy->setColor(ccGRAY);
			
			CCMenuItemSpriteLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender){
//				mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - NSDS_GI(recent_puzzle_number, kSDS_PZ_point_i));
				
				int open_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, mySGD->getOpenPuzzleCount()+1);
				PuzzleHistory t_history = mySGD->getPuzzleHistory(open_puzzle_number);
				t_history.is_open = true;
				t_history.open_type = "젬소모";
				mySGD->setPuzzleHistory(t_history, nullptr);
				
				map_mode_state = kMMS_changeMode;
				removeChildByTag(kPMS_MT_buyPuzzle);
				removeChildByTag(kPMS_MT_callTicket);
				removeChildByTag(kPMS_MT_puzzleOpenTitle);
				removeChildByTag(kPMS_MT_ticketCnt);
				
//				vector<SaveUserData_Key> save_userdata_list;
//				
//				save_userdata_list.push_back(kSaveUserData_Key_star);
//				
//				myDSH->saveUserData(save_userdata_list, nullptr);
				
				
				CCNode* open_puzzle_container = CCNode::create();
				t_popup->setContainerNode(open_puzzle_container);
				
				CCScale9Sprite* open_puzzle_case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
				open_puzzle_case_back->setPosition(CCPointZero);
				open_puzzle_container->addChild(open_puzzle_case_back);
				
				open_puzzle_case_back->setContentSize(CCSizeMake(230, 250));
				
				CCScale9Sprite* open_puzzle_content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
				open_puzzle_content_back->setPosition(ccp(0,2));
				open_puzzle_container->addChild(open_puzzle_content_back);
				
				open_puzzle_content_back->setContentSize(CCSizeMake(202, 146));
				
				CCLabelTTF* open_puzzle_title_label = CCLabelTTF::create("퍼즐 오픈", mySGD->getFont().c_str(), 20);
				open_puzzle_title_label->setPosition(ccp(0, 102));
				open_puzzle_container->addChild(open_puzzle_title_label);
				
				CCLabelTTF* open_puzzle_content_label = CCLabelTTF::create("새로운 퍼즐이\n오픈 되었습니다.", mySGD->getFont().c_str(), 18);
				open_puzzle_content_label->setPosition(CCPointZero);
				open_puzzle_container->addChild(open_puzzle_content_label);
				
				CCLabelTTF* loading_puzzle_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 12);
				loading_puzzle_label->setPosition(ccp(0,-95));
				open_puzzle_container->addChild(loading_puzzle_label);
				
				CCSprite* n_op_ok = CCSprite::create("popup2_ok.png");
				CCSprite* s_op_ok = CCSprite::create("popup2_ok.png");
				s_op_ok->setColor(ccGRAY);
				
				CCMenuItemSpriteLambda* op_ok_item = CCMenuItemSpriteLambda::create(n_op_ok, s_op_ok, [=](CCObject* sender){
					setTouchEnabled(true);
					is_menu_enable = true;
					t_popup->removeFromParent();
				});
				
				CCMenuLambda* op_ok_menu = CCMenuLambda::createWithItem(op_ok_item);
				op_ok_menu->setTouchPriority(t_popup->getTouchPriority()-1);
				op_ok_menu->setVisible(false);
				op_ok_menu->setPosition(ccp(0,-95));
				open_puzzle_container->addChild(op_ok_menu);
				
				target_visibleOn = op_ok_menu;
				
				StageListDown* t_sld = StageListDown::create(this, callfunc_selector(PuzzleMapScene::endLoadPuzzleInfo), recent_puzzle_number);
				addChild(t_sld, kPMS_Z_popup);
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
	else if(tag == kPMS_MT_callTicket)
	{
		setTouchEnabled(false);
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));
		
		TicketRequestContent* t_container = TicketRequestContent::create(t_popup->getTouchPriority(), recent_puzzle_number);
		t_popup->setContainerNode(t_container);
		addChild(t_popup);
		
		t_container->setRemoveAction([=](){
			setTouchEnabled(true);
			is_menu_enable = true;
			t_popup->removeFromParent();
		});
	}
	
//	map_mode_state = kMMS_changeMode;
//	removeChildByTag(kPMS_MT_loadPuzzleInfo);
//	
//	StageListDown* t_sld = StageListDown::create(this, callfunc_selector(PuzzleMapScene::endLoadPuzzleInfo), recent_puzzle_number);
//	addChild(t_sld, kPMS_Z_popup);
}

void PuzzleMapScene::endMovingMapNodeNotClearPuzzle()
{
	if(!is_from_call_map_mode)
		map_mode_state = kMMS_notLoadMode;
	else
		map_mode_state = kMMS_firstTouchDefault;
	map_node->removeFromParent();
	map_node = after_map_node;
	after_map_node = NULL;
	
	if(map_node->getPositionY() < 165.f)
		((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
	
	is_menu_enable = true;
}
void PuzzleMapScene::notClearPuzzleAction(CCObject* sender)
{
	
}

void PuzzleMapScene::endLoadPuzzleVisibleOn()
{
	if(target_visibleOn)
	{
		((CCNode*)target_visibleOn)->setVisible(true);
		target_visibleOn = NULL;
	}
}

void PuzzleMapScene::endLoadPuzzleInfo()
{
	map_node->removeChildByTag(99999);
	map_node->removeChildByTag(99998);
	map_node->removeChildByTag(99997);

	if(!is_from_call_map_mode)
		map_mode_state = kMMS_notLoadMode;
	else
		map_mode_state = kMMS_firstTouchDefault;

	cachingPuzzleImg2();
	//		switchMapNode(map_node);
	//		endSwitchMapNode();
}

void PuzzleMapScene::creatingPuzzle()
{
	CreatingPuzzleInfo t_info = loaded_imgs.front();
	loaded_imgs.pop_front();

	//		if(recent_puzzle_number != t_info.puzzle_number)
	//			return;
	//		if(t_info.piece_key == CCString::createWithFormat("puzzle%d_top", recent_puzzle_number)->getCString())
	//			return;
	//		else if(t_info.piece_key == CCString::createWithFormat("puzzle%d_left", recent_puzzle_number)->getCString())
	//			return;
	//		else if(t_info.piece_key == CCString::createWithFormat("puzzle%d_right", recent_puzzle_number)->getCString())
	//			return;
	//		else if(t_info.piece_key.find("thumbnail") != -1)
	//			return;
	if(t_info.piece_key == CCString::createWithFormat("puzzle%d_bottom", recent_puzzle_number)->getCString())
	{
		CCNode* target_node;
		if(after_map_node)
			target_node = after_map_node;
		else
			target_node = map_node;

		if(target_node->getChildByTag(99999))
			target_node->removeChildByTag(99999);
		if(target_node->getChildByTag(99998))
			target_node->removeChildByTag(99998);
		if(target_node->getChildByTag(99997))
			target_node->removeChildByTag(99997);

		CCSprite* map_back_center = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_center.png", recent_puzzle_number)->getCString());
		map_back_center->setPosition(CCPointZero);
		target_node->addChild(map_back_center, kPMS_Z_puzzle_back_side);

		//	CCSize center_size = CCSizeMake(520.f, 340.f);
		CCPoint original_point = ccp(-260.f, -170.f);

		PuzzleImage* left_img = PuzzleCache::getInstance()->getImage(CCString::createWithFormat("puzzle%d_left", recent_puzzle_number)->getCString());
		CCSprite* map_back_left = CCSprite::createWithTexture(left_img->makeTexture());
		map_back_left->setPosition(ccpAdd(original_point, left_img->getCutPoint()));
		target_node->addChild(map_back_left, kPMS_Z_puzzle_back);

		CCSprite* shadow_left = CCSprite::create("puzzle_shadow_1_left.png");
		shadow_left->setPosition(map_back_left->getPosition());
		target_node->addChild(shadow_left, kPMS_Z_puzzle_shadow);


		PuzzleImage* right_img = PuzzleCache::getInstance()->getImage(CCString::createWithFormat("puzzle%d_right", recent_puzzle_number)->getCString());
		CCSprite* map_back_right = CCSprite::createWithTexture(right_img->makeTexture());
		map_back_right->setPosition(ccpAdd(original_point, right_img->getCutPoint()));
		target_node->addChild(map_back_right, kPMS_Z_puzzle_back);

		CCSprite* shadow_right = CCSprite::create("puzzle_shadow_1_right.png");
		shadow_right->setPosition(map_back_right->getPosition());
		target_node->addChild(shadow_right, kPMS_Z_puzzle_shadow);


		PuzzleImage* top_img = PuzzleCache::getInstance()->getImage(CCString::createWithFormat("puzzle%d_top", recent_puzzle_number)->getCString());
		CCSprite* map_back_top = CCSprite::createWithTexture(top_img->makeTexture());
		map_back_top->setPosition(ccpAdd(original_point, top_img->getCutPoint()));
		target_node->addChild(map_back_top, kPMS_Z_puzzle_back);

		CCSprite* shadow_top = CCSprite::create("puzzle_shadow_1_top.png");
		shadow_top->setPosition(map_back_top->getPosition());
		target_node->addChild(shadow_top, kPMS_Z_puzzle_shadow);


		PuzzleImage* bottom_img = PuzzleCache::getInstance()->getImage(CCString::createWithFormat("puzzle%d_bottom", recent_puzzle_number)->getCString());
		CCSprite* map_back_bottom = CCSprite::createWithTexture(bottom_img->makeTexture());
		map_back_bottom->setPosition(ccpAdd(original_point, bottom_img->getCutPoint()));
		target_node->addChild(map_back_bottom, kPMS_Z_puzzle_back);

		CCSprite* shadow_bottom = CCSprite::create("puzzle_shadow_1_bottom.png");
		shadow_bottom->setPosition(map_back_bottom->getPosition());
		target_node->addChild(shadow_bottom, kPMS_Z_puzzle_shadow);

		shadow_batchnode = CCSpriteBatchNode::create("puzzle_shadow_1_piece.png");
		shadow_batchnode->setPosition(CCPointZero);
		target_node->addChild(shadow_batchnode, kPMS_Z_puzzle_shadow);
	}
	else
	{
		CCNode* target_node;
		if(after_map_node)
			target_node = after_map_node;
		else
			target_node = map_node;

		int stage_count = NSDS_GI(recent_puzzle_number, kSDS_PZ_stageCount_i);
		int start_stage = NSDS_GI(recent_puzzle_number, kSDS_PZ_startStage_i);

		int stage_number = 0;

		int piece_no = t_info.piece_number;

		for(int j=start_stage;j<start_stage+stage_count && stage_number == 0;j++)
		{
			if(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, j) == piece_no)
			{
				stage_number = j;
			}
		}

		CCRect stage_rect = CCRectMake(-30.f, -30.f, 60.f, 60.f);
		CCPoint original_point = ccp(-260.f, -170.f);

		CCPoint sp_position = ccpAdd(original_point, t_info.img_cut_point);
		string piece_type = "w";
		if((piece_no-1)/6%2 == 0)
		{
			if(piece_no%2 == 0)
				piece_type = "w";
			else
				piece_type = "h";
		}
		else
		{
			if(piece_no%2 == 0)
				piece_type = "h";
			else
				piece_type = "w";
		}


		if(stage_number != 0)
		{
			int stage_level = SDS_GI(kSDF_puzzleInfo, recent_puzzle_number, CCSTR_CWF("stage%d_level", stage_number)->getCString());
			if(stage_number == 1 || mySGD->isClearPiece(stage_number) ||
			   (NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
				(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 || mySGD->isClearPiece(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))))
			{
				bool is_found = false;
				int found_number = 0;
				bool is_have_card[3] = {0,};
				for(int k=3;k>=1;k--)
				{
					int card_number = SDS_GI(kSDF_stageInfo, stage_number, CCSTR_CWF("level%d_card", k)->getCString());
					if(mySGD->isHasGottenCards(card_number) > 0)
					{
						if(!is_found)
						{
							is_found = true;
							found_number = k;
						}
						is_have_card[k-1] = true;
					}
				}
				
				if(found_number == 3)
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_piece%d", recent_puzzle_number, piece_no)->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, false, false, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		target_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														target_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
					
					t_sp->setChangable(CCSTR_CWF("puzzle%d_thumbnail%d", recent_puzzle_number, piece_no)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
					t_sp->setPuzzleMode(my_puzzle_mode);
					t_sp->shadow_node = addShadow(stage_number, piece_type.c_str(), t_sp->getPosition());
				}
				else if(found_number == 2)
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_piece%d", recent_puzzle_number, piece_no)->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, false, true, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		target_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														target_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
					
					t_sp->setChangable(CCSTR_CWF("puzzle%d_thumbnail%d", recent_puzzle_number, piece_no)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
					t_sp->setPuzzleMode(my_puzzle_mode);
					t_sp->shadow_node = addShadow(stage_number, piece_type.c_str(), t_sp->getPosition());
				}
				else if(found_number == 1)
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_piece%d", recent_puzzle_number, piece_no)->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, true, true, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		target_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														target_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
					
					t_sp->setChangable(CCSTR_CWF("puzzle%d_thumbnail%d", recent_puzzle_number, piece_no)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
					t_sp->setPuzzleMode(my_puzzle_mode);
					t_sp->shadow_node = addShadow(stage_number, piece_type.c_str(), t_sp->getPosition());
				}
				else
				{
					is_puzzle_clear = false;
					StagePiece* t_sp = StagePiece::create("test_puzzle_empty.png",
														  stage_number, stage_level, sp_position, stage_rect, false, false, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		target_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														target_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
				}
			}
			else
			{
				is_puzzle_clear = false;
				if(mySGD->isClearPiece(NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("piece_buy_%s.png", piece_type.c_str())->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, false, false, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		target_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														target_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
				}
				else
				{
					StagePiece* t_sp = StagePiece::create(CCSTR_CWF("piece_lock_%s.png", piece_type.c_str())->getCString(),
														  stage_number, stage_level, sp_position, stage_rect, false, false, piece_type.c_str(),
														  this, menu_selector(PuzzleMapScene::stageAction));
					t_sp->mySetTouchEnable(false);
					
					if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		target_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
					else														target_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
				}
			}
		}
		else
		{
			StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_piece%d", recent_puzzle_number, piece_no)->getCString(),
				stage_number, 0, sp_position, stage_rect, false, false, piece_type.c_str(),
				this, menu_selector(PuzzleMapScene::stageAction));
			t_sp->mySetTouchEnable(false);

			target_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber());

			t_sp->setBackPuzzle();
			t_sp->shadow_node = addShadow(stage_number, piece_type.c_str(), t_sp->getPosition());
		}
	}

	if(t_info.piece_number == 24)
	{
		if(!is_from_call_map_mode)
		{
			if(map_mode_state == kMMS_notLoadMode)
				map_mode_state = kMMS_uiMode;
			else if(map_mode_state == kMMS_changeMode)
				map_mode_state = kMMS_loadChangingMode;
			else
				CCLOG("map_mode_state : %d", map_mode_state);
		}
		else
		{
			map_mode_state = kMMS_firstTouchDefault;
			((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
		}
		
		endLoadPuzzleVisibleOn();
		
		if(is_puzzle_clear)
		{
			PuzzleHistory t_history = mySGD->getPuzzleHistory(recent_puzzle_number);
			t_history.is_clear = true;
			mySGD->setPuzzleHistory(t_history, nullptr);
		}
	}

	if(loaded_imgs.empty())
	{
		unschedule(schedule_selector(PuzzleMapScene::creatingPuzzle));
		is_creating_puzzle = false;
	}
}

void PuzzleMapScene::cachingPuzzleImg2()
{
	unschedule(schedule_selector(PuzzleMapScene::creatingPuzzle));
	PuzzleCache::getInstance()->cancelLoadingImage(0);
	is_puzzle_clear = true;
	is_creating_puzzle = false;
	loaded_imgs.clear();
	ing_caching_cnt = 0;
	total_caching_cnt = 0;

	stage_count = NSDS_GI(recent_puzzle_number, kSDS_PZ_stageCount_i);
	start_stage_number = NSDS_GI(recent_puzzle_number, kSDS_PZ_startStage_i);

	PuzzleCache::getInstance()->loadImageWithCallback(recent_puzzle_number, [&](PuzzleImage* t_img)
	{
		if(recent_puzzle_number != t_img->getPuzzleNo())
			return;
		if(t_img->getPuzzleKey() == CCString::createWithFormat("puzzle%d_top", recent_puzzle_number)->getCString())
			return;
		else if(t_img->getPuzzleKey() == CCString::createWithFormat("puzzle%d_left", recent_puzzle_number)->getCString())
			return;
		else if(t_img->getPuzzleKey() == CCString::createWithFormat("puzzle%d_right", recent_puzzle_number)->getCString())
			return;
		else if(t_img->getPuzzleKey().find("thumbnail") != -1)
		{
			//																	CCLOG("key : %s, find : %ld", t_img->getPuzzleKey().c_str(), t_img->getPuzzleKey().find("piece"));
			return;
		}
		else if(t_img->getPuzzleKey() == CCString::createWithFormat("puzzle%d_bottom", recent_puzzle_number)->getCString())
		{
			CreatingPuzzleInfo t_info;
			t_info.puzzle_number = t_img->getPuzzleNo();
			t_info.img_cut_point = t_img->getCutPoint();
			t_info.piece_key = t_img->getPuzzleKey().c_str();
			t_info.piece_number = t_img->getPieceNo();

			loaded_imgs.push_back(t_info);

			if(!is_creating_puzzle)
			{
				is_creating_puzzle = true;
				schedule(schedule_selector(PuzzleMapScene::creatingPuzzle));
			}
		}
		else
		{
			CreatingPuzzleInfo t_info;
			t_info.puzzle_number = t_img->getPuzzleNo();
			t_info.img_cut_point = t_img->getCutPoint();
			t_info.piece_key = t_img->getPuzzleKey().c_str();
			t_info.piece_number = t_img->getPieceNo();

			loaded_imgs.push_back(t_info);

			if(!is_creating_puzzle)
			{
				is_creating_puzzle = true;
				schedule(schedule_selector(PuzzleMapScene::creatingPuzzle));
			}
		}
	});
}

void PuzzleMapScene::endLoadedMovingMapNode()
{
	map_node->removeFromParent();
	map_node = after_map_node;
	after_map_node = NULL;

	endSwitchMapNode();
}

void PuzzleMapScene::endTingMapNode()
{
	map_mode_state = original_mms;
	is_menu_enable = true;
}

void PuzzleMapScene::endSwitchMapNode()
{
	if(!is_from_call_map_mode)
	{
		if(map_mode_state == kMMS_changeMode)
			map_mode_state = kMMS_notLoadMode;
		else if(map_mode_state == kMMS_loadChangingMode)
			map_mode_state = kMMS_uiMode;
	}
	else
	{
		map_mode_state = kMMS_firstTouchDefault;
	}
	is_menu_enable = true;
}

void PuzzleMapScene::endDirectChangePuzzle()
{
	if(map_mode_state == kMMS_changeMode)
		map_mode_state = kMMS_notLoadMode;
	else if(map_mode_state == kMMS_loadChangingMode)
		map_mode_state = kMMS_uiMode;
	is_menu_enable = true;
}

CCNode* PuzzleMapScene::createMapNode()
{
	CCNode* t_node = CCNode::create();

	int puzzle_cnt = NSDS_GI(kSDS_GI_puzzleListCount_i);
	int found_index = -1;
	for(int i=0;i<puzzle_cnt && found_index == -1;i++)
	{
		if(recent_puzzle_number == NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1))
			found_index = i+1;
	}
	
	if(recent_puzzle_number == 1)
	{
		if(NSDS_GI(kSDS_GI_puzzleList_int1_version_i, found_index) > NSDS_GI(recent_puzzle_number, kSDS_PZ_version_i))
			t_node->setTag(kPMS_MT_notLoaded);
		else
			t_node->setTag(kPMS_MT_loaded);
	}
	else
	{
		if(recent_puzzle_number > mySGD->getOpenPuzzleCount())
		{
			if(mySGD->getPuzzleHistory(recent_puzzle_number-1).is_clear.getV())
			{
				if(NSDS_GI(recent_puzzle_number, kSDS_PZ_point_i) == 0 || NSDS_GI(recent_puzzle_number, kSDS_PZ_ticket_i) == 0)
				{
					if(NSDS_GI(kSDS_GI_puzzleList_int1_version_i, found_index) > NSDS_GI(recent_puzzle_number, kSDS_PZ_version_i))
						t_node->setTag(kPMS_MT_notLoaded);
					else
						t_node->setTag(kPMS_MT_loaded);
				}
				else
					t_node->setTag(kPMS_MT_notOpenedPuzzle);
			}
			else
				t_node->setTag(kPMS_MT_notClearBeforePuzzle);
		}
		else
		{
			if(NSDS_GI(kSDS_GI_puzzleList_int1_version_i, found_index) > NSDS_GI(recent_puzzle_number, kSDS_PZ_version_i))
				t_node->setTag(kPMS_MT_notLoaded);
			else
				t_node->setTag(kPMS_MT_loaded);
		}
	}
	
	CCSprite* t_back;
	CCLabelTTF* t_loading;
	if(t_node->getTag() == kPMS_MT_loaded || t_node->getTag() == kPMS_MT_notLoaded)
	{
		t_back = CCSprite::create("whitePaper.png");
		t_back->setColor(ccGRAY);
		t_back->setScaleX(520.f/480.f);
		t_back->setScaleY(340.f/320.f);
		t_loading = CCLabelTTF::create("LOADING...", mySGD->getFont().c_str(), 40);
		t_node->addChild(t_back, 0, 99999);
		t_node->addChild(t_loading, 0, 99997);
	}
	else if(t_node->getTag() == kPMS_MT_notClearBeforePuzzle)
	{
		t_back = CCSprite::create("puzzle_lock.png");
		t_loading = CCLabelTTF::create("이전 퍼즐을 클리어 하세요.", mySGD->getFont().c_str(), 30);
		t_node->addChild(t_back, 0, 99999);
		t_node->addChild(t_loading, 0, 99997);
	}
	else if(t_node->getTag() == kPMS_MT_notOpenedPuzzle)
	{
		t_back = CCSprite::create("puzzle_open_back.png");
		t_loading = CCLabelTTF::create("", mySGD->getFont().c_str(), 40);
		t_node->addChild(t_back, 0, 99999);
		t_node->addChild(t_loading, 0, 99997);
	}
	t_back->setPosition(CCPointZero);
	
	((CCLabelTTF*)main_node->getChildByTag(kPMS_MT_titleBox)->getChildByTag(1))->setString(NSDS_GS(kSDS_GI_puzzleList_int1_title_s, found_index).c_str());
	
	CCLabelTTF* t_title = CCLabelTTF::create(NSDS_GS(kSDS_GI_puzzleList_int1_title_s, found_index).c_str(), mySGD->getFont().c_str(), 23);
	t_title->setPosition(ccp(0,100));
	t_node->addChild(t_title, 0, 99998);
	
	t_loading->setPosition(CCPointZero);

	t_node->getCamera()->setEyeXYZ(0, -1.f, 2.f);
	t_node->setScale(0.53f);

	return t_node;
}

void PuzzleMapScene::startPuzzleModeChange( PuzzleMode t_mode )
{
	before_map_mode_state = map_mode_state;
	map_mode_state = kMMS_changeMode;
	my_puzzle_mode = t_mode;
	myDSH->setIntegerForKey(kDSH_Key_puzzleMode, my_puzzle_mode);
	ing_check_puzzle = start_stage_number;
	schedule(schedule_selector(PuzzleMapScene::ingPuzzleModeChange), 2.f/60.f);
}

void PuzzleMapScene::ingPuzzleModeChange()
{
	bool is_found = false;
	CCNode* found_node;

	while(!is_found && ing_check_puzzle < start_stage_number+stage_count)
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(ing_check_puzzle);
		if(!t_sp)	break;
		if(t_sp->isChangable())
		{
			t_sp->shadow_node->stopAllActions();
			t_sp->shadow_node->setOpacity(0);
			found_node = t_sp;
			is_found = true;
		}
		ing_check_puzzle++;
	}

	if(is_found)
	{
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.2f, 1.f, 0, 0, 90, 0, 0);
		CCCallFuncO* t_call1 = CCCallFuncO::create(this, callfuncO_selector(PuzzleMapScene::changePiece), found_node);
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.2f, 1.f, 0, -90, 90, 0, 0);
		CCCallFuncO* t_call2 = CCCallFuncO::create(this, callfuncO_selector(PuzzleMapScene::endChangePiece), found_node);
		CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		found_node->runAction(t_seq);
	}
	else
	{
		stopPuzzleModeChange();
	}

	if(ing_check_puzzle >= start_stage_number+stage_count)
	{
		stopPuzzleModeChange();
	}
}

void PuzzleMapScene::stopPuzzleModeChange()
{
	unschedule(schedule_selector(PuzzleMapScene::ingPuzzleModeChange));

	is_menu_enable = true;
	map_mode_state = before_map_mode_state;
}

void PuzzleMapScene::endChangePiece( CCObject* sender )
{
	CCFadeTo* t_fade = CCFadeTo::create(0.3f, 255);
	((StagePiece*)sender)->shadow_node->runAction(t_fade);
}

void PuzzleMapScene::resetStagePiece()
{
	if(touched_stage_number != 0)
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(touched_stage_number);
		if(t_sp)
			t_sp->touchCancelled(NULL, NULL);
		touched_stage_number = 0;
	}
}
