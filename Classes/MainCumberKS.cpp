//
//  MainCumberKS.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 4..
//
//

#include "MainCumberKS.h"


#include "MainCumber.h"


void MainCumberKS::moving(float dt)
{
	m_noattack_cnt++;
		
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
//		if(!m_isFuryMode)
		if(1)
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
			
			// 맵에 부딪혔을 때,
			if(afterPosition.x < 20.f*myScale || afterPosition.x > 320.f-20.f*myScale || afterPosition.y < 20.f*myScale || afterPosition.y > 430.f-20.f*myScale)
			{
				if(rand()%2)					directionAngle -= 90;
				else							directionAngle += 90;
				
				if(directionAngle < 0)			directionAngle += 360;
				else if(directionAngle > 360)	directionAngle -= 360;
			}
			else if(afterPoint.isInnerMap()) // 유효한 영역안에 있을 때,
			{
				float half_distance = 20.f*myScale; // 20.f : radius for base scale 1.f
				float calc_distance;
				IntPoint check_position;
				
				bool is_not_position = false;
				
				// side check (up, down, left, right)
				
				check_position = IntPoint(afterPoint.x-half_distance/2,afterPoint.y); // left
				if(checkingFunc(check_position, &is_not_position, &notEmotion, &before_areacrash_cnt, &is_map_visited))
					return;
				
				check_position = IntPoint(afterPoint.x+half_distance/2,afterPoint.y); // right
				if(checkingFunc(check_position, &is_not_position, &notEmotion, &before_areacrash_cnt, &is_map_visited))
					return;
				
				check_position = IntPoint(afterPoint.x,afterPoint.y+half_distance/2); // up
				if(checkingFunc(check_position, &is_not_position, &notEmotion, &before_areacrash_cnt, &is_map_visited))
					return;
				
				check_position = IntPoint(afterPoint.x,afterPoint.y-half_distance/2); // down
				if(checkingFunc(check_position, &is_not_position, &notEmotion, &before_areacrash_cnt, &is_map_visited))
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
							if(checkingFunc(check_position, &is_not_position, &notEmotion, &before_areacrash_cnt, &is_map_visited))
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
		
	}
	
		
	if(m_ableTickingTimeBomb)
	{
		m_tickingFrame--;
		if(m_tickingFrame <= 0)
		{
			tickingOff();
		}
	}
	
	mapPoint = afterPoint; // unnecessary
	myGD->setMainCumberPoint(afterPoint);
	setPosition(afterPosition);
	
	m_before_keep_frame_cnt++;
	
	if(m_before_keep_frame_cnt >= 60)
	{
		if(m_keep_number >= 100)
		{
			m_keep_number = 0;
		}
		m_before_points[m_keep_number++] = afterPoint;
		m_before_keep_frame_cnt = 0;
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

void MainCumberKS::cumberImgStartRotating(float speed)
{
	CCRotateBy* t_rotate = CCRotateBy::create(0.5, speed);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
	
	cumberImg->runAction(t_repeat);
}

void MainCumberKS::checkingJackCrash()
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
						myGD->communication("SW_createSW", check_position, this, callfuncI_selector(MainCumberKS::showEmotion));
					}
				}
			}
		}
	}
}




void MainCumberKS::startMoving()
{
	if(!m_isGameover)
	{
		myState = cumberStateMoving;
		directionAngle = rand()%360;
		schedule(schedule_selector(MainCumberKS::moving));
	}
}
void MainCumberKS::stopMoving()
{
	unschedule(schedule_selector(MainCumberKS::moving));
	myState = cumberStateStop;
}
void MainCumberKS::setCumberState(int t_cs)
{
	myState = cumberState(t_cs);
}
cumberState MainCumberKS::getCumberState()
{
	return myState;
}

void MainCumberKS::setGameover()
{
	m_isGameover = true;
}


void MainCumberKS::tickingOn()
{
	if(!m_ableTickingTimeBomb)
	{
		m_tickingImg = CCSprite::create("tickingTimeBomb_main.png");
		addChild(m_tickingImg);
		CCSprite* tickingPad = CCSprite::create("tickingTimeBomb_pad.png");
		tickingPad->setPosition(ccp(17,12));
		m_tickingImg->addChild(tickingPad);
	}
	
	m_ableTickingTimeBomb = true;
	m_tickingFrame = 300;
}

void MainCumberKS::tickingOff()
{
	if(m_ableTickingTimeBomb)
	{
		m_ableTickingTimeBomb = false;
		m_tickingImg->removeFromParentAndCleanup(true);
	}
}

void MainCumberKS::startTeleport()
{
	if(m_teleportImg)
	{
		m_teleportImg->removeFromParentAndCleanup(true);
		m_teleportImg = NULL;
	}
	
	m_teleportImg = CCSprite::create("teleport_light.png");
	m_teleportImg->setScale(0);
	addChild(m_teleportImg);
	
	CCScaleTo* t_scale = CCScaleTo::create(0.2, 2.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumberKS::smaller));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	
	m_teleportImg->runAction(t_seq);
	AudioEngine::sharedInstance()->playEffect("sound_teleport.mp3",false);
}

void MainCumberKS::smaller()
{
	CCScaleTo* t_scale = CCScaleTo::create(0.2, 0.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumberKS::changePosition));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	
	runAction(t_seq);
}

void MainCumberKS::changePosition()
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
	for(int i=0;i<m_before_points.size() && !isFound;i++)
	{
		afterPoint = m_before_points[i];
		
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
				//				AlertEngine::sharedInstance()->addSingleAlert("error", "changePosition infinity loop", "ok", 1, this, alertfuncII_selector(MainCumberKS::alertAction));
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
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumberKS::lightSmaller));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	
	runAction(t_seq);
}

void MainCumberKS::lightSmaller()
{
	CCScaleTo* t_scale = CCScaleTo::create(0.2, 0.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumberKS::endTeleport));
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	
	m_teleportImg->runAction(t_seq);
}

void MainCumberKS::endTeleport()
{
	m_teleportImg->removeFromParentAndCleanup(true);
	m_teleportImg = NULL;
	startMoving();
}

void MainCumberKS::createSheild()
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

void MainCumberKS::crashSheild()
{
	if(isSheild == 1)
	{
		isSheild = 0;
		sheildImg->removeFromParentAndCleanup(true);
	}
}

void MainCumberKS::startInvisible()
{
	m_invisibleFrame = 0;
	m_add_value = 1;
	m_zero_cnt = 0;
	schedule(schedule_selector(MainCumberKS::invisibling));
}

void MainCumberKS::stopInvisible()
{
	unschedule(schedule_selector(MainCumberKS::invisibling));
	cumberImg->setOpacity(255);
	//		if(t_chapter_number == 1 && t_stage_number == 1)	((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(255);
	//		else if(t_chapter_number == 11)							((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(255);
	//		else if(t_chapter_number == 16)						my_eye->setOpacity(255);
}

void MainCumberKS::silenceItem(bool t_b)
{
	m_is_silenced = t_b;
	if(t_b)
	{
		m_silenceImg = SilenceEffect::create();
		m_silenceImg->setPosition(ccp(-4,20));
		addChild(m_silenceImg);
		
		m_silenceImg->startAction();
	}
	else
	{
		m_silenceImg->removeFromParentAndCleanup(true);
		m_noattack_cnt = 0;
	}
}



void MainCumberKS::setCasting(bool t_b)
{
	m_is_casting = t_b;
}

void MainCumberKS::showEmotion(EmotionType t_type)
{
	if(m_mEmotion)	m_mEmotion->selfRemove();
	m_mEmotion = Emotion::create(t_type, this, callfunc_selector(MainCumberKS::nullmEmotion));
	m_mEmotion->setPosition(ccp(30,20));
	addChild(m_mEmotion);
}

void MainCumberKS::nullmEmotion()
{
	m_mEmotion = NULL;
}

CCNode* MainCumberKS::getBossEye()
{
	return m_my_eye;
}

void MainCumberKS::caughtBoss(CCObject* t_setCaught, SEL_CallFunc d_setCaught)
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
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainCumberKS::endCaughtBoss));
	CCAction* t_action_top = CCSequence::create(t_fade_top, t_move_top, t_call, NULL);
	
	prison_bottom->runAction(t_seq_bottom);
	prison_top->runAction(t_action_top);
}

void MainCumberKS::endCaughtBoss()
{
	(target_setCaught->*delegate_setCaught)();
}

void MainCumberKS::changeMaxSize(float t_p)
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

void MainCumberKS::alertAction(int t1, int t2)
{
	
}



void MainCumberKS::invisibling(float dt)
{
	m_invisibleFrame++;
	
	if(m_invisibleFrame <= 51)
	{
		cumberImg->setOpacity(255 - m_invisibleFrame*5);
		//			if(t_chapter_number == 1 && t_stage_number == 1)	((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(255 - invisibleFrame*5);
		//			else if(t_chapter_number == 11)						((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(255 - invisibleFrame*5);
		//			else if(t_chapter_number == 16)						my_eye->setOpacity(255 - invisibleFrame*5);
	}
	else
	{
		if(m_zero_cnt <= 40)
		{
			m_zero_cnt++;
		}
		else
		{
			int afterOpacity = cumberImg->getOpacity() + m_add_value;
			if(afterOpacity > 20)
			{
				m_add_value = -1;
			}
			if(afterOpacity <= 0)
			{
				m_add_value = 1;
				m_zero_cnt = 0;
			}
			cumberImg->setOpacity(afterOpacity);
			//				if(t_chapter_number == 1 && t_stage_number == 1)	((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(afterOpacity);
			//				else if(t_chapter_number == 11)						((CCSprite*)cumberImg->getChildren()->objectAtIndex(0))->setOpacity(afterOpacity);
			//				else if(t_chapter_number == 16)						my_eye->setOpacity(afterOpacity);
		}
	}
}


void MainCumberKS::crashMapForIntPoint(IntPoint t_p)
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
	}
	
	if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
	{
		//					myGD->communication("PM_pathChainBomb", t_p);
		showEmotion(kEmotionType_fun);
		myGD->communication("Jack_startDieEffect");
		myGD->communication("Main_showLineDiePosition", t_p);
	}
}

void MainCumberKS::myInit()
{
	m_my_eye = NULL;
	m_mEmotion = NULL;
	m_map_visit_cnt = 0;
	myGD = GameData::sharedGameData();
	m_is_silenced = false;
	m_is_casting = false;
	is_slowed = false;
	m_noattack_cnt = 0;
	isSheild = 0;
	myState = cumberStateStop;
	m_isGameover = false;
//	m_isFuryMode = false;
	m_ableTickingTimeBomb = false;
//	m_is_moving_shoot = false;
	m_teleportImg = NULL;
	//		move_speed = SelectedMapData::sharedInstance()->getBossSpeed();
	move_speed = 1.6f;
	
	m_before_keep_frame_cnt = 0;
	m_keep_number = 0;
	
	//		attack_rate = SelectedMapData::sharedInstance()->getAttackRate();
	m_attack_rate = 400.f;
	
	//		int worldMap = SelectedMapData::sharedInstance()->getSelectedChapter();
	//		t_chapter_number = SelectedMapData::sharedInstance()->getViewChapterNumber();
	//		t_stage_number = SelectedMapData::sharedInstance()->getSelectedStage();
	
	//		is_attackOn = worldMap >= 2;
	m_is_attackOn = true;
	
	//		if(worldMap <= 20)
	//			max_visit_cnt = 13-(worldMap-1)%10+1;
	//		else
	//			max_visit_cnt = 5;
	m_max_visit_cnt = 8;
	
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
			AlertEngine::sharedInstance()->addSingleAlert("error", "maincumber init infinity loop", "ok", 1, this, alertfuncII_selector(MainCumberKS::alertAction));
		
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

void MainCumberKS::randomShuffle()
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

bool MainCumberKS::checkingFunc(IntPoint check_position, bool* is_not_position, bool* notEmotion, int* before_areacrash_cnt, bool* is_map_visited)
{
	if(!check_position.isInnerMap())
	{
		*is_not_position = true;
	}
	else if(myGD->mapState[check_position.x][check_position.y] == mapNewline)
	{
		*is_not_position = true;
		IntPoint jackPoint = myGD->getJackPoint();
		if(jackPoint.x == check_position.x && jackPoint.y == check_position.y)
		{
			//				stopMoving();
			
			if(*notEmotion)
			{
				*notEmotion = false;
				showEmotion(kEmotionType_fun);
			}
			
			myGD->communication("Jack_startDieEffect");
			return true;
		}
		else
		{
			if(*notEmotion)
			{
				*notEmotion = false;
				showEmotion(kEmotionType_joy);
			}
			myGD->communication("SW_createSW", check_position, this, callfuncI_selector(MainCumberKS::showEmotion));
		}
	}
	else if(myGD->mapState[check_position.x][check_position.y] == mapOldline || myGD->mapState[check_position.x][check_position.y] == mapOldget)
	{
		*is_not_position = true;
		areacrash_frame_cnt = *before_areacrash_cnt + 1;
		if(!is_map_visited && !m_is_casting)
		{
			m_map_visit_cnt++;
			*is_map_visited = true;
		}
		if(m_is_attackOn && m_map_visit_cnt >= m_max_visit_cnt && !m_is_silenced && !m_ableTickingTimeBomb && !m_is_casting)
		{
			m_is_casting = true;
			stopMoving();
			m_noattack_cnt = 0;
			m_map_visit_cnt = 0;
			
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
		
		if(m_ableTickingTimeBomb)
		{
			myGD->communication("MP_createTickingTimeBomb", check_position, 60, 9, 1);
		}
	}
	return false;
}
