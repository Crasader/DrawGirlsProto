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
					CCLog("path == %d %d, %d %d", path.point.x, path.point.y, ip.x, ip.y);
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
						
						if(myGD->mapState[checkPoint4.x][checkPoint4.y] == mapType::mapOldline)
							bfs.push(AlongPath(checkPoint4, bottom));
						else
							break;
					}
				}
				CCLog("end while");
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

// from 에서 to 로 퍼져나갈 때 블럭 처리.
bool ReaverScarab::processObject(IntPoint pointFrom, IntPoint pointTo, int distance)
{
	// 주위의블럭을 처리하는데, 열려있는 공간에 들어가있다면 g 가 갱신될 수 있는지 알아봄
	// 열린 공간에도 없고 닫힌 공간에도 없으면 g 와 h 를 추론함.
	if(pointTo == endPoint)
	{
//		cc.log("corr");
	}
	
	if(pointTo.x < mapLoopRange::mapWidthInnerBegin || pointTo.x >= mapLoopRange::mapWidthInnerEnd ||
		 pointTo.y < mapLoopRange::mapHeightInnerBegin || pointTo.y >= mapLoopRange::mapHeightInnerEnd)
		return true;
	// 장애물이면 패스.
	
	if(myGD->mapState[pointTo.x][pointTo.y] == mapType::mapOldline && !(myGD->getJackPoint() == pointTo))
		return true;
	if(myGD->mapState[pointTo.x][pointFrom.y] == mapType::mapOldline &&
		 !(myGD->getJackPoint() == IntPoint(pointTo.x, pointFrom.y)))
		return true;
	if(myGD->mapState[pointFrom.x][pointTo.y] == mapType::mapOldline &&
		 !(myGD->getJackPoint() == IntPoint(pointFrom.x, pointTo.y)))
		return true;
	
	bool found = false;
	auto openedIter = m_openList.find(pointTo);
	if(openedIter != m_openList.end())
	{
		found = true;
	}
	// opened
	if(found)
	{
		if(openedIter->second.g > m_closeList[pointFrom].g + distance)
		{
			openedIter->second.dx = pointFrom.x - openedIter->first.x;
			openedIter->second.dy = pointFrom.y - openedIter->first.y;
		}
		openedIter->second.g = min(openedIter->second.g, m_closeList[pointFrom].g + distance);
		return true;
	}
	
	
	found = false;
	auto closedIter = m_openList.find(pointTo);
	if(closedIter != m_openList.end())
	{
		found = true;
	}
	
	// 닫힌 공간에 없으면
	if(found == false)
	{
		CellInfo pointToObj(0, 0, 0, 0);
//		CellInfo pointToObj = {g:0, h:0, dx:0, dy:0};
		pointToObj.g = m_closeList[pointFrom].g + distance;
		pointToObj.h = lengthToEnd(pointTo);
		pointToObj.dx = pointFrom.x - pointTo.x; // 부모 가르키기.
		pointToObj.dy = pointFrom.y - pointTo.y; // 부모 가르키기.
		
		m_openList[pointTo] = pointToObj;
	}
	return true;
}

void ReaverScarab::aStar(IntPoint endPt)
{
	//		endPt.x = parseInt(endPt.x);
	//		endPt.y = parseInt(endPt.y);
	
	//		m_parentMissile->setPosition(ccp((int)(m_parentMissile->getPosition().x),
	//																		 (int)(m_parentMissile->getPosition().y)));
	
	chrono::time_point<chrono::system_clock> start, end;
	chrono::duration<double> elapsed_seconds;
	start = chrono::system_clock::now();

	
	
	
	
	IntPoint startPoint = ccp2ip(m_parentMissile->getPosition());
	endPoint = endPt;
	
//	startPoint = IntPoint(2, 100);
//	endPoint = IntPoint(4, 104);
	m_openList[startPoint] = CellInfo(0, 0, 0, lengthToEnd(startPoint));
	
	bool foundSolution = false;
	while(m_openList.size() > 0)
	{
//		auto minElement = std::min_element(m_openList.begin(), m_openList.end());
		auto minElement = std::min_element(m_openList.begin(), m_openList.end(),
																			 [](const std::pair<IntPoint, CellInfo>& a, const std::pair<IntPoint, CellInfo>& b)->bool
																			 {
																				 return a.second.g + a.second.h < b.second.g + b.second.h;
																			 });
		IntPoint fminIndex = minElement->first;
		CellInfo fminElement = minElement->second;
		
		m_closeList[fminIndex] = fminElement;
		m_openList.erase(fminIndex);
		
		CCLog("%d %d", fminIndex.x, fminIndex.y);
		if(fminIndex == endPoint)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x - 1, fminIndex.y + 1), 14);
		if(foundSolution)
			break;
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x, fminIndex.y + 1), 10);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x + 1, fminIndex.y + 1), 14);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x - 1, fminIndex.y), 10);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x + 1, fminIndex.y), 10);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x - 1, fminIndex.y - 1), 14);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x, fminIndex.y - 1), 10);
		if(foundSolution)
			break;
		
		foundSolution = !processObject(fminIndex, IntPoint(fminIndex.x + 1, fminIndex.y - 1), 14);
		if(foundSolution)
			break;
	}
	
	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds1 = end-start;
	KS::KSLog("process step 1 / time : %", elapsed_seconds1.count());

		
	if(m_closeList.find(endPoint) != m_closeList.end())
	{
		IntPoint currentPoint = endPoint;
		while(!(currentPoint == startPoint))
		{
			IntPoint coord = currentPoint;
			currentPoint.x += m_closeList[coord].dx;
			currentPoint.y += m_closeList[coord].dy;
			myGD->mapState[currentPoint.x][currentPoint.y] = mapType::mapOldline;
			//var bulletPath = cc.Sprite.create("res/CloseSelected.png");
			//this.addChild(bulletPath);
			//bulletPath.setPosition(currentPoint);
		}
	}
	// alert("complete!!");
}
