//
//  KSCumberBase.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//
// 한글 주석 테스트...ㅎㅎㅎㅎㅎㅎ
#include "KSCumberBase.h"
// 테스트...
#include "Jack.h"
#include "PlayUI.h"
#include <chrono>

class KSJuniorBase;
template <class _Tp>
struct PassiveOp : public std::binary_function<_Tp, _Tp, _Tp>
{
	virtual _Tp operator()(const _Tp& a, const _Tp& b) const = 0;//(const _Tp& a, const _Tp& b) const
	//	_Tp operator()(const _Tp& __x, const _Tp& __y) const
	//	{
	//		return __x*(1 - __y);
	//	}
};

template <class _Tp>
struct DecreaseOp : public PassiveOp<_Tp>
{
	virtual _Tp operator()(const _Tp& a, const _Tp& b) const
	{
		return a*(1 - b);
	}
	//	_Tp operator()(const _Tp& __x, const _Tp& __y) const
	//	{
	//		return __x*(1 - __y);
	//	}
};





template <class _Tp>
struct SubtractOp : public PassiveOp<_Tp>
{
	virtual ~SubtractOp(){
	}
	virtual _Tp operator()(const _Tp& a, const _Tp& b) const
	{
		return a - b;
	}
	//	_Tp operator()(const _Tp& __x, const _Tp& __y) const
	//	{
	//		return __x*(1 - __y);
	//	}
};


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
		//                setCumberSize(MIN(1.0, getCumberSize() + scale.SCALE_ADDER));
	}
	CCPoint afterPosition;
	IntPoint afterPoint;
	//        int check_loop_cnt = 0;
	
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
		float speedX = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * MAX(0.1f, m_speed)) - 1));
		float speedY = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * MAX(0.1f, m_speed)) - 1));
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
				
				if(m_directionAngleDegree < 0)                        m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)        m_directionAngleDegree -= 360;
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				//                        CCLog("collision!!");
				onceOutlineAndMapCollision = true;
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)                        m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)        m_directionAngleDegree -= 360;
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				//                        CCLog("collision!!");
				//                        myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				myGD->communication("SW_createSW", checkPosition);
				//                                                                        callfuncI_selector(MetalSnake::showEmotion)); //##
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				if(m_directionAngleDegree < 0)                        m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)        m_directionAngleDegree -= 360;
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
				//                        CCLog("collision!!");
				m_directionAngleDegree += m_well512.GetValue(90, 270);
				
				if(m_directionAngleDegree < 0)                        m_directionAngleDegree += 360;
				else if(m_directionAngleDegree > 360)        m_directionAngleDegree -= 360;
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
	
	//        CCLog("cnt outer !! = %d", cnt);
	
	
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
		float speedX = m_speed * cos(deg2Rad(degree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
		float speedY = m_speed * sin(deg2Rad(degree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
		
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
			else if(cnt >= 5)
			{
				degree = (cnt % 8) * 45;
			}
			else
			{
				auto probSel = ProbSelector({1,1,1});
				auto pSel = probSel.getResult();
				
				switch(pSel)
				{
					case 0:
						degree = -degree;
						break;
					case 1:
						degree = m_directionAngleDegree + 90;
						break;
					case 2:
						degree = m_directionAngleDegree - 90;
						break;
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
			CCLog("aiValue : %d", this->getAiValue());
			t = ip2ccp(myGD->getJackPoint()) - getPosition();
			float goalDegree = rad2Deg(atan2(t.y, t.x));
			float deltaDegree = (goalDegree - m_follow.followDegree)/1.f;
			//m_follow.followDegree += deltaDegree;
			CCLog("%f", deltaDegree);
			m_follow.followDegree += clampf(deltaDegree, -5, 5);
			//m_fo
			//if(deltaDegree < 0)
			//{
				//m_follow.followDegree += MAX(-3, deltaDegree);
			//}
			//else
			//{
				//m_follow.followDegree += MIN(3, deltaDegree);
			//}	
			//float varRad = deg2Rad(m_follow.followDegree);
			dx = m_speed * cos(deg2Rad(m_follow.followDegree)) * 2.f;
			dy = m_speed * sin(deg2Rad(m_follow.followDegree)) * 2.f;
			//ProbSelector ps = {this->getAiValue(), 125 - this->getAiValue()};
			//int result = ps.getResult();
			//if(result == 0)
			//{
				//t = ip2ccp(myGD->getJackPoint()) - getPosition();
				//dx = m_speed * cos(atan2(t.y, t.x));
				//dy = m_speed * sin(atan2(t.y, t.x));
			//}
			//else 
			//{
				//t = ccp(m_well512.GetFloatValue(-1, 1), m_well512.GetFloatValue(-1, 1));
				//dx = m_speed * cos(atan2(t.y, t.x)) * 2.f;
				//dy = m_speed * sin(atan2(t.y, t.x)) * 2.f;
			//}
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
		auto degreeSelector = [&](int cnt, float degree)->float {
			if(cnt >= 30)
			{
				validPosition = true;
				pathFound = false;
			}
			else if(cnt >= 5)
			{
				m_directionAngleDegree = (cnt % 8) * 45;
			}
			else
			{
				m_directionAngleDegree += m_well512.GetValue(90, 360);
			}
			return m_directionAngleDegree;
		};
		if(m_state != CUMBERSTATEFURY)
		{
			if(collisionCode == kCOLLISION_JACK)
			{
				// 즉사 시킴.
				validPosition = true;
				if(myGD->getJackState() != jackStateNormal)
					myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				m_follow.lastMapCollisionTime = m_follow.timer;
				m_directionAngleDegree = degreeSelector(cnt, m_directionAngleDegree);
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
				if(myGD->getJackState() == jackStateNormal)
				{
					unAggroExec();
				}
			}
			else if(collisionCode == kCOLLISION_MAP)
			{
				m_crashCount++;
				onceOutlineAndMapCollision = true;
				m_directionAngleDegree = degreeSelector(cnt, m_directionAngleDegree);
				m_follow.lastMapCollisionTime = m_follow.timer;
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
				if(myGD->getJackState() == jackStateNormal)
				{
					unAggroExec();
					m_follow.lastMapCollisionTime = m_follow.timer;
				}
			}
			else if(collisionCode == kCOLLISION_OUTLINE)
			{
				m_crashCount++;
				//			CCLog("collision!!");
				onceOutlineAndMapCollision = true;
				m_follow.lastMapCollisionTime = m_follow.timer;
				m_directionAngleDegree = degreeSelector(cnt, m_directionAngleDegree);
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
				if(myGD->getJackState() == jackStateNormal)
				{
					unAggroExec();
				}
			}
			else if(collisionCode == kCOLLISION_NEWLINE)
			{
				m_follow.lastMapCollisionTime = m_follow.timer;
				m_directionAngleDegree = degreeSelector(cnt, m_directionAngleDegree);
				dx = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
				dy = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
								
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
		else // if(m_state == CUMBERSTATEFURY)
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
		
		float speedX = m_speed * cos(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
		float speedY = m_speed * sin(deg2Rad(m_directionAngleDegree)) * (1 + cnt / 30.f * (3.f / (0.5f * m_speed) - 1));
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

void KSCumberBase::rushMoving(float dt)
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
		if(m_furyMode.firstMoving == true)
		{
			CCLog("firstRush");
			CCPoint t = ip2ccp(myGD->getJackPoint()) - getPosition();
			m_directionAngleDegree = rad2Deg(atan2(t.y, t.x)) + m_well512.GetValue(-10, +10);
			m_furyMode.firstMoving = false;
		}
		else 
		{
			// 오직 벽에 의해서만 튕기도록함.
			//int changeDirection = ProbSelector::sel(0.001, 1.0 - 0.001, 0.0);
			//if(changeDirection == 0)
			//{
				//m_directionAngleDegree += m_well512.GetValue(0, 360);
			//}
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
		float tempSpeed = 1.5f;
		float speedX = tempSpeed * cos(deg2Rad(degree)) * (1 + cnt / 30.f * (3.f / (0.5f * tempSpeed) - 1));
		float speedY = tempSpeed * sin(deg2Rad(degree)) * (1 + cnt / 30.f * (3.f / (0.5f * tempSpeed) - 1));

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
			else if(cnt >= 5)
			{
				degree = (cnt % 8) * 45;
			}
			else
			{
				auto probSel = ProbSelector({1,1,1});
				auto pSel = probSel.getResult();

				switch(pSel)
				{
					case 0:
						degree = -degree;
						break;
					case 1:
						degree = m_directionAngleDegree + 90;
						break;
					case 2:
						degree = m_directionAngleDegree - 90;
						break;
				}
			}
			return degree;
		};


		
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

		if(cnt >= 2)
		{
			CCLog("rushMoving cnt !! = %d", cnt);
		}
	}

	m_directionAngleDegree = degree;

	//	CCLog("cnt outer !! = %d", cnt);


	if(m_state == CUMBERSTATEFURY)
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


/*
 땅이 n% 이하라면 부수기 공격 안하기
일반공격을 n 초 안에는 안함. 
n<m 인 부수기 공격은 m초 안에는 안함.
같은 패턴은 n 초안에는 안쏨
거리룰은 거리에 대해서만 적용함. (거리룰은 텔포1:크래시2 비율로 쏴야됨)

선을 긋고 있을 땐, 공격확률 높임
많이 많으면 공격확률 높임.
부수기 공격을 해야하는데, 부수기 공격이 없으면 텔포
가장자리 위주로 먹었다면 ai 를 고려하여 선따라가기 공격확률 높임
보스가 갇히면 ai 를 고려하여 크래시 확률을 높임*
 *
 *
 *
 * */
void KSCumberBase::cumberAttack(float dt)
{
	//	myJack->get
	
	//분노카운터,재공격카운터 계속 증가
	m_reAttackCnt++;
	m_furyCnt++;
	
	//if(m_slience || myGD->Fcommunication("UI_getMapPercentage") < 0.05f) // 침묵 상태이거나 5% 이하면 공격 ㄴㄴ...
	//{
		//return;
	//}
	if(m_slience || m_cumberTimer < 5.f) // 침묵 상태거나 시작한지 5초 이하면 공격 ㄴㄴ
	{
		return;
	}
	//	CCLog("%f %f %d", distance, gainPercent, m_crashCount);
	bool crashAttack = false;
	bool distanceFury = false; // 거리로 인한 분노인가.
	
	//거리분노룰 - 분노카운터와 리어택카운터가 0이상일때, 보스-유저의 거리가 떨어져있으면 부수기공격
	if(m_furyCnt > 0 && m_reAttackCnt > 0){
		
		float gainPercent = myGD->Fcommunication("UI_getMapPercentage") * 100.f;
		float distance = ccpLength(ip2ccp(myGD->getJackPoint()) - getPosition());
		
		if(m_furyRule.gainPercent < gainPercent && distance > m_furyRule.userDistance)
		{
			float w = ProbSelector::sel(m_furyRule.percent, 1.0f - m_furyRule.percent, 0.0);
			if(w == 0)
			{
				crashAttack = true;
				distanceFury = true;
				//분노카운터초기화, 앞으로 600프레임간은 거리분노룰 적용 안함.
				m_furyCnt = -400;

				//myGD->communication("Main_showTextMessage", std::string("거리 분노룰.."));


			}
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
		if(myGD->getJackState()==jackStateDrawing){
			
			m_adderCnt++;
			float distance = ccpLength(ip2ccp(myGD->getJackPoint()) - getPosition());
			
			//선긋기 시작한지 3초이후 && 멀리떨어지면 공격확률을 높임
			if(m_adderCnt > 180 && distance > m_furyRule.userDistance / 2.f){
				attackProb += 0.1;
			}
		}else{
			//안그을땐 초기화
			m_adderCnt = 0;
		}
		
		// 많이 맞았으면 공격확률 높임.
		if(getLife() / getTotalLife() <= 0.3f)
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
	// 확률로
	
	//재공격카운터 0이상일때.
	if(exeProb == 0 && m_state == CUMBERSTATEMOVING && m_reAttackCnt>=0)
	{
		// 부수기 공격이 시행됐는데, 크래시 공격이 없다면포 텔포 해야됨
		if(crashAttack && selectedAttacks.empty())
			
		{
			std::string patternData = R"({
			"atype" : "special",
			"pattern" : "1007",
			"percent" : 1,
			"target" : "no"})";
		
			Json::Reader reader;
			Json::Value attackCode;
			reader.parse(patternData, attackCode);
			int ret = myGD->communication("MP_attackWithKSCode", getPosition(), patternData, this, true);
			if(ret == 1)
			{
				attackBehavior(attackCode);
			}
		}
		else if(!selectedAttacks.empty())
		{
			ProbSelector teleportProb = {1,2};
			if(teleportProb.getResult() == 0 && distanceFury)
			{
				
				std::string patternData = R"({
				"atype" : "special",
				"pattern" : "1007",
				"percent" : 1,
				"target" : "no"})";
				Json::Reader reader;
				Json::Value attackCode;
				reader.parse(patternData, attackCode);
				int ret = myGD->communication("MP_attackWithKSCode", getPosition(), patternData, this, true);
				if(ret == 1)
				{
					attackBehavior(attackCode);
				}
			}
			else
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
							float alongMin = i["percent"].asFloat();
							float alongMax = notAlongSum / 1.5f;
							if(alongMax >= alongMin)
							{
								probSel.pushProb(i["percent"].asFloat() + (alongMax - i["percent"].asFloat()) * getAiValue() / 100.f);
							}
							else
							{
								probSel.pushProb(i["percent"].asFloat() * (1 + getAiValue() / 100.f));
							}
						}
						else
						{
							probSel.pushProb(i["percent"].asFloat());
						}
					}
					else if(i["atype"].asString() == "crash")
					{
						float patternMin = i["percent"].asFloat();
						if(bossIsClosed_) // 보스가 갇힘.
						{
							if(patternMax >= patternMin)
							{
								probSel.pushProb(i["percent"].asFloat() + (patternMax - i["percent"].asFloat()) * getAiValue() / 100.f);
							}
							else
							{
								probSel.pushProb(i["percent"].asFloat() * (1 + getAiValue() / 100.f));
							}
						}
						else
							probSel.pushProb(i["percent"].asFloat());
					}
					else
					{
						probSel.pushProb(i["percent"].asFloat());
					}
				}
				CCLog("externalCnt %d", externalOutlineCount);
				while(!searched)
				{
					searchCount++;
					//			int idx = m_well512.GetValue(selectedAttacks.size() - 1);
					int idx = probSel.getResult();
					
					if(idx<0){
						searched = false;
						break;
					}
					
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
						
						//한번 공격후 3초간 재공격 하지 않음.
						m_reAttackCnt = -180;
					}
				}
			}
		}
	}





}

void KSCumberBase::speedAdjustment(float dt)
{
	int considerFrames = 60 * 7; // 초.
	int maxCre = 7;
	int cntPerSecond = MIN(count_if(m_damagedFrames.getSTL().begin(), m_damagedFrames.getSTL().end(),
																	[=](int w){
																		return w >= m_frameCount - considerFrames;
																	}), maxCre);
	//static int t = 0;
	//t++;
	//if(t % 60)
	//{
		//myGD->communication("Main_showTextMessage", boost::str(boost::format("%||") % cntPerSecond));
	//}
	float baseSpeed = m_maxSpeed;
	float finalSpeed = m_maxSpeed - (cntPerSecond) * (m_maxSpeed - m_minSpeed) / maxCre;
	m_speed = clampf(finalSpeed, m_minSpeed, m_maxSpeed);
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
		auto ret = KS::loadCCBI<CCSprite*>(this, "bossbomb2.ccbi");
		CCPoint t = getPosition();
		t.x += m_well512.GetFloatValue(-100.f, 100.f);
		t.y += m_well512.GetFloatValue(-100.f, 100.f);
		ret.first->setPosition(t);
		getParent()->addChild(ret.first, 11);
		
		if(maxValue == m_bossDie.m_bossDieFrameCount)
		{
			auto ret = KS::loadCCBI<CCSprite*>(this, "bossbomb1.ccbi");
			CCPoint t = getPosition();
			ret.first->setPosition(t);
			getParent()->addChild(ret.first, 11);
			this->setVisible(false);
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
	//ProbSelector ps({getAiValue(), 100.f - getAiValue()});
	//int r = ps.getResult();
	//r = 0;
	//CCLog("%d %d", myGD->getJackPoint().x, myGD->getJackPoint().y);
	//CCLog("%f %f", getPosition().x, getPosition().y);
	//CCLog("distance = %f", ccpLength(ip2ccp(myGD->getJackPoint()) - getPosition()));
	//float dis = ccpLength(ip2ccp(myGD->getJackPoint()) - getPosition());
	//dis = 100.f;
	//if(r == 0 && dis >= 55.f) // 너무 가까우면 안따라가게 함.
	//{
		//m_drawMovement = FOLLOW_TYPE;
	//}
	//else
	//{
		//m_drawMovement = m_normalMovement;
	//}
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
	
}

void KSCumberBase::movingAndCrash( float dt )
{
	checkConfine(dt);
	
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
				break;
			case RUSH_TYPE:
				rushMoving(dt);
				break;
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

void KSCumberBase::followProcess(float dt)
{
	if(myGD->getJackState() == jackStateDrawing && m_drawMovement != FOLLOW_TYPE && m_normalMovement != MOVEMENT::FOLLOW_TYPE)
	{
		ProbSelector ps = {this->getAiValue() / 25.f, 125.f};
		if(ps.getResult() == 0)
		{
			CCLog("follow!!!");
			CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
			KSCumberBase* mainCumber = dynamic_cast<KSCumberBase*>(myGD->getCommunicationNode("CP_getMainCumberPointer"));

			int aggroCount = 0;
			if(mainCumber->m_normalMovement == MOVEMENT::FOLLOW_TYPE &&
				 mainCumber->m_drawMovement == MOVEMENT::FOLLOW_TYPE)
			{
				aggroCount++;
			}

			for(int i=0;i<subCumberArray->count();i++)
			{
				KSCumberBase* t_sc = (KSCumberBase*)subCumberArray->objectAtIndex(i);
				if(t_sc->m_normalMovement == MOVEMENT::FOLLOW_TYPE &&
					 t_sc->m_drawMovement == MOVEMENT::FOLLOW_TYPE)
				{
					aggroCount++;
				}
			}
			if(aggroCount < 2)
			{
				aggroExec();
			}
			else
			{
				KSCumberBase* farestMonster = nullptr;
				float maxDistance = ccpLength(ip2ccp(myGD->getJackPoint()) - this->getPosition()); // 자기 자신의 거리.
				
				// 어그로 끌린 놈들중에 잭과의 거리가 가장 먼 애를 어그로 풀고 어그로 걸림.	
				if(mainCumber->m_normalMovement == MOVEMENT::FOLLOW_TYPE &&
					 mainCumber->m_drawMovement == MOVEMENT::FOLLOW_TYPE)
				{
					if(maxDistance < ccpLength(ip2ccp(myGD->getJackPoint()) - mainCumber->getPosition()))
					{
						maxDistance = ccpLength(ip2ccp(myGD->getJackPoint()) - mainCumber->getPosition());
						farestMonster = mainCumber;
					}
				}
				
				for(int i=0;i<subCumberArray->count();i++)
				{
					KSCumberBase* t_sc = (KSCumberBase*)subCumberArray->objectAtIndex(i);
					if(t_sc->m_normalMovement == MOVEMENT::FOLLOW_TYPE &&
						 t_sc->m_drawMovement == MOVEMENT::FOLLOW_TYPE)
					{
						if(maxDistance < ccpLength(ip2ccp(myGD->getJackPoint()) - t_sc->getPosition()))
						{
							maxDistance = ccpLength(ip2ccp(myGD->getJackPoint()) - t_sc->getPosition());
							farestMonster = t_sc;
						}
					}
				}
				if(farestMonster)
				{
					// 어그로 풀고
					farestMonster->unAggroExec();		

					// 어그로 걸림.
					aggroExec();

				}
			}

		}
	}
	else
	{
		//m_drawMovement = m_normalMovement;
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
	schedule(schedule_selector(ThisClassType::timeMeasure));
}

void KSCumberBase::lightSmaller()
{
	addChild(KSTimer::create
					 (1.f, [=](){
		this->endTeleport();
	}));

	//this->endTeleport();
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
	myGD->communication("Main_showDetailMessage", std::string("warning_boss_success.ccbi")); // 말은 캐스팅 캔슬 됐다고 알려줌.
}

void KSCumberBase::settingScale( float startScale, float minScale, float maxScale )
{
	m_startScale = startScale;
	m_minScale = minScale;
	m_maxScale = maxScale;
	
	m_scale.SCALE_ADDER = m_scale.SCALE_SUBER = (m_maxScale - m_minScale) / 10.f;
	m_scale.scale.init(m_startScale, m_startScale, 0.f);
}

void KSCumberBase::assignBossData(Json::Value boss)
{
	m_properties = boss;
	float hp = MAX(boss["hp"].asInt(), 0);
	float minSpeed = MAX(boss["speed"]["min"].asDouble(), 0);
	float startSpeed = MAX(boss["speed"]["start"].asDouble(), 0); //getNumberFromJsonValue(speed["start"]);
	float maxSpeed = MAX(boss["speed"]["max"].asDouble(), 0);// getNumberFromJsonValue(speed["min"]);
	
	float minScale = MAX(boss["scale"]["min"].asDouble(), 0); // getNumberFromJsonValue(scale["min"]);
	float startScale = MAX(boss["scale"]["start"].asDouble(), 0); // getNumberFromJsonValue(scale["start"]);
	float maxScale = MAX(boss["scale"]["max"].asDouble(), 0); // getNumberFromJsonValue(scale["max"]);
	
	int normalMovement = boss["movement"].get("normal",1).asInt();
	int drawMovement = boss["movement"].get("draw", normalMovement).asInt();
//	int furyMovement = boss["movement"].get("fury", normalMovement).asInt();
	
	float agi = MAX(boss.get("agi", 0).asDouble(), 0);
	float ai = MAX(0, boss.get("ai", 0).asInt() );
	
	settingFuryRule();
	m_totalHp = m_remainHp = hp;
	m_agility = agi;
	m_aiValue = ai;
	m_startScale = startScale;
	m_minScale = minScale;
	m_maxScale = maxScale;
	
	m_startSpeed = startSpeed;
	m_minSpeed = minSpeed;
	m_maxSpeed = maxSpeed;
	
	m_normalMovement = (enum MOVEMENT)normalMovement;
	m_originalNormalMovement = m_normalMovement;
	m_drawMovement = (enum MOVEMENT)drawMovement;
	m_furyMovement = MOVEMENT::RUSH_TYPE;
}

void KSCumberBase::applyPassiveData(const std::string& passive)
{
	Json::Reader reader;
	Json::Value passiveCard;
	reader.parse(passive, passiveCard);
	shared_ptr<PassiveOp<float>> cardOperator;
	if(passiveCard["operator"].asString() == "-")
	{
		cardOperator = shared_ptr<PassiveOp<float>>(new SubtractOp<float>());
	}
	else// if(passiveCard["operator"].asString() == "*(1-x)")
	{
		cardOperator = shared_ptr<PassiveOp<float>>(new DecreaseOp<float>());
	}
	m_totalHp = MAX((*cardOperator)(m_totalHp, passiveCard["hp"].asInt()), 0);
	m_minSpeed = MAX((*cardOperator)(m_minSpeed, passiveCard["speed"].asDouble()), 0);
	m_startSpeed = MAX((*cardOperator)(m_startSpeed, passiveCard["speed"].asDouble()), 0); //getNumberFromJsonValue(speed["start"]);
	m_maxSpeed = MAX((*cardOperator)(m_maxSpeed, passiveCard["speed"].asDouble()), 0);// getNumberFromJsonValue(speed["min"]);
	
	m_minScale = MAX((*cardOperator)(m_minScale, passiveCard["scale"].asDouble()), 0); // getNumberFromJsonValue(scale["min"]);
	m_startScale = MAX((*cardOperator)(m_startScale, passiveCard["scale"].asDouble()), 0); // getNumberFromJsonValue(scale["start"]);
	m_maxScale = MAX((*cardOperator)(m_maxScale, passiveCard["scale"].asDouble()), 0); // getNumberFromJsonValue(scale["max"]);
	
	m_agility = MAX((*cardOperator)(m_agility, passiveCard["agi"].asDouble()), 0);
	
	
}
void KSCumberBase::settingFuryRule()
{
	m_furyRule.gainPercent = 40;
	m_furyRule.userDistance = 300;
	m_furyRule.percent = mySGD->getFuryPercent() * getAiValue() / 10000.f;
	//		m_furyRule
}

void KSCumberBase::applyAutoBalance()
{
	int autobalanceTry = NSDS_GI(mySD->getSilType(), kSDS_SI_autoBalanceTry_i);
	int balanceN = 5;
	float downLimit = 0.5f;
	
	int clearCount = myDSH->getIntegerForKey(kDSH_Key_achieve_seqNoFailCnt);
	int puzzleNo = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	
	
	ostringstream oss;
	oss << mySD->getSilType();
	std::string playcountKey = std::string("playcount_") + oss.str();
	int playCount = myDSH->getUserIntForStr(playcountKey, 0);
	
	int balPt = clearCount-2;
	
	
	CCLog("#################### autobalance ############################");
	CCLog("clear : %d / try : %d / autobalanceTry : %d / puzzleNo : %d",clearCount,playCount,autobalanceTry,puzzleNo);
	CCLog("AI : %d, attackPercent : %f, speed : %f~%f",m_aiValue,m_attackPercent,m_minSpeed,m_maxSpeed);
	
	//오토벨런싱트라이 값까지는 어렵게
	if(playCount<=autobalanceTry){
		float per = 1.f-(float)playCount/(float)autobalanceTry;
		//ai조절
		if(m_aiValue>0){
			int aiMax = m_aiValue*2;
			if(aiMax>90)aiMax=90;
			if(m_aiValue>aiMax)aiMax=m_aiValue;
			if(aiMax<=70)aiMax=70;
			m_aiValue = m_aiValue + (aiMax-m_aiValue)*per;
		}
		//attackterm조절
		if(m_attackPercent>0){
			float aiMax = m_attackPercent*2;
			if(aiMax>0.4)aiMax=0.4;
			if(m_attackPercent>aiMax)aiMax=m_attackPercent;
			if(aiMax<=0.3)aiMax=0.3;
			m_attackPercent = m_attackPercent + (aiMax-m_attackPercent)*per;
		}
	
		CCLog("#################### Change Balnace ############################");
		CCLog("AI : %d, attackPercent : %f, speed : %f~%f",m_aiValue,m_attackPercent,m_minSpeed,m_maxSpeed);
	//오토벨런싱+2 판까지는 원래 벨런스로 플레이
	}else if(playCount>autobalanceTry && playCount<=autobalanceTry+2){
		
	//그다음부턴 50%까지 까임
	}else{
		
	}
					 
	
	CCLog("#################### autobalance ############################");
					 
	
//	if(clearCount>1 && puzzleNo!=1){
//		
//		CCLog("UP monster abillity");
//		
//		if(balPt>10)balPt=10;
//		m_aiValue += balPt*10;
//		m_aiValue = MIN(100,m_aiValue);
//		
//		m_attackPercent *= 1+balPt/10.f;
//		m_attackPercent = MIN(0.4,m_attackPercent);
//		
//		m_maxSpeed *= 1+balPt/10.f;
//		m_minSpeed *= 1+balPt/10.f;
//		m_minSpeed = MIN(1, m_minSpeed);
//		m_maxSpeed = MIN(3, m_maxSpeed);
//		
//		CCLog("AI : %d, attackPercent : %f",m_aiValue,m_attackPercent);
//		CCLog("speed : %f~%f",m_minSpeed,m_maxSpeed);
//		
//		CCLog("#################### autobalance ############################");
//		//m_aiValue , m_attackPercent, m_maxSpeed , m_minSpeed
//	
//	// 계속 실패할경우 능력치 하향하기
//	}else{
//
//		if(autobalanceTry < playCount)
//		{
//			int exceedPlay = playCount - autobalanceTry; // 초과된 플레이.
//			float autoRate = downLimit * exceedPlay / balanceN;
//			m_aiValue = MAX(m_aiValue * downLimit, m_aiValue * (1 - autoRate));
//			m_attackPercent = MAX(m_attackPercent * downLimit, m_attackPercent * (1 - autoRate));
//			
//			CCLog("DOWN monster abillity");
//			CCLog("AI : %d, attackPercent : %f",m_aiValue,m_attackPercent);
//			if(autobalanceTry*2 < playCount){
//				float autoRate2 = downLimit * (playCount - autobalanceTry*2) / balanceN;
//				m_maxSpeed = MAX(m_maxSpeed*downLimit,m_maxSpeed * (1-autoRate2));
//				m_minSpeed = MAX(m_minSpeed*downLimit,m_minSpeed * (1-autoRate2));
//				
//				CCLog("speed : %f~%f",m_minSpeed,m_maxSpeed);
//			}
//			
//			
//		}
//		
//		CCLog("#################### autobalance ############################");
//	}
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
	
	CCLog("!!!settingAttackPercent!!! %f",ap);
	
//	int autobalanceTry = NSDS_GI(mySD->getSilType(), kSDS_SI_autoBalanceTry_i);
//	int balanceN = 10;
//	float downLimit = 0.5f;
//	//
//	ostringstream oss;
//	oss << mySD->getSilType();
//	std::string playcountKey = std::string("playcount_") + oss.str();
//	int playCount = myDSH->getUserIntForStr(playcountKey, 0);
//	
//	if(autobalanceTry < playCount)
//	{
//		int exceedPlay = playCount - autobalanceTry; // 초과된 플레이.
//		ap = MAX(ap * downLimit, ap - downLimit / balanceN * exceedPlay);
//	}
	
	
	
	//CCLog("!!!settingAttackPercent!!! %f",ap);
	
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
	return mySGD->getAiAdderOnDrewOrDamaged() * getAiValue() / 10000.f;
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
