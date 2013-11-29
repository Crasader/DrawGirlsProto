//
//  KSCumberBase.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#include "KSCumberBase.h"
#include "Jack.h"
#include "PlayUI.h"
#include <chrono>


void KSCumberBase::crashMapForIntPoint( IntPoint t_p )
{
	FixedSizeDeque<int> wee;
	std::queue<int> wee2;
	
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
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		//			if(isFuryMode) //##
		//				furyModeOff();
	}

	if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
	{
		//					myGD->communication("PM_pathChainBomb", t_p);

		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
					myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
				//			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
					myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
				//			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
					myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
				//			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
					myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
				//			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
					myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
				//			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
					myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
				//			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
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
	
	if(m_slience) // 침묵 상태면
	{
		return;
	}
	float gainPercent = myGD->Fcommunication("UI_getMapPercentage") * 100.f;
	float distance = ccpLength(ip2ccp(myGD->getJackPoint()) - getPosition());
//	CCLog("%f %f %d", distance, gainPercent, m_crashCount);
	bool crashAttack = false;

	if(m_furyRule.gainPercent < gainPercent && distance > m_furyRule.userDistance)
	{
		float w = ProbSelector::sel(m_furyRule.percent / 100.f, 1.0f - m_furyRule.percent / 100.f, 0.0);
		if(w == 0)
		{
			crashAttack = true;
		}
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
		auto end = chrono::system_clock::now();
		auto currentSecond = chrono::system_clock::to_time_t(end);
		
		if(m_attacks.size() == 1) // 하나면 그냥 넣음...
		{
			selectedAttacks.assign(m_attacks.begin(), m_attacks.end());
		}
		else for(auto iter = m_attacks.begin(); iter != m_attacks.end(); ++iter)
		{
			// 안넣어져야할 조건
			// 같은 패턴이 3초내 발동되면 해당패턴은 안넣음.
			if( (*iter)["pattern"].asString() == m_lastPattern.exePattern &&
				 currentSecond <= m_lastPattern.exeTime + 3)
			{
				
			}
			else // 넣어져야할 조건
			{
				selectedAttacks.push_back(*iter);
			}
		}
	}
	
	
	if(crashAttack)
	{
		exeProb = 0; // 무조건 실행.
	}
	else
	{
		const float originalAttackProb = m_attackPercent / 100;
		float attackProb = originalAttackProb;
		
		// 선을 긋고 있을 땐 공격확률 높임
		// 많이 맞았으면 공격확률 높임.
		if(myGD->getJackState() == jackStateDrawing ||  getLife() / getTotalLife() <= 0.3f)
		{
			attackProb += aiProbAdder();
		}
		auto ps = ProbSelector({attackProb, 1.0 - attackProb});

		exeProb = ps.getResult();
	}
	
	{
		IntPoint point = ccp2ip(getPosition());
		IntPoint afterPoint = point;
		float radius = 40.f;

		float half_distance = radius*getCumberScale(); // 20.f : radius for base scale 1.f
		int ip_half_distance = half_distance;
		int outlineCount = 0;

		for(int i=afterPoint.x-ip_half_distance;i<=afterPoint.x+ip_half_distance;i++)
		{
			for(int j=afterPoint.y-ip_half_distance;j<=afterPoint.y+ip_half_distance;j++)
			{
				float calc_distance = sqrtf(powf((afterPoint.x - i)*1,2) + powf((afterPoint.y - j)*1, 2));
				if(calc_distance < ip_half_distance)
				{
					if(IntPoint(i, j).isInnerMap() && myGD->mapState[i][j] == mapOldline)
					{
						outlineCount++;
					}
				}
			}
		}
		int s = 6; // 3초 동안
		outlineCountRatio.push_back(outlineCount);
		if(outlineCountRatio.size() > 60 * s)
		{
			outlineCountRatio.pop_front();
		}

	}
	// 확률로.
	
	if(exeProb == 0 && m_state == CUMBERSTATEMOVING)
	{
		// 부수기 공격이 시행됐는데, 크래시 공격이 없다면 텔포 해야됨
		if(crashAttack && selectedAttacks.empty())
		{
			
		}
		if(!selectedAttacks.empty())
		{
			Json::Value attackCode;
			bool searched = false;
			int searchCount = 0;
			
			// 갇힌것 판단하고 갇혔다고 판단되면 ai수치에 따라 부수기 확률을 증가시킴.
			
			ProbSelector probSel;
			
			// 바깥쪽으로 얼마나 먹었는지를...
			int externalOutlineCount = 0;
			for(int x=mapLoopRange::mapWidthInnerBegin; x != mapLoopRange::mapWidthInnerEnd; x++)
			{
				if(myGD->mapState[x][mapLoopRange::mapHeightInnerBegin] == mapOldline)
					externalOutlineCount++;
				if(myGD->mapState[x][mapLoopRange::mapHeightInnerEnd-1] == mapOldline)
					externalOutlineCount++;
			}
			for(int y=mapLoopRange::mapHeightInnerBegin; y != mapLoopRange::mapHeightInnerEnd; y++)
			{
				if(myGD->mapState[mapLoopRange::mapWidthInnerBegin][y] == mapOldline)
					externalOutlineCount++;
				if(myGD->mapState[mapLoopRange::mapWidthInnerEnd - 1][y] == mapOldline)
					externalOutlineCount++;
			}
			
			
			
			float notCrashSum = 0; // 크래시가 아닌것의 합
			int crashNumber = 0;   // 부수기 공격의 개수
			bool bossIsClosed_ = bossIsClosed();
			for(auto& i : selectedAttacks)
			{
				if(i["atype"].asString() != "crash")
				{
					notCrashSum += i["percent"].asDouble();
				}
				else
				{
					crashNumber++;
				}
			}
			float patternMax = 2 * notCrashSum / crashNumber;
			for(auto& i : selectedAttacks)
			{
				KS::KSLog("% percent!!!", i["percent"].asDouble());
				if(i["pattern"].asString() == "1017")
				{
					if(externalOutlineCount >= 150) // 가장자리 위주로 먹었다면...
					{
						float notAlongSum = 0;
						for(auto& i : selectedAttacks)
						{
							if(i["pattern"].asString() != "1017")
							{
								notAlongSum += i["percent"].asDouble();
							}
						}
						float alongMin = i["percent"].asDouble();
						float alongMax = notAlongSum / 1.5f;
						if(alongMax >= alongMin)
						{
							probSel.pushProb(i["percent"].asDouble() + (alongMax - i["percent"].asDouble()) * getAiValue() / 100.f);
						}
						else
						{
							probSel.pushProb(i["percent"].asDouble() * (1 + getAiValue() / 100.f));
						}
					}
					else
					{
						probSel.pushProb(i["percent"].asDouble());
					}
				}
				else if(i["atype"].asString() == "crash")
				{
					float patternMin = i["percent"].asDouble();
					if(bossIsClosed_) // 보스가 갇힘.
					{
						if(patternMax >= patternMin)
						{
							probSel.pushProb(i["percent"].asDouble() + (patternMax - i["percent"].asDouble()) * getAiValue() / 100.f);
						}
						else
						{
							probSel.pushProb(i["percent"].asDouble() * (1 + getAiValue() / 100.f));
						}
					}
					else
						probSel.pushProb(i["percent"].asDouble());
				}
				else
				{
					probSel.pushProb(i["percent"].asDouble());
				}
			}
			CCLog("externalCnt %d", externalOutlineCount);
			while(!searched)
			{
				searchCount++;
				//			int idx = m_well512.GetValue(selectedAttacks.size() - 1);
				int idx = probSel.getResult();
				
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
	int considerFrames = 60 * 7; // 초.
	int maxCre = 15;
	int cntPerSecond = MIN(count_if(m_damagedFrames.getSTL().begin(), m_damagedFrames.getSTL().end(),
															[=](int w){
																return w >= m_frameCount - considerFrames;
															}), maxCre);
	float baseSpeed = m_minSpeed + (m_startSpeed - m_minSpeed) * getLife() / getTotalLife();
	float finalSpeed = baseSpeed + cntPerSecond * (m_maxSpeed - baseSpeed) / maxCre;
	m_speed = finalSpeed;
}

void KSCumberBase::selfHealing(float dt)
{
	m_healingFrameCount++;
	
	// 5초마다 한번씩, 현재 라이프에서 n% 만큼 더함.
	if(m_healingFrameCount >= 60*5)
	{
		float n = 5.f;
		float adder = getTotalLife() * n / 100.f;
		setLife(MIN(getTotalLife(), getLife() + adder));
		m_healingFrameCount = 0;
	}
}
bool KSCumberBase::startDamageReaction(float damage, float angle)
{
	m_damagedFrames.push_back(m_frameCount);
//	float t = (m_maxSpeed - m_minSpeed) * 0.3f;
//	m_speed = MAX(m_speed - t, m_minSpeed);
//	m_speed.init(m_speed, to, 0.1f);
	return true; // 의미없음.
}

void KSCumberBase::onJackDie()
{
		unschedule(schedule_selector(ThisClassType::cumberAttack));
}
void KSCumberBase::onJackRevived()
{
	schedule(schedule_selector(ThisClassType::cumberAttack));
	
}
void KSCumberBase::bossDieBomb(float dt)
{
	m_bossDie.m_bossDieFrameCount++;
	int maxValue = *max_element(m_bossDie.m_bossDieBombFrameNumbers.begin(), m_bossDie.m_bossDieBombFrameNumbers.end());
	if(find(m_bossDie.m_bossDieBombFrameNumbers.begin(), m_bossDie.m_bossDieBombFrameNumbers.end(), m_bossDie.m_bossDieFrameCount)
		 != m_bossDie.m_bossDieBombFrameNumbers.end())
	{
		auto ret = KS::loadCCBI<CCSprite*>(this, "fx_bomb5.ccbi");
		CCPoint t = getPosition();
		t.x += m_well512.GetFloatValue(-100.f, 100.f);
		t.y += m_well512.GetFloatValue(-100.f, 100.f);
		ret.first->setPosition(t);
		getParent()->addChild(ret.first, 11);
		
		if(maxValue == m_bossDie.m_bossDieFrameCount)
		{
			auto ret = KS::loadCCBI<CCSprite*>(this, "fx_boss_die.ccbi");
			
			
			CCPoint t = getPosition();
			ret.first->setPosition(t);
			getParent()->addChild(ret.first, 11);
		}
	}
}


void KSCumberBase::getRandomPositionToJack(IntPoint* ip, bool* finded)
{
//	chrono::time_point<chrono::system_clock> start, end;
//	start = chrono::system_clock::now();
	//	chrono::duration<double> elapsed_seconds;
	//	start = chrono::system_clock::now();
	bool isGoodPointed = false;
	IntPoint jackPoint = myGD->getJackPoint();
//	auto comp1 = [jackPoint](const IntPoint& a, const IntPoint& b ) {
//		auto _a = (jackPoint - a).length();
//		auto _b = (jackPoint - b).length();
//		if(_a < 50)
//			return false;
////		if(_b < 50)
////			_b = 99999999;
//		return _a > _b;
//	};
//	priority_queue<IntPoint, std::vector<IntPoint>, decltype(comp1) > shuffledPositions(comp1);
//	std::vector<IntPoint>
//	priority_queue<IntPoint> shuffledPositions;
	IntPoint mapPoint;
	vector<IntPoint> shuffledPositions;
	for(int x = 1; x <= mapLoopRange::mapWidthInnerEnd - 1; x+=5)
	{
		for(int y = 1; y <= mapLoopRange::mapHeightInnerEnd - 1; y+=5)
		{
			if(myGD->mapState[x][y] == mapEmpty)
				shuffledPositions.push_back(IntPoint(x, y));
		}
	}
//	end = chrono::system_clock::now();
//	KS::KSLog("s1 %", chrono::duration_cast<chrono::milliseconds>(end - start).count());
//	start = chrono::system_clock::now();
	std::sort(shuffledPositions.begin(), shuffledPositions.end(), [jackPoint](const IntPoint& a, const IntPoint& b ) {
				auto _a = (jackPoint - a).length();
				auto _b = (jackPoint - b).length();
		
				if(_a < 35)
					return false;
		////		if(_b < 50)
		////			_b = 99999999;
		return _a < _b;
	});
	
//	end = chrono::system_clock::now();
//	KS::KSLog("s2 %", chrono::duration_cast<chrono::milliseconds>(end - start).count());
//	random_shuffle(shuffledPositions.begin(), shuffledPositions.end(), [=](int n){
//		return this->m_well512.GetValue(n-1);
//	});
	int retryLimit = 5;
	auto copiedObject = shuffledPositions;
	
	while(retryLimit)
	{
		for(const auto& mp : shuffledPositions)
//		while(copiedObject.empty() == false)
		{
//			auto mp = copiedObject.top();
//			copiedObject.pop();
		
			mapPoint = mp;
			
			float myScale = getCumberScale();
			if(mapPoint.isInnerMap() && myGD->mapState[mapPoint.x][mapPoint.y] == mapEmpty)
			{
				float half_distance = getRadius()*myScale; // 20.f : radius for base scale 1.f
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
					break;
				}
			}
		}
		
		if(isGoodPointed == false)
		{
			retryLimit--; // retryLimit 만큼 스케일을 줄여서 탐색해봄.
			m_scale.scale.init(m_scale.scale.getValue() / 2, m_scale.scale.getValue() / 2, 0.f);
		}
		else
			break;
	}
	
	
	if(isGoodPointed == true)
	{
		*ip = mapPoint;
		*finded = true;
		//		CCLog("map point %d %d", mapPoint.x, mapPoint.y);
		//		CCLog("scale %f", m_headImg->getScale());
		//		CCScaleTo* t_scale = CCScaleTo::create(0.5f, m_scale);
		//		m_headImg->runAction(t_scale);
		//		myGD->setMainCumberPoint(mapPoint);
		//
		//		setPosition(ccp((mapPoint.x-1)*pixelSize + 1,(mapPoint.y-1)*pixelSize + 1));
	}
	else
	{
		*finded = false;
		// nothing.
		CCAssert(false, "");
	}

}
void KSCumberBase::getRandomPosition(IntPoint* ip, bool* finded)
{
	bool isGoodPointed = false;
	
	IntPoint mapPoint;
	vector<IntPoint> shuffledPositions;
	for(int x = 1; x <= mapLoopRange::mapWidthInnerEnd - 1; x++)
	{
		for(int y = 1; y <= mapLoopRange::mapHeightInnerEnd - 1; y++)
		{
			shuffledPositions.push_back(IntPoint(x, y));
		}
	}
	
	random_shuffle(shuffledPositions.begin(), shuffledPositions.end(), [=](int n){
		return this->m_well512.GetValue(n-1);
	});
	int retryLimit = 5;
	while(retryLimit)
	{
		for(auto& mp : shuffledPositions)
		{
			mapPoint = mp;
			
			float myScale = getCumberScale();
			if(mapPoint.isInnerMap() && myGD->mapState[mapPoint.x][mapPoint.y] == mapEmpty)
			{
				float half_distance = getRadius()*myScale; // 20.f : radius for base scale 1.f
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
					break;
				}
			}
		}
		
		if(isGoodPointed == false)
		{
			retryLimit--; // retryLimit 만큼 스케일을 줄여서 탐색해봄.
			m_scale.scale.init(m_scale.scale.getValue() / 2, m_scale.scale.getValue() / 2, 0.f);
		}
		else
			break;
	}
	
	
	if(isGoodPointed == true)
	{
		*ip = mapPoint;
		*finded = true;
		//		CCLog("map point %d %d", mapPoint.x, mapPoint.y);
		//		CCLog("scale %f", m_headImg->getScale());
		//		CCScaleTo* t_scale = CCScaleTo::create(0.5f, m_scale);
		//		m_headImg->runAction(t_scale);
		//		myGD->setMainCumberPoint(mapPoint);
		//
		//		setPosition(ccp((mapPoint.x-1)*pixelSize + 1,(mapPoint.y-1)*pixelSize + 1));
	}
	else
	{
		*finded = false;
		// nothing.
		CCAssert(false, "");
	}
}

void KSCumberBase::onJackDrawLine()
{
	ProbSelector ps({getAiValue(), 100.f - getAiValue()});
	int r = ps.getResult();
	if(r == 0)
	{
		m_drawMovement = FOLLOW_TYPE;
	}
	else
	{
		m_drawMovement = m_normalMovement;
	}
}

bool KSCumberBase::init()
{
	CCNode::init();
	//		mEmotion = NULL;
	schedule(schedule_selector(ThisClassType::speedAdjustment));

	// 미션에 따라 on/off 해야됨.
	schedule(schedule_selector(ThisClassType::selfHealing));
	schedule(schedule_selector(ThisClassType::cumberFrame));

	return true;
}

void KSCumberBase::startMoving()
{
	m_state = CUMBERSTATEMOVING;
}

void KSCumberBase::stopMoving()
{
	//		unschedule(schedule_selector(KSCumberBase::movingAndCrash));
	//		schedule(schedule_selector(KSCumberBase::processCrash));
	//		schedule(crash)
	onStopMoving();
}

void KSCumberBase::setCumberState( int e )
{
	m_state = (CUMBER_STATE)e;
}

CUMBER_STATE KSCumberBase::getCumberState()
{
	return m_state;
}

void KSCumberBase::resetCastingCancelCount()
{
	m_castingCancelCount = 0;
}

int KSCumberBase::getCastingCancelCount()
{
	return m_castingCancelCount;
}

void KSCumberBase::setGameover()
{
	m_state = CUMBERSTATEGAMEOVER;

	//		m_scale.scale.init(m_scale.scale.getValue(), 0.f, 0.03f);
	//		runAction(CCScaleTo::create(2.f, 0.01f));
	m_minScale = 0.f;
	m_bossDie.m_bossDieBombFrameNumbers.push_back(m_well512.GetValue(0, 30));
	m_bossDie.m_bossDieBombFrameNumbers.push_back(m_well512.GetValue(30, 60));
	m_bossDie.m_bossDieBombFrameNumbers.push_back(m_well512.GetValue(60, 90));
	m_bossDie.m_bossDieFrameCount = 0;
	unschedule(schedule_selector(ThisClassType::cumberAttack));
	schedule(schedule_selector(ThisClassType::bossDieBomb));
	//		int number = m_well512.GetValue(3, 4);
	//		for(int i=0; i<number; i++)
	//		{
	//			scheduleOnce(schedule_selector(ThisClassType::bossDieBomb), m_well512.GetFloatValue(0.3f, 1.f));
	//		}

	this->setVisible(false);
}

void KSCumberBase::movingAndCrash( float dt )
{
	IntPoint mapPoint = m_mapPoint;

	// 갇혀있는지 검사함. 갇혀있으면 없앰.
	if(myGD->mapState[mapPoint.x][mapPoint.y] != mapEmpty &&
		myGD->mapState[mapPoint.x-1][mapPoint.y] != mapEmpty &&
		myGD->mapState[mapPoint.x+1][mapPoint.y] != mapEmpty &&
		myGD->mapState[mapPoint.x][mapPoint.y-1] != mapEmpty &&
		myGD->mapState[mapPoint.x][mapPoint.y+1] != mapEmpty)
	{
		AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
		int missile_type = rand()%7 + (rand()%9)*10;

		int rmCnt = 5;
		float missile_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
		myGD->communication("MP_createJackMissile", missile_type, rmCnt, missile_speed);

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

	if(m_state == CUMBERSTATEFURY)
	{
		m_furyMode.furyFrameCount++;
	}	

	auto movingBranch = [&](MOVEMENT movement)
	{
		switch(movement)
		{
		case STRAIGHT_TYPE:
			straightMoving(dt);
			break;
		case RANDOM_TYPE:
			randomMoving(dt);
			break;
		case FOLLOW_TYPE:
			followMoving(dt);
			break;
		case RIGHTANGLE_TYPE:
			rightAngleMoving(dt);
			break;
		case CIRCLE_TYPE:
			circleMoving(dt);
			break;
		case SNAKE_TYPE:
			snakeMoving(dt);
		}
	};

	if(m_state == CUMBERSTATEFURY)
	{
		movingBranch(m_furyMovement);
	}
	else
	{
		if(myGD->getJackState() == jackStateNormal)
		{
			movingBranch(m_normalMovement);
		}
		else
		{
			movingBranch(m_drawMovement);
		}
	}
}

void KSCumberBase::cumberFrame( float dt )
{
	m_frameCount++; // 쿰버의 프레임수를 잼.
}

void KSCumberBase::onStartGame()
{
	m_isStarted = true;
	schedule(schedule_selector(ThisClassType::cumberAttack));
}

void KSCumberBase::lightSmaller()
{
	addChild(KSTimer::create
					 (0.5f, [=](){
		this->endTeleport();
	}));
}

void KSCumberBase::endTeleport()
{
	startMoving();
	myGD->communication("CP_onPatternEnd");
}

void KSCumberBase::startTeleport()
{
	smaller();

	AudioEngine::sharedInstance()->playEffect("sound_teleport.mp3",false);
}

void KSCumberBase::smaller()
{
	addChild(KSGradualValue<float>::create(0.f, 30.f, 0.5f,
																				 [=](float t)
																				 {
																					 if((int)t % 4 == 0)
																					 {
																						 if((int)t % 8 == 0)
																						 {
																							 this->setVisible(true);
																						 }
																						 else
																						 {
																							 this->setVisible(false);
																						 }
																					 }
																				 },
																				 [=](float t)
																				 {
																					 this->setVisible(true);
																					 this->randomPosition();
																				 }));
}

COLLISION_CODE KSCumberBase::crashWithX( IntPoint check_position )
{
	if(check_position.x < mapLoopRange::mapWidthInnerBegin || check_position.x >= mapLoopRange::mapWidthInnerEnd ||
		check_position.y < mapLoopRange::mapHeightInnerBegin || check_position.y >= mapLoopRange::mapHeightInnerEnd ||
		myGD->mapState[check_position.x][check_position.y] == mapType::mapOutline)
	{
		// 나갔을 시.
		return COLLISION_CODE::kCOLLISION_OUTLINE;
	}

	// 이미 그려진 곳에 충돌했을 경우.
	if(myGD->mapState[check_position.x][check_position.y] == mapOldline ||
		myGD->mapState[check_position.x][check_position.y] == mapOldget)
	{
		return COLLISION_CODE::kCOLLISION_MAP;
	}

	if(myGD->mapState[check_position.x][check_position.y] == mapNewline)
	{
		return COLLISION_CODE::kCOLLISION_NEWLINE;
	}
	IntPoint jackPoint = myGD->getJackPoint();
	if(jackPoint.x == check_position.x && jackPoint.y == check_position.y)
	{
		return COLLISION_CODE::kCOLLISION_JACK;
	}
	return COLLISION_CODE::kCOLLISION_NONE;
}

void KSCumberBase::setCumberScale( float r )
{
	m_scale.scale.init(m_scale.scale.getValue(), r, 0.005f);
}

float KSCumberBase::getCumberScale()
{
	return m_scale.scale.getValue();
}

void KSCumberBase::onCanceledCasting()
{
	m_castingCancelCount++;
}

void KSCumberBase::settingScale( float startScale, float minScale, float maxScale )
{
	m_startScale = startScale;
	m_minScale = minScale;
	m_maxScale = maxScale;

	m_scale.SCALE_ADDER = m_scale.SCALE_SUBER = (m_maxScale - m_minScale) / 5.f;
	m_scale.scale.init(m_startScale, m_startScale, 0.f);
}

void KSCumberBase::settingFuryRule()
{
	m_furyRule.gainPercent = 40; //fury["gainpercent"].asDouble();
	m_furyRule.userDistance = 300; // fury["userdistance"].asDouble();
	m_furyRule.percent = aiProbAdder();// fury["percent"].asDouble();
	//		m_furyRule
}

void KSCumberBase::settingAI( int ai )
{
	int autobalanceTry = NSDS_GI(mySD->getSilType(), kSDS_SI_autoBalanceTry_i);
	int balanceN = 10;
	float downLimit = 0.5f;
//
	ostringstream oss;
	oss << mySD->getSilType();
	std::string playcountKey = std::string("playcount_") + oss.str();
	int playCount = myDSH->getUserIntForStr(playcountKey, 0);
	
	if(autobalanceTry < playCount)
	{
		int exceedPlay = playCount - autobalanceTry; // 초과된 플레이.
		ai = MAX(ai * downLimit, ai - downLimit / balanceN * exceedPlay);
	}
	
	m_aiValue = ai;
}

void KSCumberBase::settingSpeed( float startSpeed, float minSpeed, float maxSpeed )
{
	m_speed = m_startSpeed = startSpeed;
	//		m_speed.init(m_startSpeed, m_startSpeed, 0.1f);

	m_minSpeed = minSpeed;
	m_maxSpeed = maxSpeed;
}

void KSCumberBase::settingMovement( enum MOVEMENT normal, enum MOVEMENT draw, enum MOVEMENT fury )
{
	m_normalMovement = normal;
	m_drawMovement = draw;
	m_furyMovement = fury;
}

void KSCumberBase::settingPattern( Json::Value pattern )
{
	for(auto iter = pattern.begin(); iter != pattern.end(); ++iter)
	{
		//			int ratio = (*iter)["percent"].asInt(); // 빈번도
		//			for(int j = 0; j<ratio; j++)
		{
			m_attacks.push_back(pattern[iter.index()]);
		}
	}

	for(auto i : m_attacks)
	{
		KS::KSLog("%", i);
	}
}

void KSCumberBase::settingHp( float hp )
{
	m_remainHp = m_totalHp = hp;
}

void KSCumberBase::settingAttackPercent( float ap )
{
	int autobalanceTry = NSDS_GI(mySD->getSilType(), kSDS_SI_autoBalanceTry_i);
	int balanceN = 10;
	float downLimit = 0.5f;
	//
	ostringstream oss;
	oss << mySD->getSilType();
	std::string playcountKey = std::string("playcount_") + oss.str();
	int playCount = myDSH->getUserIntForStr(playcountKey, 0);
	
	if(autobalanceTry < playCount)
	{
		int exceedPlay = playCount - autobalanceTry; // 초과된 플레이.
		ap = MAX(ap * downLimit, ap - downLimit / balanceN * exceedPlay);
	}
	
	m_attackPercent = ap;
}

void KSCumberBase::decreaseLife( float damage )
{
	m_remainHp -= damage;
	if(m_remainHp <= 0)
	{
		myGD->communication("CP_removeSubCumber", this);
	}
}

float KSCumberBase::getLife()
{
	return m_remainHp;
}

void KSCumberBase::setLife( float t )
{
	m_remainHp = MAX(0, t);
}

float KSCumberBase::getTotalLife()
{
	return m_totalHp;
}

void KSCumberBase::setTotalLife( float t )
{
	m_totalHp = t;
}

void KSCumberBase::setSpeedRatio( float sr )
{
	m_speedRatio = sr;
}

void KSCumberBase::setSlience( bool s )
{
	m_slience = s;
}

void KSCumberBase::caughtAnimation()
{
	myGD->communication("UI_catchSubCumber");
	myGD->communication("CP_createSubCumber", myGD->getMainCumberPoint());
}

bool KSCumberBase::bossIsClosed()
{
	int greaterNumber = count_if(outlineCountRatio.begin(), outlineCountRatio.end(), [](int i){return i >= 20;} );
	bool closedBoss = false;
	if((float)greaterNumber / (float)outlineCountRatio.size() >= 0.8f)
	{
		closedBoss = true;
	}
	return closedBoss;
}

float KSCumberBase::aiProbAdder()
{
	return (0.02f + (0.5f - 0.02f) * getAiValue() / 100.f)/100.f;
}

int KSCumberBase::getAiValue()
{
	if(m_isStarted && myGD->getCommunicationBool("UI_isExchanged")) // CHANGE 라면
	{
		return m_aiValue * 1.2f;
	}
	else
	{
		return m_aiValue;
	}
}

float KSCumberBase::getAgility()
{
	return m_agility;
}

void KSCumberBase::setAgility( float ag )
{
	m_agility = ag;
}










































template <typename T>
void FixedSizeDeque<T>::push_back( const T& p )
{
	if(m_deque.size() >= m_maxSize)
	{
		m_deque.pop_front();
	}
	m_deque.push_back(p);
}

template <typename T>
T& FixedSizeDeque<T>::front()
{
	return m_deque.front();
}

template <typename T>
void FixedSizeDeque<T>::pop_front()
{
	m_deque.pop_front();
}
