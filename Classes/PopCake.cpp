#include "PopCake.h"
#include "bustMorphing.h"
#include "FormSetter.h"

#include "StageImgLoader.h"
#include "StarGoldData.h"
#include "FormSetter.h"
#include "ks19937.h"
#include "DataStorageHub.h"
PopCake::PopCake()
{
	
}
PopCake::~PopCake()
{
	mySIL->removeTD();
}
//void PopCake::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool PopCake::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool PopCake::init()
{
	CCLayer::init();
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	auto mode_button = CommonButton::create("이동", 10, CCSizeMake(40, 30), CommonButtonOrange, -252);
	mode_button->setPosition(ccp(350, 250));
	mode_button->setFunction([=](CCObject* sender)
							{
								is_spin_mode = !is_spin_mode;
								if(is_spin_mode)
									mode_button->setTitle("회전");
								else
									mode_button->setTitle("이동");
							});
//	mode_button->setVisible(false);
	addChild(mode_button, 2);
	setTouchEnabled(true);
//	MyNode* n = MyNode::create(CCTextureCache::sharedTextureCache()->addImage("bmTest.png"));
//	n->setPosition(ccp(0, 0));
//	n->setContentSize(CCSizeMake(640, 860));
//	n->setScale(0.5);
//	n->setAnchorPoint(ccp(0.5, 0.5));
//	addChild(n);
	
	game_node = CCNode::create();
	game_node->setScale(1.f);
	game_node->setPosition(ccp(-myDSH->ui_zero_point.x,(-430*1.5f+480.f*screen_size.height/screen_size.width)/myDSH->screen_convert_rate - myDSH->ui_zero_point.y));
	addChild(game_node, 1);
	n = MyNode::create(); // CCTextureCache::sharedTextureCache()->addImage("bmTest.png"));
	n->setPosition(ccp(160, 215));
	//n->setContentSize(CCSizeMake(640, 860));
//	n->setScale(0.5);
	n->setAnchorPoint(ccp(0.5f, .5f));
//	addChild(n);
	
	game_node->addChild(n, 1);
	
	int randomValue = ks19937::getIntValue(0, 2);
	std::string desc;
	
	if(randomValue == 0)
	{
		desc += "승용(bustmorphing1)";
		formSetter = "bustmorphing1";
	}
	else if(randomValue == 1)
	{
		desc += "남희(bustmorphing2)";
		formSetter = "bustmorphing2";
		
	}
	else if(randomValue == 2)
	{
		desc += "정순(bustmorphing3)";
		formSetter = "bustmorphing3";
	}
	
	cb = CommonButton::create(desc + "load", 20, CCSizeMake(100, 30), CommonButtonOrange,
																					0);
	addChild(cb, 1);
	cb->setPosition(ccp( 420, 300));
	cb->setFunction([=](CCObject* obj){
		
		FormSetter::get()->requestFormDataOnce([=](){
			Json::Value v =  FormSetter::get()->getFormData(formSetter);

			mySIL->downloadImg(v["original"].asString(), 999, "morphing_img.png", this, callfunc_selector(PopCake::successOriginalAction), this, callfunc_selector(PopCake::failOriginalAction));
		});
		
		cb->setVisible(false);
	});
	
	CCLOG("cb2");
	cb2 = CommonButton::create("load RGB 11 ", 20, CCSizeMake(100, 35), CommonButtonOrange,
																					 0);
	addChild(cb2, 1);
	cb2->setPosition(ccp( 420, 260));
	cb2->setFunction([=](CCObject* obj){
		
		FormSetter::get()->requestFormDataOnce([=](){
			Json::Value v =  FormSetter::get()->getFormData(formSetter);
			
			mySIL->downloadImg(v["rgb"].asString(), 999, "morphing_rgb.png", this, callfunc_selector(PopCake::successRgbAction), this, callfunc_selector(PopCake::failRgbAction));
		});
		
		cb2->setVisible(false);
	});
	
//	{
//		CommonButton* cb = CommonButton::create("left", 20, CCSizeMake(100, 35), CommonButtonOrange,
//																						0);
//		addChild(cb, 1);
//		cb->setPosition(ccp( 420, 220));
//		cb->setFunction([=](CCObject* obj){
//			n->setImageRotationDegree(n->getImageRotationDegree() - 10);
//		});
//		
//		CommonButton* cb2 = CommonButton::create("right", 20, CCSizeMake(100, 35), CommonButtonOrange,
//																						 0);
//		addChild(cb2, 1);
//		cb2->setPosition(ccp( 420, 180));
//		cb2->setFunction([=](CCObject* obj){
//			n->setImageRotationDegree(n->getImageRotationDegree() + 10);
//		});
//	}
//	{
//		{
//			CommonButton* cb = CommonButton::create("up", 20, CCSizeMake(100, 35), CommonButtonOrange,
//																							0);
//			addChild(cb, 1);
//			cb->setPosition(ccp( 420, 140));
//			cb->setFunction([=](CCObject* obj){
////				n->setPosition(n->getPosition() + ccp(0, 10));
//				n->setImageRotationDegreeX(n->getImageRotationDegreeX() - 10);
//			});
//			
//			CommonButton* cb2 = CommonButton::create("down", 20, CCSizeMake(100, 35), CommonButtonOrange,
//																							 0);
//			addChild(cb2, 1);
//			cb2->setPosition(ccp( 420, 100));
//			cb2->setFunction([=](CCObject* obj){
////				n->setPosition(n->getPosition() - ccp(0, 10));
//				n->setImageRotationDegreeX(n->getImageRotationDegreeX() + 10);
//			});
//		}
//	}
//	{
//		{
//			CommonButton* cb = CommonButton::create("zoom in", 20, CCSizeMake(100, 35), CommonButtonOrange,
//																							0);
//			addChild(cb, 1);
//			cb->setPosition(ccp( 420, 60));
//			cb->setFunction([=](CCObject* obj){
//				n->setScale(n->getScale() + 0.1f);
//			});
//			
//			CommonButton* cb2 = CommonButton::create("zoom out", 20, CCSizeMake(100, 30), CommonButtonOrange,
//																							 0);
//			addChild(cb2, 1);
//			cb2->setPosition(ccp( 420, 20));
//			cb2->setFunction([=](CCObject* obj){
//				n->setScale(n->getScale() - 0.1f);
//
//			});
//		}
//	}
	return true;
}

void PopCake::successOriginalAction()
{
	n->init(mySIL->addImage("morphing_img.png"));
}



void PopCake::failOriginalAction()
{
	cb->setVisible(true);
	
	CCLabelTTF* fail_label = CCLabelTTF::create("오리지날 다운로드 실패", mySGD->getFont().c_str(), 12);
	fail_label->setPosition(ccp(240,160));
	addChild(fail_label);
	
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(fail_label, callfunc_selector(CCLabelTTF::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	fail_label->runAction(t_seq);
}

void PopCake::successRgbAction()
{
	cb2->setVisible(true);
	
	n->putBasicInfomation();	// 기본정보 들어가게.
	n->loadRGB(CCFileUtils::sharedFileUtils()->fullPathForFilename("bmTest2.png").c_str()); // 실루엣 z 정보 넣는 곳.
	n->triangulationWithPoints();
}

void PopCake::failRgbAction()
{
	cb2->setVisible(true);
	
	CCLabelTTF* fail_label = CCLabelTTF::create("알지비 다운로드 실패", mySGD->getFont().c_str(), 12);
	fail_label->setPosition(ccp(240,160));
	addChild(fail_label);
	
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(fail_label, callfunc_selector(CCLabelTTF::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	fail_label->runAction(t_seq);
}

void PopCake::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
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

		this->unschedule(schedule_selector(PopCake::moveAnimation));

		isAnimated=false;

		if(multiTouchData.size() >= 1)
		{
			n->setTouchEnabled(false);
		}
		
		if(multiTouchData.size() == 1)
		{
			first_touch_time = touchStartTime;
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

void PopCake::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
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
					this->unschedule(schedule_selector(PopCake::moveAnimation));
					moveSpeed_p = CCPointZero;
					isAnimated = false;
					
					CCPoint rotate_sub = ccpSub(location, touch_p);
					
					float rotation_degree = n->getImageRotationDegree() + rotate_sub.x/5.f;
					if(rotation_degree > 60.f)
						rotation_degree = 60.f;
					else if(rotation_degree < -60.f)
						rotation_degree = -60.f;
					n->setImageRotationDegree(rotation_degree);
					
					float rotation_degreeX = n->getImageRotationDegreeX() - rotate_sub.y/5.f;
					if(rotation_degreeX > 60.f)
						rotation_degreeX = 60.f;
					else if(rotation_degreeX < -60.f)
						rotation_degreeX = -60.f;
					n->setImageRotationDegreeX(rotation_degreeX);
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

void PopCake::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
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
				
				if(int(((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - first_touch_time) < 100000)
				{
					n->ccTouchEnded(touch, pEvent);
				}
				else
				{
					long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
					CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
					
					float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
					if(isAnimated == false && fabsf(spd_value) > 2 && !is_spin_mode)
					{
						moveSpeed_p = _spd;
						this->schedule(schedule_selector(PopCake::moveAnimation));
					}
				}
			}
		}
	}
}

void PopCake::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{
	ccTouchesEnded(pTouches, pEvent);
}
void PopCake::moveAnimation()
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
		this->unschedule(schedule_selector(PopCake::moveAnimation));
		moveSpeed_p = CCPointZero;
		isAnimated = false;
	}
	this->moveListXY(ccpMult(moveSpeed_p, -1));
}
void PopCake::moveListXY(CCPoint t_p)
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
