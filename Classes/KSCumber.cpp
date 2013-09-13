//
//  KSCumber.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#include "KSCumber.h"
#include "GameData.h"

#include "AlertEngine.h"
#include "KSGeometry.h"
#include <cmath>
#include "ProbSelector.h"
#include "CumberEmotion.h"



bool KSCumber::init()
{
	KSCumberBase::init();
	
	m_directionAngleDegree = m_well512.GetValue(0, 360);
	m_speed = 2.f;
	m_headImg = CCSprite::create("chapter10_boss.png");
	
	addChild(m_headImg);
	
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
	
	schedule(schedule_selector(KSCumberBase::movingAndCrash));
	schedule(schedule_selector(KSCumber::attack));
	
	return true;
}

void KSCumber::movingAndCrash(float dt)
{
	m_directionAngleDegree += m_well512.GetValue(-4, +4);
	
	if(m_state == CUMBERSTATEMOVING)
	{
		int sela = ProbSelector::sel(0.005, 1.0 - 0.005, 0.0);
		if(sela == 0)
		{
			m_directionAngleDegree += m_well512.GetValue(90, 270);
		}
	}
	
	CCPoint afterPosition;
	
	bool validPosition = false;
	int cnt = 0;
	while(!validPosition)
	{
		cnt++;
		float speedX = m_speed * sin(deg2Rad(m_directionAngleDegree));
		float speedY = m_speed * cos(deg2Rad(m_directionAngleDegree));
		
		CCPoint cumberPosition = getPosition();
		afterPosition = cumberPosition + ccp(speedX, speedY);
		IntPoint afterPoint = ccp2ip(afterPosition);
		
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
//			gameData->communication("SW_createSW", checkPosition, 0, 0);
//									callfuncI_selector(KSCumber::showEmotion)); //##
			
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
			CCLog("cnt !! = %d", cnt);
		}
	}
	
	CCLog("cnt outer !! = %d", cnt);
	
	if(m_state == CUMBERSTATEMOVING)
		setPosition(afterPosition);
}

void KSCumber::startDamageReaction(float userdata)
{
	// 방사형으로 돌아가고 있는 중이라면
	if(m_state == CUMBERSTATENODIRECTION)
	{
		CCLog("m_state == CUMBERSTATENODIRECTION");
		m_noDirection.state = 2; // 돌아가라고 상태 변경때림.
	}
	else if(m_state == CUMBERSTATEMOVING)
	{
		CCLog("m_state == CUMBERSTATEMOVING");
		float rad = deg2Rad(userdata);
		m_damageData.m_damageX = cos(rad);
		m_damageData.m_damageY = sin(rad);
		//	CCLog("%f %f", dx, dy);
		m_state = CUMBERSTATEDAMAGING;
		
		m_damageData.timer = 0;
		schedule(schedule_selector(KSCumber::damageReaction));
	}

	
	
}


void KSCumber::damageReaction(float)
{
	m_damageData.timer += 1 / 60.f;
	if(m_damageData.timer < 1)
	{
		m_headImg->setColor(ccc3(255, 0, 0));
	}
	else
	{
		m_headImg->setColor(ccc3(255, 255, 255));
		m_state = CUMBERSTATEMOVING;
		unschedule(schedule_selector(KSCumber::damageReaction));
	}
}


void KSCumber::animationNoDirection(float dt)
{
	m_noDirection.timer += 1.f/60.f;
	
	if(m_noDirection.state == 1)
	{
		m_noDirection.rotationDeg += 6.f;
		if(m_noDirection.rotationDeg >= 360)
		{
			m_noDirection.rotationDeg -= 360;
			m_noDirection.rotationCnt++;
			/// 좀 돌았으면 돌아감.
			if(m_noDirection.rotationCnt >= 5)
			{
				m_noDirection.state = 2;
				return;
			}
		}
		m_noDirection.distance += 0.5f;
		m_noDirection.distance = MIN(m_noDirection.distance, 30);
		float dx = cos(deg2Rad(m_noDirection.rotationDeg)) * m_noDirection.distance;
		float dy = sin(deg2Rad(m_noDirection.rotationDeg)) * m_noDirection.distance * 1.2f; // 약간 타원
		
		
		dx = dy = 0.f; // 안돔.
		//	float speed = 2.f;
		//	dx *= speed;
		//	dy *= speed;
		
		setPosition(m_noDirection.startingPoint + ccp(dx, dy));
	}
	else if(m_noDirection.state == 2)
	{
		CCPoint dir = m_noDirection.startingPoint - getPosition();
		float rad = atan2(dir.y, dir.x);
		float dx = cos(rad);
		float dy = sin(rad);
		
		
		if(ccpLength(m_noDirection.startingPoint - getPosition()) <= 0.5f)
		{
			m_state = CUMBERSTATEMOVING;
			unschedule(schedule_selector(KSCumber::animationNoDirection));
			setPosition(m_noDirection.startingPoint);
		}
		else
			setPosition(getPosition() + ccp(dx, dy));
	}
}
void KSCumber::attack(float dt)
{
	float w = ProbSelector::sel(0.003, 1.0 - 0.003, 0.0);
	
	// 1% 확률로.
	if(w == 0 && m_state == CUMBERSTATEMOVING)
	{
		//		stopMoving();
		startAnimationNoDirection();
		
		int attackCode = 0;
		
		bool searched = false;
		while(!searched)
		{
			attackCode = m_well512.GetValue(0, 38);
			if(attackCode == 6 || attackCode == 7 || attackCode == 13 || attackCode == 19 ||
			   attackCode == 20 || attackCode == 31 || attackCode == 32 || attackCode == 34 ||
			   attackCode == 35)
			{
				searched = false;
			}
			else
				searched = true;
			
		}
		
		
		gameData->communication("MP_attackWithCode", getPosition(), attackCode);
		//		showEmotion(kEmotionType_joy);
		//		m_speed = m_well512.GetValue(2, 4);
		//		startAnimationNoDirection();
		//		gameData->communication("MP_startFire", getPosition(), false);
		//		gameData->comm("MP_attackWithCode", 33);
		if(m_well512.GetValue(0, 1))
		{
			
		}
		else{
			
		}		
	}
}
COLLISION_CODE KSCumber::crashWithX(IntPoint check_position)
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
COLLISION_CODE KSCumber::crashLooper(const vector<IntPoint> v, IntPoint* cp)
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

