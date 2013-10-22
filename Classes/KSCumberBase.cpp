//
//  KSCumberBase.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#include "KSCumberBase.h"
#include "Jack.h"
void KSCumberBase::crashMapForIntPoint( IntPoint t_p )
{
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
//		myGD->communication("EP_crashed");
		myGD->communication("MFP_createNewFragment", t_p);
		myGD->communication("VS_divideRect", t_p);
	}

	if(jackPoint.x == t_p.x && jackPoint.y == t_p.y)
	{
		//			showEmotion(kEmotionType_fun);
		myGD->communication("Jack_startDieEffect");
		//			if(isFuryMode) //##
		//				furyModeOff();
	}

	if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
	{
		//					myGD->communication("PM_pathChainBomb", t_p);

		myGD->communication("Jack_startDieEffect");
		myGD->communication("Main_showLineDiePosition", t_p);
		//			if(isFuryMode) //##
		//				furyModeOff();
	}
}


void KSCumberBase::randomMoving(float dt)
{
	

	
	m_scale.timer += 1/60.f;
	
	
	if(m_scale.collisionStartTime + 1 < m_scale.timer || m_state != CUMBERSTATEMOVING)
	{
		m_scale.collisionCount = 0;
		m_scale.collisionStartTime = m_scale.timer;
		//		setCumberSize(MIN(1.0, getCumberSize() + scale.SCALE_ADDER));
	}
	CCPoint afterPosition;
	IntPoint afterPoint;
	//	int check_loop_cnt = 0;
	
	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		int changeDirection = ProbSelector::sel(0.05, 1.0 - 0.05, 0.0);
		if(changeDirection == 0)
		{
			m_directionAngleDegree += m_well512.GetValue(-4, +4);
		}
		
		
		int sela = ProbSelector::sel(0.005, 1.0 - 0.005, 0.0);
		if(sela == 0)
		{
			m_directionAngleDegree += m_well512.GetValue(90, 270);
		}
	}
	
	bool validPosition = false;
	int cnt = 0;
	bool onceOutlineAndMapCollision = false;
	bool pathFound = true; // 일단 찾았다고 셋 해놓고 특수한 경우에 false 시킴.
	while(!validPosition)
	{
		cnt++;
		float speedX = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
		float speedY = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
		
		CCPoint cumberPosition = getPosition();
		afterPosition = cumberPosition + ccp(speedX, speedY);
		afterPoint = ccp2ip(afterPosition);
		
		IntPoint checkPosition;
		COLLISION_CODE collisionCode = getCrashCode(afterPoint, &checkPosition);
		if(m_state != CUMBERSTATEFURY)
		{
			if(collisionCode == kCOLLISION_JACK)
			{
				validPosition = true;
				// 즉사 시킴.
				if(myGD->getJackState() != jackStateNormal)
					myGD->communication("Jack_startDieEffect");
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				onceOutlineAndMapCollision = true;
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				//			CCLog("collision!!");
				onceOutlineAndMapCollision = true;
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//			CCLog("collision!!");
				//			myGD->communication("Jack_startDieEffect");
				myGD->communication("SW_createSW", checkPosition);
				//									callfuncI_selector(MetalSnake::showEmotion)); //##
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else
			{
				CCLog("what!?");
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
				//			CCLog("collision!!");
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
			}
			else
			{
				validPosition = true;
			}
			if(m_furyMode.furyFrameCount % 8 == 0) // n 프레임당 한번 깎음.
			{
				crashMapForPosition(afterPosition);
			}
		}
		if(cnt >= 2)
		{
			CCLog("random cnt !! = %d", cnt);
		}
		if(cnt >= 30)
		{
			pathFound = false;
			validPosition = true;
		}
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	
	//	CCLog("cnt outer !! = %d", cnt);
	
	
 	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		if(pathFound)
			setPosition(afterPosition);
	}
	if(onceOutlineAndMapCollision)
	{
		
		if(m_scale.collisionCount == 0)
		{
			m_scale.collisionStartTime = m_scale.timer;
			
		}
		m_scale.collisionCount++;
		if(m_scale.collisionCount >= LIMIT_COLLISION_PER_SEC)
		{
			CCLog("decrese Size !!");
			setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER));
		}
	}

}

void KSCumberBase::straightMoving(float dt)
{
//	CCLog("%f %f", getPosition().x, getPosition().y);
	m_scale.timer += 1/60.f;
	
	
	if(m_scale.collisionStartTime + 1 < m_scale.timer || m_state != CUMBERSTATEMOVING)
	{
		m_scale.collisionCount = 0;
		m_scale.collisionStartTime = m_scale.timer;
		//		setCumberSize(MIN(1.0, getCumberSize() + scale.SCALE_ADDER));
	}
	CCPoint afterPosition;
	IntPoint afterPoint;
	//	int check_loop_cnt = 0;
	
	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		int changeDirection = ProbSelector::sel(0.001, 1.0 - 0.001, 0.0);
		if(changeDirection == 0)
		{
			m_directionAngleDegree += m_well512.GetValue(0, 360);
		}
	}
	
	bool validPosition = false;
	int cnt = 0;
	bool onceOutlineAndMapCollision = false;
	float degree = m_directionAngleDegree;
	bool pathFound = true; // 일단 찾았다고 셋 해놓고 특수한 경우에 false 시킴.
	while(!validPosition)
	{
		cnt++;
		float speedX = m_speed * cos(deg2Rad(degree)) * (1 + MIN(2, 0.01f*cnt));
		float speedY = m_speed * sin(deg2Rad(degree)) * (1 + MIN(2, 0.01f*cnt));
		
		CCPoint cumberPosition = getPosition();
		afterPosition = cumberPosition + ccp(speedX, speedY);
		afterPoint = ccp2ip(afterPosition);
		
		IntPoint checkPosition;
		COLLISION_CODE collisionCode = getCrashCode(afterPoint, &checkPosition);
				auto degreeSelector = [&](int cnt, float degree)->float {
					if(cnt >= 30)
					{
						validPosition = true;
						pathFound = false;
					}
					else if(cnt >= 10)
					{
						degree = m_well512.GetValue(360);
					}
					else if(cnt >= 5)
					{
						if(m_well512.GetValue(1))
						{
							degree = -degree;
						}
						else
						{
							if(m_well512.GetValue(1) == 1)
							{
								degree = m_directionAngleDegree + 90;
							}
							else
							{
								degree = m_directionAngleDegree - 90;
							}
						}
					}
					else
					{
						if(m_well512.GetValue(1) == 1)
						{
							degree = m_directionAngleDegree + 90;
						}
						else
						{
							degree = m_directionAngleDegree - 90;
						}
					}
					return degree;
				};
		
		
		if(m_state != CUMBERSTATEFURY)
		{
			if(collisionCode == kCOLLISION_JACK)
			{
				// 즉사 시킴.
				validPosition = true;
				if(myGD->getJackState() != jackStateNormal)
					myGD->communication("Jack_startDieEffect");
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				m_crashCount++;
				onceOutlineAndMapCollision = true;
				degree = degreeSelector(cnt, degree);
				
				
				if(degree < 0)			degree += 360;
				else if(degree > 360)	degree -= 360;
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				//			CCLog("collision!!");
				m_crashCount++;
				onceOutlineAndMapCollision = true;

				degree = degreeSelector(cnt, degree);
				
				
				if(degree < 0)			degree += 360;
				else if(degree > 360)	degree -= 360;
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//			CCLog("collision!!");
				//			myGD->communication("Jack_startDieEffect");
				myGD->communication("SW_createSW", checkPosition);
				degree = degreeSelector(cnt, degree);
				
				if(degree < 0)			degree += 360;
				else if(degree > 360)	degree -= 360;
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else
			{
				CCLog("what!?");
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
				m_crashCount++;
				degree = degreeSelector(cnt, degree);				
				if(degree < 0)			degree += 360;
				else if(degree > 360)	degree -= 360;
			}
			else
			{
				validPosition = true;
			}
			if(m_furyMode.furyFrameCount % 8 == 0) // n 프레임당 한번 깎음.
			{
				crashMapForPosition(afterPosition);
			}
		}
		if(cnt >= 2)
		{
			CCLog("straightMoving cnt !! = %d", cnt);
		}
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	
	m_directionAngleDegree = degree;

	//	CCLog("cnt outer !! = %d", cnt);
	
	
	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		if(pathFound)
			setPosition(afterPosition);
	}
	if(onceOutlineAndMapCollision)
	{
		
		if(m_scale.collisionCount == 0)
		{
			m_scale.collisionStartTime = m_scale.timer;
			
		}
		m_scale.collisionCount++;
		if(m_scale.collisionCount >= LIMIT_COLLISION_PER_SEC)
		{
			CCLog("decrese Size !!");
			setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER));
		}
	}
	
	
}

void KSCumberBase::followMoving(float dt)
{
	m_scale.timer += 1/60.f;
	m_follow.timer += 1 / 60.f;
	
	if(m_scale.collisionStartTime + 1 < m_scale.timer || m_state != CUMBERSTATEMOVING)
	{
		m_scale.collisionCount = 0;
		m_scale.collisionStartTime = m_scale.timer;
		//		setCumberSize(MIN(1.0, getCumberSize() + scale.SCALE_ADDER));
	}
	CCPoint afterPosition;
	IntPoint afterPoint;
	//	int check_loop_cnt = 0;
	
	float dx, dy;
	dx = dy = 0;
	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		if(m_follow.timer - m_follow.lastMapCollisionTime > 1.f)
		{
			CCPoint t = ip2ccp(myGD->getJackPoint()) - getPosition();
			dx = m_speed * cos(atan2(t.y, t.x));
			dy = m_speed * sin(atan2(t.y, t.x));
		}
		else
		{
			dx = m_speed * cos(deg2Rad(m_directionAngleDegree));
			dy = m_speed * sin(deg2Rad(m_directionAngleDegree));
		}		
	}
	
	bool validPosition = false;
	int cnt = 0;
	bool onceOutlineAndMapCollision = false;
	bool pathFound = true; // 일단 찾았다고 셋 해놓고 특수한 경우에 false 시킴.
	while(!validPosition)
	{
		cnt++;
		
		CCPoint cumberPosition = getPosition();
		
		
		afterPosition = cumberPosition + ccp(dx, dy);
		afterPoint = ccp2ip(afterPosition);
		
		IntPoint checkPosition;
		COLLISION_CODE collisionCode = getCrashCode(afterPoint, &checkPosition);
		if(m_state != CUMBERSTATEFURY)
		{
			if(collisionCode == kCOLLISION_JACK)
			{
				// 즉사 시킴.
				validPosition = true;
				if(myGD->getJackState() != jackStateNormal)
					myGD->communication("Jack_startDieEffect");
				m_follow.lastMapCollisionTime = m_follow.timer;
				m_directionAngleDegree += m_well512.GetValue(90, 360);
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				m_crashCount++;
				onceOutlineAndMapCollision = true;
				m_follow.lastMapCollisionTime = m_follow.timer;
				m_directionAngleDegree += m_well512.GetValue(90, 360);
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				m_crashCount++;
				//			CCLog("collision!!");
				onceOutlineAndMapCollision = true;
				m_follow.lastMapCollisionTime = m_follow.timer;
				m_directionAngleDegree += m_well512.GetValue(90, 360);
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);

				
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				m_follow.lastMapCollisionTime = m_follow.timer;
				m_directionAngleDegree += m_well512.GetValue(90, 360);
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
				//			CCLog("collision!!");
				//			myGD->communication("Jack_startDieEffect");
				myGD->communication("SW_createSW", checkPosition);
				//									callfuncI_selector(MetalSnake::showEmotion)); //##
				
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else
			{
				CCLog("what!?");
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
				m_crashCount++;
				//			CCLog("collision!!");
			}
			else
			{
				validPosition = true;
			}
			if(m_furyMode.furyFrameCount % 8 == 0) // n 프레임당 한번 깎음.
			{
				crashMapForPosition(afterPosition);
			}
		}
		if(cnt >= 2)
		{
			CCLog("followMoving cnt !! = %d", cnt);
		}
		if(cnt >= 30)
		{
			pathFound = false;
			validPosition = true;
		}
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	

	//	CCLog("cnt outer !! = %d", cnt);
	
	
	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		if(pathFound)
			setPosition(afterPosition);
	}
	if(onceOutlineAndMapCollision)
	{
		
		if(m_scale.collisionCount == 0)
		{
			m_scale.collisionStartTime = m_scale.timer;
			
		}
		m_scale.collisionCount++;
		if(m_scale.collisionCount >= LIMIT_COLLISION_PER_SEC)
		{
			CCLog("decrese Size !!");
			setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER));
		}
	}
}

void KSCumberBase::rightAngleMoving(float dt)
{

	m_scale.timer += 1/60.f;
	
	
	if(m_scale.collisionStartTime + 1 < m_scale.timer || m_state != CUMBERSTATEMOVING)
	{
		m_scale.collisionCount = 0;
		m_scale.collisionStartTime = m_scale.timer;
		//		setCumberSize(MIN(1.0, getCumberSize() + scale.SCALE_ADDER));
	}
	CCPoint afterPosition;
	IntPoint afterPoint;
	//	int check_loop_cnt = 0;
	
	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		int changeDirection = m_well512.GetValue(3);
		if(ProbSelector::sel(0.05, 1.0 - 0.05, 0.0) == 0)
		{
			switch (changeDirection) {
				case 0:
					m_directionAngleDegree = 90;
					break;
				case 1:
					m_directionAngleDegree = 180;
					break;
				case 2:
					m_directionAngleDegree = 270;
					break;
				case 3:
					m_directionAngleDegree = 0;
					break;
			}
		}
	}
	
	bool validPosition = false;
	int cnt = 0;
	bool onceOutlineAndMapCollision = false;
	bool pathFound = true; // 일단 찾았다고 셋 해놓고 특수한 경우에 false 시킴.
	while(!validPosition)
	{
		cnt++;
		float speedX = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + MIN(2, 0.01f*cnt));
		float speedY = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + MIN(2, 0.01f*cnt));
		
		CCPoint cumberPosition = getPosition();
		afterPosition = cumberPosition + ccp(speedX, speedY);
		afterPoint = ccp2ip(afterPosition);
		
		IntPoint checkPosition;
		COLLISION_CODE collisionCode = getCrashCode(afterPoint, &checkPosition);
		if(m_state != CUMBERSTATEFURY)
		{
			if(collisionCode == kCOLLISION_JACK)
			{
				// 즉사 시킴.
				validPosition = true;
				if(myGD->getJackState() != jackStateNormal)
					myGD->communication("Jack_startDieEffect");
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				m_crashCount++;
				onceOutlineAndMapCollision = true;
				int changeDirection = m_well512.GetValue(3);

				{
					switch (changeDirection) {
						case 0:
							m_directionAngleDegree = 90;
							break;
						case 1:
							m_directionAngleDegree = 180;
							break;
						case 2:
							m_directionAngleDegree = 270;
							break;
						case 3:
							m_directionAngleDegree = 0;
							break;
					}
				}
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				//			CCLog("collision!!");
				m_crashCount++;
				onceOutlineAndMapCollision = true;
				m_directionAngleDegree += 180;
				
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//			CCLog("collision!!");
				//			myGD->communication("Jack_startDieEffect");
				myGD->communication("SW_createSW", checkPosition);
				//									callfuncI_selector(MetalSnake::showEmotion)); //##
				
				int changeDirection = m_well512.GetValue(3);
				
				{
					switch (changeDirection) {
						case 0:
							m_directionAngleDegree = 90;
							break;
						case 1:
							m_directionAngleDegree = 180;
							break;
						case 2:
							m_directionAngleDegree = 270;
							break;
						case 3:
							m_directionAngleDegree = 0;
							break;
					}
				}
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else
			{				
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
				//			CCLog("collision!!");
				m_crashCount++;
				m_directionAngleDegree += 180;
			}
			else
			{
				validPosition = true;
			}
			if(m_furyMode.furyFrameCount % 8 == 0) // n 프레임당 한번 깎음.
			{
				crashMapForPosition(afterPosition);
			}
		}
		if(cnt >= 2)
		{
			CCLog("rightAngleMoving cnt !! = %d", cnt);
		}
		if(cnt >= 30)
		{
			pathFound = false;
			validPosition = true;
		}
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	
	//	CCLog("cnt outer !! = %d", cnt);
	
	
 	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		if(pathFound)
			setPosition(afterPosition);
	}
	if(onceOutlineAndMapCollision)
	{
		
		if(m_scale.collisionCount == 0)
		{
			m_scale.collisionStartTime = m_scale.timer;
			
		}
		m_scale.collisionCount++;
		if(m_scale.collisionCount >= LIMIT_COLLISION_PER_SEC)
		{
			CCLog("decrese Size !!");
			setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER));
		}
	}
	
}


void KSCumberBase::circleMoving(float dt)
{
	m_scale.timer += 1/60.f;
	if(m_scale.timer - m_circle.lastMovingTime >= 3/60.f) // 3 프레임 이상 차이 나면 다시 설정.
	{
		m_circle.setRelocation(getPosition(), m_well512);
	}
	
	if(m_scale.collisionStartTime + 1 < m_scale.timer || m_state != CUMBERSTATEMOVING)
	{
		m_scale.collisionCount = 0;
		m_scale.collisionStartTime = m_scale.timer;
		//		setCumberSize(MIN(1.0, getCumberSize() + scale.SCALE_ADDER));
	}
	CCPoint afterPosition;
	IntPoint afterPoint;
	//	int check_loop_cnt = 0;
	
	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		if(ProbSelector::sel(0.003, 1.0 - 0.003, 0.0) == 0)
		{
			// m_snake 변수를 재지정 ...
			
			m_circle.setRelocation(getPosition(), m_well512);
		}
	}
	
	bool validPosition = false;
	int cnt = 0;
	bool onceOutlineAndMapCollision = false;
	bool pathFound = true; // 일단 찾았다고 셋 해놓고 특수한 경우에 false 시킴.
	while(!validPosition)
	{
		cnt++;

		float circleRadius = sqrt(pow((m_circle.centerPosition.x - m_circle.relocationPosition.x), 2) +
								  pow((m_circle.centerPosition.y - m_circle.relocationPosition.y), 2));
		
		// 쿰버위치에서 센터 위치까지의 각도.
		
		float theta = atan2f(m_circle.relocationPosition.y - m_circle.centerPosition.y,
							 m_circle.relocationPosition.x - m_circle.centerPosition.x);
		
		float a = circleRadius;
		float b = circleRadius;
		afterPosition = ccp(m_circle.centerPosition.x + a * cos(theta) * cos(m_circle.angleRad) - b * sin(theta) * sin(m_circle.angleRad),
							m_circle.centerPosition.y + a * cos(m_circle.angleRad) * sin(theta) + b * sin(m_circle.angleRad) * cos(theta));
		afterPoint = ccp2ip(afterPosition);
		IntPoint checkPosition;
		COLLISION_CODE collisionCode = getCrashCode(afterPoint, &checkPosition);
		if(m_state != CUMBERSTATEFURY)
		{
			if(collisionCode == kCOLLISION_JACK)
			{
				// 즉사 시킴.
				validPosition = true;
				if(myGD->getJackState() != jackStateNormal)
					myGD->communication("Jack_startDieEffect");
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				m_crashCount++;
				onceOutlineAndMapCollision = true;
				m_circle.setRelocation(getPosition(), m_well512);
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				m_crashCount++;
				//			CCLog("collision!!");
				onceOutlineAndMapCollision = true;
				// m_circle 변수를 재지정 ...
				m_circle.setRelocation(getPosition(), m_well512);
				
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//			CCLog("collision!!");
				//			myGD->communication("Jack_startDieEffect");
				myGD->communication("SW_createSW", checkPosition);
				//									callfuncI_selector(MetalSnake::showEmotion)); //##
				
				// m_circle 변수를 재지정 ...
				m_circle.setRelocation(getPosition(), m_well512);
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else
			{
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
				m_crashCount++;
				// m_circle 변수를 재지정 ...
				m_circle.setRelocation(getPosition(), m_well512);
			}
			else
			{
				validPosition = true;
			}
			
			if(m_furyMode.furyFrameCount % 8 == 0) // n 프레임당 한번 깎음.
			{
				crashMapForPosition(afterPosition);
			}
		}
		if(cnt >= 3)
			CCLog("snakeMoving cnt !! = %d", cnt);
		if(cnt >= 30)
		{
			pathFound = false;
			validPosition = true;
		}
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	
	//	CCLog("cnt outer !! = %d", cnt);
	
	
	
 	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		if(pathFound)
		{
			float circleRadius = sqrt(pow((m_circle.centerPosition.x - m_circle.relocationPosition.x), 2) +
									  pow((m_circle.centerPosition.y - m_circle.relocationPosition.y), 2));
			m_circle.angleRad += m_speed * m_circle.sign / circleRadius;
			
			//		CCLog("%f %f", afterPosition.x, afterPosition.y);
			setPosition(afterPosition);
		}
		
	}
	if(onceOutlineAndMapCollision)
	{
		
		if(m_scale.collisionCount == 0)
		{
			m_scale.collisionStartTime = m_scale.timer;
			
		}
		m_scale.collisionCount++;
		if(m_scale.collisionCount >= LIMIT_COLLISION_PER_SEC)
		{
			CCLog("decrese Size !!");
			setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER));
		}
	}

	m_circle.lastMovingTime = m_scale.timer;
}

void KSCumberBase::snakeMoving(float dt)
{
	m_scale.timer += 1/60.f;
	if(m_scale.timer - m_snake.lastMovingTime >= 3/60.f) // 3 프레임 이상 차이 나면 다시 설정.
	{
		m_snake.setRelocation(getPosition(), m_well512);
	}
	
	if(m_scale.collisionStartTime + 1 < m_scale.timer || m_state != CUMBERSTATEMOVING)
	{
		m_scale.collisionCount = 0;
		m_scale.collisionStartTime = m_scale.timer;
		//		setCumberSize(MIN(1.0, getCumberSize() + scale.SCALE_ADDER));
	}
	CCPoint afterPosition;
	IntPoint afterPoint;
	//	int check_loop_cnt = 0;
	
	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		if(ProbSelector::sel(0.005, 1.0 - 0.005, 0.0) == 0)
		{
			// m_snake 변수를 재지정 ...
			
			m_snake.setRelocation(getPosition(), m_well512);
		}
	}
	
	bool validPosition = false;
	int cnt = 0;
	bool onceOutlineAndMapCollision = false;
	bool pathFound = true; // 일단 찾았다고 셋 해놓고 특수한 경우에 false 시킴.
	while(!validPosition)
	{
		cnt++;
		
		float circleRadius = sqrt(pow((m_snake.centerPosition.x - m_snake.relocationPosition.x), 2) +
								  pow((m_snake.centerPosition.y - m_snake.relocationPosition.y), 2));
		
		// 쿰버위치에서 센터 위치까지의 각도.
		
		float theta = atan2f(m_snake.relocationPosition.y - m_snake.centerPosition.y,
							 m_snake.relocationPosition.x - m_snake.centerPosition.x);
		
		float a = circleRadius;
		float b = a * m_snake.shortRadianRatio;
		afterPosition = ccp(m_snake.centerPosition.x + a * cos(theta) * cos(m_snake.angleRad) - b * sin(theta) * sin(m_snake.angleRad),
							m_snake.centerPosition.y + a * cos(m_snake.angleRad) * sin(theta) + b * sin(m_snake.angleRad) * cos(theta));
		afterPoint = ccp2ip(afterPosition);
		IntPoint checkPosition;
		COLLISION_CODE collisionCode = getCrashCode(afterPoint, &checkPosition);
		if(m_state != CUMBERSTATEFURY)
		{
			if(collisionCode == kCOLLISION_JACK)
			{
				// 즉사 시킴.
				validPosition = true;
				if(myGD->getJackState() != jackStateNormal)
					myGD->communication("Jack_startDieEffect");
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				m_crashCount++;
				onceOutlineAndMapCollision = true;
				m_snake.setRelocation(getPosition(), m_well512);
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				m_crashCount++;
				//			CCLog("collision!!");
				onceOutlineAndMapCollision = true;
				// m_snake 변수를 재지정 ...
				m_snake.setRelocation(getPosition(), m_well512);
				
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//			CCLog("collision!!");
				//			myGD->communication("Jack_startDieEffect");
				myGD->communication("SW_createSW", checkPosition);
				//									callfuncI_selector(MetalSnake::showEmotion)); //##
				
				// m_snake 변수를 재지정 ...
				m_snake.setRelocation(getPosition(), m_well512);
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else
			{
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
				m_crashCount++;
				// m_snake 변수를 재지정 ...
				m_snake.setRelocation(getPosition(), m_well512);
			}
			else
			{
				validPosition = true;
			}
			
			if(m_furyMode.furyFrameCount % 8 == 0) // n 프레임당 한번 깎음.
			{
				crashMapForPosition(afterPosition);
			}
		}
		if(cnt >= 3)
			CCLog("circleMoving cnt !! = %d", cnt);
		if(cnt >= 30)
		{
			pathFound = false;
			validPosition = true;
		}
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	
	//	CCLog("cnt outer !! = %d", cnt);
	
	
	
 	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		if(pathFound)
		{
			float circleRadius = sqrt(pow((m_snake.centerPosition.x - m_snake.relocationPosition.x), 2) +
									  pow((m_snake.centerPosition.y - m_snake.relocationPosition.y), 2));
			m_snake.angleRad += m_speed * m_snake.sign / circleRadius;
			
			//		CCLog("%f %f", afterPosition.x, afterPosition.y);
			setPosition(afterPosition);
		}
		
	}
	if(onceOutlineAndMapCollision)
	{
		if(m_scale.collisionCount == 0)
		{
			m_scale.collisionStartTime = m_scale.timer;
			
		}
		m_scale.collisionCount++;
		if(m_scale.collisionCount >= LIMIT_COLLISION_PER_SEC)
		{
			CCLog("decrese Size !!");
			setCumberScale(MAX(m_minScale, getCumberScale() - m_scale.SCALE_SUBER));
		}
	}
	
	m_snake.lastMovingTime = m_scale.timer;
}

void KSCumberBase::cumberAttack(float dt)
{
//	myJack->get
	float gainPercent = myGD->Fcommunication("UI_getMapPercentage") * 100.f;
	float distance = ccpLength(ip2ccp(myGD->getJackPoint()) - getPosition());
//	CCLog("%f %f %d", distance, gainPercent, m_crashCount);
	bool crashAttack = false;
	
	
	
	if(m_furyRule.gainPercent < gainPercent && distance > m_furyRule.userDistance)
	{
		float w = ProbSelector::sel(m_furyRule.percent / 100.f, 1.0 - m_furyRule.percent / 100.f, 0.0);
		if(w == 0)
		{
			crashAttack = true;
		}
	}
	
	if(m_crashCount > m_furyRule.gtCount && m_furyRule.ltPercent > gainPercent)
	{
		m_crashCount = 0;
		crashAttack = true;
	}
	
	std::vector<Json::Value> selectedAttacks;
	float exeProb;
	if(crashAttack)
	{
		for(auto iter = m_attacks.begin(); iter != m_attacks.end(); ++iter)
		{
			if( (*iter)["atype"].asString() == "crash" )
			{
				selectedAttacks.push_back(*iter);
			}
		}
	}
	else
	{
		selectedAttacks.assign(m_attacks.begin(), m_attacks.end());
	}
	if(crashAttack)
	{
		exeProb = 0; // 무조건 실행.
	}
	else
	{
		auto ps = ProbSelector({m_attackPercent / 100.f, 1.0 - m_attackPercent / 100.f});
//		exeProb = ProbSelector::sel(m_attackPercent / 100.f, 1.0 - m_attackPercent / 100.f, 0.0);
		exeProb = ps.getResult();
	}

	// 1% 확률로.
	if(exeProb == 0 && m_state == CUMBERSTATEMOVING && !selectedAttacks.empty())
	{
		Json::Value attackCode;
		bool searched = false;
		int searchCount = 0;
		while(!searched)
		{
			searchCount++;
			int idx = m_well512.GetValue(selectedAttacks.size() - 1);
			
			attackCode = selectedAttacks[idx];
			
			searched = true;
			
			if(attackCode["pattern"].asString() == "1008" && m_invisible.startInvisibleScheduler)
				searched = false;
			if(attackCode["pattern"].asString() == "109" && m_state == CUMBERSTATEFURY)
				searched = false;
			if(searchCount >= 30)
			{
				searched = false;
				break;
			}
		}
		
		if(searched)
		{
			KS::KSLog("%", attackCode);
			Json::FastWriter fw;
			std::string patternData = fw.write(attackCode);
			if(attackCode["pattern"].asString() == "109") // fury
			{
				m_state = CUMBERSTATESTOP;
				attackBehavior(attackCode);
				
				myGD->communication("MP_attackWithKSCode", getPosition(), patternData, this, true);
			}
			else
			{
				int ret = myGD->communication("MP_attackWithKSCode", getPosition(), patternData, this, true);
				if(ret == 1)
				{
					attackBehavior(attackCode);
				}
			}
		}
		
	}

}

void KSCumberBase::speedAdjustment(float dt)
{
//	m_speed.step();
	float t = (m_maxSpeed - m_minSpeed) * 0.0005f;
	m_speed = MIN(m_maxSpeed, m_speed + t);
}

bool KSCumberBase::startDamageReaction(float damage, float angle)
{
	float t = (m_maxSpeed - m_minSpeed) * 0.3f;
	m_speed = MAX(m_speed - t, m_minSpeed);
//	m_speed.init(m_speed, to, 0.1f);
	return true; // 의미없음.
}


void KSCumberBase::bossDieBomb(float dt)
{
	m_bossDie.m_bossDieFrameCount++;
	int maxValue = *max_element(m_bossDie.m_bossDieBombFrameNumbers.begin(), m_bossDie.m_bossDieBombFrameNumbers.end());
	if(find(m_bossDie.m_bossDieBombFrameNumbers.begin(), m_bossDie.m_bossDieBombFrameNumbers.end(), m_bossDie.m_bossDieFrameCount)
		 != m_bossDie.m_bossDieBombFrameNumbers.end())
	{
		auto ret = KS::loadCCBI<CCSprite*>(this, "fx_bossbomb.ccbi");
		
		CCPoint t = getPosition();
		t.x += m_well512.GetFloatValue(-100.f, 100.f);
		t.y += m_well512.GetFloatValue(-100.f, 100.f);
		ret.first->setPosition(t);
		addChild(ret.first, 11);
		
		if(maxValue == m_bossDie.m_bossDieFrameCount)
		{
			auto ret = KS::loadCCBI<CCSprite*>(this, "fx_bossdie.ccbi");
			
			
			CCPoint t = getPosition();
			ret.first->setPosition(t);
			CCLog("bossposition2 %f %f", t.x, t.y);
			addChild(ret.first, 11);
		}
	}
}


