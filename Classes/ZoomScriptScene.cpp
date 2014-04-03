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
#include "PuzzleScene.h"
#include "utf8.h"
#include "MainFlowScene.h"
#include "CommonButton.h"
#include "NewMainFlowScene.h"
#include "bustMorphing.h"

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
	
	int card_number;
	
	if(is_exchanged)	card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 2);
	else				card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 1);
	
	first_img = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()));
	first_img->putBasicInfomation();	// 기본정보 들어가게.
	if(NSDS_GI(kSDS_CI_int1_grade_i, card_number) < 3)
		first_img->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.
	first_img->triangulationWithPoints();
	
	first_img->setPosition(ccp(160,215));
	first_img->setTouchEnabled(false);
	game_node->addChild(first_img, kZS_Z_first_img);
	
	target_node = first_img;
	
	zoom_img = CCSprite::create("ending_expand.png");
	zoom_img->setPosition(ccp(445,myDSH->ui_top-35));
	addChild(zoom_img, kZS_Z_script_case);
	
	script_label = CCLabelTTF::create("", mySGD->getFont().c_str(), 16);
	script_label->setPosition(ccp(210,30));
	addChild(script_label, kZS_Z_script_label);
	
	script_case = CCSprite::create("ending_script_case.png");
	script_case->setVisible(false);
	script_case->setPosition(ccp(210,30));
	addChild(script_case, kZS_Z_script_case);
	
	
	next_button = CommonButton::create("확 인",15,CCSizeMake(80,50), CommonButtonYellow, -160);
	next_button->setFunction([=](CCObject* sender){menuAction(sender);});
	next_button->setPosition(ccp(480-50,30));
	next_button->setVisible(false);
	addChild(next_button, kZS_Z_next_button);
	
	is_spin_mode = false;
	mode_button = CommonButton::create("이동", 10, CCSizeMake(40, 30), CommonButtonOrange, -160);
	mode_button->setPosition(ccp(480-50,myDSH->ui_top-30));
	mode_button->setFunction([=](CCObject* sender)
							 {
								 is_spin_mode = !is_spin_mode;
								 if(is_spin_mode)
									 mode_button->setTitle("회전");
								 else
									 mode_button->setTitle("이동");
							 });
	mode_button->setVisible(false);
	addChild(mode_button, kZS_Z_next_button);
	
//	is_touched_menu = false;
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
	save_text = NSDS_GS(kSDS_CI_int1_script_s, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, (is_exchanged ? 2 : 1)));
	
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
		script_case->setVisible(true);
		
		if(typing_frame == text_length)
		{
			unschedule(schedule_selector(ZoomScript::typingAnimation));
			(this->*delegate_typing_after)();
		}
	}
	else
	{
		unschedule(schedule_selector(ZoomScript::typingAnimation));
		(this->*delegate_typing_after)();
	}
}

void ZoomScript::startTouchAction()
{
	is_actioned = false;
	setTouchEnabled(true);
	next_button->setVisible(true);
	mode_button->setVisible(true);
	
//	target_node->setTouchEnabled(true);
	
	is_scrolling = false;
	is_before_scrolling = is_scrolling;
	
	save_position = game_node->getPosition();
	schedule(schedule_selector(ZoomScript::moveChecking));
}

void ZoomScript::moveChecking()
{
	CCPoint after_position = game_node->getPosition();
	
	if(is_scrolling)
	{
		target_node->movingDistance(ccpSub(after_position, save_position));
		is_before_scrolling = is_scrolling;
	}
	else if(is_before_scrolling)
	{
		is_before_scrolling = false;
		target_node->movingDistance(CCPointZero);
	}
	save_position = after_position;
}

void ZoomScript::menuAction(CCObject *sender)
{
	if(!is_actioned)
	{
		unschedule(schedule_selector(ZoomScript::moveChecking));
		
		is_actioned = true;
		next_button->setVisible(false);
		mode_button->setVisible(false);
		setTouchEnabled(false);
		target_node->setTouchEnabled(false);
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
//			CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
			if(mySD->getSilType() < 10000)
			{
				CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
			}
			else
				CCDirector::sharedDirector()->replaceScene(NewMainFlowScene::scene());
		}
	}
}

void ZoomScript::showtimeFirstAction()
{
	showtime_back->removeFromParent();
	
	script_label->setString("");
	script_case->setVisible(false);
	
	int card_number;
	
	if(is_exchanged)		card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3);
	else					card_number = NSDS_GI(silType, kSDS_SI_level_int1_card_i, 2);
	
	second_img = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()));
	second_img->putBasicInfomation();	// 기본정보 들어가게.
	if(NSDS_GI(kSDS_CI_int1_grade_i, card_number) < 3)
		second_img->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.
	second_img->triangulationWithPoints();
	
	second_img->setPosition(ccp(160,215));
	second_img->setTouchEnabled(false);
	game_node->addChild(second_img, kZS_Z_second_img);
	
	target_node = second_img;
	
	game_node->setScale(1.5f);
	game_node->setPosition(ccp(0,-430*game_node->getScale()+480*screen_size.height/screen_size.width));
	first_img->removeFromParentAndCleanup(true);
	
	int third_card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3);
	
	if(is_exchanged && NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, third_card_number))
	{
		eye_ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, third_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, third_card_number));
		loop_length = NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopLength_i, third_card_number);
		
		for(int i=0;i<loop_length;i++)
			animation_frame.push_back(NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, third_card_number, i));
		
		CCTexture2D* eye_texture = mySIL->addImage(CCString::createWithFormat("card%d_animation.png", third_card_number)->getCString());
		
		CCSprite* eye = CCSprite::createWithTexture(eye_texture, CCRectMake(0, 0, eye_ani_size.width, eye_ani_size.height));
		eye->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, third_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, third_card_number)));
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
	if(is_exchanged && NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3)))
	{
		startStageAnimation();
	}
	script_label->setVisible(true);
	script_case->setVisible(true);
	
	save_text = NSDS_GS(kSDS_CI_int1_script_s, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, (is_exchanged ? 3 : 2)));
	
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
//	is_touched_menu = false;
	next_button->setVisible(true);
	mode_button->setVisible(true);
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
	{
		if(a_p.x > (480.f-320.f*game_node->getScale())/2.f+40.f)
			a_p.x = (480.f-320.f*game_node->getScale())/2.f+40.f;
		else if(a_p.x < (480.f-320.f*game_node->getScale())/2.f-40.f)
			a_p.x = (480.f-320.f*game_node->getScale())/2.f-40.f;
	}
	else
	{
		if(a_p.x > 40.f)
			a_p.x = 40.f;
		else if(a_p.x < 480-320*game_node->getScale()-40.f)
			a_p.x = 480-320*game_node->getScale()-40.f;
	}
	
	if(a_p.y > 0+40.f)
		a_p.y = 0+40.f;
	if(a_p.y < -430*game_node->getScale()+480*screen_size.height/screen_size.width-40.f)
		a_p.y = -430*game_node->getScale()+480*screen_size.height/screen_size.width-40.f;
	
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

void ZoomScript::frameAnimation()
{
	int loop_point = animation_frame[ing_animation_frame];
	((CCSprite*)second_img->getChildByTag(1))->setTextureRect(CCRectMake(loop_point*eye_ani_size.width, 0, eye_ani_size.width, eye_ani_size.height));

	ing_animation_frame++;
	if(ing_animation_frame >= loop_length)
		startStageAnimation();
	else
	{
		CCDelayTime* t_delay = CCDelayTime::create(0.1f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::frameAnimation));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		second_img->getChildByTag(1)->runAction(t_seq);
	}
}

void ZoomScript::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch *touch;

	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));

		multiTouchData[(int)touch] = location;

		touch_p = location;

		timeval time;
		gettimeofday(&time, NULL);
		touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
		touchStart_p = location;

		this->unschedule(schedule_selector(ZoomScript::moveAnimation));

		isAnimated=false;

		if(multiTouchData.size() >= 1)
		{
			target_node->setTouchEnabled(false);
		}
		
		if(multiTouchData.size() == 1)
		{
			first_touch_time = touchStartTime;
			is_scrolling = true;
//			if(!is_touched_menu && next_button->ccTouchBegan(touch, pEvent))
//			{
//				is_touched_menu = true;
//			}
		}
		else if(multiTouchData.size() == 2)
		{
			is_scrolling = false;
			CCPoint sub_point = CCPointZero;
			map<int, CCPoint>::iterator it;
			for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
			{
				sub_point = ccpMult(sub_point, -1);
				sub_point = ccpAdd(sub_point, it->second);
			}

			zoom_base_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
		}
		else
		{
			is_scrolling = false;
//			if(is_touched_menu)
//			{
//				next_button->ccTouchCancelled(touch, pEvent);
//				is_touched_menu = false;
//			}
		}
	}
}

void ZoomScript::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch* touch;

	for(iter = pTouches->begin();iter != pTouches->end();++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));

		map<int, CCPoint>::iterator o_it;
		o_it = multiTouchData.find((int)touch);
		if(o_it != multiTouchData.end())
		{
			o_it->second = location;
			if(multiTouchData.size() == 1)
			{
//				if(is_touched_menu)
//				{
//					next_button->ccTouchMoved(touch, pEvent);
//				}
				
				if(is_spin_mode)
				{
					this->unschedule(schedule_selector(ZoomScript::moveAnimation));
					moveSpeed_p = CCPointZero;
					isAnimated = false;
					
					CCPoint rotate_sub = ccpSub(location, touch_p);
					
					float rotation_degree = target_node->getImageRotationDegree() + rotate_sub.x/5.f;
					if(rotation_degree > 60.f)
						rotation_degree = 60.f;
					else if(rotation_degree < -60.f)
						rotation_degree = -60.f;
					target_node->setImageRotationDegree(rotation_degree);
					
					float rotation_degreeX = target_node->getImageRotationDegreeX() - rotate_sub.y/5.f;
					if(rotation_degreeX > 60.f)
						rotation_degreeX = 60.f;
					else if(rotation_degreeX < -60.f)
						rotation_degreeX = -60.f;
					target_node->setImageRotationDegreeX(rotation_degreeX);
				}
				else
					this->moveListXY(ccpSub(touch_p, location));
				touch_p = location;
			}
			else if(multiTouchData.size() == 2)
			{
				CCPoint sub_point = CCPointZero;
				map<int, CCPoint>::iterator it;
				for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
				{
					sub_point = ccpMult(sub_point, -1);
					sub_point = ccpAdd(sub_point, it->second);
				}

				script_label->setVisible(false);
				script_case->setVisible(false);

				float before_scale = game_node->getScale();

				float changed_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
				float after_scale = game_node->getScale()*changed_distance/zoom_base_distance;
				if(after_scale > 2.5f)			after_scale = 2.5f;
				else if(after_scale < minimum_scale)		after_scale = minimum_scale;
				zoom_base_distance = changed_distance;
				game_node->setScale(after_scale);

				CCPoint a_p;
				{
					float comp_scale = before_scale < 1.5f ? 1.5f : before_scale;
					comp_scale = game_node->getScale() - comp_scale;
					
					a_p.x = game_node->getPositionX() - 320*comp_scale/2.f;
				}
				
				if(game_node->getScale() <= 1.5f)
				{
					if(a_p.x > (480.f-320.f*game_node->getScale())/2.f+40.f)
						game_node->setPositionX((480.f-320.f*game_node->getScale())/2.f+40.f);
					else if(a_p.x < (480.f-320.f*game_node->getScale())/2.f-40.f)
						game_node->setPositionX((480.f-320.f*game_node->getScale())/2.f-40.f);
				}
				else
				{
					game_node->setPositionX(a_p.x);

					if(game_node->getPositionX() > 40.f)
						game_node->setPositionX(40.f);
					else if(game_node->getPositionX() < 480-320*game_node->getScale()-40.f)
						game_node->setPositionX(480-320*game_node->getScale()-40.f);
				}

				float comp_scale = before_scale;
				comp_scale = game_node->getScale() - comp_scale;

				game_node->setPositionY(game_node->getPositionY() - 430*comp_scale/2.f);

				if(game_node->getPositionY() > 0+40.f)
					game_node->setPositionY(0+40.f);
				else if(game_node->getPositionY() < -430*game_node->getScale()+480*screen_size.height/screen_size.width-40.f)
					game_node->setPositionY(-430*game_node->getScale()+480*screen_size.height/screen_size.width-40.f);
			}
		}
	}
}

void ZoomScript::startStageAnimation()
{
	ing_animation_frame = 0;

	int random_value = rand()%16;

	if(random_value >= 2 && random_value <= 4)
		random_value = 7;
	else if(random_value >= 5 && random_value <= 10)
		random_value = 10;
	else if(random_value >= 11 && random_value <= 13)
		random_value = 13;
	else if(random_value == 14 || random_value == 15)
		random_value = 16;
	else
		random_value++;

	CCDelayTime* t_delay = CCDelayTime::create(random_value*0.1f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::frameAnimation));
	CCAction* t_seq = CCSequence::create(t_delay, t_call, NULL);
	second_img->getChildByTag(1)->runAction(t_seq);
}

void ZoomScript::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch* touch;

	for(iter = pTouches->begin();iter != pTouches->end();++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));

		map<int, CCPoint>::iterator o_it;
		o_it = multiTouchData.find((int)touch);
		if(o_it != multiTouchData.end())
		{
			multiTouchData.erase((int)touch);

			if(multiTouchData.size() == 1)
			{
				is_scrolling = true;
			}
			else
			{
				is_scrolling = false;
			}
			
			if(multiTouchData.size() == 0)
			{
//				target_node->setTouchEnabled(true);
				
//				if(is_touched_menu)
//				{
//					next_button->ccTouchEnded(touch, pEvent);
//					is_touched_menu = false;
//				}

				timeval time;
				gettimeofday(&time, NULL);
				
				if(int(((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - first_touch_time) < 100000)
				{
					target_node->ccTouchEnded(touch, pEvent);
				}
				else
				{
					long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
					CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
					
					float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
					if(isAnimated == false && fabsf(spd_value) > 2 && !is_spin_mode)
					{
						moveSpeed_p = _spd;
						this->schedule(schedule_selector(ZoomScript::moveAnimation));
					}
				}
			}
		}
	}
}

void ZoomScript::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{
	ccTouchesEnded(pTouches, pEvent);
}
