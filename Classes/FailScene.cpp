//
//  FailScene.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#include "FailScene.h"
//#include "StartingScene.h"
#include "WorldMapScene.h"
#include "ScreenSide.h"
#include "StarGoldData.h"
#include "EnumDefine.h"
#include "StageSettingPopup.h"

typedef enum tMenuTagFailScene{
	kMT_FS_main = 1,
	kMT_FS_replay
}MenuTagFailScene;

typedef enum tZorderFailScene{
	kZ_FS_back = 1,
	kZ_FS_img,
	kZ_FS_menu,
	kZ_FS_popup
}ZorderFailScene;

CCScene* FailScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    FailScene *layer = FailScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FailScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	setKeypadEnabled(true);
	
	CCSprite* fail_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 480, 320));
	fail_back->setPosition(ccp(240,160));
	addChild(fail_back, kZ_FS_back);
	
	FailCode fail_code = mySGD->fail_code;
	
	if(fail_code == kFC_gameover)
	{
		CCSprite* title = CCSprite::create("ending_gameover.png");
		title->setPosition(ccp(240,210));
		addChild(title, kZ_FS_img);
	}
	else if(fail_code == kFC_timeover)
	{
		CCSprite* title = CCSprite::create("ending_timeover.png");
		title->setPosition(ccp(240,210));
		addChild(title, kZ_FS_img);
	}
	else if(fail_code == kFC_missionfail)
	{
		CCSprite* title = CCSprite::create("ending_missionfail.png");
		title->setPosition(ccp(240,250));
		addChild(title, kZ_FS_img);
		
		CLEAR_CONDITION clr_cdt = mySD->getClearCondition();
		
		CCSprite* fail_condition = CCSprite::create(CCString::createWithFormat("condition%d_menu.png", clr_cdt)->getCString());
		fail_condition->setPosition(ccp(240,170));
		addChild(fail_condition, kZ_FS_img);
		
		if(clr_cdt == kCLEAR_subCumberCatch)
		{
			int catch_cnt = mySD->getClearConditionCatchSubCumber();
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", catch_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccp(240,165));
			addChild(clr_cdt_label, kZ_FS_img);
		}
		else if(clr_cdt == kCLEAR_bigArea)
		{
			float clr_cdt_per = mySD->getClearConditionBigAreaPer();
			int clr_cdt_cnt = mySD->getClearConditionBigAreaCnt();
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%2.0f%%:%d", clr_cdt_per*100.f, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccp(240,165));
			addChild(clr_cdt_label, kZ_FS_img);
		}
		else if(clr_cdt == kCLEAR_itemCollect)
		{
			int collect_cnt = mySD->getClearConditionItemCollect();
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", collect_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccp(240,165));
			addChild(clr_cdt_label, kZ_FS_img);
		}
		else if(clr_cdt == kCLEAR_perfect)
		{
			float perfect_per = mySD->getClearConditionPerfectBase();
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f%%", perfect_per*100.f)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccp(240,165));
			addChild(clr_cdt_label, kZ_FS_img);
		}
	}
	
	CCSprite* n_main = CCSprite::create("ending_main.png");
	CCSprite* s_main = CCSprite::create("ending_main.png");
	s_main->setColor(ccGRAY);
	
	CCMenuItem* main_item = CCMenuItemSprite::create(n_main, s_main, this, menu_selector(FailScene::menuAction));
	main_item->setTag(kMT_FS_main);
	
	CCMenu* main_menu = CCMenu::createWithItem(main_item);
	main_menu->setPosition(ccp(130,70));
	addChild(main_menu, kZ_FS_menu);
	
	
	CCSprite* n_replay = CCSprite::create("ending_replay.png");
	CCSprite* s_replay = CCSprite::create("ending_replay.png");
	s_replay->setColor(ccGRAY);
	
	CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(FailScene::menuAction));
	replay_item->setTag(kMT_FS_replay);
	
	CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
	replay_menu->setPosition(ccp(350,70));
	addChild(replay_menu, kZ_FS_menu);
	
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void FailScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kMT_FS_main)
	{
		is_menu_enable = false;
//		CCDirector::sharedDirector()->replaceScene(StartingScene::scene());
		CCDirector::sharedDirector()->replaceScene(WorldMapScene::scene());
	}
	else if(tag == kMT_FS_replay)
	{
		is_menu_enable = false;
		StageSettingPopupLayer* t_sspl = StageSettingPopupLayer::create(mySD->getSilType(), this, callfunc_selector(FailScene::closeReplayPopup));
		addChild(t_sspl, kZ_FS_popup);
	}
}

void FailScene::closeReplayPopup()
{
	is_menu_enable = true;
}

void FailScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void FailScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(FailScene::alertAction));
}