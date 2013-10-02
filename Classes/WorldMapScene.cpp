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
#include "StageInfoDown.h"
#include "GraphDog.h"
#include "OptionScene.h"
#include "GachaPopup.h"
#include "RankPopup.h"
#include "PostboxPopup.h"
#include "EventPopup.h"
#include "RubyShopPopup.h"
#include "CardSettingScene.h"

#include <algorithm>
#include <ostream>

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
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

enum WMS_Zorder{
	kWMS_Z_back = 1,
	kWMS_Z_stage,
	kWMS_Z_ui_button,
	kWMS_Z_popup
};

enum WMS_MenuTag{
	kWMS_MT_stageBase = 0,
	kWMS_MT_collection = 501,
	kWMS_MT_option = 502,
	kWMS_MT_gacha = 503,
	kWMS_MT_rank = 504,
	kWMS_MT_postbox = 505,
	kWMS_MT_event = 506,
	kWMS_MT_cardSetting = 507,
	kWMS_MT_rubyShop = 508,
	kWMS_MT_goldShop = 509,
	kWMS_MT_lifeShop = 510
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
	
	graphdog->setup("12345678", 1);
	
//	showConvertSildata("intun1");
//	showConvertSildata("intun2");
	
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
	
	CCSprite* n_collection = CCSprite::create("worldmap_collection.png");
	CCSprite* s_collection = CCSprite::create("worldmap_collection.png");
	s_collection->setColor(ccGRAY);
	
	CCMenuItem* collection_item = CCMenuItemSprite::create(n_collection, s_collection, this, menu_selector(WorldMapScene::menuAction));
	collection_item->setTag(kWMS_MT_collection);
	
	CCMenu* collection_menu = CCMenu::createWithItem(collection_item);
	collection_menu->setPosition(getUiButtonPosition(kWMS_MT_collection));
	addChild(collection_menu, kWMS_Z_ui_button);
	
	
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
	
	is_menu_enable = true;
	
	srand(time(NULL));
	
	//////////////////////////////////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
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
	
	if(tag < kWMS_MT_collection)
	{
		tag -= kWMS_MT_stageBase;
		mySD->setSilType(tag);
		
		StageInfoDown* t_sid = StageInfoDown::create(this, callfunc_selector(WorldMapScene::stageCancel));
		addChild(t_sid, kWMS_Z_popup);
		
//		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
	else if(tag == kWMS_MT_cardSetting)
	{
		mySGD->before_cardsetting = kSceneCode_WorldMapScene;
		CCDirector::sharedDirector()->replaceScene(CardSettingScene::scene());
	}
	else if(tag == kWMS_MT_collection)
	{
		CCDirector::sharedDirector()->replaceScene(CollectionListScene::scene());
	}
	else if(tag == kWMS_MT_option)
	{
		CCDirector::sharedDirector()->replaceScene(OptionScene::scene());
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