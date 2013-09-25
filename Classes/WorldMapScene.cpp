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
#include "CollectionListScene.h"

#include <algorithm>

using namespace std;

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
				
				result_data.append(to_string(cmp_cnt));
				cmp_cnt = 1;
				is_zero = false;
			}
			else
			{
				CCLog("invalid data %s", convert_data.substr(i,1).c_str());
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
				result_data.append(to_string(cmp_cnt));
				cmp_cnt = 1;
				is_zero = true;
			}
			else
			{
				CCLog("invalid data %s", convert_data.substr(i,1).c_str());
			}
		}
	}
	
	if(cmp_cnt >= 10000)		result_data.append("D");
	else if(cmp_cnt >= 1000)	result_data.append("C");
	else if(cmp_cnt >= 100)		result_data.append("B");
	else if(cmp_cnt >= 10)		result_data.append("A");
	result_data.append(to_string(cmp_cnt));
	
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
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

enum WMS_Zorder{
	kWMS_Z_back = 1,
	kWMS_Z_stage,
	kWMS_Z_collection,
	kWMS_Z_popup
};

enum WMS_MenuTag{
	kWMS_MT_stageBase = 0,
	kWMS_MT_collection = 501,
	kWMS_MT_uiSettingLeft = 1001,
	kWMS_MT_uiSettingFull = 1002,
	kWMS_MT_uiSettingRight = 1003
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
	
//	showConvertSildata("stage3_level1");
//	showConvertSildata("stage3_level2");
//	showConvertSildata("stage5_level1");
//	showConvertSildata("stage5_level2");
//	showConvertSildata("stage6_level1");
//	showConvertSildata("stage6_level2");
//	showConvertSildata("stage7_level1");
//	showConvertSildata("stage7_level2");
//	showConvertSildata("stage8_level1");
//	showConvertSildata("stage8_level2");
	
	setKeypadEnabled(true);
	
	CCSprite* worldmap_back = CCSprite::create("worldmap_back.png");
	worldmap_back->setPosition(ccp(240,160));
	addChild(worldmap_back, kWMS_Z_back);
	
	
	CCSprite* n_stage1 = CCSprite::create("worldmap_stage.png");
	CCLabelTTF* n_stage1_label = CCLabelTTF::create("1", mySGD->getFont().c_str(), 10);
	n_stage1_label->setPosition(ccp(n_stage1->getContentSize().width/2.f, n_stage1->getContentSize().height/2.f));
	n_stage1->addChild(n_stage1_label);
	
	CCSprite* s_stage1 = CCSprite::create("worldmap_stage.png");
	s_stage1->setColor(ccGRAY);
	CCLabelTTF* s_stage1_label = CCLabelTTF::create("1", mySGD->getFont().c_str(), 10);
	s_stage1_label->setPosition(ccp(s_stage1->getContentSize().width/2.f, s_stage1->getContentSize().height/2.f));
	s_stage1->addChild(s_stage1_label);
	
	
	CCMenuItem* stage1_item = CCMenuItemSprite::create(n_stage1, s_stage1, this, menu_selector(WorldMapScene::menuAction));
	stage1_item->setTag(kWMS_MT_stageBase + 1);
	
	CCMenu* stage1_menu = CCMenu::createWithItem(stage1_item);
	stage1_menu->setPosition(getStagePosition(1));
	addChild(stage1_menu, kWMS_Z_stage);
	
	CCSprite* stage1_thumbnail = CCSprite::create("stage1_thumbnail.png");
	stage1_thumbnail->setPosition(ccpAdd(getStagePosition(1), ccp(0,40)));
	addChild(stage1_thumbnail, kWMS_Z_stage);
	
	int cleared_number = DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_theme_int1_clearednumber, 1);
	int updated_stage = mySD->getLastUpdateStageNumber();
	
	for(int i=2;i<=10;i++)
	{
		if(updated_stage >= i && i-1 <= cleared_number)
		{
			CCSprite* n_stage = CCSprite::create("worldmap_stage.png");
			CCLabelTTF* n_stage_label = CCLabelTTF::create(CCString::createWithFormat("%d", i)->getCString(), mySGD->getFont().c_str(), 10);
			n_stage_label->setPosition(ccp(n_stage->getContentSize().width/2.f, n_stage->getContentSize().height/2.f));
			n_stage->addChild(n_stage_label);
			
			CCSprite* s_stage = CCSprite::create("worldmap_stage.png");
			s_stage->setColor(ccGRAY);
			CCLabelTTF* s_stage_label = CCLabelTTF::create(CCString::createWithFormat("%d", i)->getCString(), mySGD->getFont().c_str(), 10);
			s_stage_label->setPosition(ccp(s_stage->getContentSize().width/2.f, s_stage->getContentSize().height/2.f));
			s_stage->addChild(s_stage_label);
			
			
			CCMenuItem* stage_item = CCMenuItemSprite::create(n_stage, s_stage, this, menu_selector(WorldMapScene::menuAction));
			stage_item->setTag(kWMS_MT_stageBase+i);
			
			CCMenu* stage_menu = CCMenu::createWithItem(stage_item);
			stage_menu->setPosition(getStagePosition(i));
			addChild(stage_menu, kWMS_Z_stage);
		}
		else
		{
			CCSprite* d_stage = CCSprite::create("worldmap_gray.png");
			CCLabelTTF* d_stage_label = CCLabelTTF::create(CCString::createWithFormat("%d", i)->getCString(), mySGD->getFont().c_str(), 10);
			d_stage_label->setPosition(ccp(d_stage->getContentSize().width/2.f, d_stage->getContentSize().height/2.f));
			d_stage->addChild(d_stage_label);
			
			d_stage->setPosition(getStagePosition(i));
			addChild(d_stage, kWMS_Z_stage);
		}
		
		if(updated_stage >= i && i-2 <= cleared_number)
		{
			if(i-1 <= cleared_number)
			{
				CCSprite* stage_thumbnail = CCSprite::create(CCString::createWithFormat("stage%d_thumbnail.png", i)->getCString());
				stage_thumbnail->setPosition(ccpAdd(getStagePosition(i), ccp(0,40)));
				addChild(stage_thumbnail, kWMS_Z_stage);
			}
			else
			{
				CCSprite* stage_thumbnail = CCSprite::create("question_thumbnail.png");
				stage_thumbnail->setPosition(ccpAdd(getStagePosition(i), ccp(0,40)));
				addChild(stage_thumbnail, kWMS_Z_stage);
			}
		}
	}
	
	
	CCSprite* n_collection = CCSprite::create("worldmap_collection.png");
	CCSprite* s_collection = CCSprite::create("worldmap_collection.png");
	s_collection->setColor(ccGRAY);
	
	CCMenuItem* collection_item = CCMenuItemSprite::create(n_collection, s_collection, this, menu_selector(WorldMapScene::menuAction));
	collection_item->setTag(kWMS_MT_collection);
	
	CCMenu* collection_menu = CCMenu::createWithItem(collection_item);
	collection_menu->setPosition(ccp(120,34));
	addChild(collection_menu, kWMS_Z_collection);
	
	
	CCSprite* n_left = CCSprite::create("ui_setting_left.png");
	CCSprite* s_left = CCSprite::create("ui_setting_left.png");
	s_left->setColor(ccGRAY);
	
	CCMenuItem* left_item = CCMenuItemSprite::create(n_left, s_left, this, menu_selector(WorldMapScene::menuAction));
	left_item->setTag(kWMS_MT_uiSettingLeft);
	
	CCMenu* left_menu = CCMenu::createWithItem(left_item);
	left_menu->setPosition(ccp(320,40));
	addChild(left_menu, kWMS_Z_stage);
	
	
	CCSprite* n_full = CCSprite::create("ui_setting_full.png");
	CCSprite* s_full = CCSprite::create("ui_setting_full.png");
	s_full->setColor(ccGRAY);
	
	CCMenuItem* full_item = CCMenuItemSprite::create(n_full, s_full, this, menu_selector(WorldMapScene::menuAction));
	full_item->setTag(kWMS_MT_uiSettingFull);
	
	CCMenu* full_menu = CCMenu::createWithItem(full_item);
	full_menu->setPosition(ccp(377,40));
	addChild(full_menu, kWMS_Z_stage);
	
	
	CCSprite* n_right = CCSprite::create("ui_setting_right.png");
	CCSprite* s_right = CCSprite::create("ui_setting_right.png");
	s_right->setColor(ccGRAY);
	
	CCMenuItem* right_item = CCMenuItemSprite::create(n_right, s_right, this, menu_selector(WorldMapScene::menuAction));
	right_item->setTag(kWMS_MT_uiSettingRight);
	
	CCMenu* right_menu = CCMenu::createWithItem(right_item);
	right_menu->setPosition(ccp(434,40));
	addChild(right_menu, kWMS_Z_stage);
	
	is_menu_enable = true;
	
	srand(time(NULL));
	
	int selected_chapter = rand()%38+1;
	if(selected_chapter > 26)
	{
		selected_chapter += 4;
	}
	if(selected_chapter > 36)
	{
		selected_chapter += 4;
	}
	
	SelectedMapData::sharedInstance()->setSelectedChapter(selected_chapter);
	SelectedMapData::sharedInstance()->setSelectedStage(5);
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void WorldMapScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag < kWMS_MT_collection)
	{
		tag -= kWMS_MT_stageBase;
		mySD->setSilType(tag);
		
		
		
		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
	else if(tag == kWMS_MT_collection)
	{
		CCDirector::sharedDirector()->replaceScene(CollectionListScene::scene());
	}
	else if(tag == kWMS_MT_uiSettingLeft)
	{
		myGD->setUItype(kGT_leftUI);
		is_menu_enable = true;
	}
	else if(tag == kWMS_MT_uiSettingFull)
	{
		myGD->setUItype(kGT_full);
		is_menu_enable = true;
	}
	else if(tag == kWMS_MT_uiSettingRight)
	{
		myGD->setUItype(kGT_rightUI);
		is_menu_enable = true;
	}
}

void WorldMapScene::stageCancel()
{
	is_menu_enable = true;
}

CCPoint WorldMapScene::getStagePosition(int stage)
{
	CCPoint return_value;
	if(stage == 1)			return_value = ccp(40,179);
	else if(stage == 2)		return_value = ccp(84,232);
	else if(stage == 3)		return_value = ccp(128,179);
	else if(stage == 4)		return_value = ccp(172,124);
	else if(stage == 5)		return_value = ccp(217,179);
	else if(stage == 6)		return_value = ccp(261,232);
	else if(stage == 7)		return_value = ccp(305,179);
	else if(stage == 8)		return_value = ccp(349,124);
	else if(stage == 9)		return_value = ccp(393,179);
	else if(stage == 10)	return_value = ccp(439,232);
	
	return return_value;
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