//
//  ControlType.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 6..
//
//

#include "ControlType.h"
#include "Jack.h"
#include "DataStorageHub.h"
#include "GameData.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "StyledLabelTTF.h"
#include "CommonAnimation.h"

///////////////////////////////////////////////// Button Side ////////////////////////////////////////////////////////////////////
//void ControlButtonSide::ingSchedule()
//{
//	if(isTouched)
//	{
//		if(!myJack->willBackTracking && !isStun)
//		{
//			touchAction(beforePosition, kCBS_Touch_move);
//		}
//	}
//	
//	if(myState != kCBS_State_move && myState != kCBS_State_moving && !myJack->isMoving && !myJack->isDrawingOn && myJack->getJackState() == jackStateNormal)
//	{
//		changeState(kCBS_State_move);
//	}
//}
//
//void ControlButtonSide::changeState(CBS_State after_state)
//{
//	if(after_state == kCBS_State_move)
//	{
//		left_spr->setColor(ccGRAY);
//		down_spr->setColor(ccGRAY);
//		right_spr->setColor(ccGRAY);
//		up_spr->setColor(ccGRAY);
//		center_spr->setColor(ccGRAY);
//		center_spr->setTexture(CCTextureCache::sharedTextureCache()->addImage("control_move.png"));
//		ment_spr->setVisible(false);
//	}
//	else if(after_state == kCBS_State_moving)
//	{
//		ment_spr->setVisible(true);
//	}
//	else if(after_state == kCBS_State_stop)
//	{
//		center_spr->setTexture(CCTextureCache::sharedTextureCache()->addImage("control_stop.png"));
//	}
//	else if(after_state == kCBS_State_return)
//	{
//		center_spr->setTexture(CCTextureCache::sharedTextureCache()->addImage("control_return.png"));
//	}
//	
//	myState = after_state;
//}
//
//#define minimumDistance2	30.0
//void ControlButtonSide::touchAction(CCPoint t_p, CBS_Touch t_t)
//{
//	CCSize distanceSize = CCSizeMake(t_p.x - origin_position.x, t_p.y - origin_position.y);
//	float distanceValue = sqrt(pow(distanceSize.width, 2.0) + pow(distanceSize.height, 2.0));
//	float angle = atan2(distanceSize.height, distanceSize.width)/M_PI*180.0; // -180 ~ 180
//	
//	if(distanceValue > minimumDistance2)
//	{
//		if(myState == kCBS_State_moving)
//		{
//			origin_position = t_p;
//			if(origin_position.x < 80)		origin_position.x = 80;
//			if(origin_position.x > 240)		origin_position.x = 240;
//			if(origin_position.y < 80)		origin_position.y = 80;
//			if(origin_position.y > 350)		origin_position.y = 350;
//			control_node->setPosition(origin_position);
//			
//			if(t_t == kCBS_Touch_end)
//			{
//				myDSH->setIntegerForKey(kDSH_Key_controlOriginX, origin_position.x);
//				myDSH->setIntegerForKey(kDSH_Key_controlOriginY, origin_position.y);
//				changeState(kCBS_State_move);
//			}
//			return;
//		}
//		else if(myState == kCBS_State_move)
//		{
//			changeState(kCBS_State_stop);
//		}
//		else if(myState == kCBS_State_return)
//		{
//			changeState(kCBS_State_stop);
//		}
//		
//		IntDirection angleDirection;
//		IntDirection secondDirection;
//		
//		IntPoint jackPoint = myGD->getJackPoint();
//		
//		if(angle < -135.f)
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < -90.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else if(angle < -45.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 0.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < 45.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		else if(angle < 90.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 135.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		
//		myJack->changeDirection(angleDirection, secondDirection);
//		if(angleDirection != beforeDirection)
//		{
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//			beforeDirection = angleDirection;
//			
//			left_spr->setColor(ccGRAY);
//			right_spr->setColor(ccGRAY);
//			down_spr->setColor(ccGRAY);
//			up_spr->setColor(ccGRAY);
//			center_spr->setColor(ccGRAY);
//			
//			if(angleDirection == directionLeft)
//			{
//				left_spr->setColor(ccWHITE);
//			}
//			else if(angleDirection == directionRight)
//			{
//				right_spr->setColor(ccWHITE);
//			}
//			else if(angleDirection == directionDown)
//			{
//				down_spr->setColor(ccWHITE);
//			}
//			else if(angleDirection == directionUp)
//			{
//				up_spr->setColor(ccWHITE);
//			}
//		}
//	}
//	else
//	{
//
//		
//		center_spr->setColor(ccWHITE);
//		if(myState == kCBS_State_moving)
//		{
//			origin_position = t_p;
//			if(origin_position.x < 80)		origin_position.x = 80;
//			if(origin_position.x > 240)		origin_position.x = 240;
//			if(origin_position.y < 80)		origin_position.y = 80;
//			if(origin_position.y > 350)		origin_position.y = 350;
//			control_node->setPosition(origin_position);
//			
//			if(t_t == kCBS_Touch_end)
//			{
//				myDSH->setIntegerForKey(kDSH_Key_controlOriginX, origin_position.x);
//				myDSH->setIntegerForKey(kDSH_Key_controlOriginY, origin_position.y);
//				changeState(kCBS_State_move);
//			}
//			return;
//		}
//		else if(myState == kCBS_State_move && t_t == kCBS_Touch_began)
//		{
//			changeState(kCBS_State_moving);
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//		}
//		else if(myState == kCBS_State_stop && t_t == kCBS_Touch_end)
//		{
//			myJack->changeDirection(directionStop, directionStop);
//			if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
//				changeState(kCBS_State_return);
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//		}
//		else if(myState == kCBS_State_return && t_t == kCBS_Touch_end)
//		{
//			changeState(kCBS_State_move);
//			(target_main->*delegate_readyBack)();
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//		}
//		
//		beforeDirection = directionStop;
//		left_spr->setColor(ccGRAY);
//		right_spr->setColor(ccGRAY);
//		down_spr->setColor(ccGRAY);
//		up_spr->setColor(ccGRAY);
//	}
//}
//
//bool ControlButtonSide::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	if(!myJack->willBackTracking &&
//	   location.y < 320 &&
//	   location.y > 0 &&
//	   location.x > 0 &&
//	   location.x < 480)
//	{
//		isTouched = true;
//		beforePosition = location;
//		touchAction(location, kCBS_Touch_began);
//	}
//	
//	return true;
//}
//
//void ControlButtonSide::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	if(!myJack->willBackTracking &&
//	   location.y < 320 &&
//	   location.y > 0 &&
//	   location.x > 0 &&
//	   location.x < 480)
//	{
//		isTouched = true;
//		beforePosition = location;
//		touchAction(location, kCBS_Touch_move);
//	}
//}
//
//void ControlButtonSide::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	if(!myJack->willBackTracking &&
//	   location.y < 320 &&
//	   location.y > 0 &&
//	   location.x > 0 &&
//	   location.x < 480)
//	{
//		touchAction(location, kCBS_Touch_end);
//	}
//	
//	left_spr->setColor(ccGRAY);
//	right_spr->setColor(ccGRAY);
//	down_spr->setColor(ccGRAY);
//	up_spr->setColor(ccGRAY);
//	isTouched = false;
//	beforePosition = ccp(160,80);
//}
//
//void ControlButtonSide::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	if(!myJack->willBackTracking &&
//	   location.y < 320 &&
//	   location.y > 0 &&
//	   location.x > 0 &&
//	   location.x < 480)
//	{
//		touchAction(location, kCBS_Touch_end);
//	}
//	
//	left_spr->setColor(ccGRAY);
//	right_spr->setColor(ccGRAY);
//	down_spr->setColor(ccGRAY);
//	up_spr->setColor(ccGRAY);
//	isTouched = false;
//	beforePosition = ccp(160,80);
//}
//
//void ControlButtonSide::registerWithTouchDispatcher()
//{
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
//}

//////////////////////////////////////////////// Joystick Button ///////////////////////////////////////////////////////////////////////////////

const float CONTROL_OUT_DISTANCE = 150.f;
const float CONTROL_IN_TIME = 0.5f;

void ControlJoystickButton::touchAction(CCPoint t_p, bool t_b)
{
	
	//조이스틱이 십자가 형태로 움직이도록 함.
	if(!isEnableIrregularDirection)
	{
		if(beforeDirection == directionDown || beforeDirection == directionUp){
			control_ball->setPosition(ccp(control_circle->getPositionX(),control_ball->getPositionY()));
		}else if(beforeDirection == directionRight || beforeDirection == directionLeft){
			control_ball->setPosition(ccp(control_ball->getPositionX(),control_circle->getPositionY()));
		}
	}
	
	
	CCPoint distancePoint = ccp(t_p.x - control_circle->getPositionX(), t_p.y - control_circle->getPositionY());
	float distanceValue = sqrtf(powf(distancePoint.x, 2.f) + powf(distancePoint.y, 2.f));
	float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
	
	//조이스틱 보정하기
	if(!t_b && distanceValue<20*joystick_size_value){
		if(beforeDirection == directionLeft || beforeDirection == directionRight){
			if(distancePoint.x<0){
				beforeDirection=directionLeft;
			}else if(distancePoint.x>0){
				beforeDirection=directionRight;
			}
		}
		
		if(beforeDirection == directionUp || beforeDirection == directionDown){
			if(distancePoint.y<0){
				beforeDirection=directionDown;
			}else if(distancePoint.y>0){
				beforeDirection=directionUp;
			}
		}
		
		myJack->changeDirection(beforeDirection, beforeDirection);
        game_pad_direction = beforeDirection;
//        game_pad_distance->setString(ccsf("368 changeDirection : %d , %d | rand : %d", beforeDirection, beforeDirection, rand()));
		return;
	}
	
	
	if(myJack->isStun)// || myJack->willBackTracking || myJack->getJackState() == jackStateBackTracking)
	{
		myJack->changeDirection(directionStop, directionStop);
		beforeDirection = directionStop;
		beforeSecondDirection = directionStop;
		unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
		myJack->setTouchPointByJoystick(CCPointZero, directionStop, true);
        game_pad_direction = beforeDirection;
//        game_pad_distance->setString(ccsf("381 changeDirection : %d , %d | rand : %d", directionStop, directionStop, rand()));
//		joystick_touch = NULL;
		return;
	}
	
	IntDirection angleDirection;
	IntDirection secondDirection;
	
	if(t_b)
	{
		if(distanceValue > minimumDistanceJ && (t_p.x > 480.f-TouchOutWidth || t_p.x < TouchOutWidth || t_p.y > myDSH->ui_top-TouchOutWidth || t_p.y < TouchOutWidth))
		{
			CCLOG("Touch Out Action!!!");
			
			if(!myDSH->getBoolForKey(kDSH_Key_isOffJoystickVib))
				GraphDog::get()->vibAction();
			
//			IntPoint jackPoint = myGD->getJackPoint();
//			
//			if(angle < -180.f+regular_spectrum/2.f)
//			{
//				angleDirection = directionLeft;
//				if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//				else												secondDirection = directionDown;
//			}
//			else if(angle < -180.f+(regular_spectrum+irregular_spectrum)/2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionLeftDown;
//					if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionDown;
//					else												secondDirection = directionLeft;
//				}
//				else
//				{
//					angleDirection = directionLeft;
//					if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//					else												secondDirection = directionDown;
//				}
//			}
//			else if(angle < -180.f+regular_spectrum/2.f+irregular_spectrum)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionLeftDown;
//					if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionLeft;
//					else												secondDirection = directionDown;
//				}
//				else
//				{
//					angleDirection = directionDown;
//					if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//					else												secondDirection = directionLeft;
//				}
//			}
//			else if(angle < -180.f+regular_spectrum+irregular_spectrum)
//			{
//				angleDirection = directionDown;
//				if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//				else												secondDirection = directionLeft;
//			}
//			else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum)
//			{
//				angleDirection = directionDown;
//				if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//				else												secondDirection = directionRight;
//			}
//			else if(angle < -180.f+(regular_spectrum+irregular_spectrum)*3.f/2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionRightDown;
//					if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionRight;
//					else												secondDirection = directionDown;
//				}
//				else
//				{
//					angleDirection = directionDown;
//					if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//					else												secondDirection = directionRight;
//				}
//			}
//			else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionRightDown;
//					if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionDown;
//					else												secondDirection = directionRight;
//				}
//				else
//				{
//					angleDirection = directionRight;
//					if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//					else												secondDirection = directionDown;
//				}
//			}
//			else if(angle < 0.f)
//			{
//				angleDirection = directionRight;
//				if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//				else												secondDirection = directionDown;
//			}
//			else if(angle < regular_spectrum/2.f)
//			{
//				angleDirection = directionRight;
//				if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//				else												secondDirection = directionUp;
//			}
//			else if(angle < regular_spectrum/2.f+irregular_spectrum/2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionRightUp;
//					if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionUp;
//					else												secondDirection = directionRight;
//				}
//				else
//				{
//					angleDirection = directionRight;
//					if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//					else												secondDirection = directionUp;
//				}
//			}
//			else if(angle < regular_spectrum/2.f+irregular_spectrum)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionRightUp;
//					if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionRight;
//					else												secondDirection = directionUp;
//				}
//				else
//				{
//					angleDirection = directionUp;
//					if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//					else												secondDirection = directionRight;
//				}
//			}
//			else if(angle < regular_spectrum+irregular_spectrum)
//			{
//				angleDirection = directionUp;
//				if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//				else												secondDirection = directionRight;
//			}
//			else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum)
//			{
//				angleDirection = directionUp;
//				if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//				else												secondDirection = directionLeft;
//			}
//			else if(angle < (regular_spectrum+irregular_spectrum)*3.f/2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionLeftUp;
//					if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionLeft;
//					else												secondDirection = directionUp;
//				}
//				else
//				{
//					angleDirection = directionUp;
//					if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//					else												secondDirection = directionLeft;
//				}
//			}
//			else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
//			{
//				if(isEnableIrregularDirection)
//				{
//					angleDirection = directionLeftUp;
//					if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionUp;
//					else												secondDirection = directionLeft;
//				}
//				else
//				{
//					angleDirection = directionLeft;
//					if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//					else												secondDirection = directionUp;
//				}
//			}
//			else
//			{
//				angleDirection = directionLeft;
//				if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//				else												secondDirection = directionUp;
//			}
//			
//			
//			if(myJack->isDrawingOn && !isEnableIrregularDirection){
//					secondDirection = angleDirection;
//			}
//			
//			if(angleDirection != beforeDirection || isButtonAction)
//			{
//				isButtonAction = false;
//				myJack->changeDirection(angleDirection, secondDirection);
//				schedule(schedule_selector(ControlJoystickButton::directionKeeping));
//				//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//				beforeDirection = angleDirection;
//				beforeSecondDirection = secondDirection;
//			}
//			else if(beforeSecondDirection != secondDirection)
//			{
//				myJack->changeDirection(angleDirection, secondDirection);
//				schedule(schedule_selector(ControlJoystickButton::directionKeeping));
//				beforeDirection = angleDirection;
//				beforeSecondDirection = secondDirection;
//			}
		}
//		else
//		{
			myJack->changeDirection(directionStop, directionStop);
			beforeDirection = directionStop;
			beforeSecondDirection = directionStop;
			unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
			myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
            game_pad_direction = beforeDirection;
//            game_pad_distance->setString(ccsf("599 changeDirection : %d , %d | rand : %d", directionStop, directionStop, rand()));
			return;
//		}
	}
	
	if(button_touch && !myJack->isDie && !myJack->isDrawingOn && myJack->getJackState() == jackStateNormal)
	{
		myJack->isDrawingOn = true;
		myJack->setJackState(jackStateDrawing);
	}
	
	if(distanceValue > minimumDistanceJ)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		
		if(angle < -180.f+regular_spectrum/2.f)
		{
			angleDirection = directionLeft;
			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
			else												secondDirection = directionDown;
		}
		else if(angle < -180.f+(regular_spectrum+irregular_spectrum)/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftDown;
				if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionDown;
				else												secondDirection = directionLeft;
			}
			else
			{
				angleDirection = directionLeft;
				if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
				else												secondDirection = directionDown;
			}
		}
		else if(angle < -180.f+regular_spectrum/2.f+irregular_spectrum)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftDown;
				if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionLeft;
				else												secondDirection = directionDown;
			}
			else
			{
				angleDirection = directionDown;
				if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
				else												secondDirection = directionLeft;
			}
		}
		else if(angle < -180.f+regular_spectrum+irregular_spectrum)
		{
			angleDirection = directionDown;
			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
			else												secondDirection = directionLeft;
		}
		else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum)
		{
			angleDirection = directionDown;
			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
			else												secondDirection = directionRight;
		}
		else if(angle < -180.f+(regular_spectrum+irregular_spectrum)*3.f/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightDown;
				if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionRight;
				else												secondDirection = directionDown;
			}
			else
			{
				angleDirection = directionDown;
				if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
				else												secondDirection = directionRight;
			}
		}
		else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightDown;
				if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionDown;
				else												secondDirection = directionRight;
			}
			else
			{
				angleDirection = directionRight;
				if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
				else												secondDirection = directionDown;
			}
		}
		else if(angle < 0.f)
		{
			angleDirection = directionRight;
			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
			else												secondDirection = directionDown;
		}
		else if(angle < regular_spectrum/2.f)
		{
			angleDirection = directionRight;
			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
			else												secondDirection = directionUp;
		}
		else if(angle < regular_spectrum/2.f+irregular_spectrum/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightUp;
				if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionUp;
				else												secondDirection = directionRight;
			}
			else
			{
				angleDirection = directionRight;
				if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
				else												secondDirection = directionUp;
			}
		}
		else if(angle < regular_spectrum/2.f+irregular_spectrum)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightUp;
				if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionRight;
				else												secondDirection = directionUp;
			}
			else
			{
				angleDirection = directionUp;
				if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
				else												secondDirection = directionRight;
			}
		}
		else if(angle < regular_spectrum+irregular_spectrum)
		{
			angleDirection = directionUp;
			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
			else												secondDirection = directionRight;
		}
		else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum)
		{
			angleDirection = directionUp;
			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
			else												secondDirection = directionLeft;
		}
		else if(angle < (regular_spectrum+irregular_spectrum)*3.f/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftUp;
				if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionLeft;
				else												secondDirection = directionUp;
			}
			else
			{
				angleDirection = directionUp;
				if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
				else												secondDirection = directionLeft;
			}
		}
		else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftUp;
				if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionUp;
				else												secondDirection = directionLeft;
			}
			else
			{
				angleDirection = directionLeft;
				if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
				else												secondDirection = directionUp;
			}
		}
		else
		{
			angleDirection = directionLeft;
			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
			else												secondDirection = directionUp;
		}
		
//        game_pad_distance->setString(ccsf("direction %d | sec_dir : %d | rand : %d", angleDirection, secondDirection, rand()));
        
		if(myJack->isDrawingOn && !isEnableIrregularDirection){
			secondDirection = angleDirection;
		}
		
		if(angleDirection != beforeDirection || isButtonAction)
		{
			isButtonAction = false;
			myJack->changeDirection(angleDirection, secondDirection);
			schedule(schedule_selector(ControlJoystickButton::directionKeeping));
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
			beforeDirection = angleDirection;
			beforeSecondDirection = secondDirection;
            game_pad_direction = beforeDirection;
		}
		else if(beforeSecondDirection != secondDirection)
		{
			myJack->changeDirection(angleDirection, secondDirection);
			schedule(schedule_selector(ControlJoystickButton::directionKeeping));
			beforeDirection = angleDirection;
			beforeSecondDirection = secondDirection;
            game_pad_direction = beforeDirection;
		}
		
		myJack->setTouchPointByJoystick(distancePoint, angleDirection, t_b);
	}
	else
	{
//        game_pad_distance->setString(ccsf("stop | distance : %.1f <= minimum : %.1f | rand : %d", distanceValue, minimumDistanceJ, rand()));
        
		if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
		{
			myJack->changeDirection(directionStop, directionStop);
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
			beforeDirection = directionStop;
            game_pad_direction = beforeDirection;
			unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
		}
		myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
	}
}

void ControlJoystickButton::directionKeeping()
{
//	chrono::time_point<chrono::system_clock> start, end;
//	start = chrono::system_clock::now();
//	chrono::duration<double> elapsed_seconds;
	
	if(myJack->isStun)// || myJack->willBackTracking || myJack->getJackState() == jackStateBackTracking)
	{
		myJack->changeDirection(directionStop, directionStop);
		beforeDirection = directionStop;
		beforeSecondDirection = directionStop;
		unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
		myJack->setTouchPointByJoystick(CCPointZero, directionStop, true);
//		joystick_touch = NULL;
		return;
	}
	myJack->changeDirection(beforeDirection, beforeSecondDirection);
	
//	end = chrono::system_clock::now();
//	CCLOG("directionKeeping time : %.8f", chrono::duration<double>(end - start).count());
}

void ControlJoystickButton::stopMySchedule()
{
	myJack->changeDirection(directionStop, directionStop);
	beforeDirection = directionStop;
	beforeSecondDirection = directionStop;
	unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
	myJack->setTouchPointByJoystick(CCPointZero, directionStop, true);
//	joystick_touch = NULL;
}

void ControlJoystickButton::resetTouch()
{
	button_touch = NULL;
	joystick_touch = NULL;
    game_pad_direction = directionStop;
    game_pad_draw_keycode = -1;
	offButton();
	myJack->willBackTracking = false;
	myJack->setTouchPointByJoystick(CCPointZero, directionStop, true);
	myJack->isDrawingOn = false;
	if(myJack->getJackState() == jackStateDrawing && !myJack->isStun)
	{
		(target_main->*delegate_readyBack)();
	}
}

#define JOYSTICK_SCREEN_IN_DISTANCE	60
#define BUTTON_SCREEN_IN_DISTANCE	60

void ControlJoystickButton::invisibleControl()
{
	offButton();
	control_ball->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	control_circle->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	
	if(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick)
	{
		if(controlJoystickDirection == kControlJoystickDirection_right)
		{
			control_circle->setPosition(ccp(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
			control_ball->setPosition(ccp(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
		}
		else
		{
			control_circle->setPosition(ccp(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
			control_ball->setPosition(ccp(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
		}
	}
}

void ControlJoystickButton::offDrawButtonTutorial()
{
	keep_is_draw_button_tutorial_on = false;
}

ControlJoystickButton::~ControlJoystickButton()
{
    if(game_pad_key != -1)
    {
//        game_pad_state->setString(ccsf("unregister : %d | %d", game_pad_key, rand()));
        myHSP->unregisterGamePadCallback(game_pad_key);
        game_pad_key = -1;
        game_pad_direction = directionStop;
        game_pad_draw_keycode = -1;
    }
	if(button_ani)
	{
		
	}
}

CCPoint ControlJoystickButton::directionToTouchPoint(IntDirection t_direction)
{
    CCPoint return_value = control_circle->getPosition();
    
    float t_distance = (20*joystick_size_value+2) > (minimumDistanceJ+2.f) ? (20*joystick_size_value+2) : (minimumDistanceJ+2.f);
    
    if(t_direction == directionLeft)
    {
        return_value = return_value + ccp(-t_distance, 0);
    }
    else if(t_direction == directionLeftUp)
    {
        return_value = return_value + ccp(t_distance*cos(deg2Rad(135.f)), t_distance*sin(deg2Rad(135.f)));
    }
    else if(t_direction == directionUp)
    {
        return_value = return_value + ccp(0, t_distance);
    }
    else if(t_direction == directionRightUp)
    {
        return_value = return_value + ccp(t_distance*cos(deg2Rad(45.f)), t_distance*sin(deg2Rad(45.f)));
    }
    else if(t_direction == directionRight)
    {
        return_value = return_value + ccp(t_distance, 0);
    }
    else if(t_direction == directionRightDown)
    {
        return_value = return_value + ccp(t_distance*cos(deg2Rad(-45.f)), t_distance*sin(deg2Rad(-45.f)));
    }
    else if(t_direction == directionDown)
    {
        return_value = return_value + ccp(0, -t_distance);
    }
    else if(t_direction == directionLeftDown)
    {
        return_value = return_value + ccp(t_distance*cos(deg2Rad(-135.f)), t_distance*sin(deg2Rad(-135.f)));
    }
    
    return return_value;
}

void ControlJoystickButton::gamePadAction(Json::Value pad_data)
{
//    game_pad_state->setString(ccsf("in gamePadAction | %d", rand()));
    if(isStun)
    {
        return;
    }
    
    string pad_type = pad_data["type"].asString();
    
//    game_pad_state->setString(ccsf("pad type : %s | %d", pad_type.c_str(), rand()));
    if(pad_type == "motion")
    {
        IntDirection t_direction;
        
        IntVector pad_point = IntVector(pad_data["x"].asInt(), pad_data["y"].asInt()*(-1));
        t_direction = pad_point.getDirection();
        
        CCPoint dir_position = directionToTouchPoint(t_direction);
        
//        game_pad_motion->setString(ccsf("motion x : %d | y : %d | point x : %.1f | y : %.1f\nin_dir : %d | ori_dir : %d | rand : %d", pad_point.dx, pad_point.dy, dir_position.x, dir_position.y, t_direction, game_pad_direction, rand()));
        
        if(t_direction == game_pad_direction)
            return;
        
        if(t_direction != directionStop)
        {
            unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
            
//            game_pad_distance->setString(ccsf("willBack : %d | isDisButt : %d | isBacking : %d | rand : %d", myJack->willBackTracking, isDisableDrawButton, isBacking, rand()));
            
            if(!myJack->willBackTracking && !isStun)
            {
//                game_pad_log->setString(ccsf("%d touchAction", __LINE__));
                touchAction(directionToTouchPoint(t_direction), false);
            }
            
            if(isDisableDrawButton && isBacking)
            {
                (target_main->*pauseBackTracking)();
                
                myJack->setJackState(jackStateDrawing);
                
                IntPoint jackPoint = myGD->getJackPoint();
                if(myGD->mapState[jackPoint.x][jackPoint.y] == mapEmpty)
                {
                    if(!isDisableLineOver)
                        myGD->communication("PM_checkBeforeNewline", jackPoint);
                    myGD->mapState[jackPoint.x][jackPoint.y] = mapNewline;
                }
                
//                game_pad_log->setString(ccsf("%d touchAction", __LINE__));
                touchAction(directionToTouchPoint(t_direction), false);
            }
        }
        else
        {
//            game_pad_log->setString(ccsf("%d touchAction", __LINE__));
			touchAction(directionToTouchPoint(t_direction), true);
			
			if(isDisableDrawButton && myJack->getJackState() == jackStateDrawing && !myJack->isStun)
			{
				(target_main->*delegate_readyBack)();
			}
			
			game_pad_direction = directionStop;
        }
    }
    else if(pad_type == "keydown")
    {
        if(game_pad_draw_keycode != -1)
            return;
        
        myGD->communication("Main_hideDrawButtonTutorial");
        keep_is_draw_button_tutorial_on = mySGD->is_draw_button_tutorial;
        mySGD->is_draw_button_tutorial = false;
        game_pad_draw_keycode = pad_data["keycode"].asInt();
        myJack->isDrawingOn = true;
        onButton(ccp(-500,-500));
        
        if(isBacking)
        {
            (target_main->*pauseBackTracking)();
            
            myJack->setJackState(jackStateDrawing);
            
            IntPoint jackPoint = myGD->getJackPoint();
            if(myGD->mapState[jackPoint.x][jackPoint.y] == mapEmpty)
            {
                if(!isDisableLineOver)
                    myGD->communication("PM_checkBeforeNewline", jackPoint);
                myGD->mapState[jackPoint.x][jackPoint.y] = mapNewline;
            }
            
            if(game_pad_direction != directionStop)
            {
                isButtonAction = true;
                
//                game_pad_log->setString(ccsf("%d touchAction", __LINE__));
                touchAction(directionToTouchPoint(game_pad_direction), false);
                
                return;
            }
        }
        
        if(game_pad_direction != directionStop && !myJack->isMoving)
        {
            isButtonAction = true;
            
//            game_pad_log->setString(ccsf("%d touchAction", __LINE__));
            touchAction(directionToTouchPoint(game_pad_direction), false);
        }
    }
    else if(pad_type == "keyup")
    {
        if(game_pad_draw_keycode == -1)
            return;
        
        mySGD->is_draw_button_tutorial = keep_is_draw_button_tutorial_on;
        
        myJack->isDrawingOn = false;
        offButton();
        //			draw_button->setColor(ccWHITE);
        if(myJack->getJackState() == jackStateDrawing && !myJack->isStun)
        {
            (target_main->*delegate_readyBack)();
        }
        game_pad_draw_keycode = -1;
    }
}

void ControlJoystickButton::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if(isStun)
	{
		return;
	}
	
	CCSetIterator iter;
	CCTouch* touch;
	
	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
	
		bool is_button_x;
		
		if(controlJoystickDirection == kControlJoystickDirection_left)
		{
			if(location.x > 480-100)
				is_button_x = true;
			else
				is_button_x = false;
		}
		else
		{
			if(location.x < 100)
				is_button_x = true;
			else
				is_button_x = false;
		}
//#if CC_TARGET_PLATFORM == CC_TARGET_OS_IPHONE
////		button_touch = touch;
//		myJack->isDrawingOn = true;
//		onButton();
//		myJack->setJackState(jackStateDrawing); //## 컴터로 임시.
//#endif
		
		if(!isDisableDrawButton && is_button_x)
		{
			// button or ui

			
			if(!button_touch && location.y < 100)
			{
				// button
				myGD->communication("Main_hideDrawButtonTutorial");
				keep_is_draw_button_tutorial_on = mySGD->is_draw_button_tutorial;
				mySGD->is_draw_button_tutorial = false;
				button_touch = touch;
				myJack->isDrawingOn = true;
				onButton(location);
				
//				draw_button->setColor(ccGRAY);
				
				if(isBacking)
				{
					(target_main->*pauseBackTracking)();
					
					myJack->setJackState(jackStateDrawing);
					
					IntPoint jackPoint = myGD->getJackPoint();
					if(myGD->mapState[jackPoint.x][jackPoint.y] == mapEmpty)
					{
						if(!isDisableLineOver)
							myGD->communication("PM_checkBeforeNewline", jackPoint);
						myGD->mapState[jackPoint.x][jackPoint.y] = mapNewline;
					}
					
					if(joystick_touch)
					{
						isButtonAction = true;
						CCPoint joystick_location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(joystick_touch->getLocationInView()));
						touchAction(joystick_location, false);
						continue;
					}
				}
				
				if(joystick_touch && !myJack->isMoving)
				{
					isButtonAction = true;
					CCPoint joystick_location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(joystick_touch->getLocationInView()));
					touchAction(joystick_location, false);
				}
			}
			else
			{
				// ui
			}
		}
		else
		{
			unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
			
			if(!joystick_touch && !myJack->willBackTracking && !isStun)
			{
				CCPoint after_circle_position = location;
				
				if(controlJoystickDirection == kControlJoystickDirection_right)
				{
					if(after_circle_position.x < 90)
						after_circle_position.x = 90;
					else if(after_circle_position.x > 480-JoystickCenterLimit)
						after_circle_position.x = 480-JoystickCenterLimit;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(isJoystickCenterNotFixed)
					{
						
					}
					else if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x < 480-JOYSTICK_SCREEN_IN_DISTANCE-120 || after_circle_position.y > JOYSTICK_SCREEN_IN_DISTANCE+120)
							return;
						else
							control_circle->setPosition(after_circle_position);
					}
				}
				else
				{
					if(after_circle_position.x < JoystickCenterLimit)
						after_circle_position.x = JoystickCenterLimit;
					else if(after_circle_position.x > 390)
						after_circle_position.x = 390;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(isJoystickCenterNotFixed)
					{
						
					}
					else if(isControlJoystickNotFixed && !isJoystickCenterNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x > JOYSTICK_SCREEN_IN_DISTANCE+120 || after_circle_position.y > JOYSTICK_SCREEN_IN_DISTANCE+120)
							return;
						else
							control_circle->setPosition(after_circle_position);
					}
				}
				
				control_circle->setVisible(true);
				
				control_ball->setPosition(location);
				control_ball->setVisible(true);
				
				joystick_touch = touch;
				
				touchAction(location, false);
			}
			
			if(isDisableDrawButton && !joystick_touch && isBacking)
			{
				(target_main->*pauseBackTracking)();
				
				myJack->setJackState(jackStateDrawing);
				
				IntPoint jackPoint = myGD->getJackPoint();
				if(myGD->mapState[jackPoint.x][jackPoint.y] == mapEmpty)
				{
					if(!isDisableLineOver)
						myGD->communication("PM_checkBeforeNewline", jackPoint);
					myGD->mapState[jackPoint.x][jackPoint.y] = mapNewline;
				}
				
				joystick_touch = touch;
				CCPoint after_circle_position = location;
				
				if(controlJoystickDirection == kControlJoystickDirection_right)
				{
					if(after_circle_position.x < 90)
						after_circle_position.x = 90;
					else if(after_circle_position.x > 480-JoystickCenterLimit)
						after_circle_position.x = 480-JoystickCenterLimit;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(isJoystickCenterNotFixed)
					{
						
					}
					else if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x < 480-JOYSTICK_SCREEN_IN_DISTANCE-120 || after_circle_position.y > JOYSTICK_SCREEN_IN_DISTANCE+120)
							return;
						else
							control_circle->setPosition(after_circle_position);
					}
				}
				else
				{
					if(after_circle_position.x < JoystickCenterLimit)
						after_circle_position.x = JoystickCenterLimit;
					else if(after_circle_position.x > 390)
						after_circle_position.x = 390;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(isJoystickCenterNotFixed)
					{
						
					}
					else if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x > JOYSTICK_SCREEN_IN_DISTANCE+120 || after_circle_position.y > JOYSTICK_SCREEN_IN_DISTANCE+120)
							return;
						else
							control_circle->setPosition(after_circle_position);
					}
				}
				
				
				control_circle->setVisible(true);
				
				control_ball->setPosition(location);
				control_ball->setVisible(true);
				
				touchAction(location, false);
				
				if(joystick_touch)
				{
					isButtonAction = true;
					CCPoint joystick_location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(joystick_touch->getLocationInView()));
					touchAction(joystick_location, false);
					continue;
				}
			}
		}
	}
}


void ControlJoystickButton::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if(isStun)
		return;
	
	CCSetIterator iter;
	CCTouch* touch;
	
	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		
		if(touch == button_touch)
		{
			
		}
		else if(touch == joystick_touch)
		{
			if(!myJack->willBackTracking && !isStun)
			{
				CCPoint distancePoint = ccp(location.x - control_circle->getPositionX(), location.y - control_circle->getPositionY());
				float distanceValue = sqrtf(powf(distancePoint.x, 2.0) + powf(distancePoint.y, 2.0));
				float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
				
				CCPoint after_circle_position = CCPointZero;
				if(distanceValue > JOYSTICK_FOLLOW_DISTANCE)
				{
					float sub_value = distanceValue - JOYSTICK_FOLLOW_DISTANCE;
					after_circle_position.x = 1;
					after_circle_position.y = tanf(angle/180.f*M_PI);
					
					float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
					if(angle >= 90 || angle <= -90)
						after_circle_position = ccpMult(after_circle_position, -sub_value/div_value);
					else
						after_circle_position = ccpMult(after_circle_position, sub_value/div_value);
				}
				
				after_circle_position = ccpAdd(after_circle_position, control_circle->getPosition());
				
				if(controlJoystickDirection == kControlJoystickDirection_right)
				{
					if(after_circle_position.x < 90)
						after_circle_position.x = 90;
					else if(after_circle_position.x > 480-JoystickCenterLimit)
						after_circle_position.x = 480-JoystickCenterLimit;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(!isJoystickCenterNotFixed) // !myDSH->getBoolForKey(kDSH_Key_isControlJoystickFixed) &&
					{
						if(!isControlJoystickNotFixed)
						{
							CCPoint circle_position = after_circle_position;
							if(circle_position.x < 480-JOYSTICK_SCREEN_IN_DISTANCE-120)
								circle_position.x = 480-JOYSTICK_SCREEN_IN_DISTANCE-120;
							if(circle_position.y > JOYSTICK_SCREEN_IN_DISTANCE+120)
								circle_position.y = JOYSTICK_SCREEN_IN_DISTANCE+120;
							
							control_circle->setPosition(circle_position);
							
							float t_distance = distanceValue;
							if(distanceValue > 25*joystick_size_value)
								t_distance = 25*joystick_size_value;
							
							CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
							
							control_ball->setPosition(inner_position);
						}
						else
						{
							control_circle->setPosition(after_circle_position);
							
							float t_distance = distanceValue;
							if(distanceValue > 25*joystick_size_value)
								t_distance = 25*joystick_size_value;
							
							CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
							control_ball->setPosition(inner_position);
						}
					}
					else
					{
						float t_distance = distanceValue;
						if(distanceValue > 25*joystick_size_value)
							t_distance = 25*joystick_size_value;
						
						CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
						
						control_ball->setPosition(inner_position);
					}
				}
				else
				{
					if(after_circle_position.x < JoystickCenterLimit)
						after_circle_position.x = JoystickCenterLimit;
					else if(after_circle_position.x > 390)
						after_circle_position.x = 390;
					if(after_circle_position.y < JoystickCenterLimit)
						after_circle_position.y = JoystickCenterLimit;
					else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
						after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
					
					if(!isJoystickCenterNotFixed) // !myDSH->getBoolForKey(kDSH_Key_isControlJoystickFixed) &&
					{
						if(!isControlJoystickNotFixed)
						{
							CCPoint circle_position = after_circle_position;
							if(circle_position.x > JOYSTICK_SCREEN_IN_DISTANCE+120)
								circle_position.x = JOYSTICK_SCREEN_IN_DISTANCE+120;
							if(circle_position.y > JOYSTICK_SCREEN_IN_DISTANCE+120)
								circle_position.y = JOYSTICK_SCREEN_IN_DISTANCE+120;
							
							control_circle->setPosition(circle_position);
							
							float t_distance = distanceValue;
							if(distanceValue > 25*joystick_size_value)
								t_distance = 25*joystick_size_value;
							
							CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
							
							control_ball->setPosition(inner_position);
						}
						else
						{
							control_circle->setPosition(after_circle_position);
							float t_distance = distanceValue;
							if(distanceValue > 25*joystick_size_value)
								t_distance = 25*joystick_size_value;
							
							CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
							control_ball->setPosition(inner_position);
						}
					}
					else
					{
						float t_distance = distanceValue;
						if(distanceValue > 25*joystick_size_value)
							t_distance = 25*joystick_size_value;
						
						CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
						
						control_ball->setPosition(inner_position);
					}
				}
				
				
				touchAction(location, false);
			}
		}
	}
}

void ControlJoystickButton::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
//	if(isStun)
//		return;
	
	CCSetIterator iter;
	CCTouch* touch;
	
	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		
		if(touch == button_touch)
		{
			mySGD->is_draw_button_tutorial = keep_is_draw_button_tutorial_on;
			
			myJack->isDrawingOn = false;
			offButton();
//			draw_button->setColor(ccWHITE);
			if(myJack->getJackState() == jackStateDrawing && !myJack->isStun)
			{
				(target_main->*delegate_readyBack)();
			}
			button_touch = NULL;
		}
		else if(touch == joystick_touch)
		{
			CCPoint distancePoint = ccp(location.x - control_circle->getPositionX(), location.y - control_circle->getPositionY());
			float distanceValue = sqrt(pow(distancePoint.x, 2.0) + pow(distancePoint.y, 2.0));
			float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
			
			CCPoint after_circle_position = CCPointZero;
			if(distanceValue > JOYSTICK_FOLLOW_DISTANCE)
			{
				float sub_value = distanceValue - JOYSTICK_FOLLOW_DISTANCE;
				after_circle_position.x = 1;
				after_circle_position.y = tanf(angle/180.f*M_PI);
				
				float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
				if(angle >= 90 || angle <= -90)
					after_circle_position = ccpMult(after_circle_position, -sub_value/div_value);
				else
					after_circle_position = ccpMult(after_circle_position, sub_value/div_value);
				
////				float sub_value = distanceValue - 37;
//				after_circle_position.x = 1;
//				after_circle_position.y = tanf(angle/180.f*M_PI);
//				
//				float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
//				if(angle >= 90 || angle <= -90)
//					after_circle_position = ccpMult(after_circle_position, -1.f/div_value);
//				else
//					after_circle_position = ccpMult(after_circle_position, 1.f/div_value);
			}
			after_circle_position = ccpAdd(after_circle_position, control_circle->getPosition());
			
			if(controlJoystickDirection == kControlJoystickDirection_right)
			{
				if(after_circle_position.x < 90)
					after_circle_position.x = 90;
				else if(after_circle_position.x > 480-JoystickCenterLimit)
					after_circle_position.x = 480-JoystickCenterLimit;
				if(after_circle_position.y < JoystickCenterLimit)
					after_circle_position.y = JoystickCenterLimit;
				else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
					after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
				
				if(!isJoystickCenterNotFixed) // !myDSH->getBoolForKey(kDSH_Key_isControlJoystickFixed) &&
				{
					if(!isControlJoystickNotFixed)
					{
						CCPoint circle_position = after_circle_position;
						if(circle_position.x < 480-JOYSTICK_SCREEN_IN_DISTANCE-120)
							circle_position.x = 480-JOYSTICK_SCREEN_IN_DISTANCE-120;
						if(circle_position.y > JOYSTICK_SCREEN_IN_DISTANCE+120)
							circle_position.y = JOYSTICK_SCREEN_IN_DISTANCE+120;
						
						control_circle->setPosition(circle_position);
						
						float t_distance = distanceValue;
						if(distanceValue > 25*joystick_size_value)
							t_distance = 25*joystick_size_value;
						
						CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
						
						control_ball->setPosition(inner_position);
					}
					else
					{
						control_circle->setPosition(after_circle_position);
						control_ball->setPosition(location);
					}
				}
				else
				{
					float t_distance = distanceValue;
					if(distanceValue > 25*joystick_size_value)
						t_distance = 25*joystick_size_value;
					
					CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
					
					control_ball->setPosition(inner_position);
				}
			}
			else
			{
				if(after_circle_position.x < JoystickCenterLimit)
					after_circle_position.x = JoystickCenterLimit;
				else if(after_circle_position.x > 390)
					after_circle_position.x = 390;
				if(after_circle_position.y < JoystickCenterLimit)
					after_circle_position.y = JoystickCenterLimit;
				else if(after_circle_position.y > myDSH->ui_top-JoystickCenterLimit)
					after_circle_position.y = myDSH->ui_top-JoystickCenterLimit;
				
				if(!isJoystickCenterNotFixed) // !myDSH->getBoolForKey(kDSH_Key_isControlJoystickFixed) &&
				{
					if(!isControlJoystickNotFixed)
					{
						CCPoint circle_position = after_circle_position;
						if(circle_position.x > JOYSTICK_SCREEN_IN_DISTANCE+120)
							circle_position.x = JOYSTICK_SCREEN_IN_DISTANCE+120;
						if(circle_position.y > JOYSTICK_SCREEN_IN_DISTANCE+120)
							circle_position.y = JOYSTICK_SCREEN_IN_DISTANCE+120;
						
						control_circle->setPosition(circle_position);
						
						float t_distance = distanceValue;
						if(distanceValue > 25*joystick_size_value)
							t_distance = 25*joystick_size_value;
						
						CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
						
						control_ball->setPosition(inner_position);
					}
					else
					{
						control_circle->setPosition(after_circle_position);
						control_ball->setPosition(location);
					}
				}
				else
				{
					float t_distance = distanceValue;
					if(distanceValue > 25*joystick_size_value)
						t_distance = 25*joystick_size_value;
					
					CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
					
					control_ball->setPosition(inner_position);
				}
			}
			
			touchAction(location, true);
			if(isControlJoystickNotFixed && isAlwaysInvisibleJoystick)
			{
				control_circle->setVisible(false);
				control_ball->setVisible(false);
			}
			else
			{
				if(controlJoystickDirection == kControlJoystickDirection_right)
				{
					control_circle->setPosition(ccp(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
					control_ball->setPosition(ccp(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
				}
				else
				{
					control_circle->setPosition(ccp(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
					control_ball->setPosition(ccp(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
				}
			}
			
			if(isDisableDrawButton && myJack->getJackState() == jackStateDrawing && !myJack->isStun)
			{
				(target_main->*delegate_readyBack)();
			}
			
			joystick_touch = NULL;
		}
	}
}

void ControlJoystickButton::ccTouchesCancelled(CCSet *pTouch, CCEvent *pEvent)
{
	ccTouchesEnded(pTouch, pEvent);
}

void ControlJoystickButton::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, -110);
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
}

ControlJoystickButton* ControlJoystickButton::create( CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack )
{
	ControlJoystickButton* t_cjf = new ControlJoystickButton();
	t_cjf->myInit(t_main, d_readyBack, t_jack);
	t_cjf->autorelease();
	return t_cjf;
}

void ControlJoystickButton::setTouchEnabled( bool t_b )
{
	ControlCommon::setTouchEnabled(t_b);
	if(!t_b)
	{
        if(game_pad_key != -1)
        {
//            game_pad_state->setString(ccsf("unregister : %d | %d", game_pad_key, rand()));
            myHSP->unregisterGamePadCallback(game_pad_key);
            game_pad_key = -1;
        }
        { // game pad
            if(game_pad_draw_keycode != -1)
            {
                offButton();
                game_pad_draw_keycode = -1;
            }
            
            if(game_pad_direction != directionStop)
            {
                if(controlJoystickDirection == kControlJoystickDirection_left)	control_circle->setPosition(ccp(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
                else															control_circle->setPosition(ccp(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
                
                control_ball->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
                myJack->setTouchPointByJoystick(CCPointZero, directionStop, t_b);
                game_pad_direction = directionStop;
            }
        }
        
        
		unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
		if(button_touch)
		{
			//				draw_button->setColor(ccWHITE);
			offButton();
			button_touch = NULL;
		}

		if(joystick_touch)
		{
			if(controlJoystickDirection == kControlJoystickDirection_left)	control_circle->setPosition(ccp(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));
			else															control_circle->setPosition(ccp(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value, JOYSTICK_SCREEN_IN_DISTANCE));

			control_ball->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
			myJack->setTouchPointByJoystick(CCPointZero, directionStop, t_b);
			joystick_touch = NULL;
		}
	}
    else
    {
        game_pad_key = myHSP->registerGamePadCallback(json_selector(this, ControlJoystickButton::gamePadAction));
//        game_pad_state->setString(ccsf("register : %d | %d", game_pad_key, rand()));
    }
}

void ControlJoystickButton::myInit( CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack )
{
	CCLayer::init();
	
//    game_pad_state = KSLabelTTF::create("", mySGD->getFont().c_str(), 10);
//    game_pad_state->setAnchorPoint(ccp(0,1));
//    game_pad_state->setPosition(ccp(5, myDSH->ui_top-50));
//    addChild(game_pad_state);
//    
//    game_pad_motion = KSLabelTTF::create("", mySGD->getFont().c_str(), 10);
//    game_pad_motion->setAnchorPoint(ccp(0,1));
//    game_pad_motion->setPosition(ccp(0,0));
//    game_pad_state->addChild(game_pad_motion);
//    
//    game_pad_distance = KSLabelTTF::create("", mySGD->getFont().c_str(), 10);
//    game_pad_distance->setAnchorPoint(ccp(0,1));
//    game_pad_distance->setPosition(ccp(0,0));
//    game_pad_motion->addChild(game_pad_distance);
//    
//    game_pad_log = KSLabelTTF::create("", mySGD->getFont().c_str(), 10);
//    game_pad_log->setAnchorPoint(ccp(0,1));
//    game_pad_log->setPosition(ccp(0,0));
//    game_pad_distance->addChild(game_pad_log);
    
    game_pad_direction = directionStop;
    game_pad_draw_keycode = -1;
    game_pad_key = -1;
    
	is_show_draw_button_tutorial = false;
	
//	CCLog("device width : %.2lf", myHSP->getScreenRealWidth());
	
	joystick_size_value = (myDSH->getIntegerForKey(kDSH_Key_joystickSize)+9)/10.f; // -5
	
	TouchOutWidth = 10.0*joystick_size_value;
	JOYSTICK_FOLLOW_DISTANCE = 65.0*joystick_size_value;
	
	CCLog("JOYSTICK_FOLLOW_DISTANCE : %.2lf", JOYSTICK_FOLLOW_DISTANCE);
	
	minimumDistanceJ = 8.0*joystick_size_value;
	JoystickCenterLimit = 30.0*joystick_size_value;
	
	regular_spectrum = 70.f;
	irregular_spectrum = 20.f;

	isButtonAction = false;
	button_touch = NULL;
	joystick_touch = NULL;
	isBacking = false;

	isEnableIrregularDirection = myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection);
	isControlJoystickNotFixed = true;// myDSH->getBoolForKey(kDSH_Key_isControlJoystickNotFixed);
	isAlwaysInvisibleJoystick = myDSH->getBoolForKey(kDSH_Key_isAlwaysInvisibleJoystick);
	controlJoystickDirection = myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection);
	isDisableDrawButton = myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton);
	isDisableLineOver = myDSH->getBoolForKey(kDSH_Key_isDisableLineOver);
	isJoystickCenterNotFixed = myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed);
	
	ControlCommon::myInit(t_main, d_readyBack, t_jack);
	beforeDirection = directionStop;
	control_circle = CCSprite::create("control_joystick_big_circle.png");
	control_circle->setScale(joystick_size_value);
	control_circle->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	addChild(control_circle);

	control_ball = CCSprite::create("control_joystick_big_ball.png");
	control_ball->setScale(joystick_size_value);
	control_ball->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	addChild(control_ball);
	
	if(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick)
	{
		if(controlJoystickDirection == kControlJoystickDirection_right)
		{
			control_circle->setPosition(ccp(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value+CONTROL_OUT_DISTANCE, JOYSTICK_SCREEN_IN_DISTANCE));
			control_ball->setPosition(ccp(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value+CONTROL_OUT_DISTANCE, JOYSTICK_SCREEN_IN_DISTANCE));
			
			addChild(KSGradualValue<float>::create(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value + CONTROL_OUT_DISTANCE, 480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value, CONTROL_IN_TIME,
												   [=](float t){control_circle->setPositionX(t);control_ball->setPositionX(t);},
												   [=](float t){control_circle->setPositionX(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value);control_ball->setPositionX(480-JOYSTICK_SCREEN_IN_DISTANCE-myGD->boarder_value);}));
		}
		else
		{
			control_circle->setPosition(ccp(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value-CONTROL_OUT_DISTANCE, JOYSTICK_SCREEN_IN_DISTANCE));
			control_ball->setPosition(ccp(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value-CONTROL_OUT_DISTANCE, JOYSTICK_SCREEN_IN_DISTANCE));
			
			addChild(KSGradualValue<float>::create(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value - CONTROL_OUT_DISTANCE, JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value, CONTROL_IN_TIME,
												   [=](float t){control_circle->setPositionX(t);control_ball->setPositionX(t);},
												   [=](float t){control_circle->setPositionX(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value);control_ball->setPositionX(JOYSTICK_SCREEN_IN_DISTANCE+myGD->boarder_value);}));
		}
	}

	if(!isDisableDrawButton)
	{
//		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//		CCBReader* reader = new CCBReader(nodeLoader);
//		draw_button = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile(CCString::createWithFormat("gameui_button_%s.ccbi", myLoc->getLocalCode().c_str())->getCString(),this));
		auto t_ccb = KS::loadCCBI<CCSprite*>(this, CCString::createWithFormat("gameui_button_%s.ccbi", myLoc->getSupportLocalCode())->getCString());
		draw_button = t_ccb.first;
		button_ani = t_ccb.second;
		//		draw_button = CCSprite::create("ui_draw.png");
		if(controlJoystickDirection == kControlJoystickDirection_left)
		{
			draw_button->setPosition(ccp(480-BUTTON_SCREEN_IN_DISTANCE+CONTROL_OUT_DISTANCE,BUTTON_SCREEN_IN_DISTANCE));
			
			addChild(KSGradualValue<float>::create(480-BUTTON_SCREEN_IN_DISTANCE + CONTROL_OUT_DISTANCE, 480-BUTTON_SCREEN_IN_DISTANCE, CONTROL_IN_TIME,
												   [=](float t){draw_button->setPositionX(t);},
												   [=](float t){draw_button->setPositionX(480-BUTTON_SCREEN_IN_DISTANCE);}));
		}
		else
		{
			draw_button->setPosition(ccp(BUTTON_SCREEN_IN_DISTANCE-CONTROL_OUT_DISTANCE,BUTTON_SCREEN_IN_DISTANCE));
			
			addChild(KSGradualValue<float>::create(BUTTON_SCREEN_IN_DISTANCE - CONTROL_OUT_DISTANCE, BUTTON_SCREEN_IN_DISTANCE, CONTROL_IN_TIME,
												   [=](float t){draw_button->setPositionX(t);},
												   [=](float t){draw_button->setPositionX(BUTTON_SCREEN_IN_DISTANCE);}));
		}
		addChild(draw_button);
		
		click_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_controlClick), mySGD->getFont().c_str(), 18);
		click_label->enableOuterStroke(ccBLACK, 0.5f);
		click_label->setPosition(ccp(draw_button->getContentSize().width/2.f, draw_button->getContentSize().height/2.f));
		click_label->setVisible(false);
		draw_button->addChild(click_label);
	}
	else
	{
		myJack->isDrawingOn = true;
		button_ani = NULL;
	}

	mType = kCT_Type_Joystick_button;
}

void ControlJoystickButton::onButton(CCPoint t_location)
{
	if(button_ani)
	{
//		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		
		button_ani->runAnimationsForSequenceNamed("cast1start");
		click_label->stopAllActions();
		CCFadeTo* t_fade = CCFadeTo::create(0.5f, 50);
		click_label->runAction(t_fade);
		
		draw_button->setPosition(t_location);
	}
}

void ControlJoystickButton::offButton()
{
	if(button_ani)
	{
//		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		
		button_ani->runAnimationsForSequenceNamed("cast1stop");
		click_label->stopAllActions();
		CCFadeTo* t_fade = CCFadeTo::create(0.3f, 255);
		click_label->runAction(t_fade);
		
		if(controlJoystickDirection == kControlJoystickDirection_left)
			draw_button->setPosition(ccp(480-BUTTON_SCREEN_IN_DISTANCE,BUTTON_SCREEN_IN_DISTANCE));
		else
			draw_button->setPosition(ccp(BUTTON_SCREEN_IN_DISTANCE,BUTTON_SCREEN_IN_DISTANCE));
	}
}

void ControlJoystickButton::showDrawButtonTutorial()
{
	is_show_draw_button_tutorial = true;
	
	draw_button_tutorial_img = CCSprite::create("ingame_tutorial_warning_back.png");
	draw_button_tutorial_img->setPosition(ccp(240, myDSH->ui_center_y));
	addChild(draw_button_tutorial_img);
	
	StyledLabelTTF* t_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_buttonTutorialMent), mySGD->getFont().c_str(), 16, 999, StyledAlignment::kCenterAlignment);
	t_label->setAnchorPoint(ccp(0.5f,0.5f));
	t_label->setPosition(ccpFromSize(draw_button_tutorial_img->getContentSize()/2.f));
	draw_button_tutorial_img->addChild(t_label);
	
	CommonAnimation::openPopup(this, draw_button_tutorial_img, NULL);
	
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
	draw_button_tutorial_ccb = KS::loadCCBI<CCSprite*>(this, "ingame_tutorial_button.ccbi").first;// dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_tutorial_button.ccbi",this));
	draw_button_tutorial_ccb->setPosition(draw_button->getPosition());
	addChild(draw_button_tutorial_ccb);
//	reader->release();
}

void ControlJoystickButton::hideDrawButtonTutorial()
{
	if(is_show_draw_button_tutorial)
	{
		CCSprite* t_img = draw_button_tutorial_img;
		draw_button_tutorial_img = NULL;
		CommonAnimation::closePopup(this, t_img, NULL, [=](){}, [=]()
		{
			t_img->removeFromParent();
		});
		
		draw_button_tutorial_ccb->removeFromParent();
		draw_button_tutorial_ccb = NULL;
		
		is_show_draw_button_tutorial = false;
	}
	
	if(mySGD->is_draw_button_tutorial)
		mySGD->draw_button_tutorial_ing = 1;
}

//////////////////////////////////////////////// Joystick ///////////////////////////////////////////////////////////////////////////////
//void ControlJoystick::touchAction(CCPoint t_p, bool t_b)
//{
//	CCPoint distancePoint = ccp(t_p.x - control_circle->getPositionX(), t_p.y - control_circle->getPositionY());
//	float distanceValue = sqrt(pow(distancePoint.x, 2.0) + pow(distancePoint.y, 2.0));
//	float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
//	
//	//	if(!isStun && t_b)
//	//	{
//	//		if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
//	//		{
//	//			beforeDirection = directionStop;
//	//			myJack->changeDirection(directionStop, directionStop);
//	//			(target_main->*delegate_readyBack)();
//	//		}
//	//		else
//	//		{
//	//			beforeDirection = directionStop;
//	//			myJack->changeDirection(directionStop, directionStop);
//	//		}
//	//		myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
//	//		return;
//	//	}
//	
//	IntDirection angleDirection;
//	IntDirection secondDirection;
//	
////	if(!isStun && myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing && t_b)
////	{
////		myJack->changeDirection(directionStop, directionStop);
////		(target_main->*delegate_readyBack)();
////		beforeDirection = directionStop;
////		unschedule(schedule_selector(ControlJoystick::directionKeeping));
////		myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
////		AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
////		return;
////	}
//	if(t_b)
//	{
//		myJack->changeDirection(directionStop, directionStop);
//		beforeDirection = directionStop;
//		unschedule(schedule_selector(ControlJoystick::directionKeeping));
//		myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
//		return;
//	}
//	
//	if(distanceValue > minimumDistanceJ)
//	{
//		IntPoint jackPoint = myGD->getJackPoint();
//		
//		if(angle < -135.f)
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < -90.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else if(angle < -45.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 0.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < 45.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		else if(angle < 90.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 135.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		
//		if(angleDirection != beforeDirection)
//		{
//			myJack->changeDirection(angleDirection, secondDirection);
//			schedule(schedule_selector(ControlJoystick::directionKeeping));
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//			beforeDirection = angleDirection;
//		}
//		myJack->setTouchPointByJoystick(distancePoint, angleDirection, t_b);
//	}
//	else
//	{
//		if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
//		{
//			myJack->changeDirection(directionStop, directionStop);
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//			beforeDirection = directionStop;
//			unschedule(schedule_selector(ControlJoystick::directionKeeping));
//		}
//		myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
//	}
//}
//
//void ControlJoystick::directionKeeping()
//{
//	if(myJack->isStun)
//	{
//		unschedule(schedule_selector(ControlJoystick::directionKeeping));
//		return;
//	}
//	myJack->changeDirection(beforeDirection, directionStop);
//}
//
//bool ControlJoystick::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	if(!myJack->willBackTracking && !isStun &&
//	   location.y < 320 &&
//	   location.y > 0 &&
//	   location.x > 0 &&
//	   location.x < 480)
//	{
//		CCPoint after_circle_position = location;
//		if(after_circle_position.x < 37)
//			after_circle_position.x = 37;
//		else if(after_circle_position.x > 443)
//			after_circle_position.x = 443;
//		if(after_circle_position.y < 37)
//			after_circle_position.y = 37;
//		else if(after_circle_position.y > 283)
//			after_circle_position.y = 283;
//		
//		control_circle->setPosition(after_circle_position);
//		control_circle->setVisible(true);
//		
//		control_ball->setPosition(location);
//		control_ball->setVisible(true);
//		
//		touchAction(location, false);
//	}
//	
//	return true;
//}
//
//void ControlJoystick::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	if(!myJack->willBackTracking && !isStun &&
//	   location.y < 320 &&
//	   location.y > 0 &&
//	   location.x > 0 &&
//	   location.x < 480)
//	{
//		CCPoint distancePoint = ccp(location.x - control_circle->getPositionX(), location.y - control_circle->getPositionY());
//		float distanceValue = sqrt(pow(distancePoint.x, 2.0) + pow(distancePoint.y, 2.0));
//		float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
//		
//		CCPoint after_circle_position = CCPointZero;
//		if(distanceValue > 37)
//		{
//			float sub_value = distanceValue - 37;
//			after_circle_position.x = 1;
//			after_circle_position.y = tanf(angle/180.f*M_PI);
//			
//			float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
//			if(angle >= 90 || angle <= -90)
//				after_circle_position = ccpMult(after_circle_position, -sub_value/div_value);
//			else
//				after_circle_position = ccpMult(after_circle_position, sub_value/div_value);
//		}
//		after_circle_position = ccpAdd(after_circle_position, control_circle->getPosition());
//		
//		if(after_circle_position.x < 37)
//			after_circle_position.x = 37;
//		else if(after_circle_position.x > 443)
//			after_circle_position.x = 443;
//		if(after_circle_position.y < 37)
//			after_circle_position.y = 37;
//		else if(after_circle_position.y > 283)
//			after_circle_position.y = 283;
//		
//		control_circle->setPosition(after_circle_position);
//		
//		control_ball->setPosition(location);
//		touchAction(location, false);
//	}
//}
//
//void ControlJoystick::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	CCPoint distancePoint = ccp(location.x - control_circle->getPositionX(), location.y - control_circle->getPositionY());
//	float distanceValue = sqrt(pow(distancePoint.x, 2.0) + pow(distancePoint.y, 2.0));
//	float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
//	
//	CCPoint after_circle_position = CCPointZero;
//	if(distanceValue > 37)
//	{
////		float sub_value = distanceValue - 37;
//		after_circle_position.x = 1;
//		after_circle_position.y = tanf(angle/180.f*M_PI);
//		
//		float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
//		if(angle >= 90 || angle <= -90)
//			after_circle_position = ccpMult(after_circle_position, -1.f/div_value);
//		else
//			after_circle_position = ccpMult(after_circle_position, 1.f/div_value);
//	}
//	after_circle_position = ccpAdd(after_circle_position, control_circle->getPosition());
//	
//	if(after_circle_position.x < 37)
//		after_circle_position.x = 37;
//	else if(after_circle_position.x > 443)
//		after_circle_position.x = 443;
//	if(after_circle_position.y < 37)
//		after_circle_position.y = 37;
//	else if(after_circle_position.y > 283)
//		after_circle_position.y = 283;
//	
//	control_circle->setPosition(after_circle_position);
//	
//	control_ball->setPosition(location);
//	touchAction(location, true);
//	control_circle->setVisible(false);
//	control_ball->setVisible(false);
//}
//
//void ControlJoystick::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	CCPoint distancePoint = ccp(location.x - control_circle->getPositionX(), location.y - control_circle->getPositionY());
//	float distanceValue = sqrt(pow(distancePoint.x, 2.0) + pow(distancePoint.y, 2.0));
//	float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
//	
//	CCPoint after_circle_position = CCPointZero;
//	if(distanceValue > 37)
//	{
////		float sub_value = distanceValue - 37;
//		after_circle_position.x = 1;
//		after_circle_position.y = tanf(angle/180.f*M_PI);
//		
//		float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
//		if(angle >= 90 || angle <= -90)
//			after_circle_position = ccpMult(after_circle_position, -1.f/div_value);
//		else
//			after_circle_position = ccpMult(after_circle_position, 1.f/div_value);
//	}
//	after_circle_position = ccpAdd(after_circle_position, control_circle->getPosition());
//	
//	if(after_circle_position.x < 37)
//		after_circle_position.x = 37;
//	else if(after_circle_position.x > 443)
//		after_circle_position.x = 443;
//	if(after_circle_position.y < 37)
//		after_circle_position.y = 37;
//	else if(after_circle_position.y > 283)
//		after_circle_position.y = 283;
//	
//	control_circle->setPosition(after_circle_position);
//	
//	control_ball->setPosition(location);
//	touchAction(location, true);
//	control_circle->setVisible(false);
//	control_ball->setVisible(false);
//}
//
//void ControlJoystick::registerWithTouchDispatcher()
//{
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
//}


//////////////////////////////////////////////// Joystick Fix //////////////////////////////////////////////////////////////////////

//#define minimumDistance	30.f
//
//void ControlJoystickFix::touchAction(CCPoint t_p, bool t_b)
//{
//	CCPoint distancePoint = ccp(t_p.x - 250, t_p.y - 70);
//	float distanceValue = sqrt(pow(distancePoint.x, 2.0) + pow(distancePoint.y, 2.0));
//	float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
//	
//	IntDirection angleDirection;
//	IntDirection secondDirection;
//	
//	if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing && t_b)
//	{
//		(target_main->*delegate_readyBack)();
//		beforeDirection = directionStop;
//		myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
//		control_spr->setVisible(true);
//		AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//		return;
//	}
//	if(t_b)
//		control_spr->setVisible(true);
//	else
//		control_spr->setVisible(false);
//	
//	if(distanceValue > minimumDistance)
//	{
//		IntPoint jackPoint = myGD->getJackPoint();
//		
//		if(angle < -135.f)
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < -90.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else if(angle < -45.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 0.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < 45.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		else if(angle < 90.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 135.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		
//		myJack->changeDirection(angleDirection, secondDirection);
//		if(angleDirection != beforeDirection)
//		{
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//			beforeDirection = angleDirection;
//		}
//		myJack->setTouchPointByJoystick(distancePoint, angleDirection, t_b);
//	}
//	else
//	{
//		if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
//		{
//			myJack->changeDirection(directionStop, directionStop);
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//			beforeDirection = directionStop;
//		}
//		myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
//	}
//}
//
//bool ControlJoystickFix::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	if(!myJack->willBackTracking && !isStun &&
//	   location.y < 320 &&
//	   location.y > 0 &&
//	   location.x > 0 &&
//	   location.x < 480)
//	{
//		touchAction(location, false);
//	}
//	
//	return true;
//}
//
//void ControlJoystickFix::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	if(!myJack->willBackTracking && !isStun &&
//	   location.y < 320 &&
//	   location.y > 0 &&
//	   location.x > 0 &&
//	   location.x < 480)
//	{
//		touchAction(location, false);
//	}
//}
//
//void ControlJoystickFix::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	touchAction(location, true);
//}
//
//void ControlJoystickFix::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	touchAction(location, true);
//}
//
//void ControlJoystickFix::registerWithTouchDispatcher()
//{
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
//}

////////////////////////////////////////// Original Gesture //////////////////////////////////////////////////////////////////////

//void ControlOriginalGesture::resetTouchBeganPoint()
//{
//	touch_began_point = ccp(-1.f, -1.f);
//	is_gestured = false;
//	gesture_cnt = 0;
//	beforeGesture = directionStop;
//}
//
//bool ControlOriginalGesture::isSetTouchBeganPoint()
//{
//	if(touch_began_point.x < 0 || touch_began_point.y < 0)
//		return false;
//	else
//		return true;
//}
//
//bool ControlOriginalGesture::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	if(!myJack->willBackTracking &&
//	   location.y < 320 &&
//	   location.y > 0 &&
//	   location.x > 0 &&
//	   location.x < 480)
//	{
//		touch_began_point = location;
//	}
//	
//	return true;
//}
//
//void ControlOriginalGesture::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
//{
//	if(!isSetTouchBeganPoint())
//		return;
//	
//	CCTouch* touch = pTouch;
//	CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
//	
//	// calc direction
//	CCSize distanceSize = CCSizeMake(location.x - touch_began_point.x, location.y - touch_began_point.y);
//	float distanceValue = sqrt(pow(distanceSize.width, 2.0) + pow(distanceSize.height, 2.0));
//	float angle = atan2(distanceSize.height, distanceSize.width)/M_PI*180.0; // -180 ~ 180
//	
//	if(distanceValue > minimumDistance/device_rate*((is_gestured || (myJack->isDrawingOn && myJack->getRecentDirection() == directionStop)) ? 2 : 1.3))
//	{
//		IntDirection angleDirection;
//		IntDirection secondDirection;
//		
//		IntPoint jackPoint = myGD->getJackPoint();
//		
//		if(angle < -135.f)
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < -90.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else if(angle < -45.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 0.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < 45.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		else if(angle < 90.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 135.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		
//		if(angleDirection != beforeGesture)
//		{
//			myJack->changeDirection(angleDirection, secondDirection);
//			touch_began_point = location;
//			
////			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//			beforeGesture = angleDirection;
//			if(is_gestured)
//				gesture_cnt++;
//		}
//		is_gestured = true;
////		resetTouchBeganPoint();
//	}
//}
//
//void ControlOriginalGesture::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//	if(!isSetTouchBeganPoint())
//		return;
//	
//	if(!isStun)
//	{
//		if(!is_gestured)
//		{
//			if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
//			{
//				if(myJack->getRecentDirection() == directionStop)
//				{
////					readyBackTracking();
//					(target_main->*delegate_readyBack)();
//				}
//				else
//				{
//					myJack->changeDirection(directionStop, directionStop);
//				}
//			}
//			else
//			{
//				myJack->changeDirection(directionStop, directionStop);
//			}
//		}
//		else if(gesture_cnt > 0)
//		{
//			myJack->changeDirection(directionStop, directionStop);
//		}
//		
//	}
//	resetTouchBeganPoint();
//}
//
//void ControlOriginalGesture::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
//{
//	if(!isSetTouchBeganPoint())
//		return;
//	
//	if(!isStun)
//	{
//		if(!is_gestured)
//		{
//			if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
//			{
//				if(myJack->getRecentDirection() == directionStop)
//				{
////					readyBackTracking();
//					(target_main->*delegate_readyBack)();
//				}
//				else
//				{
//					myJack->changeDirection(directionStop, directionStop);
//				}
//			}
//			else
//			{
//				myJack->changeDirection(directionStop, directionStop);
//			}
//		}
//		else if(gesture_cnt > 0)
//		{
//			myJack->changeDirection(directionStop, directionStop);
//		}
//	}
//	resetTouchBeganPoint();
//}
//
//void ControlOriginalGesture::registerWithTouchDispatcher()
//{
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
//}

void ControlJoystickButton::startControl()
{
	setTouchEnabled(true);
}

void ControlCommon::startControl()
{
	//		if(((CCNode*)target_main)->getTag() == 1)
	
	setTouchEnabled(true);
}

void ControlCommon::setTouchEnabled( bool t_b )
{
	CCLayer::setTouchEnabled(t_b);
	is_control = t_b;
}

void ControlCommon::myInit( CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack )
{
	myJack = t_jack;
	isStun = false;
	is_control = false;

	//		device_rate = myDSH->getDeviceRate();
	target_main = t_main;
	delegate_readyBack = d_readyBack;
}
