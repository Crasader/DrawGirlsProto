//
//  ZoomScriptScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 6..
//
//

#include "ZoomScriptScene.h"

CCScene* ZoomScript::scene()
{
    CCScene *scene = CCScene::create();
    ZoomScript *layer = ZoomScript::create();
    scene->addChild(layer);
	
    return scene;
}

bool ZoomScript::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	
	
	return true;
}