//
//  CollectionListScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 20..
//
//

#include "CollectionListScene.h"
#include "ScreenSide.h"
#include "MyLocalization.h"
#include "WorldMapScene.h"


CCScene* CollectionListScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CollectionListScene *layer = CollectionListScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

enum CLS_Zorder{
	kCLS_Z_back = 1,
	kCLS_Z_content
};

enum CLS_MenuTag{
	kCLS_MT_close = 1,
	kCLS_MT_cardBase = 100
};

// on "init" you need to initialize your instance
bool CollectionListScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	setKeypadEnabled(true);
	
	CCSprite* collectionlist_back = CCSprite::create("collectionlist_back.png");
	collectionlist_back->setPosition(ccp(240,160));
	addChild(collectionlist_back, kCLS_Z_back);
	
	CCMenuItem* close_item = CCMenuItemImage::create("sspl_cancel.png", "sspl_cancel.png", this, menu_selector(CollectionListScene::menuAction));
	close_item->setTag(kCLS_MT_close);
	
	close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(ccp(430,290));
	addChild(close_menu, kCLS_Z_content);
	
	
	collection_view = CollectionView::create();
	addChild(collection_view, kCLS_Z_content);
	
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

void CollectionListScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kCLS_MT_close)
	{
		CCDirector::sharedDirector()->replaceScene(WorldMapScene::scene());
	}
}

void CollectionListScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void CollectionListScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(CollectionListScene::alertAction));
}