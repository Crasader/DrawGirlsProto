//
//  MapScanner.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 28..
//
//

#include "MapScanner.h"
#include <chrono>
#include <ctime>

void MapScanner::scanMap()
{
//	chrono::time_point<chrono::system_clock> start, end;
//	chrono::duration<double> elapsed_seconds;
//	start = chrono::system_clock::now();
	
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		if(myGD->mapState[i][mapHeightInnerBegin] == mapEmpty)
			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, mapHeightInnerBegin));
		if(myGD->mapState[i][mapHeightInnerEnd-1] == mapEmpty)
			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, mapHeightInnerEnd-1));
		if(myGD->game_step == kGS_limited)
		{
			if(myGD->mapState[i][myGD->limited_step_top] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, myGD->limited_step_top));
			if(myGD->mapState[i][myGD->limited_step_bottom] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, myGD->limited_step_bottom));
		}
	}
	
//	end = chrono::system_clock::now();
//	elapsed_seconds = end-start;
//	CCLog("process step 1 / time : %f", elapsed_seconds.count());
//	start = chrono::system_clock::now();
	
	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(myGD->mapState[mapWidthInnerBegin][j] == mapEmpty)
			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(mapWidthInnerBegin, j));
		if(myGD->mapState[mapWidthInnerEnd-1][j] == mapEmpty)
			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(mapWidthInnerEnd-1, j));
	}
	
//	end = chrono::system_clock::now();
//	elapsed_seconds = end-start;
//	CCLog("process step 2 / time : %f", elapsed_seconds.count());
//	start = chrono::system_clock::now();
	
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	
	// locked main cumber then reverse
	if(!mainCumberPoint.isNull() && myGD->mapState[mainCumberPoint.x][mainCumberPoint.y] == mapEmpty) // != mapScaningEmptySide
	{
		if(myGD->game_step == kGS_limited)
		{
			for(int j=mapHeightInnerBegin;j < myGD->limited_step_bottom;j++)
			{
				for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
				{
					if(myGD->mapState[i][j] == mapOutline)
						myGD->mapState[i][j] = mapNewget;
				}
			}
			for(int j=myGD->limited_step_top+1;j < mapHeightInnerEnd;j++)
			{
				for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
				{
					if(myGD->mapState[i][j] == mapOutline)
						myGD->mapState[i][j] = mapNewget;
				}
			}
		}
		
		bfsCheck(mapEmpty, mapScaningEmptySide, mainCumberPoint); // main cumber
		
		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			if(myGD->mapState[i][mapHeightInnerBegin] == mapScaningEmptySide)
				bfsCheck(mapScaningEmptySide, mapNewget, IntPoint(i, mapHeightInnerBegin));
			if(myGD->mapState[i][mapHeightInnerEnd-1] == mapScaningEmptySide)
				bfsCheck(mapScaningEmptySide, mapNewget, IntPoint(i, mapHeightInnerEnd-1));
		}
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(myGD->mapState[mapWidthInnerBegin][j] == mapScaningEmptySide)
				bfsCheck(mapScaningEmptySide, mapNewget, IntPoint(mapWidthInnerBegin, j));
			if(myGD->mapState[mapWidthInnerEnd-1][j] == mapScaningEmptySide)
				bfsCheck(mapScaningEmptySide, mapNewget, IntPoint(mapWidthInnerEnd-1, j));
		}
	}
	
//	end = chrono::system_clock::now();
//	elapsed_seconds = end-start;
//	CCLog("process step 3 / time : %f", elapsed_seconds.count());
//	start = chrono::system_clock::now();
	
	// new inside check
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(myGD->mapState[i][j] == mapEmpty)
				bfsCheck(mapEmpty, mapNewget, IntPoint(i, j));
		}
	}
	
//	end = chrono::system_clock::now();
//	elapsed_seconds = end-start;
//	CCLog("process step 4 / time : %f", elapsed_seconds.count());
//	start = chrono::system_clock::now();
	
	// outside recovery and new inside add show
	int newInsideCnt = 0;
	int sil_inside_cnt = 0;
	int empty_inside_cnt = 0;
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(myGD->mapState[i][j] == mapScaningEmptySide)
				myGD->mapState[i][j] = mapEmpty;
			else if(myGD->mapState[i][j] == mapNewline)
			{
				myGD->mapState[i][j] = mapOldline;
				newInsideCnt++;
				if(mySD->silData[i][j])		sil_inside_cnt++;
				else						empty_inside_cnt++;
			}
			else if(myGD->mapState[i][j] == mapNewget)
			{
				myGD->mapState[i][j] = mapOldget;
				newInsideCnt++;
				if(mySD->silData[i][j])		sil_inside_cnt++;
				else						empty_inside_cnt++;
			}
		}
	}
	
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(myGD->mapState[i][j] == mapOldline &&
			   myGD->mapState[i-1][j] != mapEmpty && myGD->mapState[i-1][j+1] != mapEmpty &&
			   myGD->mapState[i-1][j-1] != mapEmpty && myGD->mapState[i][j+1] != mapEmpty &&
			   myGD->mapState[i][j-1] != mapEmpty && myGD->mapState[i+1][j+1] != mapEmpty &&
			   myGD->mapState[i+1][j] != mapEmpty && myGD->mapState[i+1][j-1] != mapEmpty &&
			   
			   myGD->mapState[i-1][j] != mapOutline && myGD->mapState[i-1][j+1] != mapOutline &&
			   myGD->mapState[i-1][j-1] != mapOutline && myGD->mapState[i][j+1] != mapOutline &&
			   myGD->mapState[i][j-1] != mapOutline && myGD->mapState[i+1][j+1] != mapOutline &&
			   myGD->mapState[i+1][j] != mapOutline && myGD->mapState[i+1][j-1] != mapOutline)
			{
				myGD->mapState[i][j] = mapOldget;
			}
		}
	}
	
//	end = chrono::system_clock::now();
//	elapsed_seconds = end-start;
//	CCLog("process step 5 / time : %f", elapsed_seconds.count());
//	start = chrono::system_clock::now();
	
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		if(myGD->mapState[i][mapHeightInnerBegin] != mapEmpty && myGD->mapState[i][mapHeightInnerBegin] != mapOutline)
			myGD->mapState[i][mapHeightInnerBegin] = mapOldline;
		if(myGD->mapState[i][mapHeightInnerEnd-1] != mapEmpty && myGD->mapState[i][mapHeightInnerEnd-1] != mapOutline)
			myGD->mapState[i][mapHeightInnerEnd-1] = mapOldline;
	}
	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(myGD->mapState[mapWidthInnerBegin][j] != mapEmpty && myGD->mapState[mapWidthInnerBegin][j] != mapOutline)
			myGD->mapState[mapWidthInnerBegin][j] = mapOldline;
		if(myGD->mapState[mapWidthInnerEnd-1][j] != mapEmpty && myGD->mapState[mapWidthInnerEnd-1][j] != mapOutline)
			myGD->mapState[mapWidthInnerEnd-1][j] = mapOldline;
	}
	
//	end = chrono::system_clock::now();
//	elapsed_seconds = end-start;
//	CCLog("process step 6 / time : %f", elapsed_seconds.count());
	
	if(myGD->game_step == kGS_limited)
	{
		int total_cell = 0;
		int getted_cell = 0;
		for(int j=myGD->limited_step_bottom;j<=myGD->limited_step_top;j++)
		{
			for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
			{
				total_cell++;
				if(myGD->mapState[i][j] == mapOldget || myGD->mapState[i][j] == mapOldline)
					getted_cell++;
			}
		}
		
		if(1.f*getted_cell/total_cell > 0.4f)
			myGD->communication("Main_setUnlimitMap");
	}
	
	// new inside score add
	float rate;
	if(newInsideCnt < 100)			rate = 0.2f; // max 1/344 = 0.29%
	else if(newInsideCnt < 500)		rate = 0.3f; // max 5/344 = 1.45%
	else if(newInsideCnt < 1000)	rate = 0.4f; // max 10/344 = 2.9%
	else if(newInsideCnt < 3000)	rate = 0.5f; // max 30/344 = 8.7%
	else if(newInsideCnt < 5000)	rate = 0.6f; // max 50/344 = 14.5%
	else							rate = 0.8f; // ^
	
//	int addScore = newInsideCnt*rate;
	int addScore = (sil_inside_cnt*2 + empty_inside_cnt)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d)*rate;
	
	myGD->communication("UI_addScore", addScore);
	
	resetRects();
}

void MapScanner::resetRects()
{
//	chrono::time_point<chrono::system_clock> start, end;
//	chrono::duration<double> elapsed_seconds;
//	start = chrono::system_clock::now();
	
	// view rects reset
	CCArray* rects = CCArray::createWithCapacity(256);
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(myGD->mapState[i][j] == mapOldline || myGD->mapState[i][j] == mapOldget)
			{
				IntRect* t_rect = newRectChecking(IntMoveState(i, j, directionRightUp));
				rects->addObject(t_rect);
			}
		}
	}
	
//	end = chrono::system_clock::now();
//	elapsed_seconds = end-start;
//	CCLog("reset rects : %f", elapsed_seconds.count());
	
	visibleImg->setDrawRects(rects);
	
	float drawCellCnt = 0;
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(myGD->mapState[i][j] == mapScaningCheckLine)				myGD->mapState[i][j] = mapOldline;
			else if(myGD->mapState[i][j] == mapScaningCheckGet)			myGD->mapState[i][j] = mapOldget;
			
			if((myGD->mapState[i][j] == mapOldget || myGD->mapState[i][j] == mapOldline) && mySD->silData[i][j])				drawCellCnt++;
		}
	}
	
	myGD->communication("UI_setPercentage", float(drawCellCnt/mySD->must_cnt));
}

IntRect* MapScanner::newRectChecking(IntMoveState start)
{
	IntPoint origin = IntPoint(start.origin.x, start.origin.y);
	IntSize size = IntSize(0, 0);
	
	bool isUpper = true;
	bool isRighter = true;
	queue<IntMoveState> loopArray;
	loopArray.push(start);
	
	queue<IntMoveState> nextLoopArray;
	
//	int loopCnt;
	
	while(!loopArray.empty())
	{
		if(isUpper)				size.height++;
		if(isRighter)			size.width++;
		
		bool upable = isUpper;
		bool rightable = isRighter;
		
		while(!loopArray.empty())
		{
//			loopCnt++;
			IntMoveState t_ms = loopArray.front();
			loopArray.pop();
			
			
			if(t_ms.direction == directionUp && !isUpper)
				continue;
			if(t_ms.direction == directionRight && !isRighter)
				continue;
			
			if(myGD->mapState[t_ms.origin.x][t_ms.origin.y] == mapOldget)				myGD->mapState[t_ms.origin.x][t_ms.origin.y] = mapScaningCheckGet;
			else if(myGD->mapState[t_ms.origin.x][t_ms.origin.y] == mapOldline)			myGD->mapState[t_ms.origin.x][t_ms.origin.y] = mapScaningCheckLine;
			
			if(t_ms.direction == directionUp)
			{
				if(isUpper)
				{
					IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_msUp.origin.isInnerMap() && (myGD->mapState[n_msUp.origin.x][n_msUp.origin.y] == mapOldline || myGD->mapState[n_msUp.origin.x][n_msUp.origin.y] == mapOldget))
						nextLoopArray.push(n_msUp);
					else		upable = false;
				}
			}
			else if(t_ms.direction == directionRight)
			{
				if(isRighter)
				{
					IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_msRight.origin.isInnerMap() && (myGD->mapState[n_msRight.origin.x][n_msRight.origin.y] == mapOldline || myGD->mapState[n_msRight.origin.x][n_msRight.origin.y] == mapOldget))
						nextLoopArray.push(n_msRight);
					else		rightable = false;
				}
			}
			else if(t_ms.direction == directionRightUp)
			{
				if(isUpper)
				{
					IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
					if(n_msUp.origin.isInnerMap() && (myGD->mapState[n_msUp.origin.x][n_msUp.origin.y] == mapOldline || myGD->mapState[n_msUp.origin.x][n_msUp.origin.y] == mapOldget))
						nextLoopArray.push(n_msUp);
					else		upable = false;
				}
				
				if(isRighter)
				{
					IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
					if(n_msRight.origin.isInnerMap() && (myGD->mapState[n_msRight.origin.x][n_msRight.origin.y] == mapOldline || myGD->mapState[n_msRight.origin.x][n_msRight.origin.y] == mapOldget))
						nextLoopArray.push(n_msRight);
					else		rightable = false;
				}
				
				if(upable && rightable)
				{
					IntMoveState n_msRightUp = IntMoveState(t_ms.origin.x+1, t_ms.origin.y+1, directionRightUp);
					if(n_msRightUp.origin.isInnerMap() && (myGD->mapState[n_msRightUp.origin.x][n_msRightUp.origin.y] == mapOldline || myGD->mapState[n_msRightUp.origin.x][n_msRightUp.origin.y] == mapOldget))
						nextLoopArray.push(n_msRightUp);
					else		rightable = false;
				}
			}
		}
		
		isUpper = upable;
		isRighter = rightable;
		
		if(isUpper || isRighter)
		{
			while(!nextLoopArray.empty())
			{
				loopArray.push(nextLoopArray.front());
				nextLoopArray.pop();
			}
		}
	}
	
//	CCLog("loop count : %d", loopCnt);
	
	IntRect* r_rect = new IntRect((origin.x-1)*pixelSize, (origin.y-1)*pixelSize, size.width*pixelSize, size.height*pixelSize);
	r_rect->autorelease();
	return r_rect;
}

void MapScanner::bfsCheck(mapType beforeType, mapType afterType, IntPoint startPoint)
{
	BFS_Point s_p;
	s_p.x = startPoint.x;
	s_p.y = startPoint.y;
	queue<BFS_Point> bfsArray;
	myGD->mapState[s_p.x][s_p.y] = afterType;
	bfsArray.push(s_p);
	
	while(!bfsArray.empty())
	{
		BFS_Point t_p = bfsArray.front();
		bfsArray.pop();
		for(int i=directionLeft;i<=directionUp;i+=2)
		{
			BFS_Point t_v = directionVector((IntDirection)i);
			BFS_Point a_p;
			a_p.x = t_p.x+t_v.x;
			a_p.y = t_p.y+t_v.y;
			
			if(isInnerMap(a_p) && myGD->mapState[a_p.x][a_p.y] == beforeType)
			{
				myGD->mapState[a_p.x][a_p.y] = afterType;
				bfsArray.push(a_p);
			}
		}
	}
}
