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
	
	next_scene_name = mySGD->getNextSceneName();
	
	CCSprite* tip_img = getLoadingTipImage();
	tip_img->setPosition(ccp(240,160));
	addChild(tip_img, kLoadingTipZorder_back);
	
	readyLoading();
		
	return true;
}

CCSprite* LoadingTipScene::getLoadingTipImage()
{
	int total_loading_tip = 14;
	int selected_loading_tip = rand()%total_loading_tip;
	
	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
	string tip_filename = "loading_tip_";
	if(selected_loading_tip == 0)
		tip_filename += "achievement";
	else if(selected_loading_tip == 1)
		tip_filename += "bonusgame";
	else if(selected_loading_tip == 2)
		tip_filename += "bosstip1";
	else if(selected_loading_tip == 3)
		tip_filename += "bosstip2";
	else if(selected_loading_tip == 4)
		tip_filename += "cardsetting";
	else if(selected_loading_tip == 5)
		tip_filename += "challenge";
	else if(selected_loading_tip == 6)
		tip_filename += "change";
	else if(selected_loading_tip == 7)
		tip_filename += "clear";
	else if(selected_loading_tip == 8)
		tip_filename += "continue";
	else if(selected_loading_tip == 9)
		tip_filename += "newitem1";
	else if(selected_loading_tip == 10)
		tip_filename += "newitem2";
	else if(selected_loading_tip == 11)
		tip_filename += "newpuzzle";
	else if(selected_loading_tip == 12)
		tip_filename += "option";
	else if(selected_loading_tip == 13)
		tip_filename += "shop";
	else
		tip_filename += "bonusgame";
	
	tip_filename += ".png";
	
	CCSprite* content_img = CCSprite::create(tip_filename.c_str());
	content_img->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f));
	loading_tip_back->addChild(content_img);
	
	return loading_tip_back;
}

void LoadingTipScene::readyLoading()
{
	if(next_scene_name == "maingame")
	{
		sil_load_list.push_back(CCString::createWithFormat("card%d_visible.png", NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))->getCString());
		sil_load_list.push_back(CCString::createWithFormat("card%d_invisible.png", NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))->getCString());
		
		default_load_list.push_back("top_back.png");
		default_load_list.push_back("bottom_back.png");
		default_load_list.push_back("normal_frame_top.png");
		default_load_list.push_back("normal_frame_bottom.png");
		default_load_list.push_back("normal_frame_left.png");
		default_load_list.push_back("normal_frame_right.png");
		
		total_load_img = sil_load_list.size() + default_load_list.size();
		ing_load_img = 0;
		
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
		loading_progress_img->setPosition(ccp(240,50));
		addChild(loading_progress_img, kLoadingTipZorder_content);
		reader->release();
		
		progress_label = CCLabelBMFont::create(CCString::createWithFormat("%.0f", (100.f*ing_load_img)/total_load_img)->getCString(), "etc_font.fnt");
		progress_label->setPosition(ccp(240,50));
		addChild(progress_label, kLoadingTipZorder_content);
	}
	
	is_minimum_time = false;
	CCDelayTime* t_delay = CCDelayTime::create(1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(LoadingTipScene::onMinimumTime));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	runAction(t_seq);
	
	startLoading();
}

void LoadingTipScene::onMinimumTime()
{
	is_minimum_time = true;
	if(ing_load_img == total_load_img)
	{
		is_minimum_time = false;
		if(next_scene_name == "maingame")
			CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	}
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
	
	if(ing_load_img == total_load_img && is_minimum_time)
	{
		is_minimum_time = false;
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