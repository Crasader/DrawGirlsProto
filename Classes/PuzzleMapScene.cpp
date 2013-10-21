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
#include "ServerDataSave.h"
#include "StageListDown.h"
#include "StageInfoDown.h"
#include "EventPopup.h"
#include "CardSettingScene.h"
#include "OptionScene.h"
#include "GachaPopup.h"
#include "RankPopup.h"
#include "PostboxPopup.h"

enum PMS_Zorder{
	kPMS_Z_back = 1,
	kPMS_Z_puzzle_back_side,
	kPMS_Z_puzzle_shadow,
	kPMS_Z_puzzle_back,
	kPMS_Z_stage,
	kPMS_Z_ui_button,
	kPMS_Z_popup
};

enum PMS_MenuTag{
	kPMS_MT_stageBase = 0,
	kPMS_MT_option = 10001,
	kPMS_MT_gacha,
	kPMS_MT_rank,
	kPMS_MT_postbox,
	kPMS_MT_diary,
	kPMS_MT_event,
	kPMS_MT_eventClose,
	kPMS_MT_screen,
	kPMS_MT_showui,
	kPMS_MT_top,
	kPMS_MT_bottom,
	kPMS_MT_rubyShop,
	kPMS_MT_goldShop,
	kPMS_MT_lifeShop
};

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
	
	Json::Value param;
	param["ManualLogin"] = true;
	
	hspConnector::get()->login(param, param, std::bind(&PuzzleMapScene::resultLogin, this, std::placeholders::_1));
	
    return true;
}

void PuzzleMapScene::resultLogin(Json::Value result_data)
{
	CCLog("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	
	//	hspConnector::get()->kLoadFriends(std::bind(&WorldMapScene::resultFriendList, this, std::placeholders::_1));
	
	StageListDown* t_sld = StageListDown::create(this, callfunc_selector(PuzzleMapScene::startSceneSetting));
	addChild(t_sld, kPMS_Z_popup);
}

void PuzzleMapScene::startSceneSetting()
{
	map_mode_state = kMMS_default;
	
	my_puzzle_mode = kPM_default;
	
	setMapNode();
	setUIs();
	
	is_menu_enable = false;
	startChangeUiMode();
	setTouchEnabled(true);
}

void PuzzleMapScene::setMapNode()
{
	maximum_scale = 2.f;
	minimum_scale = 1.f;
	
	map_node = CCNode::create();
	map_node->setPosition(ccp(240,160));
	addChild(map_node, kPMS_Z_puzzle_back_side);
	
	map_node->getCamera()->setEyeXYZ(0, 0, 2.f);
	
	CCSprite* map_back_center = CCSprite::create("test_map_back_center.png");
	map_back_center->setPosition(CCPointZero);
	map_node->addChild(map_back_center, kPMS_Z_puzzle_back_side);
	
	CCSize center_size = CCSizeMake(520.f, 340.f);
	
	CCSprite* map_back_left = CCSprite::create("test_map_back_left.png");
	map_back_left->setAnchorPoint(ccp(0.f,0.5f));
	map_back_left->setPosition(ccp(-center_size.width/2.f, 0));
	map_node->addChild(map_back_left, kPMS_Z_puzzle_back);
	
	CCSprite* map_back_right = CCSprite::create("test_map_back_right.png");
	map_back_right->setAnchorPoint(ccp(1.f,0.5f));
	map_back_right->setPosition(ccp(center_size.width/2.f, 0));
	map_node->addChild(map_back_right, kPMS_Z_puzzle_back);
	
	CCSprite* map_back_top = CCSprite::create("test_map_back_top.png");
	map_back_top->setAnchorPoint(ccp(0.5f,1.f));
	map_back_top->setPosition(ccp(0, center_size.height/2.f));
	map_node->addChild(map_back_top, kPMS_Z_puzzle_back);
	
	CCSprite* map_back_bottom = CCSprite::create("test_map_back_bottom.png");
	map_back_bottom->setAnchorPoint(ccp(0.5f,0.f));
	map_back_bottom->setPosition(ccp(0,-center_size.height/2.f));
	map_node->addChild(map_back_bottom, kPMS_Z_puzzle_back);
	
	shadow_batchnode = CCSpriteBatchNode::create("test_map_shadow.png");
	shadow_batchnode->setPosition(CCPointZero);
	map_node->addChild(shadow_batchnode, kPMS_Z_puzzle_shadow);
	
	CCRect stage_rect = CCRectMake(-25.f, -25.f, 50.f, 50.f);
	int theme_code = 1;
	
	CCPoint base_position = ccp(-171.25f, 102.75f);
	
	for(int i=1;i<=4;i++)
	{
		for(int j=1;j<=6;j++)
		{
			int stage_level = 1;// SDS_GI(kStageInfo..........)
			
			if(myDSH->getBoolForKey(kDSH_Key_isOpenTheme_int1_Stage_int2, theme_code, (i-1)*6 + j))
			{
				bool is_found = false;
				int found_number = 0;
				bool is_have_card[3] = {0,};
				for(int k=3;k>=1;k--)
				{
					int card_number = SDS_GI(kSDF_stageInfo, (i-1)*6 + j, CCString::createWithFormat("level%d_card", k)->getCString());
					int card_durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number);
					if(card_durability > 0)
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
					StagePiece* t_sp = StagePiece::create(CCString::createWithFormat("test_puzzle%d_%d.png", i, j)->getCString(),
														  (i-1)*6 + j, stage_level, ccpAdd(base_position, ccp((j-1)*68.5f, -(i-1)*68.5f)), stage_rect, false, false);
					map_node->addChild(t_sp, kPMS_Z_stage, t_sp->getStageNumber());
					t_sp->setChangable(CCString::createWithFormat("test_thumb%d_%d.png", i, j)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
					t_sp->shadow_node = addShadow(i, j, base_position);
				}
				else if(found_number == 2)
				{
					StagePiece* t_sp = StagePiece::create(CCString::createWithFormat("test_puzzle%d_%d.png", i, j)->getCString(),
														  (i-1)*6 + j, stage_level, ccpAdd(base_position, ccp((j-1)*68.5f, -(i-1)*68.5f)), stage_rect, false, true);
					map_node->addChild(t_sp, kPMS_Z_stage, t_sp->getStageNumber());
					t_sp->setChangable(CCString::createWithFormat("test_thumb%d_%d.png", i, j)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
					t_sp->shadow_node = addShadow(i, j, base_position);
				}
				else if(found_number == 1)
				{
					StagePiece* t_sp = StagePiece::create(CCString::createWithFormat("test_puzzle%d_%d.png", i, j)->getCString(),
														  (i-1)*6 + j, stage_level, ccpAdd(base_position, ccp((j-1)*68.5f, -(i-1)*68.5f)), stage_rect, true, true);
					map_node->addChild(t_sp, kPMS_Z_stage, t_sp->getStageNumber());
					t_sp->setChangable(CCString::createWithFormat("test_thumb%d_%d.png", i, j)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
					t_sp->shadow_node = addShadow(i, j, base_position);
				}
				else
				{
					StagePiece* t_sp = StagePiece::create("test_puzzle_empty.png",
														  (i-1)*6 + j, stage_level, ccpAdd(base_position, ccp((j-1)*68.5f, -(i-1)*68.5f)), stage_rect, false, false);
					map_node->addChild(t_sp, kPMS_Z_stage, t_sp->getStageNumber());
				}
			}
			else
			{
				StagePiece* t_sp = StagePiece::create("test_puzzle_empty.png",
													  (i-1)*6 + j, stage_level, ccpAdd(base_position, ccp((j-1)*68.5f, -(i-1)*68.5f)), stage_rect, false, false);
				map_node->addChild(t_sp, kPMS_Z_stage, t_sp->getStageNumber());
			}
		}
	}
	
	stage_count = 4*6;
	touched_stage_number = 0;
}

CCSprite* PuzzleMapScene::addShadow(int i, int j, CCPoint base_position)
{
	bool is_long = ((i-1)*6+j)%2 == 1 ? true : false;
	if(((i-1)*6+j-1)/6%2 == 1)	is_long = !is_long;
	
	CCSprite* t_shadow = CCSprite::create("test_map_shadow.png", CCRectMake(is_long ? 0 : 120, 0, 120, 120));
	t_shadow->setPosition(ccpAdd(base_position, ccp((j-1)*68.5f, -(i-1)*68.5f)));
	shadow_batchnode->addChild(t_shadow);
	
	return t_shadow;
}

void PuzzleMapScene::setUIs()
{
	CCSprite* ui_back = CCSprite::create("test_ui_back.png");
	ui_back->setPosition(ccp(240,160));
	addChild(ui_back, kPMS_Z_back);
	
	
	CCSprite* top_case = CCSprite::create("test_ui_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(getUiButtonPosition(kPMS_MT_top));
	addChild(top_case, kPMS_Z_ui_button, kPMS_MT_top);
	
	
	CCSprite* bottom_case = CCSprite::create("test_ui_bottom.png");
	bottom_case->setAnchorPoint(ccp(0.5f,0.f));
	bottom_case->setPosition(getUiButtonPosition(kPMS_MT_bottom));
	addChild(bottom_case, kPMS_Z_ui_button, kPMS_MT_bottom);
	
	CCSize bottom_size = bottom_case->getContentSize();
	
	CCMenuItem* postbox_item = CCMenuItemImage::create("test_ui_bottom_empty.png", "test_ui_bottom_empty.png", this, menu_selector(PuzzleMapScene::menuAction));
	postbox_item->setTag(kPMS_MT_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2-56.f*2.f,0)));
	bottom_case->addChild(postbox_menu);
	
	
	CCMenuItem* diary_item = CCMenuItemImage::create("test_ui_bottom_empty.png", "test_ui_bottom_empty.png", this, menu_selector(PuzzleMapScene::menuAction));
	diary_item->setTag(kPMS_MT_diary);
	
	CCMenu* diary_menu = CCMenu::createWithItem(diary_item);
	diary_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2-56.f,0)));
	bottom_case->addChild(diary_menu);
	
	
	CCMenuItem* rank_item = CCMenuItemImage::create("test_ui_bottom_empty.png", "test_ui_bottom_empty.png", this, menu_selector(PuzzleMapScene::menuAction));
	rank_item->setTag(kPMS_MT_rank);
	
	CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
	rank_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2,0)));
	bottom_case->addChild(rank_menu);
	
	CCMenuItem* gacha_item = CCMenuItemImage::create("test_ui_bottom_empty.png", "test_ui_bottom_empty.png", this, menu_selector(PuzzleMapScene::menuAction));
	gacha_item->setTag(kPMS_MT_gacha);
	
	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
	gacha_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2+56.f,0)));
	bottom_case->addChild(gacha_menu);
	
	CCMenuItem* option_item = CCMenuItemImage::create("test_ui_bottom_empty.png", "test_ui_bottom_empty.png", this, menu_selector(PuzzleMapScene::menuAction));
	option_item->setTag(kPMS_MT_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccpAdd(ccp(bottom_size.width/2.f, bottom_size.height/2.f), ccp(2+56.f*2.f,0)));
	bottom_case->addChild(option_menu);
	
	
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
	
	
	int event_length = SDS_GI(kSDF_gameInfo, "event_count");
	if(event_length > 0)
	{
		CCSprite* n_event = CCSprite::create("test_ui_event.png");
		CCSprite* s_event = CCSprite::create("test_ui_event.png");
		s_event->setColor(ccGRAY);
		
		CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(PuzzleMapScene::menuAction));
		event_item->setTag(kPMS_MT_event);
		
		CCMenu* event_menu = CCMenu::createWithItem(event_item);
		event_menu->setEnabled(false);
		event_menu->setPosition(getUiButtonPosition(kPMS_MT_event));
		addChild(event_menu, kPMS_Z_ui_button, kPMS_MT_event);
		
		
		CCSprite* n_event_close = CCSprite::create("test_ui_event_close.png");
		CCSprite* s_event_close = CCSprite::create("test_ui_event_close.png");
		s_event_close->setColor(ccGRAY);
		
		CCMenuItem* event_close_item = CCMenuItemSprite::create(n_event_close, s_event_close, this, menu_selector(PuzzleMapScene::menuAction));
		event_close_item->setTag(kPMS_MT_eventClose);
		
		CCMenu* event_close_menu = CCMenu::createWithItem(event_close_item);
		event_close_menu->setEnabled(false);
		event_close_menu->setPosition(getUiButtonPosition(kPMS_MT_eventClose));
		addChild(event_close_menu, kPMS_Z_ui_button, kPMS_MT_eventClose);
		
//		showEventButton();
	}
}

void PuzzleMapScene::showEventButton()
{
	CCMenu* event_menu = (CCMenu*)getChildByTag(kPMS_MT_event);
	CCMenu* event_close_menu = (CCMenu*)getChildByTag(kPMS_MT_eventClose);
	
	CCMoveTo* t_move1 = CCMoveTo::create(0.5f, ccp(420,-(myDSH->puzzle_ui_top-320.f)/2.f + 52.f));
	CCMoveTo* t_move2 = CCMoveTo::create(0.5f, ccp(450,-(myDSH->puzzle_ui_top-320.f)/2.f + 102.f));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::onEventButton));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
	
	event_menu->runAction(t_move1);
	event_close_menu->runAction(t_seq);
}
void PuzzleMapScene::onEventButton()
{
	((CCMenu*)getChildByTag(kPMS_MT_event))->setEnabled(true);
	((CCMenu*)getChildByTag(kPMS_MT_eventClose))->setEnabled(true);
}
void PuzzleMapScene::hideEventButton()
{
	((CCMenu*)getChildByTag(kPMS_MT_event))->setEnabled(false);
	((CCMenu*)getChildByTag(kPMS_MT_eventClose))->setEnabled(false);
	
	CCMenu* event_menu = (CCMenu*)getChildByTag(kPMS_MT_event);
	CCMenu* event_close_menu = (CCMenu*)getChildByTag(kPMS_MT_eventClose);
	
	CCMoveTo* t_move1 = CCMoveTo::create(0.5f, getUiButtonPosition(kPMS_MT_event));
	CCMoveTo* t_move2 = CCMoveTo::create(0.5f, getUiButtonPosition(kPMS_MT_eventClose));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleMapScene::onEventButton));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_call);
	
	event_menu->runAction(t_move1);
	event_close_menu->runAction(t_seq);
}

CCPoint PuzzleMapScene::getUiButtonPosition(int t_tag)
{
	CCPoint return_value;
	
//	if(t_tag == kWMS_MT_cardSetting)		return_value = ccp(50,63);
//	else if(t_tag == kWMS_MT_option)		return_value = ccp(120,34);
//	else if(t_tag == kWMS_MT_gacha)			return_value = ccp(175,34);
//	else if(t_tag == kWMS_MT_rank)			return_value = ccp(230,34);
//	else if(t_tag == kWMS_MT_postbox)		return_value = ccp(285,34);
	if(t_tag == kPMS_MT_event)				return_value = ccp(420,-(myDSH->puzzle_ui_top-320.f)/2.f - 100.f); // after move animation
	else if(t_tag == kPMS_MT_eventClose)	return_value = ccp(450,-(myDSH->puzzle_ui_top-320.f)/2.f - 50.f); // after move animation
	else if(t_tag == kPMS_MT_screen)		return_value = ccp(455,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f - 19.f);
	else if(t_tag == kPMS_MT_showui)		return_value = ccp(240,-10);
	else if(t_tag == kPMS_MT_top)			return_value = ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f + 33.f); // after_move_animation
	else if(t_tag == kPMS_MT_bottom)		return_value = ccp(145,-(myDSH->puzzle_ui_top-320.f)/2.f - 65.f); // after_move_animation
//	else if(t_tag == kWMS_MT_rubyShop)		return_value = ccp(140,297);
//	else if(t_tag == kWMS_MT_goldShop)		return_value = ccp(294,297);
//	else if(t_tag == kWMS_MT_lifeShop)		return_value = ccp(448,297);
//	
//	else if(t_tag == kWMS_MT_collection)	return_value = ccp(120,92);
	//	else if(t_tag == )				return_value = ;
	
	return return_value;
}

void PuzzleMapScene::stageAction(int t_number)
{
	is_menu_enable = false;
	
	mySD->setSilType(t_number);
	myDSH->setIntegerForKey(kDSH_Key_lastSelectedStage, t_number);
	
	StageInfoDown* t_sid = StageInfoDown::create(this, callfunc_selector(PuzzleMapScene::stageCancel));
	addChild(t_sid, kPMS_Z_popup);
}

void PuzzleMapScene::startChangeUiMode()
{
	map_mode_state = kMMS_changeMode;
	
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(false);
	((CCMenu*)getChildByTag(kPMS_MT_eventClose))->setVisible(false);
	
	if(((CCMenu*)getChildByTag(kPMS_MT_event))->getPositionY() < -(myDSH->puzzle_ui_top-320.f)/2.f - 24.f)
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
	
	CCSprite* bottom_case = (CCSprite*)getChildByTag(kPMS_MT_bottom);
	bottom_case->setPositionY(bottom_case->getPositionY() + change_bottom_dy);
	
	if(change_frame >= 30)
		stopChangeUiMode();
}
void PuzzleMapScene::stopChangeUiMode()
{
	unschedule(schedule_selector(PuzzleMapScene::changeUiMode));
	map_mode_state = kMMS_uiMode;
	is_menu_enable = true;
}

void PuzzleMapScene::startChangeMapMode()
{
	map_mode_state = kMMS_changeMode;
	
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
	
	CCSprite* bottom_case = (CCSprite*)getChildByTag(kPMS_MT_bottom);
	bottom_case->setPositionY(bottom_case->getPositionY() + change_bottom_dy);
	
	if(change_frame >= 30)
		stopChangeMapMode();
}
void PuzzleMapScene::stopChangeMapMode()
{
	((CCMenu*)getChildByTag(kPMS_MT_showui))->setVisible(true);
	
	unschedule(schedule_selector(PuzzleMapScene::changeMapMode));
	map_mode_state = kMMS_default;
	is_menu_enable = true;
}

void PuzzleMapScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	CCLog("menu %d", tag);
	
	if(tag == kPMS_MT_eventClose)
	{
		hideEventButton();
		is_menu_enable = true;
	}
	else if(tag == kPMS_MT_event)
	{
		EventPopup* t_ep = EventPopup::create(this, callfunc_selector(PuzzleMapScene::popupClose));
		addChild(t_ep, kPMS_Z_popup);
	}
	else if(tag == kPMS_MT_screen)
	{
		//////////////
//		is_menu_enable = true;
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
		startChangeUiMode();
	}
	else if(tag == kPMS_MT_diary)
	{
		mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
		CCDirector::sharedDirector()->replaceScene(CardSettingScene::scene());
	}
	else if(tag == kPMS_MT_option)
	{
		CCDirector::sharedDirector()->replaceScene(OptionScene::scene());
	}
	else if(tag == kPMS_MT_gacha)
	{
		GachaPopup* t_gp = GachaPopup::create(this, callfunc_selector(PuzzleMapScene::popupClose));
		addChild(t_gp, kPMS_Z_popup);
	}
	else if(tag == kPMS_MT_rank)
	{
		RankPopup* t_rp = RankPopup::create(this, callfunc_selector(PuzzleMapScene::popupClose));
		addChild(t_rp, kPMS_Z_popup);
	}
	else if(tag == kPMS_MT_postbox)
	{
		PostboxPopup* t_pp = PostboxPopup::create(this, callfunc_selector(PuzzleMapScene::popupClose));
		addChild(t_pp, kPMS_Z_popup);
	}
//	else if(tag == kWMS_MT_rubyShop)
//	{
//		RubyShopPopup* t_rsp = RubyShopPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
//		addChild(t_rsp, kWMS_Z_popup);
//	}
//	else if(tag == kWMS_MT_goldShop)
//	{
//		RubyShopPopup* t_rsp = RubyShopPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
//		addChild(t_rsp, kWMS_Z_popup);
//	}
//	else if(tag == kWMS_MT_lifeShop)
//	{
//		RubyShopPopup* t_rsp = RubyShopPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
//		addChild(t_rsp, kWMS_Z_popup);
//	}
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
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(PuzzleMapScene::alertAction));
}