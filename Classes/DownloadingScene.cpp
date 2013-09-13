//
//  DownloadingScene.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 14..
//
//

#include "DownloadingScene.h"
#include "SelectedMapData.h"
#include "AlertEngine.h"
#include "AudioEngine.h"
#include "StarGoldData.h"

CCScene* Downloading::scene()
{
    CCScene *scene = CCScene::create();
    Downloading *layer = Downloading::create();
    scene->addChild(layer);
	
    return scene;
}

bool Downloading::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	isAction = false;
	
	chapter_bonusImg_number = SelectedMapData::sharedInstance()->getImageNumber(kIN_bonusImage);
	my_si = ScanningImg::create(CCString::createWithFormat("c%d_s5_invisible.png", chapter_bonusImg_number)->getCString());
	my_si->setPosition(ccp(160,240));
	addChild(my_si);
	
	my_scanner = CCSprite::create("down_img_scanner.png");
	my_scanner->setPosition(ccp(160,430));
	addChild(my_scanner);
	
	loading_ment = CCSprite::create("img_downloading.png");
	loading_ment->setPosition(ccp(245,20));
	addChild(loading_ment);
	
	StageImgLoader::sharedInstance()->downloadImgForCS(chapter_bonusImg_number, 5, this, callfunc_selector(Downloading::successAction), this, callfunc_selector(Downloading::failAction));
	
	is_successed = false;
	
	return true;
}

void Downloading::successAction()
{
	if(!isAction)
	{
		isAction = true;
		my_si->setPercentage(1.f);
		my_scanner->setPositionY(0);
		unschedule(schedule_selector(Downloading::ingFrame));
		
		SuccessfulImg* t_si = SuccessfulImg::create(chapter_bonusImg_number, this, callfunc_selector(Downloading::realSuccess));
		addChild(t_si);
	}
}

void Downloading::realSuccess()
{
	is_successed = true;
	closeShutter();
	
}

void Downloading::closeShutter()
{
	AudioEngine::sharedInstance()->playEffect("sound_shuttermove_start.m4a", false);
	
	CCSprite* bottom = CCSprite::create("loading_bottom.png");
	bottom->setAnchorPoint(ccp(0.5,1));
	bottom->setPosition(ccp(160,-10));
	addChild(bottom);
	
	CCSprite* top = CCSprite::create("loading_top.png");
	top->setAnchorPoint(ccp(0.5,0));
	top->setPosition(ccp(160,490));
	addChild(top);
	
	CCMoveTo* bottom_move = CCMoveTo::create(0.5f, ccp(160,240));
	CCMoveTo* top_move = CCMoveTo::create(0.5f, ccp(160,240));
	CCCallFunc* top_sound = CCCallFunc::create(this, callfunc_selector(Downloading::shutterClosedSound));
	CCDelayTime* top_delay = CCDelayTime::create(0.1f);
	CCCallFunc* top_call = CCCallFunc::create(this, callfunc_selector(Downloading::endCloseShutter));
	CCAction* top_seq = CCSequence::create(top_move, top_sound, top_delay, top_call, NULL);
	
	bottom->runAction(bottom_move);
	top->runAction(top_seq);
}

void Downloading::shutterClosedSound()
{
	AudioEngine::sharedInstance()->playEffect("sound_shutter_closed.m4a", false);
}

void Downloading::endCloseShutter()
{
	if(is_successed)
	{
		myReplaceScene(StageImgLoader::sharedInstance()->getSuccessSceneCode());
	}
	else
	{
		myReplaceScene(StageImgLoader::sharedInstance()->getFailSceneCode());
	}
}

void Downloading::failAction()
{
	if(!isAction)
	{
		isAction = true;
		unschedule(schedule_selector(Downloading::ingFrame));
		AlertEngine::sharedInstance()->addSingleAlert("error", "failed image download", "Ok", 1, this, alertfuncII_selector(Downloading::alertAction));
	}
}

void Downloading::realFail()
{
	is_successed = false;
	closeShutter();
}

void Downloading::alertAction(int t1, int t2)
{
	if(t1 == 1)
	{
		realFail();
	}
}

void Downloading::ingFrame()
{
	float per = StageImgLoader::sharedInstance()->getDownloadPercentage();
	my_si->setPercentage(per);
	my_scanner->setPositionY(430.f-(per > 1.f ? 1.f : per)*430.f);
	
	if(per >= 0.999f)
	{
		unschedule(schedule_selector(Downloading::ingFrame));
	}
}

void Downloading::myReplaceScene(DownAfterScene a_code)
{
//	if(a_code == kDAS_ChapterSetting)
//		mySGD->setAfterScene(kSceneCode_ChapterSetting);
//	else if(a_code == kDAS_StageSetting)
//		mySGD->setAfterScene(kSceneCode_StageSetting);
//	else if(a_code == kDAS_Gallery)
//		mySGD->setAfterScene(kSceneCode_Gallery);
//	else if(a_code == kDAS_WorldMapScene)
//		mySGD->setAfterScene(kSceneCode_WorldMapScene);
//	
//	CCDirector::sharedDirector()->replaceScene(NewLoading::scene());
}

void Downloading::onEnter()
{
	CCLayer::onEnter();
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	
	schedule(schedule_selector(Downloading::ingFrame));
}

void Downloading::onExit()
{
	CCLayer::onExit();
}