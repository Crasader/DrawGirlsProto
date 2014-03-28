//
//  PlayTutorial.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 19..
//
//

#include "PlayTutorial.h"
#include "DataStorageHub.h"
#include "KSUtil.h"
#include "StageImgLoader.h"
#include "ServerDataSave.h"
#include "StoryManager.h"
#include "StarGoldData.h"
#include "LoadingTipScene.h"

#define minimumDistanceJ	8.f
#define JoystickCenterLimit	30.f

void TutoPathManager::myInit(function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, int t_height)
{
	getMapData = t_getMapData;
	setMapData = t_setMapData;
	height_value = t_height;
	
	overlap_map.clear();
	overlap_map.resize(240*height_value);
	
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			overlap_map[j*240+i] = 0;
		}
	}
	
	int path_color_code = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_lineColor_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1);
	if(path_color_code == 1)
		path_color = "life";
	else if(path_color_code == 2)
		path_color = "fire";
	else if(path_color_code == 3)
		path_color = "water";
	else if(path_color_code == 4)
		path_color = "wind";
	else if(path_color_code == 5)
		path_color = "lightning";
	else if(path_color_code == 6)
		path_color = "plasma";
	else
		path_color = "empty";
}

void TutoCharacter::changeDirection(IntDirection t_d, IntDirection t_sd)
{
	if(my_point.isNull())
		return;
	
	if((getMapData(my_point) != kTutoMapType_line && getMapData(my_point) != kTutoMapType_newLine) || (
	   getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_newLine &&
	   getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_newLine &&
	   getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_newLine &&
	   getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_newLine))
	{
		escapeCharacter();
	}
	
	IntDirection c_d = directionStop;
	IntDirection c_sd = directionStop;
	if(t_d == directionLeftDown)
	{
		c_d = t_sd;
		if(t_sd == directionLeft)						c_sd = directionDown;
		else if(t_sd == directionDown)					c_sd = directionLeft;
	}
	else if(t_d == directionRightDown)
	{
		c_d = t_sd;
		if(t_sd == directionDown)						c_sd = directionRight;
		else if(t_sd == directionRight)					c_sd = directionDown;
	}
	else if(t_d == directionRightUp)
	{
		c_d = t_sd;
		if(t_sd == directionRight)						c_sd = directionUp;
		else if(t_sd == directionUp)					c_sd = directionRight;
	}
	else if(t_d == directionLeftUp)
	{
		c_d = t_sd;
		if(t_sd == directionLeft)						c_sd = directionUp;
		else if(t_sd == directionUp)					c_sd = directionLeft;
	}
	else
	{
		c_d = t_d;
		c_sd = t_sd;
	}
	
	IntVector d_vector = IntVector::directionVector(c_d);
	IntVector sd_vector = IntVector::directionVector(c_sd);
	
	IntPoint d_point = IntPoint(my_point.x + d_vector.dx, my_point.y + d_vector.dy);
	IntPoint sd_point = IntPoint(my_point.x + sd_vector.dx, my_point.y + sd_vector.dy);
	
	bool d_valid = d_point.x >= 0 && d_point.x < 240 && d_point.y >= 0 && d_point.y <= height_value;
	bool sd_valid = sd_point.x >= 0 && sd_point.x < 240 && sd_point.y >= 0 && sd_point.y <= height_value;
	
	if(c_d != directionStop && d_valid && getMapData(d_point) == kTutoMapType_line)
	{
		if(getMapData(my_point) == kTutoMapType_line)
		{
			setCharacterPoint(d_point);
		}
		else if(getMapData(my_point) == kTutoMapType_newLine)
		{
			setCharacterPoint(d_point);
			cleanPath();
			scanMap();
			if((getMapData(my_point) != kTutoMapType_line && getMapData(my_point) != kTutoMapType_newLine) || (
			   getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_newLine))
			{
				escapeCharacter();
			}
		}
	}
	else if(c_d != directionStop && d_valid && isDrawingOn && (getMapData(d_point) == kTutoMapType_empty || getMapData(d_point) == kTutoMapType_newLine))
	{
		addPath(IntPointVector(my_point.x, my_point.y, d_vector.dx, d_vector.dy));
		setCharacterPoint(d_point);
		setMapData(d_point, kTutoMapType_newLine);
	}
	else if(c_sd != directionStop && sd_valid && getMapData(sd_point) == kTutoMapType_line)
	{
		if(getMapData(my_point) == kTutoMapType_line)
		{
			setCharacterPoint(sd_point);
		}
		else if(getMapData(my_point) == kTutoMapType_newLine)
		{
			setCharacterPoint(sd_point);
			cleanPath();
			scanMap();
			if((getMapData(my_point) != kTutoMapType_line && getMapData(my_point) != kTutoMapType_newLine) || (
			   getMapData(IntPoint(my_point.x, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x-1, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_line && getMapData(IntPoint(my_point.x+1, my_point.y)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y-1)) != kTutoMapType_newLine &&
			   getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_line && getMapData(IntPoint(my_point.x, my_point.y+1)) != kTutoMapType_newLine))
			{
				escapeCharacter();
			}
		}
	}
	else if(c_sd != directionStop && sd_valid && isDrawingOn && (getMapData(sd_point) == kTutoMapType_empty || getMapData(sd_point) == kTutoMapType_newLine))
	{
		addPath(IntPointVector(my_point.x, my_point.y, sd_vector.dx, sd_vector.dy));
		setCharacterPoint(sd_point);
		setMapData(sd_point, kTutoMapType_newLine);
	}
}

void TutoCharacter::escapeCharacter()
{
	queue<IntMoveState> bfsArray;
	bfsArray.push(IntMoveState(my_point.x, my_point.y, directionStop));
	
	bool isFinded = false;
	IntPoint findedPoint;
	
	while(!bfsArray.empty() && !isFinded)
	{
		IntMoveState t_ms = bfsArray.front();
		bfsArray.pop();
		if(getMapData(t_ms.origin) == kTutoMapType_line)
		{
			isFinded = true;
			findedPoint = t_ms.origin;
		}
		else
		{
			if(t_ms.direction == directionStop)
			{
				for(int i = directionLeftUp;i <= directionUp; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionLeftUp)
			{
				for(int i = directionLeftUp;i <= directionLeft; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
				
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionLeft)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x-1, t_ms.origin.y, directionLeft);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionLeftDown)
			{
				for(int i = directionLeft;i <= directionDown; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionDown)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y-1, directionDown);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionRightDown)
			{
				for(int i = directionDown;i <= directionRight; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionRight)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionRightUp)
			{
				for(int i = directionRight;i <= directionUp; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionUp)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_ms.origin.x >= 0 && n_ms.origin.x < 240 && n_ms.origin.y >= 0 && n_ms.origin.y <= height_value)
						bfsArray.push(n_ms);}
			}
		}
	}
	
	if(isFinded && !findedPoint.isNull())
	{
		setCharacterPoint(findedPoint);
	}
	else // escape point not found
	{
		CCLog("escape point not found!");
	}
}

void TutoCharacter::setCharacterPoint(IntPoint t_point)
{
	my_point = t_point;
	setPosition(ccpAdd(my_point.convertToCCP(), ccp(2,2)));
	
	if(getRecentStep() == 1 && my_point.x == 120 && my_point.y == (height_value+1)/2 + 11)
	{
		controlerStop();
		nextStep();
	}
}

void TutoCharacter::myInit(function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, int t_height, function<void(IntPointVector)> t_addPath, function<void(void)> t_cleanPath, function<void(void)> t_scanMap, function<int(void)> t_getRecentStep, function<void(void)> t_nextStep)
{
	getMapData = t_getMapData;
	setMapData = t_setMapData;
	addPath = t_addPath;
	cleanPath = t_cleanPath;
	scanMap = t_scanMap;
	
	getRecentStep = t_getRecentStep;
	nextStep = t_nextStep;
	
	height_value = t_height;
	
	auto t_pair = KS::loadCCBIForFullPath<CCSprite*>(this, StageImgLoader::sharedInstance()->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1) + ".ccbi");
	
	character_img = t_pair.first;
	character_manager = t_pair.second;
	
	addChild(character_img);
	
	isDrawingOn = false;
}

void TutoControler::touchAction(CCPoint t_p, bool t_b)
{
	CCPoint distancePoint = ccp(t_p.x - control_circle->getPositionX(), t_p.y - control_circle->getPositionY());
	float distanceValue = sqrtf(powf(distancePoint.x, 2.f) + powf(distancePoint.y, 2.f));
	float angle = atan2(distancePoint.y, distancePoint.x)/M_PI*180.0; // -180 ~ 180
	
	IntDirection angleDirection;
	IntDirection secondDirection;
	
	if(t_b)
	{
		my_char->changeDirection(directionStop, directionStop);
		beforeDirection = directionStop;
		beforeSecondDirection = directionStop;
		unschedule(schedule_selector(TutoControler::directionKeeping));
		return;
	}
	
	if(button_touch && !my_char->isDrawingOn)
	{
		my_char->isDrawingOn = true;
	}
	
	if(distanceValue > minimumDistanceJ)
	{
		IntPoint jackPoint = my_char->my_point;
		
		if(angle < -180.f+regular_spectrum/2.f)
		{
			angleDirection = directionLeft;
			if(jackPoint.y == 0)				secondDirection = directionUp;
			else												secondDirection = directionDown;
		}
		else if(angle < -180.f+(regular_spectrum+irregular_spectrum)/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftDown;
				if(jackPoint.x == 0)				secondDirection = directionDown;
				else												secondDirection = directionLeft;
			}
			else
			{
				angleDirection = directionLeft;
				if(jackPoint.y == 0)				secondDirection = directionUp;
				else												secondDirection = directionDown;
			}
		}
		else if(angle < -180.f+regular_spectrum/2.f+irregular_spectrum)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftDown;
				if(jackPoint.y == 0)				secondDirection = directionLeft;
				else												secondDirection = directionDown;
			}
			else
			{
				angleDirection = directionDown;
				if(jackPoint.x == 0)				secondDirection = directionRight;
				else												secondDirection = directionLeft;
			}
		}
		else if(angle < -180.f+regular_spectrum+irregular_spectrum)
		{
			angleDirection = directionDown;
			if(jackPoint.x == 0)				secondDirection = directionRight;
			else												secondDirection = directionLeft;
		}
		else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum)
		{
			angleDirection = directionDown;
			if(jackPoint.x == 239)				secondDirection = directionLeft;
			else												secondDirection = directionRight;
		}
		else if(angle < -180.f+(regular_spectrum+irregular_spectrum)*3.f/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightDown;
				if(jackPoint.y == 0)				secondDirection = directionRight;
				else												secondDirection = directionDown;
			}
			else
			{
				angleDirection = directionDown;
				if(jackPoint.x == 239)				secondDirection = directionLeft;
				else												secondDirection = directionRight;
			}
		}
		else if(angle < -180.f+regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightDown;
				if(jackPoint.x == 239)				secondDirection = directionDown;
				else												secondDirection = directionRight;
			}
			else
			{
				angleDirection = directionRight;
				if(jackPoint.y == 0)				secondDirection = directionUp;
				else												secondDirection = directionDown;
			}
		}
		else if(angle < 0.f)
		{
			angleDirection = directionRight;
			if(jackPoint.y == 0)				secondDirection = directionUp;
			else												secondDirection = directionDown;
		}
		else if(angle < regular_spectrum/2.f)
		{
			angleDirection = directionRight;
			if(jackPoint.y == height_value)				secondDirection = directionDown;
			else												secondDirection = directionUp;
		}
		else if(angle < regular_spectrum/2.f+irregular_spectrum/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightUp;
				if(jackPoint.x == 239)				secondDirection = directionUp;
				else												secondDirection = directionRight;
			}
			else
			{
				angleDirection = directionRight;
				if(jackPoint.y == height_value)				secondDirection = directionDown;
				else												secondDirection = directionUp;
			}
		}
		else if(angle < regular_spectrum/2.f+irregular_spectrum)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionRightUp;
				if(jackPoint.y == height_value)				secondDirection = directionRight;
				else												secondDirection = directionUp;
			}
			else
			{
				angleDirection = directionUp;
				if(jackPoint.x == 239)				secondDirection = directionLeft;
				else												secondDirection = directionRight;
			}
		}
		else if(angle < regular_spectrum+irregular_spectrum)
		{
			angleDirection = directionUp;
			if(jackPoint.x == 239)				secondDirection = directionLeft;
			else												secondDirection = directionRight;
		}
		else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum)
		{
			angleDirection = directionUp;
			if(jackPoint.x == 0)				secondDirection = directionRight;
			else												secondDirection = directionLeft;
		}
		else if(angle < (regular_spectrum+irregular_spectrum)*3.f/2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftUp;
				if(jackPoint.y == height_value)				secondDirection = directionLeft;
				else												secondDirection = directionUp;
			}
			else
			{
				angleDirection = directionUp;
				if(jackPoint.x == 0)				secondDirection = directionRight;
				else												secondDirection = directionLeft;
			}
		}
		else if(angle < regular_spectrum*3.f/2.f+irregular_spectrum*2.f)
		{
			if(isEnableIrregularDirection)
			{
				angleDirection = directionLeftUp;
				if(jackPoint.x == 0)				secondDirection = directionUp;
				else												secondDirection = directionLeft;
			}
			else
			{
				angleDirection = directionLeft;
				if(jackPoint.y == height_value)				secondDirection = directionDown;
				else												secondDirection = directionUp;
			}
		}
		else
		{
			angleDirection = directionLeft;
			if(jackPoint.y == height_value)				secondDirection = directionDown;
			else												secondDirection = directionUp;
		}
		
		if(angleDirection != beforeDirection || isButtonAction)
		{
			isButtonAction = false;
			my_char->changeDirection(angleDirection, secondDirection);
			schedule(schedule_selector(TutoControler::directionKeeping));
			beforeDirection = angleDirection;
			beforeSecondDirection = secondDirection;
		}
		else if(beforeSecondDirection != secondDirection)
		{
			my_char->changeDirection(angleDirection, secondDirection);
			schedule(schedule_selector(TutoControler::directionKeeping));
			beforeDirection = angleDirection;
			beforeSecondDirection = secondDirection;
		}
	}
	else
	{
		if(my_char->isDrawingOn)
		{
			my_char->changeDirection(directionStop, directionStop);
			beforeDirection = directionStop;
			unschedule(schedule_selector(TutoControler::directionKeeping));
		}
	}
}

void TutoControler::directionKeeping()
{
	my_char->changeDirection(beforeDirection, beforeSecondDirection);
}

void TutoControler::stopMySchedule()
{
	setTouchEnabled(false);
	my_char->changeDirection(directionStop, directionStop);
	beforeDirection = directionStop;
	beforeSecondDirection = directionStop;
	unschedule(schedule_selector(TutoControler::directionKeeping));
}

void TutoControler::resetTouch()
{
	button_touch = NULL;
	joystick_touch = NULL;
	offButton();
	
	if(controlJoystickDirection == kControlJoystickDirection_right)
	{
		control_circle->setPosition(ccp(480-40-7.f, 40));
		control_ball->setPosition(ccp(480-40-7.f, 40));
	}
	else
	{
		control_circle->setPosition(ccp(40+7.f, 40));
		control_ball->setPosition(ccp(40+7.f, 40));
	}
}

void TutoControler::invisibleControl()
{
	offButton();
	control_ball->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	control_circle->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	
	if(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick)
	{
		if(controlJoystickDirection == kControlJoystickDirection_right)
		{
			control_circle->setPosition(ccp(480-40-7.f, 40));
			control_ball->setPosition(ccp(480-40-7.f, 40));
		}
		else
		{
			control_circle->setPosition(ccp(40+7.f, 40));
			control_ball->setPosition(ccp(40+7.f, 40));
		}
	}
}

void TutoControler::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
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
		
		if(!isDisableDrawButton && is_button_x)
		{
			// button or ui
			
			
			if(!button_touch && location.y < 100)
			{
				// button
				button_touch = touch;
				my_char->isDrawingOn = true;
				onButton();
				
				if(isBacking)
				{
					(target_main->*pauseBackTracking)();
					
					IntPoint jackPoint = my_char->my_point;
					if(getMapData(jackPoint) == kTutoMapType_empty)
					{
						if(!isDisableLineOver)
							checkBeforeNewLine(jackPoint);
						setMapData(jackPoint, kTutoMapType_newLine);
					}
					
					if(joystick_touch)
					{
						isButtonAction = true;
						CCPoint joystick_location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(joystick_touch->getLocationInView()));
						touchAction(joystick_location, false);
						continue;
					}
				}
				
				if(joystick_touch && !my_char->isMoving)
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
			if(!joystick_touch)
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
					
					if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x < 480-40-120 || after_circle_position.y > 40+120)
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
					
					if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x > 40+120 || after_circle_position.y > 40+120)
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
				
				IntPoint jackPoint = my_char->my_point;
				if(getMapData(jackPoint) == kTutoMapType_empty)
				{
					if(!isDisableLineOver)
						checkBeforeNewLine(jackPoint);
					setMapData(jackPoint, kTutoMapType_newLine);
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
					
					if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x < 480-40-120 || after_circle_position.y > 40+120)
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
					
					if(isControlJoystickNotFixed)
						control_circle->setPosition(after_circle_position);
					else
					{
						if(after_circle_position.x > 40+120 || after_circle_position.y > 40+120)
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

void TutoControler::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
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
//			if(!myJack->willBackTracking && !isStun)
//			{
				CCPoint distancePoint = ccp(location.x - control_circle->getPositionX(), location.y - control_circle->getPositionY());
				float distanceValue = sqrtf(powf(distancePoint.x, 2.0) + powf(distancePoint.y, 2.0));
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
							if(circle_position.x < 480-40-120)
								circle_position.x = 480-40-120;
							if(circle_position.y > 40+120)
								circle_position.y = 40+120;
							
							control_circle->setPosition(circle_position);
							
							float t_distance = distanceValue;
							if(distanceValue > 20)
								t_distance = 20;
							
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
						if(distanceValue > 20)
							t_distance = 20;
						
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
							if(circle_position.x > 40+120)
								circle_position.x = 40+120;
							if(circle_position.y > 40+120)
								circle_position.y = 40+120;
							
							control_circle->setPosition(circle_position);
							
							float t_distance = distanceValue;
							if(distanceValue > 20)
								t_distance = 20;
							
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
						if(distanceValue > 20)
							t_distance = 20;
						
						CCPoint inner_position = ccpAdd(control_circle->getPosition(), ccpMult(ccp(cosf(angle/180.f*M_PI), sinf(angle/180.f*M_PI)), t_distance));
						
						control_ball->setPosition(inner_position);
					}
				}
				
				
				touchAction(location, false);
//			}
		}
	}
}

void TutoControler::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter;
	CCTouch* touch;
	
	for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		touch = (CCTouch*)(*iter);
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		
		if(touch == button_touch)
		{
			offButton();
			
			if(my_char->isDrawingOn)
				(target_main->*delegate_readyBack)();
			
			my_char->isDrawingOn = false;
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
				after_circle_position.x = 1;
				after_circle_position.y = tanf(angle/180.f*M_PI);
				
				float div_value = sqrtf(powf(after_circle_position.x, 2.f) + powf(after_circle_position.y, 2.f));
				if(angle >= 90 || angle <= -90)
					after_circle_position = ccpMult(after_circle_position, -1.f/div_value);
				else
					after_circle_position = ccpMult(after_circle_position, 1.f/div_value);
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
				
				if(!isJoystickCenterNotFixed)
				{
					if(!isControlJoystickNotFixed)
					{
						CCPoint circle_position = after_circle_position;
						if(circle_position.x < 480-40-120)
							circle_position.x = 480-40-120;
						if(circle_position.y > 40+120)
							circle_position.y = 40+120;
						
						control_circle->setPosition(circle_position);
						
						float t_distance = distanceValue;
						if(distanceValue > 20)
							t_distance = 20;
						
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
					if(distanceValue > 20)
						t_distance = 20;
					
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
				
				if(!isJoystickCenterNotFixed)
				{
					if(!isControlJoystickNotFixed)
					{
						CCPoint circle_position = after_circle_position;
						if(circle_position.x > 40+120)
							circle_position.x = 40+120;
						if(circle_position.y > 40+120)
							circle_position.y = 40+120;
						
						control_circle->setPosition(circle_position);
						
						float t_distance = distanceValue;
						if(distanceValue > 20)
							t_distance = 20;
						
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
					if(distanceValue > 20)
						t_distance = 20;
					
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
					control_circle->setPosition(ccp(480-40-7.f, 40));
					control_ball->setPosition(ccp(480-40-7.f, 40));
				}
				else
				{
					control_circle->setPosition(ccp(40+7.f, 40));
					control_ball->setPosition(ccp(40+7.f, 40));
				}
			}
			
			if(isDisableDrawButton && my_char->isDrawingOn)
			{
				(target_main->*delegate_readyBack)();
			}
			
			joystick_touch = NULL;
		}
	}
}

void TutoControler::ccTouchesCancelled(CCSet *pTouch, CCEvent *pEvent)
{
	ccTouchesEnded(pTouch, pEvent);
}

void TutoControler::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, -110);
}

TutoControler* TutoControler::create(TutoCharacter* t_char, int t_height, function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, function<void(IntPoint)> t_checkBeforeNewLine)
{
	TutoControler* t_cjf = new TutoControler();
	t_cjf->myInit(t_char, t_height, t_getMapData, t_setMapData, t_checkBeforeNewLine);
	t_cjf->autorelease();
	return t_cjf;
}

void TutoControler::buttonSetVisible(bool t_b)
{
	isDisableDrawButton = !t_b;
	draw_button->setVisible(t_b);
}
void TutoControler::joystickSetVisible(bool t_b)
{
	control_ball->setVisible(t_b);
	control_circle->setVisible(t_b);
	
	setTouchEnabled(t_b);
}

void TutoControler::myInit(TutoCharacter* t_char, int t_height, function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, function<void(IntPoint)> t_checkBeforeNewLine)
{
	CCLayer::init();
	
	my_char = t_char;
	height_value = t_height;
	getMapData = t_getMapData;
	setMapData = t_setMapData;
	checkBeforeNewLine = t_checkBeforeNewLine;
	
	regular_spectrum = 70.f;
	irregular_spectrum = 20.f;
	
	isButtonAction = false;
	button_touch = NULL;
	joystick_touch = NULL;
	isBacking = false;
	
	isEnableIrregularDirection = myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection);
	isControlJoystickNotFixed = myDSH->getBoolForKey(kDSH_Key_isControlJoystickNotFixed);
	isAlwaysInvisibleJoystick = myDSH->getBoolForKey(kDSH_Key_isAlwaysInvisibleJoystick);
	controlJoystickDirection = myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection);
	isDisableDrawButton = myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton);
	isDisableLineOver = myDSH->getBoolForKey(kDSH_Key_isDisableLineOver);
	isJoystickCenterNotFixed = myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed);
	
	beforeDirection = directionStop;
	control_circle = CCSprite::create("control_joystick_big_circle.png");
	control_circle->setScale(1.f);
	control_circle->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	addChild(control_circle);
	
	control_ball = CCSprite::create("control_joystick_big_ball.png");
	control_ball->setScale(1.f);
	control_ball->setVisible(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick);
	addChild(control_ball);
	
	if(!isControlJoystickNotFixed || !isAlwaysInvisibleJoystick)
	{
		if(controlJoystickDirection == kControlJoystickDirection_right)
		{
			control_circle->setPosition(ccp(480-40-7.f, 40));
			control_ball->setPosition(ccp(480-40-7.f, 40));
		}
		else
		{
			control_circle->setPosition(ccp(40+7.f, 40));
			control_ball->setPosition(ccp(40+7.f, 40));
		}
	}
	
	if(!isDisableDrawButton)
	{
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		draw_button = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("gameui_button.ccbi",this));
		button_ani = reader->getAnimationManager();
		//		draw_button = CCSprite::create("ui_draw.png");
		if(controlJoystickDirection == kControlJoystickDirection_left)		draw_button->setPosition(ccp(480-40,40));
		else								draw_button->setPosition(ccp(40,40));
		addChild(draw_button);
		reader->release();
	}
}

void TutoControler::onButton()
{
	if(button_ani)
		button_ani->runAnimationsForSequenceNamed("cast1start");
}

void TutoControler::offButton()
{
	if(button_ani)
		button_ani->runAnimationsForSequenceNamed("cast1stop");
}

void BackImg::visit()
{
	silhouette->visit();
	
	unsigned int loopCnt = drawRects->count();
	
	glEnable(GL_SCISSOR_TEST);
	
	for(int i=0;i<loopCnt;i++)
	{
		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
		
		float wScale = viewport[2] / design_resolution_size.width;
		float hScale = viewport[3] / design_resolution_size.height;
		
		float x, y, w, h;
		
		x = (t_rect->origin.x)*wScale + viewport[0]-1;
		y = (t_rect->origin.y)*hScale + viewport[1]-1;
		w = (t_rect->size.width)*wScale+2;
		h = (t_rect->size.height)*hScale+2;
		
		glScissor(x,y,w,h);
		draw();
	}
	
	glDisable(GL_SCISSOR_TEST);
}

void BackImg::startSilhouette()
{
	ing_cnt = 0;
	schedule(schedule_selector(BackImg::silhouetteAction));
}

void BackImg::silhouetteAction()
{
	ing_cnt++;
	if(ing_cnt%30 == 0)
	{
		int tick_cnt = ing_cnt/30;
		if(tick_cnt%2 == 1)
			silhouette->setColor(ccc3(100, 100, 255));
		else
			silhouette->setColor(ccBLUE);
		
		if(tick_cnt == 6)
			stopSilhouette();
	}
}

void BackImg::stopSilhouette()
{
	silhouette->setColor(ccBLUE);
	unschedule(schedule_selector(BackImg::silhouetteAction));
}

void BackImg::myInit()
{
	initWithFile("play_tutorial_real.png");
	setPosition(ccp(240,myDSH->ui_center_y));
	
	silhouette = CCSprite::create("play_tutorial_sil.png");
	silhouette->setPosition(ccp(getContentSize().width/2.f,getContentSize().height/2.f));
	addChild(silhouette, -1);
	silhouette->setColor(ccBLUE);
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	design_resolution_size = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	drawRects = new CCArray(1);
}


CCScene* PlayTutorial::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	PlayTutorial *layer = PlayTutorial::create();
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool PlayTutorial::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	map_data.clear();
	height_value = int(myDSH->ui_top/2);
	map_data.resize(240*height_value);
	
	//초기값 넣어주기
	for(int j=0;j<24;j++)
	{
		for(int i=0;i<30;i++)
		{
			if(j == 0 || j == 24-1 || i == 0 || i == 30-1)
				setMapData(120-15+i, height_value/2-12+j, kTutoMapType_line);
			else
				setMapData(120-15+i, height_value/2-12+j, kTutoMapType_get);
		}
	}

	view_img = BackImg::create();
	addChild(view_img, 1);
	
	resetRects();
	
	path_manager = TutoPathManager::create([=](IntPoint t_p){ return getMapData(t_p.x, t_p.y); }, [=](IntPoint t_p, TutoMapType t_type){ setMapData(t_p.x, t_p.y, t_type); }, height_value);
	addChild(path_manager, 2);
	
	character = TutoCharacter::create([=](IntPoint t_p){ return getMapData(t_p.x, t_p.y); }, [=](IntPoint t_p, TutoMapType t_type){ setMapData(t_p.x, t_p.y, t_type); }, height_value-1,
									  [=](IntPointVector t_pv){ path_manager->addPath(t_pv); }, [=](){ path_manager->cleanPath(); }, [=](){ scanMap(); }, [=](){ return getRecentStep(); }, [=](){nextStep();});
	character->setCharacterPoint(IntPoint(120,height_value/2-12));
	addChild(character, 3);
	
	controler = TutoControler::create(character, height_value-1, [=](IntPoint t_p){ return getMapData(t_p.x, t_p.y); }, [=](IntPoint t_p, TutoMapType t_type){ setMapData(t_p.x, t_p.y, t_type); },
									  [=](IntPoint t_p){ path_manager->checkBeforeNewline(t_p); });
	controler->target_main = this;
	controler->delegate_readyBack = callfunc_selector(PlayTutorial::startBackTracking);
	controler->pauseBackTracking = callfunc_selector(PlayTutorial::stopBackTracking);
	addChild(controler, 5);
	
	character->controlerStop = [=](){controler->stopMySchedule(); controler->resetTouch();};
	
	top_label = CCLabelTTF::create("", mySGD->getFont().c_str(), 14);
	top_label->setPosition(ccp(240, myDSH->ui_top-30));
	addChild(top_label, 6);
	
	mark_img = KS::loadCCBI<CCSprite*>(this, "tutorial_target.ccbi").first;
	mark_img->setVisible(false);
	mark_img->setPosition(ccp(240, height_value+24));
	addChild(mark_img, 2);
	
	controler->buttonSetVisible(false);
	controler->joystickSetVisible(false);
	
	tutorial_step = 0;
	
	StoryManager* t_sm = StoryManager::create(-500);
	addChild(t_sm, 100);
	
	
	CCSprite* gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(0,-160+myDSH->ui_center_y));
	gray->setScaleY(myDSH->ui_top/320.f);
	gray->runAction(CCFadeTo::create(0.5f, 255));
	t_sm->back_node->addChild(gray);
	
	t_sm->addMent(true, "", "", "조작방법에 대한 튜토리얼을 시작하겠습니다.", [=]()
	{
		gray->runAction(CCFadeTo::create(0.3f, 0));
		t_sm->addMent(true, "", "", "가운데 빨간 동그라미가 캐릭터 입니다.\n캐릭터를 이동시켜서 영역 가장자리를 이동할 수도 있고\n영역을 획득할 수도 있습니다.", [=]()
		{
			t_sm->addMent(true, "", "", "먼저 영역 위를 이동하는 방법에 대해 소개해드릴게요.", [=]()
			{
				controler->joystickSetVisible(true);
				mark_img->setVisible(true);
				t_sm->addMent(true, "", "", "오른쪽 아래에 조이스틱이 있습니다.\n이 조이스틱으로 캐릭터를 원하는 방향으로 이동시킬 수 있어요.\n조이스틱으로 캐릭터를 위로 이동시켜보세요.", [=]()
				{
					top_label->setString("캐릭터를 위로 이동시키기");
					tutorial_step = 1;
					t_sm->removeFromParent();
				});
			});
		});
	});
	
	return true;
}

int PlayTutorial::getRecentStep()
{
	return tutorial_step;
}

void PlayTutorial::nextStep()
{
	tutorial_step++;
	
	if(tutorial_step == 2)
	{
		top_label->setString("");
		mark_img->setVisible(false);
		
		
		StoryManager* t_sm = StoryManager::create(-500);
		addChild(t_sm, 100);
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(0,-160+myDSH->ui_center_y));
		gray->setScaleY(myDSH->ui_top/320.f);
		gray->runAction(CCFadeTo::create(0.5f, 255));
		t_sm->back_node->addChild(gray);
		
		t_sm->addMent(true, "", "", "훌륭해요!\n다음에는 영역을 획득하는 방법을 알아보도록 해요.", [=]()
		{
			gray->runAction(CCFadeTo::create(0.3f, 0));
			controler->buttonSetVisible(true);
			t_sm->addMent(true, "", "", "왼쪽 아래의 꾸욱 버튼이 보이시나요?\n꾸욱 버튼을 누르고 있으면 영역 바깥으로 나갈 수 있답니다.", [=]()
			{
				area_take_sample = CCClippingNode::create(CCSprite::create("whitePaper.png", CCRectMake(0, 0, 330, 210)));
				CCSprite* t_ccbi = KS::loadCCBI<CCSprite*>(this, "tutorial_new.ccbi").first;
				area_take_sample->addChild(t_ccbi);
				area_take_sample->setPosition(ccp(240,220));
				addChild(area_take_sample, 101);
				t_sm->addMent(true, "", "", "보이는 것처럼 영역을 획득해보세요.                                                                  ", [=]()
				{
					controler->setTouchEnabled(true);
					area_take_sample->removeFromParent();
					mark_img->setPosition(ccp(80,myDSH->ui_center_y));
					mark_img->setVisible(true);
					
					top_label->setString("영역 획득하기");
					tutorial_step = 3;
					
					startCatching();
					
					t_sm->removeFromParent();
				});
			});
		});
	}
	else if(tutorial_step == 4)
	{
		top_label->setString("");
		mark_img->setVisible(false);
		
		StoryManager* t_sm = StoryManager::create(-500);
		addChild(t_sm, 100);
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(0,-160+myDSH->ui_center_y));
		gray->setScaleY(myDSH->ui_top/320.f);
		gray->runAction(CCFadeTo::create(0.5f, 255));
		t_sm->back_node->addChild(gray);
		
		t_sm->addMent(true, "", "", "훌륭해요!\n기본적인 조작방법을 모두 익히셨습니다.               ", [=]()
		{
			gray->stopAllActions();
			gray->runAction(CCFadeTo::create(0.3f, 0));
			
			CCSprite* ui_percent = CCSprite::create("play_tutorial_gage.png");
			ui_percent->setPosition(ccp(240,myDSH->ui_top-15));
			addChild(ui_percent, 4);
			
			ui_percent->setVisible(false);
			
			CCSequence* t_sequ = CCSequence::create(CCDelayTime::create(0.5f), CCShow::create(), NULL);
			CCSequence* t_seq = CCSequence::create(CCDelayTime::create(0.5f), CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), NULL);
			CCRepeat* t_repeat = CCRepeat::create(t_seq, 3);
			CCSequence* t_seqq = CCSequence::create(t_sequ, t_repeat, NULL);
			
			ui_percent->runAction(t_seqq);
			
			view_img->startSilhouette();
			
			t_sm->addMent(true, "", "", "실루엣 영역을 획득해야 게임 달성도가 올라갑니다.", [=]()
			{
				view_img->stopSilhouette();
				ui_percent->stopAllActions();
				ui_percent->setVisible(true);
				
				CCSprite* clear_condition = CCSprite::create("play_tutorial_gage_guide.png");
				clear_condition->setPosition(ccp(240,myDSH->ui_top-58));
				addChild(clear_condition, 4);
				
				CCSequence* t_seq = CCSequence::create(CCDelayTime::create(0.5f), CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), NULL);
				CCRepeat* t_repeat = CCRepeat::create(t_seq, 3);
				clear_condition->runAction(t_repeat);
				
				CCLabelBMFont* time_label = CCLabelBMFont::create("100", "timefont.fnt");
				time_label->setPosition(ccp(240,35));
				addChild(time_label, 4);
				
				CCSequence* t_seq2 = CCSequence::create(CCDelayTime::create(0.5f), CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), NULL);
				CCRepeat* t_repeat2 = CCRepeat::create(t_seq2, 3);
				time_label->runAction(t_repeat2);
				
				CCSprite* time_case = CCSprite::create("play_tutorial_time_guide.png");
				time_case->setPosition(ccp(240,52));
				addChild(time_case, 4);
				
				CCSequence* t_seq3 = CCSequence::create(CCDelayTime::create(0.5f), CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), NULL);
				CCRepeat* t_repeat3 = CCRepeat::create(t_seq3, 3);
				time_case->runAction(t_repeat3);
				
				t_sm->addMent(true, "", "", "제한시간 내에 달성도 85%를 넘기면 클리어!!", [=]()
				{
					clear_condition->stopAllActions();
					clear_condition->setVisible(true);
					
					time_label->stopAllActions();
					time_label->setVisible(true);
					
					time_case->stopAllActions();
					time_case->setVisible(true);
					
					mySGD->setGold(mySGD->getGold() + 5000);
					
					t_sm->addMent(true, "", "", "기본 튜토리얼을 모두 진행하셨습니다.\n보상으로 5000골드를 드립니다.\n월드맵으로 돌아갑니다.", [=]()
					{
						t_sm->removeFromParent();
						mySGD->setNextSceneName("maingame");
						CCDirector::sharedDirector()->replaceScene(LoadingTipScene::scene());
					});
				}, CCSizeMake(350,100), ccp(0,0), 12);
			});
		});
	}
}

void PlayTutorial::startCatching()
{
	schedule(schedule_selector(PlayTutorial::catchingAction));
}
void PlayTutorial::catchingAction()
{
	if(getMapData(40, height_value/2) == kTutoMapType_get || getMapData(40, height_value/2) == kTutoMapType_line)
	{
		stopCatching();
	}
}
void PlayTutorial::stopCatching()
{
	unschedule(schedule_selector(PlayTutorial::catchingAction));
	controler->stopMySchedule();
	controler->resetTouch();
	nextStep();
}

TutoMapType PlayTutorial::getMapData(int x, int y)
{
	if(!(x >= 0 && x < 240) || !(y >= 0 && y < height_value))
	{
		return kTutoMapType_empty;
	}
//	CCAssert(x >= 0 && x < 240, "invalid x");
//	CCAssert(y >= 0 && y < height_value, "invalid y");
	
	return map_data[y*240+x];
}
void PlayTutorial::setMapData(int x, int y, TutoMapType value)
{
	CCAssert(x >= 0 && x < 240, "invalid x");
	CCAssert(y >= 0 && y < height_value, "invalid y");
	
	map_data[y*240+x] = value;
}

void PlayTutorial::startBackTracking()
{
	CCLog("start backtracking");
	controler->isBacking = true;
	schedule(schedule_selector(PlayTutorial::backTracking));
}
void PlayTutorial::backTracking()
{
	IntPoint afterJackPoint = path_manager->pathBackTracking();
	
	if(afterJackPoint.isNull())
	{
		stopBackTracking();
		return;
	}
	else
	{
		character->setCharacterPoint(afterJackPoint);
	}
	
	afterJackPoint = path_manager->pathBackTracking();
	
	if(afterJackPoint.isNull())
	{
		stopBackTracking();
		return;
	}
	else
	{
		character->setCharacterPoint(afterJackPoint);
	}
	
	afterJackPoint = path_manager->pathBackTracking();
	
	if(afterJackPoint.isNull())
	{
		stopBackTracking();
		return;
	}
	else
	{
		character->setCharacterPoint(afterJackPoint);
	}
}
void PlayTutorial::stopBackTracking()
{
	CCLog("stop backtracking");
	unschedule(schedule_selector(PlayTutorial::backTracking));
	controler->isBacking = false;
}

void PlayTutorial::scanMap()
{
	for(int i=0;i<240;i++)
	{
		if(getMapData(i, 0) == kTutoMapType_empty)
			bfsCheck(kTutoMapType_empty, kTutoMapType_scanningEmpty, IntPoint(i, 0));
		if(getMapData(i, height_value-1) == kTutoMapType_empty)
			bfsCheck(kTutoMapType_empty, kTutoMapType_scanningEmpty, IntPoint(i, height_value-1));
	}
	
	for(int j=0;j<height_value;j++)
	{
		if(getMapData(0, j) == kTutoMapType_empty)
			bfsCheck(kTutoMapType_empty, kTutoMapType_scanningEmpty, IntPoint(0, j));
		if(getMapData(240-1, j) == kTutoMapType_empty)
			bfsCheck(kTutoMapType_empty, kTutoMapType_scanningEmpty, IntPoint(240-1, j));
	}
	
	// new inside check
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_empty)
				bfsCheck(kTutoMapType_empty, kTutoMapType_newGet, IntPoint(i, j));
		}
	}
	
	// outside recovery and new inside add show
//	int newInsideCnt = 0;
//	int sil_inside_cnt = 0;
//	int empty_inside_cnt = 0;
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_scanningEmpty)
				setMapData(i, j, kTutoMapType_empty);
			else if(getMapData(i, j) == kTutoMapType_newLine)
			{
				setMapData(i, j, kTutoMapType_line);
//				newInsideCnt++;
//				if(mySD->silData[i][j])		sil_inside_cnt++;
//				else						empty_inside_cnt++;
			}
			else if(getMapData(i, j) == kTutoMapType_newGet)
			{
				setMapData(i, j, kTutoMapType_get);
//				newInsideCnt++;
//				if(mySD->silData[i][j])		sil_inside_cnt++;
//				else						empty_inside_cnt++;
			}
		}
	}
	
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_line &&
			   getMapData(i-1, j) != kTutoMapType_empty && getMapData(i-1, j+1) != kTutoMapType_empty &&
			   getMapData(i-1, j-1) != kTutoMapType_empty && getMapData(i, j+1) != kTutoMapType_empty &&
			   getMapData(i, j-1) != kTutoMapType_empty && getMapData(i+1, j+1) != kTutoMapType_empty &&
			   getMapData(i+1, j) != kTutoMapType_empty && getMapData(i+1, j-1) != kTutoMapType_empty &&
			   
			   i >= 0 && i < 240 && j >= 0 && j < height_value)
			{
				setMapData(i, j, kTutoMapType_get);
			}
		}
	}
	
	for(int i=0;i<240;i++)
	{
		if(getMapData(i, 0) != kTutoMapType_empty)
			setMapData(i, 0, kTutoMapType_line);
		if(getMapData(i, height_value-1) != kTutoMapType_empty)
			setMapData(i, height_value-1, kTutoMapType_line);
	}
	for(int j=0;j<height_value;j++)
	{
		if(getMapData(0, j) != kTutoMapType_empty)
			setMapData(0, j, kTutoMapType_line);
		if(getMapData(240-1, j) != kTutoMapType_empty)
			setMapData(240-1, j, kTutoMapType_line);
	}
	
	resetRects();
}

void PlayTutorial::bfsCheck(TutoMapType beforeType, TutoMapType afterType, IntPoint startPoint)
{
	T_BFS_Point s_p;
	s_p.x = startPoint.x;
	s_p.y = startPoint.y;
	queue<T_BFS_Point> bfsArray;
	setMapData(s_p.x, s_p.y, afterType);
	bfsArray.push(s_p);
	
	while(!bfsArray.empty())
	{
		T_BFS_Point t_p = bfsArray.front();
		bfsArray.pop();
		for(int i=directionLeft;i<=directionUp;i+=2)
		{
			IntVector t_iv = IntVector::directionVector((IntDirection)i);
			
			T_BFS_Point t_v;
			t_v.x = t_iv.dx;
			t_v.y = t_iv.dy;
			
			T_BFS_Point a_p;
			a_p.x = t_p.x+t_v.x;
			a_p.y = t_p.y+t_v.y;
			
			if(a_p.x >= 0 && a_p.x < 240 && a_p.y >= 0 && a_p.y < height_value && getMapData(a_p.x, a_p.y) == beforeType)
			{
				setMapData(a_p.x, a_p.y, afterType);
				bfsArray.push(a_p);
			}
		}
	}
}

void PlayTutorial::resetRects()
{
	CCArray* rects = CCArray::createWithCapacity(256);
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_line || getMapData(i, j) == kTutoMapType_get)
			{
				IntRect* t_rect = newRectChecking(IntMoveState(i, j, directionRightUp));
				rects->addObject(t_rect);
			}
		}
	}
	
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<height_value;j++)
		{
			if(getMapData(i, j) == kTutoMapType_checkLine)
				setMapData(i, j, kTutoMapType_line);
			else if(getMapData(i, j) == kTutoMapType_checkGet)
				setMapData(i, j, kTutoMapType_get);
		}
	}
	
	view_img->setDrawRects(rects);
}

IntRect* PlayTutorial::newRectChecking(IntMoveState start)
{
	IntPoint origin = IntPoint(start.origin.x, start.origin.y);
	IntSize size = IntSize(0, 0);
	
	bool isUpper = true;
	bool isRighter = true;
	queue<IntMoveState> loopArray;
	loopArray.push(start);
	
	queue<IntMoveState> nextLoopArray;
	
	//	int loopCnt;
	
	while(!loopArray.empty())
	{
		if(isUpper)				size.height++;
		if(isRighter)			size.width++;
		
		bool upable = isUpper;
		bool rightable = isRighter;
		
		while(!loopArray.empty())
		{
			//			loopCnt++;
			IntMoveState t_ms = loopArray.front();
			loopArray.pop();
			
			
			if(t_ms.direction == directionUp && !isUpper)
				continue;
			if(t_ms.direction == directionRight && !isRighter)
				continue;
			
			if(getMapData(t_ms.origin.x, t_ms.origin.y) == kTutoMapType_get)
				setMapData(t_ms.origin.x, t_ms.origin.y, kTutoMapType_checkGet);
			else if(getMapData(t_ms.origin.x, t_ms.origin.y) == kTutoMapType_line)
				setMapData(t_ms.origin.x, t_ms.origin.y, kTutoMapType_checkLine);
			
			if(t_ms.direction == directionUp)
			{
				if(isUpper)
				{
					IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_msUp.origin.x >= 0 && n_msUp.origin.x < 240 && n_msUp.origin.y >= 0 && n_msUp.origin.y < height_value &&
					   (getMapData(n_msUp.origin.x, n_msUp.origin.y) == kTutoMapType_line || getMapData(n_msUp.origin.x, n_msUp.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msUp);
					else		upable = false;
				}
			}
			else if(t_ms.direction == directionRight)
			{
				if(isRighter)
				{
					IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_msRight.origin.x >= 0 && n_msRight.origin.x < 240 && n_msRight.origin.y >= 0 && n_msRight.origin.y < height_value &&
					   (getMapData(n_msRight.origin.x, n_msRight.origin.y) == kTutoMapType_line || getMapData(n_msRight.origin.x, n_msRight.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msRight);
					else		rightable = false;
				}
			}
			else if(t_ms.direction == directionRightUp)
			{
				if(isUpper)
				{
					IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_msUp.origin.x >= 0 && n_msUp.origin.x < 240 && n_msUp.origin.y >= 0 && n_msUp.origin.y < height_value &&
					   (getMapData(n_msUp.origin.x, n_msUp.origin.y) == kTutoMapType_line || getMapData(n_msUp.origin.x, n_msUp.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msUp);
					else		upable = false;
				}
				
				if(isRighter)
				{
					IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_msRight.origin.x >= 0 && n_msRight.origin.x < 240 && n_msRight.origin.y >= 0 && n_msRight.origin.y < height_value &&
					   (getMapData(n_msRight.origin.x, n_msRight.origin.y) == kTutoMapType_line || getMapData(n_msRight.origin.x, n_msRight.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msRight);
					else		rightable = false;
				}
				
				if(upable && rightable)
				{
					IntMoveState n_msRightUp = IntMoveState(t_ms.origin.x+1, t_ms.origin.y+1, directionRightUp);
					if(n_msRightUp.origin.x >= 0 && n_msRightUp.origin.x < 240 && n_msRightUp.origin.y >= 0 && n_msRightUp.origin.y < height_value &&
					   (getMapData(n_msRightUp.origin.x, n_msRightUp.origin.y) == kTutoMapType_line || getMapData(n_msRightUp.origin.x, n_msRightUp.origin.y) == kTutoMapType_get))
						nextLoopArray.push(n_msRightUp);
					else		rightable = false;
				}
			}
		}
		
		isUpper = upable;
		isRighter = rightable;
		
		if(isUpper || isRighter)
		{
			while(!nextLoopArray.empty())
			{
				loopArray.push(nextLoopArray.front());
				nextLoopArray.pop();
			}
		}
	}
	
	//	CCLog("loop count : %d", loopCnt);
	
	IntRect* r_rect = new IntRect(origin.x*pixelSize, origin.y*pixelSize, size.width*pixelSize, size.height*pixelSize);
	r_rect->autorelease();
	return r_rect;
}