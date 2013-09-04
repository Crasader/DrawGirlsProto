//
//  MainCumber.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 20..
//
//

#include "MainCumber.h"


void MainCumber::moving()
{
	noattack_cnt++;
	if(is_attackOn && !is_silenced && rand()%(attack_rate-noattack_cnt) == 0 && !isFuryMode && !ableTickingTimeBomb && !is_moving_shoot && !is_casting)
	{
		noattack_cnt = 0;
		stopMoving();
		
		showEmotion(kEmotionType_joy);
		
		myGD->communication("MP_startFire", getPosition(), false);
		return;
	}
	
	bool isFindedAfterPoint = false, is_map_visited = false;
	CCPoint afterPosition;
	IntPoint afterPoint;
//	int check_loop_cnt = 0;
	bool notEmotion = true;
	
	int before_areacrash_cnt = areacrash_frame_cnt;
	
	randomShuffle();
	int checking_cnt = 0;
	
	while(!isFindedAfterPoint)
	{
		if(!isFuryMode)
		{
//			check_loop_cnt++;
//			if(check_loop_cnt > 1000)
//			{
//				myScale -= 0.1f;
//				if(myScale < minScale)
//					myScale = minScale;
//				
//				CCScaleTo* t_scale = CCScaleTo::create(0.5f, myScale);
//				cumberImg->runAction(t_scale);
//				
//				stopMoving();
//				myGD->communication("CP_startTeleport");
//				return;
//			}
			
			directionAngle += checking_array[checking_cnt++];
			if(directionAngle < 0)			directionAngle += 360;
			else if(directionAngle > 360)	directionAngle -= 360;
				
			///////////////////////////////////////////////////////////
			CCPoint d_p;
			
			d_p.x = 1.f;
			d_p.y = tanf(directionAngle/180.f*M_PI);
			
			if(directionAngle > 90 && directionAngle < 270)
			{
				d_p.x *= -1.f;
				d_p.y *= -1.f;
			}
			
			float length = sqrtf(powf(d_p.x, 2.f) + powf(d_p.y, 2.f));
			
			d_p = ccpMult(d_p, move_speed/length);
			if(is_slowed)
				d_p = ccpMult(d_p, 1.f/2.f);
			
			afterPosition = ccpAdd(getPosition(), d_p);
			afterPoint.x = (afterPosition.x-1)/pixelSize + 1.f;
			afterPoint.y = (afterPosition.y-1)/pixelSize + 1.f;
		
			
			if(afterPosition.x < 20.f*myScale || afterPosition.x > 320.f-20.f*myScale || afterPosition.y < 20.f*myScale || afterPosition.y > 430.f-20.f*myScale)
			{
				if(rand()%2)					directionAngle -= 90;
				else							directionAngle += 90;
				
				if(directionAngle < 0)			directionAngle += 360;
				else if(directionAngle > 360)	directionAngle -= 360;
			}
			else if(afterPoint.isInnerMap())
			{
				float half_distance = 20.f*myScale; // 20.f : radius for base scale 1.f
				float calc_distance;
				IntPoint check_position;
				
				bool is_not_position = false;
				
				// side check (up, down, left, right)
				
				check_position = IntPoint(afterPoint.x-half_distance/2,afterPoint.y); // left
				if(checkingFunc(check_position, is_not_position, notEmotion, before_areacrash_cnt, is_map_visited))
					return;
				
				check_position = IntPoint(afterPoint.x+half_distance/2,afterPoint.y); // right
				if(checkingFunc(check_position, is_not_position, notEmotion, before_areacrash_cnt, is_map_visited))
					return;
				
				check_position = IntPoint(afterPoint.x,afterPoint.y+half_distance/2); // up
				if(checkingFunc(check_position, is_not_position, notEmotion, before_areacrash_cnt, is_map_visited))
					return;
				
				check_position = IntPoint(afterPoint.x,afterPoint.y-half_distance/2); // down
				if(checkingFunc(check_position, is_not_position, notEmotion, before_areacrash_cnt, is_map_visited))
					return;
				
				// inner check
				for(int i=afterPoint.x-half_distance/2;i<=afterPoint.x+half_distance/2 && !is_not_position;i++)
				{
					for(int j=afterPoint.y-half_distance/2;j<=afterPoint.y+half_distance/2 && !is_not_position;j++)
					{
						calc_distance = sqrtf(powf((afterPoint.x - i)*pixelSize,2) + powf((afterPoint.y - j)*pixelSize, 2));
						if(calc_distance < half_distance)
						{
							check_position = IntPoint(i,j);
							if(checkingFunc(check_position, is_not_position, notEmotion, before_areacrash_cnt, is_map_visited))
								return;
							
							
							
//							else if(check_position.isInnerMap() && myGD->mapState[check_position.x][check_position.y] == mapEmpty)
//							{
//								is_not_position = false;
//							}
						}
					}
				}
				if(!is_not_position)
				{
					isFindedAfterPoint = true;
					if(directionAngle <= 90 || directionAngle > 270)
					{
						cumberImg->setFlipX(true);
						//				if(t_chapter_number == 1 && t_stage_number == 1)
						//					((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(true);
						//				else if(t_chapter_number == 11)
						//					((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(true);
						//				else if(t_chapter_number == 16)
						//					my_eye->setFlipX(true);
					}
					else
					{
						cumberImg->setFlipX(false);
						//				if(t_chapter_number == 1 && t_stage_number == 1)
						//					((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(false);
						//				else if(t_chapter_number == 11)
						//					((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(false);
						//				else if(t_chapter_number == 16)
						//					my_eye->setFlipX(false);
					}
				}
			}
			
			if(!isFindedAfterPoint)
			{
				directionAngle -= checking_array[checking_cnt-1];
				if(directionAngle < 0)			directionAngle += 360;
				else if(directionAngle > 360)	directionAngle -= 360;
			}
			
			if(checking_cnt > 360)
			{
				directionAngle -= checking_array[checking_cnt-1];
				if(directionAngle < 0)			directionAngle += 360;
				else if(directionAngle > 360)	directionAngle -= 360;
				stopMoving();
				myGD->communication("CP_startTeleport");
				return;
			}
		}
		else
		{
			CCPoint d_p;
			
			d_p.x = 1.f;
			d_p.y = tanf(dash_angle/180.f*M_PI);
			
			if((dash_angle >= 90 && dash_angle <= 270) || dash_angle <= -90)
			{
				d_p.x *= -1.f;
				d_p.y *= -1.f;
			}
			
			float length = sqrtf(powf(d_p.x, 2.f) + powf(d_p.y, 2.f));
			
			d_p = ccpMult(d_p, move_speed/length);
			if(is_slowed)
				d_p = ccpMult(d_p, 1.f/2.f);
			
			afterPosition = ccpAdd(getPosition(), d_p);
			afterPoint.x = (afterPosition.x-1)/pixelSize + 1.f;
			afterPoint.y = (afterPosition.y-1)/pixelSize + 1.f;
			
			
			if(afterPosition.x < 20.f*myScale || afterPosition.x > 320.f-20.f*myScale || afterPosition.y < 20.f*myScale || afterPosition.y > 430.f-20.f*myScale)
			{
				furyModeOff();
				return;
			}
			else if(!afterPoint.isInnerMap())
			{
				furyModeOff();
				return;
			}
			else
			{
				float half_distance = 20.f*myScale; // 20.f : radius for base scale 1.f
				float calc_distance;
				IntPoint check_position;
				
				bool is_not_position = false;
				
				check_position = IntPoint(afterPoint.x-half_distance/2,afterPoint.y); // left
				if(!check_position.isInnerMap())
					is_not_position = true;
				
				if(!is_not_position)
				{
					check_position = IntPoint(afterPoint.x+half_distance/2,afterPoint.y); // right
					if(!check_position.isInnerMap())
						is_not_position = true;
				}
				
				if(!is_not_position)
				{
					check_position = IntPoint(afterPoint.x,afterPoint.y+half_distance/2); // up
					if(!check_position.isInnerMap())
						is_not_position = true;
				}
					
				if(!is_not_position)
				{
					check_position = IntPoint(afterPoint.x,afterPoint.y-half_distance/2); // down
					if(!check_position.isInnerMap())
						is_not_position = true;
				}
				
				if(!is_not_position)
				{
					for(int i=afterPoint.x-half_distance/2;i<=afterPoint.x+half_distance/2 && !is_not_position;i++)
					{
						for(int j=afterPoint.y-half_distance/2;j<=afterPoint.y+half_distance/2 && !is_not_position;j++)
						{
							calc_distance = sqrtf(powf((afterPoint.x - i)*pixelSize,2) + powf((afterPoint.y - j)*pixelSize, 2));
							if(calc_distance < half_distance)
							{
								check_position = IntPoint(i,j);
								if(!check_position.isInnerMap())
								{
									is_not_position = true;
								}
							}
						}
					}
				}
				if(!is_not_position)
				{
					isFindedAfterPoint = true;
					if(dash_angle <= 90 || dash_angle > 270)
					{
						cumberImg->setFlipX(true);
						//				if(t_chapter_number == 1 && t_stage_number == 1)
						//					((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(true);
						//				else if(t_chapter_number == 11)
						//					((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(true);
						//				else if(t_chapter_number == 16)
						//					my_eye->setFlipX(true);
					}
					else
					{
						cumberImg->setFlipX(false);
						//				if(t_chapter_number == 1 && t_stage_number == 1)
						//					((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(false);
						//				else if(t_chapter_number == 11)
						//					((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(false);
						//				else if(t_chapter_number == 16)
						//					my_eye->setFlipX(false);
					}
				}
				else
				{
					furyModeOff();
					return;
				}
			}
		}
	}
	
	if(isFuryMode)
	{
		CCPoint subPosition = ccpSub(b_c_p, afterPosition);
		subPosition.x = fabsf(subPosition.x);
		subPosition.y = fabsf(subPosition.y);
		
		if(subPosition.x > 8 || subPosition.y > 8)
		{
			b_c_p = afterPosition;
			int t_angle = directionAngle;
			t_angle += 90;
			if(t_angle>180)		t_angle -= 360;
			
			CCPoint dv;
			dv.x = 1;
			dv.y = tanf(directionAngle/180.f*M_PI);
			
			float div_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
			
			dv = ccpMult(dv, 1.f/div_value);
			
			if((directionAngle <= 90 && directionAngle >= -90) || directionAngle >= 270)
				dv = ccpMult(dv, -1.f);
			
			furyMode->setAngle(t_angle);
			furyMode->setGravity(ccpMult(dv, 150.f));
			//		furyMode->setPosition(CCPointZero); // ccpMult(dv, -30)
			
			for(int i=-8;i<=8;i++)
			{
				int addValue;
				if(i == -8 || i == 8)									addValue = 3;
				else if(i == -7 || i == 7)								addValue = 5;
				else if(i == -6 || i == 6)								addValue = 6;
				else if(i == -5 || i == 5 || i == -4 || i == 4)			addValue = 7;
				else													addValue = 8;
				for(int j=-addValue;j<=addValue;j++)
				{
					crashMapForIntPoint(IntPoint(afterPoint.x+i, afterPoint.y+j));
				}
			}
		}
		
		furyFrame--;
		if(furyFrame <= 0)
		{
			furyModeOff();
		}
	}
	
	if(ableTickingTimeBomb)
	{
		tickingFrame--;
		if(tickingFrame <= 0)
		{
			tickingOff();
		}
	}
	
	mapPoint = afterPoint; // unnecessary
	myGD->setMainCumberPoint(afterPoint);
	setPosition(afterPosition);
	
	before_keep_frame_cnt++;
	
	if(before_keep_frame_cnt >= 60)
	{
		if(keep_number >= 100)
		{
			keep_number = 0;
		}
		before_points[keep_number++] = afterPoint;
		before_keep_frame_cnt = 0;
	}
	
	move_frame++;
	
	if(move_frame >= 45)
	{
		float areacrash_rate = 1.f*areacrash_frame_cnt/move_frame;
		
		if(areacrash_rate <= 1.f/60.f)
			myScale += 0.03f;
		else if(areacrash_rate >= 1.f/20.f)
			myScale -= 0.03f;
		else
		{
			if(((areacrash_rate-1.f/60.f) / (1.f/20.f - 1.f/60.f)) + 0.7f > myScale)
				myScale -= 0.02f;
			else
				myScale += 0.02f;
		}
		
		if(myScale > maxScale)
			myScale = maxScale;
		else if(myScale < minScale)
			myScale = minScale;
		
		CCScaleTo* t_scale = CCScaleTo::create(0.5f, myScale);
		cumberImg->runAction(t_scale);
		
		move_frame = 0;
		areacrash_frame_cnt = 0;
	}
}