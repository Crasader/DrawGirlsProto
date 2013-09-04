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
	kMT_SS_start = 1
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
	
	GameData::sharedGameData()->resetGameData();
	
	CCSprite* start_back = CCSprite::create("start_back.png");
	start_back->setPosition(ccp(240,160));
	addChild(start_back, kZ_SS_back);
	
	
	CCMenuItem* start_item = CCMenuItemImage::create("start_start.png", "start_start.png", this, menu_selector(StartingScene::menuAction));
	start_item->setTag(kMT_SS_start);
	
	CCMenu* start_menu = CCMenu::createWithItem(start_item);
	start_menu->setPosition(ccp(240,80));
	addChild(start_menu, kZ_SS_menu);
	
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
	
	if(tag == kMT_SS_start)
	{
		is_menu_enable = false;
		
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(480, 320, kResolutionFixedWidth);
		
		StarGoldData::sharedInstance()->setGameStart();
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