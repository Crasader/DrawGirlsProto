//
//  Jack.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 29..
//
//

#include "Jack.h"
#include "ASPopupView.h"
#include "StarGoldData.h"
#include "CCMenuLambda.h"
#include "MaingameScene.h"
#include "TutorialFlowStep.h"
#include "MyLocalization.h"
#include "AchieveNoti.h"
#include "CommonAnimation.h"
void Jack::searchAndMoveOldline(IntMoveState searchFirstMoveState)
{
	queue<IntMoveState> bfsArray;
	bfsArray.push(searchFirstMoveState);
	
	bool isFinded = false;
	IntPoint findedPoint;
	bool is_not_outline_found = false;
	IntPoint not_outline_found_point;
	
	while(!bfsArray.empty() && !is_not_outline_found)//!isFinded)
	{
		IntMoveState t_ms = bfsArray.front();
		bfsArray.pop();
		if(myGD->mapState[t_ms.origin.x][t_ms.origin.y] == mapOldline)
		{
			if(!isFinded)
			{
				isFinded = true;
				findedPoint = t_ms.origin;
			}
			
			if((t_ms.origin.x != mapWidthInnerBegin && t_ms.origin.x != mapWidthInnerEnd-1) && (t_ms.origin.y != mapHeightInnerBegin || t_ms.origin.y != mapHeightInnerEnd-1))
			{
				is_not_outline_found = true;
				not_outline_found_point = t_ms.origin;
			}
			else
			{
				if(t_ms.direction == directionStop)
				{
					for(int i = directionLeftUp;i <= directionUp; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
				}
				else if(t_ms.direction == directionLeftUp)
				{
					for(int i = directionLeftUp;i <= directionLeft; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
					
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
				else if(t_ms.direction == directionLeft)
				{
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x-1, t_ms.origin.y, directionLeft);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
				else if(t_ms.direction == directionLeftDown)
				{
					for(int i = directionLeft;i <= directionDown; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
				}
				else if(t_ms.direction == directionDown)
				{
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y-1, directionDown);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
				else if(t_ms.direction == directionRightDown)
				{
					for(int i = directionDown;i <= directionRight; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
				}
				else if(t_ms.direction == directionRight)
				{
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
				else if(t_ms.direction == directionRightUp)
				{
					for(int i = directionRight;i <= directionUp; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
				}
				else if(t_ms.direction == directionUp)
				{
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
			}
		}
		else
		{
			if(t_ms.direction == directionStop)
			{
				for(int i = directionLeftUp;i <= directionUp; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionLeftUp)
			{
				for(int i = directionLeftUp;i <= directionLeft; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
				
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionLeft)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x-1, t_ms.origin.y, directionLeft);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionLeftDown)
			{
				for(int i = directionLeft;i <= directionDown; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionDown)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y-1, directionDown);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionRightDown)
			{
				for(int i = directionDown;i <= directionRight; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionRight)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
			else if(t_ms.direction == directionRightUp)
			{
				for(int i = directionRight;i <= directionUp; i++)
				{
					IntVector t_v = IntVector::directionVector((IntDirection)i);
					IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);
				}
			}
			else if(t_ms.direction == directionUp)
			{
				{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_ms.origin.isInnerMap())
						bfsArray.push(n_ms);}
			}
		}
	}
	
	if(is_not_outline_found && !not_outline_found_point.isNull())
	{
		myGD->setJackPoint(not_outline_found_point);
		setPosition(not_outline_found_point.convertToCCP());
	}
	else if(isFinded && !findedPoint.isNull())
	{
		myGD->setJackPoint(findedPoint);
		setPosition(findedPoint.convertToCCP());
	}
	else // escape point not found
	{
		CCLOG("escape point not found!");
		int i = kAchievementCode_hidden_dieEasy;
		
		if(!myAchieve->isCompleted(AchievementCode(i)) && !myAchieve->isAchieve(AchievementCode(i)))
		{
			if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
			   myGD->getCommunication("UI_getUseTime") <= myAchieve->getCondition(AchievementCode(i)))
			{
				myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
				AchieveNoti* t_noti = AchieveNoti::create(AchievementCode(i));
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		
		mySGD->fail_code = kFC_gameover;
		myGD->setIsGameover(true);
		myGD->communication("Main_gameover");
	}
}

//////////////////////////////////////////////////////////////////////////////// move test /////////////////////////////////////////////////////////

void Jack::moveTest()
{
	IntPoint jp = myGD->getJackPoint();
	CCPoint beforePosition = ccp((jp.x-1)*pixelSize+1, (jp.y-1)*pixelSize+1);
	IntVector dv = IntVector::directionVector(direction);
	IntVector s_dv = IntVector::directionVector(secondDirection);
	IntVector c_dv = IntVector::directionVector(no_draw_direction);
	IntVector c_s_dv = IntVector::directionVector(no_draw_secondDirection);
	
	if(jp.isNull())		return;
	
	if(test_speed >= 4.f)
	{
		if(is_double_moving == false)
			is_double_moving = true;
		else
			is_double_moving = false;
	}
	else
		is_double_moving = false;
	
	IntPoint checkPoint;
	IntPoint s_checkPoint;
	
	IntVector s_dv_reverse;
	IntPoint s_checkPoint_reverse;
	
	if(direction == directionLeftDown)
	{
		if(before_x_direction != directionLeft && before_x_direction != directionDown)
		{
			dv = IntVector::directionVector(secondDirection);
			before_x_direction = secondDirection;
		}
		else	dv = IntVector::directionVector(before_x_direction);
		
		if(before_x_direction == secondDirection)
		{
			if(before_x_direction == directionLeft)
			{
				s_dv = IntVector::directionVector(directionDown);
				s_dv_reverse = IntVector::directionVector(directionUp);
			}
			else
			{
				s_dv = IntVector::directionVector(directionLeft);
				s_dv_reverse = IntVector::directionVector(directionRight);
			}
		}
		else	s_dv_reverse = IntVector::reverseDirectionVector(secondDirection);
	}
	else if(direction == directionRightDown)
	{
		if(before_x_direction != directionRight && before_x_direction != directionDown)
		{
			dv = IntVector::directionVector(secondDirection);
			before_x_direction = secondDirection;
		}
		else	dv = IntVector::directionVector(before_x_direction);
		
		if(before_x_direction == secondDirection)
		{
			if(before_x_direction == directionRight)
			{
				s_dv = IntVector::directionVector(directionDown);
				s_dv_reverse = IntVector::directionVector(directionUp);
			}
			else
			{
				s_dv = IntVector::directionVector(directionRight);
				s_dv_reverse = IntVector::directionVector(directionLeft);
			}
		}
		else	s_dv_reverse = IntVector::reverseDirectionVector(secondDirection);
	}
	else if(direction == directionRightUp)
	{
		if(before_x_direction != directionRight && before_x_direction != directionUp)
		{
			dv = IntVector::directionVector(secondDirection);
			before_x_direction = secondDirection;
		}
		else	dv = IntVector::directionVector(before_x_direction);
		
		if(before_x_direction == secondDirection)
		{
			if(before_x_direction == directionRight)
			{
				s_dv = IntVector::directionVector(directionUp);
				s_dv_reverse = IntVector::directionVector(directionDown);
			}
			else
			{
				s_dv = IntVector::directionVector(directionRight);
				s_dv_reverse = IntVector::directionVector(directionLeft);
			}
		}
		else	s_dv_reverse = IntVector::reverseDirectionVector(secondDirection);
	}
	else if(direction == directionLeftUp)
	{
		if(before_x_direction != directionLeft && before_x_direction != directionUp)
		{
			dv = IntVector::directionVector(secondDirection);
			before_x_direction = secondDirection;
		}
		else	dv = IntVector::directionVector(before_x_direction);
		
		if(before_x_direction == secondDirection)
		{
			if(before_x_direction == directionLeft)
			{
				s_dv = IntVector::directionVector(directionUp);
				s_dv_reverse = IntVector::directionVector(directionDown);
			}
			else
			{
				s_dv = IntVector::directionVector(directionLeft);
				s_dv_reverse = IntVector::directionVector(directionRight);
			}
		}
		else	s_dv_reverse = IntVector::reverseDirectionVector(secondDirection);
	}
	else
		s_dv_reverse = IntVector::reverseDirectionVector(secondDirection);
		
	checkPoint = IntPoint(jp.x+dv.dx, jp.y+dv.dy);
	s_checkPoint = IntPoint(jp.x+s_dv.dx, jp.y+s_dv.dy);
	s_checkPoint_reverse = IntPoint(jp.x+s_dv_reverse.dx, jp.y+s_dv_reverse.dy);
	
	
	IntPoint c_checkPoint = IntPoint(jp.x+c_dv.dx, jp.y+c_dv.dy);
	IntPoint c_s_checkPoint = IntPoint(jp.x+c_s_dv.dx, jp.y+c_s_dv.dy);
	
	float t_speed = test_speed > 2.f ? 2.f : test_speed;
	
	if(myState == jackStateNormal)
	{
		// main direction moving
		if(c_dv.dx == 0 && c_dv.dy == 0)
			stopMove();
		else if(c_checkPoint.isInnerMap() && myGD->mapState[c_checkPoint.x][c_checkPoint.y] == mapOldline) // moving
		{
			afterPoint = IntPoint(c_checkPoint.x, c_checkPoint.y);
			
			CCPoint t_ap = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
			if(sqrtf(powf(t_ap.x-getPositionX(), 2.f)+powf(t_ap.y-getPositionY(), 2.f)) > 5.f)
			{
				CCLOG("line %d, gPx %.1f, gPy %.1f, aPx %.1f, aPy %.1f", __LINE__, getPositionX(), getPositionY(), t_ap.x, t_ap.y);
			}
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*c_dv.dx,t_speed*c_dv.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			setPosition(turnPosition);
			
//			if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//			{
				IntDirection t_direction = c_dv.getDirection();
				
				if(t_direction == directionLeft)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move_left"))
						jack_ccb_manager->runAnimationsForSequenceNamed("move_left");
				}
				else if(t_direction == directionRight)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move_right"))
						jack_ccb_manager->runAnimationsForSequenceNamed("move_right");
				}
				else if(t_direction == directionUp)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move_up"))
						jack_ccb_manager->runAnimationsForSequenceNamed("move_up");
				}
				else if(t_direction == directionDown)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move_down"))
						jack_ccb_manager->runAnimationsForSequenceNamed("move_down");
				}
				
				jack_img_direction = t_direction;
//			}
//			else
//			{
//				if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move"))
//					jack_ccb_manager->runAnimationsForSequenceNamed("move");
//				
//				if(c_dv.dx == -1 && jack_img_direction == directionRight)
//				{
//					jackImg->setScaleX(-1.f);
//					jack_img_direction = directionLeft;
//				}
//				else if(c_dv.dx == 1 && jack_img_direction == directionLeft)
//				{
//					jackImg->setScaleX(1.f);
//					jack_img_direction = directionRight;
//				}
//			}
		}
		// main direction drawing
		else if(c_checkPoint.isInnerMap() && myGD->mapState[c_checkPoint.x][c_checkPoint.y] == mapEmpty && isDrawingOn) // main drawing start
		{
			myGD->communication("CP_onJackDrawLine");
			// path add
			if(is_end_turn)
			{
				is_end_turn = false;
				IntPointVector t_pv = IntPointVector(jp.x, jp.y, c_dv.dx, c_dv.dy);
				myGD->communication("PM_addPath", t_pv);
			}
			
			// jack drawing
			setJackState(jackStateDrawing);
			afterPoint = IntPoint(c_checkPoint.x, c_checkPoint.y);
			
			CCPoint t_ap = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
			if(sqrtf(powf(t_ap.x-getPositionX(), 2.f)+powf(t_ap.y-getPositionY(), 2.f)) > 5.f)
			{
				CCLOG("line %d, gPx %.1f, gPy %.1f, aPx %.1f, aPy %.1f", __LINE__, getPositionX(), getPositionY(), t_ap.x, t_ap.y);
			}
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*c_dv.dx,t_speed*c_dv.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			setPosition(turnPosition);
//			if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//			{
				IntDirection t_direction = c_dv.getDirection();
				
				if(t_direction == directionLeft)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_left"))
						jack_ccb_manager->runAnimationsForSequenceNamed("draw_left");
				}
				else if(t_direction == directionRight)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_right"))
						jack_ccb_manager->runAnimationsForSequenceNamed("draw_right");
				}
				else if(t_direction == directionUp)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_up"))
						jack_ccb_manager->runAnimationsForSequenceNamed("draw_up");
				}
				else if(t_direction == directionDown)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_down"))
						jack_ccb_manager->runAnimationsForSequenceNamed("draw_down");
				}
				
				jack_img_direction = t_direction;
//			}
//			else
//			{
//				if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw"))
//					jack_ccb_manager->runAnimationsForSequenceNamed("draw");
//				
//				if(c_dv.dx == -1 && jack_img_direction == directionRight)
//				{
//					jackImg->setScaleX(-1.f);
//					jack_img_direction = directionLeft;
//				}
//				else if(c_dv.dx == 1 && jack_img_direction == directionLeft)
//				{
//					jackImg->setScaleX(1.f);
//					jack_img_direction = directionRight;
//				}
//			}
		}
		else if(c_s_dv.dx == 0 && c_s_dv.dy == 0)
			stopMove();
		else if(c_checkPoint.isInnerMap() && (myGD->mapState[c_checkPoint.x][c_checkPoint.y] == mapEmpty || myGD->mapState[c_checkPoint.x][c_checkPoint.y] == mapOldget) &&
				c_s_checkPoint.isInnerMap() && myGD->mapState[c_s_checkPoint.x][c_s_checkPoint.y] == mapOldline)
		{
			afterPoint = IntPoint(c_s_checkPoint.x, c_s_checkPoint.y);
			
			CCPoint t_ap = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
			if(sqrtf(powf(t_ap.x-getPositionX(), 2.f)+powf(t_ap.y-getPositionY(), 2.f)) > 5.f)
			{
				CCLOG("line %d, gPx %.1f, gPy %.1f, aPx %.1f, aPy %.1f", __LINE__, getPositionX(), getPositionY(), t_ap.x, t_ap.y);
			}
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*c_s_dv.dx,t_speed*c_s_dv.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			setPosition(turnPosition);
			
//			if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//			{
				IntDirection t_direction = c_s_dv.getDirection();
				
				if(t_direction == directionLeft)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move_left"))
						jack_ccb_manager->runAnimationsForSequenceNamed("move_left");
				}
				else if(t_direction == directionRight)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move_right"))
						jack_ccb_manager->runAnimationsForSequenceNamed("move_right");
				}
				else if(t_direction == directionUp)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move_up"))
						jack_ccb_manager->runAnimationsForSequenceNamed("move_up");
				}
				else if(t_direction == directionDown)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move_down"))
						jack_ccb_manager->runAnimationsForSequenceNamed("move_down");
				}
				
				jack_img_direction = t_direction;
//			}
//			else
//			{
//				if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move"))
//					jack_ccb_manager->runAnimationsForSequenceNamed("move");
//				
//				if(c_s_dv.dx == -1 && jack_img_direction == directionRight)
//				{
//					jackImg->setScaleX(-1.f);
//					jack_img_direction = directionLeft;
//				}
//				else if(c_s_dv.dx == 1 && jack_img_direction == directionLeft)
//				{
//					jackImg->setScaleX(1.f);
//					jack_img_direction = directionRight;
//				}
//			}
		}
		else // don't move
		{
			stopMove();
		}
	}
	else // myState == jackStateDrawing
	{
		// main direction drawing
		if(dv.dx == 0 && dv.dy == 0)
			stopMove();
		else if(checkPoint.isInnerMap() && (myGD->mapState[checkPoint.x][checkPoint.y] == mapOldline ||
									   myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty) && isDrawingOn)	//
		{
			// path add
			if(is_end_turn)
			{
				is_end_turn = false;
				IntPointVector t_pv = IntPointVector(jp.x, jp.y, dv.dx, dv.dy);
				myGD->communication("PM_addPath", t_pv);
			}
			
			// jack drawing
			afterPoint = IntPoint(checkPoint.x, checkPoint.y);
			
			CCPoint t_ap = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
			if(sqrtf(powf(t_ap.x-getPositionX(), 2.f)+powf(t_ap.y-getPositionY(), 2.f)) > 5.f)
			{
				CCLOG("line %d, gPx %.1f, gPy %.1f, aPx %.1f, aPy %.1f", __LINE__, getPositionX(), getPositionY(), t_ap.x, t_ap.y);
			}
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*dv.dx,t_speed*dv.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			setPosition(turnPosition);
			
//			if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//			{
				IntDirection t_direction = dv.getDirection();
				
				if(t_direction == directionLeft)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_left"))
						jack_ccb_manager->runAnimationsForSequenceNamed("draw_left");
				}
				else if(t_direction == directionRight)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_right"))
						jack_ccb_manager->runAnimationsForSequenceNamed("draw_right");
				}
				else if(t_direction == directionUp)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_up"))
						jack_ccb_manager->runAnimationsForSequenceNamed("draw_up");
				}
				else if(t_direction == directionDown)
				{
					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_down"))
						jack_ccb_manager->runAnimationsForSequenceNamed("draw_down");
				}
				
				jack_img_direction = t_direction;
//			}
//			else
//			{
//				if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw"))
//					jack_ccb_manager->runAnimationsForSequenceNamed("draw");
//				
//				if(dv.dx == -1 && jack_img_direction == directionRight)
//				{
//					jackImg->setScaleX(-1.f);
//					jack_img_direction = directionLeft;
//				}
//				else if(dv.dx == 1 && jack_img_direction == directionLeft)
//				{
//					jackImg->setScaleX(1.f);
//					jack_img_direction = directionRight;
//				}
//			}
		}
		else if(!myDSH->getBoolForKey(kDSH_Key_isDisableLineOver))
		{
			if(s_dv.dx == 0 && s_dv.dy == 0)
				stopMove();
			else if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapNewline && isDrawingOn)
			{
				// path add
				if(is_end_turn)
				{
					is_end_turn = false;
					IntPointVector t_pv = IntPointVector(jp.x, jp.y, dv.dx, dv.dy);
					myGD->communication("PM_addPath", t_pv);
				}
				
				// jack drawing
				afterPoint = IntPoint(checkPoint.x, checkPoint.y);
				
				CCPoint t_ap = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
				if(sqrtf(powf(t_ap.x-getPositionX(), 2.f)+powf(t_ap.y-getPositionY(), 2.f)) > 5.f)
				{
					CCLOG("line %d, gPx %.1f, gPy %.1f, aPx %.1f, aPy %.1f", __LINE__, getPositionX(), getPositionY(), t_ap.x, t_ap.y);
				}
				
				CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*dv.dx,t_speed*dv.dy));
				turnPosition = checkOutlineTurnPosition(turnPosition);
				setPosition(turnPosition);
				
//				if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//				{
					IntDirection t_direction = dv.getDirection();
					
					if(t_direction == directionLeft)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_left"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_left");
					}
					else if(t_direction == directionRight)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_right"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_right");
					}
					else if(t_direction == directionUp)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_up"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_up");
					}
					else if(t_direction == directionDown)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_down"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_down");
					}
					
					jack_img_direction = t_direction;
//				}
//				else
//				{
//					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw"))
//						jack_ccb_manager->runAnimationsForSequenceNamed("draw");
//					
//					if(dv.dx == -1 && jack_img_direction == directionRight)
//					{
//						jackImg->setScaleX(-1.f);
//						jack_img_direction = directionLeft;
//					}
//					else if(dv.dx == 1 && jack_img_direction == directionLeft)
//					{
//						jackImg->setScaleX(1.f);
//						jack_img_direction = directionRight;
//					}
//				}
			}
			else
				stopMove();
		}
		else
		{
			if(s_dv.dx == 0 && s_dv.dy == 0)
				stopMove();
			else if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapNewline && isDrawingOn &&
					s_checkPoint.isInnerMap() && (myGD->mapState[s_checkPoint.x][s_checkPoint.y] == mapEmpty || myGD->mapState[s_checkPoint.x][s_checkPoint.y] == mapOldline))
			{
				if(is_end_turn)
				{
					is_end_turn = false;
					IntPointVector t_pv = IntPointVector(jp.x, jp.y, s_dv.dx, s_dv.dy);
					myGD->communication("PM_addPath", t_pv);
				}
				
				// jack drawing
				afterPoint = IntPoint(s_checkPoint.x, s_checkPoint.y);
				
				CCPoint t_ap = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
				if(sqrtf(powf(t_ap.x-getPositionX(), 2.f)+powf(t_ap.y-getPositionY(), 2.f)) > 5.f)
				{
					CCLOG("line %d, gPx %.1f, gPy %.1f, aPx %.1f, aPy %.1f", __LINE__, getPositionX(), getPositionY(), t_ap.x, t_ap.y);
				}
				
				CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*s_dv.dx,t_speed*s_dv.dy));
				turnPosition = checkOutlineTurnPosition(turnPosition);
				setPosition(turnPosition);
				
//				if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//				{
					IntDirection t_direction = s_dv.getDirection();
					
					if(t_direction == directionLeft)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_left"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_left");
					}
					else if(t_direction == directionRight)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_right"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_right");
					}
					else if(t_direction == directionUp)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_up"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_up");
					}
					else if(t_direction == directionDown)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_down"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_down");
					}
					
					jack_img_direction = t_direction;
//				}
//				else
//				{
//					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw"))
//						jack_ccb_manager->runAnimationsForSequenceNamed("draw");
//					
//					if(s_dv.dx == -1 && jack_img_direction == directionRight)
//					{
//						jackImg->setScaleX(-1.f);
//						jack_img_direction = directionLeft;
//					}
//					else if(s_dv.dx == 1 && jack_img_direction == directionLeft)
//					{
//						jackImg->setScaleX(1.f);
//						jack_img_direction = directionRight;
//					}
//				}
			}
			else if(s_dv_reverse.dx == 0 && s_dv_reverse.dy == 0)
				stopMove();
			else if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapNewline && isDrawingOn &&
					s_checkPoint_reverse.isInnerMap() && (myGD->mapState[s_checkPoint_reverse.x][s_checkPoint_reverse.y] == mapEmpty || myGD->mapState[s_checkPoint_reverse.x][s_checkPoint_reverse.y] == mapOldline))
			{
				if(is_end_turn)
				{
					is_end_turn = false;
					IntPointVector t_pv = IntPointVector(jp.x, jp.y, s_dv_reverse.dx, s_dv_reverse.dy);
					myGD->communication("PM_addPath", t_pv);
				}
				
				// jack drawing
				afterPoint = IntPoint(s_checkPoint_reverse.x, s_checkPoint_reverse.y);
				
				CCPoint t_ap = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
				if(sqrtf(powf(t_ap.x-getPositionX(), 2.f)+powf(t_ap.y-getPositionY(), 2.f)) > 5.f)
				{
					CCLOG("line %d, gPx %.1f, gPy %.1f, aPx %.1f, aPy %.1f", __LINE__, getPositionX(), getPositionY(), t_ap.x, t_ap.y);
				}
				
				CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*s_dv_reverse.dx,t_speed*s_dv_reverse.dy));
				turnPosition = checkOutlineTurnPosition(turnPosition);
				setPosition(turnPosition);
				
//				if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
//				{
					IntDirection t_direction = s_dv_reverse.getDirection();
					
					if(t_direction == directionLeft)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_left"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_left");
					}
					else if(t_direction == directionRight)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_right"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_right");
					}
					else if(t_direction == directionUp)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_up"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_up");
					}
					else if(t_direction == directionDown)
					{
						if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_down"))
							jack_ccb_manager->runAnimationsForSequenceNamed("draw_down");
					}
					
					jack_img_direction = t_direction;
//				}
//				else
//				{
//					if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw"))
//						jack_ccb_manager->runAnimationsForSequenceNamed("draw");
//					
//					if(s_dv_reverse.dx == -1 && jack_img_direction == directionRight)
//					{
//						jackImg->setScaleX(-1.f);
//						jack_img_direction = directionLeft;
//					}
//					else if(s_dv_reverse.dx == 1 && jack_img_direction == directionLeft)
//					{
//						jackImg->setScaleX(1.f);
//						jack_img_direction = directionRight;
//					}
//				}
			}
			else // don't move
			{
				stopMove();
			}
		}
	}
	
	float t_distance = sqrtf(powf(beforePosition.x-getPositionX(), 2.f) + powf(beforePosition.y-getPositionY(), 2.f));
	if(t_distance >= 2.f)
	{
		is_end_turn = true;
		check_turn_cnt++;
		
		IntPoint beforePoint = myGD->getJackPoint();

		CCPoint t_ap = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
		if(sqrtf(powf(t_ap.x-getPositionX(), 2.f)+powf(t_ap.y-getPositionY(), 2.f)) > 5.f)
		{
			CCLOG("line %d, gPx %.1f, gPy %.1f, aPx %.1f, aPy %.1f", __LINE__, getPositionX(), getPositionY(), t_ap.x, t_ap.y);
			afterPoint = IntPoint::convertToIntPoint(getPosition());
		}
		myGD->setJackPoint(afterPoint);
		
		if(myGD->mapState[afterPoint.x][afterPoint.y] == mapOldline && myGD->mapState[beforePoint.x][beforePoint.y] == mapNewline) // != mapOldline 
		{
			if(myState == jackStateDrawing)
			{
				setPosition(ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1));
//				isDrawingOn = false;
				afterDirection = directionStop;
				setJackState(jackStateNormal);
				
				// new rect get !!!
				myGD->communication("SW_stopAllSW");
				
				myGD->communication("MS_ingNewlineToRealNewline");
				
				myGD->communication("MS_scanMap");
				myGD->communication("PM_cleanPath");
				
				myGD->communication("UI_addTurnCnt");
				
				escapeJack();
			}
		}
		else if(myGD->mapState[afterPoint.x][afterPoint.y] == mapEmpty)
		{
			if(!myDSH->getBoolForKey(kDSH_Key_isDisableLineOver))
				myGD->communication("PM_checkBeforeNewline", afterPoint);
			myGD->mapState[afterPoint.x][afterPoint.y] = mapNewline;
//			CCLOG("draw after point x : %d, y : %d", afterPoint.x, afterPoint.y);
		}
		else if(!myDSH->getBoolForKey(kDSH_Key_isDisableLineOver) && myGD->mapState[afterPoint.x][afterPoint.y] == mapNewline)
		{
			if(!myDSH->getBoolForKey(kDSH_Key_isDisableLineOver))
				myGD->communication("PM_checkBeforeNewline", afterPoint);
		}
		
		if(afterDirection == directionStop)
		{
//			CCPoint t_ap = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
//			if(sqrtf(powf(t_ap.x-getPositionX(), 2.f)+powf(t_ap.y-getPositionY(), 2.f)) > 5.f)
//			{
//				CCLOG("line %d, gPx %.1f, gPy %.1f, aPx %.1f, aPy %.1f", __LINE__, getPositionX(), getPositionY(), t_ap.x, t_ap.y);
//				afterPoint = IntPoint::convertToIntPoint(getPosition());
//			}
			setPosition(ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1));
//			direction = afterDirection;
			stopMove();
		}
		else
		{
			int check_top_line, check_bottom_line;
			
			if(myGD->game_step == kGS_limited)
			{
				check_top_line = myGD->limited_step_top;
				check_bottom_line = myGD->limited_step_bottom;
			}
			else
			{
				check_top_line = mapHeightInnerEnd-1;
				check_bottom_line = mapHeightInnerBegin;
			}
			
			if(direction == directionLeftDown)
			{
				if(before_x_cnt > 0)
				{
					if(before_x_direction == directionLeft && afterPoint.y > check_bottom_line)		before_x_direction = directionDown;
					else if(before_x_direction == directionDown && afterPoint.x > mapWidthInnerBegin)	before_x_direction = directionLeft;
					
					before_x_cnt = 0;
				}
				else
				{
					if(before_x_direction != directionLeft && before_x_direction != directionDown)
					{
						before_x_direction = secondDirection;
						before_x_cnt = 0;
					}
					else
						before_x_cnt++;
				}
			}
			else if(direction == directionRightDown)
			{
				if(before_x_cnt > 0)
				{
					if(before_x_direction == directionRight && afterPoint.y > check_bottom_line)		before_x_direction = directionDown;
					else if(before_x_direction == directionDown && afterPoint.x < mapWidthInnerEnd-1)	before_x_direction = directionRight;
				
					before_x_cnt = 0;
				}
				else
				{
					if(before_x_direction != directionRight && before_x_direction != directionDown)
					{
						before_x_direction = secondDirection;
						before_x_cnt = 0;
					}
					else
						before_x_cnt++;
				}
			}
			else if(direction == directionRightUp)
			{
				if(before_x_cnt > 0)
				{
					if(before_x_direction == directionRight && afterPoint.y < check_top_line)		before_x_direction = directionUp;
					else if(before_x_direction == directionUp && afterPoint.x < mapWidthInnerEnd-1)				before_x_direction = directionRight;
					
					before_x_cnt = 0;
				}
				else
				{
					if(before_x_direction != directionRight && before_x_direction != directionUp)
					{
						before_x_direction = secondDirection;
						before_x_cnt = 0;
					}
					else
						before_x_cnt++;
				}
			}
			else if(direction == directionLeftUp)
			{
				if(before_x_cnt > 0)
				{
					if(before_x_direction == directionLeft && afterPoint.y < check_top_line)		before_x_direction = directionUp;
					else if(before_x_direction == directionUp && afterPoint.x > mapWidthInnerBegin)		before_x_direction = directionLeft;
					
					before_x_cnt = 0;
				}
				else
				{
					if(before_x_direction != directionLeft && before_x_direction != directionUp)
					{
						before_x_direction = secondDirection;
						before_x_cnt = 0;
					}
					else
						before_x_cnt++;
				}
			}
			
			if(direction != afterDirection)
			{
				check_turn_cnt = 0;
				IntPoint a_jp = afterPoint;
				setPosition(ccp((a_jp.x-1)*pixelSize+1, (a_jp.y-1)*pixelSize+1));
//				CCLOG("change direction x : %d , y : %d , before : %d , after : %d", a_jp.x, a_jp.y, direction, afterDirection);
//				myGD->communication("PM_lastPathRemove");
				IntVector t_vector = IntVector::directionVector(afterDirection);
				IntPointVector t_pv = IntPointVector(afterPoint.x, afterPoint.y, t_vector.dx, t_vector.dy);
				myGD->communication("PM_checkLastAddPath", t_pv);
				before_x_direction = directionStop;
				before_x_cnt = 0;
				direction = afterDirection;
			}
		}
		
		test_speed = after_speed;
	}
	if(willBackTracking)
	{
		//			direction = afterDirection = directionStop;
		stopMove();
		setPosition(ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1));
		if(myGD->mapState[afterPoint.x][afterPoint.y] == mapNewline)
		{
			if(!myDSH->getBoolForKey(kDSH_Key_isDisableLineOver))
			{
				if(myGD->getCommunicationBool("PM_checkRemoveNewline", afterPoint))
					myGD->mapState[afterPoint.x][afterPoint.y] = mapEmpty;
			}
			else
				myGD->mapState[afterPoint.x][afterPoint.y] = mapEmpty;
		}
		setJackState(jackStateBackTracking);
		if(!btPoint.isNull())
		{
			myGD->setJackPoint(btPoint);
			setPosition(ccp((btPoint.x-1)*pixelSize+1, (btPoint.y-1)*pixelSize+1));
		}
		isDrawingOn = myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton);
		if(!isStun)
			myGD->communication("Main_startBackTracking");
		else
			myGD->communication("Main_stunBackTracking");
	}
	
	if(is_double_moving)
	{
		moveTest();
	}
}

Jack* Jack::create()
{
	Jack* myJack = new Jack();
	myJack->myInit();
	myJack->autorelease();
	return myJack;
}

void Jack::setPosition( CCPoint t_sp )
{
//	float t_distance = sqrtf(powf(t_sp.x-getPositionX(), 2.f) + powf(t_sp.y-getPositionY(), 2.f));
//	if(t_distance > 5.f)
//	{
//		CCLOG("what?!");
//	}
	CCNode::setPosition(t_sp);
	myGD->communication("Main_moveGamePosition", t_sp);
	myGD->communication("VS_setMoveGamePosition", t_sp);
}

void Jack::changeDirection( IntDirection t_d, IntDirection t_sd )
{
	if(isReverse)
	{
		t_d = reverseDirection(t_d);
		t_sd = reverseDirection(t_sd);
	}

	IntPoint jp = myGD->getJackPoint();
	if(jp.isNull())
		return;
	if(myGD->mapState[jp.x-1][jp.y] != mapOldline && myGD->mapState[jp.x-1][jp.y] != mapNewline &&
		myGD->mapState[jp.x+1][jp.y] != mapOldline && myGD->mapState[jp.x+1][jp.y] != mapNewline &&
		myGD->mapState[jp.x][jp.y-1] != mapOldline && myGD->mapState[jp.x][jp.y-1] != mapNewline &&
		myGD->mapState[jp.x][jp.y+1] != mapOldline && myGD->mapState[jp.x][jp.y+1] != mapNewline)
	{
		escapeJack();
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

	if(direction == directionStop && t_d != directionStop) // move start
	{
		IntVector dv = IntVector::directionVector(t_d);
		IntVector cv = IntVector::directionVector(c_d);

		if(!isDrawingOn && myGD->mapState[jp.x+cv.dx][jp.y+cv.dy] == mapEmpty)
		{
			IntVector sdv = IntVector::directionVector(c_sd);
			if(!myGD->mapState[jp.x+sdv.dx][jp.y+sdv.dy] == mapOldline)
			{
				return;
			}
			else
			{
				check_turn_cnt = 0;
				direction = t_d;
				no_draw_direction = c_d;
				afterDirection = direction;
				secondDirection = t_sd;
				no_draw_secondDirection = c_sd;
				startMove();
				return;
			}
			//				isDrawingOn = true;
		}
		//			else if(!isDrawingOn && myGD->mapState[jp.x+dv.dx][jp.y+dv.dy] == mapOldget)											// if gesture
		//			{
		//				IntVector sdv = IntVector::directionVector(t_sd);
		//				int loop_cnt = 0;
		//				while(myGD->mapState[jp.x+sdv.dx][jp.y+sdv.dy] == mapOldline && loop_cnt < 3)
		//				{
		//					loop_cnt++;
		//					if(myGD->mapState[jp.x+sdv.dx+dv.dx][jp.y+sdv.dy+dv.dy] == mapOldline)
		//					{
		//						check_turn_cnt = 4;
		//						direction = t_sd;
		//						afterDirection = direction;
		//						secondDirection = t_d;
		//						loop_cnt = 5;
		//						startMove();
		//						break;
		//					}
		//					else
		//					{
		//						IntVector t_sdv = IntVector::directionVector(t_sd);
		//						sdv.dx += t_sdv.dx;
		//						sdv.dy += t_sdv.dy;
		//					}
		//				}
		//				if(loop_cnt < 5)
		//				{
		//					sdv = IntVector::reverseDirectionVector(t_sd);
		//					loop_cnt = 0;
		//					while(myGD->mapState[jp.x+sdv.dx][jp.y+sdv.dy] == mapOldline && loop_cnt < 3)
		//					{
		//						loop_cnt++;
		//						if(myGD->mapState[jp.x+sdv.dx+dv.dx][jp.y+sdv.dy+dv.dy] == mapOldline)
		//						{
		//							check_turn_cnt = 4;
		//							direction = IntVector::getReverseDirection(t_sd);
		//							afterDirection = direction;
		//							secondDirection = t_d;
		//							loop_cnt = 5;
		//							startMove();
		//							break;
		//						}
		//						else
		//						{
		//							IntVector t_sdv = IntVector::reverseDirectionVector(t_sd);
		//							sdv.dx += t_sdv.dx;
		//							sdv.dy += t_sdv.dy;
		//						}
		//					}
		//				}
		//				
		//				return;
		//			}
		//			else if(myGD->mapState[jp.x+dv.dx][jp.y+dv.dy] == mapNewline)
		//			{
		//				check_turn_cnt = 4;
		//				isReverseGesture = true;
		//				direction = t_sd;
		//				afterDirection = t_d;
		//				secondDirection = t_d;
		//				keep_direction = kKeepDirection_empty;
		//				startMove();
		//				return;
		//			}
		check_turn_cnt = 0;
		direction = t_d;
		no_draw_direction = c_d;
		afterDirection = direction;
		secondDirection = t_sd;
		no_draw_secondDirection = c_sd;
		startMove();
	}
	//		else if(t_d == directionStop) // move stop
	//		{
	//			afterDirection = t_d;
	//			secondDirection = t_sd;
	//		}
	else // real change direction
	{
		IntVector dv = IntVector::directionVector(t_d);
		IntVector cv = IntVector::directionVector(c_d);

		if(!isDrawingOn && (myGD->mapState[jp.x+cv.dx][jp.y+cv.dy] == mapEmpty || myGD->mapState[jp.x+cv.dx][jp.y+cv.dy] == mapOldget))
		{
			IntVector sdv = IntVector::directionVector(c_sd);
			if(!myGD->mapState[jp.x+sdv.dx][jp.y+sdv.dy] == mapOldline)
			{
				if(isMoving)
					stopMove();
			}
			else
			{
				check_turn_cnt = 0;
				direction = t_d;
				no_draw_direction = c_d;
				afterDirection = direction;
				secondDirection = t_sd;
				no_draw_secondDirection = c_sd;
				//					startMove();
			}
			return;
		}

		if(t_d == direction)
		{
			check_turn_cnt = 0;
		}

		//			if((t_d != directionStop && t_sd != directionStop) && myGD->mapState[jp.x+dv.dx][jp.y+dv.dy] == mapNewline)
		//			{
		//				isReverseGesture = true;
		//				reverseTurnCnt = 0;
		//				afterDirection = t_sd;
		//				secondDirection = t_d;
		//			}
		//			else
		//			{
		//				if(isReverseGesture)	isReverseGesture = false;
		afterDirection = t_d;
		secondDirection = t_sd;
		no_draw_direction = c_d;
		no_draw_secondDirection = c_sd;
		keep_direction = kKeepDirection_empty;
		//			}
	}
	//		if()
	//		{
	//			direction = directionStop;
	//			afterDirection = directionStop;
	//			secondDirection = directionStop;
	//		}
}

void Jack::backTrackingAtAfterMoving( IntPoint t_p )
{
	if(t_p.isNull())
		return;

	if(isMoving)
	{
		btPoint = t_p;
	}
	else
	{
		myGD->setJackPoint(t_p);
		setPosition(ccp((t_p.x-1)*pixelSize+1, (t_p.y-1)*pixelSize+1));
		if(myState != jackStateBackTracking)
			myGD->communication("Main_startBackTracking");
		setJackState(jackStateBackTracking);
	}
}

void Jack::endBackTracking()
{
	setJackState(jackStateNormal);
	if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("stop"))
	{
		jack_ccb_manager->runAnimationsForSequenceNamed("stop");
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
			jack_img_direction = directionStop;
	}

	afterDirection = directionStop;
	secondDirection = directionStop;
	keep_direction = kKeepDirection_empty;
	//		isReverseGesture = false;

	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(myGD->mapState[i][j] == mapNewline)
			{
				if(!myDSH->getBoolForKey(kDSH_Key_isDisableLineOver))
				{
					if(myGD->getCommunicationBool("PM_checkRemoveNewline", IntPoint(i,j)))
						myGD->mapState[i][j] = mapEmpty;
				}
				else
					myGD->mapState[i][j] = mapEmpty;
			}
		}
	}
	
	afterPoint = IntPoint::convertToIntPoint(getPosition());
	
	if(myGD->mapState[afterPoint.x][afterPoint.y] != mapOldline)
	{
		IntMoveState searchFirstMoveState = IntMoveState(afterPoint.x, afterPoint.y, directionStop);
		searchAndMoveOldline(searchFirstMoveState);
	}
}

void Jack::changeSpeed( float t_s )
{
	if(t_s > 4.f)			t_s = 4.f;
	else if(t_s > 2.f)		t_s = 2.f;
	else if(t_s < 0.8f)		t_s = 0.8f;

	after_speed = t_s;
	if(test_speed < after_speed)
	{
		speed_change_img = SpeedChangeEffect::create(true);
		addChild(speed_change_img, kJackZ_stunEffect);
		speed_change_img->startAction();
	}
	else if(test_speed > after_speed)
	{
		speed_change_img = SpeedChangeEffect::create(false);
		addChild(speed_change_img, kJackZ_stunEffect);
		speed_change_img->startAction();
	}
}

void Jack::createHammer()
{
	if(!t_se)
	{
		t_se = StunHammer::create(this, callfunc_selector(Jack::stunJack));
		addChild(t_se, kJackZ_stunEffect);
		((StunHammer*)t_se)->startAction();
	}
	else
	{
		((StunHammer*)t_se)->showHammer();
	}
}

void Jack::createFog()
{
	if(!t_se)
	{
		t_se = IceFog::create(this, callfunc_selector(Jack::stunJack));
		addChild(t_se, kJackZ_stunEffect);
		((IceFog*)t_se)->startAction();
	}
	else
	{
		((IceFog*)t_se)->showFog();
	}
}

void Jack::createSleep()
{
	if(!t_se)
	{
		t_se = Sleep::create(this, callfunc_selector(Jack::stunJack));
		addChild(t_se, kJackZ_stunEffect);
		((Sleep*)t_se)->startAction();
	}
	else
	{
		((Sleep*)t_se)->showCircle();
	}
}

void Jack::createChaos()
{
	if(!t_chaos)
	{
		t_chaos = Chaos::create(this, callfunc_selector(Jack::reverseOn));
		addChild(t_chaos, kJackZ_stunEffect);
		t_chaos->startAction();
	}
	else
	{
		t_chaos->showCircle();
	}
}

void Jack::reverseOn()
{
	isReverse = true;
}

void Jack::reverseOff()
{
	isReverse = false;
	if(t_chaos)
	{
		t_chaos->removeFromParentAndCleanup(true);
		t_chaos = NULL;
	}
}

void Jack::stunJack()
{
	myGD->communication("Main_touchEnd");
	if(isDrawingOn)
		isDrawingOn = myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton);
}

IntDirection Jack::getDirection()
{
	return direction;
}

IntDirection Jack::getSecondDirection()
{
	return secondDirection;
}

jackState Jack::getJackState()
{
	return myState;
}

void Jack::stopMove()
{
	if(getJackState() == jackStateNormal)
	{
		if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("stop"))
		{
			jack_ccb_manager->runAnimationsForSequenceNamed("stop");
//			if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
				jack_img_direction = directionStop;
		}
	}
	else if(getJackState() == jackStateDrawing)
	{
		if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_stop"))
		{
			jack_ccb_manager->runAnimationsForSequenceNamed("draw_stop");
//			if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
				jack_img_direction = directionStop;
		}
	}
	
	direction = directionStop;
	afterDirection = directionStop;
	secondDirection = directionStop;
	before_x_direction = directionStop;
	before_x_cnt = 0;
	isMoving = false;
	unschedule(schedule_selector(Jack::moveTest));
	escapeJack();
}

void Jack::stopJack()
{
	unschedule(schedule_selector(Jack::moveTest));
}

void Jack::startDieEffect( int die_type ) /* after coding */
{
	//		return;
	if(!isDie && !myGD->getJackIsUnbeatable() && !myGD->getIsGameover())
	{
		myGD->communication("Main_checkHideStartMapGacha");
		
		AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_die%d.mp3", rand()%3+1)->getCString(), false, true);
		myGD->toFun();
		myGD->communication("UI_writeDie");
		
		myGD->communication("CP_onJackDie");
		
		if(die_type == DieType::kDieType_other)
		{
			myLog->addLog(kLOG_die_other, myGD->getCommunication("UI_getUseTime"));
			
//			if(!myDSH->getBoolForKey(kDSH_Key_wasTutorialPopupCrashArea))
//			{
//				myDSH->setBoolForKey(kDSH_Key_wasTutorialPopupCrashArea, true);
//				CCNode* exit_target = getParent()->getParent();
//				exit_target->onExit();
//				
//				ASPopupView* t_popup = ASPopupView::create(-200);
//				
//				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//				if(screen_scale_x < 1.f)
//					screen_scale_x = 1.f;
//				
//				t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
//				t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
//				t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
//				
//				CCNode* t_container = CCNode::create();
//				t_popup->setContainerNode(t_container);
//				exit_target->getParent()->addChild(t_popup);
//				
////				CCScale9Sprite* case_back = CCScale9Sprite::create("popup3_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
////				case_back->setPosition(CCPointZero);
////				t_container->addChild(case_back);
////				
////				case_back->setContentSize(CCSizeMake(348, 245));
//				
//				CCSprite* content_back = CCSprite::create("tutorial_popup3.png");
//				content_back->setPosition(ccp(0,0));
//				t_container->addChild(content_back);
//				
//				KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_dieTutorial3), mySGD->getFont().c_str(), 12.5f);
//				content_label->setHorizontalAlignment(kCCTextAlignmentLeft);
//				content_label->setAnchorPoint(ccp(0,0.5));
//				content_label->setPosition(ccp(60,35));
//				content_back->addChild(content_label);
//				
////				CCSprite* title_img = CCSprite::create("tutorial_popup_title.png");
////				title_img->setPosition(ccp(0, 102));
////				t_container->addChild(title_img);
//				
////				CCLabelTTF* content_label = CCLabelTTF::create("몬스터가 쏘는 미사일중에는\n획득영역을 지우는 것도 있어요.", mySGD->getFont().c_str(), 11);
////				content_label->setPosition(ccp(12,-65));
////				t_container->addChild(content_label);
//				
//				CCSprite* n_close = CCSprite::create("whitePaper.png");
//				n_close->setOpacity(0);
//				CCSprite* s_close = CCSprite::create("whitePaper.png");
//				s_close->setOpacity(0);
//				
//				CCMenuLambda* close_menu = CCMenuLambda::create();
//				
//				close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
//				close_menu->setPosition(ccp(240, myDSH->ui_center_y));
//				close_menu->setVisible(false);
//				t_popup->addChild(close_menu);
//				
//				CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
//																					{
//																						close_menu->setVisible(false);
//																						
//																						t_container->addChild(KSTimer::create(0.2f, [=](){
//																							exit_target->onEnter();
//																							((Maingame*)exit_target)->controlStunOff();
//																							t_popup->removeFromParent();
//																						}));
//																						
//																						t_container->addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
//																							t_container->addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.f);}));}));
//																						
//																						t_container->addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(t_container, t);}, [=](int t){KS::setOpacity(t_container, 0);}));
//																						
//																						
//																					});
//				
//				close_menu->addChild(close_item);
//				
//				
//				
//				t_container->setScaleY(0.f);
//				
//				t_container->addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
//					t_container->addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.8f);
//						t_container->addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.f);
//							close_menu->setVisible(true);}));}));}));
//				
//				t_container->addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(t_container, t);}, [=](int t){KS::setOpacity(t_container, 255);}));
//			}
		}
		else if(die_type == DieType::kDieType_missileToLine)
		{
			myLog->addLog(kLOG_die_missileToLine, myGD->getCommunication("UI_getUseTime"));
			
			if(!myDSH->getBoolForKey(kDSH_Key_wasTutorialPopupMissileTrace))
			{
				myDSH->setBoolForKey(kDSH_Key_wasTutorialPopupMissileTrace, true);
				CCNode* exit_target = getParent()->getParent();
				exit_target->onExit();
				
				ASPopupView* t_popup = ASPopupView::create(-200);
				
				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
				if(screen_scale_x < 1.f)
					screen_scale_x = 1.f;
				
				t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
				t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
				t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
				
				CCNode* t_container = CCNode::create();
				t_popup->setContainerNode(t_container);
				exit_target->getParent()->addChild(t_popup);

				
				CCSprite* content_back = CCSprite::create("tutorial_popup1.png");
				content_back->setPosition(ccp(0,0));
				t_container->addChild(content_back);
				
				KSLabelTTF* warning_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_warningDie), mySGD->getFont().c_str(), 15.f);
				warning_label->disableOuterStroke();
				warning_label->setPosition(ccp(52,66));
				content_back->addChild(warning_label);
			
				KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_dieTutorial1), mySGD->getFont().c_str(), 12.5f);
				content_label->setColor(ccc3(20, 50, 70));
				content_label->disableOuterStroke();
				content_label->setAnchorPoint(ccp(0.5f,0.5f));
				content_label->setPosition(ccp(content_back->getContentSize().width/2.f,50));
				content_back->addChild(content_label);
				
				
				CCSprite* n_close = CCSprite::create("whitePaper.png");
				n_close->setOpacity(0);
				CCSprite* s_close = CCSprite::create("whitePaper.png");
				s_close->setOpacity(0);
				
				CCMenuLambda* close_menu = CCMenuLambda::create();
				
				close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
				close_menu->setPosition(ccp(240, myDSH->ui_center_y));
				close_menu->setVisible(false);
				t_popup->addChild(close_menu);
				
				CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
																					{
																						close_menu->setVisible(false);
																						
																						t_container->addChild(KSTimer::create(0.2f, [=](){
																							exit_target->onEnter();
																							((Maingame*)exit_target)->controlStunOff();
																							t_popup->removeFromParent();
																						}));
																						CommonAnimation::closePopup(t_popup, t_container, nullptr, [=](){
																							
																						}, [=](){
//																							end_func(); removeFromParent();
																						});
																						
																					});
				
				close_menu->addChild(close_item);
				
				
				
				CommonAnimation::openPopup(t_popup, t_container, nullptr, [=](){
					
				}, [=](){
					close_menu->setVisible(true);
				});
			}
		}
		else if(die_type == DieType::kDieType_shockwave)
		{
			myLog->addLog(kLOG_die_shockwave, myGD->getCommunication("UI_getUseTime"));
			
			if(!myDSH->getBoolForKey(kDSH_Key_wasTutorialPopupShockWave))
			{
				myDSH->setBoolForKey(kDSH_Key_wasTutorialPopupShockWave, true);
				CCNode* exit_target = getParent()->getParent();
				exit_target->onExit();
				
				ASPopupView* t_popup = ASPopupView::create(-200);
				
				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
				if(screen_scale_x < 1.f)
					screen_scale_x = 1.f;
				
				t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
				t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
				t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
				
				CCNode* t_container = CCNode::create();
				t_popup->setContainerNode(t_container);
				exit_target->getParent()->addChild(t_popup);
			
				
				CCSprite* content_back = CCSprite::create("tutorial_popup2.png");
				content_back->setPosition(ccp(0,0));
				t_container->addChild(content_back);
				
				KSLabelTTF* warning_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_warningDie), mySGD->getFont().c_str(), 15.f);
				warning_label->disableOuterStroke();
				warning_label->setPosition(ccp(52,66));
				content_back->addChild(warning_label);
			
				KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_dieTutorial2), mySGD->getFont().c_str(), 12.5f);
				content_label->setColor(ccc3(20, 50, 70));
				content_label->disableOuterStroke();
				content_label->setAnchorPoint(ccp(0.5f,0.5f));
				content_label->setPosition(ccp(content_back->getContentSize().width/2.f,50));
				content_back->addChild(content_label);
				
				
				CCSprite* n_close = CCSprite::create("whitePaper.png");
				n_close->setOpacity(0);
				CCSprite* s_close = CCSprite::create("whitePaper.png");
				s_close->setOpacity(0);
				
				CCMenuLambda* close_menu = CCMenuLambda::create();
				
				close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
				close_menu->setPosition(ccp(240, myDSH->ui_center_y));
				close_menu->setVisible(false);
				t_popup->addChild(close_menu);
				
				CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
																					{
																						close_menu->setVisible(false);
																						
																						t_container->addChild(KSTimer::create(0.2f, [=](){
																							exit_target->onEnter();
																							((Maingame*)exit_target)->controlStunOff();
																							t_popup->removeFromParent();
																						}));

																						CommonAnimation::closePopup(t_container, t_container, nullptr, [=](){
																							
																						}, [=](){
//																							end_func(); removeFromParent();
																						});
																						
																					});
				
				close_menu->addChild(close_item);
				
				
				CommonAnimation::openPopup(t_popup, t_container, nullptr, [=](){
					
				}, [=](){
					close_menu->setVisible(true);
				});
				
			}
		}
		else if(die_type == DieType::kDieType_timeover)
		{
			
		}

		myGD->communication("UI_endFever");
		myGD->communication("UI_stopCombo");

		//			Well512 t_well512;
		//			myGD->setJackPoint(IntPoint(t_well512.GetValue(mapWidthInnerBegin, mapWidthInnerEnd),t_well512.GetValue(mapHeightInnerBegin, mapHeightInnerEnd)));
		//			if(getJackState() == jackStateDrawing)
		//			{
		//				jack_drawing->setVisible(false);
		//			}
		setJackState(jackStateNormal);
//		jack_barrier->setVisible(false);
		isDrawingOn = myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton);

		myGD->removeMapNewline();
		myGD->communication("PM_cleanPath");

		isStun = true;

		myGD->communication("Main_startSpecialAttack");
		//			AudioEngine::sharedInstance()->playEffect("sound_jack_die.mp3", false);
//		AudioEngine::sharedInstance()->playEffect("sound_die_jack.mp3", false);
		isDie = true;
		dieEffectCnt = 0;
		
		if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("die"))
		{
			jack_ccb_manager->runAnimationsForSequenceNamed("die");
//			if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
				jack_img_direction = directionStop;
		}
		
//		jackImg->removeFromParentAndCleanup(true);

		auto t_ccb = KS::loadCCBI<CCSprite*>(this, "fx_cha_die1.ccbi");
//		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//		CCBReader* reader = new CCBReader(nodeLoader);
		die_particle = t_ccb.first;// dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_cha_die1.ccbi",this));
//		reader->getAnimationManager()->setDelegate(this);
		t_ccb.second->setDelegate(this);
		die_manager = t_ccb.second;//reader->getAnimationManager();
		//			jackImg = CCSprite::create("jack_die.png");
		//			jackImg->setScale(0.2f);
		addChild(die_particle, kJackZ_main);
//		reader->release();
		
		KS::setBlendFunc(die_particle, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		
		schedule(schedule_selector(Jack::dieEffect));
	}
}

void Jack::completedAnimationSequenceNamed (char const * name)
{
	string t_name = name;
	if(t_name == "end_die_animation")
	{
		die_particle->removeFromParent();
	}
}

void Jack::showMB()
{
	if(!isDie)
	{
		MissileBarrier* t_mb = MissileBarrier::create();
//		t_mb->setScale(0.8f);
		t_mb->setPosition(ccp(0,9));
		addChild(t_mb, kJackZ_ActiveBarrier);
	}
}

IntDirection Jack::getRecentDirection()
{
	return direction;
}

void Jack::setTouchPointByJoystick( CCPoint t_p, IntDirection t_direction, bool is_touchEnd )
{
	if(!joystickSpr_byJoystick)
	{
		joystickSpr_byJoystick = CCSprite::create("control_joystick_small_circle.png");
		addChild(joystickSpr_byJoystick, kJackZ_defaultBarrier);
	}
	if(!touchPointSpr_byJoystick)
	{
		touchPointSpr_byJoystick = CCSprite::create("control_joystick_small_ball.png");
		addChild(touchPointSpr_byJoystick, kJackZ_defaultBarrier);
	}
	if(!directionSpr_byJoystick)
	{
		directionSpr_byJoystick = CCSprite::create("control_joystick_arrow.png");
		addChild(directionSpr_byJoystick, kJackZ_defaultBarrier);
	}

	if(is_touchEnd || t_direction == directionStop)
	{
		touchPointSpr_byJoystick->setVisible(false);
		directionSpr_byJoystick->setVisible(false);
		joystickSpr_byJoystick->setVisible(false);
		return;
	}
	else
	{
		touchPointSpr_byJoystick->setVisible(false); // true
		directionSpr_byJoystick->setVisible(false); // true
		joystickSpr_byJoystick->setVisible(false); // true
	}

	touchPointSpr_byJoystick->setPosition(ccpMult(t_p, 0.385f));
	if(t_direction == directionLeft)
	{
		directionSpr_byJoystick->setRotation(-90);
		directionSpr_byJoystick->setPosition(ccp(-30,0));
	}
	else if(t_direction == directionDown)
	{
		directionSpr_byJoystick->setRotation(-180);
		directionSpr_byJoystick->setPosition(ccp(0,-30));
	}
	else if(t_direction == directionRight)
	{
		directionSpr_byJoystick->setRotation(90);
		directionSpr_byJoystick->setPosition(ccp(30,0));
	}
	else if(t_direction == directionUp)
	{
		directionSpr_byJoystick->setRotation(0);
		directionSpr_byJoystick->setPosition(ccp(0,30));
	}
	else if(t_direction == directionStop)
	{
		directionSpr_byJoystick->setVisible(false);
	}
    else if(isEnableIrregularDirection)
    {
        if(t_direction == directionLeftUp)
        {
            directionSpr_byJoystick->setRotation(-45);
            directionSpr_byJoystick->setPosition(ccp(30*cos(deg2Rad(135.f)),30*sin(deg2Rad(135.f))));
        }
        else if(t_direction == directionRightUp)
        {
            directionSpr_byJoystick->setRotation(45);
            directionSpr_byJoystick->setPosition(ccp(30*cos(deg2Rad(45.f)),30*sin(deg2Rad(45.f))));
        }
        else if(t_direction == directionRightDown)
        {
            directionSpr_byJoystick->setRotation(135);
            directionSpr_byJoystick->setPosition(ccp(30*cos(deg2Rad(-45.f)),30*sin(deg2Rad(-45.f))));
        }
        else if(t_direction == directionLeftDown)
        {
            directionSpr_byJoystick->setRotation(-135);
            directionSpr_byJoystick->setPosition(ccp(30*cos(deg2Rad(-135.f)),30*sin(deg2Rad(-135.f))));
        }
    }
}

void Jack::takeSpeedUpItem()
{
	if(myGD->jack_base_speed + speed_up_value >= 2.f)
	{
		myGD->communication("Main_takeSpeedUpEffect",  int( round(((2.f-1.1f) - (2.f-(myGD->jack_base_speed + speed_up_value)))/0.1f)) );

		AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_attack%d.mp3", rand()%4+1)->getCString(), false, true);
		
		int weapon_type = mySGD->getSelectedCharacterHistory().characterNo.getV()-1;
		int weapon_level = mySGD->getSelectedCharacterHistory().level.getV();
		
		int weapon_rank = (weapon_level-1)/5 + 1;
		weapon_level = (weapon_level-1)%5 + 1;
		
		myGD->createJackMissileWithStoneFunctor((StoneType)weapon_type, weapon_rank, weapon_level, 1, getPosition(), int(mySGD->getSelectedCharacterHistory().power.getV()*((rand()%21-10+100)/100.f)));
		
//		string missile_code;
//		missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
//		int missile_type = MissileDamageData::getMissileType(missile_code.c_str());
//		
//		//				myGD->communication("Main_goldGettingEffect", jackPosition, int((t_p - t_beforePercentage)/JM_CONDITION*myDSH->getGoldGetRate()));
//		float missile_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
//		
//		myGD->communication("MP_createJackMissile", missile_type, 1, missile_speed, getPosition());
	}
	else
	{
		speed_up_value += 0.1f;
		changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);
		
		myGD->communication("Main_takeSpeedUpEffect", int( round(((2.f-1.1f) - (2.f-(myGD->jack_base_speed + speed_up_value)))/0.1f) ));
	}
}

float Jack::getSpeedUpValue()
{
	return speed_up_value;
}

float Jack::getAlphaSpeed()
{
	return alpha_speed_value;
}

void Jack::setAlphaSpeed( float t_s )
{
	alpha_speed_value = t_s;
	changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);
}

void Jack::initStartPosition( CCPoint t_p )
{
	int base_value = roundf(-t_p.y/((480.f-myGD->boarder_value*2)/(320.f))/2.f); // 중간 괄호 : myGD->game_scale
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_height = roundf(480*screen_size.height/screen_size.width/2.f);

	IntPoint checking_point = IntPoint(80,base_value+roundf(screen_height/((480.f-myGD->boarder_value*2)/(320.f))/2.f)); // 중간 괄호 : myGD->game_scale

	int map_end_check_cnt = 0;
	bool is_found = false;
	for(int i=0;!is_found && map_end_check_cnt < 2;i++)
	{
		if(i%2 == 0)
			checking_point.x -= i;
		else
			checking_point.x += i;

		if(!checking_point.isInnerMap())
		{
			map_end_check_cnt++;
			continue;
		}

		if(myGD->mapState[checking_point.x][checking_point.y] == mapOldline)
		{
			is_found = true;
			myGD->setJackPoint(checking_point);
			afterPoint = checking_point;
			
			CCNode::setPosition(checking_point.convertToCCP());
			break;
		}
	}

	if(!is_found)
	{
		CCLOG("faskdhfn;asjbfv;kjqdhbf;kvuhqasdk;cn");
	}
}

void Jack::setJackState( jackState t_s )
{
	bool is_changed = t_s != myState;
	
	myState = t_s;
	myGD->setJackState(myState);
	if(myState == jackStateNormal)
	{
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() != 2)
//		{
//			if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move"))
//				jack_ccb_manager->runAnimationsForSequenceNamed("move");
//		}
//		else
//		{
			if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("move_down"))
				jack_ccb_manager->runAnimationsForSequenceNamed("move_down");
//		}
//		jackImg->setColor(ccWHITE);
//		jackImg->setVisible(true);
		line_edge->setVisible(false);
		myGD->communication("Main_setLineParticle", false);
//		if(!is_hard && !jack_barrier->isVisible())
//			jack_barrier->setVisible(true);
		
		if(is_changed)
			myGD->communication("GIM_removeBeautyStone");
	}
	else if(myState == jackStateDrawing)
	{
		line_edge->setVisible(true);
		myGD->communication("Main_setLineParticle", true);
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() != 2)
//		{
//			if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw"))
//				jack_ccb_manager->runAnimationsForSequenceNamed("draw");
//		}
//		else
//		{
			if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("draw_down"))
				jack_ccb_manager->runAnimationsForSequenceNamed("draw_down");
//		}
//		jackImg->setVisible(false);
//		if(!is_hard && jack_barrier->isVisible())
//			jack_barrier->setVisible(false);
		
		if(is_changed)
			myGD->communication("GIM_showBeautyStone");
	}
	else if(myState == jackStateBackTracking)
	{
		line_edge->setVisible(false);
		myGD->communication("Main_setLineParticle", false);
		if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("rewind"))
		{
			jack_ccb_manager->runAnimationsForSequenceNamed("rewind");
//			if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
				jack_img_direction = directionStop;
		}
//		jackImg->setColor(ccGRAY);
//		if(!is_hard && jack_barrier->isVisible())
//			jack_barrier->setVisible(false);
		
		if(is_changed)
			myGD->communication("GIM_removeBeautyStone");
	}
}

IntDirection Jack::reverseDirection( IntDirection t_d )
{
	IntDirection returnDirection;
	if(t_d == directionLeftUp)					returnDirection = directionRightDown;
	else if(t_d == directionLeft)				returnDirection = directionRight;
	else if(t_d == directionLeftDown)			returnDirection = directionRightUp;
	else if(t_d == directionDown)				returnDirection = directionUp;
	else if(t_d == directionRightDown)			returnDirection = directionLeftUp;
	else if(t_d == directionRight)				returnDirection = directionLeft;
	else if(t_d == directionRightUp)			returnDirection = directionLeftDown;
	else if(t_d == directionUp)					returnDirection = directionDown;
	else										returnDirection = directionStop;

	return returnDirection;
}

void Jack::patternRemover()
{
	
	bool canceled = false;
	for(auto cumber : myGD->getMainCumberVector())
	{
		for(auto i : cumber->getCharges())
		{
			i->cancelCharge();
			canceled = true;
		}
		cumber->setCumberState(kCumberStateMoving);
		cumber->m_furyMode.furyFrameCount = cumber->m_furyMode.totalFrame;
	}
	if(canceled)
	{
		myGD->communication("Main_hideScreenSideWarning"); // 화면에 빨간 테두리 지우는 함수
	}
	for(int i=0; i<myGD->getMissileParent()->getPatternContainer()->getChildrenCount(); ++i)
	{
		AttackPattern* temp = (AttackPattern*)myGD->getMissileParent()->getPatternContainer()->getChildren()->objectAtIndex(i);
		if(temp)
		{
			if(dynamic_cast<CobWeb*>(temp) || dynamic_cast<ChaosAttack*>(temp) ||
				 dynamic_cast<FreezeAttack*>(temp))
			{
			}
			else
			{
				temp->onExit();
			}
		}
		else
		{
			CCLOG("hue~");
		}

	}
	
	MissileParent* pattern = myGD->getMissileParent();
	CCNode* patternStencil = pattern->getPatternContainer()->getStencil();
	//				patternStencil->setPosition(getPosition());
	patternStencil->setPosition(ip2ccp(myGD->getJackPoint()));
	
	CCSprite* removerSprite = CCSprite::create("clear_wave.png");
	pattern->addChild(removerSprite, 9999);
	removerSprite->setPosition(ip2ccp(myGD->getJackPoint()));
	
	
	addChild(KSGradualValue<float>::create(0.1f, 2.5f, 0.8f, [=](float t){
		patternStencil->setScale(t);
		removerSprite->setScale(t);
	}, [=](float t){
		patternStencil->setScale(t);
		removerSprite->setScale(t);
		removerSprite->removeFromParent();
		for(int i=0; i<myGD->getMissileParent()->getPatternContainer()->getChildrenCount(); ++i)
		{
			AttackPattern* temp = (AttackPattern*)myGD->getMissileParent()->getPatternContainer()->getChildren()->objectAtIndex(i);
			if(temp)
			{
				temp->stopMyAction();
			}
			else
			{
				CCLOG("hue~");
			}

		}
		for(auto cumber : myGD->getMainCumberVector())
		{
			cumber->setAttackPattern(nullptr);
			cumber->setCumberState(kCumberStateMoving);
			cumber->m_furyMode.furyFrameCount = cumber->m_furyMode.totalFrame;
		}
		
//		addChild(KSTimer::create(0.3f, [=](){
			myGD->removeAllPattern();
//		}));
		
		patternStencil->setScale(0.0001f);
	}));
}
void Jack::dieEffect()
{
	dieEffectCnt++;
	if(dieEffectCnt < 30)
	{
		//			jackImg->setScale(0.2f + dieEffectCnt*0.02f);
		//			jackImg->setOpacity(255-dieEffectCnt*5);
	}
	else if(dieEffectCnt == 30)
	{
		unschedule(schedule_selector(Jack::dieEffect));

		dieEscapeJack();

		if(myGD->getIsGameover())
		{
			jackImg->setVisible(false);
			endGame();
		}
		else
		{
			if(myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep) == kTutorialFlowStep_ingame)
			{
				myGD->communication("UI_addGameTime30Sec");
				speed_up_value = 0.f;
				changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);
				
				patternRemover();
				
				startReviveAnimation(jackImg);
			}
			else if(mySGD->is_hell_mode)
			{
				myGD->communication("UI_hellModeResult");
			}
			else if(myGD->getCommunicationBool("UI_beRevivedJack"))
			{
				speed_up_value = 0.f;
				changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);

//				jackImg->removeFromParentAndCleanup(true);
//
//				CCTexture2D* jack_texture = CCTextureCache::sharedTextureCache()->addImage("jack2.png");
//
//				jackImg = CCSprite::createWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
//				jackImg->setScale(0.8f);
//				addChild(jackImg, kJackZ_main);
				
				
				patternRemover();
				startReviveAnimation(jackImg);
			}
			else
			{
//				if(mySGD->is_endless_mode || continue_on_count < 2)
//				{
					continue_on_count = continue_on_count.getV() + 1;
					myGD->communication("UI_showContinuePopup", this, callfunc_selector(Jack::endGame), this, callfunc_selector(Jack::continueGame));
//				}
//				else
//				{
//					jackImg->setVisible(false);
//					endGame();
//				}
			}
		}
	}
	//		else if(dieEffectCnt > 80)
	//		{
	//			unschedule(schedule_selector(Jack::dieEffect));
	//			
	//			if(myGD->getCommunicationBool("UI_beRevivedJack"))
	//			{
	//				speed_up_value = 0.f;
	//				changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);
	//				
	//				isDie = false;
	//				isStun = false;
	//				
	//				dieEscapeJack();
	//				
	//				if(myGD->getIsGameover())
	//					endGame();
	//				else
	//				{
	//					jackImg->removeFromParentAndCleanup(true);
	//					
	//					CCTexture2D* jack_texture = CCTextureCache::sharedTextureCache()->addImage("jack2.png");
	//					
	//					jackImg = CCSprite::createWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
	//					jackImg->setScale(0.8f);
	//					addChild(jackImg, kJackZ_main);
	//					
	//					CCAnimation* jack_animation = CCAnimation::create();
	//					jack_animation->setDelayPerUnit(0.1f);
	//					jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
	//					jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
	//					jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(23, 0, 23, 23));
	//					
	//					CCAnimate* jack_animate = CCAnimate::create(jack_animation);
	//					CCRepeatForever* jack_repeat = CCRepeatForever::create(jack_animate);
	//					jackImg->runAction(jack_repeat);
	//					
	//					setTouchPointByJoystick(CCPointZero, directionStop, true);
	//					setJackState(jackStateNormal);
	//					
	//					myGD->communication("GIM_dieCreateItem");
	//					myGD->communication("Main_resetIsLineDie");
	//					myGD->communication("Main_stopSpecialAttack");
	//				}
	//			}
	//			else
	//			{
	//				myGD->communication("UI_showContinuePopup", this, callfunc_selector(Jack::endGame), this, callfunc_selector(Jack::continueGame));
	//			}
	//		}
}

void Jack::endReviveJack()
{
	isDie = false;
	isStun = false;
	
	myGD->communication("Main_stopBackingCheck");

//	CCTexture2D* jack_texture = CCTextureCache::sharedTextureCache()->addImage("jack2.png");
//
//	CCAnimation* jack_animation = CCAnimation::create();
//	jack_animation->setDelayPerUnit(0.1f);
//	jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
//	jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
//	jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(23, 0, 23, 23));
//
//	CCAnimate* jack_animate = CCAnimate::create(jack_animation);
//	CCRepeatForever* jack_repeat = CCRepeatForever::create(jack_animate);
//	jackImg->runAction(jack_repeat);

	if(myGD->mapState[afterPoint.x][afterPoint.y] != mapOldline)
	{
		IntMoveState searchFirstMoveState = IntMoveState(afterPoint.x, afterPoint.y, directionStop);
		searchAndMoveOldline(searchFirstMoveState);
	}
	
	setTouchPointByJoystick(CCPointZero, directionStop, true);
	setJackState(jackStateNormal);
	if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("stop"))
	{
		jack_ccb_manager->runAnimationsForSequenceNamed("stop");
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
			jack_img_direction = directionStop;
	}
//	jack_barrier->setVisible(true);
	
	myGD->communication("UI_resumeCounting");
	myGD->communication("GIM_dieCreateItem");
	myGD->communication("Main_resetIsLineDie");
	myGD->communication("Main_stopSpecialAttack");
	myGD->communication("CP_onJackRevived");
}

void Jack::continueGame()
{
	speed_up_value = 0.f;
	changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);

//	jackImg->removeFromParentAndCleanup(true);
//
//	CCTexture2D* jack_texture = CCTextureCache::sharedTextureCache()->addImage("jack2.png");
//
//	jackImg = CCSprite::createWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
//	jackImg->setScale(0.8f);
//	addChild(jackImg, kJackZ_main);

	patternRemover();
	startReviveAnimation(jackImg);
}

void Jack::endGame()
{
	int i = kAchievementCode_hidden_dieEasy;
	
	if(!myAchieve->isCompleted(AchievementCode(i)) && !myAchieve->isAchieve(AchievementCode(i)))
	{
		if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
		   myGD->getCommunication("UI_getUseTime") <= myAchieve->getCondition(AchievementCode(i)))
		{
			myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition(AchievementCode(i)));
			AchieveNoti* t_noti = AchieveNoti::create(AchievementCode(i));
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
	}
	
	mySGD->fail_code = kFC_gameover;
	myGD->setIsGameover(true);
	myGD->communication("Main_allStopSchedule");
	myGD->communication("Main_gameover");
}

void Jack::escapeJack()
{
	if(afterPoint.isInnerMap())
	{
		if(myGD->mapState[afterPoint.x-1][afterPoint.y] == mapOldget &&
			myGD->mapState[afterPoint.x+1][afterPoint.y] == mapOldget &&
			myGD->mapState[afterPoint.x][afterPoint.y-1] == mapOldget &&
			myGD->mapState[afterPoint.x][afterPoint.y+1] == mapOldget)
		{
			IntMoveState searchFirstMoveState = IntMoveState(afterPoint.x, afterPoint.y, directionStop);
			searchAndMoveOldline(searchFirstMoveState);
		}
		
		bool is_go_inner = (myGD->mapState[afterPoint.x][afterPoint.y] == mapNewline ||
							myGD->mapState[afterPoint.x-1][afterPoint.y] == mapNewline ||
							myGD->mapState[afterPoint.x+1][afterPoint.y] == mapNewline ||
							myGD->mapState[afterPoint.x][afterPoint.y-1] == mapNewline ||
							myGD->mapState[afterPoint.x][afterPoint.y+1] == mapNewline);
		for(int x = mapWidthInnerBegin+1;x < mapWidthInnerEnd-1 && !is_go_inner;x++)
		{
			if(!(myGD->mapState[x][mapHeightInnerBegin] == mapOldline && myGD->mapState[x][mapHeightInnerBegin+1] == mapOldget))
				is_go_inner = true;
			if(!(myGD->mapState[x][mapHeightInnerEnd-1] == mapOldline && myGD->mapState[x][mapHeightInnerEnd-1-1] == mapOldget))
				is_go_inner = true;
		}
		for(int y = mapHeightInnerBegin+1;y < mapHeightInnerEnd-1 && !is_go_inner;y++)
		{
			if(!(myGD->mapState[mapWidthInnerBegin][y] == mapOldline && myGD->mapState[mapWidthInnerBegin+1][y] == mapOldget))
				is_go_inner = true;
			if(!(myGD->mapState[mapWidthInnerEnd-1][y] == mapOldline && myGD->mapState[mapWidthInnerEnd-1-1][y] == mapOldget))
				is_go_inner = true;
		}
		
		if(!is_go_inner)
		{
			IntMoveState searchFirstMoveState = IntMoveState(afterPoint.x, afterPoint.y, directionStop);
			searchAndMoveOldline(searchFirstMoveState);
		}
	}
}

void Jack::dieEscapeJack()
{
	IntMoveState searchFirstMoveState = IntMoveState(afterPoint.x, afterPoint.y, directionStop);
	searchAndMoveOldline(searchFirstMoveState);
}

void Jack::startMove()
{
	is_end_turn = true;

	isMoving = true;
	moveValue = 0;
	move_loop_cnt = 0;
	moveTest();
	schedule(schedule_selector(Jack::moveTest));
}

void Jack::resetStopEffects()
{
	t_se = NULL;
//	t_chaos = NULL;
}

void Jack::positionRefresh()
{
	setPosition(getPosition());
}

bool Jack::isDieJack()
{
	return isDie;
}

CCNode* Jack::getJack()
{
	return this;
}

int Jack::getContinueOnCount()
{
	return continue_on_count.getV();
}

void Jack::myInit()
{
	continue_on_count = 0;
	before_x_direction = directionStop;
	before_x_cnt = 0;
	keep_direction = kKeepDirection_empty;
	isDrawingOn = myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton);
    isEnableIrregularDirection = myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection);
	//		isReverseGesture = false;
	isReverse = false;
	t_se = NULL;
	t_chaos = NULL;
	isStun = false;
	isDie = false;
	is_double_moving = false;

	myGD->V_F["Jack_changeSpeed"] = std::bind(&Jack::changeSpeed, this, _1);
	myGD->V_I["Jack_startDieEffect"] = std::bind(&Jack::startDieEffect, this, _1);
	myGD->V_V["Jack_createHammer"] = std::bind(&Jack::createHammer, this);
	myGD->V_V["Jack_createFog"] = std::bind(&Jack::createFog, this);
	myGD->V_V["Jack_createSleep"] = std::bind(&Jack::createSleep, this);
	myGD->V_V["Jack_createChaos"] = std::bind(&Jack::createChaos, this);
	myGD->V_V["Jack_reverseOff"] = std::bind(&Jack::reverseOff, this);
	myGD->V_V["Jack_resetStopEffects"] = std::bind(&Jack::resetStopEffects, this);
	myGD->V_V["Jack_showMB"] = std::bind(&Jack::showMB, this);
	myGD->V_V["Jack_takeSpeedUpItem"] = std::bind(&Jack::takeSpeedUpItem, this);
	myGD->F_V["Jack_getAlphaSpeed"] = std::bind(&Jack::getAlphaSpeed, this);
	myGD->V_F["Jack_setAlphaSpeed"] = std::bind(&Jack::setAlphaSpeed, this, _1);
	myGD->F_V["Jack_getSpeedUpValue"] = std::bind(&Jack::getSpeedUpValue, this);
	myGD->V_V["Jack_positionRefresh"] = std::bind(&Jack::positionRefresh, this);
	myGD->B_V["Jack_isDie"] = std::bind(&Jack::isDieJack, this);
	myGD->CCN_V["Jack_getJack"] = std::bind(&Jack::getJack, this);
	myGD->I_V["Jack_getContinueOnCount"] = std::bind(&Jack::getContinueOnCount, this);


	isMoving = false;
	willBackTracking = false;
	btPoint = IntPoint();
	direction = directionStop;
	afterDirection = directionStop;

	//////////////////////////////////////////////////////////// move test ////////////////////////////////
	speed_up_value = 0.f;
	alpha_speed_value = 0.f;
	test_speed = myGD->jack_base_speed + speed_up_value + alpha_speed_value;
	after_speed = test_speed;
	//////////////////////////////////////////////////////////// move test ////////////////////////////////

	myState = jackStateNormal;
	afterState = jackStateNormal;

	string path_color;
	int path_color_code = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_lineColor_i, mySGD->getSelectedCharacterHistory().characterIndex.getV());
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
	
	line_edge = CCSprite::create("jack_drawing_point.png");//("path_edge_" + path_color + ".png").c_str());
	line_edge->setVisible(false);
	line_edge->setScale(0.5f);
	addChild(line_edge, kJackZ_line);
	
	
	auto t_pair = KS::loadCCBIForFullPath<CCSprite*>(this, StageImgLoader::sharedInstance()->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, mySGD->getSelectedCharacterHistory().characterIndex.getV()) + ".ccbi");
	
	jackImg = t_pair.first;
	jack_ccb_manager = t_pair.second;
//	CCTexture2D* jack_texture = CCTextureCache::sharedTextureCache()->addImage("jack2.png");
//
//	jackImg = CCSprite::createWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
//	jackImg->setScale(0.8f);
	addChild(jackImg, kJackZ_main);
	
	if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("stop"))
	{
		jack_ccb_manager->runAnimationsForSequenceNamed("stop");
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
			jack_img_direction = directionStop;
	}
//	if(mySGD->getSelectedCharacterHistory().characterNo.getV() != 2)
//		jack_img_direction = directionRight;

	startShowJackAnimation(jackImg);

//	CCAnimation* jack_animation = CCAnimation::create();
//	jack_animation->setDelayPerUnit(0.1f);
//	jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
//	jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
//	jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(23, 0, 23, 23));
//
//	CCAnimate* jack_animate = CCAnimate::create(jack_animation);
//	CCRepeatForever* jack_repeat = CCRepeatForever::create(jack_animate);
//	jackImg->runAction(jack_repeat);

	is_hard = false;

//	CCSprite* t_texture = CCSprite::create("jack_barrier.png");
//
//	jack_barrier = CCSprite::createWithTexture(t_texture->getTexture(), CCRectMake(100, 0, 25, 25));
//	jack_barrier->setScale(0.8f);
//	addChild(jack_barrier, kJackZ_defaultBarrier);
//	jack_barrier->setOpacity(0);
//
//	CCAnimation* t_animation = CCAnimation::create();
//	t_animation->setDelayPerUnit(0.1);
//	for(int i=0;i<5;i++)
//		t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*25, 0, 25, 25));
//	CCAnimate* t_animate = CCAnimate::create(t_animation);
//	CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
//
//	jack_barrier->runAction(t_repeat);
	
	setScale(1/myGD->game_scale);//NSDS_GD(mySD->getSilType(), kSDS_SI_scale_d)
}

void Jack::setStartPosition()
{
	IntMoveState searchFirstMoveState = IntMoveState(160/2, 215/2, directionStop);
	searchAndMoveOldline(searchFirstMoveState);
}

void Jack::keepDirectionAction( IntPoint jp, IntDirection t_d )
{
	IntVector left_vector = IntVector::directionVector(IntVector::getLeftDirection(t_d));
	IntVector right_vector = IntVector::directionVector(IntVector::getRightDirection(t_d));

	IntPoint left_point = IntPoint(jp.x+left_vector.dx, jp.y+left_vector.dy);
	IntPoint right_point = IntPoint(jp.x+right_vector.dx, jp.y+right_vector.dy);

	if(left_point.isInnerMap() && right_point.isInnerMap() &&
		(((myGD->mapState[left_point.x][left_point.y] == mapOldget || myGD->mapState[left_point.x][left_point.y] == mapOldline) &&
		(myGD->mapState[right_point.x][right_point.y] == mapOldget || myGD->mapState[right_point.x][right_point.y] == mapOldline)) ||
		(myGD->mapState[left_point.x][left_point.y] == mapEmpty && myGD->mapState[right_point.x][right_point.y] == mapEmpty)))
	{

	}
	else
	{
		if(left_point.isInnerMap() && (myGD->mapState[left_point.x][left_point.y] == mapOldget || myGD->mapState[left_point.x][left_point.y] == mapOldline))
			keep_direction = kKeepDirection_left;
		else if(right_point.isInnerMap() && (myGD->mapState[right_point.x][right_point.y] == mapOldget || myGD->mapState[right_point.x][right_point.y] == mapOldline))
			keep_direction = kKeepDirection_right;
	}
}

bool Jack::rotarySelection( IntPoint jp, IntDirection t_d )
{
	//		if(myGD->mapState[jp.x][jp.y] != mapOldline)
	//			return false;

	IntVector left_vector = IntVector::directionVector(IntVector::getLeftDirection(t_d));
	IntVector right_vector = IntVector::directionVector(IntVector::getRightDirection(t_d));
	IntVector direct_vector = IntVector::directionVector(t_d);

	IntPoint left_point = IntPoint(jp.x+left_vector.dx, jp.y+left_vector.dy);
	IntPoint right_point = IntPoint(jp.x+right_vector.dx, jp.y+right_vector.dy);
	IntPoint direct_point = IntPoint(jp.x+direct_vector.dx, jp.y+direct_vector.dy);

	IntPoint return_point = IntPoint(jp.x-direct_vector.dx, jp.y-direct_vector.dy);

	int rotary_cnt = 0;
	bool is_left = false;
	bool is_right = false;
	bool is_direct = false;

	int oldget_cnt = 0;

	if(left_point.isInnerMap() && myGD->mapState[left_point.x][left_point.y] == mapOldline)
	{
		is_left = true;
		rotary_cnt++;
	}
	else if(left_point.isInnerMap() && myGD->mapState[left_point.x][left_point.y] == mapOldget)
	{
		oldget_cnt++;
	}

	if(right_point.isInnerMap() && myGD->mapState[right_point.x][right_point.y] == mapOldline)
	{
		is_right = true;
		rotary_cnt++;
	}
	else if(right_point.isInnerMap() && myGD->mapState[right_point.x][right_point.y] == mapOldget)
	{
		oldget_cnt++;
	}

	if(direct_point.isInnerMap() && myGD->mapState[direct_point.x][direct_point.y] == mapOldline)
	{
		is_direct = true;
		rotary_cnt++;
	}
	else if(direct_point.isInnerMap() && myGD->mapState[direct_point.x][direct_point.y] == mapOldget)
	{
		oldget_cnt++;
	}

	if(return_point.isInnerMap() && myGD->mapState[return_point.x][return_point.y] == mapOldget)
	{
		oldget_cnt++;
	}


	if(rotary_cnt >= 2 && oldget_cnt >= 2)
	{
		return false;
	}

	if(rotary_cnt >= 2 && keep_direction != kKeepDirection_empty)
	{
		if(keep_direction == kKeepDirection_left && is_right)
		{
			check_turn_cnt = 4;
			direction = IntVector::getRightDirection(t_d);
			afterDirection = direction;
		}
		else if(keep_direction == kKeepDirection_right && is_left)
		{
			check_turn_cnt = 4;
			direction = IntVector::getLeftDirection(t_d);
			afterDirection = direction;
		}
		else	return false;

		return true;
	}
	else
		return false;
}

CCPoint Jack::checkOutlineTurnPosition( CCPoint turnPosition )
{
	if(turnPosition.x < (mapWidthInnerBegin-1)*pixelSize+1)			turnPosition.x = (mapWidthInnerBegin-1)*pixelSize+1;
	if(turnPosition.x > (mapWidthInnerEnd-1-1)*pixelSize+1)			turnPosition.x = (mapWidthInnerEnd-1-1)*pixelSize+1;

	if(myGD->game_step == kGS_limited)
	{
		if(turnPosition.y < (myGD->limited_step_bottom-1)*pixelSize+1)	turnPosition.y = (myGD->limited_step_bottom-1)*pixelSize+1;
		if(turnPosition.y > (myGD->limited_step_top-1)*pixelSize+1)		turnPosition.y = (myGD->limited_step_top-1)*pixelSize+1;
	}
	else
	{
		if(turnPosition.y < (mapHeightInnerBegin-1)*pixelSize+1)		turnPosition.y = (mapHeightInnerBegin-1)*pixelSize+1;
		if(turnPosition.y > (mapHeightInnerEnd-1-1)*pixelSize+1)		turnPosition.y = (mapHeightInnerEnd-1-1)*pixelSize+1;
	}
	return turnPosition;
}

void Jack::startReviveAnimation( CCSprite* t_jack_img )
{
	AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_resurrection%d.mp3", rand()%2+1)->getCString(), false, true);
	
	if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("born"))
	{
		jack_ccb_manager->runAnimationsForSequenceNamed("born");
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
			jack_img_direction = directionStop;
	}
//	t_jack_img->setOpacity(0);
//	t_jack_img->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.8f), CCFadeTo::create(0.5f, 255)));

	CCNode* animation_node = CCNode::create();
	animation_node->setPosition(ccp(t_jack_img->getContentSize().width/2.f, t_jack_img->getContentSize().height/2.f));
	t_jack_img->addChild(animation_node);

	startInnerParticle(animation_node);
	CCDelayTime* delay1 = CCDelayTime::create(0.1f);
	CCCallFuncO* call1 = CCCallFuncO::create(this, callfuncO_selector(Jack::startLightSprite), animation_node);
	CCDelayTime* delay2 = CCDelayTime::create(0.2f);
	CCCallFuncO* call2 = CCCallFuncO::create(this, callfuncO_selector(Jack::startOutterParticle), animation_node);
	CCDelayTime* delay3 = CCDelayTime::create(0.5f);
	CCCallFunc* call3 = CCCallFunc::create(this, callfunc_selector(Jack::endReviveJack));
	CCCallFunc* call4 = CCCallFunc::create(animation_node, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::create(delay1, call1, delay2, call2, delay3, call3, call4, NULL);

	animation_node->runAction(t_seq);
}

void Jack::startShowJackAnimation( CCSprite* t_jack_img )
{
	if(jack_ccb_manager->getRunningSequenceName() == NULL || jack_ccb_manager->getRunningSequenceName() != string("born"))
	{
		jack_ccb_manager->runAnimationsForSequenceNamed("born");
//		if(mySGD->getSelectedCharacterHistory().characterNo.getV() == 2)
			jack_img_direction = directionStop;
	}
//	t_jack_img->setOpacity(0);
//	t_jack_img->runAction(CCFadeTo::create(1.3f, 255));

	CCNode* animation_node = CCNode::create();
	animation_node->setPosition(ccp(t_jack_img->getContentSize().width/2.f, t_jack_img->getContentSize().height/2.f));
	t_jack_img->addChild(animation_node);

	startInnerParticle(animation_node);
	CCDelayTime* delay1 = CCDelayTime::create(0.2f);
	CCCallFuncO* call1 = CCCallFuncO::create(this, callfuncO_selector(Jack::startLightSprite), animation_node);
	CCDelayTime* delay2 = CCDelayTime::create(0.4f);
	CCCallFuncO* call2 = CCCallFuncO::create(this, callfuncO_selector(Jack::startOutterParticle), animation_node);
	CCDelayTime* delay3 = CCDelayTime::create(1.f);
	CCCallFunc* call4 = CCCallFunc::create(animation_node, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::create(delay1, call1, delay2, call2, delay3, call4, NULL);

	animation_node->runAction(t_seq);
}

void Jack::startInnerParticle( CCNode* target_node )
{
	CCParticleSystemQuad* inner_particle = CCParticleSystemQuad::createWithTotalParticles(100);
	inner_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle2.png"));
	inner_particle->setEmissionRate(166.67f);
	inner_particle->setAngle(90.f);
	inner_particle->setAngleVar(360.f);
	ccBlendFunc inner_blend_func = {GL_SRC_ALPHA, GL_ONE};
	inner_particle->setBlendFunc(inner_blend_func);
	inner_particle->setDuration(0.5f);
	inner_particle->setEmitterMode(kCCParticleModeRadius);
	inner_particle->setStartColor(ccc4f(1.f, 1.f, 0.55f, 1.f));
	inner_particle->setStartColorVar(ccc4f(0.f, 0.28f, 0.57f, 1.f));
	inner_particle->setEndColor(ccc4f(0.f, 0.f, 0.f, 1.f));
	inner_particle->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
	inner_particle->setStartSize(15.f);
	inner_particle->setStartSizeVar(7.f);
	inner_particle->setEndSize(10.f);
	inner_particle->setEndSizeVar(5.f);
	inner_particle->setRotatePerSecond(0.f);
	inner_particle->setRotatePerSecondVar(0.f);
	inner_particle->setStartRadius(80.f);
	inner_particle->setStartRadiusVar(40.f);
	inner_particle->setEndRadius(15.f);
	inner_particle->setTotalParticles(100);
	inner_particle->setLife(0.6f);
	inner_particle->setLifeVar(0.3f);
	inner_particle->setStartSpin(0.f);
	inner_particle->setStartSpinVar(0.f);
	inner_particle->setEndSpin(0.f);
	inner_particle->setEndSpinVar(0.f);
	inner_particle->setPosition(ccp(0,0));
	inner_particle->setPosVar(CCPointZero);
	inner_particle->setAutoRemoveOnFinish(true);
	target_node->addChild(inner_particle);
}

void Jack::startLightSprite( CCNode* target_node )
{
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	CCSprite* lighter = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_cha_new.ccbi",this));
	CCSprite* lighter = KS::loadCCBI<CCSprite*>(this, "fx_cha_new.ccbi").first;
	lighter->setPosition(ccp(0,0));
	target_node->addChild(lighter);
	KS::setBlendFunc(lighter, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
//	reader->release();
}

void Jack::startOutterParticle( CCNode* target_node )
{
	CCParticleSystemQuad* outter_particle = CCParticleSystemQuad::createWithTotalParticles(100);
	outter_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle2.png"));
	outter_particle->setEmissionRate(166.67f);
	outter_particle->setAngle(90.f);
	outter_particle->setAngleVar(90.f);
	ccBlendFunc outter_blend_func = {GL_SRC_ALPHA, GL_ONE};
	outter_particle->setBlendFunc(outter_blend_func);
	outter_particle->setDuration(0.4f);
	outter_particle->setEmitterMode(kCCParticleModeGravity);
	outter_particle->setStartColor(ccc4f(1.f, 1.f, 0.55f, 1.f));
	outter_particle->setStartColorVar(ccc4f(0.f, 0.28f, 0.57f, 1.f));
	outter_particle->setEndColor(ccc4f(0.f, 0.f, 0.f, 1.f));
	outter_particle->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
	outter_particle->setStartSize(10.f);
	outter_particle->setStartSizeVar(5.f);
	outter_particle->setEndSize(10.f);
	outter_particle->setEndSizeVar(5.f);
	outter_particle->setGravity(ccp(0.f,300.f));
	outter_particle->setRadialAccel(0.f);
	outter_particle->setRadialAccelVar(0.f);
	outter_particle->setSpeed(100.f);
	outter_particle->setSpeedVar(40.f);
	outter_particle->setTangentialAccel(0.f);
	outter_particle->setTangentialAccelVar(0.f);
	outter_particle->setTotalParticles(100);
	outter_particle->setLife(0.4f);
	outter_particle->setLifeVar(0.5f);
	outter_particle->setStartSpin(0.f);
	outter_particle->setStartSpinVar(0.f);
	outter_particle->setEndSpin(0.f);
	outter_particle->setEndSpinVar(0.f);
	outter_particle->setPosition(ccp(0,0));
	outter_particle->setPosVar(ccp(40.f,40.f));
	outter_particle->setAutoRemoveOnFinish(true);
	target_node->addChild(outter_particle);
}

StunHammer* StunHammer::create( CCObject* t_jack, SEL_CallFunc d_stun )
{
	StunHammer* t_sh = new StunHammer();
	t_sh->myInit(t_jack, d_stun);
	t_sh->autorelease();
	return t_sh;
}

void StunHammer::startAction()
{
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1);
	t_animation->addSpriteFrameWithFileName("stun_hammer1.png");
	t_animation->addSpriteFrameWithFileName("stun_hammer1.png");
	t_animation->addSpriteFrameWithFileName("stun_hammer2.png");

	CCAnimate* t_animate = CCAnimate::create(t_animation);

	CCRotateBy* t_rotate = CCRotateBy::create(0.3, -90);

	CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_animate, t_rotate);

	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StunHammer::afterAction));

	CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);

	hammerImg->runAction(t_seq);
}

void StunHammer::showHammer()
{
	CCSprite* t_hammer = CCSprite::create("stun_hammer1.png");
	t_hammer->setAnchorPoint(ccp(1.0,0.5));
	t_hammer->setRotation(90);

	t_hammer->setPosition(ccp(34,20));

	addChild(t_hammer);

	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1);
	t_animation->addSpriteFrameWithFileName("stun_hammer1.png");
	t_animation->addSpriteFrameWithFileName("stun_hammer1.png");
	t_animation->addSpriteFrameWithFileName("stun_hammer2.png");

	CCAnimate* t_animate = CCAnimate::create(t_animation);

	CCRotateBy* t_rotate = CCRotateBy::create(0.3, -90);

	CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_animate, t_rotate);

	CCCallFuncO* t_call = CCCallFuncO::create(this, callfuncO_selector(StunHammer::deleteTempHammer), t_hammer);

	CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);

	t_hammer->runAction(t_seq);
}

void StunHammer::selfRemove()
{
	myGD->communication("Jack_resetStopEffects");
	myGD->communication("Main_touchOn");
	removeFromParentAndCleanup(true);
}

void StunHammer::deleteTempHammer( CCObject* t_hammer )
{
	((CCNode*)t_hammer)->removeFromParentAndCleanup(true);
}

void StunHammer::afterAction()
{
	hammerImg->removeFromParentAndCleanup(true);

	AudioEngine::sharedInstance()->playEffect("sound_stun_hit.mp3",false);
	(target_jack->*delegate_stun)();

	starImg = CCSprite::create("stun_star.png", CCRectMake(0, 0, 20, 13));
	starImg->setPosition(ccp(0,12));

	CCSprite* t_texture = CCSprite::create("stun_star.png");
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1);
	for(int i=0;i<3;i++)
	{
		t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(0, i*13, 20, 13));
	}
	CCAnimate* t_animate = CCAnimate::create(t_animation);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);

	addChild(starImg);

	starImg->runAction(t_repeat);
}

void StunHammer::myInit( CCObject* t_jack, SEL_CallFunc d_stun )
{
	target_jack = t_jack;
	delegate_stun = d_stun;

	hammerImg = CCSprite::create("stun_hammer1.png");
	hammerImg->setAnchorPoint(ccp(1.0,0.5));
	hammerImg->setRotation(90);

	hammerImg->setPosition(ccp(34,20));

	addChild(hammerImg);
}

IceFog* IceFog::create( CCObject* t_jack, SEL_CallFunc d_freeze )
{
	IceFog* t_if = new IceFog();
	t_if->myInit(t_jack, d_freeze);
	t_if->autorelease();
	return t_if;
}

void IceFog::startAction()
{
	CCDelayTime* t_delay = CCDelayTime::create(0.2f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(IceFog::afterAction));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);

	runAction(t_seq);
}

void IceFog::showFog()
{
	fogImg->removeFromParent();
	stopAllActions();
	
	fogImg = KS::loadCCBI<CCSprite*>(this, "fx_freezing_1.ccbi").first;
		KS::setBlendFunc(fogImg, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	addChild(fogImg);
}

void IceFog::selfRemove()
{
	myGD->communication("Jack_resetStopEffects");
	myGD->communication("Main_touchOn");
	fog_manager->runAnimationsForSequenceNamed("stop");
	CCDelayTime* t_delay = CCDelayTime::create(0.3f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
	runAction(t_seq);
//	removeFromParentAndCleanup(true);
}

void IceFog::deleteFog()
{
//	fogImg->removeFromParentAndCleanup(true);
}

void IceFog::afterAction()
{
	AudioEngine::sharedInstance()->playEffect("sound_ice_hold.mp3", false);
	(target_jack->*delegate_freeze)();

//	fogImg->removeFromParentAndCleanup(true);
//	iceImg = CCSprite::create("ice.png");
//	addChild(iceImg);
}

void IceFog::myInit( CCObject* t_jack, SEL_CallFunc d_freeze )
{
	target_jack = t_jack;
	delegate_freeze = d_freeze;

	auto t_ccb = KS::loadCCBI<CCSprite*>(this, "fx_freezing_1.ccbi");
	
	fogImg = t_ccb.first;
	KS::setBlendFunc(fogImg, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	addChild(fogImg);
	
	fog_manager = t_ccb.second;
}

Sleep* Sleep::create( CCObject* t_jack, SEL_CallFunc d_sleep )
{
	Sleep* t_s = new Sleep();
	t_s->myInit(t_jack, d_sleep);
	t_s->autorelease();
	return t_s;
}

void Sleep::startAction()
{
	CCDelayTime* t_delay = CCDelayTime::create(1.0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Sleep::afterAction));

	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);

	runAction(t_seq);
}

void Sleep::showCircle()
{
	CircleCreater* t_cc = CircleCreater::create(ccYELLOW, 12);
	addChild(t_cc);

	CCDelayTime* t_delay = CCDelayTime::create(1.0);
	CCCallFuncO* t_call = CCCallFuncO::create(this, callfuncO_selector(Sleep::deleteCircle), t_cc);

	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);

	runAction(t_seq);
}

void Sleep::selfRemove()
{
	myGD->communication("Jack_resetStopEffects");
	myGD->communication("Main_touchOn");
	removeFromParentAndCleanup(true);
}

void Sleep::deleteCircle( CCObject* t_remove )
{
	((CircleCreater*)t_remove)->stopCreate();
}

void Sleep::afterAction()
{
	AudioEngine::sharedInstance()->playEffect("sound_sleep.mp3", false);
	(target_jack->*delegate_sleep)();

	my_cc->stopCreate();
	// real sleep add
	sleepImg = CCSprite::create("sleep_zzz.png", CCRectMake(0, 0, 30, 25));
	sleepImg->setPosition(ccp(10,18));
	addChild(sleepImg);

	CCSprite* t_texture = CCSprite::create("sleep_zzz.png");
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1);
	for(int i=0;i<5;i++)
	{
		t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*30, 0, 30, 25));
	}

	CCAnimate* t_animate = CCAnimate::create(t_animation);

	CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);

	sleepImg->runAction(t_repeat);
}

void Sleep::myInit( CCObject* t_jack, SEL_CallFunc d_sleep )
{
	target_jack = t_jack;
	delegate_sleep = d_sleep;

	my_cc = CircleCreater::create(ccYELLOW, 12);
	addChild(my_cc);
}

Chaos* Chaos::create( CCObject* t_jack, SEL_CallFunc d_chaos )
{
	Chaos* t_c = new Chaos();
	t_c->myInit(t_jack, d_chaos);
	t_c->autorelease();
	return t_c;
}

void Chaos::startAction()
{
	CCDelayTime* t_delay = CCDelayTime::create(1.0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Chaos::afterAction));

	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);

	runAction(t_seq);
}

void Chaos::showCircle()
{
	CircleCreater* t_cc = CircleCreater::create(ccBLUE, 12);
	addChild(t_cc);

	CCDelayTime* t_delay = CCDelayTime::create(1.0);
	CCCallFuncO* t_call = CCCallFuncO::create(this, callfuncO_selector(Chaos::deleteCircle), t_cc);

	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);

	runAction(t_seq);
}

void Chaos::selfRemove()
{
	myGD->communication("Jack_resetStopEffects");
	removeFromParentAndCleanup(true);
}

void Chaos::deleteCircle( CCObject* t_remove )
{
	((CircleCreater*)t_remove)->stopCreate();
}

void Chaos::afterAction()
{
	(target_jack->*delegate_chaos)();

	my_cc->stopCreate();

	chaosImg = CCSprite::create("chaos.png");
	chaosImg->setAnchorPoint(ccp(0.5,0));
	chaosImg->setRotation(-45);
	addChild(chaosImg);

	CCRotateTo* t_rotate_left = CCRotateTo::create(0.5, 45);
	CCRotateTo* t_rotate_right = CCRotateTo::create(0.5, -45);
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_rotate_left, t_rotate_right);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);

	chaosImg->runAction(t_repeat);
}

void Chaos::myInit( CCObject* t_jack, SEL_CallFunc d_chaos )
{
	target_jack = t_jack;
	delegate_chaos = d_chaos;

	my_cc = CircleCreater::create(ccBLUE, 12);
	addChild(my_cc);
}

MissileBarrier* MissileBarrier::create()
{
	MissileBarrier* t_mb = new MissileBarrier();
	t_mb->myInit();
	t_mb->autorelease();
	return t_mb;
}

void MissileBarrier::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void MissileBarrier::myInit()
{
	AudioEngine::sharedInstance()->playEffect("sound_barrier_pass.mp3",false);
	CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("jack_missile_barrier.png");
	initWithTexture(t_texture, CCRectMake(0, 0, 38, 38));

	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1);
	for(int i=0;i<3;i++)
		t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(38*i, 0, 38, 38));
	CCAnimate* t_animate = CCAnimate::create(t_animation);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MissileBarrier::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
	runAction(t_seq);
}
