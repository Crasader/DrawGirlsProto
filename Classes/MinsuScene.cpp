//
//  MinsuScene.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 29..
//
//

#include "MinsuScene.h"
#include "DataStorageHub.h"

CCScene* MinsuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MinsuScene *layer = MinsuScene::create();
	
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

bool MinsuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	
	return true;
}