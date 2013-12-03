//
//  MinsuScene.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 29..
//
//

#include "MinsuScene.h"
#include "DataStorageHub.h"
#include "StageImgLoader.h"
#include "cocos-ext.h"
#include "KSUtil.h"

USING_NS_CC_EXT;

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
	
	TempDownload down1;
	down1.url = "http://litqoo.com/dgproto/testcharacter.ccbi";
	down1.filename = "testcharacter.ccbi";
	download_list.push_back(down1);
	
	TempDownload down2;
	down2.url = "http://litqoo.com/dgproto/testcharacter.plist";
	down2.filename = "testcharacter.plist";
	download_list.push_back(down2);
	
	TempDownload down3;
	down3.url = "http://litqoo.com/dgproto/testcharacter.pvr.ccz";
	down3.filename = "testcharacter.pvr.ccz";
	download_list.push_back(down3);
	
	startAction();
	
	return true;
}

void MinsuScene::startAction()
{
	ing_download_cnt = 0;
	StageImgLoader::sharedInstance()->downloadImg(download_list[ing_download_cnt].url, 0, download_list[ing_download_cnt].filename, this, callfunc_selector(MinsuScene::successAction), this, callfunc_selector(MinsuScene::failAction));
}

void MinsuScene::successAction()
{
	ing_download_cnt++;
	
	if(ing_download_cnt < 3)
	{
		StageImgLoader::sharedInstance()->downloadImg(download_list[ing_download_cnt].url, 0, download_list[ing_download_cnt].filename, this, callfunc_selector(MinsuScene::successAction), this, callfunc_selector(MinsuScene::failAction));
	}
	else
	{
		CCLayer* testchar = KS::loadCCBI<CCLayer*>(this, "testcharacter.ccbi").first;
		testchar->setPosition(ccp(240,250));
		addChild(testchar);
	}
}

void MinsuScene::failAction()
{
	CCSprite* fail_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 100, 100));
	fail_img->setPosition(ccp(240,160));
	addChild(fail_img);
}