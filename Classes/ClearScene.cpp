//
//  ClearScene.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#include "ClearScene.h"
//#include "StartingScene.h"
#include "WorldMapScene.h"
#include "ScreenSide.h"

typedef enum tMenuTagClearScene{
	kMT_CS_ok = 1
}MenuTagClearScene;

typedef enum tZorderClearScene{
	kZ_CS_back = 1,
	kZ_CS_menu
}ZorderClearScene;

CCScene* ClearScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ClearScene *layer = ClearScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ClearScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
    
	CCSprite* clear_back = CCSprite::create("clear_back.png");
	clear_back->setPosition(ccp(240,160));
	addChild(clear_back, kZ_CS_back);
	
	
	CCMenuItem* ok_item = CCMenuItemImage::create("ending_ok.png", "ending_ok.png", this, menu_selector(ClearScene::menuAction));
	ok_item->setTag(kMT_CS_ok);
	
	CCMenu* ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setPosition(ccp(240,80));
	addChild(ok_menu, kZ_CS_menu);
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void ClearScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kMT_CS_ok)
	{
		is_menu_enable = false;
//		CCDirector::sharedDirector()->replaceScene(StartingScene::scene());
		CCDirector::sharedDirector()->replaceScene(WorldMapScene::scene());
	}
}

void ClearScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void ClearScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(ClearScene::alertAction));
}