//
//  ShowTimeScene.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 3. 8..
//
//

#include "ShowTimeScene.h"
#include "SelectedMapData.h"
#include "StarGoldData.h"
#include "ClearScene.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "StageImgLoader.h"

using namespace cocos2d;

#define ST_SCROLL_SPEED_MAX_BASE	20
#define ST_SCROLL_SPEED_DECEASE_BASE	0.2f

CCScene* ShowTime::scene()
{
    CCScene *scene = CCScene::create();
    ShowTime *layer = ShowTime::create();
    scene->addChild(layer);
	
    return scene;
}

bool ShowTime::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	AudioEngine::sharedInstance()->stopSound();
	AudioEngine::sharedInstance()->playEffect("sound_ending_clear.mp3", false);
	
	setKeypadEnabled(true);
	
//	TempAd* temp_ad = TempAd::create();
//	addChild(temp_ad, 9999);
	
	int chapter_bonusImg_number = SelectedMapData::sharedInstance()->getImageNumber(kIN_bonusImage);
	int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
	
	string filename;
	
	if(chapter_bonusImg_number == 1 && selected_stage == 1)
	{
		bonus_img = CCSprite::create("beach_back.png");
		bonus_img->setPosition(ccp(160,240));
		addChild(bonus_img);
		
		bonus_eye = CCSprite::create("beach_eye.png", CCRectMake(0, 0, 100, 50));
		bonus_eye->setPosition(ccp(184,322));
		bonus_img->addChild(bonus_eye);
		
		randomFlicker();
	}
	else if(selected_stage == 5 && chapter_bonusImg_number <= 21)
	{
		filename = CCString::createWithFormat("test%d_visible.png", chapter_bonusImg_number)->getCString();
		
		bonus_img = CCSprite::create(filename.c_str());
		bonus_img->setPosition(ccp(160,240));
		addChild(bonus_img);
	}
	else if(chapter_bonusImg_number == 1)
	{
		if(selected_stage == 1)				filename = "c1_s1_visible.png";
		else if(selected_stage == 5)		filename = "c1_s5_visible.png";
		
		bonus_img = CCSprite::create(filename.c_str());
		bonus_img->setPosition(ccp(160,240));
		addChild(bonus_img);
	}
	else if(chapter_bonusImg_number == 11)
	{
		if(selected_stage == 1)				filename = "c11_s1_visible.png";
		else if(selected_stage == 5)		filename = "c11_s5_visible.png";
		
		bonus_img = CCSprite::create(filename.c_str());
		bonus_img->setPosition(ccp(160,240));
		addChild(bonus_img);
	}
	else
	{
		if(selected_stage == 5)				filename = CCString::createWithFormat("c%d_s5_visible.png",chapter_bonusImg_number)->getCString();
		
//		bonus_img = StageImgLoader::sharedInstance()->getLoadedImg(chapter_bonusImg_number, selected_stage);
		bonus_img->setPosition(ccp(160,240));
		addChild(bonus_img);
	}
	
//	touch_label = CCLabelTTF::create(MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_touchPlease), mySGD->getFont().c_str(), 20);
//	touch_label->setPosition(ccp(160,150));
//	touch_label->setVisible(false);
//	addChild(touch_label);
	
	CCSprite* n_check = CCSprite::create("showtime_check.png");
	CCSprite* s_check = CCSprite::create("showtime_check.png");
	s_check->setColor(ccGRAY);
	
	CCMenuItem* check_item = CCMenuItemSprite::create(n_check, s_check, this, menu_selector(ShowTime::menuAction));
	check_menu = CCMenu::createWithItem(check_item);
	check_menu->setVisible(false);
	check_menu->setPosition(ccp(290,450));
	addChild(check_menu);
	
	is_action = true;

	return true;
}

void ShowTime::touchOn()
{
	startTwinkle();
}

void ShowTime::startTwinkle()
{
	is_action = false;
	check_menu->setVisible(true);
	twinkle_cnt = 0;
//	touch_label->setVisible(true);
	
	setTouchEnabled(true);
	schedule(schedule_selector(ShowTime::twinkle));
}

void ShowTime::stopTwinkle()
{
	unschedule(schedule_selector(ShowTime::twinkle));
	setTouchEnabled(false);
}

void ShowTime::twinkle()
{
	twinkle_cnt++;
	
	if(twinkle_cnt % 30 == 0)
	{
//		touch_label->setVisible(!touch_label->isVisible());
	}
}

void ShowTime::onEnter()
{
	CCLayer::onEnter();
	
	FirecrackerParent* t_fp = FirecrackerParent::create();
	addChild(t_fp);
	
	CCDelayTime* t_delay = CCDelayTime::create(2.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ShowTime::touchOn));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	runAction(t_seq);
}

void ShowTime::onExit()
{
	CCLayer::onExit();
}

//bool ShowTime::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//	stopTwinkle();
//	closeShutter();
//	return true;
//}

void ShowTime::closeShutter()
{
	AudioEngine::sharedInstance()->playEffect("sound_shuttermove_start.m4a", false);
	
	bottom_shutter = CCSprite::create("loading_bottom.png");
	bottom_shutter->setAnchorPoint(ccp(0.5,1));
	bottom_shutter->setPosition(ccp(160,-10));
	addChild(bottom_shutter, 100);
	
	top_shutter = CCSprite::create("loading_top.png");
	top_shutter->setAnchorPoint(ccp(0.5,0));
	top_shutter->setPosition(ccp(160,490));
	addChild(top_shutter, 100);
	
	CCMoveTo* bottom_move = CCMoveTo::create(0.5f, ccp(160,240));
	CCMoveTo* top_move = CCMoveTo::create(0.5f, ccp(160,240));
	CCCallFunc* top_sound = CCCallFunc::create(this, callfunc_selector(ShowTime::shutterClosedSound));
	CCDelayTime* top_delay = CCDelayTime::create(0.1f);
	CCCallFunc* top_call = CCCallFunc::create(this, callfunc_selector(ShowTime::endCloseShutter));
	CCAction* top_seq = CCSequence::create(top_move, top_sound, top_delay, top_call, NULL);
	
	bottom_shutter->runAction(bottom_move);
	top_shutter->runAction(top_seq);
}
void ShowTime::shutterClosedSound()
{
	AudioEngine::sharedInstance()->playEffect("sound_shutter_closed.m4a", false);
}
void ShowTime::endCloseShutter()
{
	bottom_shutter->removeFromParentAndCleanup(true);
	top_shutter->removeFromParentAndCleanup(true);
	
	CCDirector::sharedDirector()->replaceScene(ClearScene::scene());
}

void ShowTime::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	
	pDispatcher->addStandardDelegate(this, -150);
	
//	pDispatcher->addTargetedDelegate(this, -150, true);
}

void ShowTime::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void ShowTime::moveListXY(CCPoint t_p)
{
	if(t_p.x > ST_SCROLL_SPEED_MAX_BASE/bonus_img->getScale())		t_p.x = ST_SCROLL_SPEED_MAX_BASE/bonus_img->getScale();
	if(t_p.x < -ST_SCROLL_SPEED_MAX_BASE/bonus_img->getScale())		t_p.x = -ST_SCROLL_SPEED_MAX_BASE/bonus_img->getScale();
	if(t_p.y > ST_SCROLL_SPEED_MAX_BASE/bonus_img->getScale())		t_p.y = ST_SCROLL_SPEED_MAX_BASE/bonus_img->getScale();
	if(t_p.y < -ST_SCROLL_SPEED_MAX_BASE/bonus_img->getScale())		t_p.y = -ST_SCROLL_SPEED_MAX_BASE/bonus_img->getScale();
	
	CCPoint a_p = ccpSub(bonus_img->getPosition(), t_p);
	
	if(a_p.x > 160.f+(bonus_img->getScale()-1.f)*160.f)		a_p.x = 160.f+(bonus_img->getScale()-1.f)*160.f;
	if(a_p.x < 160.f-(bonus_img->getScale()-1.f)*160.f)		a_p.x = 160.f-(bonus_img->getScale()-1.f)*160.f;
	if(a_p.y > 240.f+(bonus_img->getScale()-1.f)*240.f)		a_p.y = 240.f+(bonus_img->getScale()-1.f)*240.f;
	if(a_p.y < 240.f-(bonus_img->getScale()-1.f)*240.f)		a_p.y = 240.f-(bonus_img->getScale()-1.f)*240.f;
	
	bonus_img->setPosition(a_p);
}

void ShowTime::moveAnimation()
{
	isAnimated = true;
	if(moveSpeed_p.x >= ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale())			moveSpeed_p.x -= ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale();
	else if(moveSpeed_p.x <= -ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale())	moveSpeed_p.x += ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale();
	else							moveSpeed_p.x = 0;
	
	if(moveSpeed_p.y >= ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale())			moveSpeed_p.y -= ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale();
	else if(moveSpeed_p.y <= ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale())	moveSpeed_p.y += ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale();
	else							moveSpeed_p.y = 0;
	
	if((moveSpeed_p.x < ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale() && moveSpeed_p.x > -ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale() &&
		moveSpeed_p.y < ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale() && moveSpeed_p.y > -ST_SCROLL_SPEED_DECEASE_BASE/bonus_img->getScale()) || isAnimated == false)
	{
		this->unschedule(schedule_selector(ShowTime::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}

void ShowTime::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(ShowTime::alertAction));
}