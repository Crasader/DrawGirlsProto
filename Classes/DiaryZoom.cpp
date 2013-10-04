//
//  DiaryZoom.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 2..
//
//

#include "DiaryZoom.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "CollectionBook.h"

#define ZS_SCROLL_SPEED_MAX_BASE	20
#define ZS_SCROLL_SPEED_DECEASE_BASE	0.2f

CCScene* DiaryZoom::scene()
{
    CCScene *scene = CCScene::create();
    DiaryZoom *layer = DiaryZoom::create();
    scene->addChild(layer);
	
    return scene;
}

enum DZ_Zorder{
	kDZ_Z_first_img = 1,
	kDZ_Z_next_button
};

bool DiaryZoom::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	game_node = CCNode::create();
	game_node->setScale(1.5f);
	addChild(game_node, kDZ_Z_first_img);
	
	int card_number = mySGD->selected_collectionbook;
	
	int stage_number = card_number/10;
	int level_number = card_number%10 + 1;
	
	first_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", stage_number, level_number)->getCString());
	first_img->setPosition(ccp(160,215));
	game_node->addChild(first_img, kDZ_Z_first_img);
	
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
	
	CCMenuItem* next_item = CCMenuItemSprite::create(n_next, s_next, this, menu_selector(DiaryZoom::menuAction));
	
	next_button = CCMenu::createWithItem(next_item);
	next_button->setPosition(ccp(480-60,30));
	next_button->setVisible(false);
	addChild(next_button, kDZ_Z_next_button);
	
	is_touched_menu = false;
	is_actioned = true;
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	minimum_scale = (screen_size.height*320)/(screen_size.width*430)*1.5f;
	
	game_node->setPosition(ccp(0,-430*1.5f+480.f*screen_size.height/screen_size.width));
	
	return true;
}

void DiaryZoom::onEnterTransitionDidFinish()
{
	startTouchAction();
	if(is_animation)
		startStageAnimation();
}

void DiaryZoom::startTouchAction()
{
	is_actioned = false;
	setTouchEnabled(true);
	next_button->setVisible(true);
}

void DiaryZoom::menuAction(CCObject *sender)
{
	if(!is_actioned)
	{
		is_actioned = true;
		next_button->setVisible(false);
		setTouchEnabled(false);
		unschedule(schedule_selector(DiaryZoom::moveAnimation));
		
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(480, 320, kResolutionNoBorder);
		
		CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
	}
}

void DiaryZoom::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	
	pDispatcher->addStandardDelegate(this, -150);
}

void DiaryZoom::moveListXY(CCPoint t_p)
{
	if(t_p.x > ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.x < -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
	if(t_p.y > ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.y < -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
	CCPoint a_p = ccpSub(game_node->getPosition(), t_p);
	
	if(game_node->getScale() <= 1.5f)
		a_p.x = (480.f-320.f*game_node->getScale())/2.f;
	else
	{
		if(a_p.x > 0)
			a_p.x = 0;
		else if(a_p.x < 480-320*game_node->getScale())
			a_p.x = 480-320*game_node->getScale();
	}
	
	if(a_p.y > 0)
		a_p.y = 0;
	if(a_p.y < -430*game_node->getScale()+480*screen_size.height/screen_size.width)
		a_p.y = -430*game_node->getScale()+480*screen_size.height/screen_size.width;
	
	game_node->setPosition(a_p);
}

void DiaryZoom::moveAnimation()
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
		this->unschedule(schedule_selector(DiaryZoom::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}
