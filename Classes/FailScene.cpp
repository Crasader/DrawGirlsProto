//
//  FailScene.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#include "FailScene.h"
#include "StartingScene.h"
#include "ScreenSide.h"

typedef enum tMenuTagFailScene{
	kMT_FS_ok = 1
}MenuTagFailScene;

typedef enum tZorderFailScene{
	kZ_FS_back = 1,
	kZ_FS_menu
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
	
	CCSprite* fail_back = CCSprite::create("fail_back.png");
	fail_back->setPosition(ccp(240,160));
	addChild(fail_back, kZ_FS_back);
	
	
	CCMenuItem* ok_item = CCMenuItemImage::create("ending_ok.png", "ending_ok.png", this, menu_selector(FailScene::menuAction));
	ok_item->setTag(kMT_FS_ok);
	
	CCMenu* ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setPosition(ccp(240,80));
	addChild(ok_menu, kZ_FS_menu);
	
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
	
	if(tag == kMT_FS_ok)
	{
		is_menu_enable = false;
		CCDirector::sharedDirector()->replaceScene(StartingScene::scene());
	}
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