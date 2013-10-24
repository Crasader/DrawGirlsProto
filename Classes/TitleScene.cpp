//
//  TitleScene.cpp
//  DGproto
//
//  Created by LitQoo on 13. 10. 22..
//
//

#include "TitleScene.h"

CCScene* TitleScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	TitleScene *layer = TitleScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	is_menu_enable = false;
	
	state_label = CCLabelTTF::create("까똑 로그인 ing...", mySGD->getFont().c_str(), 20);
	state_label->setPosition(ccp(240,290));
	addChild(state_label);
	
	Json::Value param;
	param["ManualLogin"] = true;
	
	hspConnector::get()->login(param, param, std::bind(&TitleScene::resultLogin, this, std::placeholders::_1));
	
	return true;
}