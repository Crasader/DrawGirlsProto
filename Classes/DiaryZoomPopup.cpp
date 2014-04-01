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
#include "DataStorageHub.h"
#include "TouchSuctionLayer.h"
#include "CommonButton.h"
#include "TouchSwallowManagement.h"
#include "bustMorphing.h"

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
	
	before_sub_position = CCPointZero;
	
	int card_number = mySGD->selected_collectionbook;
	
	first_img = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()));
	first_img->putBasicInfomation();	// 기본정보 들어가게.
	if(NSDS_GI(kSDS_CI_int1_grade_i, card_number) < 3)
		first_img->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.
	first_img->triangulationWithPoints();
	
	first_img->setPosition(ccp(160,215));
	first_img->setTouchEnabled(false);
	game_node->addChild(first_img, kDZP_Z_first_img);
	
	is_animation = NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, card_number);
	if(is_animation)
	{
		eye_ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, card_number));
		loop_length = NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopLength_i, card_number);
		
		for(int i=0;i<loop_length;i++)
			animation_frame.push_back(NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, card_number, i));
		
		CCTexture2D* eye_texture = mySIL->addImage(CCString::createWithFormat("card%d_animation.png", card_number)->getCString());
		
		CCSprite* eye = CCSprite::createWithTexture(eye_texture, CCRectMake(0, 0, eye_ani_size.width, eye_ani_size.height));
		eye->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, card_number)));
		first_img->addChild(eye, 1, 1);
	}
	
	next_button = CommonButton::createCloseButton(-252);
	next_button->setPosition(ccp(480-60,30));
	next_button->setFunction([=](CCObject* sender)
							 {
								 menuAction(sender);
							 });
	next_button->setVisible(false);
	addChild(next_button, kDZP_Z_next_button);
	
	
	is_spin_mode = false;
	
	mode_button = CommonButton::create("이동", 10, CCSizeMake(40, 30), CommonButtonOrange, -252);
	mode_button->setPosition(ccp(480-60,290));
	mode_button->setFunction([=](CCObject* sender)
							{
								is_spin_mode = !is_spin_mode;
								if(is_spin_mode)
									mode_button->setTitle("회전");
								else
									mode_button->setTitle("이동");
							});
	mode_button->setVisible(false);
	addChild(mode_button, kDZP_Z_next_button);
	
	
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
	
	CCDelayTime* img_fade = CCDelayTime::create(0.5f);
//	CCFadeTo* img_fade = CCFadeTo::create(0.5f, 255);
	CCCallFunc* img_call = CCCallFunc::create(this, callfunc_selector(DiaryZoomPopup::endShowPopup));
	CCSequence* img_seq = CCSequence::createWithTwoActions(img_fade, img_call);
	first_img->runAction(img_seq);
}

void DiaryZoomPopup::endShowPopup()
{
	TouchSwallowManagement::sharedInstance()->offAllSwallow();
	
	is_actioned = true;
	startTouchAction();
	if(is_animation)
		startStageAnimation();
}

void DiaryZoomPopup::hidePopup()
{
	TouchSwallowManagement::sharedInstance()->onAllSwallow();
	setTouchEnabled(false);
	next_button->setVisible(false);
	mode_button->setVisible(false);
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
	mode_button->setVisible(true);
//	first_img->setTouchEnabled(true);
	
	is_scrolling = false;
	is_before_scrolling = is_scrolling;
	
	save_position = game_node->getPosition();
	schedule(schedule_selector(DiaryZoomPopup::moveChecking));
}

void DiaryZoomPopup::moveChecking()
{
	CCPoint after_position = game_node->getPosition();
	
	if(is_scrolling)
	{
		CCPoint sub_position = ccpSub(after_position, save_position);
		if(before_sub_position.x == 0.f && before_sub_position.y == 0.f && sub_position.x == 0.f && sub_position.y == 0.f)
		{
			
		}
		else
		{
			first_img->movingDistance(sub_position);
			before_sub_position = sub_position;
		}
		is_before_scrolling = is_scrolling;
	}
	else if(is_before_scrolling)
	{
		is_before_scrolling = false;
		first_img->movingDistance(CCPointZero);
		before_sub_position = CCPointZero;
	}
	save_position = after_position;
}

void DiaryZoomPopup::menuAction(CCObject *sender)
{
	if(!is_actioned)
	{
		unschedule(schedule_selector(DiaryZoomPopup::moveChecking));
		is_actioned = true;
		next_button->setVisible(false);
		mode_button->setVisible(false);
		setTouchEnabled(false);
		first_img->setTouchEnabled(false);
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
	
	pDispatcher->addStandardDelegate(this, -250);
}

void DiaryZoomPopup::moveListXY(CCPoint t_p)
{
	if(t_p.x > ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.x < -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
	if(t_p.y > ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.y < -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = -ZS_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
	CCPoint a_p = ccpSub(game_node->getPosition(), t_p);
	
	if(game_node->getScale() <= 1.5f)
	{
		if(a_p.x > ((480.f-320.f*game_node->getScale())/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x+40.f)
			a_p.x = ((480.f-320.f*game_node->getScale())/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x+40.f;
		else if(a_p.x < ((480.f-320.f*game_node->getScale())/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x-40.f)
			a_p.x = ((480.f-320.f*game_node->getScale())/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x-40.f;
	}
	else
	{
		if(a_p.x > -myDSH->ui_zero_point.x+40.f)
			a_p.x = -myDSH->ui_zero_point.x+40.f;
		else if(a_p.x < (480-320*game_node->getScale())/myDSH->screen_convert_rate - myDSH->ui_zero_point.x-40.f)
			a_p.x = (480-320*game_node->getScale())/myDSH->screen_convert_rate - myDSH->ui_zero_point.x-40.f;
	}
	
	if(a_p.y > -myDSH->ui_zero_point.y+40.f)
		a_p.y = -myDSH->ui_zero_point.y+40.f;
	if(a_p.y < (-430*game_node->getScale()+480*screen_size.height/screen_size.width)/myDSH->screen_convert_rate - myDSH->ui_zero_point.y-40.f)
		a_p.y = (-430*game_node->getScale()+480*screen_size.height/screen_size.width)/myDSH->screen_convert_rate - myDSH->ui_zero_point.y-40.f;
	
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

void DiaryZoomPopup::startStageAnimation()
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
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(DiaryZoomPopup::frameAnimation));
	CCAction* t_seq = CCSequence::create(t_delay, t_call, NULL);
	first_img->getChildByTag(1)->runAction(t_seq);
}

void DiaryZoomPopup::frameAnimation()
{
	int loop_point = animation_frame[ing_animation_frame];
	((CCSprite*)first_img->getChildByTag(1))->setTextureRect(CCRectMake(loop_point*eye_ani_size.width, 0, eye_ani_size.width, eye_ani_size.height));

	ing_animation_frame++;
	if(ing_animation_frame >= loop_length)
		startStageAnimation();
	else
	{
		CCDelayTime* t_delay = CCDelayTime::create(0.1f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(DiaryZoomPopup::frameAnimation));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		first_img->getChildByTag(1)->runAction(t_seq);
	}
}

void DiaryZoomPopup::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch *touch;

	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		location = myDSH->wideWidthFixTouch(location);

		multiTouchData[(int)touch] = location;

		touch_p = location;

		timeval time;
		gettimeofday(&time, NULL);
		touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
		touchStart_p = location;

		this->unschedule(schedule_selector(DiaryZoomPopup::moveAnimation));

		isAnimated=false;

		if(multiTouchData.size() >= 1)
		{
			first_img->setTouchEnabled(false);
		}
		
		if(multiTouchData.size() == 1)
		{
			is_scrolling = true;
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
		}
	}
}

void DiaryZoomPopup::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch* touch;

	for(iter = pTouches->begin();iter != pTouches->end();++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		location = myDSH->wideWidthFixTouch(location);

		map<int, CCPoint>::iterator o_it;
		o_it = multiTouchData.find((int)touch);
		if(o_it != multiTouchData.end())
		{
			o_it->second = location;
			if(multiTouchData.size() == 1)
			{
				if(is_spin_mode)
				{
					this->unschedule(schedule_selector(DiaryZoomPopup::moveAnimation));
					moveSpeed_p = CCPointZero;
					isAnimated = false;
					
					CCPoint rotate_sub = ccpSub(location, touch_p);
					
					float rotation_degree = first_img->getImageRotationDegree() + rotate_sub.x/5.f;
					if(rotation_degree > 60.f)
						rotation_degree = 60.f;
					else if(rotation_degree < -60.f)
						rotation_degree = -60.f;
					first_img->setImageRotationDegree(rotation_degree);
					
					float rotation_degreeX = first_img->getImageRotationDegreeX() - rotate_sub.y/5.f;
					if(rotation_degreeX > 60.f)
						rotation_degreeX = 60.f;
					else if(rotation_degreeX < -60.f)
						rotation_degreeX = -60.f;
					first_img->setImageRotationDegreeX(rotation_degreeX);
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
					
					a_p.x = ((game_node->getPositionX()+myDSH->ui_zero_point.x)*myDSH->screen_convert_rate - 320*comp_scale/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x;
				}
				
				if(game_node->getScale() <= 1.5f)
				{
					if(a_p.x > ((480.f-320.f*game_node->getScale())/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x+40.f)
						a_p.x = ((480.f-320.f*game_node->getScale())/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x+40.f;
					else if(a_p.x < ((480.f-320.f*game_node->getScale())/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x-40.f)
						a_p.x = ((480.f-320.f*game_node->getScale())/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.x-40.f;
					
					game_node->setPositionX(a_p.x);
				}
				else
				{
					game_node->setPositionX(a_p.x);
					
					if(game_node->getPositionX() > -myDSH->ui_zero_point.x+40.f)
						game_node->setPositionX(-myDSH->ui_zero_point.x+40.f);
					else if(game_node->getPositionX() < (480-320*game_node->getScale())/myDSH->screen_convert_rate - myDSH->ui_zero_point.x-40.f)
						game_node->setPositionX((480-320*game_node->getScale())/myDSH->screen_convert_rate - myDSH->ui_zero_point.x-40.f);
				}

				float comp_scale = before_scale;
				comp_scale = game_node->getScale() - comp_scale;

				game_node->setPositionY(((game_node->getPositionY()+myDSH->ui_zero_point.y)*myDSH->screen_convert_rate - 430*comp_scale/2.f)/myDSH->screen_convert_rate - myDSH->ui_zero_point.y);

				if(game_node->getPositionY() > -myDSH->ui_zero_point.y+40.f)
					game_node->setPositionY(-myDSH->ui_zero_point.y+40.f);
				else if(game_node->getPositionY() < (-430*game_node->getScale()+480*screen_size.height/screen_size.width)/myDSH->screen_convert_rate - myDSH->ui_zero_point.y-40.f)
					game_node->setPositionY((-430*game_node->getScale()+480*screen_size.height/screen_size.width)/myDSH->screen_convert_rate - myDSH->ui_zero_point.y-40.f);
			}
		}
	}
}

void DiaryZoomPopup::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter;
	CCTouch* touch;

	for(iter = pTouches->begin();iter != pTouches->end();++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		location = myDSH->wideWidthFixTouch(location);

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
//				first_img->setTouchEnabled(true);
				
				timeval time;
				gettimeofday(&time, NULL);
				long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
				CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);

				float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
				if(isAnimated == false && fabsf(spd_value) > 2 && !is_spin_mode)
				{
					moveSpeed_p = _spd;
					this->schedule(schedule_selector(DiaryZoomPopup::moveAnimation));
				}
			}
		}
	}
}

void DiaryZoomPopup::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{
	ccTouchesEnded(pTouches, pEvent);
}
