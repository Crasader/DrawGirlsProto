#include "MissileUnit.h"


void AlongOfTheLine::myInit(CCPoint cumberPosition, CCPoint jackPosition, int totalFrame)
{
	m_step = 1;
	m_totalFrame = totalFrame;
	
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

void ReaverScarab::update(float dt)
{
	//		CCLog("pokjuk %d", m_frame);
	if(m_step == 1)
	{
		m_frame++;
		if(m_bulletReversePath.empty() == false && m_bulletIter != m_bulletReversePath.rend())
		{
			m_parentMissile->setPosition((*m_bulletIter));

			++m_bulletIter;
		}
		else
		{
			m_step = 2;
			crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), 10);
		}
//		aStar(m_jackPoint);
		

		if(m_frame >= 60*4)
		{
			m_step = 2;
			crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), 10);
		}
	}
	if(m_step == 2) // 폭발.
	{
		m_parentMissile->removeFromParent();
		schedule(schedule_selector(ThisClassType::selfRemove));
		CCParticleSystemQuad* particle = CCParticleSystemQuad::createWithTotalParticles(50);
		
		particle->setAutoRemoveOnFinish(true);
		particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("tickingTime_bomb.png");
		particle->setTexture(texture);
		particle->setEmissionRate(250.00);
		particle->setAngle(90.0);
		particle->setAngleVar(360.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		particle->setBlendFunc(blendFunc);
		particle->setDuration(0.20);
		particle->setEmitterMode(kCCParticleModeGravity);
		ccColor4F startColor = {0.87,0.81,0.12,1.00}; // 0.76 0.25 0.12
		particle->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0};
		particle->setStartColorVar(startColorVar);
		ccColor4F endColor = {0.68,0.16,0.00,1.00};
		particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		particle->setEndColorVar(endColorVar);
		particle->setStartSize(20.00);
		particle->setStartSizeVar(10.0);
		particle->setEndSize(40.0);
		particle->setEndSizeVar(10.0);
		particle->setGravity(ccp(0,0));
		particle->setRadialAccel(0.0);
		particle->setRadialAccelVar(0.0);
		particle->setSpeed(250);
		particle->setSpeedVar(60.0);
		particle->setTangentialAccel(0);
		particle->setTangentialAccelVar(0);
		particle->setTotalParticles(50);
		particle->setLife(0.30);
		particle->setLifeVar(0.0);
		particle->setStartSpin(0.0);
		particle->setStartSpinVar(0.0);
		particle->setEndSpin(0.0);
		particle->setEndSpinVar(0.0);
		particle->setPosition(m_parentMissile->getPosition());
		particle->setPosVar(CCPointZero);
		addChild(particle);
		m_step = 3;
	}
}


void ReaverScarab::aStar(IntPoint endPt)
{
	chrono::time_point<chrono::system_clock> start, end;
	chrono::duration<double> elapsed_seconds;
	start = chrono::system_clock::now();

	
	
	
	
	IntPoint startPoint = ccp2ip(m_parentMissile->getPosition());

	m_openList = decltype(m_openList)();
	m_closeListMap.clear();

	m_bulletReversePath.clear();
	auto ret = CoordAndCellInfo(startPoint.x, startPoint.y, 0, 0, 0, lengthToEnd(startPoint));
	m_openList.insert(ret);
//	m_openList[startPoint] = CellInfo(0, 0, 0, lengthToEnd(startPoint));
	
	bool foundSolution = false;
	int counter = 0;
	

	while(m_openList.size() > 0)
	{
		counter++;
		if(counter >= 1000)
			break;
		auto minElement = *m_openList.begin();
		
		
		IntPoint fminIndex = IntPoint(minElement.x, minElement.y);
		CellInfo fminElement = CellInfo(minElement.dx, minElement.dy, minElement.g, minElement.h);
		
		m_closeListMap[fminIndex] = fminElement;

		m_openList.erase(m_openList.begin());
//		m_openList.erase(fminIndex);
		
//		CCLog("%d %d -> %d %d", fminIndex.x, fminIndex.y, endPoint.x, endPoint.y);
		if(fminIndex == endPt)
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
	KS::KSLog("process step 1 / time : %, %", elapsed_seconds1.count(), counter);

	
	IntPoint coordNearEndPoint = m_jackPoint;
	int minDistance = INT_MAX;
	for(auto obj : m_closeListMap)
	{
		if(minDistance > (obj.first - m_jackPoint).length())
		{
			minDistance = (obj.first - m_jackPoint).length();
			coordNearEndPoint = obj.first;
		}
	}
	
	
	
	if(m_closeListMap.find(coordNearEndPoint) != m_closeListMap.end())
	{
		IntPoint currentPoint = coordNearEndPoint;
		IntPoint beforePoint = coordNearEndPoint;
		while(!(currentPoint == startPoint))
		{
			IntPoint coord = currentPoint;
			m_bulletReversePath.push_back(ip2ccp(currentPoint));
			beforePoint = currentPoint;
			currentPoint.x += m_closeListMap[coord].dx;
			currentPoint.y += m_closeListMap[coord].dy;
		}
		m_bulletIter = m_bulletReversePath.rbegin();
		while(0);
	}

//	int t = ;
#if 0 // 탐색하는 과정 보임.
	ccColor3B c3 = ccc3(m_well512.GetValue(0, 255),m_well512.GetValue(0, 255),m_well512.GetValue(0, 255));
	for(auto cl : m_closeListMap)
	{
		CCSprite* rt = CCSprite::create();
		rt->setTextureRect(CCRectMake(0, 0, 2, 2));
		rt->setColor(c3);
		rt->setPosition(ip2ccp(cl.first));
//		rt->setOpacity(100);
		addChild(rt);
		rt->runAction(CCSequence::create(CCDelayTime::create(4.f), CCRemoveSelf::create(), 0));
	}
#endif
	// alert("complete!!");
}

// from 에서 to 로 퍼져나갈 때 블럭 처리.
bool ReaverScarab::processObject(IntPoint pointFrom, IntPoint pointTo, int distance)
{
	if(pointTo.x < mapLoopRange::mapWidthInnerBegin || pointTo.x >= mapLoopRange::mapWidthInnerEnd ||
		 pointTo.y < mapLoopRange::mapHeightInnerBegin || pointTo.y >= mapLoopRange::mapHeightInnerEnd)
		return true;
	// 장애물이면 패스.
	
	if(!(myGD->getJackPoint() == pointTo))
	{
		if(myGD->mapState[pointTo.x][pointTo.y] == mapType::mapOldline ||
			 myGD->mapState[pointTo.x][pointTo.y] == mapType::mapOutline)
			return true;
		if(myGD->mapState[pointTo.x][pointFrom.y] == mapType::mapOldline ||
			 myGD->mapState[pointTo.x][pointFrom.y] == mapType::mapOutline )
			return true;
		if(myGD->mapState[pointFrom.x][pointTo.y] == mapType::mapOldline ||
			 myGD->mapState[pointFrom.x][pointTo.y] == mapType::mapOutline)
			return true;
	}
	
	bool found = false;
	auto openedIter = m_openList.find(CoordAndCellInfo(pointTo.x, pointTo.y, 0, 0, 0, 0));
	
	auto closedIter = m_closeListMap.find(pointTo);
	// 닫힌 쪽에 있지 않고 열린 쪽이라면
	if(openedIter != m_openList.end() && closedIter == m_closeListMap.end() )
	{
		m_openList.erase(openedIter);
		auto openedValue = *openedIter;
//		auto smart = ProbSelector({1,50}).getResult();
//		smart = 1;
		if(openedValue.g > m_closeListMap[pointFrom].g + distance)
		{
			openedValue.dx = pointFrom.x - openedValue.x;
			openedValue.dy = pointFrom.y - openedValue.y;
			openedValue.g = m_closeListMap[pointFrom].g + distance;
		}

		m_openList.insert(openedValue);
		return true;
	}
	
	// 닫힌쪽도 아니고 열린쪽도 아니면!
	if(closedIter == m_closeListMap.end() && openedIter == m_openList.end())
	{
		CellInfo pointToObj(0, 0, 0, 0);
		//		CellInfo pointToObj = {g:0, h:0, dx:0, dy:0};
		pointToObj.g = m_closeListMap[pointFrom].g + distance;
		pointToObj.h = lengthToEnd(pointTo);
		pointToObj.dx = pointFrom.x - pointTo.x; // 부모 가르키기.
		pointToObj.dy = pointFrom.y - pointTo.y; // 부모 가르키기.
		m_insertCount++;

		auto obj = CoordAndCellInfo(pointTo.x, pointTo.y, pointToObj.dx, pointToObj.dy, pointToObj.g, pointToObj.h);
		obj.order = m_insertCount;
		m_openList.insert(obj);
//		m_openList[pointTo] = pointToObj;
	}
	return true;
}



