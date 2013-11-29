//
//  ControlType.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 6..
//
//

#include "ControlType.h"

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

#define minimumDistanceJ	5.f

void ControlJoystickButton::touchAction(CCPoint t_p, bool t_b)
{
	CCPoint distancePoint = ccp(t_p.x - control_circle->getPositionX(), t_p.y - control_circle->getPositionY());
	float distanceValue = sqrt(pow(distancePoint.x, 2.0) + pow(distancePoint.y, 2.0));
	float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
	
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
	
	IntDirection angleDirection;
	IntDirection secondDirection;
	
	if(t_b)
	{
		myJack->changeDirection(directionStop, directionStop);
		beforeDirection = directionStop;
		beforeSecondDirection = directionStop;
		unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
		myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
		return;
	}
	
	if(distanceValue > minimumDistanceJ)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		
		if(angle < -148.f)
		{
			angleDirection = directionLeft;
			
			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
			else												secondDirection = directionDown;
		}
		else if(angle < -135.f)
		{
			angleDirection = directionLeftDown;
			
			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionDown;
			else												secondDirection = directionLeft;
		}
		else if(angle < -122.f)
		{
			angleDirection = directionLeftDown;
			
			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionLeft;
			else												secondDirection = directionDown;
		}
		else if(angle < -90.f)
		{
			angleDirection = directionDown;
			
			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
			else												secondDirection = directionLeft;
		}
		else if(angle < -58.f)
		{
			angleDirection = directionDown;
			
			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
			else												secondDirection = directionRight;
		}
		else if(angle < -45.f)
		{
			angleDirection = directionRightDown;
			
			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionRight;
			else												secondDirection = directionDown;
		}
		else if(angle < -32.f)
		{
			angleDirection = directionRightDown;
			
			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionDown;
			else												secondDirection = directionRight;
		}
		else if(angle < 0.f)
		{
			angleDirection = directionRight;
			
			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
			else												secondDirection = directionDown;
		}
		else if(angle < 32.f)
		{
			angleDirection = directionRight;
			
			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
			else												secondDirection = directionUp;
		}
		else if(angle < 45.f)
		{
			angleDirection = directionRightUp;
			
			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionUp;
			else												secondDirection = directionRight;
		}
		else if(angle < 58.f)
		{
			angleDirection = directionRightUp;
			
			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionRight;
			else												secondDirection = directionUp;
		}
		else if(angle < 90.f)
		{
			angleDirection = directionUp;
			
			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
			else												secondDirection = directionRight;
		}
		else if(angle < 122.f)
		{
			angleDirection = directionUp;
			
			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
			else												secondDirection = directionLeft;
		}
		else if(angle < 135.f)
		{
			angleDirection = directionLeftUp;
			
			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionLeft;
			else												secondDirection = directionUp;
		}
		else if(angle < 148.f)
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
		
		if(angleDirection != beforeDirection || isButtonAction)
		{
			isButtonAction = false;
			myJack->changeDirection(angleDirection, secondDirection);
			schedule(schedule_selector(ControlJoystickButton::directionKeeping));
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
			beforeDirection = angleDirection;
			beforeSecondDirection = secondDirection;
		}
		else if(beforeSecondDirection != secondDirection)
		{
			myJack->changeDirection(angleDirection, secondDirection);
			schedule(schedule_selector(ControlJoystickButton::directionKeeping));
			beforeDirection = angleDirection;
			beforeSecondDirection = secondDirection;
		}
		
		myJack->setTouchPointByJoystick(distancePoint, angleDirection, t_b);
	}
	else
	{
		if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
		{
			myJack->changeDirection(directionStop, directionStop);
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
			beforeDirection = directionStop;
			unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
		}
		myJack->setTouchPointByJoystick(distancePoint, directionStop, t_b);
	}
}

void ControlJoystickButton::directionKeeping()
{
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
	myJack->willBackTracking = false;
	myJack->setTouchPointByJoystick(CCPointZero, directionStop, true);
}

void ControlJoystickButton::invisibleControl()
{
	offButton();
	control_ball->setVisible(false);
	control_circle->setVisible(false);
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
		
		if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_left)
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
		
		if(is_button_x)
		{
			// button or ui

			
			if(!button_touch && location.y < 100)
			{
				// button
				button_touch = touch;
				myJack->isDrawingOn = true;
				onButton();
//				draw_button->setColor(ccGRAY);
				
				if(isBacking)
				{
					(target_main->*pauseBackTracking)();
					
					myJack->setJackState(jackStateDrawing);
					
					IntPoint jackPoint = myGD->getJackPoint();
					if(myGD->mapState[jackPoint.x][jackPoint.y] == mapEmpty)
						myGD->mapState[jackPoint.x][jackPoint.y] = mapNewline;
					
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
			if(!joystick_touch && !myJack->willBackTracking && !isStun)
			{
				joystick_touch = touch;
				CCPoint after_circle_position = location;
				
				if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_right)
				{
					if(after_circle_position.x < 90)
						after_circle_position.x = 90;
					else if(after_circle_position.x > 470)
						after_circle_position.x = 470;
					if(after_circle_position.y < 10)
						after_circle_position.y = 10;
					else if(after_circle_position.y > 310)
						after_circle_position.y = 310;
				}
				else
				{
					if(after_circle_position.x < 10)
						after_circle_position.x = 10;
					else if(after_circle_position.x > 390)
						after_circle_position.x = 390;
					if(after_circle_position.y < 10)
						after_circle_position.y = 10;
					else if(after_circle_position.y > 310)
						after_circle_position.y = 310;
				}
				
				control_circle->setPosition(after_circle_position);
				control_circle->setVisible(true);
				
				control_ball->setPosition(location);
				control_ball->setVisible(true);
				
				touchAction(location, false);
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
				float distanceValue = sqrt(pow(distancePoint.x, 2.0) + pow(distancePoint.y, 2.0));
				float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
				
				CCPoint after_circle_position = CCPointZero;
				if(distanceValue > 37)
				{
					float sub_value = distanceValue - 37;
					after_circle_position.x = 1;
					after_circle_position.y = tanf(angle/180.f*M_PI);
					
					float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
					if(angle >= 90 || angle <= -90)
						after_circle_position = ccpMult(after_circle_position, -sub_value/div_value);
					else
						after_circle_position = ccpMult(after_circle_position, sub_value/div_value);
				}
				
//				if(distancePoint.x > 37)
//					after_circle_position.x = distancePoint.x - 37;
//				else if(distancePoint.x < -37)
//					after_circle_position.x = distancePoint.x + 37;
//				if(distancePoint.y > 37)
//					after_circle_position.y = distancePoint.y - 37;
//				else if(distancePoint.y < -37)
//					after_circle_position.y = distancePoint.y + 37;
				
				after_circle_position = ccpAdd(after_circle_position, control_circle->getPosition());
				
				if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_right)
				{
					if(after_circle_position.x < 90)
						after_circle_position.x = 90;
					else if(after_circle_position.x > 470)
						after_circle_position.x = 470;
					if(after_circle_position.y < 10)
						after_circle_position.y = 10;
					else if(after_circle_position.y > 310)
						after_circle_position.y = 310;
				}
				else
				{
					if(after_circle_position.x < 10)
						after_circle_position.x = 10;
					else if(after_circle_position.x > 390)
						after_circle_position.x = 390;
					if(after_circle_position.y < 10)
						after_circle_position.y = 10;
					else if(after_circle_position.y > 310)
						after_circle_position.y = 310;
				}
				
				control_circle->setPosition(after_circle_position);
				
				control_ball->setPosition(location);
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
			if(distanceValue > 37)
			{
//				float sub_value = distanceValue - 37;
				after_circle_position.x = 1;
				after_circle_position.y = tanf(angle/180.f*M_PI);
				
				float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
				if(angle >= 90 || angle <= -90)
					after_circle_position = ccpMult(after_circle_position, -1.f/div_value);
				else
					after_circle_position = ccpMult(after_circle_position, 1.f/div_value);
			}
			after_circle_position = ccpAdd(after_circle_position, control_circle->getPosition());
			
			if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_right)
			{
				if(after_circle_position.x < 90)
					after_circle_position.x = 90;
				else if(after_circle_position.x > 470)
					after_circle_position.x = 470;
				if(after_circle_position.y < 10)
					after_circle_position.y = 10;
				else if(after_circle_position.y > 310)
					after_circle_position.y = 310;
			}
			else
			{
				if(after_circle_position.x < 10)
					after_circle_position.x = 10;
				else if(after_circle_position.x > 390)
					after_circle_position.x = 390;
				if(after_circle_position.y < 10)
					after_circle_position.y = 10;
				else if(after_circle_position.y > 310)
					after_circle_position.y = 310;
			}
			
			control_circle->setPosition(after_circle_position);
			
			control_ball->setPosition(location);
			touchAction(location, true);
			control_circle->setVisible(false);
//			if(myGD->gamescreen_type == kGT_rightUI)	control_circle->setPosition(ccp(50+myGD->boarder_value, 50));
//			else										control_circle->setPosition(ccp(480-50-myGD->boarder_value, 50));;
			control_ball->setVisible(false);
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
		unschedule(schedule_selector(ControlJoystickButton::directionKeeping));
		if(button_touch)
		{
			//				draw_button->setColor(ccWHITE);
			offButton();
			button_touch = NULL;
		}

		if(joystick_touch)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_left)	control_circle->setPosition(ccp(50+myGD->boarder_value, 50));
			else										control_circle->setPosition(ccp(480-50-myGD->boarder_value, 50));;

			control_ball->setVisible(false);
			myJack->setTouchPointByJoystick(CCPointZero, directionStop, t_b);
			joystick_touch = NULL;
		}
	}
}

void ControlJoystickButton::myInit( CCObject* t_main, SEL_CallFunc d_readyBack, Jack* t_jack )
{
	CCLayer::init();

	isButtonAction = false;
	button_touch = NULL;
	joystick_touch = NULL;
	isBacking = false;

	ControlCommon::myInit(t_main, d_readyBack, t_jack);
	beforeDirection = directionStop;
	control_circle = CCSprite::create("control_joystick_big_circle.png");
	control_circle->setScale(1.f);
	control_circle->setVisible(false);
	//		if(myGD->gamescreen_type == kGT_rightUI)	control_circle->setPosition(ccp(50+myGD->boarder_value, 50));
	//		else										control_circle->setPosition(ccp(480-50-myGD->boarder_value, 50));;
	addChild(control_circle);

	control_ball = CCSprite::create("control_joystick_big_ball.png");
	control_ball->setScale(1.f);
	control_ball->setVisible(false);
	addChild(control_ball);

	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	draw_button = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("gameui_button.ccbi",this));
	button_ani = reader->getAnimationManager();
	//		draw_button = CCSprite::create("ui_draw.png");
	if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_left)		draw_button->setPosition(ccp(480-25,25));
//	else if(myGD->gamescreen_type == kGT_leftUI)	draw_button->setPosition(ccp(25,25));
	else								draw_button->setPosition(ccp(25,25));
	addChild(draw_button);

	mType = kCT_Type_Joystick_button;
}

void ControlJoystickButton::onButton()
{
	button_ani->runAnimationsForSequenceNamed("cast1start");
}

void ControlJoystickButton::offButton()
{
	button_ani->runAnimationsForSequenceNamed("cast1stop");
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
