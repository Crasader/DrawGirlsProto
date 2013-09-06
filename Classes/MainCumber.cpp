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

void MainCumber::cumberImgStartRotating(float speed)
{
	CCRotateBy* t_rotate = CCRotateBy::create(0.5, speed);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
	
	cumberImg->runAction(t_repeat);
}

void MainCumber::checkingJackCrash()
{
	float half_distance = 20.f*myScale; // 20.f : radius for base scale 1.f
	float calc_distance;
	IntPoint check_position;
	
	for(int i= mapPoint.x-half_distance/2;i<=mapPoint.x+half_distance/2;i++)
	{
		for(int j=mapPoint.y-half_distance/2;j<=mapPoint.y+half_distance/2;j++)
		{
			calc_distance = sqrtf(powf((mapPoint.x - i)*pixelSize,2) + powf((mapPoint.y - j)*pixelSize, 2));
			if(calc_distance < half_distance)
			{
				check_position = IntPoint(i,j);
				if(check_position.isInnerMap() && myGD->mapState[check_position.x][check_position.y] == mapNewline)
				{
					IntPoint jackPoint = myGD->getJackPoint();
					if(jackPoint.x == check_position.x && jackPoint.y == check_position.y)
					{
						myGD->communication("Jack_startDieEffect");
						return;
					}
					else
					{
						myGD->communication("SW_createSW", check_position, this, callfuncI_selector(MainCumber::showEmotion));
					}
				}
			}
		}
	}
}

void MainCumber::startSpringCumber(float t_springAngle)
{
	if(is_moving_shoot)
	{
		myGD->communication("MP_deleteKeepAP35");
	}
	furyModeOff();
	tickingOff();
	stopMoving();
	stopSpringCumber();
	cumberImg->setColor(ccRED);
	//		if(t_chapter_number == 1 && t_stage_number == 1)		((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setColor(ccRED);
	//		else if(t_chapter_number == 11)							((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setColor(ccRED);
	//		else if(t_chapter_number == 16)							my_eye->setColor(ccRED);
	springCnt = 0;
	springAngle = t_springAngle;
	if(springAngle > -90.f && springAngle <= 90.f)
	{
		cumberImg->setFlipX(false);
		//			if(t_chapter_number == 1 && t_stage_number == 1)
		//				((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(false);
		//			else if(t_chapter_number == 11)
		//				((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(false);
		//			else if(t_chapter_number == 16)
		//				my_eye->setFlipX(false);
	}
	else
	{
		cumberImg->setFlipX(true);
		//			if(t_chapter_number == 1 && t_stage_number == 1)
		//				((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(true);
		//			else if(t_chapter_number == 11)
		//				((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setFlipX(true);
		//			else if(t_chapter_number == 16)
		//				my_eye->setFlipX(true);
	}
	
	myScale -= 0.1f;
	
	if(myScale > maxScale)
		myScale = maxScale;
	else if(myScale < minScale)
		myScale = minScale;
	
	CCScaleTo* t_scale = CCScaleTo::create(0.5f, myScale);
	cumberImg->runAction(t_scale);
	
	springPower = 3;
	schedule(schedule_selector(MainCumber::springCumber));
}

void MainCumber::stopSpringCumber()
{
	cumberImg->setColor(ccWHITE);
	//		if(t_chapter_number == 1 && t_stage_number == 1)		((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setColor(ccWHITE);
	//		else if(t_chapter_number == 11)							((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setColor(ccWHITE);
	//		else if(t_chapter_number == 16)							my_eye->setColor(ccWHITE);
	myGD->setMainCumberPoint(IntPoint((getPositionX()-1)/pixelSize+1,(getPositionY()-1)/pixelSize+1));
	unschedule(schedule_selector(MainCumber::springCumber));
}

void MainCumber::startMoving()
{
	if(!isGameover)
	{
		myState = cumberStateMoving;
		directionAngle = rand()%360;
		schedule(schedule_selector(MainCumber::moving));
	}
}
void MainCumber::stopMoving()
{
	unschedule(schedule_selector(MainCumber::moving));
	myState = cumberStateStop;
}
void MainCumber::setCumberState(int t_cs)
{
	myState = cumberState(t_cs);
}
cumberState MainCumber::getCumberState()
{
	return myState;
}

void MainCumber::setGameover()
{
	isGameover = true;
}

void MainCumber::furyModeOn()
{
	if(!isFuryMode)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		CCPoint subPosition = ccpSub(jackPosition, getPosition());
		
		dash_angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
		
		myGD->communication("EP_startCrashAction");
		b_c_p = getPosition();
		isFuryMode = true;
		
		CCPoint dv;
		dv.x = 1;
		dv.y = tanf(dash_angle/180.f*M_PI);
		
		float div_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
		
		dv = ccpMult(dv, 1.f/div_value);
		
		if((dash_angle >= 90 && dash_angle <= 270) || dash_angle <= -90)
			dv = ccpMult(dv, -1.f);
		
		furyMode = CCParticleSystemQuad::createWithTotalParticles(100);
		furyMode->setPositionType(kCCPositionTypeFree);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("furyMode.png");
		furyMode->setTexture(texture);
		furyMode->setEmissionRate(500.00);
		furyMode->setAngle(dash_angle+90);
		furyMode->setAngleVar(0.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		furyMode->setBlendFunc(blendFunc);
		furyMode->setDuration(-1.00);
		furyMode->setEmitterMode(kCCParticleModeGravity);
		ccColor4F startColor = {1.00,0.98,0.40,1.00};
		furyMode->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0};
		furyMode->setStartColorVar(startColorVar);
		ccColor4F endColor = {1.00,0.06,0.03,0.44};
		furyMode->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		furyMode->setEndColorVar(endColorVar);
		furyMode->setStartSize(20.00);
		furyMode->setStartSizeVar(10.0);
		furyMode->setEndSize(10.0);
		furyMode->setEndSizeVar(10.0);
		furyMode->setGravity(ccpMult(dv, 150.f));
		furyMode->setRadialAccel(0.0);
		furyMode->setRadialAccelVar(0.0);
		furyMode->setSpeed(0);
		furyMode->setSpeedVar(80.0);
		furyMode->setTangentialAccel(0);
		furyMode->setTangentialAccelVar(0);
		furyMode->setTotalParticles(100);
		furyMode->setLife(0.20);
		furyMode->setLifeVar(0.80);
		furyMode->setStartSpin(0.0);
		furyMode->setStartSpinVar(0.0);
		furyMode->setEndSpin(0.0);
		furyMode->setEndSpinVar(0.0);
		furyMode->setPosition(CCPointZero); // ccpMult(dv, -30)
		furyMode->setPosVar(CCPointZero);
		addChild(furyMode);
	}
	furyFrame = 300;
}

void MainCumber::furyModeOff()
{
	if(isFuryMode)
	{
		myGD->communication("EP_stopCrashAction");
		myGD->communication("MS_resetRects");
		isFuryMode = false;
		furyMode->removeFromParentAndCleanup(true);
	}
}

void MainCumber::tickingOn()
{
	if(!ableTickingTimeBomb)
	{
		tickingImg = CCSprite::create("tickingTimeBomb_main.png");
		addChild(tickingImg);
		CCSprite* tickingPad = CCSprite::create("tickingTimeBomb_pad.png");
		tickingPad->setPosition(ccp(17,12));
		tickingImg->addChild(tickingPad);
	}
	
	ableTickingTimeBomb = true;
	tickingFrame = 300;
}

void MainCumber::tickingOff()
{
	if(ableTickingTimeBomb)
	{
		ableTickingTimeBomb = false;
		tickingImg->removeFromParentAndCleanup(true);
	}
}

void MainCumber::startTeleport()
{
	if(teleportImg)
	{
		teleportImg->removeFromParentAndCleanup(true);
		teleportImg = NULL;
	}
	
	teleportImg = CCSprite::create("teleport_light.png");
	teleportImg->setScale(0);
	addChild(teleportImg);
	
	CCScaleTo* t_scale = CCScaleTo::create(0.2, 2.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumber::smaller));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	
	teleportImg->runAction(t_seq);
	AudioEngine::sharedInstance()->playEffect("sound_teleport.mp3",false);
}

void MainCumber::smaller()
{
	CCScaleTo* t_scale = CCScaleTo::create(0.2, 0.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumber::changePosition));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	
	runAction(t_seq);
}

void MainCumber::changePosition()
{
	//		bool changable_map[162][217] = {0,};
	//
	//		queue<IntMoveState> checking_queue;
	//
	//		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	//		{
	//			for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	//			{
	//
	//			}
	//		}
	//
	//
	//
	//
	
	IntPoint afterPoint;
	bool isFound = false;
	for(int i=0;i<before_points.size() && !isFound;i++)
	{
		afterPoint = before_points[i];
		
		if(afterPoint.isInnerMap())
		{
			float half_distance = 20.f*myScale; // 20.f : radius for base scale 1.f
			float calc_distance;
			IntPoint check_position;
			
			bool is_not_position = false;
			
			for(int i=afterPoint.x-half_distance/2;i<=afterPoint.x+half_distance/2 && !is_not_position;i++)
			{
				for(int j=afterPoint.y-half_distance/2;j<=afterPoint.y+half_distance/2 && !is_not_position;j++)
				{
					calc_distance = sqrtf(powf((afterPoint.x - i)*pixelSize,2) + powf((afterPoint.y - j)*pixelSize, 2));
					if(calc_distance < half_distance)
					{
						check_position = IntPoint(i,j);
						if(!check_position.isInnerMap() || myGD->mapState[check_position.x][check_position.y] != mapEmpty)
						{
							is_not_position = true;
						}
					}
				}
			}
			if(!is_not_position)
			{
				isFound = true;
			}
		}
	}
	
	if(!isFound)
	{
		int check_loop_cnt = 0;
		
		while(!isFound)
		{
			check_loop_cnt++;
			if(check_loop_cnt > 100)
			{
				myScale -= 0.05f;
				if(myScale < minScale)
				{
					myScale = minScale;
					vector<IntPoint> searched_points;
					
					float calc_distance;
					float half_distance = 20.f*myScale;
					bool is_not_position;
					
					int searched_cnt = 0;
					int loop_cnt = 0;
					for(int i=mapWidthInnerBegin+10.f*myScale;i<mapWidthInnerEnd-10.f*myScale;i++)
					{
						for(int j=mapHeightInnerBegin+10.f*myScale;j<mapHeightInnerEnd-10.f*myScale;j++)
						{
							IntPoint target_point = IntPoint(i,j);
							if(myGD->mapState[target_point.x][target_point.y] == mapEmpty &&
							   myGD->mapState[int(target_point.x-10.f*myScale)][target_point.y] == mapEmpty &&
							   myGD->mapState[int(target_point.x+10.f*myScale)][target_point.y] == mapEmpty &&
							   myGD->mapState[target_point.x][int(target_point.y-10.f*myScale)] == mapEmpty &&
							   myGD->mapState[target_point.x][int(target_point.y+10.f*myScale)] == mapEmpty)
							{
								is_not_position = false;
								for(int k=target_point.x-10.f*myScale;k<=target_point.x+10.f*myScale && !is_not_position;k++)
								{
									for(int l=target_point.y-10.f*myScale;l<=target_point.y+10.f*myScale && !is_not_position;l++)
									{
										loop_cnt++;
										calc_distance = sqrtf(powf((target_point.x - k)*pixelSize,2) + powf((target_point.y - l)*pixelSize, 2));
										if(calc_distance < half_distance && myGD->mapState[k][l] != mapEmpty)
										{
											is_not_position = true;
										}
									}
								}
								if(!is_not_position)
								{
									searched_points.push_back(target_point);
									searched_cnt++;
								}
							}
						}
					}
					int random_value = rand()%searched_cnt;
					afterPoint.x = searched_points[random_value].x;
					afterPoint.y = searched_points[random_value].y;
					isFound = true;
					
					CCScaleTo* t_scale = CCScaleTo::create(0.5f, myScale);
					cumberImg->runAction(t_scale);
					
					break;
				}
				
				CCScaleTo* t_scale = CCScaleTo::create(0.5f, myScale);
				cumberImg->runAction(t_scale);
				
				check_loop_cnt = 0;
				//				AlertEngine::sharedInstance()->addSingleAlert("error", "changePosition infinity loop", "ok", 1, this, alertfuncII_selector(MainCumber::alertAction));
			}
			
			afterPoint.x = rand()%int(mapWidthInnerEnd-1-10.f*myScale)+1+10.f*myScale;
			afterPoint.y = rand()%int(mapHeightInnerEnd-1-10.f*myScale)+1+10.f*myScale;
			
			if(afterPoint.isInnerMap())
			{
				float half_distance = 20.f*myScale; // 20.f : radius for base scale 1.f
				float calc_distance;
				IntPoint check_position;
				
				bool is_not_position = false;
				
				for(int i=afterPoint.x-half_distance/2;i<=afterPoint.x+half_distance/2 && !is_not_position;i++)
				{
					for(int j=afterPoint.y-half_distance/2;j<=afterPoint.y+half_distance/2 && !is_not_position;j++)
					{
						calc_distance = sqrtf(powf((afterPoint.x - i)*pixelSize,2) + powf((afterPoint.y - j)*pixelSize, 2));
						if(calc_distance < half_distance)
						{
							check_position = IntPoint(i,j);
							if(!check_position.isInnerMap() || myGD->mapState[check_position.x][check_position.y] != mapEmpty)
							{
								is_not_position = true;
							}
						}
					}
				}
				if(!is_not_position)
				{
					isFound = true;
				}
			}
		}
	}
	
	CCPoint afterPosition = ccp((afterPoint.x-1)*pixelSize+1, (afterPoint.y-1)*pixelSize+1);
	setPosition(afterPosition);
	
	CCScaleTo* t_scale = CCScaleTo::create(0.2, 1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumber::lightSmaller));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	
	runAction(t_seq);
}

void MainCumber::lightSmaller()
{
	CCScaleTo* t_scale = CCScaleTo::create(0.2, 0.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumber::endTeleport));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	
	teleportImg->runAction(t_seq);
}

void MainCumber::endTeleport()
{
	teleportImg->removeFromParentAndCleanup(true);
	teleportImg = NULL;
	startMoving();
}

void MainCumber::createSheild()
{
	if(isSheild == 0)
	{
		isSheild = 1;
		sheildImg = CCSprite::create("sheild.png", CCRectMake(0, 0, 90, 90));
		addChild(sheildImg);
		
		CCSprite* t_texture = CCSprite::create("sheild.png");
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=0;i<4;i++)
		{
			t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*90, 0, 90, 90));
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		
		sheildImg->runAction(t_repeat);
	}
}

void MainCumber::crashSheild()
{
	if(isSheild == 1)
	{
		isSheild = 0;
		sheildImg->removeFromParentAndCleanup(true);
	}
}

void MainCumber::startInvisible()
{
	invisibleFrame = 0;
	add_value = 1;
	zero_cnt = 0;
	schedule(schedule_selector(MainCumber::invisibling));
}

void MainCumber::stopInvisible()
{
	unschedule(schedule_selector(MainCumber::invisibling));
	cumberImg->setOpacity(255);
	//		if(t_chapter_number == 1 && t_stage_number == 1)	((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(255);
	//		else if(t_chapter_number == 11)							((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(255);
	//		else if(t_chapter_number == 16)						my_eye->setOpacity(255);
}

void MainCumber::silenceItem(bool t_b)
{
	is_silenced = t_b;
	if(t_b)
	{
		silenceImg = SilenceEffect::create();
		silenceImg->setPosition(ccp(-4,20));
		addChild(silenceImg);
		
		silenceImg->startAction();
	}
	else
	{
		silenceImg->removeFromParentAndCleanup(true);
		noattack_cnt = 0;
	}
}

void MainCumber::setMovingShoot(bool t_b)
{
	is_moving_shoot = t_b;
}

void MainCumber::setCasting(bool t_b)
{
	is_casting = t_b;
}

void MainCumber::showEmotion(EmotionType t_type)
{
	if(mEmotion)	mEmotion->selfRemove();
	mEmotion = Emotion::create(t_type, this, callfunc_selector(MainCumber::nullmEmotion));
	mEmotion->setPosition(ccp(30,20));
	addChild(mEmotion);
}

void MainCumber::nullmEmotion()
{
	mEmotion = NULL;
}

CCNode* MainCumber::getBossEye()
{
	return my_eye;
}

void MainCumber::caughtBoss(CCObject* t_setCaught, SEL_CallFunc d_setCaught)
{
	target_setCaught = t_setCaught;
	delegate_setCaught = d_setCaught;
	
	CCSprite* prison_bottom = CCSprite::create("subCumber_prison_bottom.png");
	prison_bottom->setScale(1.4f);
	prison_bottom->setOpacity(0);
	prison_bottom->setAnchorPoint(ccp(0.5,0.f));
	prison_bottom->setPosition(ccpAdd(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f), ccp(0,-115)));
	cumberImg->addChild(prison_bottom);
	
	CCSprite* prison_top = CCSprite::create("subCumber_prison_top.png");
	prison_top->setScale(1.4f);
	prison_top->setOpacity(0);
	prison_top->setAnchorPoint(ccp(0.5,1.f));
	prison_top->setPosition(ccpAdd(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f), ccp(0,115)));
	cumberImg->addChild(prison_top);
	
	CCFadeIn* t_fade_bottom = CCFadeIn::create(0.2f);
	CCMoveTo* t_move_bottom = CCMoveTo::create(0.3f, ccpAdd(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f), ccp(0,-40)));
	CCSequence* t_seq_bottom = CCSequence::createWithTwoActions(t_fade_bottom, t_move_bottom);
	
	CCFadeIn* t_fade_top = CCFadeIn::create(0.2f);
	CCMoveTo* t_move_top = CCMoveTo::create(0.3f, ccpAdd(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f), ccp(0,40)));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumber::endCaughtBoss));
	CCAction* t_action_top = CCSequence::create(t_fade_top, t_move_top, t_call, NULL);
	
	prison_bottom->runAction(t_seq_bottom);
	prison_top->runAction(t_action_top);
}

void MainCumber::endCaughtBoss()
{
	(target_setCaught->*delegate_setCaught)();
}

void MainCumber::changeMaxSize(float t_p)
{
	if(t_p < 0.1f)
		maxScale = 1.7f;
	else if(t_p > 0.85f)
		maxScale = 0.7f;
	else
	{
		maxScale = 1.7f -((t_p - 0.1f) / (0.85f-0.1f));
	}
}

void MainCumber::alertAction(int t1, int t2)
{
	
}



void MainCumber::invisibling()
{
	invisibleFrame++;
	
	if(invisibleFrame <= 51)
	{
		cumberImg->setOpacity(255 - invisibleFrame*5);
		//			if(t_chapter_number == 1 && t_stage_number == 1)	((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(255 - invisibleFrame*5);
		//			else if(t_chapter_number == 11)						((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(255 - invisibleFrame*5);
		//			else if(t_chapter_number == 16)						my_eye->setOpacity(255 - invisibleFrame*5);
	}
	else
	{
		if(zero_cnt <= 40)
		{
			zero_cnt++;
		}
		else
		{
			int afterOpacity = cumberImg->getOpacity() + add_value;
			if(afterOpacity > 20)
			{
				add_value = -1;
			}
			if(afterOpacity <= 0)
			{
				add_value = 1;
				zero_cnt = 0;
			}
			cumberImg->setOpacity(afterOpacity);
			//				if(t_chapter_number == 1 && t_stage_number == 1)	((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(afterOpacity);
			//				else if(t_chapter_number == 11)						((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(afterOpacity);
			//				else if(t_chapter_number == 16)						my_eye->setOpacity(afterOpacity);
		}
	}
}

void MainCumber::springCumber()
{
	springCnt++;
	
	CCPoint movePosition;
	movePosition.x = 1.f;
	movePosition.y = tanf(springAngle/180.f*M_PI);
	
	if(springAngle > 90 && springAngle < 270)
		movePosition = ccpMult(movePosition, -1);
	
	float  collectionValue = sqrtf(powf(movePosition.x, 2.f) + powf(movePosition.y, 2.f));
	movePosition = ccpMult(movePosition, 1/collectionValue);
	movePosition = ccpMult(movePosition, springPower);
	
	movePosition = ccpAdd(getPosition(), movePosition);
	
	IntPoint movePoint;
	movePoint.x = int(roundf((movePosition.x-1)/pixelSize + 1.f));
	movePoint.y = int(roundf((movePosition.y-1)/pixelSize + 1.f));
	
	IntPoint half_movePoint;
	half_movePoint.x = int(roundf(((movePosition.x-getPositionX())/2.f+getPositionX()-1)/pixelSize + 1.f));
	half_movePoint.y = int(roundf(((movePosition.y-getPositionY())/2.f+getPositionY()-1)/pixelSize + 1.f));
	
	if(movePosition.x < 20.f*myScale || movePosition.x > 320-20.f*myScale || movePosition.y < 20.f*myScale || movePosition.y > 430-20.f*myScale || !movePoint.isInnerMap() ||
	   myGD->mapState[movePoint.x][movePoint.y] != mapEmpty || myGD->mapState[half_movePoint.x][half_movePoint.y] != mapEmpty)
	{
		stopSpringCumber();
		if(!isGameover)
			startMoving();
		return;
	}
	else
	{
		float half_distance = 20.f*myScale; // 20.f : radius for base scale 1.f
		float calc_distance;
		IntPoint check_position;
		
		bool is_not_position = false;
		
		for(int i=movePoint.x-half_distance/2;i<=movePoint.x+half_distance/2 && !is_not_position;i++)
		{
			for(int j=movePoint.y-half_distance/2;j<=movePoint.y+half_distance/2 && !is_not_position;j++)
			{
				calc_distance = sqrtf(powf((movePoint.x - i)*pixelSize,2) + powf((movePoint.y - j)*pixelSize, 2));
				if(calc_distance < half_distance)
				{
					check_position = IntPoint(i,j);
					if(!check_position.isInnerMap() || myGD->mapState[check_position.x][check_position.y] != mapEmpty)
					{
						is_not_position = true;
					}
				}
			}
		}
		if(is_not_position)
		{
			stopSpringCumber();
			if(!isGameover)
				startMoving();
			return;
		}
	}
	
	
	setPosition(movePosition);
	mapPoint = movePoint; // unnecessary
	myGD->setMainCumberPoint(movePoint);
	
	springPower -= 0.1;
	
	if(springCnt >= 30)
	{
		stopSpringCumber();
		if(!isGameover)
			startMoving();
		return;
	}
}

void MainCumber::crashMapForIntPoint(IntPoint t_p)
{
	GameData* myGD = GameData::sharedGameData();
	IntPoint jackPoint = myGD->getJackPoint();
	
	if(t_p.isInnerMap() && (myGD->mapState[t_p.x][t_p.y] == mapOldline || myGD->mapState[t_p.x][t_p.y] == mapOldget)) // just moment, only map crash
	{
		myGD->mapState[t_p.x][t_p.y] = mapEmpty;
		for(int k = -1;k<=1;k++)
		{
			for(int l = -1;l<=1;l++)
			{
				if(k == 0 && l == 0)	continue;
				if(myGD->mapState[t_p.x+k][t_p.y+l] == mapOldget)		myGD->mapState[t_p.x+k][t_p.y+l] = mapOldline;
			}
		}
		myGD->communication("EP_crashed");
		myGD->communication("MFP_createNewFragment", t_p);
		myGD->communication("VS_divideRect", t_p);
	}
	
	if(jackPoint.x == t_p.x && jackPoint.y == t_p.y)
	{
		showEmotion(kEmotionType_fun);
		myGD->communication("Jack_startDieEffect");
		if(isFuryMode)
			furyModeOff();
	}
	
	if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
	{
		//					myGD->communication("PM_pathChainBomb", t_p);
		showEmotion(kEmotionType_fun);
		myGD->communication("Jack_startDieEffect");
		myGD->communication("Main_showLineDiePosition", t_p);
		if(isFuryMode)
			furyModeOff();
	}
}

void MainCumber::myInit()
{
	my_eye = NULL;
	mEmotion = NULL;
	map_visit_cnt = 0;
	myGD = GameData::sharedGameData();
	is_silenced = false;
	is_casting = false;
	is_slowed = false;
	noattack_cnt = 0;
	isSheild = 0;
	myState = cumberStateStop;
	isGameover = false;
	isFuryMode = false;
	ableTickingTimeBomb = false;
	is_moving_shoot = false;
	teleportImg = NULL;
	//		move_speed = SelectedMapData::sharedInstance()->getBossSpeed();
	move_speed = 1.6f;
	
	before_keep_frame_cnt = 0;
	keep_number = 0;
	
	//		attack_rate = SelectedMapData::sharedInstance()->getAttackRate();
	attack_rate = 400.f;
	
	//		int worldMap = SelectedMapData::sharedInstance()->getSelectedChapter();
	//		t_chapter_number = SelectedMapData::sharedInstance()->getViewChapterNumber();
	//		t_stage_number = SelectedMapData::sharedInstance()->getSelectedStage();
	
	//		is_attackOn = worldMap >= 2;
	is_attackOn = true;
	
	//		if(worldMap <= 20)
	//			max_visit_cnt = 13-(worldMap-1)%10+1;
	//		else
	//			max_visit_cnt = 5;
	max_visit_cnt = 8;
	
	//		if(SelectedMapData::sharedInstance()->getViewChapterNumber() == 1 &&
	//		   SelectedMapData::sharedInstance()->getSelectedStage() == 1)
	//		{
	//			cumberImg = CCSprite::create(CCString::createWithFormat("chapter%d_1_boss_body.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_mainCumber))->getCString());
	//			addChild(cumberImg);
	//
	//			CCSprite* side = CCSprite::create(CCString::createWithFormat("chapter%d_1_boss_side.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_mainCumber))->getCString());
	//			side->setPosition(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f));
	//			cumberImg->addChild(side, -1);
	//		}
	//		else if(SelectedMapData::sharedInstance()->getViewChapterNumber() == 11)
	//		{
	//			cumberImg = CCSprite::create(CCString::createWithFormat("chapter%d_boss_body.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_mainCumber))->getCString());
	//			addChild(cumberImg);
	//
	//			CCSprite* side = CCSprite::create(CCString::createWithFormat("chapter%d_boss_side.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_mainCumber))->getCString());
	//			side->setPosition(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f));
	//			cumberImg->addChild(side, -1);
	//		}
	//		else
	//		{
	//			cumberImg = CCSprite::create(CCString::createWithFormat("chapter%d_boss.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_mainCumber))->getCString());;
	//			addChild(cumberImg);
	//		}
	
	myScale = 1.5f;
	maxScale = 1.7f;
	minScale = 0.7f;
	move_frame = 0;
	areacrash_frame_cnt = 0;
	
	cumberImg = CCSprite::create(CCString::createWithFormat("chapter%d_boss.png", 20)->getCString());;
	cumberImg->setScale(myScale);
	addChild(cumberImg);
	
	//		if(SelectedMapData::sharedInstance()->getViewChapterNumber() == 10)
	//		{
	//			CCNode* outside_parent = CCNode::create();
	//			outside_parent->setPosition(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f));
	//			for(int i=0;i<10;i++)
	//			{
	//				CCSprite* t_outside = CCSprite::create(CCString::createWithFormat("chapter%d_boss_outside.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_mainCumber))->getCString());
	//				t_outside->setRotation(i*36);
	//				outside_parent->addChild(t_outside);
	//			}
	//			cumberImg->addChild(outside_parent);
	//			CCRotateBy* t_rotate = CCRotateBy::create(2.f, 360);
	//			CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
	//			outside_parent->runAction(t_repeat);
	//		}
	//		else if(SelectedMapData::sharedInstance()->getViewChapterNumber() == 16)
	//		{
	//			my_eye = BossEye::create(SelectedMapData::sharedInstance()->getViewChapterSubNumber());
	//			cumberImg->addChild(my_eye);
	//		}
	CCNode* outside_parent = CCNode::create();
	outside_parent->setPosition(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f));
	for(int i=0;i<10;i++)
	{
		CCSprite* t_outside = CCSprite::create(CCString::createWithFormat("chapter%d_boss_outside.png", 20)->getCString());
		t_outside->setRotation(i*36);
		outside_parent->addChild(t_outside);
	}
	cumberImg->addChild(outside_parent);
	CCRotateBy* t_rotate = CCRotateBy::create(2.f, 360);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
	outside_parent->runAction(t_repeat);
	
	
	
	bool isGoodPointed = false;
	int check_loop_cnt = 0;
	while(!isGoodPointed)
	{
		check_loop_cnt++;
		if(check_loop_cnt > 3000)
			AlertEngine::sharedInstance()->addSingleAlert("error", "maincumber init infinity loop", "ok", 1, this, alertfuncII_selector(MainCumber::alertAction));
		
		mapPoint.x = rand()%160 + 1;
		mapPoint.y = rand()%215 + 1;
		
		if(mapPoint.isInnerMap() && myGD->mapState[mapPoint.x][mapPoint.y] == mapEmpty)
		{
			float half_distance = 20.f*myScale; // 20.f : radius for base scale 1.f
			float calc_distance;
			IntPoint check_position;
			
			bool is_not_position = false;
			
			for(int i=mapPoint.x-half_distance/2;i<=mapPoint.x+half_distance/2 && !is_not_position;i++)
			{
				for(int j=mapPoint.y-half_distance/2;j<=mapPoint.y+half_distance/2 && !is_not_position;j++)
				{
					calc_distance = sqrtf(powf((mapPoint.x - i)*pixelSize,2) + powf((mapPoint.y - j)*pixelSize, 2));
					if(calc_distance < half_distance)
					{
						check_position = IntPoint(i,j);
						if(!check_position.isInnerMap() || myGD->mapState[check_position.x][check_position.y] != mapEmpty)
						{
							is_not_position = true;
						}
					}
				}
			}
			if(!is_not_position)
			{
				isGoodPointed = true;
			}
		}
	}
	
	myGD->setMainCumberPoint(mapPoint);
	
	setPosition(ccp((mapPoint.x-1)*pixelSize + 1,(mapPoint.y-1)*pixelSize + 1));
	
	int input_change_angle = -10;
	for(int i=0;i<360;i++)
	{
		checking_array[i] = input_change_angle++;
		
		if(i == 20)			input_change_angle = -20;
		else if(i == 30)	input_change_angle = 11;
		else if(i == 40)	input_change_angle = -30;
		else if(i == 50)	input_change_angle = 21;
		else if(i == 60)	input_change_angle = -40;
		else if(i == 70)	input_change_angle = 31;
		else if(i == 80)	input_change_angle = -50;
		else if(i == 90)	input_change_angle = 41;
		else if(i == 100)	input_change_angle = -60;
		else if(i == 110)	input_change_angle = 51;
		else if(i == 120)	input_change_angle = -70;
		else if(i == 130)	input_change_angle = 61;
		else if(i == 140)	input_change_angle = -80;
		else if(i == 150)	input_change_angle = 71;
		else if(i == 160)	input_change_angle = -90;
		else if(i == 170)	input_change_angle = 81;
		else if(i == 180)	input_change_angle = -100;
		else if(i == 190)	input_change_angle = 91;
		else if(i == 200)	input_change_angle = -110;
		else if(i == 210)	input_change_angle = 101;
		else if(i == 220)	input_change_angle = -120;
		else if(i == 230)	input_change_angle = 111;
		else if(i == 240)	input_change_angle = -130;
		else if(i == 250)	input_change_angle = 121;
		else if(i == 260)	input_change_angle = -140;
		else if(i == 270)	input_change_angle = 131;
		else if(i == 280)	input_change_angle = -150;
		else if(i == 290)	input_change_angle = 141;
		else if(i == 300)	input_change_angle = -160;
		else if(i == 310)	input_change_angle = 151;
		else if(i == 320)	input_change_angle = -170;
		else if(i == 330)	input_change_angle = 161;
		else if(i == 340)	input_change_angle = -180;
		else if(i == 350)	input_change_angle = 171;
	}
}

void MainCumber::randomShuffle()
{
	random_shuffle(checking_array, checking_array+21);
	random_shuffle(checking_array+21, checking_array+41);
	random_shuffle(checking_array+41, checking_array+61);
	random_shuffle(checking_array+61, checking_array+81);
	random_shuffle(checking_array+81, checking_array+101);
	random_shuffle(checking_array+101, checking_array+121);
	random_shuffle(checking_array+121, checking_array+141);
	random_shuffle(checking_array+141, checking_array+161);
	random_shuffle(checking_array+161, checking_array+181);
	random_shuffle(checking_array+181, checking_array+201);
	random_shuffle(checking_array+201, checking_array+221);
	random_shuffle(checking_array+221, checking_array+241);
	random_shuffle(checking_array+241, checking_array+261);
	random_shuffle(checking_array+261, checking_array+281);
	random_shuffle(checking_array+281, checking_array+301);
	random_shuffle(checking_array+301, checking_array+321);
	random_shuffle(checking_array+321, checking_array+341);
	random_shuffle(checking_array+341, checking_array+360);
}

bool MainCumber::checkingFunc(IntPoint check_position, bool& is_not_position, bool& notEmotion, int& before_areacrash_cnt, bool& is_map_visited)
{
	if(!check_position.isInnerMap())
	{
		is_not_position = true;
	}
	else if(myGD->mapState[check_position.x][check_position.y] == mapNewline)
	{
		is_not_position = true;
		IntPoint jackPoint = myGD->getJackPoint();
		if(jackPoint.x == check_position.x && jackPoint.y == check_position.y)
		{
			//				stopMoving();
			
			if(notEmotion)
			{
				notEmotion = false;
				showEmotion(kEmotionType_fun);
			}
			
			myGD->communication("Jack_startDieEffect");
			return true;
		}
		else
		{
			if(notEmotion)
			{
				notEmotion = false;
				showEmotion(kEmotionType_joy);
			}
			myGD->communication("SW_createSW", check_position, this, callfuncI_selector(MainCumber::showEmotion));
		}
	}
	else if(myGD->mapState[check_position.x][check_position.y] == mapOldline || myGD->mapState[check_position.x][check_position.y] == mapOldget)
	{
		is_not_position = true;
		areacrash_frame_cnt = before_areacrash_cnt + 1;
		if(!is_map_visited && !is_casting)
		{
			map_visit_cnt++;
			is_map_visited = true;
		}
		if(is_attackOn && map_visit_cnt >= max_visit_cnt && !is_silenced && !isFuryMode && !ableTickingTimeBomb && !is_moving_shoot && !is_casting)
		{
			is_casting = true;
			stopMoving();
			noattack_cnt = 0;
			map_visit_cnt = 0;
			
			if(notEmotion)
			{
				notEmotion = false;
				showEmotion(kEmotionType_angry);
			}
			
			myGD->communication("MP_startFire", getPosition(), true);
			return true;
		}
		
		if(rand()%2)					directionAngle -= 90;
		else							directionAngle += 90;
		
		if(directionAngle < 0)			directionAngle += 360;
		else if(directionAngle > 360)	directionAngle -= 360;
		
		if(ableTickingTimeBomb)
		{
			myGD->communication("MP_createTickingTimeBomb", check_position, 60, 9, 1);
		}
	}
	return false;
}


