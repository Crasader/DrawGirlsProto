//
//  StartingScene.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#include "StartingScene.h"
#include "MaingameScene.h"
#include "StarGoldData.h"
#include "GameData.h"
#include "AudioEngine.h"
#include "ScreenSide.h"

typedef enum tMenuTagStartingScene{
	kMT_SS_summer = 1,
	kMT_SS_christmas,
	kMT_SS_hospital,
	kMT_SS_sports,
	kMT_SS_idol
}MenuTagStartingScene;

typedef enum tZorderStartingScene{
	kZ_SS_back = 1,
	kZ_SS_menu
}ZorderStartingScene;

CCScene* StartingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StartingScene *layer = StartingScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	setKeypadEnabled(true);
	
	myGD->resetGameData();
	
	CCSprite* start_back = CCSprite::create("start_back.png");
	start_back->setPosition(ccp(240,160));
	addChild(start_back, kZ_SS_back);
	
	
	CCMenuItem* summer_item = CCMenuItemImage::create("start_summer.png", "start_summer.png", this, menu_selector(StartingScene::menuAction));
	summer_item->setTag(kMT_SS_summer);
	
	CCMenu* summer_menu = CCMenu::createWithItem(summer_item);
	summer_menu->setPosition(ccp(75,170));
	addChild(summer_menu, kZ_SS_menu);
	
	
	CCMenuItem* christmas_item = CCMenuItemImage::create("start_christmas.png", "start_christmas.png", this, menu_selector(StartingScene::menuAction));
	christmas_item->setTag(kMT_SS_christmas);
	
	CCMenu* christmas_menu = CCMenu::createWithItem(christmas_item);
	christmas_menu->setPosition(ccp(175,170));
	addChild(christmas_menu, kZ_SS_menu);
	
	
	CCMenuItem* hospital_item = CCMenuItemImage::create("start_hospital.png", "start_hospital.png", this, menu_selector(StartingScene::menuAction));
	hospital_item->setTag(kMT_SS_hospital);
	
	CCMenu* hospital_menu = CCMenu::createWithItem(hospital_item);
	hospital_menu->setPosition(ccp(275,170));
	addChild(hospital_menu, kZ_SS_menu);
	
	
	CCMenuItem* sports_item = CCMenuItemImage::create("start_sports.png", "start_sports.png", this, menu_selector(StartingScene::menuAction));
	sports_item->setTag(kMT_SS_sports);
	
	CCMenu* sports_menu = CCMenu::createWithItem(sports_item);
	sports_menu->setPosition(ccp(75,65));
	addChild(sports_menu, kZ_SS_menu);
	
	
	CCMenuItem* idol_item = CCMenuItemImage::create("start_idol.png", "start_idol.png", this, menu_selector(StartingScene::menuAction));
	idol_item->setTag(kMT_SS_idol);
	
	CCMenu* idol_menu = CCMenu::createWithItem(idol_item);
	idol_menu->setPosition(ccp(175,65));
	addChild(idol_menu, kZ_SS_menu);
	
	CCSprite* update2 = CCSprite::create("start_update.png");
	update2->setPosition(ccp(275,65));
	addChild(update2, kZ_SS_menu);
	
	
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
	
	AudioEngine::sharedInstance()->preloadEffectScene("Title");
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void StartingScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kMT_SS_summer)
	{
		is_menu_enable = false;
		
		mySD->setSilType(1);
		
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(480, 320, kResolutionFixedWidth);
		
		mySGD->setGameStart();
		CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	}
	else if(tag == kMT_SS_christmas)
	{
		is_menu_enable = false;
		
		mySD->setSilType(2);
		
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(480, 320, kResolutionFixedWidth);
		
		mySGD->setGameStart();
		CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	}
	else if(tag == kMT_SS_hospital)
	{
		is_menu_enable = false;
		
		mySD->setSilType(3);
		
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(480, 320, kResolutionFixedWidth);
		
		mySGD->setGameStart();
		CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	}
	else if(tag == kMT_SS_sports)
	{
		is_menu_enable = false;
		
		mySD->setSilType(4);
		
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(480, 320, kResolutionFixedWidth);
		
		mySGD->setGameStart();
		CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	}
	else if(tag == kMT_SS_idol)
	{
		is_menu_enable = false;
		
		mySD->setSilType(5);
		
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(480, 320, kResolutionFixedWidth);
		
		mySGD->setGameStart();
		CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	}
}

void StartingScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void StartingScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(StartingScene::alertAction));
}