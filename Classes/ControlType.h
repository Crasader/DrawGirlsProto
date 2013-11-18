//
//  ControlType.h
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 6..
//
//

#ifndef __DrawingJack__ControlType__
#define __DrawingJack__ControlType__

#include "cocos2d.h"
#include "Jack.h"
#include "DataStorageHub.h"
#include "GameData.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

using namespace cocos2d;
using namespace std;

typedef enum t_CT_Type{
	kCT_Type_gesture_original = 1,
	kCT_Type_Joystick_fix,
	kCT_Type_button_side,
	kCT_Type_Joystick,
	kCT_Type_Joystick_button
}CT_Type;

class ControlCommon : public CCLayer
{
public:
	bool isStun;
	CT_Type mType;
	
	void startControl()
	{
//		if(((CCNode*)target_main)->getTag() == 1)
			setTouchEnabled(true);
	}
	
	virtual void setTouchEnabled(bool t_b)
	{
		CCLayer::setTouchEnabled(t_b);
		is_control = t_b;
	}
	
protected:
	Jack* myJack;
	bool is_control;
	CCObject* target_main;
	SEL_CallFunc delegate_readyBack;
	float device_rate;
	
	
	
	void myInit(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
	{
		myJack = t_jack;
		isStun = false;
		is_control = false;
		
//		device_rate = myDSH->getDeviceRate();
		target_main = t_main;
		delegate_readyBack = d_readyBack;
	}
};

enum CBS_State{
	kCBS_State_move = 1,
	kCBS_State_moving,
	kCBS_State_stop,
	kCBS_State_return
};

enum CBS_Touch{
	kCBS_Touch_began = 1,
	kCBS_Touch_move,
	kCBS_Touch_end
};

//class ControlButtonSide : public ControlCommon
//{
//public:
//	static ControlButtonSide* create(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
//	{
//		ControlButtonSide* t_cbs = new ControlButtonSide();
//		t_cbs->myInit(t_main, d_readyBack, t_jack);
//		t_cbs->autorelease();
//		return t_cbs;
//	}
//	
//private:
//	CCNode* control_node;
//	CCSprite* left_spr;
//	CCSprite* right_spr;
//	CCSprite* down_spr;
//	CCSprite* up_spr;
//	CCSprite* center_spr;
//	CCSprite* ment_spr;
//	
//	CBS_State myState;
//	
//	CCPoint origin_position;
//	
//	
//	CCPoint beforePosition;
//	IntDirection beforeDirection;
//	
//	void changeState(CBS_State after_state);
//	bool moveEventStart;
//	bool isTouched;
//	void ingSchedule();
//	
//	void myInit(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
//	{
//		isTouched = false;
//		moveEventStart = false;
//		CCLayer::init();
//		ControlCommon::myInit(t_main, d_readyBack, t_jack);
//		mType = kCT_Type_button_side;
//		beforeDirection = directionStop;
//		
//		
//		origin_position = ccp(myDSH->getIntegerForKey(kDSH_Key_controlOriginX),myDSH->getIntegerForKey(kDSH_Key_controlOriginY));
//		
//		control_node = CCNode::create();
//		control_node->setPosition(origin_position);
//		addChild(control_node);
//		
//		left_spr = CCSprite::create("control_arrow.png");
//		left_spr->setColor(ccGRAY);
//		left_spr->setRotation(0);
//		left_spr->setPosition(ccp(-50,0));
//		control_node->addChild(left_spr);
//		
//		down_spr = CCSprite::create("control_arrow.png");
//		down_spr->setColor(ccGRAY);
//		down_spr->setRotation(-90);
//		down_spr->setPosition(ccp(0,-50));
//		control_node->addChild(down_spr);
//		
//		right_spr = CCSprite::create("control_arrow.png");
//		right_spr->setColor(ccGRAY);
//		right_spr->setRotation(180);
//		right_spr->setPosition(ccp(50,0));
//		control_node->addChild(right_spr);
//		
//		up_spr = CCSprite::create("control_arrow.png");
//		up_spr->setColor(ccGRAY);
//		up_spr->setRotation(90);
//		up_spr->setPosition(ccp(0,50));
//		control_node->addChild(up_spr);
//		
//		ment_spr = CCSprite::create("control_move_ment.png");
//		ment_spr->setVisible(false);
//		ment_spr->setPosition(ccp(0,80));
//		control_node->addChild(ment_spr);
//		
//		string filename;
//		if(myJack)
//		{
//			if(myJack->isDrawingOn)
//			{
//				if(myJack->isMoving)
//				{
//					filename = "control_stop.png";
//					myState = kCBS_State_stop;
//				}
//				else
//				{
//					filename = "control_return.png";
//					myState = kCBS_State_return;
//				}
//			}
//			else
//			{
//				if(myJack->isMoving)
//				{
//					filename = "control_stop.png";
//					myState = kCBS_State_stop;
//				}
//				else
//				{
//					filename = "control_move.png";
//					myState = kCBS_State_move;
//				}
//			}
//		}
//		else
//		{
//			filename = "control_move.png";
//			myState = kCBS_State_move;
//		}
//		
//		center_spr = CCSprite::create(filename.c_str());
//		center_spr->setColor(ccGRAY);
//		center_spr->setPosition(CCPointZero);
//		control_node->addChild(center_spr);
//		
//		schedule(schedule_selector(ControlButtonSide::ingSchedule), 0.1f);
//	}
//	
//	void touchAction(CCPoint t_p, CBS_Touch t_t);
//	
//	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
//	
//	virtual void registerWithTouchDispatcher(void);
//};

class ControlJoystickButton : public ControlCommon
{
public:
	static ControlJoystickButton* create(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
	{
		ControlJoystickButton* t_cjf = new ControlJoystickButton();
		t_cjf->myInit(t_main, d_readyBack, t_jack);
		t_cjf->autorelease();
		return t_cjf;
	}
	
	virtual void setTouchEnabled(bool t_b)
	{
		ControlCommon::setTouchEnabled(t_b);
		
		if(!t_b)
		{
			unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
			if(button_touch)
			{
//				draw_button->setColor(ccWHITE);
				offButton();
				button_touch = NULL;
			}
			
			if(joystick_touch)
			{
				if(myGD->gamescreen_type == kGT_rightUI)	control_circle->setPosition(ccp(50+myGD->boarder_value, 50));
				else										control_circle->setPosition(ccp(480-50-myGD->boarder_value, 50));;
				
				control_ball->setVisible(false);
				myJack->setTouchPointByJoystick(CCPointZero, directionStop, t_b);
				joystick_touch = NULL;
			}
		}
	}
	
	void stopMySchedule();
	bool isBacking;
	
	SEL_CallFunc pauseBackTracking;
	
private:
	IntDirection beforeDirection;
	IntDirection beforeSecondDirection;
	CCSprite* control_circle;
	CCSprite* control_ball;
	
	CCSprite* draw_button;
	
	CCTouch* button_touch;
	CCTouch* joystick_touch;
	
	bool isButtonAction;
	
	CCBAnimationManager* button_ani;
	
	void myInit(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
	{
		CCLayer::init();
		
		isButtonAction = false;
		button_touch = NULL;
		joystick_touch = NULL;
		isBacking = false;
		
		ControlCommon::myInit(t_main, d_readyBack, t_jack);
		beforeDirection = directionStop;
		control_circle = CCSprite::create("control_joystick_big_circle.png");
		control_circle->setScale(0.5f);
		control_circle->setVisible(false);
//		if(myGD->gamescreen_type == kGT_rightUI)	control_circle->setPosition(ccp(50+myGD->boarder_value, 50));
//		else										control_circle->setPosition(ccp(480-50-myGD->boarder_value, 50));;
		addChild(control_circle);
		
		control_ball = CCSprite::create("control_joystick_big_ball.png");
		control_ball->setScale(0.5f);
		control_ball->setVisible(false);
		addChild(control_ball);
		
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		draw_button = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("gameui_button.ccbi",this));
		button_ani = reader->getAnimationManager();
//		draw_button = CCSprite::create("ui_draw.png");
		if(myGD->gamescreen_type == kGT_rightUI)		draw_button->setPosition(ccp(480-25,25));
		else if(myGD->gamescreen_type == kGT_leftUI)	draw_button->setPosition(ccp(25,25));
		else								draw_button->setPosition(ccp(25,25));
		addChild(draw_button);
		
		mType = kCT_Type_Joystick_button;
	}
	
	void onButton()
	{
		button_ani->runAnimationsForSequenceNamed("cast1start");
	}
	
	void offButton()
	{
		button_ani->runAnimationsForSequenceNamed("cast1stop");
	}
	
	void touchAction(CCPoint t_p, bool t_b);
	
	void directionKeeping();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
//	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher(void);
};

//class ControlJoystick : public ControlCommon
//{
//public:
//	static ControlJoystick* create(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
//	{
//		ControlJoystick* t_cjf = new ControlJoystick();
//		t_cjf->myInit(t_main, d_readyBack, t_jack);
//		t_cjf->autorelease();
//		return t_cjf;
//	}
//	
//private:
//	IntDirection beforeDirection;
//	CCSprite* control_circle;
//	CCSprite* control_ball;
//	
//	
//	void myInit(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
//	{
//		CCLayer::init();
//		ControlCommon::myInit(t_main, d_readyBack, t_jack);
//		beforeDirection = directionStop;
//		control_circle = CCSprite::create("control_joystick_big_circle.png");
//		control_circle->setScale(0.5f);
//		control_circle->setVisible(false);
//		addChild(control_circle);
//		
//		control_ball = CCSprite::create("control_joystick_big_ball.png");
//		control_ball->setScale(0.5f);
//		control_ball->setVisible(false);
//		addChild(control_ball);
//		mType = kCT_Type_Joystick;
//	}
//	
//	void touchAction(CCPoint t_p, bool t_b);
//	
//	void directionKeeping();
//	
//	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
//	
//	virtual void registerWithTouchDispatcher(void);
//};
//
//class ControlJoystickFix : public ControlCommon
//{
//public:
//	static ControlJoystickFix* create(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
//	{
//		ControlJoystickFix* t_cjf = new ControlJoystickFix();
//		t_cjf->myInit(t_main, d_readyBack, t_jack);
//		t_cjf->autorelease();
//		return t_cjf;
//	}
//	
//private:
//	IntDirection beforeDirection;
//	CCSprite* control_spr;
//	
//	void myInit(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
//	{
//		CCLayer::init();
//		ControlCommon::myInit(t_main, d_readyBack, t_jack);
//		beforeDirection = directionStop;
//		control_spr = CCSprite::create("control.png");
//		control_spr->setPosition(ccp(320-70,70));
//		control_spr->setOpacity(100);
//		addChild(control_spr);
//		mType = kCT_Type_Joystick_fix;
//	}
//	
//	void touchAction(CCPoint t_p, bool t_b);
//	
//	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
//	
//	virtual void registerWithTouchDispatcher(void);
//};
//
//class ControlOriginalGesture : public ControlCommon
//{
//public:
//	static ControlOriginalGesture* create(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
//	{
//		ControlOriginalGesture* t_cog = new ControlOriginalGesture();
//		t_cog->myInit(t_main, d_readyBack, t_jack);
//		t_cog->autorelease();
//		return t_cog;
//	}
//	
//private:
//	bool is_gestured;
//	int gesture_cnt;
//	IntDirection beforeGesture;
//	
//	CCPoint touch_began_point;
//	
//	void myInit(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack)
//	{
//		CCLayer::init();
//		ControlCommon::myInit(t_main, d_readyBack, t_jack);
//		mType = kCT_Type_gesture_original;
//	}
//	
//	void resetTouchBeganPoint();
//	bool isSetTouchBeganPoint();
//	
//	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
//	
//	virtual void registerWithTouchDispatcher(void);
//};

#endif /* defined(__DrawingJack__ControlType__) */
