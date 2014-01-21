//
//  LoadingTipScene.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 1. 21..
//
//

#include "LoadingTipScene.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "ServerDataSave.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "StageImgLoader.h"
#include "SilhouetteData.h"
#include "MaingameScene.h"

CCScene* LoadingTipScene::scene()
{
    CCScene *scene = CCScene::create();
    
    LoadingTipScene *layer = LoadingTipScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    scene->addChild(layer);
	
    return scene;
}

enum LoadingTipZorder{
	kLoadingTipZorder_back = 1,
	kLoadingTipZorder_content,
	kLoadingTipZorder_popup
};

bool LoadingTipScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	
	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kLoadingTipZorder_back);
	
	next_scene_name = mySGD->getNextSceneName();
	
	CCLabelTTF* test_label = CCLabelTTF::create(CCString::createWithFormat("로딩팁이랍니다.\n임시화면입니다.\n진짜랍니다.\n%s로 이동하는 중입니다.", next_scene_name.c_str())->getCString(), mySGD->getFont().c_str(), 20);
	test_label->setPosition(ccp(240,190));
	addChild(test_label, kLoadingTipZorder_content);
	
	readyLoading();
		
	return true;
}

void LoadingTipScene::readyLoading()
{
	if(next_scene_name == "maingame")
	{
		sil_load_list.push_back(CCString::createWithFormat("stage%d_level1_visible.png", mySD->getSilType())->getCString());
		sil_load_list.push_back(CCString::createWithFormat("stage%d_level1_invisible.png", mySD->getSilType())->getCString());
		
		default_load_list.push_back("top_back.png");
		default_load_list.push_back("bottom_back.png");
		default_load_list.push_back("normal_frame_top.png");
		default_load_list.push_back("normal_frame_bottom.png");
		default_load_list.push_back("normal_frame_left.png");
		default_load_list.push_back("normal_frame_right.png");
		
		total_load_img = sil_load_list.size() + default_load_list.size();
		ing_load_img = 0;
		
		progress_label = CCLabelBMFont::create(CCString::createWithFormat("%d    /    %d", ing_load_img, total_load_img)->getCString(), "etc_font.fnt");
		progress_label->setPosition(ccp(240,130));
		addChild(progress_label, kLoadingTipZorder_content);
	}
	
	
	
	startLoading();
}

void LoadingTipScene::startLoading()
{
	for(int i=0;i<total_load_img;i++)
	{
		if(i < sil_load_list.size())
			mySIL->addImageAsync(sil_load_list[ing_load_img].c_str(), this, callfuncO_selector(LoadingTipScene::countingFunc));
		else
			CCTextureCache::sharedTextureCache()->addImageAsync(default_load_list[i-sil_load_list.size()].c_str(), this, callfuncO_selector(LoadingTipScene::countingFunc));
	}
}

void LoadingTipScene::countingFunc(CCObject *sender)
{
	ing_load_img++;
	progress_label->setString(CCString::createWithFormat("%d\t%d", ing_load_img, total_load_img)->getCString());
	
	if(ing_load_img == total_load_img)
	{
		if(next_scene_name == "maingame")
			CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	}
}

void LoadingTipScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void LoadingTipScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(LoadingTipScene::alertAction));
}