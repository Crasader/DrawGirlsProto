//
//  TutorialScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 30..
//
//

#include "TutorialScene.h"
#include "DataStorageHub.h"
#include "PuzzleMapScene.h"

CCScene* TutorialScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	TutorialScene *layer = TutorialScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

enum TutorialZorder{
	kTutorialZorder_back = 1,
	kTutorialZorder_menus
};

enum TutorialMenuTag{
	kTutorialMenuTag_cancel = 1
};

bool TutorialScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	
	is_menu_enable = false;
	
	CCSprite* back_img = CCSprite::create("tutorial_back.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kTutorialZorder_back);
	
	
	CCSprite* n_cancel = CCSprite::create("tutorial_common_cancel.png");
	CCSprite* s_cancel = CCSprite::create("tutorial_common_cancel.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(TutorialScene::menuAction));
	cancel_item->setTag(kTutorialMenuTag_cancel);
	
	CCMenu* cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(getContentPosition(kTutorialMenuTag_cancel));
	addChild(cancel_menu, kTutorialZorder_menus);
	
	
	
	
	is_menu_enable = true;

	return true;
}

void TutorialScene::menuAction(CCObject *sender)
{
	if(!is_menu_enable)		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kTutorialMenuTag_cancel)
	{
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
}

CCPoint TutorialScene::getContentPosition(int t_tag)
{
	CCPoint rv;
	
	if(t_tag == kTutorialMenuTag_cancel)
		rv = ccp(372,292);
	
	return rv;
}