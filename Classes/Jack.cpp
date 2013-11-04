//
//  Jack.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 29..
//
//

#include "Jack.h"


void Jack::searchAndMoveOldline(IntMoveState searchFirstMoveState)
{
	queue<IntMoveState> bfsArray;
	bfsArray.push(searchFirstMoveState);
	
	bool isFinded = false;
	IntPoint findedPoint;
	
	while(!bfsArray.empty() && !isFinded)
	{
		IntMoveState t_ms = bfsArray.front();
		bfsArray.pop();
		if(myGD->mapState[t_ms.origin.x][t_ms.origin.y] == mapOldline)
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
	
	if(isFinded && !findedPoint.isNull())
	{
		myGD->setJackPoint(findedPoint);
		setPosition(ccp((findedPoint.x-1)*pixelSize+1, (findedPoint.y-1)*pixelSize+1));
	}
	else // escape point not found
	{
		CCLog("escape point not found!");
		endGame();
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
		if(c_checkPoint.isInnerMap() && myGD->mapState[c_checkPoint.x][c_checkPoint.y] == mapOldline) // moving
		{
			afterPoint = IntPoint(c_checkPoint.x, c_checkPoint.y);
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*c_dv.dx,t_speed*c_dv.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			
			setPosition(turnPosition);
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
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*c_dv.dx,t_speed*c_dv.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			
			setPosition(turnPosition);
		}
		else if(c_checkPoint.isInnerMap() && (myGD->mapState[c_checkPoint.x][c_checkPoint.y] == mapEmpty || myGD->mapState[c_checkPoint.x][c_checkPoint.y] == mapOldget) &&
				c_s_checkPoint.isInnerMap() && myGD->mapState[c_s_checkPoint.x][c_s_checkPoint.y] == mapOldline)
		{
			afterPoint = IntPoint(c_s_checkPoint.x, c_s_checkPoint.y);
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*c_s_dv.dx,t_speed*c_s_dv.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			
			setPosition(turnPosition);
		}
		else // don't move
		{
			stopMove();
		}
	}
	else // myState == jackStateDrawing
	{
		// main direction drawing
		if(checkPoint.isInnerMap() && (myGD->mapState[checkPoint.x][checkPoint.y] == mapOldline ||
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
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*dv.dx,t_speed*dv.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			
			setPosition(turnPosition);
		}
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
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*s_dv.dx,t_speed*s_dv.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			
			setPosition(turnPosition);
		}
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
			
			CCPoint turnPosition = ccpAdd(getPosition(), ccp(t_speed*s_dv_reverse.dx,t_speed*s_dv_reverse.dy));
			turnPosition = checkOutlineTurnPosition(turnPosition);
			
			setPosition(turnPosition);
		}
		else // don't move
		{
			stopMove();
		}
	}
	
	float t_distance = sqrtf(powf(beforePosition.x-getPositionX(), 2.f) + powf(beforePosition.y-getPositionY(), 2.f));
	if(t_distance >= 2.f)
	{
		is_end_turn = true;
		check_turn_cnt++;
		
		IntPoint beforePoint = myGD->getJackPoint();
		
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
				
				myGD->communication("MS_scanMap");
				myGD->communication("PM_cleanPath");
				escapeJack();
			}
		}
		else if(myGD->mapState[afterPoint.x][afterPoint.y] == mapEmpty)
		{
			myGD->mapState[afterPoint.x][afterPoint.y] = mapNewline;
		}
		if(afterDirection == directionStop)
		{
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
				IntPoint a_jp = myGD->getJackPoint();
				setPosition(ccp((a_jp.x-1)*pixelSize+1, (a_jp.y-1)*pixelSize+1));
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
			myGD->mapState[afterPoint.x][afterPoint.y] = mapEmpty;
		setJackState(jackStateBackTracking);
		if(!btPoint.isNull())
		{
			myGD->setJackPoint(btPoint);
			setPosition(ccp((btPoint.x-1)*pixelSize+1, (btPoint.y-1)*pixelSize+1));
		}
		isDrawingOn = false;
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