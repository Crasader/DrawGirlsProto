//
//  CardViewScene.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 4..
//
//

#include "CardViewScene.h"
#include "DataStorageHub.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "utf8.h"
#include "CommonButton.h"
#include "bustMorphing.h"
#include "RankUpPopup.h"
#include "GraySprite.h"
#include "CCMenuLambda.h"
#include "BuyMorphingPopup.h"

#define CV_SCROLL_SPEED_MAX_BASE	20
#define CV_SCROLL_SPEED_DECEASE_BASE	0.2f

CCScene* CardViewScene::scene()
{
    CCScene *scene = CCScene::create();
    CardViewScene *layer = CardViewScene::create();
    scene->addChild(layer);
	
    return scene;
}

enum CV_Zorder{
	kCV_Z_back = 0,
	kCV_Z_first_img,
	kCV_Z_next_button
};

bool CardViewScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	CCLayer* top_bottom_layer = CCLayer::create();
	top_bottom_layer->setPosition(ccp(0, 0));
	addChild(top_bottom_layer, kCV_Z_back);
	
	CCSpriteBatchNode* side_back = CCSpriteBatchNode::create("ingame_side_pattern.png");
	top_bottom_layer->addChild(side_back);
	
	CCSize pattern_size = side_back->getTexture()->getContentSize();
	
	for(int i=0;i*pattern_size.width < 480;i++)
	{
		for(int j=0;j*pattern_size.height < myDSH->ui_top;j++)
		{
			CCSprite* t_pattern = CCSprite::createWithTexture(side_back->getTexture());
			t_pattern->setAnchorPoint(ccp(0,0));
			t_pattern->setPosition(ccp(i*pattern_size.width,j*pattern_size.height));
			side_back->addChild(t_pattern);
		}
	}
	
	game_node = CCNode::create();
	game_node->setScale(1.5f);
	addChild(game_node, kCV_Z_first_img);
	
	int card_number = mySGD->selected_collectionbook;
	
	is_morphing = mySGD->isCardMorphing(card_number);
	
	CCLog("why two2");
	first_img = MyNode::create(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", card_number)->getCString()));

	if(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()))
		first_img->loadRGB(mySIL->getDocumentPath() + CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()); // 실루엣 z 정보 넣는 곳.

	
	first_img->setPosition(ccp(160,215));
	first_img->setTouchEnabled(false);
	game_node->addChild(first_img, kCV_Z_first_img);
	
	if(mySGD->is_safety_mode)
	{
		safety_img = EffectSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_invisible.png", card_number)->getCString()));
		safety_img->setSilhouetteConvert(0);
		safety_img->setPosition(ccp(160, 215));
		game_node->addChild(safety_img, kCV_Z_first_img);
	}
	
	CCPoint center_position = ccp(160,215);
	
	CCSprite* top_case = CCSprite::create("diary_frame_top.png");
	top_case->setPosition(ccpAdd(center_position, ccp(0,215)));
	game_node->addChild(top_case, kCV_Z_first_img);
	
	CCSprite* bottom_case = CCSprite::create("diary_frame_bottom.png");
	bottom_case->setPosition(ccpAdd(center_position, ccp(0,-215)));
	game_node->addChild(bottom_case, kCV_Z_first_img);
	
	CCSprite* left_case = CCSprite::create("diary_frame_left.png");
	left_case->setPosition(ccpAdd(center_position, ccp(-160,0)));
	game_node->addChild(left_case, kCV_Z_first_img);
	
	CCSprite* right_case = CCSprite::create("diary_frame_right.png");
	right_case->setPosition(ccpAdd(center_position, ccp(160,0)));
	game_node->addChild(right_case, kCV_Z_first_img);
	
	
//	zoom_img = CCSprite::create("ending_expand.png");
	
	
	CCPoint morphing_position = ccp(435,45);
	
	string morphing_filename;
	auto liveGirl = [=](){
		is_actioned = false;
		is_morphing = true;
		
		buy_morphing->removeFromParent();
		
		morphing_img->removeFromParent();
		morphing_img = KS::loadCCBI<CCSprite*>(this, "morphing_heart_on.ccbi").first;
		morphing_img->setPosition(morphing_position);
		addChild(morphing_img, kCV_Z_next_button);
		
		CCTouch* t_touch = new CCTouch();
		t_touch->setTouchInfo(0, 0, 0);
		t_touch->autorelease();
		
		first_img->ccTouchEnded(t_touch, NULL);
		
		auto tuto = KS::loadCCBI<CCSprite*>(this, "tutorial_touch.ccbi");
		
		zoom_img = tuto.first;
		tuto.second->runAnimationsForSequenceNamed("Default Timeline");
		
		
		zoom_img->setPosition(ccp(240, myDSH->ui_center_y));
		addChild(zoom_img, kCV_Z_next_button);
	};
	if(!is_morphing)
	{
		buy_morphing = CommonButton::create("", 10, CCSizeMake(80, 50), CommonButtonLightPupple, -160);
		buy_morphing->setPosition(morphing_position);
		buy_morphing->setFunction([=](CCObject* sender)
															{
																if(!is_actioned)
																{
																	is_actioned = true;
																	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																	
																	BuyMorphingPopup* t_popup = BuyMorphingPopup::create(-200, [=](){
																		is_actioned = false;
																	}, liveGirl);
																	addChild(t_popup, 999);
																}
															});
		addChild(buy_morphing, kCV_Z_next_button);
		
		morphing_filename = "morphing_heart_off.ccbi";
	}
	else
	{
		morphing_filename = "morphing_heart_on.ccbi";
		
		auto tuto = KS::loadCCBI<CCSprite*>(this, "tutorial_touch.ccbi");
		
		zoom_img = tuto.first;
		tuto.second->runAnimationsForSequenceNamed("Default Timeline");
		
		
		zoom_img->setPosition(ccp(240, myDSH->ui_center_y));
		addChild(zoom_img, kCV_Z_next_button);
	}
	
	
	
	morphing_img = KS::loadCCBI<CCSprite*>(this, morphing_filename.c_str()).first;
	morphing_img->setPosition(morphing_position);
	addChild(morphing_img, kCV_Z_next_button);
	
	if(!is_morphing)
	{
		BuyMorphingPopup* t_popup = BuyMorphingPopup::create(-200, [=](){is_actioned = false;}, liveGirl);
		addChild(t_popup, 999);
	}
	
	
	next_button = CommonButton::createCloseButton(-160);
	next_button->setFunction([=](CCObject* sender){menuAction(sender);});
	next_button->setPosition(ccp(480-35,myDSH->ui_top-35));
	next_button->setVisible(false);
	addChild(next_button, kCV_Z_next_button);
	
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
	addChild(mode_button, kCV_Z_next_button);
	
	//	is_touched_menu = false;
	is_actioned = true;
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	minimum_scale = (screen_size.height*320)/(screen_size.width*430)*1.5f;
	
	game_node->setPosition(ccp(0,-430*1.5f+480.f*screen_size.height/screen_size.width));
	
	return true;
}

void CardViewScene::onEnterTransitionDidFinish()
{
	startTouchAction();
	
//	CCMoveTo* move1 = CCMoveTo::create(1.f, ccp(0,0));
//	CCDelayTime* delay1 = CCDelayTime::create(1.f);
//	
//	CCMoveTo* move2 = CCMoveTo::create(0.7f, ccp((480.f-320.f*minimum_scale)/2.f, 0));
//	CCScaleTo* t_scale = CCScaleTo::create(0.7f, minimum_scale);
//	CCSpawn* t_spawn = CCSpawn::create(move2, t_scale, NULL);
//	
//	//	CCMoveTo* move2 = CCMoveTo::create(1.f, ccp(0,-430*1.5f+480.f*screen_size.height/screen_size.width));
//	CCDelayTime* delay2 = CCDelayTime::create(1.f);
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ZoomScript::startScript));
//	game_node->runAction(CCSequence::create(move1, delay1, t_spawn, delay2, t_call, NULL));
}

void CardViewScene::startTouchAction()
{
	is_actioned = false;
	setTouchEnabled(true);
	next_button->setVisible(true);
	//	mode_button->setVisible(true);
	
	//	target_node->setTouchEnabled(true);
	
	is_scrolling = false;
	is_before_scrolling = is_scrolling;
	
	save_position = game_node->getPosition();
	schedule(schedule_selector(CardViewScene::moveChecking));
}

void CardViewScene::moveChecking()
{
	CCPoint after_position = game_node->getPosition();
	
	if(is_scrolling)
	{
		if(is_morphing)
			first_img->movingDistance(ccpSub(after_position, save_position));
		is_before_scrolling = is_scrolling;
	}
	else if(is_before_scrolling)
	{
		is_before_scrolling = false;
		if(is_morphing)
			first_img->movingDistance(CCPointZero);
	}
	save_position = after_position;
}

void CardViewScene::menuAction(CCObject *sender)
{
	if(!is_actioned)
	{
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		
		unschedule(schedule_selector(CardViewScene::moveChecking));
		
		is_actioned = true;
		next_button->setVisible(false);
		mode_button->setVisible(false);
		setTouchEnabled(false);
		first_img->setTouchEnabled(false);
		unschedule(schedule_selector(CardViewScene::moveAnimation));
		
		
		
//		CCTransitionFadeTR* t_trans = CCTransitionFadeTR::create(1.f, ZoomScript::scene());
		CCDirector::sharedDirector()->popScene();
	}
}

void CardViewScene::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	
	pDispatcher->addStandardDelegate(this, -150);
}

void CardViewScene::moveListXY(CCPoint t_p)
{
	if(t_p.x > CV_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = CV_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.x < -CV_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.x = -CV_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
	if(t_p.y > CV_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = CV_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	if(t_p.y < -CV_SCROLL_SPEED_MAX_BASE/game_node->getScale())		t_p.y = -CV_SCROLL_SPEED_MAX_BASE/game_node->getScale();
	
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

void CardViewScene::moveAnimation()
{
	isAnimated = true;
	
	if(moveSpeed_p.x >= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())			moveSpeed_p.x -= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else if(moveSpeed_p.x <= -CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())	moveSpeed_p.x += CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else							moveSpeed_p.x = 0;
	
	if(moveSpeed_p.y >= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())			moveSpeed_p.y -= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else if(moveSpeed_p.y <= CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale())	moveSpeed_p.y += CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale();
	else							moveSpeed_p.y = 0;
	
	if((moveSpeed_p.x < CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() && moveSpeed_p.x > -CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() &&
		moveSpeed_p.y < CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale() && moveSpeed_p.y > -CV_SCROLL_SPEED_DECEASE_BASE/game_node->getScale()) || isAnimated == false)
	{
		this->unschedule(schedule_selector(CardViewScene::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}

void CardViewScene::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
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
		
		this->unschedule(schedule_selector(CardViewScene::moveAnimation));
		
		isAnimated=false;
		
		if(multiTouchData.size() >= 1)
		{
			first_img->setTouchEnabled(false);
		}
		
		if(multiTouchData.size() == 1)
		{
			first_touch_time = touchStartTime;
			first_touch_p = (int)touch;
			first_touch_point = location;
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

void CardViewScene::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
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
			
			if((int)touch == first_touch_p)
			{
				if(first_touch_point.getDistanceSq(location) > 100.f)
				{
					first_touch_time = -200000;
				}
			}
			
			if(multiTouchData.size() == 1)
			{
				//				if(is_touched_menu)
				//				{
				//					next_button->ccTouchMoved(touch, pEvent);
				//				}
				
				if(is_spin_mode)
				{
					this->unschedule(schedule_selector(CardViewScene::moveAnimation));
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

void CardViewScene::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
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
				
				if((int)touch == first_touch_p && (((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - first_touch_time) < 200000)
				{
					if(is_morphing)
						first_img->ccTouchEnded(touch, pEvent);
				}
				else
				{
					unsigned long long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
					CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
					
					float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
					if(isAnimated == false && fabsf(spd_value) > 2 && !is_spin_mode)
					{
						moveSpeed_p = _spd;
						this->schedule(schedule_selector(CardViewScene::moveAnimation));
					}
				}
			}
		}
	}
}

void CardViewScene::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{
	ccTouchesEnded(pTouches, pEvent);
}
