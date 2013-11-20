//
//  WorldMapScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 10..
//
//

#include "WorldMapScene.h"
#include "ScreenSide.h"
#include "MyLocalization.h"
#include "StageSettingScene.h"
#include "StageInfoDown.h"
#include "OptionScene.h"
#include "GachaPopup.h"
#include "RankPopup.h"
#include "PostboxPopup.h"
#include "EventPopup.h"
#include "RubyShopPopup.h"
#include "CardSettingScene.h"
#include "StageListDown.h"
#include "GradationBGI.h"
#include "TicketNeedPopup.h"

#include <algorithm>
#include <ostream>

void WorldMapScene::showConvertSildata(string filename)
{
	CCLog("%s", (filename+"_sildata.txt").c_str());
	string path = CCFileUtils::sharedFileUtils()->fullPathForFilename((filename+".txt").c_str());
	
	unsigned long buff = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rt", &buff);
	
	string convert_data((char*)pBuffer);
	
	remove(convert_data.begin(), convert_data.end(), '\t');
	remove(convert_data.begin(), convert_data.end(), '\n');
	
	convert_data = convert_data.substr(0,convert_data.find(":::")); // ,buff);
	
	bool is_zero = true;
	int error_cnt = 0;
	int cmp_cnt = 0;
	string result_data = "";
	for(int i=0;i<convert_data.size();i++)
	{
		if(is_zero)
		{
			if(convert_data[i] == '0')
			{
				cmp_cnt++;
			}
			else if(convert_data[i] == '1')
			{
				if(cmp_cnt >= 10000)		result_data.append("D");
				else if(cmp_cnt >= 1000)	result_data.append("C");
				else if(cmp_cnt >= 100)		result_data.append("B");
				else if(cmp_cnt >= 10)		result_data.append("A");
				
				ostringstream oss;
				oss<<cmp_cnt;
				
				result_data.append(oss.str().c_str());
				cmp_cnt = 1;
				is_zero = false;
			}
			else
			{
//				CCLog("invalid data %s", convert_data.substr(i,1).c_str());
				if(error_cnt == 0)
					CCLog("error data : %d", convert_data[i]);
				error_cnt++;
			}
		}
		else
		{
			if(convert_data[i] == '1')
			{
				cmp_cnt++;
			}
			else if(convert_data[i] == '0')
			{
				if(cmp_cnt >= 10000)		result_data.append("D");
				else if(cmp_cnt >= 1000)	result_data.append("C");
				else if(cmp_cnt >= 100)		result_data.append("B");
				else if(cmp_cnt >= 10)		result_data.append("A");
				
				ostringstream oss;
				oss<<cmp_cnt;
				
				result_data.append(oss.str().c_str());
				cmp_cnt = 1;
				is_zero = true;
			}
			else
			{
//				CCLog("invalid data %s", convert_data.substr(i,1).c_str());
				if(error_cnt == 0)
					CCLog("error data : %d", convert_data[i]);
				error_cnt++;
			}
		}
	}
	
	CCLog("error cnt : %d", error_cnt);
	
	if(cmp_cnt >= 10000)		result_data.append("D");
	else if(cmp_cnt >= 1000)	result_data.append("C");
	else if(cmp_cnt >= 100)		result_data.append("B");
	else if(cmp_cnt >= 10)		result_data.append("A");
	
	ostringstream oss;
	oss<<cmp_cnt;
	
	result_data.append(oss.str().c_str());
	
	CCLog("result size : %d", (int)result_data.size());
	CCLog("%s", result_data.c_str());
	CCLog("!!!");
}

CCScene* WorldMapScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    WorldMapScene *layer = WorldMapScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

enum WMS_Zorder{
	kWMS_Z_back = 1,
	kWMS_Z_track,
	kWMS_Z_stage,
	kWMS_Z_ui_button,
	kWMS_Z_popup
};

enum WMS_MenuTag{
	kWMS_MT_stageBase = 0,
	kWMS_MT_couponBase = 2000,
	kWMS_MT_collection = 5001,
	kWMS_MT_option = 5002,
	kWMS_MT_gacha = 5003,
	kWMS_MT_rank = 5004,
	kWMS_MT_postbox = 5005,
	kWMS_MT_event = 5006,
	kWMS_MT_cardSetting = 5007,
	kWMS_MT_rubyShop = 5008,
	kWMS_MT_goldShop = 5009,
	kWMS_MT_lifeShop = 5010
};


// on "init" you need to initialize your instance
bool WorldMapScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	//graphdog->setup("12345678", 1);
	
//	showConvertSildata("intun1");
//	showConvertSildata("intun2");
	
	setKeypadEnabled(true);
	
	////////////////////////////////////////////////////
	
//	Json::Value param;
//	param["ManualLogin"] = true;
//	
//	hspConnector::get()->login(param, param, std::bind(&WorldMapScene::resultLogin, this, std::placeholders::_1));
	
//	StageListDown* t_sld = StageListDown::create(this, callfunc_selector(WorldMapScene::setWorldMapScene));
//	addChild(t_sld);
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void WorldMapScene::resultLogin(Json::Value result_data)
{
	CCLog("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	
//	hspConnector::get()->kLoadFriends(std::bind(&WorldMapScene::resultFriendList, this, std::placeholders::_1));
	
//	StageListDown* t_sld = StageListDown::create(this, callfunc_selector(WorldMapScene::setWorldMapScene));
//	addChild(t_sld);
}

void WorldMapScene::resultFriendList(Json::Value result_data)
{
	CCLog("resultFriendList data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
}

void WorldMapScene::passTicketStage()
{
//	myDSH->setBoolForKey(kDSH_Key_isPassCoupon_int1, pass_target_stage, true);
	resetWorldMapScene();
}

void WorldMapScene::resetWorldMapScene()
{
	map_node->removeFromParent();
	setMapNode();
}

void WorldMapScene::setWorldMapScene()
{
	setMapNode();
	setUIs();
}

void WorldMapScene::setMapNode()
{
	map_node = CCNode::create();
	addChild(map_node, kWMS_Z_track);
	
	CCSpriteBatchNode* track_batch_node = CCSpriteBatchNode::create("worldmap_track.png");
	map_node->addChild(track_batch_node, kWMS_Z_track);
	
	CCSprite* n_stage1 = mySIL->getLoadedImg(CCSTR_CWF("stage%d_thumbnail.png", 1)->getCString());
	
	CCSize tb_size = n_stage1->getContentSize();
	tb_size.width /= 2.f;
	tb_size.height /= 2.f;
	CCPoint tb_point = ccp(tb_size.width, tb_size.height);
	
	CCSprite* n_case1 = CCSprite::create("worldmap_normal_on.png");
	n_case1->setPosition(tb_point);
	n_stage1->addChild(n_case1);
	CCLabelTTF* n_stage1_label = CCLabelTTF::create("1", mySGD->getFont().c_str(), 10);
	n_stage1_label->setColor(ccBLACK);
	n_stage1_label->setPosition(ccpAdd(tb_point, ccp(0,-30)));
	n_stage1->addChild(n_stage1_label);
	
	CCSprite* s_stage1 = mySIL->getLoadedImg(CCSTR_CWF("stage%d_thumbnail.png", 1)->getCString());
	s_stage1->setColor(ccGRAY);
	CCSprite* s_case1 = CCSprite::create("worldmap_normal_on.png");
	s_case1->setPosition(tb_point);
	s_case1->setColor(ccGRAY);
	s_stage1->addChild(s_case1);
	CCLabelTTF* s_stage1_label = CCLabelTTF::create("1", mySGD->getFont().c_str(), 10);
	s_stage1_label->setColor(ccBLACK);
	s_stage1_label->setPosition(ccpAdd(tb_point, ccp(0,-30)));
	s_stage1->addChild(s_stage1_label);
	
	CCMenuItem* stage1_item = CCMenuItemSprite::create(n_stage1, s_stage1, this, menu_selector(WorldMapScene::menuAction));
	stage1_item->setTag(kWMS_MT_stageBase + 1);
	
	CCMenu* stage1_menu = CCMenu::createWithItem(stage1_item);
	stage1_menu->setPosition(getStagePosition(1));
	map_node->addChild(stage1_menu, kWMS_Z_stage);
	
//	int cleared_number = DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_theme_int1_clearednumber, 1);
	int updated_stage = 1;
	
	for(int i=2;i<=updated_stage;i++)
	{
		bool is_coupon = false;
		bool is_pass = false;
		if(i-1 <= 1)//cleared_number)
		{
			if(SDS_GB(kSDF_gameInfo, CCSTR_CWF("is_coupon%d", i)->getCString()))
			{
				is_coupon = true;
				if(1)//myDSH->getBoolForKey(kDSH_Key_isPassCoupon_int1, i))
				{
					is_pass = true;
					CCSprite* pass_img = CCSprite::create("worldmap_ticket_open.png");
					pass_img->setPosition(getCouponPosition(i));
					map_node->addChild(pass_img, kWMS_Z_stage);
					
					
					CCSprite* n_stage = mySIL->getLoadedImg(CCSTR_CWF("stage%d_thumbnail.png", i)->getCString());
					CCSprite* n_case = CCSprite::create("worldmap_normal_on.png");
					n_case->setPosition(tb_point);
					n_stage->addChild(n_case);
					CCLabelTTF* n_stage_label = CCLabelTTF::create(CCSTR_CWF("%d", i)->getCString(), mySGD->getFont().c_str(), 10);
					n_stage_label->setColor(ccBLACK);
					n_stage_label->setPosition(ccpAdd(tb_point, ccp(0,-30)));
					n_stage->addChild(n_stage_label);
					
					CCSprite* s_stage = mySIL->getLoadedImg(CCSTR_CWF("stage%d_thumbnail.png", i)->getCString());
					s_stage->setColor(ccGRAY);
					CCSprite* s_case = CCSprite::create("worldmap_normal_on.png");
					s_case->setPosition(tb_point);
					s_case->setColor(ccGRAY);
					s_stage->addChild(s_case);
					CCLabelTTF* s_stage_label = CCLabelTTF::create(CCSTR_CWF("%d", i)->getCString(), mySGD->getFont().c_str(), 10);
					s_stage_label->setColor(ccBLACK);
					s_stage_label->setPosition(ccpAdd(tb_point, ccp(0,-30)));
					s_stage->addChild(s_stage_label);
					
					CCMenuItem* stage_item = CCMenuItemSprite::create(n_stage, s_stage, this, menu_selector(WorldMapScene::menuAction));
					stage_item->setTag(kWMS_MT_stageBase+i);
					
					CCMenu* stage_menu = CCMenu::createWithItem(stage_item);
					stage_menu->setPosition(getStagePosition(i));
					map_node->addChild(stage_menu, kWMS_Z_stage);
				}
				else
				{
					CCSprite* n_coupon = mySIL->getLoadedImg(CCSTR_CWF("coupon%d_thumbnail.png", i)->getCString());
					CCSprite* s_coupon = mySIL->getLoadedImg(CCSTR_CWF("coupon%d_thumbnail.png", i)->getCString());
					s_coupon->setColor(ccGRAY);
					
					CCMenuItem* coupon_item = CCMenuItemSprite::create(n_coupon, s_coupon, this, menu_selector(WorldMapScene::menuAction));
					coupon_item->setTag(kWMS_MT_couponBase+i);
					
					CCMenu* coupon_menu = CCMenu::createWithItem(coupon_item);
					coupon_menu->setPosition(getCouponPosition(i));
					map_node->addChild(coupon_menu, kWMS_Z_stage);
				}
			}
			else
			{
				CCSprite* n_stage = mySIL->getLoadedImg(CCSTR_CWF("stage%d_thumbnail.png", i)->getCString());
				CCSprite* n_case = CCSprite::create("worldmap_normal_on.png");
				n_case->setPosition(tb_point);
				n_stage->addChild(n_case);
				CCLabelTTF* n_stage_label = CCLabelTTF::create(CCSTR_CWF("%d", i)->getCString(), mySGD->getFont().c_str(), 10);
				n_stage_label->setColor(ccBLACK);
				n_stage_label->setPosition(ccpAdd(tb_point, ccp(0,-30)));
				n_stage->addChild(n_stage_label);
				
				CCSprite* s_stage = mySIL->getLoadedImg(CCSTR_CWF("stage%d_thumbnail.png", i)->getCString());
				s_stage->setColor(ccGRAY);
				CCSprite* s_case = CCSprite::create("worldmap_normal_on.png");
				s_case->setPosition(tb_point);
				s_case->setColor(ccGRAY);
				s_stage->addChild(s_case);
				CCLabelTTF* s_stage_label = CCLabelTTF::create(CCSTR_CWF("%d", i)->getCString(), mySGD->getFont().c_str(), 10);
				s_stage_label->setColor(ccBLACK);
				s_stage_label->setPosition(ccpAdd(tb_point, ccp(0,-30)));
				s_stage->addChild(s_stage_label);
				
				
				CCMenuItem* stage_item = CCMenuItemSprite::create(n_stage, s_stage, this, menu_selector(WorldMapScene::menuAction));
				stage_item->setTag(kWMS_MT_stageBase+i);
				
				CCMenu* stage_menu = CCMenu::createWithItem(stage_item);
				stage_menu->setPosition(getStagePosition(i));
				map_node->addChild(stage_menu, kWMS_Z_stage);
			}
		}
		else
		{
			CCSprite* d_stage = CCSprite::create("worldmap_normal_off.png");
			CCLabelTTF* d_stage_label = CCLabelTTF::create(CCString::createWithFormat("%d", i)->getCString(), mySGD->getFont().c_str(), 10);
			d_stage_label->setColor(ccBLACK);
			d_stage_label->setPosition(ccpAdd(tb_point, ccp(0,-30)));
			d_stage->addChild(d_stage_label);
			
			d_stage->setPosition(getStagePosition(i));
			map_node->addChild(d_stage, kWMS_Z_stage);
			break;
		}
		
		string sub_value = "";
		CCPoint track_position;
		if(!is_coupon)
		{
			sub_value = "stage";
			track_position = getStagePosition(i);
		}
		else
		{
			sub_value = "coupon";
			track_position = getCouponPosition(i);
		}
		
		
		int loop_cnt_j = SDS_GI(kSDF_gameInfo, CCSTR_CWF("%s%d_path_length", sub_value.c_str(), i)->getCString());
		
		string direction = SDS_GS(kSDF_gameInfo, CCSTR_CWF("%s%d_path_direction%d", sub_value.c_str(), i, 0)->getCString());
		CCSprite* t_track1 = CCSprite::create("worldmap_track.png", CCRectMake(0, 0, 40, 40));
		t_track1->setPosition(track_position);
		t_track1->setRotation(getRotateValue(direction));
		track_batch_node->addChild(t_track1, kWMS_Z_track);
		
		track_position = ccpAdd(track_position, getTrackPositionMove(direction));
		
		for(int j=0;j<loop_cnt_j;j++)
		{
			string direction = SDS_GS(kSDF_gameInfo, CCSTR_CWF("%s%d_path_direction%d", sub_value.c_str(), i, j)->getCString());
			int d_count = SDS_GI(kSDF_gameInfo, CCSTR_CWF("%s%d_path_count%d", sub_value.c_str(), i, j)->getCString());
			for(int k=0;k<d_count;k++)
			{
				if(k < d_count-1)
				{
					CCSprite* t_track = CCSprite::create("worldmap_track.png", CCRectMake(40, 0, 40, 40));
					t_track->setPosition(track_position);
					t_track->setRotation(getRotateValue(direction));
					track_batch_node->addChild(t_track, kWMS_Z_track);
					track_position = ccpAdd(track_position, getTrackPositionMove(direction));
				}
				else if(j == loop_cnt_j-1)
				{
					CCSprite* t_track = CCSprite::create("worldmap_track.png", CCRectMake(40, 0, 40, 40));
					t_track->setPosition(track_position);
					t_track->setRotation(getRotateValue(direction));
					track_batch_node->addChild(t_track, kWMS_Z_track);
					track_position = ccpAdd(track_position, getTrackPositionMove(direction));
				}
				else if(j < loop_cnt_j-1)
				{
					string a_direction = SDS_GS(kSDF_gameInfo, CCSTR_CWF("%s%d_path_direction%d", sub_value.c_str(), i, j+1)->getCString());
					CCSprite* t_track = CCSprite::create("worldmap_track.png", CCRectMake(80, 0, 40, 40));
					t_track->setPosition(track_position);
					t_track->setRotation(getTurnRotateValue(direction, a_direction));
					track_batch_node->addChild(t_track, kWMS_Z_track);
					track_position = ccpAdd(track_position, getTrackPositionMove(a_direction));
				}
			}
			
			if(i == 2 && j == loop_cnt_j-1)
			{
				CCSprite* t_track = CCSprite::create("worldmap_track.png", CCRectMake(0, 0, 40, 40));
				t_track->setPosition(getStagePosition(1));
				t_track->setRotation(getRotateValue(direction) + 180);
				track_batch_node->addChild(t_track, kWMS_Z_track);
			}
		}
		
		if(is_coupon && is_pass)
		{
			sub_value = "stage";
			
			track_position = getStagePosition(i);
			int loop_cnt_j = SDS_GI(kSDF_gameInfo, CCSTR_CWF("%s%d_path_length", sub_value.c_str(), i)->getCString());
			
			string direction = SDS_GS(kSDF_gameInfo, CCSTR_CWF("%s%d_path_direction%d", sub_value.c_str(), i, 0)->getCString());
			CCSprite* t_track1 = CCSprite::create("worldmap_track.png", CCRectMake(0, 0, 40, 40));
			t_track1->setPosition(track_position);
			t_track1->setRotation(getRotateValue(direction));
			track_batch_node->addChild(t_track1, kWMS_Z_track);
			
			track_position = ccpAdd(track_position, getTrackPositionMove(direction));
			
			for(int j=0;j<loop_cnt_j;j++)
			{
				string direction = SDS_GS(kSDF_gameInfo, CCSTR_CWF("%s%d_path_direction%d", sub_value.c_str(), i, j)->getCString());
				int d_count = SDS_GI(kSDF_gameInfo, CCSTR_CWF("%s%d_path_count%d", sub_value.c_str(), i, j)->getCString());
				for(int k=0;k<d_count;k++)
				{
					if(k < d_count-1)
					{
						CCSprite* t_track = CCSprite::create("worldmap_track.png", CCRectMake(40, 0, 40, 40));
						t_track->setPosition(track_position);
						t_track->setRotation(getRotateValue(direction));
						track_batch_node->addChild(t_track, kWMS_Z_track);
						track_position = ccpAdd(track_position, getTrackPositionMove(direction));
					}
					else if(j == loop_cnt_j-1)
					{
						CCSprite* t_track = CCSprite::create("worldmap_track.png", CCRectMake(40, 0, 40, 40));
						t_track->setPosition(track_position);
						t_track->setRotation(getRotateValue(direction));
						track_batch_node->addChild(t_track, kWMS_Z_track);
						track_position = ccpAdd(track_position, getTrackPositionMove(direction));
					}
					else if(j < loop_cnt_j-1)
					{
						string a_direction = SDS_GS(kSDF_gameInfo, CCSTR_CWF("%s%d_path_direction%d", sub_value.c_str(), i, j+1)->getCString());
						CCSprite* t_track = CCSprite::create("worldmap_track.png", CCRectMake(80, 0, 40, 40));
						t_track->setPosition(track_position);
						t_track->setRotation(getTurnRotateValue(direction, a_direction));
						track_batch_node->addChild(t_track, kWMS_Z_track);
						track_position = ccpAdd(track_position, getTrackPositionMove(a_direction));
					}
				}
				
				if(i == 2 && j == loop_cnt_j-1)
				{
					CCSprite* t_track = CCSprite::create("worldmap_track.png", CCRectMake(0, 0, 40, 40));
					t_track->setPosition(getStagePosition(1));
					t_track->setRotation(getRotateValue(direction) + 180);
					track_batch_node->addChild(t_track, kWMS_Z_track);
				}
			}
		}
		
		if(is_coupon && !is_pass)
		{
			break;
		}
	}
	
	left_x = SDS_GI(kSDF_gameInfo, "left_x")*40.f+20.f;
	right_x = SDS_GI(kSDF_gameInfo, "right_x")*40.f+20.f;
	top_y = SDS_GI(kSDF_gameInfo, "top_y")*40.f+20.f;
	bottom_y = SDS_GI(kSDF_gameInfo, "bottom_y")*40.f+20.f;
	
	GradationBGI* t_gra = GradationBGI::create(left_x, right_x, bottom_y, top_y);
	t_gra->setPosition(CCPointZero);
	map_node->addChild(t_gra, kWMS_Z_back);
	
	maximum_scale = 1.f;
	minimum_scale = maximum_scale/3.f;
	
	int last_selected_stage = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage);
	if(last_selected_stage == 0)
	{
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedStage, 1);
		last_selected_stage = 1;
	}
	
	CCPoint init_position;
	
	init_position.x = SDS_GI(kSDF_gameInfo, CCSTR_CWF("stage%d_x", last_selected_stage)->getCString());
	init_position.y = SDS_GI(kSDF_gameInfo, CCSTR_CWF("stage%d_y", last_selected_stage)->getCString());
	
	init_position = ccpMult(init_position, -40.f);
	init_position = ccpAdd(init_position, ccp(-20.f,-20.f));
	init_position = ccpAdd(init_position, ccp(240,160));
	
	map_node->setPosition(init_position);
	is_menu_enable = true;
}

void WorldMapScene::setUIs()
{
	CCSprite* worldmap_shopback = CCSprite::create("worldmap_shopback.png");
	worldmap_shopback->setPosition(ccp(240,295));
	addChild(worldmap_shopback, kWMS_Z_ui_button);
	
	CCSprite* n_card = CCSprite::create("worldmap_card.png");
	CCSprite* s_card = CCSprite::create("worldmap_card.png");
	s_card->setColor(ccGRAY);
	
	CCMenuItem* card_item = CCMenuItemSprite::create(n_card, s_card, this, menu_selector(WorldMapScene::menuAction));
	card_item->setTag(kWMS_MT_cardSetting);
	
	CCMenu* card_menu = CCMenu::createWithItem(card_item);
	card_menu->setPosition(getUiButtonPosition(kWMS_MT_cardSetting));
	addChild(card_menu, kWMS_Z_stage);
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	if(selected_card_number > 0)
	{
		int selected_card_stage = selected_card_number/10;
		int selected_card_level = selected_card_number%10 + 1;
		
		CCSprite* card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", selected_card_stage, selected_card_level)->getCString());
		card_img->setScale(0.21f);
		card_img->setPosition(getUiButtonPosition(kWMS_MT_cardSetting));
		addChild(card_img, kWMS_Z_ui_button);
		
		if(selected_card_level == 3 && mySD->isAnimationStage(selected_card_stage))
		{
			CCSize ani_size = mySD->getAnimationCutSize(selected_card_stage);
			CCSprite* card_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", selected_card_stage, selected_card_level)->getCString(),
													 CCRectMake(0, 0, ani_size.width, ani_size.height));
			card_ani->setPosition(mySD->getAnimationPosition(selected_card_stage));
			card_img->addChild(card_ani);
		}
	}
	
	//	CCSprite* n_collection = CCSprite::create("worldmap_collection.png");
	//	CCSprite* s_collection = CCSprite::create("worldmap_collection.png");
	//	s_collection->setColor(ccGRAY);
	//
	//	CCMenuItem* collection_item = CCMenuItemSprite::create(n_collection, s_collection, this, menu_selector(WorldMapScene::menuAction));
	//	collection_item->setTag(kWMS_MT_collection);
	//
	//	CCMenu* collection_menu = CCMenu::createWithItem(collection_item);
	//	collection_menu->setPosition(getUiButtonPosition(kWMS_MT_collection));
	//	addChild(collection_menu, kWMS_Z_ui_button);
	
	
	CCSprite* n_option = CCSprite::create("worldmap_option.png");
	CCSprite* s_option = CCSprite::create("worldmap_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(WorldMapScene::menuAction));
	option_item->setTag(kWMS_MT_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(getUiButtonPosition(kWMS_MT_option));
	addChild(option_menu, kWMS_Z_ui_button);
	
	
	CCSprite* n_gacha = CCSprite::create("worldmap_gacha.png");
	CCSprite* s_gacha = CCSprite::create("worldmap_gacha.png");
	s_gacha->setColor(ccGRAY);
	
	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(WorldMapScene::menuAction));
	gacha_item->setTag(kWMS_MT_gacha);
	
	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
	gacha_menu->setPosition(getUiButtonPosition(kWMS_MT_gacha));
	addChild(gacha_menu, kWMS_Z_ui_button);
	
	
	CCSprite* n_rank = CCSprite::create("worldmap_rank.png");
	CCSprite* s_rank = CCSprite::create("worldmap_rank.png");
	s_rank->setColor(ccGRAY);
	
	CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(WorldMapScene::menuAction));
	rank_item->setTag(kWMS_MT_rank);
	
	CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
	rank_menu->setPosition(getUiButtonPosition(kWMS_MT_rank));
	addChild(rank_menu, kWMS_Z_ui_button);
	
	
	CCSprite* n_postbox = CCSprite::create("worldmap_postbox.png");
	CCSprite* s_postbox = CCSprite::create("worldmap_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(WorldMapScene::menuAction));
	postbox_item->setTag(kWMS_MT_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(getUiButtonPosition(kWMS_MT_postbox));
	addChild(postbox_menu, kWMS_Z_ui_button);
	
	
	CCSprite* n_event = CCSprite::create("worldmap_event.png");
	CCSprite* s_event = CCSprite::create("worldmap_event.png");
	s_event->setColor(ccGRAY);
	
	CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(WorldMapScene::menuAction));
	event_item->setTag(kWMS_MT_event);
	
	CCMenu* event_menu = CCMenu::createWithItem(event_item);
	event_menu->setPosition(getUiButtonPosition(kWMS_MT_event));
	addChild(event_menu, kWMS_Z_ui_button);
	
	
	CCSprite* n_ruby_shop = CCSprite::create("worldmap_shop.png");
	CCSprite* s_ruby_shop = CCSprite::create("worldmap_shop.png");
	s_ruby_shop->setColor(ccGRAY);
	
	CCMenuItem* ruby_shop_item = CCMenuItemSprite::create(n_ruby_shop, s_ruby_shop, this, menu_selector(WorldMapScene::menuAction));
	ruby_shop_item->setTag(kWMS_MT_rubyShop);
	
	CCMenu* ruby_shop_menu = CCMenu::createWithItem(ruby_shop_item);
	ruby_shop_menu->setPosition(getUiButtonPosition(kWMS_MT_rubyShop));
	addChild(ruby_shop_menu, kWMS_Z_ui_button);
	
	
	CCSprite* n_gold_shop = CCSprite::create("worldmap_shop.png");
	CCSprite* s_gold_shop = CCSprite::create("worldmap_shop.png");
	s_gold_shop->setColor(ccGRAY);
	
	CCMenuItem* gold_shop_item = CCMenuItemSprite::create(n_gold_shop, s_gold_shop, this, menu_selector(WorldMapScene::menuAction));
	gold_shop_item->setTag(kWMS_MT_goldShop);
	
	CCMenu* gold_shop_menu = CCMenu::createWithItem(gold_shop_item);
	gold_shop_menu->setPosition(getUiButtonPosition(kWMS_MT_goldShop));
	addChild(gold_shop_menu, kWMS_Z_ui_button);
	
	
	CCSprite* n_life_shop = CCSprite::create("worldmap_shop.png");
	CCSprite* s_life_shop = CCSprite::create("worldmap_shop.png");
	s_life_shop->setColor(ccGRAY);
	
	CCMenuItem* life_shop_item = CCMenuItemSprite::create(n_life_shop, s_life_shop, this, menu_selector(WorldMapScene::menuAction));
	life_shop_item->setTag(kWMS_MT_lifeShop);
	
	CCMenu* life_shop_menu = CCMenu::createWithItem(life_shop_item);
	life_shop_menu->setPosition(getUiButtonPosition(kWMS_MT_lifeShop));
	addChild(life_shop_menu, kWMS_Z_ui_button);
	
	setTouchEnabled(true);
	is_menu_enable = true;
	
	srand(time(NULL));
}

CCPoint WorldMapScene::getTrackPositionMove(string d)
{
	CCPoint return_value = CCPointZero;
	if(d == "R")		return_value = ccp(40.f,0.f);
	else if(d == "U")	return_value = ccp(0.f,40.f);
	else if(d == "L")	return_value = ccp(-40.f,0.f);
	else if(d == "D")	return_value = ccp(0.f,-40.f);
	return return_value;
}

int WorldMapScene::getRotateValue(string d)
{
	int return_value = 0;
	if(d == "R")		return_value = 0;
	else if(d == "U")	return_value = -90;
	else if(d == "L")	return_value = -180;
	else if(d == "D")	return_value = 90;
	return return_value;
}

int WorldMapScene::getTurnRotateValue(string d, string a_d)
{
	int return_value = 0;
	if(d == "R")
	{
		if(a_d == "U")		return_value = -180;
		else if(a_d == "D")	return_value = 90;
	}
	else if(d == "U")
	{
		if(a_d == "L")		return_value = 90;
		else if(a_d == "R")	return_value = 0;
	}
	else if(d == "L")
	{
		if(a_d == "U")		return_value = -90;
		else if(a_d == "D")	return_value = 0;
	}
	else if(d == "D")
	{
		if(a_d == "L")		return_value = -180;
		else if(a_d == "R")	return_value = -90;
	}
	
	return return_value;
}

CCPoint WorldMapScene::getUiButtonPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kWMS_MT_cardSetting)		return_value = ccp(50,63);
	else if(t_tag == kWMS_MT_option)		return_value = ccp(120,34);
	else if(t_tag == kWMS_MT_gacha)			return_value = ccp(175,34);
	else if(t_tag == kWMS_MT_rank)			return_value = ccp(230,34);
	else if(t_tag == kWMS_MT_postbox)		return_value = ccp(285,34);
	else if(t_tag == kWMS_MT_event)			return_value = ccp(420,34);
	else if(t_tag == kWMS_MT_rubyShop)		return_value = ccp(140,297);
	else if(t_tag == kWMS_MT_goldShop)		return_value = ccp(294,297);
	else if(t_tag == kWMS_MT_lifeShop)		return_value = ccp(448,297);
	
	else if(t_tag == kWMS_MT_collection)	return_value = ccp(120,92);
//	else if(t_tag == )				return_value = ;
	
	return return_value;
}

void WorldMapScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag < kWMS_MT_couponBase)
	{
		tag -= kWMS_MT_stageBase;
		mySD->setSilType(tag);
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedStage, tag);
		
//		StageInfoDown* t_sid = StageInfoDown::create(this, callfunc_selector(WorldMapScene::stageCancel));
//		addChild(t_sid, kWMS_Z_popup);
		
//		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
	else if(tag < kWMS_MT_option)
	{
		tag -= kWMS_MT_couponBase;
		pass_target_stage = tag;
		
		TicketNeedPopup* t_tnp = TicketNeedPopup::create(this, callfunc_selector(WorldMapScene::popupClose), this, callfunc_selector(WorldMapScene::passTicketStage));
		addChild(t_tnp, kWMS_Z_popup);
	}
	else if(tag == kWMS_MT_cardSetting)
	{
		mySGD->before_cardsetting = kSceneCode_WorldMapScene;
		CCDirector::sharedDirector()->replaceScene(CardSettingScene::scene());
	}
//	else if(tag == kWMS_MT_collection)
//	{
//		CCDirector::sharedDirector()->replaceScene(CollectionListScene::scene());
//	}
	else if(tag == kWMS_MT_option)
	{
//		CCDirector::sharedDirector()->replaceScene(OptionScene::scene());
	}
	else if(tag == kWMS_MT_gacha)
	{
		GachaPopup* t_gp = GachaPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
		addChild(t_gp, kWMS_Z_popup);
	}
	else if(tag == kWMS_MT_rank)
	{
		RankPopup* t_rp = RankPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
		addChild(t_rp, kWMS_Z_popup);
	}
	else if(tag == kWMS_MT_postbox)
	{
		PostboxPopup* t_pp = PostboxPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
		addChild(t_pp, kWMS_Z_popup);
	}
	else if(tag == kWMS_MT_event)
	{
		EventPopup* t_ep = EventPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
		addChild(t_ep, kWMS_Z_popup);
	}
	else if(tag == kWMS_MT_rubyShop)
	{
		RubyShopPopup* t_rsp = RubyShopPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
		addChild(t_rsp, kWMS_Z_popup);
	}
	else if(tag == kWMS_MT_goldShop)
	{
		RubyShopPopup* t_rsp = RubyShopPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
		addChild(t_rsp, kWMS_Z_popup);
	}
	else if(tag == kWMS_MT_lifeShop)
	{
		RubyShopPopup* t_rsp = RubyShopPopup::create(this, callfunc_selector(WorldMapScene::popupClose));
		addChild(t_rsp, kWMS_Z_popup);
	}
}

void WorldMapScene::popupClose()
{
	is_menu_enable = true;
}

void WorldMapScene::stageCancel()
{
	is_menu_enable = true;
}

CCPoint WorldMapScene::getCouponPosition(int stage)
{
	CCPoint return_value;
	
	return_value.x = SDS_GI(kSDF_gameInfo, CCSTR_CWF("coupon%d_x", stage)->getCString());
	return_value.y = SDS_GI(kSDF_gameInfo, CCSTR_CWF("coupon%d_y", stage)->getCString());
	
	return_value = ccpMult(return_value, 40.f);
	return_value = ccpAdd(return_value, ccp(20.f,20.f));
	
	return return_value;
}

CCPoint WorldMapScene::getStagePosition(int stage)
{
	CCPoint return_value;
	
	return_value.x = SDS_GI(kSDF_gameInfo, CCSTR_CWF("stage%d_x", stage)->getCString());
	return_value.y = SDS_GI(kSDF_gameInfo, CCSTR_CWF("stage%d_y", stage)->getCString());
	
	return_value = ccpMult(return_value, 40.f);
	return_value = ccpAdd(return_value, ccp(20.f,20.f));
	
//	if(stage == 1)			return_value = ccp(40,179);
//	else if(stage == 2)		return_value = ccp(84,232);
//	else if(stage == 3)		return_value = ccp(128,179);
//	else if(stage == 4)		return_value = ccp(172,124);
//	else if(stage == 5)		return_value = ccp(217,179);
//	else if(stage == 6)		return_value = ccp(261,232);
//	else if(stage == 7)		return_value = ccp(305,179);
//	else if(stage == 8)		return_value = ccp(349,124);
//	else if(stage == 9)		return_value = ccp(393,179);
//	else if(stage == 10)	return_value = ccp(439,232);
	
	return return_value;
}


void WorldMapScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, kCCMenuHandlerPriority+1);
}

#define WMS_SCROLL_SPEED_MAX_BASE	20
#define WMS_SCROLL_SPEED_DECEASE_BASE	0.2f

void WorldMapScene::moveListXY(CCPoint t_p)
{
	if(t_p.x > WMS_SCROLL_SPEED_MAX_BASE/map_node->getScale())		t_p.x = WMS_SCROLL_SPEED_MAX_BASE/map_node->getScale();
	if(t_p.x < -WMS_SCROLL_SPEED_MAX_BASE/map_node->getScale())		t_p.x = -WMS_SCROLL_SPEED_MAX_BASE/map_node->getScale();
	
	if(t_p.y > WMS_SCROLL_SPEED_MAX_BASE/map_node->getScale())		t_p.y = WMS_SCROLL_SPEED_MAX_BASE/map_node->getScale();
	if(t_p.y < -WMS_SCROLL_SPEED_MAX_BASE/map_node->getScale())		t_p.y = -WMS_SCROLL_SPEED_MAX_BASE/map_node->getScale();
	
	CCPoint a_p = ccpSub(map_node->getPosition(), t_p);
	
//	if(map_node->getScale() <= maximum_scale)
//		a_p.x = (480.f*maximum_scale-480.f*map_node->getScale())/2.f;
//	else
//	{
		if(a_p.x > -(left_x*map_node->getScale() - 240.f))
			a_p.x = -(left_x*map_node->getScale() - 240.f);
		else if(a_p.x < -(right_x*map_node->getScale() - 240.f))
			a_p.x = -(right_x*map_node->getScale() - 240.f);
//	}
	
	if(a_p.y > -(bottom_y*map_node->getScale() - 160.f))
		a_p.y = -(bottom_y*map_node->getScale() - 160.f);
	if(a_p.y < -(top_y*map_node->getScale() - 160.f))
		a_p.y = -(top_y*map_node->getScale() - 160.f);
	
	map_node->setPosition(a_p);
}

void WorldMapScene::moveAnimation()
{
	isAnimated = true;
	
	if(moveSpeed_p.x >= WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale())			moveSpeed_p.x -= WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale();
	else if(moveSpeed_p.x <= -WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale())	moveSpeed_p.x += WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale();
	else							moveSpeed_p.x = 0;
	
	if(moveSpeed_p.y >= WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale())			moveSpeed_p.y -= WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale();
	else if(moveSpeed_p.y <= WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale())	moveSpeed_p.y += WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale();
	else							moveSpeed_p.y = 0;
	
	if((moveSpeed_p.x < WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale() && moveSpeed_p.x > -WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale() &&
		moveSpeed_p.y < WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale() && moveSpeed_p.y > -WMS_SCROLL_SPEED_DECEASE_BASE/map_node->getScale()) || isAnimated == false)
	{
		this->unschedule(schedule_selector(WorldMapScene::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}

void WorldMapScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void WorldMapScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(WorldMapScene::alertAction));
}