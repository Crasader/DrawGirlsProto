//
//  DiaryZoomPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#include "DiaryZoomPopup.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "CollectionBookPopup.h"
#include "PuzzleMapScene.h"

#define ZS_SCROLL_SPEED_MAX_BASE	20
#define ZS_SCROLL_SPEED_DECEASE_BASE	0.2f

enum DZP_Zorder{
	kDZP_Z_gray = 1,
	kDZP_Z_first_img,
	kDZP_Z_next_button
};

void DiaryZoomPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

bool DiaryZoomPopup::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kDZP_Z_gray);
	
	
	game_node = CCNode::create();
	game_node->setScale(1.5f);
	addChild(game_node, kDZP_Z_first_img);
	
	int card_number = mySGD->selected_collectionbook;
	
	int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
	int level_number = NSDS_GI(kSDS_CI_int1_grade_i, card_number);
	
	first_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, level_number)->getCString());
	first_img->setOpacity(0);
	first_img->setPosition(ccp(160,215));
	game_node->addChild(first_img, kDZP_Z_first_img);
	
	is_animation = false;
	if(level_number == 3 && mySD->isAnimationStage(stage_number))
	{
		is_animation = true;
		eye_ani_size = mySD->getAnimationCutSize(stage_number);
		loop_length = mySD->getAnimationLoopLength(stage_number);
		
		for(int i=0;i<loop_length;i++)
			animation_frame.push_back(mySD->getAnimationLoopPoint(i));
		
		CCTexture2D* eye_texture = mySIL->addImage(CCString::createWithFormat("stage%d_level3_animation.png", stage_number)->getCString());
		
		CCSprite* eye = CCSprite::createWithTexture(eye_texture, CCRectMake(0, 0, eye_ani_size.width, eye_ani_size.height));
		eye->setPosition(mySD->getAnimationPosition());
		first_img->addChild(eye, 1, 1);
	}
	
	
	CCSprite* n_next = CCSprite::create("script_ok.png");
	CCSprite* s_next = CCSprite::create("script_ok.png");
	s_next->setColor(ccGRAY);
	
	CCMenuItem* next_item = CCMenuItemSprite::create(n_next, s_next, this, menu_selector(DiaryZoomPopup::menuAction));
	
	next_button = CCMenu::createWithItem(next_item);
	next_button->setPosition(ccp(480-60,30));
	next_button->setVisible(false);
	addChild(next_button, kDZP_Z_next_button);
	
	is_touched_menu = false;
	is_actioned = false;
	
	minimum_scale = (screen_size.height*320)/(screen_size.width*430)*1.5f;
	
	game_node->setPosition(ccp(-myDSH->ui_zero_point.x,(-430*1.5f+480.f*screen_size.height/screen_size.width)/myDSH->screen_convert_rate - myDSH->ui_zero_point.y));
	
	return true;
}

void DiaryZoomPopup::onEnterTransitionDidFinish()
{
	showPopup();
}

void DiaryZoomPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCFadeTo* img_fade = CCFadeTo::create(0.5f, 255);
	CCCallFunc* img_call = CCCallFunc::create(this, callfunc_selector(DiaryZoomPopup::endShowPopup));
	CCSequence* img_seq = CCSequence::createWithTwoActions(img_fade, img_call);
	first_img->runAction(img_seq);
}

void DiaryZoomPopup::endShowPopup()
{
	is_actioned = true;
	startTouchAction();
	if(is_animation)
		startStageAnimation();
}

void DiaryZoomPopup::hidePopup()
{
	setTouchEnabled(false);
	next_button->setVisible(false);
	is_actioned = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCFadeTo* img_fade = CCFadeTo::create(0.5f, 0);
	CCCallFunc* img_call = CCCallFunc::create(this, callfunc_selector(DiaryZoomPopup::endHidePopup));
	CCSequence* img_seq = CCSequence::createWithTwoActions(img_fade, img_call);
	first_img->runAction(img_seq);
}

void DiaryZoomPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void DiaryZoomPopup::startTouchAction()
{
	is_actioned = false;
	setTouchEnabled(true);
	next_button->setVisible(true);
}

void DiaryZoomPopup::menuAction(CCObject *sender)
{
	if(!is_actioned)
	{
		is_actioned = true;
		next_button->setVisible(false);
		setTouchEnabled(false);
		unschedule(schedule_selector(DiaryZoomPopup::moveAnimation));
		
		CollectionBookPopup* t_popup = CollectionBookPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, kPMS_Z_popup);
		
		target_final = NULL;
		hidePopup();
		
		
//		CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
	}
}

void DiaryZoomPopup::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	
	pDispatcher->addStandardDelegate(this, -150);
}

void DiaryZoomPopup::moveListXY(CCPoint t_p)
{
	if(t_p.x > ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.x < -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
	if(t_p.y > ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.y < -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
	CCPoint a_p = ccpSub(game_node->getPosition(), t_p);
	
	if(game_node->getScale() <= 1.5f)
		a_p.x = ((480.f-320.f*game_node->getScale())/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x;
	else
	{
		if(a_p.x > -myDSH->ui_zero_point.x)
			a_p.x = -myDSH->ui_zero_point.x;
		else if(a_p.x < (480-320*game_node->getScale())/myDSH->screen_convert_rate - myDSH->ui_zero_point.x)
			a_p.x = (480-320*game_node->getScale())/myDSH->screen_convert_rate - myDSH->ui_zero_point.x;
	}
	
	if(a_p.y > -myDSH->ui_zero_point.y)
		a_p.y = -myDSH->ui_zero_point.y;
	if(a_p.y < (-430*game_node->getScale()+480*screen_size.height/screen_size.width)/myDSH->screen_convert_rate - myDSH->ui_zero_point.y)
		a_p.y = (-430*game_node->getScale()+480*screen_size.height/screen_size.width)/myDSH->screen_convert_rate - myDSH->ui_zero_point.y;
	
	game_node->setPosition(a_p);
}

void DiaryZoomPopup::moveAnimation()
{
	isAnimated = true;
	
	if(moveSpeed_p.x >= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())			moveSpeed_p.x -= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else if(moveSpeed_p.x <= -ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())	moveSpeed_p.x += ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else							moveSpeed_p.x = 0;
	
	if(moveSpeed_p.y >= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())			moveSpeed_p.y -= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else if(moveSpeed_p.y <= ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())	moveSpeed_p.y += ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else							moveSpeed_p.y = 0;
	
	if((moveSpeed_p.x < ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() && moveSpeed_p.x > -ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() &&
		moveSpeed_p.y < ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() && moveSpeed_p.y > -ZS_SCROLL_SPEED_DECEASE_BASE/game_node->getScale()) || isAnimated == false)
	{
		this->unschedule(schedule_selector(DiaryZoomPopup::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}