//
//  SnakeCumber.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 10..
//
//

#include "SnakeCumber.h"
#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
#include "CumberEmotion.h"

SnakeCumber::~SnakeCumber()
{
	
}



bool SnakeCumber::init()
{
	KSCumberBase::init();
	
	m_directionAngleDegree = m_well512.GetValue(0, 360);
	m_speed = 2.f;
	m_headImg = CCSprite::create("boss1_head.png");
//	m_headImg->setVisible(false);
	addChild(m_headImg, 10);
	
	for(int i=0; i<7; i++)
	{
		CCSprite* body = CCSprite::create("boss1_body.png");
		addChild(body, 9 - i);
		m_Bodies.push_back(body);
	}
	
	bool isGoodPointed = false;
	int check_loop_cnt = 0;
	IntPoint mapPoint;
	while(!isGoodPointed)
	{
		check_loop_cnt++;
		if(check_loop_cnt > 3000)
		{
			CCAssert(false, "");
		}
		//			AlertEngine::sharedInstance()->addSingleAlert("error", "maincumber init infinity loop", "ok", 1, this, alertfuncII_selector(MainCumber::alertAction));
		
		mapPoint.x = m_well512.GetValue(1, mapLoopRange::mapWidthInnerEnd - 1);
		mapPoint.y = m_well512.GetValue(1, mapLoopRange::mapHeightInnerEnd - 1);
		
		float myScale = m_scale;
		if(mapPoint.isInnerMap() && gameData->mapState[mapPoint.x][mapPoint.y] == mapEmpty)
		{
			float half_distance = RADIUS*myScale; // 20.f : radius for base scale 1.f
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
						if(!check_position.isInnerMap() || gameData->mapState[check_position.x][check_position.y] != mapEmpty)
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

	gameData->setMainCumberPoint(mapPoint);
	setPosition(ccp((mapPoint.x-1)*pixelSize + 1,(mapPoint.y-1)*pixelSize + 1));
//	startMoving();
	schedule(schedule_selector(SnakeCumber::attack));
	return true;
}

void SnakeCumber::setHeadAndBodies()
{
	Trace lastTrace = m_cumberTrace.back();
	float tt = rad2Deg( lastTrace.directionRad );
//	CCLog("deg %f", tt);
//	m_headImg->setVisible(false);
	m_headImg->setRotation(-rad2Deg( lastTrace.directionRad ));
	
	int lastTraceIndex = m_cumberTrace.size() - 1; // to 0
	int bodyIndex = 0;
	for(auto i : m_cumberTrace)
	{
		// 순서대로 머리에 가까운 몸통처리.
		float distance = 0;
		for(int traceIndex = lastTraceIndex - 1; traceIndex >= 0; traceIndex--)
		{
			Trace t = m_cumberTrace[traceIndex];
			// t 와 tr 의 거리차이.
//			float distance = ccpLength(lastTrace.position - t.position);
//			int distance = lastTraceIndex - traceIndex;
			
			// traceIndex + 1 위치와 traceIndex 위치의 누적
			distance += ccpLength(m_cumberTrace[traceIndex + 1].position - t.position);
			if(distance >= 20)
			{
				lastTraceIndex = traceIndex;
				lastTrace = t;
				break;
			}
		}
		
		if(bodyIndex < m_Bodies.size())
		{
			m_Bodies[bodyIndex]->setRotation(-rad2Deg(lastTrace.directionRad));
			m_Bodies[bodyIndex]->setPosition(lastTrace.position);
//			CCLog("%d / %d, %d", lastTraceIndex, (int)m_cumberTrace.size(), (int)m_cumberTrace.size() - lastTraceIndex);
		}
		else
		{
			break;
		}
		bodyIndex++;
	}
	
//	m_headImg->setScale(tt / 360);
}

void SnakeCumber::startAttackReaction(int userdata)
{
	CCLog("attack reaction!");
}
void SnakeCumber::movingAndCrash(float dt)
{
	bool isFindedAfterPoint = false, is_map_visited = false;
	CCPoint afterPosition;
	IntPoint afterPoint;
	//	int check_loop_cnt = 0;
	
	if(m_state != CUMBERSTATESTOP)
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
	while(!validPosition)
	{
		cnt++;
		float speedX = m_speed * sin(deg2Rad(m_directionAngleDegree));
		float speedY = m_speed * cos(deg2Rad(m_directionAngleDegree));
		
		CCPoint cumberPosition = getPosition();
		afterPosition = cumberPosition + ccp(speedX, speedY);
		afterPoint = ccp2ip(afterPosition);
		
		float half_distance = RADIUS*m_scale; // 20.f : radius for base scale 1.f
		int ip_half_distance = half_distance / 2;
		IntPoint checkPosition;
		vector<IntPoint> ips;
		for(int i=afterPoint.x-ip_half_distance;i<=afterPoint.x+ip_half_distance;i++)
		{
			for(int j=afterPoint.y-ip_half_distance;j<=afterPoint.y+ip_half_distance;j++)
			{
				float calc_distance = sqrtf(powf((afterPoint.x - i)*1,2) + powf((afterPoint.y - j)*1, 2));
				if(calc_distance < ip_half_distance)
				{
					ips.push_back(IntPoint(i, j));
				}
			}
		}
		
		COLLISION_CODE collisionCode = crashLooper(ips, &checkPosition);
		if(collisionCode == kCOLLISION_JACK)
		{
			// 즉사 시킴.
			gameData->communication("Jack_startDieEffect");
		}
		else if(collisionCode == kCOLLISION_MAP)
		{
			m_directionAngleDegree += m_well512.GetValue(90, 270);
			
			if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
			else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
		}
		else if(collisionCode == kCOLLISION_NEWLINE)
		{
//			gameData->communication("Jack_startDieEffect");
			gameData->communication("SW_createSW", checkPosition, 0, 0);
			//									callfuncI_selector(SnakeCumber::showEmotion)); //##
			
			m_directionAngleDegree += m_well512.GetValue(90, 270);
			
			if(m_directionAngleDegree < 0)			m_directionAngleDegree += 360;
			else if(m_directionAngleDegree > 360)	m_directionAngleDegree -= 360;
		}
		else if(collisionCode == kCOLLISION_OUTLINE)
		{
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
			validPosition = true;
		}
		//		setPosition(afterPosition);
		if(cnt % 100 == 0)
		{
//			CCLog("cnt !! = %d", cnt);
		}
	}
	
//	CCLog("cnt outer !! = %d", cnt);
	
	if(m_state != CUMBERSTATESTOP)
		setPosition(afterPosition);
}

void SnakeCumber::attack(float dt)
{
	float w = ProbSelector::sel(0.01, 1.0 - 0.01, 0.0);
	
	// 1% 확률로.
	if(w == 0 && m_state == CUMBERSTATEMOVING)
	{
		stopMoving();
		
		showEmotion(kEmotionType_joy);
//		m_speed = m_well512.GetValue(2, 4);
		
		gameData->communication("MP_startFire", getPosition(), false);
	}
}
COLLISION_CODE SnakeCumber::crashWithX(IntPoint check_position)
{
	// 이미 그려진 곳에 충돌했을 경우.
	if(gameData->mapState[check_position.x][check_position.y] == mapOldline ||
	   gameData->mapState[check_position.x][check_position.y] == mapOldget)
	{
		return COLLISION_CODE::kCOLLISION_MAP;
	}
	
	if(gameData->mapState[check_position.x][check_position.y] == mapNewline)
	{
		return COLLISION_CODE::kCOLLISION_NEWLINE;
	}
	IntPoint jackPoint = gameData->getJackPoint();
	if(jackPoint.x == check_position.x && jackPoint.y == check_position.y)
	{
		return COLLISION_CODE::kCOLLISION_JACK;
	}
	
	
	if(check_position.x < mapLoopRange::mapWidthInnerBegin || check_position.x >= mapLoopRange::mapWidthInnerEnd ||
	   check_position.y < mapLoopRange::mapHeightInnerBegin || check_position.y >= mapLoopRange::mapHeightInnerEnd )
	{
		// 나갔을 시.
		return COLLISION_CODE::kCOLLISION_OUTLINE;
	}
	
	
	return COLLISION_CODE::kCOLLISION_NONE;
	
}
COLLISION_CODE SnakeCumber::crashLooper(const vector<IntPoint> v, IntPoint* cp)
{
	for(auto i : v)
	{
		auto result = crashWithX(i);
		if(result != kCOLLISION_NONE)
		{
			*cp = i;
			return result;
		}
	}
	return kCOLLISION_NONE;
}

