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
		myGD->communication("EP_crashed");
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
				if(gameData->getJackState() != jackStateNormal)
					gameData->communication("Jack_startDieEffect");
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
				//			gameData->communication("Jack_startDieEffect");
				gameData->communication("SW_createSW", checkPosition, 0, 0);
				//									callfuncI_selector(MetalSnake::showEmotion)); //##
				
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else if(afterPoint.isInnerMap())
			{
				CCAssert(false, "");
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
				if(cnt % 100 == 0)
		{
			CCLog("cnt !! = %d", cnt);
		}
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	
	//	CCLog("cnt outer !! = %d", cnt);
	
	
 	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
		setPosition(afterPosition);
	
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
			if(cnt >= 10)
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
				if(gameData->getJackState() != jackStateNormal)
					gameData->communication("Jack_startDieEffect");
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				onceOutlineAndMapCollision = true;
				degree = degreeSelector(cnt, degree);
				
				
				if(degree < 0)			degree += 360;
				else if(degree > 360)	degree -= 360;
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				//			CCLog("collision!!");
				onceOutlineAndMapCollision = true;

				degree = degreeSelector(cnt, degree);
				
				
				if(degree < 0)			degree += 360;
				else if(degree > 360)	degree -= 360;
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//			CCLog("collision!!");
				//			gameData->communication("Jack_startDieEffect");
				gameData->communication("SW_createSW", checkPosition, 0, 0);
				//									callfuncI_selector(MetalSnake::showEmotion)); //##
				degree = degreeSelector(cnt, degree);
				
				if(degree < 0)			degree += 360;
				else if(degree > 360)	degree -= 360;
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else if(afterPoint.isInnerMap())
			{
				CCAssert(false, "");
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
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
		setPosition(afterPosition);
	
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
				if(gameData->getJackState() != jackStateNormal)
					gameData->communication("Jack_startDieEffect");
				m_follow.lastMapCollisionTime = m_follow.timer;
				m_directionAngleDegree += m_well512.GetValue(90, 360);
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				onceOutlineAndMapCollision = true;
				m_follow.lastMapCollisionTime = m_follow.timer;
				m_directionAngleDegree += m_well512.GetValue(90, 360);
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + 0.01f*cnt);
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
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
				//			gameData->communication("Jack_startDieEffect");
				gameData->communication("SW_createSW", checkPosition, 0, 0);
				//									callfuncI_selector(MetalSnake::showEmotion)); //##
				
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else if(afterPoint.isInnerMap())
			{
				CCAssert(false, "");
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
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
		if(cnt % 100 == 0)
		{
			CCLog("followMoving cnt !! = %d", cnt);
		}
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	

	//	CCLog("cnt outer !! = %d", cnt);
	
	
	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
		setPosition(afterPosition);
	
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
				// 즉사 시킴.
				validPosition = true;
				if(gameData->getJackState() != jackStateNormal)
					gameData->communication("Jack_startDieEffect");
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
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
				onceOutlineAndMapCollision = true;
				m_directionAngleDegree += 180;
				
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//			CCLog("collision!!");
				//			gameData->communication("Jack_startDieEffect");
				gameData->communication("SW_createSW", checkPosition, 0, 0);
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
			else if(afterPoint.isInnerMap())
			{
				CCAssert(false, "");
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
				//			CCLog("collision!!");
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
		if(cnt % 100 == 0)
		{
			CCLog("rightAngleMoving cnt !! = %d", cnt);
		}
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	
	//	CCLog("cnt outer !! = %d", cnt);
	
	
 	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
		setPosition(afterPosition);
	
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
		if(ProbSelector::sel(0.005, 1.0 - 0.005, 0.0) == 0)
		{
			// m_circle 변수를 재지정 ...
			
			m_circle.setRelocation(getPosition(), m_well512);
		}
	}
	
	bool validPosition = false;
	int cnt = 0;
	bool onceOutlineAndMapCollision = false;
	while(!validPosition)
	{
		cnt++;

		float circleRadius = sqrt(pow((m_circle.centerPosition.x - m_circle.relocationPosition.x), 2) +
								  pow((m_circle.centerPosition.y - m_circle.relocationPosition.y), 2));
		
		
		afterPosition = ccp(m_circle.centerPosition.x + circleRadius * (1) * cos(m_circle.angleRad),
							m_circle.centerPosition.y + circleRadius * (1) * sin(m_circle.angleRad));
		afterPoint = ccp2ip(afterPosition);
		IntPoint checkPosition;
		COLLISION_CODE collisionCode = getCrashCode(afterPoint, &checkPosition);
		if(m_state != CUMBERSTATEFURY)
		{
			if(collisionCode == kCOLLISION_JACK)
			{
				// 즉사 시킴.
				validPosition = true;
				if(gameData->getJackState() != jackStateNormal)
					gameData->communication("Jack_startDieEffect");
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				onceOutlineAndMapCollision = true;
				m_circle.setRelocation(getPosition(), m_well512);
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				//			CCLog("collision!!");
				onceOutlineAndMapCollision = true;
				// m_circle 변수를 재지정 ...
				m_circle.setRelocation(getPosition(), m_well512);
				
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//			CCLog("collision!!");
				//			gameData->communication("Jack_startDieEffect");
				gameData->communication("SW_createSW", checkPosition, 0, 0);
				//									callfuncI_selector(MetalSnake::showEmotion)); //##
				
				// m_circle 변수를 재지정 ...
				m_circle.setRelocation(getPosition(), m_well512);
			}
			
			else if(collisionCode == kCOLLISION_NONE)
			{
				validPosition = true;
			}
			else if(afterPoint.isInnerMap())
			{
				CCAssert(false, "");
				validPosition = true;
			}
		}
		else
		{
			if(collisionCode == kCOLLISION_OUTLINE)
			{
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
			CCLog("circleMoving cnt !! = %d", cnt);
		if(m_state != CUMBERSTATEMOVING && m_state != CUMBERSTATEFURY)
		{
			validPosition = true;
		}
	}
	
	//	CCLog("cnt outer !! = %d", cnt);
	
	
	
 	if(m_state == CUMBERSTATEMOVING || m_state == CUMBERSTATEFURY)
	{
		float circleRadius = sqrt(pow((m_circle.centerPosition.x - m_circle.relocationPosition.x), 2) +
								  pow((m_circle.centerPosition.y - m_circle.relocationPosition.y), 2));
		m_circle.angleRad += m_speed * m_circle.sign / circleRadius;
//		CCLog("%f %f", afterPosition.x, afterPosition.y);
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

	m_circle.lastMovingTime = m_scale.timer;
}
