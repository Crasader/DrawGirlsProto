//
//  ZoomScriptScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 6..
//
//

#include "ZoomScriptScene.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "PuzzleMapScene.h"
#include "utf8.h"

#define ZS_SCROLL_SPEED_MAX_BASE	20
#define ZS_SCROLL_SPEED_DECEASE_BASE	0.2f

CCScene* ZoomScript::scene()
{
    CCScene *scene = CCScene::create();
    ZoomScript *layer = ZoomScript::create();
    scene->addChild(layer);
	
    return scene;
}

enum ZS_Zorder{
	kZS_Z_first_img = 1,
	kZS_Z_second_img,
	kZS_Z_script_case,
	kZS_Z_script_label,
	kZS_Z_next_button,
	kZS_Z_showtime_back,
	kZS_Z_whitePaper
};

bool ZoomScript::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	game_node = CCNode::create();
	game_node->setScale(1.5f);
	addChild(game_node, kZS_Z_first_img);
	
	silType = mySD->getSilType();
	
	is_showtime = mySGD->is_showtime;
	is_exchanged = mySGD->is_exchanged;
	
	string first_filename;
	
	if(is_exchanged)			first_filename = CCString::createWithFormat("stage%d_level2_visible.png", silType)->getCString();
	else						first_filename = CCString::createWithFormat("stage%d_level1_visible.png", silType)->getCString();
	
	first_img = mySIL->getLoadedImg(first_filename.c_str());
	first_img->setPosition(ccp(160,215));
	game_node->addChild(first_img, kZS_Z_first_img);
	
	script_label = CCLabelTTF::create("", mySGD->getFont().c_str(), 18);
	script_label->setPosition(ccp(210,30));
	addChild(script_label, kZS_Z_script_label);
	
	script_case = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 0, 0));
	script_case->setColor(ccc3(10, 40, 70));
	script_case->setOpacity(175);
	script_case->setPosition(ccp(210,30));
	addChild(script_case, kZS_Z_script_case);
	
	
	CCSprite* n_next = CCSprite::create("script_ok.png");
	CCSprite* s_next = CCSprite::create("script_ok.png");
	s_next->setColor(ccGRAY);
	
	CCMenuItem* next_item = CCMenuItemSprite::create(n_next, s_next, this, menu_selector(ZoomScript::menuAction));
	
	next_button = CCMenu::createWithItem(next_item);
	next_button->setPosition(ccp(480-60,30));
	next_button->setVisible(false);
	addChild(next_button, kZS_Z_next_button);
	
	is_touched_menu = false;
	is_actioned = true;
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	minimum_scale = (screen_size.height*320)/(screen_size.width*430)*1.5f;
	
	game_node->setPosition(ccp(0,-430*1.5f+480.f*screen_size.height/screen_size.width));
	
	return true;
}

void ZoomScript::onEnterTransitionDidFinish()
{
	CCMoveTo* move1 = CCMoveTo::create(1.3f, ccp(0,0));
	CCDelayTime* delay1 = CCDelayTime::create(1.f);
	CCMoveTo* move2 = CCMoveTo::create(1.3f, ccp(0,-430*1.5f+480.f*screen_size.height/screen_size.width));
	CCDelayTime* delay2 = CCDelayTime::create(1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::startScript));
	game_node->runAction(CCSequence::create(move1, delay1, move2, delay2, t_call, NULL));
}

void ZoomScript::startScript()
{
	save_text = mySD->getScriptString(is_exchanged ? 2 : 1);
	
	basic_string<wchar_t> result;
	utf8::utf8to16(save_text.begin(), save_text.end(), back_inserter(result));
	text_length = result.length();
	typing_frame = 0;
	delegate_typing_after = callfunc_selector(ZoomScript::startTouchAction);
	schedule(schedule_selector(ZoomScript::typingAnimation), 1.f/10.f);
}

void ZoomScript::typingAnimation()
{
	typing_frame++;
	AudioEngine::sharedInstance()->playEffect("sound_crashed_map.mp3", false);
	if(typing_frame <= text_length)
	{
		basic_string<wchar_t> result;
		utf8::utf8to16(save_text.begin(), save_text.end(), back_inserter(result));
		result = result.substr(0, typing_frame);
		string conver;
		utf8::utf16to8(result.begin(), result.end(), back_inserter(conver));
		script_label->setString(conver.c_str());
		script_case->setTextureRect(CCRectMake(0, 0, script_label->getContentSize().width+8, script_label->getContentSize().height+8));
		
		if(typing_frame == text_length)
		{
			unschedule(schedule_selector(ZoomScript::typingAnimation));
			(this->*delegate_typing_after)();
		}
	}
}

void ZoomScript::startTouchAction()
{
	is_actioned = false;
	setTouchEnabled(true);
	next_button->setVisible(true);
}

void ZoomScript::menuAction(CCObject *sender)
{
	if(!is_actioned)
	{
		is_actioned = true;
		next_button->setVisible(false);
		setTouchEnabled(false);
		unschedule(schedule_selector(ZoomScript::moveAnimation));
		
		if(is_showtime)
		{
			showtime_back = CCSprite::create("showtime_back.png");
			showtime_back->setScale(10.f);
			showtime_back->setPosition(ccp(240,myDSH->ui_center_y));
			showtime_back->setOpacity(0);
			addChild(showtime_back, kZS_Z_showtime_back);
			
			white_paper = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 480, 320));
			white_paper->setOpacity(0);
			white_paper->setScaleY(myDSH->ui_top/320.f);
			white_paper->setPosition(ccp(240,myDSH->ui_center_y));
			addChild(white_paper, kZS_Z_whitePaper);
			
			CCDelayTime* white_paper_delay = CCDelayTime::create(46.f/60.f);
			CCFadeTo* white_paper_fade = CCFadeTo::create(18.f/60.f, 255);
			CCSequence* white_paper_seq = CCSequence::createWithTwoActions(white_paper_delay, white_paper_fade);
			white_paper->runAction(white_paper_seq);
			
			CCScaleTo* showtime_scale1 = CCScaleTo::create(28.f/60.f, 1.f);
			CCDelayTime* showtime_delay1 = CCDelayTime::create(18.f/60.f);
			CCScaleTo* showtime_scale2 = CCScaleTo::create(18.f/60.f, 12.f);
			CCSequence* showtime_seq1 = CCSequence::create(showtime_scale1, showtime_delay1, showtime_scale2, NULL);
			
			CCFadeTo* showtime_fade1 = CCFadeTo::create(28.f/60.f, 255);
			CCDelayTime* showtime_delay2 = CCDelayTime::create(18.f/60.f);
			CCFadeTo* showtime_fade2 = CCFadeTo::create(18.f/60.f, 0);
			CCSequence* showtime_seq2 = CCSequence::create(showtime_fade1, showtime_delay2, showtime_fade2, NULL);
			
			CCSpawn* showtime_spawn = CCSpawn::create(showtime_seq1, showtime_seq2, NULL);
			CCDelayTime* showtime_delay = CCDelayTime::create(8.f/60.f);
			
			CCCallFunc* showtime_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::showtimeFirstAction));
			CCSequence* showtime_seq = CCSequence::create(showtime_spawn, showtime_delay, showtime_call, NULL);
			
			showtime_back->runAction(showtime_seq);
		}
		else
		{
			myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_clear);
			CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
		}
	}
}

void ZoomScript::showtimeFirstAction()
{
	showtime_back->removeFromParent();
	
	script_label->setString("");
	script_case->setTextureRect(CCRectMake(0, 0, 0, 0));
	
	string second_filename;
	if(is_exchanged)
		second_filename = CCString::createWithFormat("stage%d_level3_visible.png", silType)->getCString();
	else
		second_filename = CCString::createWithFormat("stage%d_level2_visible.png", silType)->getCString();
	
	second_img = mySIL->getLoadedImg(second_filename.c_str());
	second_img->setPosition(ccp(160,215));
	game_node->addChild(second_img, kZS_Z_second_img);
	game_node->setScale(1.5f);
	game_node->setPosition(ccp(0,-430*game_node->getScale()+480*screen_size.height/screen_size.width));
	first_img->removeFromParentAndCleanup(true);
	
	if(is_exchanged && mySD->isAnimationStage())
	{
		eye_ani_size = mySD->getAnimationCutSize();
		loop_length = mySD->getAnimationLoopLength();
		
		for(int i=0;i<loop_length;i++)
			animation_frame.push_back(mySD->getAnimationLoopPoint(i));
		
		CCTexture2D* eye_texture = mySIL->addImage(CCString::createWithFormat("stage%d_level3_animation.png", silType)->getCString());
		
		CCSprite* eye = CCSprite::createWithTexture(eye_texture, CCRectMake(0, 0, eye_ani_size.width, eye_ani_size.height));
		eye->setPosition(mySD->getAnimationPosition());
		second_img->addChild(eye, 1, 1);
	}
	
	CCFadeTo* t_fade = CCFadeTo::create(1.f/6.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::showtimeSecondAction));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	white_paper->runAction(t_seq);
}

void ZoomScript::showtimeSecondAction()
{
	white_paper->removeFromParent();
	
	CCDelayTime* delay1 = CCDelayTime::create(0.5f);
	CCMoveTo* move1 = CCMoveTo::create(1.3f, ccp(0,0));
	CCDelayTime* delay2 = CCDelayTime::create(1.f);
	CCMoveTo* move2 = CCMoveTo::create(1.3f, ccp(0,-430*game_node->getScale()+480*screen_size.height/screen_size.width));
	CCDelayTime* delay3 = CCDelayTime::create(1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::showtimeThirdAction));
	
	CCAction* t_seq = CCSequence::create(delay1, move1, delay2, move2, delay3, t_call, NULL);
	
	game_node->runAction(t_seq);
}

void ZoomScript::showtimeThirdAction()
{
	if(is_exchanged && mySD->isAnimationStage())
	{
		startStageAnimation();
	}
	script_label->setVisible(true);
	script_case->setVisible(true);
	
	save_text = mySD->getScriptString(is_exchanged ? 3 : 2);
	
	basic_string<wchar_t> result;
	utf8::utf8to16(save_text.begin(), save_text.end(), back_inserter(result));
	text_length = result.length();
	typing_frame = 0;
	delegate_typing_after = callfunc_selector(ZoomScript::showtimeForthAction);
	schedule(schedule_selector(ZoomScript::typingAnimation), 1.f/10.f);
}

void ZoomScript::showtimeForthAction()
{
	is_actioned = false;
	is_showtime = false;
	is_touched_menu = false;
	next_button->setVisible(true);
	setTouchEnabled(true);
}

void ZoomScript::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	
	pDispatcher->addStandardDelegate(this, -150);
}

void ZoomScript::moveListXY(CCPoint t_p)
{
	script_label->setVisible(false);
	script_case->setVisible(false);
	
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

void ZoomScript::moveAnimation()
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
		this->unschedule(schedule_selector(ZoomScript::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}
