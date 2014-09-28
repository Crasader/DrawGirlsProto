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
#include "cocos-ext.h"
#include "IntSeries.h"
#include "jsoncpp/json.h"

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

class Jack;
class ControlCommon : public CCLayer
{
public:
	bool isStun;
	CT_Type mType;
	
	virtual void startControl();
	
	virtual void setTouchEnabled(bool t_b);
	
protected:
	Jack* myJack;
	bool is_control;
	CCObject* target_main;
	SEL_CallFunc delegate_readyBack;
	float device_rate;
	
	
	
	void myInit(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack);
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
class KSLabelTTF;
class ControlJoystickButton : public ControlCommon
{
public:
	static ControlJoystickButton* create(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack);
	
	virtual void setTouchEnabled(bool t_b);
	
	void stopMySchedule();
	bool isBacking;
	
	virtual void startControl();
	
	SEL_CallFunc pauseBackTracking;
	
	void resetTouch();
	void invisibleControl();
	
	void showDrawButtonTutorial();
	void hideDrawButtonTutorial();
	
	void offDrawButtonTutorial();
    
    virtual ~ControlJoystickButton();
	
private:
	bool is_show_draw_button_tutorial;
	CCSprite* draw_button_tutorial_img;
	CCSprite* draw_button_tutorial_ccb;
	
	IntDirection beforeDirection;
	IntDirection beforeSecondDirection;
	
//	IntDirection changing_direction;
//	IntDirection changing_second_direction;
	
	CCSprite* control_circle;
	CCSprite* control_ball;
	
	CCSprite* draw_button;
	KSLabelTTF* click_label;
	
	CCTouch* button_touch;
	CCTouch* joystick_touch;
	
	float regular_spectrum;
	float irregular_spectrum;
	
	bool isButtonAction;
	
	bool keep_is_draw_button_tutorial_on;
	
	CCBAnimationManager* button_ani;
	
	bool isEnableIrregularDirection;
	bool isControlJoystickNotFixed;
	bool isAlwaysInvisibleJoystick;
	int controlJoystickDirection;
	bool isDisableDrawButton;
	bool isDisableLineOver;
	bool isJoystickCenterNotFixed;
	double TouchOutWidth;
	double JOYSTICK_FOLLOW_DISTANCE;
	
	double minimumDistanceJ;
	double JoystickCenterLimit;
	
	float joystick_size_value;
	
	
	void myInit(CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack);
	
	void onButton(CCPoint t_location);
	void offButton();
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
    
//    KSLabelTTF* game_pad_state;
//    KSLabelTTF* game_pad_motion;
//    KSLabelTTF* game_pad_distance;
//    KSLabelTTF* game_pad_log;
    
    IntDirection game_pad_direction;
    int game_pad_draw_keycode;
    int game_pad_key;
    CCPoint directionToTouchPoint(IntDirection t_direction);
    void gamePadAction(Json::Value pad_data);
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
