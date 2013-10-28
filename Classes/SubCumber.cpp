//
//  SubCumber.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 20..
//
//

#include "SubCumber.h"

void SubCumber::caughtAnimation()
{
	myGD->communication("UI_catchSubCumber");
	myGD->communication("CP_createSubCumber", myGD->getMainCumberPoint());
	
	CCSprite* prison_bottom = CCSprite::create("subCumber_prison_bottom.png");
	prison_bottom->setOpacity(0);
	prison_bottom->setAnchorPoint(ccp(0.5,0.f));
	prison_bottom->setPosition(ccpAdd(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f), ccp(0,-85)));
	cumberImg->addChild(prison_bottom);
	
	CCSprite* prison_top = CCSprite::create("subCumber_prison_top.png");
	prison_top->setOpacity(0);
	prison_top->setAnchorPoint(ccp(0.5,1.f));
	prison_top->setPosition(ccpAdd(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f), ccp(0,85)));
	cumberImg->addChild(prison_top);
	
	CCFadeIn* t_fade_bottom = CCFadeIn::create(0.2f);
	CCMoveTo* t_move_bottom = CCMoveTo::create(0.3f, ccpAdd(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f), ccp(0,-30)));
	CCSequence* t_seq_bottom = CCSequence::createWithTwoActions(t_fade_bottom, t_move_bottom);
	
	CCFadeIn* t_fade_top = CCFadeIn::create(0.2f);
	CCMoveTo* t_move_top = CCMoveTo::create(0.3f, ccpAdd(ccp(cumberImg->getContentSize().width/2.f,cumberImg->getContentSize().height/2.f), ccp(0,30)));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(SubCumber::caughtSpinAnimation));
	CCAction* t_action_top = CCSequence::create(t_fade_top, t_move_top, t_call, NULL);
	
	prison_bottom->runAction(t_seq_bottom);
	prison_top->runAction(t_action_top);
}

void SubCumber::caughtSpinAnimation()
{
	CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.f);
	CCRotateBy* t_rotate = CCRotateBy::create(1.f, 540);
	CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scale, t_rotate);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(SubCumber::caughtSelfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
	
	cumberImg->runAction(t_seq);
}

void SubCumber::easyMoving()
{
	if(myGD->mapState[mapPoint.x][mapPoint.y] != mapEmpty &&
	   myGD->mapState[mapPoint.x-1][mapPoint.y] != mapEmpty &&
	   myGD->mapState[mapPoint.x+1][mapPoint.y] != mapEmpty &&
	   myGD->mapState[mapPoint.x][mapPoint.y-1] != mapEmpty &&
	   myGD->mapState[mapPoint.x][mapPoint.y+1] != mapEmpty)
	{
		stopMoving();
		
		AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
		int missile_type = my_element;
		
		int rmCnt = 6/2 + 1;
		float damage_per = 6*0.8f / rmCnt;
		myGD->communication("MP_createJackMissile", missile_type, rmCnt, damage_per);
		
		
		myGD->communication("CP_removeSubCumber", this);
		
		if(mySD->getClearCondition() == kCLEAR_subCumberCatch)
		{
			caughtAnimation();
		}
		else
		{
			removeFromParentAndCleanup(true);
		}
		return;
	}
	
	bool isFindedAfterPoint = false;
	CCPoint afterPosition;
	IntPoint afterPoint;
	int check_loop_cnt = 0;
//	bool notEmotion = true;
	
	int before_areacrash_cnt = areacrash_frame_cnt;
	do
	{
		check_loop_cnt++;
		if(check_loop_cnt > 3000)
		{
			stopMoving();
			myGD->communication("CP_removeSubCumber", this);
			removeFromParentAndCleanup(true);
			return;
		}
		
		CCPoint d_p;
		
		d_p.x = 1.f;
		d_p.y = tanf(directionAngle/180.f*M_PI);
		
		if(directionAngle > 90 && directionAngle < 270)
			d_p = ccpMult(d_p, -1);
		
		float length = sqrtf(powf(d_p.x, 2.f) + powf(d_p.y, 2.f));
		
		d_p = ccpMult(d_p, move_speed/length);
		if(is_slowed)
			d_p = ccpMult(d_p, 1.f/2.f);
		
		afterPosition = ccpAdd(getPosition(), d_p);
		afterPoint.x = roundf((afterPosition.x-1)/pixelSize + 1.f);
		afterPoint.y = roundf((afterPosition.y-1)/pixelSize + 1.f);
		
		if(!afterPoint.isInnerMap())
		{
			searchAfterDirection();
		}
		else if(afterPosition.x < 0 || afterPosition.x > 320 || afterPosition.y < 0 || afterPosition.y > 430)
		{
			searchAfterDirection();
		}
		else if(afterPoint.isInnerMap() && myGD->mapState[afterPoint.x][afterPoint.y] == mapNewline)
		{
			searchAfterDirection();
			
			IntPoint jackPoint = myGD->getJackPoint();
			if(jackPoint.x == afterPoint.x && jackPoint.y == afterPoint.y)
			{
//				stopMoving();
				
//				if(notEmotion)
//				{
//					notEmotion = false;
//					showEmotion(kEmotionType_fun);
//				}
				
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				return;
			}
			else
			{
//				if(notEmotion)
//				{
//					notEmotion = false;
//					showEmotion(kEmotionType_joy);
//				}
				myGD->communication("SW_createSW", afterPoint);
			}
		}
		else if(afterPoint.isInnerMap() && myGD->mapState[afterPoint.x][afterPoint.y] != mapEmpty)
		{
			areacrash_frame_cnt = before_areacrash_cnt+1;
			searchAfterDirection();
		}
		else if(afterPoint.isInnerMap() && myGD->mapState[afterPoint.x][afterPoint.y] == mapEmpty)
		{
			isFindedAfterPoint = true;
			if(directionAngle <= 90 || directionAngle > 270)
				cumberImg->setFlipX(true);
			else
				cumberImg->setFlipX(false);
		}
		
		if(!isFindedAfterPoint)
		{
			CCPoint d_p;
			
			d_p.x = 1.f;
			d_p.y = tanf(directionAngle/180.f*M_PI);
			
			if(directionAngle > 90 && directionAngle < 270)
				d_p = ccpMult(d_p, -1);
			
			float length = sqrtf(powf(d_p.x, 2.f) + powf(d_p.y, 2.f));
			
			d_p = ccpMult(d_p, move_speed/length);
			if(is_slowed)
				d_p = ccpMult(d_p, 1.f/2.f);
			
			afterPosition = ccpAdd(getPosition(), d_p);
			afterPoint.x = roundf((afterPosition.x-1)/pixelSize + 1.f);
			afterPoint.y = roundf((afterPosition.y-1)/pixelSize + 1.f);
			
			isFindedAfterPoint = afterPoint.isInnerMap();
		}
	}while (!isFindedAfterPoint);
	
	mapPoint = afterPoint; // unnecessary
	setPosition(afterPosition);
	
	move_frame++;
	
	if(move_frame >= 45)
	{
		float areacrash_rate = 1.f*areacrash_frame_cnt/move_frame;
		
		if(areacrash_rate <= 1.f/60.f)
			myScale += 0.02f;
		else if(areacrash_rate >= 1.f/10.f)
			myScale -= 0.02f;
		else
		{
			if(((areacrash_rate-1.f/60.f) / (1.f/10.f - 1.f/60.f)) + 0.7f > myScale)
				myScale -= 0.01f;
			else
				myScale += 0.01f;
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

CCPoint SubCumber::getAfterPosition(int a_angle)
{
	CCPoint after_position;
	CCPoint d_p = ccp(1.f, tanf(a_angle/180.f*M_PI));
	
	if(a_angle > 90 && a_angle < 270)
		d_p = ccpMult(d_p, -1);
	
	float length = sqrtf(powf(d_p.x, 2.f) + powf(d_p.y, 2.f));
	
	d_p = ccpMult(d_p, move_speed/length);
	if(is_slowed)
		d_p = ccpMult(d_p, 1.f/2.f);
	
	after_position = ccpAdd(getPosition(), d_p);
	return after_position;
}

void SubCumber::searchAfterDirection()
{
	int a_angle;
	
	if(directionAngle < 45 || (directionAngle >= 135 && directionAngle < 225) || directionAngle >= 315)
	{
		// 180 - x, -x, reverse
		a_angle = 180 - directionAngle;
		if(a_angle < 0)			a_angle += 360;
		if(a_angle >= 360)		a_angle -= 360;
		
		CCPoint after_position = getAfterPosition(a_angle);
		IntPoint after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
		if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
		{
			a_angle = -directionAngle;
			if(a_angle < 0)			a_angle += 360;
			if(a_angle >= 360)		a_angle -= 360;
		}
		else
		{
			directionAngle = a_angle;
			return;
		}
		
		after_position = getAfterPosition(a_angle);
		after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
		if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
		{
			a_angle = directionAngle + 180;
			if(a_angle < 0)			a_angle += 360;
			if(a_angle >= 360)		a_angle -= 360;
		}
		else
		{
			directionAngle = a_angle;
			return;
		}
		
		after_position = getAfterPosition(a_angle);
		directionAngle = a_angle;
		return;
	}
	else
	{
		// -x, 180 - x, reverse
		a_angle = -directionAngle;
		if(a_angle < 0)			a_angle += 360;
		if(a_angle >= 360)		a_angle -= 360;
		
		CCPoint after_position = getAfterPosition(a_angle);
		IntPoint after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
		if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
		{
			a_angle = 180 - directionAngle;
			if(a_angle < 0)			a_angle += 360;
			if(a_angle >= 360)		a_angle -= 360;
		}
		else
		{
			directionAngle = a_angle;
			return;
		}
		
		after_position = getAfterPosition(a_angle);
		after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
		if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
		{
			a_angle = directionAngle + 180;
			if(a_angle < 0)			a_angle += 360;
			if(a_angle >= 360)		a_angle -= 360;
		}
		else
		{
			directionAngle = a_angle;
			return;
		}
		
		after_position = getAfterPosition(a_angle);
		directionAngle = a_angle;
		return;
	}
}

void SubCumber::moving()
{
	if(myGD->mapState[mapPoint.x][mapPoint.y] != mapEmpty &&
	   myGD->mapState[mapPoint.x-1][mapPoint.y] != mapEmpty &&
	   myGD->mapState[mapPoint.x+1][mapPoint.y] != mapEmpty &&
	   myGD->mapState[mapPoint.x][mapPoint.y-1] != mapEmpty &&
	   myGD->mapState[mapPoint.x][mapPoint.y+1] != mapEmpty)
	{
		stopMoving();
		
		AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
		int missile_type = my_element;
		
		int rmCnt = 6/2 + 1;
		float damage_per = 6*0.8f / rmCnt;
		myGD->communication("MP_createJackMissile", missile_type, rmCnt, damage_per);
		
		
		myGD->communication("CP_removeSubCumber", this);
		
		if(mySD->getClearCondition() == kCLEAR_subCumberCatch)
		{
			caughtAnimation();
		}
		else
		{
			removeFromParentAndCleanup(true);
		}
		return;
	}
	
	// replication
	//	if(rand()%(700+700*myGD->getCommunication("CP_getSubCumberCount")) == 0) // replication
	//	{
	//		stopMoving();
	//		myGD->communication("MP_createSubCumberReplication", getPosition(), this, callfunc_selector(SubCumber::startMoving));
	//	}
	
	bool isFindedAfterPoint = false;
	CCPoint afterPosition;
	IntPoint afterPoint;
	int check_loop_cnt = 0;
//	bool notEmotion = true;
	
	int before_areacrash_cnt = areacrash_frame_cnt;
	
	while(!isFindedAfterPoint)
	{
		check_loop_cnt++;
		if(check_loop_cnt > 3000)
		{
			stopMoving();
			myGD->communication("CP_removeSubCumber", this);
			removeFromParentAndCleanup(true);
			return;
		}
		
		int changeAngleValue = rand()%5 - 2;
		changeAngleValue *= rand()%5+1;
		
		directionAngle += changeAngleValue;
		if(directionAngle < 0)			directionAngle += 360;
		else if(directionAngle > 360)	directionAngle -= 360;
		
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
		
		if(afterPosition.x < 0 || afterPosition.x > 320 || afterPosition.y < 0 || afterPosition.y > 430)
		{
			if(rand()%2)					directionAngle -= 90;
			else							directionAngle += 90;
			
			if(directionAngle < 0)			directionAngle += 360;
			else if(directionAngle > 360)	directionAngle -= 360;
		}
		else if(afterPoint.isInnerMap() && myGD->mapState[afterPoint.x][afterPoint.y] == mapNewline)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			if(jackPoint.x == afterPoint.x && jackPoint.y == afterPoint.y)
			{
//				stopMoving();
				
//				if(notEmotion)
//				{
//					notEmotion = false;
//					showEmotion(kEmotionType_fun);
//				}
				
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				return;
			}
			else
			{
//				if(notEmotion)
//				{
//					notEmotion = false;
//					showEmotion(kEmotionType_joy);
//				}
				myGD->communication("SW_createSW", afterPoint);
			}
		}
		else if(afterPoint.isInnerMap() && (myGD->mapState[afterPoint.x][afterPoint.y] == mapOldline || myGD->mapState[afterPoint.x][afterPoint.y] == mapOldget))
		{
			areacrash_frame_cnt = before_areacrash_cnt + 1;
			
			if(rand()%2)					directionAngle -= 90;
			else							directionAngle += 90;
			
			if(directionAngle < 0)			directionAngle += 360;
			else if(directionAngle > 360)	directionAngle -= 360;
		}
		else if(afterPoint.isInnerMap() && myGD->mapState[afterPoint.x][afterPoint.y] == mapEmpty)
		{
			isFindedAfterPoint = true;
			if(directionAngle <= 90 || directionAngle > 270)
				cumberImg->setFlipX(true);
			else
				cumberImg->setFlipX(false);
		}
	}
	
	mapPoint = afterPoint; // unnecessary
	setPosition(afterPosition);
	
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