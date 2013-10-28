#include "MissileUnit.h"


void AlongOfTheLine::myInit(CCPoint cumberPosition, CCPoint jackPosition)
{
	m_step = 1;
	m_totalFrame = 600;
	

	vector<IntPoint> oldLines;
	
	for(int y=0; y<mapLoopRange::mapHeightInnerEnd; y++)
	{
		for(int x=0; x<mapLoopRange::mapWidthInnerEnd; x++)
		{
			if(myGD->mapState[x][y] == mapType::mapOldline)
			{
				IntPoint ip;
				ip.x = x;
				ip.y = y;
				
				queue<AlongPath> bfs;
				if(m_directions.find(ip) != m_directions.end())
					continue;
				bfs.push(AlongPath(ip, IntPoint(0, 1)));
				while(1)
				{
					auto path = bfs.front();
					bfs.pop();
					IntPoint left(-1, 0);
					if(path.direction == IntPoint(-1, 0))
					{
						left = left.rotation(90 * M_PI / 180.f);
					}
					else if(path.direction == IntPoint(0, -1))
					{
						left = left.rotation(180 * M_PI / 180.f);
					}
					else if(path.direction == IntPoint(1, 0))
					{
						left = left.rotation(270 * M_PI / 180.f);
					}
					IntPoint top = left.rotation(-90 * M_PI / 180.f);
					IntPoint right = left.rotation(-180 * M_PI / 180.f);
					IntPoint bottom = left.rotation(90 * M_PI / 180.f);
					
					IntPoint checkPoint1 = path.point + left;
					IntPoint checkPoint2 = path.point + top;
					IntPoint checkPoint3 = path.point + right;
					if(myGD->mapState[checkPoint1.x][checkPoint1.y] == mapType::mapOldline)
					{
						m_directions[path.point] = left;
						if(checkPoint1 == ip)
							break;
						bfs.push(AlongPath(checkPoint1, left));
					}
					else if(myGD->mapState[checkPoint2.x][checkPoint2.y] == mapType::mapOldline)
					{
						m_directions[path.point] = top;
						if(checkPoint2 == ip)
							break;
						bfs.push(AlongPath(checkPoint2, top));
					}
					else if(myGD->mapState[checkPoint3.x][checkPoint3.y] == mapType::mapOldline)
					{
						m_directions[path.point] = right;
						if(checkPoint3 == ip)
							break;
						bfs.push(AlongPath(checkPoint3, right));

					}
					else // 아무곳도 갈곳이 없음.
					{
						m_directions[path.point] = bottom;
						IntPoint checkPoint4 = path.point + bottom;
						if(checkPoint4 == ip)
							break;
						bfs.push(AlongPath(checkPoint4, bottom));	
					}
				}
				
			}
		}
	}
	
//	for(auto dir : m_directions)
//	{
//		CCSprite* spr = CCSprite::create();
//		spr->setTextureRect(CCRectMake(0, 0, 1, 1));
//		IntPoint left(-1, 0);
//		IntPoint top = left.rotation(-90 * M_PI / 180.f);
//		IntPoint right = left.rotation(-180 * M_PI / 180.f);
//		IntPoint bottom = left.rotation(90 * M_PI / 180.f);
//		if(dir.second == left)
//		{
//			spr->setColor(ccc3(255, 0, 0));
//		}
//		else if(dir.second == top)
//		{
//			spr->setColor(ccc3(0, 255, 0));
//		}
//		else if(dir.second == right)
//		{
//			spr->setColor(ccc3(0, 0, 255));
//		}
//		else if(dir.second == bottom)
//		{
//			spr->setColor(ccc3(80, 0, 80));
//		}
//		spr->setPosition(ip2ccp(dir.first));
//		addChild(spr, 100);
//	}
//
	// m_directions 은 위치에 따른 directions 을 가짐.
	
	int numbers = 4;
	for(int i=0; i<numbers; i++)
	{
		auto iter = m_directions.begin();
		std::advance( iter, m_well512.GetValue(0, m_directions.size() - 1) );
		auto point = iter->first;
		auto direction = iter->second;
		
		// 목표 위치 부착.
		CCSprite* goal = CCSprite::create("satelliteBeam_targeting.png");
		addChild(goal);
		goal->setPosition(ip2ccp(point));
		
		
		Pollution pollution;
		pollution.glue.init(cumberPosition, ip2ccp(point), 0.01f * ccpLength(ip2ccp(point) - cumberPosition));
		pollution.alongPath.point = point;
		pollution.alongPath.direction = direction;
		pollution.spr = KS::loadCCBI<CCSprite*>(this, "fx_pollution3.ccbi").first;
		pollution.goal = goal;
		
		m_pollutions.push_back(pollution);
		addChild(pollution.spr);
		
		
		
	}

//	scheduleUpdate();
	schedule(schedule_selector(ThisClassType::update), 1/60.f);
}
void AlongOfTheLine::hidingAnimation(float dt)
{
	bool r = m_scaleTo.step(1 / 30.f);
	if(!r) // finish
	{
		removeFromParent();
	}
	else for(auto i : m_pollutions){
		i.spr->setScale(m_scaleTo.getValue());
	}
	
}
void AlongOfTheLine::update(float dt)
{
	m_frame++;
	
	if(m_frame >= m_totalFrame)
	{
		unschedule(schedule_selector(ThisClassType::update));
		m_scaleTo.init(1.f, 0.f, 0.5f);
		schedule(schedule_selector(ThisClassType::hidingAnimation));
	}
	else for(auto i = m_pollutions.begin(); i != m_pollutions.end();)
	{
		bool r = i->glue.step(1 / 60.f);

		i->spr->setPosition(i->glue.getValue());
		if(!r && i->step == 1)
		{
			i->goal->removeFromParent();
			i->step = 2;
		}
		bool erased = false;
		if(i->step == 2)
		{
			i->spr->setPosition(ip2ccp(i->alongPath.point));
			if(myGD->mapState[i->alongPath.point.x - 1][i->alongPath.point.y] == mapType::mapOldget &&
				 myGD->mapState[i->alongPath.point.x + 1][i->alongPath.point.y] == mapType::mapOldget &&
				 myGD->mapState[i->alongPath.point.x][i->alongPath.point.y + 1] == mapType::mapOldget &&
				 myGD->mapState[i->alongPath.point.x][i->alongPath.point.y - 1] == mapType::mapOldget)
			{
//				i->spr->runAction(KSSequenceAndRemove::create(i->spr, {CCScaleTo::create(0.5f, 0.f)}));
				i->spr->removeFromParentAndCleanup(true);
				i = m_pollutions.erase(i);
				
				erased = true;
				continue;
			}
			IntPoint jackPoint = myGD->getJackPoint();
			
			if(i->alongPath.point == jackPoint)
			{
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				unschedule(schedule_selector(ThisClassType::update));
				m_scaleTo.init(1.f, 0.f, 0.5f);
				CCLog("abc");
				schedule(schedule_selector(ThisClassType::hidingAnimation));
				break;
			}
			i->alongPath.point = i->alongPath.point + i->alongPath.direction; // 방향 대로 움직임.
			
			IntPoint left(-1, 0);
			if(i->alongPath.direction == IntPoint(-1, 0))
			{
				left = left.rotation(90 * M_PI / 180.f);
			}
			else if(i->alongPath.direction == IntPoint(0, -1))
			{
				left = left.rotation(180 * M_PI / 180.f);
			}
			else if(i->alongPath.direction == IntPoint(1, 0))
			{
				left = left.rotation(270 * M_PI / 180.f);
			}
			IntPoint top = left.rotation(-90 * M_PI / 180.f);
			IntPoint right = left.rotation(-180 * M_PI / 180.f);
			IntPoint bottom = left.rotation(90 * M_PI / 180.f);
			
			IntPoint checkPoint1 = i->alongPath.point + left;
			IntPoint checkPoint2 = i->alongPath.point + top;
			IntPoint checkPoint3 = i->alongPath.point + right;
			if(myGD->mapState[checkPoint1.x][checkPoint1.y] == mapType::mapOldline)
			{
				i->alongPath.direction = left;
			}
			else if(myGD->mapState[checkPoint2.x][checkPoint2.y] == mapType::mapOldline)
			{
				i->alongPath.direction = top;
			}
			else if(myGD->mapState[checkPoint3.x][checkPoint3.y] == mapType::mapOldline)
			{
				i->alongPath.direction = right;
			}
			else // 아무곳도 갈곳이 없음.
			{
				i->alongPath.direction = bottom;
			}
		}
		if(erased == false)
			++i;
	}
}




