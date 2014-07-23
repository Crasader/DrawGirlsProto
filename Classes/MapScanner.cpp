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
#include "DataStorageHub.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "TextureReloader.h"

bool MapScanner::isCheckBossLocked()
{
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
	
	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(myGD->mapState[mapWidthInnerBegin][j] == mapEmpty)
			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(mapWidthInnerBegin, j));
		if(myGD->mapState[mapWidthInnerEnd-1][j] == mapEmpty)
			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(mapWidthInnerEnd-1, j));
	}
	
	vector<CCNode*> main_cumber_vector = myGD->getMainCumberCCNodeVector();
	int main_cumber_count = main_cumber_vector.size();
	bool is_found = false;
	IntPoint mainCumberPoint = IntPoint();
	for(int i=0;i<main_cumber_count && !is_found;i++)
	{
		CCNode* t_boss = main_cumber_vector[i];
		IntPoint t_boss_point = myGD->getMainCumberPoint(t_boss);
		if(!t_boss_point.isNull() && myGD->mapState[t_boss_point.x][t_boss_point.y] == mapScaningEmptySide)
		{
			is_found = true;
			mainCumberPoint = t_boss_point;
		}
	}
	
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		if(myGD->mapState[i][mapHeightInnerBegin] == mapScaningEmptySide)
			bfsCheck(mapScaningEmptySide, mapEmpty, IntPoint(i, mapHeightInnerBegin));
		if(myGD->mapState[i][mapHeightInnerEnd-1] == mapScaningEmptySide)
			bfsCheck(mapScaningEmptySide, mapEmpty, IntPoint(i, mapHeightInnerEnd-1));
	}
	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(myGD->mapState[mapWidthInnerBegin][j] == mapScaningEmptySide)
			bfsCheck(mapScaningEmptySide, mapEmpty, IntPoint(mapWidthInnerBegin, j));
		if(myGD->mapState[mapWidthInnerEnd-1][j] == mapScaningEmptySide)
			bfsCheck(mapScaningEmptySide, mapEmpty, IntPoint(mapWidthInnerEnd-1, j));
	}
	
	return is_found;
}

void MapScanner::ingNewlineToRealNewline()
{
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(myGD->mapState[i][j] == mapNewline)
				myGD->mapState[i][j] = mapRealNewline;
		}
	}
}

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
//	CCLOG("process step 1 / time : %f", elapsed_seconds.count());
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
//	CCLOG("process step 2 / time : %f", elapsed_seconds.count());
//	start = chrono::system_clock::now();
	
	vector<CCNode*> main_cumber_vector = myGD->getMainCumberCCNodeVector();
	int main_cumber_count = main_cumber_vector.size();
	bool is_found = false;
	IntPoint mainCumberPoint = IntPoint();
	for(int i=0;i<main_cumber_count && !is_found;i++)
	{
		CCNode* t_boss = main_cumber_vector[i];
		IntPoint t_boss_point = myGD->getMainCumberPoint(t_boss);
		if(!t_boss_point.isNull() && myGD->mapState[t_boss_point.x][t_boss_point.y] == mapScaningEmptySide)
		{
			is_found = true;
			mainCumberPoint = t_boss_point;
		}
	}
	
	
//	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	
	// locked main cumber then reverse
	if(!is_found) // != mapScaningEmptySide
	{
		for(int i=0;i<main_cumber_count;i++)
		{
			CCNode* t_boss = main_cumber_vector[i];
			IntPoint t_boss_point = myGD->getMainCumberPoint(t_boss);
			if(!t_boss_point.isNull() && myGD->mapState[t_boss_point.x][t_boss_point.y] == mapEmpty)
			{
				mainCumberPoint = t_boss_point;
			}
		}
		//CCAssert(!mainCumberPoint.isNull(), "what?");
		if(!mainCumberPoint.isNull())
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
	}
	
//	end = chrono::system_clock::now();
//	elapsed_seconds = end-start;
//	CCLOG("process step 3 / time : %f", elapsed_seconds.count());
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
//	CCLOG("process step 4 / time : %f", elapsed_seconds.count());
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
			else if(myGD->mapState[i][j] == mapRealNewline)
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
//	CCLOG("process step 5 / time : %f", elapsed_seconds.count());
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
//	CCLOG("process step 6 / time : %f", elapsed_seconds.count());
	
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
		
		if(1.f*getted_cell/total_cell > 0.3f)
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
//	int addScore = (sil_inside_cnt*2 + empty_inside_cnt)*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d)*rate;
	
	float take_area_rate = newInsideCnt*100.f/(160*215);
	
	int addScore = (take_area_rate*1000 + sqrtf(take_area_rate/20*1000*4))*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d);
	
	mySGD->area_score = mySGD->area_score.getV() + addScore;
	
	myGD->communication("UI_addScore", addScore);
	
	resetRects(true);
}

void MapScanner::resetRects(bool is_after_scanmap)
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
//	CCLOG("reset rects : %f", elapsed_seconds.count());
	
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
	
	myGD->communication("UI_setPercentage", float(drawCellCnt/mySD->must_cnt), is_after_scanmap);
	
	if(mySGD->is_write_replay)
	{
		if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() > 0)
			myGD->communication("UI_checkMapTimeVector");
		else
			myGD->communication("UI_writeMap");
	}
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
	
//	CCLOG("loop count : %d", loopCnt);
	
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
	
	vector<BFS_Point> check_new_line_list;
	check_new_line_list.clear();
	
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
			
			if(isInnerMap(a_p))
			{
				if(myGD->mapState[a_p.x][a_p.y] == beforeType)
				{
					myGD->mapState[a_p.x][a_p.y] = afterType;
					bfsArray.push(a_p);
				}
				else if(myGD->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
				{
					check_new_line_list.push_back(a_p);
					bfsArray.push(a_p);
				}
			}
		}
	}
}

MapScanner* MapScanner::create()
{
	MapScanner* myMS = new MapScanner();
	myMS->myInit();
	myMS->autorelease();
	return myMS;
}

void MapScanner::visit()
{
	glEnable(GL_SCISSOR_TEST);

	int viewport [4];
	glGetIntegerv (GL_VIEWPORT, viewport);
	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float rate = frame_size.height/frame_size.width;
	CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
	float wScale = viewport[2] / rSize.width;
	float hScale = viewport[3] / rSize.height;

	float x = 0*wScale + viewport[0];
	float y = 0*hScale + viewport[1];
	float w = 480*wScale;
	float h = 320*(rate/(320.f/480.f))*hScale;

	glScissor(x,y,w,h);

	CCNode::visit();

	glDisable(GL_SCISSOR_TEST);
}

void MapScanner::exchangeMS()
{
	top_boarder->removeFromParent();
	top_boarder = CCSprite::create("frame_top.png");
	top_boarder->setAnchorPoint(ccp(0.5,0));
	top_boarder->setPosition(ccp(160,430));
	addChild(top_boarder, boarderZorder);
	
	bottom_boarder->removeFromParent();
	bottom_boarder = CCSprite::create("frame_bottom.png");
	bottom_boarder->setAnchorPoint(ccp(0.5,1));
	bottom_boarder->setPosition(ccp(160,0));
	addChild(bottom_boarder, boarderZorder);
	
	left_boarder->removeFromParent();
	left_boarder = CCSprite::create("frame_left.png");
	left_boarder->setAnchorPoint(ccp(1,0.5));
	left_boarder->setPosition(ccp(0,215));
	addChild(left_boarder, boarderZorder);
	
	right_boarder->removeFromParent();
	right_boarder = CCSprite::create("frame_right.png");
	right_boarder->setAnchorPoint(ccp(0,0.5));
	right_boarder->setPosition(ccp(320,215));
	addChild(right_boarder, boarderZorder);
	
	if(invisibleImg)
	{
		invisibleImg->removeFromParentAndCleanup(true);
		invisibleImg = NULL;
	}

	invisibleImg = InvisibleSprite::create(CCString::createWithFormat("card%d_invisible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 2))->getCString(), false);
	invisibleImg->setPosition(CCPointZero);
//	int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
//	t_spr->setColorSilhouette(NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d));
	invisibleImg->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(0.5f),CCFadeIn::create(0.5f),nullptr)));
	

	addChild(invisibleImg, invisibleZorder);
	
	// ######################## hs code bbu woo~ ##############################
	EffectSprite* t_spr = (EffectSprite*)invisibleImg->getChildByTag(8706);
	int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	int orir =NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d);
	int orig =NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d);
	int orib =NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d);
	
	float rm = orir/20.f;
	float gm =  orig/20.f;
	float bm = orib/20.f;
	addChild(KSSchedule::create([=](float dt){
		static float r=0;
		static float g=0;
		static float b=0;
		static int mark=1;
		if(mark){
			r+=rm;
			g+=gm;
			b+=bm;
			if(r>orir || b>orib || g>orig)mark=0;
		}else{
			r-=rm;
			g-=gm;
			b-=bm;
			if(r<0 || g<0 || b<0)mark=1;
		}
		
		t_spr->setColorSilhouette(r, g, b);
		return true;
	}));
	// ######################## hs code bbu woo~ ##############################
	
	
	CCArray* t_rect_array = new CCArray();

	if(visibleImg)
	{
		t_rect_array->initWithArray(visibleImg->getDrawRects());
		visibleImg->removeFromParentAndCleanup(true);
		visibleImg = NULL;
	}

	visibleImg = VisibleParent::create(CCString::createWithFormat("card%d_visible.png",NSDS_GI(silType, kSDS_SI_level_int1_card_i, 2))->getCString(), false, CCString::createWithFormat("card%d_invisible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 2))->getCString());
	visibleImg->setPosition(CCPointZero);
	addChild(visibleImg, visibleZorder);

	visibleImg->setDrawRects(t_rect_array);

	t_rect_array->autorelease();

	scanMap();
}

void MapScanner::randomingRectView( CCPoint t_p )
{
//	if(my_tic_toc)
//		AudioEngine::sharedInstance()->playEffect("sound_casting_attack.mp3", false);

	my_tic_toc = !my_tic_toc;

	int base_value = roundf(-t_p.y/((480.f-myGD->boarder_value*2)/(320.f))/2.f); // 중간 괄호 : myGD->game_scale

	int gacha_cnt = mySGD->getStartMapGachaCnt();

	if(gacha_cnt)
		if(gacha_cnt > 1)
			gacha_cnt = 1;

	gacha_cnt = rand()%(gacha_cnt*5+1);

	if(gacha_cnt <= 3)
	{
		random_device rd;
		default_random_engine e1(rd());
		uniform_int_distribution<int> uniform_dist(0, 24);

		init_rect.size.width = uniform_dist(e1) + 6;//rand()%(maxSize.width-minSize.width + 1) + minSize.width;
		init_rect.size.height = uniform_dist(e1) + 6;//rand()%(maxSize.height-minSize.height + 1) + minSize.height
	}
	else if(gacha_cnt <= 5)
	{
		random_device rd;
		default_random_engine e1(rd());
		uniform_int_distribution<int> uniform_dist(0, 30);

		init_rect.size.width = uniform_dist(e1) + 50;//rand()%(maxSize.width-minSize.width + 1) + minSize.width; // rand()%(60-30 + 1)
		init_rect.size.height = uniform_dist(e1) + 40;//rand()%(maxSize.height-minSize.height + 1) + minSize.height
	}

	IntPoint maxPoint = IntPoint(mapWidthInnerEnd-init_rect.size.width-2-mapWidthInnerBegin-20, init_rect.size.height-2);

	init_rect.origin.x = rand()%maxPoint.x+10;//mapWidthInnerBegin+10;
	init_rect.origin.y = rand()%maxPoint.y+base_value+roundf(screen_height/((480.f-myGD->boarder_value*2)/(320.f))/2.f)-init_rect.size.height+1; // 중간 괄호 : myGD->game_scale

	if(!random_rect_img)
	{
		random_rect_img = CCSprite::create("whitePaper.png");
		random_rect_img->setColor(ccGRAY);
		addChild(random_rect_img, blockZorder);
	}

	random_rect_img->setTextureRect(CCRectMake(0, 0, init_rect.size.width*2.f, init_rect.size.height*2.f));
	random_rect_img->setPosition(ccp(init_rect.origin.x*pixelSize + init_rect.size.width, init_rect.origin.y*pixelSize-1 + init_rect.size.height));
}

void MapScanner::stopRandomingRectView()
{
	random_rect_img->removeFromParentAndCleanup(true);
	myGD->initUserSelectedStartRect(init_rect);
}

void MapScanner::startGame()
{
	start_map_lucky_item->checkInnerRect();
}

BFS_Point MapScanner::directionVector( IntDirection direction )
{
	BFS_Point r_v;
	if(direction == directionLeftUp)			{		r_v.x = -1;		r_v.y = 1;		}
	else if(direction == directionLeft)			{		r_v.x = -1;		r_v.y = 0;		}
	else if(direction == directionLeftDown)		{		r_v.x = -1;		r_v.y = -1;		}
	else if(direction == directionDown)			{		r_v.x = 0;		r_v.y = -1;		}
	else if(direction == directionRightDown)	{		r_v.x = 1;		r_v.y = -1;		}
	else if(direction == directionRight)		{		r_v.x = 1;		r_v.y = 0;		}
	else if(direction == directionRightUp)		{		r_v.x = 1;		r_v.y = 1;		}
	else if(direction == directionUp)			{		r_v.x = 0;		r_v.y = 1;		}
	else										{		r_v.x = 0;		r_v.y = 0;		}
	return r_v;
}

bool MapScanner::isInnerMap( BFS_Point t_p )
{
	if(t_p.x >= mapWidthInnerBegin && t_p.x < mapWidthInnerEnd && t_p.y >= mapHeightInnerBegin && t_p.y < mapHeightInnerEnd)
		return true;
	else
		return false;
}

BackFilename MapScanner::getBackVisibleFilename()
{
	BackFilename r_value;

	r_value.filename = CCString::createWithFormat("card%d_visible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 1))->getCString();
	r_value.isPattern = false;

	return r_value;
}

BackFilename MapScanner::getBackInvisibleFilename()
{
	BackFilename r_value;

	r_value.filename = CCString::createWithFormat("card%d_invisible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 1))->getCString();
	r_value.isPattern = false;

	return r_value;
}

void MapScanner::setMapImg()
{
	CCSprite* silhouette_back = CCSprite::create("ingame_cardback.png");
	silhouette_back->setPosition(ccp(160,215));
	addChild(silhouette_back);
	
	if(invisibleImg)
	{
		invisibleImg->release();
		invisibleImg = NULL;
	}

	BackFilename visible_filename = getBackVisibleFilename();
	BackFilename invisible_filename = getBackInvisibleFilename();

	
	invisibleImg = InvisibleSprite::create(invisible_filename.filename.c_str(), invisible_filename.isPattern);
	invisibleImg->setPosition(CCPointZero);
	addChild(invisibleImg, invisibleZorder);

	
	if(visibleImg)
	{
		visibleImg->release();
		visibleImg = NULL;
	}

	visibleImg = VisibleParent::create(visible_filename.filename.c_str(), visible_filename.isPattern, invisible_filename.filename);
	visibleImg->setPosition(CCPointZero);
	addChild(visibleImg, visibleZorder);

	
	if(blockParent)
	{
		blockParent->release();
		blockParent = NULL;
	}

	blockParent = CCNode::create();
	addChild(blockParent, blockZorder);

	
	top_boarder = CCSprite::create("normal_frame_top.png");
	top_boarder->setAnchorPoint(ccp(0.5,0));
	top_boarder->setPosition(ccp(160,430));
	addChild(top_boarder, boarderZorder);

	bottom_boarder = CCSprite::create("normal_frame_bottom.png");
	bottom_boarder->setAnchorPoint(ccp(0.5,1));
	bottom_boarder->setPosition(ccp(160,0));
	addChild(bottom_boarder, boarderZorder);

	left_boarder = CCSprite::create("normal_frame_left.png");
	left_boarder->setAnchorPoint(ccp(1,0.5));
	left_boarder->setPosition(ccp(0,215));
	addChild(left_boarder, boarderZorder);

	right_boarder = CCSprite::create("normal_frame_right.png");
	right_boarder->setAnchorPoint(ccp(0,0.5));
	right_boarder->setPosition(ccp(320,215));
	addChild(right_boarder, boarderZorder);
	
}

void MapScanner::setTopBottomBlock()
{
	float top_y = (myGD->limited_step_top-1)*pixelSize;
	float bottom_y = (myGD->limited_step_bottom-1)*pixelSize+2.f;

	
//	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//	if(screen_scale_x < 1.f)
//		screen_scale_x = 1.f;
//	
//	float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//	CCSprite* stencil_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 430, 320));
//	stencil_node->setRotation(90);
//	stencil_node->setPosition(ccp(160,215));
//	CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
//	float change_scale = 1.f;
//	CCPoint change_origin = ccp(0,0);
//	if(screen_scale_x > 1.f)
//	{
//		change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
//		change_scale = screen_scale_x;
//	}
//	if(screen_scale_y > 1.f)
//		change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
//	CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//	cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
//	cliping_node->setAlphaThreshold(0.05f);
//	cliping_node->setPosition(CCPointZero);
//	addChild(cliping_node, blockZorder);
	
	CCSprite* t_spr = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 32, 27));
	
	top_block_manager = CCSpriteBatchNode::createWithTexture(t_spr->getTexture());//"temp_block.png");
	top_block_manager->setPosition(CCPointZero);
	addChild(top_block_manager, blockZorder);
	
	top_block_line_manager = CCSpriteBatchNode::create("temp_block_crossline.png");
	top_block_line_manager->setPosition(CCPointZero);
	addChild(top_block_line_manager, blockZorder);

	CCSize block_size = top_block_manager->getTexture()->getContentSize();
	
	CCSize line_size = top_block_line_manager->getTexture()->getContentSize();
	
	int line_cnt = 0;
	for(int i=0;i*line_size.width < 320 - line_size.width;i++)
	{
		line_cnt++;
		CCSprite* t_line = CCSprite::createWithTexture(top_block_line_manager->getTexture());
		t_line->setAnchorPoint(ccp(0,0));
		t_line->setPosition(ccp(i*line_size.width,top_y-1));
		//	t_line->setTag(top_cnt*8);
		top_block_line_manager->addChild(t_line);
	}
	{
		CCSprite* t_line = CCSprite::createWithTexture(top_block_line_manager->getTexture(), CCRectMake(0, 0, 320-line_cnt*line_size.width, line_size.height));
		t_line->setAnchorPoint(ccp(0,0));
		t_line->setPosition(ccp(line_cnt*line_size.width, top_y-1));
		top_block_line_manager->addChild(t_line);
	}
	
	int top_cnt = 0;
	while(top_y < 430)
	{
		top_cnt++;
		if(top_y+block_size.height >= 430)
		{
			float sub_value = top_y+block_size.height-430;
			for(int i=0;i<10;i++) // float 문제때문에
			{
				CCSprite* t_block = CCSprite::createWithTexture(top_block_manager->getTexture(), CCRectMake(0, sub_value, block_size.width, block_size.height-sub_value));
				t_block->setColor(ccBLACK);
				t_block->setOpacity(180);
				t_block->setAnchorPoint(ccp(0,0));
				t_block->setPosition(ccp(i*block_size.width, top_y+2));
				t_block->setTag(top_cnt*10+i); // float 문제때문에
				top_block_manager->addChild(t_block);
			}
		}
		else
		{
			for(int i=0;i<10;i++) // float 문제때문에
			{
				CCSprite* t_block = CCSprite::createWithTexture(top_block_manager->getTexture());
				t_block->setColor(ccBLACK);
				t_block->setOpacity(180);
				t_block->setAnchorPoint(ccp(0,0));
				t_block->setPosition(ccp(i*block_size.width, top_y+2));
				t_block->setTag(top_cnt*10+i); // float 문제때문에
				top_block_manager->addChild(t_block);
			}
		}
		
		top_y += block_size.height;
	}
	top_block_manager->setTag(top_cnt);
	
	if((myGD->limited_step_top-1)*pixelSize + 31 > 430) // 31 : "temp_block_lock.png" height
	{
		float sub_value = (myGD->limited_step_top-1)*pixelSize + 31 - 430;
		top_block_lock = CCSprite::create("temp_block_lock.png", CCRectMake(0, sub_value, 74, 31-sub_value)); // 74 : "temp_block_lock.png" width
		top_block_lock->setAnchorPoint(ccp(0.5,0));
		top_block_lock->setPosition(ccp(160,(myGD->limited_step_top-1)*pixelSize));
		addChild(top_block_lock, blockZorder);
		
		CCClippingNode* t_clipping = CCClippingNode::create(CCSprite::create("temp_block_lock.png", CCRectMake(0, sub_value, top_block_lock->getContentSize().width, top_block_lock->getContentSize().height-sub_value)));
		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//		
//		float change_scale = 1.f;
//		CCPoint change_origin = ccp(0,0);
//		if(screen_scale_x > 1.f)
//		{
//			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
//			change_scale = screen_scale_x;
//		}
//		if(screen_scale_y > 1.f)
//			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
//		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		
		t_clipping->getStencil()->setAnchorPoint(ccp(0.5,0));
		t_clipping->setPosition(ccp(top_block_lock->getContentSize().width/2.f,0));
		top_block_lock->addChild(t_clipping);
		
		t_clipping->setAlphaThreshold(0.1f);
		
		KSLabelTTF* lock_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_locked), mySGD->getFont().c_str(), 15);
		lock_label->disableOuterStroke();
		lock_label->setAnchorPoint(ccp(0.5f,0.5f));
		lock_label->setPosition(ccp(0,31/2.f));
		t_clipping->addChild(lock_label);
	}
	else
	{
		top_block_lock = CCSprite::create("temp_block_lock.png");
		top_block_lock->setAnchorPoint(ccp(0.5,0));
		top_block_lock->setPosition(ccp(160,(myGD->limited_step_top-1)*pixelSize));
		addChild(top_block_lock, blockZorder);
		
		KSLabelTTF* lock_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_locked), mySGD->getFont().c_str(), 15);
		lock_label->disableOuterStroke();
		lock_label->setPosition(ccp(top_block_lock->getContentSize().width/2.f,top_block_lock->getContentSize().height/2.f));
		top_block_lock->addChild(lock_label);
	}

	bottom_block_manager = CCSpriteBatchNode::createWithTexture(t_spr->getTexture());//"temp_block.png");
	bottom_block_manager->setPosition(CCPointZero);
	addChild(bottom_block_manager, blockZorder);
	
	bottom_block_line_manager = CCSpriteBatchNode::create("temp_block_crossline.png");
	bottom_block_line_manager->setPosition(CCPointZero);
	addChild(bottom_block_line_manager, blockZorder);

	int bottom_cnt = 0;
	{
		int line_cnt = 0;
		for(int i=0;i*line_size.width < 320 - line_size.width;i++)
		{
			line_cnt++;
			CCSprite* t_line = CCSprite::createWithTexture(bottom_block_line_manager->getTexture());
			t_line->setAnchorPoint(ccp(0,1.f));
			t_line->setPosition(ccp(i*line_size.width,bottom_y-1));
			//	t_line->setTag(bottom_cnt*8);
			bottom_block_line_manager->addChild(t_line);
		}
		{
			CCSprite* t_line = CCSprite::createWithTexture(bottom_block_line_manager->getTexture(), CCRectMake(0, 0, 320-line_cnt*line_size.width, line_size.height));
			t_line->setAnchorPoint(ccp(0,1.f));
			t_line->setPosition(ccp(line_cnt*line_size.width, bottom_y-1));
			bottom_block_line_manager->addChild(t_line);
		}
	}
	
	while(bottom_y > 0)
	{
		bottom_cnt++;
		
		if(bottom_y-block_size.height <= 0)
		{
			float sub_value = -(bottom_y-block_size.height);
			for(int i=0;i<10;i++)
			{
				CCSprite* t_block = CCSprite::createWithTexture(bottom_block_manager->getTexture(), CCRectMake(0, 0, block_size.width, block_size.height-sub_value));
				t_block->setColor(ccBLACK);
				t_block->setOpacity(180);
				t_block->setAnchorPoint(ccp(0,1.f));
				t_block->setPosition(ccp(i*block_size.width, bottom_y-2));
				t_block->setTag(bottom_cnt*10+i);
				bottom_block_manager->addChild(t_block);
			}
		}
		else
		{
			for(int i=0;i<10;i++)
			{
				CCSprite* t_block = CCSprite::createWithTexture(bottom_block_manager->getTexture());
				t_block->setColor(ccBLACK);
				t_block->setOpacity(180);
				t_block->setAnchorPoint(ccp(0,1.f));
				t_block->setPosition(ccp(i*block_size.width, bottom_y-2));
				t_block->setTag(bottom_cnt*10+i);
				bottom_block_manager->addChild(t_block);
			}
		}
		
		bottom_y -= block_size.height;
	}
	bottom_block_manager->setTag(bottom_cnt);
	
	if((myGD->limited_step_bottom-1)*pixelSize+2 - 31 < 0)
	{
		float sub_value = -((myGD->limited_step_bottom-1)*pixelSize+2 - 31);
		bottom_block_lock = CCSprite::create("temp_block_lock.png", CCRectMake(0, 0, 74, 31-sub_value));
		bottom_block_lock->setAnchorPoint(ccp(0.5,1.f));
		bottom_block_lock->setPosition(ccp(160,(myGD->limited_step_bottom-1)*pixelSize+2));
		addChild(bottom_block_lock, blockZorder);
		
		CCClippingNode* t_clipping = CCClippingNode::create(CCSprite::create("temp_block_lock.png", CCRectMake(0, 0, bottom_block_lock->getContentSize().width, bottom_block_lock->getContentSize().height-sub_value)));
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;

		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;

		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		
		t_clipping->getStencil()->setAnchorPoint(ccp(0.5,1.f));
		t_clipping->setPosition(ccp(bottom_block_lock->getContentSize().width/2.f,bottom_block_lock->getContentSize().height-sub_value));
		bottom_block_lock->addChild(t_clipping);
		
		t_clipping->setAlphaThreshold(0.1f);
		
		KSLabelTTF* lock_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_locked), mySGD->getFont().c_str(), 15);
		lock_label->setAnchorPoint(ccp(0.5f,0.5f));
		lock_label->disableOuterStroke();
		lock_label->setPosition(ccp(0,-31/2.f));
		t_clipping->addChild(lock_label);
	}
	else
	{
		bottom_block_lock = CCSprite::create("temp_block_lock.png");
		bottom_block_lock->setAnchorPoint(ccp(0.5,1.f));
		bottom_block_lock->setPosition(ccp(160,(myGD->limited_step_bottom-1)*pixelSize+2));
		addChild(bottom_block_lock, blockZorder);
		
		KSLabelTTF* lock_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_locked), mySGD->getFont().c_str(), 15);
		lock_label->disableOuterStroke();
		lock_label->setPosition(ccp(bottom_block_lock->getContentSize().width/2.f,bottom_block_lock->getContentSize().height/2.f));
		bottom_block_lock->addChild(lock_label);
	}

	random_device rd;
	default_random_engine e1(rd());
	uniform_int_distribution<int> uniform_dist1(mapWidthInnerBegin+10, mapWidthInnerEnd-10);
	uniform_int_distribution<int> uniform_dist2(myGD->limited_step_bottom+5, myGD->limited_step_top-5);

	int random_x = uniform_dist1(e1);
	int random_y = myGD->limited_step_bottom + roundf((myGD->limited_step_top-myGD->limited_step_bottom)/2.f);

	start_map_lucky_item = StartMapLuckyItem::create(IntPoint(random_x, random_y));
	addChild(start_map_lucky_item, blockZorder);
}

void MapScanner::startRemoveBlock()
{
	AudioEngine::sharedInstance()->playEffect("ment_unlockmap.mp3", false, true);
	
	is_removed_top_block = false;
	is_removed_bottom_block = false;
	remove_block_cnt = 0;
	
	top_block_lock->removeFromParent();
	bottom_block_lock->removeFromParent();
//	top_block_line_manager->removeChildByTag(remove_block_cnt*8);
	top_block_line_manager->removeFromParent();
//	bottom_block_line_manager->removeChildByTag(remove_block_cnt*8);
	bottom_block_line_manager->removeFromParent();
	
	schedule(schedule_selector(MapScanner::removingBlock), 0.15f);
}

void MapScanner::removingBlock()
{
	remove_block_cnt++;

	if(!is_removed_top_block)
	{
		if(top_block_manager->getTag() < remove_block_cnt)
			is_removed_top_block = true;
		else
		{
			for(int i=0;i<10;i++)
				top_block_manager->removeChildByTag(remove_block_cnt*10+i);
		}
	}
	if(!is_removed_bottom_block)
	{
		if(bottom_block_manager->getTag() < remove_block_cnt)
			is_removed_bottom_block = true;
		else
		{
			for(int i=0;i<10;i++)
				bottom_block_manager->removeChildByTag(remove_block_cnt*10+i);
		}
	}


	if(is_removed_top_block && is_removed_bottom_block)
	{
		unschedule(schedule_selector(MapScanner::removingBlock));
		top_block_manager->removeFromParent();
		bottom_block_manager->removeFromParent();
	}
}

void MapScanner::showEmptyPoint( CCPoint t_point )
{
	CCSprite* show_empty_point = CCSprite::create("show_empty_point.png");
	show_empty_point->setAnchorPoint(ccp(0.5f,0.f));
	show_empty_point->setPosition(t_point);
	show_empty_point->setScale(1.f/myGD->game_scale);
	addChild(show_empty_point, boarderZorder, 99999);

	CCMoveTo* t_move1 = CCMoveTo::create(0.3f, ccpAdd(t_point, ccp(0,20)));
	CCMoveTo* t_move2 = CCMoveTo::create(0.3f, t_point);
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move1, t_move2);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);

	show_empty_point->runAction(t_repeat);
}

void MapScanner::removeEmptyPoint()
{
	CCNode* t_empty_point = getChildByTag(99999);
	if(t_empty_point)
		t_empty_point->removeFromParent();
}

void MapScanner::myInit()
{
	
	
	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	screen_height = roundf(480*screen_size.height/screen_size.width/2.f);

	silType = mySD->getSilType();
	

	random_rect_img = NULL;
	invisibleImg = NULL;
	visibleImg = NULL;
	blockParent = NULL;

	myGD->V_V["MS_scanMap"] = std::bind(&MapScanner::scanMap, this);
	myGD->V_B["MS_resetRects"] = std::bind(&MapScanner::resetRects, this, _1);
	myGD->V_CCP["MS_showEmptyPoint"] = std::bind(&MapScanner::showEmptyPoint, this, _1);
	myGD->V_V["MS_setTopBottomBlock"] = std::bind(&MapScanner::setTopBottomBlock, this);
	myGD->V_V["MS_startRemoveBlock"] = std::bind(&MapScanner::startRemoveBlock, this);
	myGD->B_V["MS_isCheckBossLocked"] = std::bind(&MapScanner::isCheckBossLocked, this);
	myGD->V_V["MS_removeEmptyPoint"] = std::bind(&MapScanner::removeEmptyPoint, this);
	myGD->V_V["MS_ingNewlineToRealNewline"] = std::bind(&MapScanner::ingNewlineToRealNewline, this);

	
	setMapImg();
	
}

InvisibleSprite* InvisibleSprite::create( const char* filename, bool isPattern )
{
	InvisibleSprite* t_iv = new InvisibleSprite();
	t_iv->myInit(filename, isPattern);
	t_iv->autorelease();
	return t_iv;
}

void InvisibleSprite::myInit( const char* filename, bool isPattern )
{
//	CCSpriteBatchNode* pattern_node = CCSpriteBatchNode::create("ingame_side_pattern.png");
//	pattern_node->setPosition(ccp(0,0));
//	addChild(pattern_node);
//	
//	CCTexture2D* pattern_texture = pattern_node->getTexture();
//	CCSize pattern_size = pattern_texture->getContentSize();
//	
//	for(int j=0;j<14;j++)
//	{
//		for(int i=0;i<6;i++)
//		{
//			CCSprite* t_pattern = CCSprite::createWithTexture(pattern_texture);
//			t_pattern->setAnchorPoint(ccp(0,0));
//			t_pattern->setPosition(ccp(i*pattern_size.width, j*pattern_size.height));
//			pattern_node->addChild(t_pattern);
//		}
//		
//		CCSprite* t_pattern = CCSprite::createWithTexture(pattern_texture, CCRectMake(0, 0, 320-6*pattern_size.width, pattern_size.height));
//		t_pattern->setAnchorPoint(ccp(0,0));
//		t_pattern->setPosition(ccp(6*pattern_size.width, j*pattern_size.height));
//		pattern_node->addChild(t_pattern);
//	}
//	
//	for(int i=0;i<6;i++)
//	{
//		CCSprite* t_pattern = CCSprite::createWithTexture(pattern_texture, CCRectMake(0, 0, pattern_size.width, 430-14*pattern_size.height));
//		t_pattern->setAnchorPoint(ccp(0,0));
//		t_pattern->setPosition(ccp(i*pattern_size.width, 14*pattern_size.height));
//		pattern_node->addChild(t_pattern);
//	}
//	
//	CCSprite* t_pattern = CCSprite::createWithTexture(pattern_texture, CCRectMake(0, 0, 320-6*pattern_size.width, 430-14*pattern_size.height));
//	t_pattern->setAnchorPoint(ccp(0,0));
//	t_pattern->setPosition(ccp(6*pattern_size.width, 14*pattern_size.height));
//	pattern_node->addChild(t_pattern);
	
	EffectSprite* t_spr = EffectSprite::createWithTexture(mySIL->addImage(filename));
	t_spr->setPosition(ccp(160,215));
	int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	t_spr->setColorSilhouette(NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d));

	// ######################## hs code bbu woo~ ##############################
	t_spr->setTag(8706);
	// ######################## hs code bbu woo~ ##############################
	
	addChild(t_spr);
	
//	myTR->addAliveNode(t_spr, [=]()
//					   {
//						   t_spr->setNonEffect();
//						   t_spr->setColorSilhouette(NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d));
//					   });
	
//	CCRenderTexture* t_render = CCRenderTexture::create(320, 430);
//	t_render->setPosition(ccp(160,215));
//	t_render->beginWithClear(0, 0, 0, 0);
//	t_spr->visit();
//	t_render->end();
//	addChild(t_render);
}

VisibleSprite* VisibleSprite::create( const char* filename, bool isPattern, CCArray* t_drawRects, string sil_filename )
{
	VisibleSprite* t_v = new VisibleSprite();
	t_v->myInit(filename, isPattern, t_drawRects, sil_filename);
	t_v->autorelease();
	return t_v;
}

void VisibleSprite::setMoveGamePosition( CCPoint t_p )
{
	jack_position = t_p;
}

CCPoint VisibleSprite::getMoveGamePosition()
{
	return jack_position;
}

void VisibleSprite::setSceneNode( CCObject* t_scene_node )
{
	scene_node = (CCNode*)t_scene_node;
	is_set_scene_node = true;
}

void VisibleSprite::visit()
{
	unsigned int loopCnt = drawRects->count();
	float game_node_scale = myGD->Fcommunication("Main_getGameNodeScale");
	
	glEnable(GL_SCISSOR_TEST);

	for(int i=0;i<loopCnt;i++)
	{
		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
		
		float wScale = viewport[2] / design_resolution_size.width;
		float hScale = viewport[3] / design_resolution_size.height;
		
		float x, y, w, h;
		
		if(is_set_scene_node)
		{
			x = (t_rect->origin.x*game_node_scale+jack_position.x+scene_node->getPositionX())*wScale + viewport[0]-1-1;
			y = (t_rect->origin.y*game_node_scale+jack_position.y+scene_node->getPositionY())*hScale + viewport[1]-1-1;
			w = (t_rect->size.width*game_node_scale)*wScale+2+2;
			h = (t_rect->size.height*game_node_scale)*hScale+2+2;
		}
		else
		{
			x = (t_rect->origin.x*game_node_scale+jack_position.x)*wScale + viewport[0]-1-1;
			y = (t_rect->origin.y*game_node_scale+jack_position.y)*hScale + viewport[1]-1-1;
			w = (t_rect->size.width*game_node_scale)*wScale+2+2;
			h = (t_rect->size.height*game_node_scale)*hScale+2+2;
		}
		
		if(y > screen_size.height || y+h < 0.f)
			continue;
		else
		{
			glScissor(x,y,w,h);
			light_img->draw();
		}
		
	}
	
	for(int i=0;i<loopCnt;i++)
	{
		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);

		float wScale = viewport[2] / design_resolution_size.width;
		float hScale = viewport[3] / design_resolution_size.height;

		float x, y, w, h;

		if(is_set_scene_node)
		{
			x = (t_rect->origin.x*game_node_scale+jack_position.x+scene_node->getPositionX())*wScale + viewport[0]-1;
			y = (t_rect->origin.y*game_node_scale+jack_position.y+scene_node->getPositionY())*hScale + viewport[1]-1;
			w = (t_rect->size.width*game_node_scale)*wScale+2;
			h = (t_rect->size.height*game_node_scale)*hScale+2;
		}
		else
		{
			x = (t_rect->origin.x*game_node_scale+jack_position.x)*wScale + viewport[0]-1;
			y = (t_rect->origin.y*game_node_scale+jack_position.y)*hScale + viewport[1]-1;
			w = (t_rect->size.width*game_node_scale)*wScale+2;
			h = (t_rect->size.height*game_node_scale)*hScale+2;
		}

		if(y > screen_size.height || y+h < 0.f)
			continue;
		else
		{
			glScissor(x,y,w,h);
			draw();
			if(mySGD->is_safety_mode)
				safety_img->draw();
		}

	}

	glDisable(GL_SCISSOR_TEST);
}

void VisibleSprite::visitForThumb()
{
	unsigned int loopCnt = drawRects->count();
	
	glEnable(GL_SCISSOR_TEST);
	
	for(int i=0;i<loopCnt;i++)
	{
		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
		
		float wScale = viewport[2] / (design_resolution_size.width + (viewport[2]-960.f)/2.f); // 1024, 768 / 480, 360 -> + 32, 24
		float hScale = viewport[3] / (design_resolution_size.height + (viewport[2]-960.f)/2.f*design_resolution_size.height/design_resolution_size.width); // 1136, 641 / 480, 271 -> + 89, 50
		
		float x, y, w, h;
		
		if(is_set_scene_node)
		{
			x = t_rect->origin.x*wScale + viewport[0]-1;
			y = t_rect->origin.y*hScale + viewport[1]-1;
			w = t_rect->size.width*wScale + 2;
			h = t_rect->size.height*hScale + 2;
		}
		else
		{
			x = t_rect->origin.x*wScale + viewport[0]-1;
			y = t_rect->origin.y*hScale + viewport[1]-1;
			w = t_rect->size.width*wScale + 2;
			h = t_rect->size.height*hScale + 2;
		}
		
		glScissor(x,y,w,h);
		draw();
		if(mySGD->is_safety_mode)
			safety_img->draw();
	}
	
	glDisable(GL_SCISSOR_TEST);
}

void VisibleSprite::replayVisitForThumb(int temp_time)
{
	int play_index = mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_playIndex)].asInt();
	if(play_index >= mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size())
		return;
	
	if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)][play_index].asInt() > temp_time)
		return;
		
	
	
	int draw_array[162][217] = {mapEmpty,};
	Json::Value map_data = mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapData)][play_index];
	for(int y=0;y<map_data.size();y++) // y
	{
		int record = map_data[y].asInt();
		for(int x = 31;x>=0;--x)
		{
			bool is_draw = record & 0x1;
			record = record >> 1;
			
			if(is_draw)
				for(int i=mapWidthInnerBegin+x*5;i<mapWidthInnerBegin+(x+1)*5;++i)
					for(int j=mapHeightInnerBegin+y*5;j<mapHeightInnerBegin+(y+1)*5;j++)
						draw_array[i][j] = mapOldline;
		}
	}
	
	
	CCArray* rects = CCArray::createWithCapacity(256);
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(draw_array[i][j] == mapOldline)
			{
				IntPoint origin = IntPoint(i, j);
				IntSize size = IntSize(0, 0);
				
				bool isUpper = true;
				bool isRighter = true;
				queue<IntMoveState> loopArray;
				loopArray.push(IntMoveState(i, j, directionRightUp));
				
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
						
						if(draw_array[t_ms.origin.x][t_ms.origin.y] == mapOldline)			draw_array[t_ms.origin.x][t_ms.origin.y] = mapScaningCheckLine;
						
						if(t_ms.direction == directionUp)
						{
							if(isUpper)
							{
								IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
								if(n_msUp.origin.isInnerMap() && draw_array[n_msUp.origin.x][n_msUp.origin.y] == mapOldline)
									nextLoopArray.push(n_msUp);
								else		upable = false;
							}
						}
						else if(t_ms.direction == directionRight)
						{
							if(isRighter)
							{
								IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
								if(n_msRight.origin.isInnerMap() && draw_array[n_msRight.origin.x][n_msRight.origin.y] == mapOldline)
									nextLoopArray.push(n_msRight);
								else		rightable = false;
							}
						}
						else if(t_ms.direction == directionRightUp)
						{
							if(isUpper)
							{
								IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
								if(n_msUp.origin.isInnerMap() && draw_array[n_msUp.origin.x][n_msUp.origin.y] == mapOldline)
									nextLoopArray.push(n_msUp);
								else		upable = false;
							}
							
							if(isRighter)
							{
								IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
								if(n_msRight.origin.isInnerMap() && draw_array[n_msRight.origin.x][n_msRight.origin.y] == mapOldline)
									nextLoopArray.push(n_msRight);
								else		rightable = false;
							}
							
							if(upable && rightable)
							{
								IntMoveState n_msRightUp = IntMoveState(t_ms.origin.x+1, t_ms.origin.y+1, directionRightUp);
								if(n_msRightUp.origin.isInnerMap() && draw_array[n_msRightUp.origin.x][n_msRightUp.origin.y] == mapOldline)
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
				
				IntRect* r_rect = new IntRect((origin.x-1)*pixelSize, (origin.y-1)*pixelSize, size.width*pixelSize, size.height*pixelSize);
				r_rect->autorelease();
				
				rects->addObject(r_rect);
			}
		}
	}
	
	unsigned int loopCnt = rects->count();
	
	glEnable(GL_SCISSOR_TEST);
	
	for(int i=0;i<loopCnt;i++)
	{
		IntRect* t_rect = (IntRect*)rects->objectAtIndex(i);
		
		float wScale = viewport[2] / (design_resolution_size.width + (viewport[2]-960.f)/2.f); // 1024, 768 / 480, 360 -> + 32, 24
		float hScale = viewport[3] / (design_resolution_size.height + (viewport[2]-960.f)/2.f*design_resolution_size.height/design_resolution_size.width); // 1136, 641 / 480, 271 -> + 89, 50
		
		float x, y, w, h;
		
		if(is_set_scene_node)
		{
			x = t_rect->origin.x*wScale + viewport[0]-1;
			y = t_rect->origin.y*hScale + viewport[1]-1;
			w = t_rect->size.width*wScale + 2;
			h = t_rect->size.height*hScale + 2;
		}
		else
		{
			x = t_rect->origin.x*wScale + viewport[0]-1;
			y = t_rect->origin.y*hScale + viewport[1]-1;
			w = t_rect->size.width*wScale + 2;
			h = t_rect->size.height*hScale + 2;
		}
		
		glScissor(x,y,w,h);
		draw();
		if(mySGD->is_safety_mode)
			safety_img->draw();
	}
	
	glDisable(GL_SCISSOR_TEST);
	
	mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_playIndex)] = play_index+1;
}

void VisibleSprite::setLight()
{
	stopAllActions();
	setBrighten(1.8f);
//	setColor(ccWHITE);
	CCDelayTime* t_delay = CCDelayTime::create(0.1f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(VisibleSprite::setDark));
	CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
	runAction(t_seq);
}

void VisibleSprite::setDark()
{
	setBrighten(1.f);
//	setColor(ccGRAY);
}

void VisibleSprite::myInit( const char* filename, bool isPattern, CCArray* t_drawRects, string sil_filename )
{
	initWithTexture(mySIL->addImage(filename));
	setBrighten(1.f);
//	setColor(ccGRAY);
	setPosition(ccp(160,215));

	
	is_set_scene_node = false;

	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	design_resolution_size = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
	glGetIntegerv(GL_VIEWPORT, viewport);

	
	drawRects = t_drawRects;
	
	safety_img = EffectSprite::createWithTexture(mySIL->addImage(sil_filename.c_str()));
	safety_img->setSilhouetteConvert(0);
	safety_img->setPosition(ccp(getContentSize().width/2.f, getContentSize().height/2.f));
	addChild(safety_img);
	
	
	light_img = CCSprite::create("ingame_whiteback.png");
	light_img->setPosition(ccp(getContentSize().width/2.f, getContentSize().height/2.f));
	addChild(light_img, -1);
	

	CCTintTo* t_tint1 = CCTintTo::create(1.f, 255, 100, 100);
	CCTintTo* t_tint2 = CCTintTo::create(1.f, 100, 255, 100);
	CCTintTo* t_tint3 = CCTintTo::create(1.f, 100, 100, 255);
	CCTintTo* t_tint4 = CCTintTo::create(1.f, 255, 100, 255);
	CCTintTo* t_tint5 = CCTintTo::create(1.f, 255, 255, 100);
	CCTintTo* t_tint6 = CCTintTo::create(1.f, 100, 255, 255);
	CCTintTo* t_tint7 = CCTintTo::create(1.f, 255, 255, 255);
	CCSequence* t_seq = CCSequence::create(t_tint1, t_tint2, t_tint3, t_tint4, t_tint5, t_tint6, t_tint7, NULL);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
	light_img->runAction(t_repeat);
}

CCTexture2D* VisibleSprite::createSafetyImage(string fullpath){
	
	
	CCImage* img = new CCImage();
	img->initWithImageFileThreadSafe(fullpath.c_str());
	
//	CCLOG("fuckfuckfuck android %d,%d,%d,%d",newImg->getDataLen(), newImg->getWidth(), newImg->getHeight(), newImg->getBitsPerComponent());
	
//	CCImage* img = new CCImage();
//	
//	img->initWithImageData(newImg->getData(), newImg->getDataLen())
//	
	
	
//	newImg->release();
	
	
	int imgByte = 3;
	
	if(img->hasAlpha())imgByte = 4;
	
	unsigned char* oData = img->getData();
	
	int oy = img->getHeight();
	int ox = img->getWidth();
	
	unsigned char* oDataPos;
	
	int i;
	
	
	for(int y=0;y<oy;y++){
		for(int x=0;x<ox;x++){
			i = (y*ox+x)*imgByte;
			
			oDataPos = &oData[i+3];
			
			if(oData[i+2]>10 || oData[i+1]>10 || oData[i]>10){
				if(imgByte==4)oData[i+3] = 255;
				oData[i+2] = 255;
				oData[i+1] = 255;
				oData[i] = 255;
			}else{
				if(imgByte==4)oData[i+3] = 0;
				oData[i+2] = 0;
				oData[i+1] = 0;
				oData[i] = 0;
			}
		}
	}
	
	
	CCTexture2D* texture = new CCTexture2D;
	texture->initWithImage(img);
	texture->autorelease();
	
	img->release();
	
	
	return texture;
	
}

VisibleParent* VisibleParent::create( const char* filename, bool isPattern, string sil_filename )
{
	VisibleParent* t_vp = new VisibleParent();
	t_vp->myInit(filename, isPattern, sil_filename);
	t_vp->autorelease();
	return t_vp;
}

void VisibleParent::setDrawRects( CCArray* t_rects )
{
	drawRects->removeAllObjects();
	drawRects->addObjectsFromArray(t_rects);
}

CCArray* VisibleParent::getDrawRects()
{
	return drawRects;
}

void VisibleParent::divideRect( IntPoint crashPoint )
{
	if(mySD->silData[crashPoint.x][crashPoint.y] == true)
		myGD->communication("UI_decreasePercentage");

	crashPoint.x = (crashPoint.x-1)*pixelSize;
	crashPoint.y = (crashPoint.y-1)*pixelSize;

	vector<IntRect*> removeArray;
	int loopCnt = drawRects->count();
	for(int i=0;i<loopCnt;i++)
	{
		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
		if(crashPoint.x >= t_rect->origin.x && crashPoint.x < t_rect->origin.x + t_rect->size.width && crashPoint.y >= t_rect->origin.y && crashPoint.y < t_rect->origin.y + t_rect->size.height)
		{
			// divide rect
			IntSize t_size;

			// left down
			t_size.width = crashPoint.x - t_rect->origin.x;
			t_size.height = crashPoint.y - t_rect->origin.y + pixelSize;
			if(t_size.width >= pixelSize) // left down create
			{
				IntRect* n_rect = new IntRect(t_rect->origin.x, t_rect->origin.y, t_size.width, t_size.height);
				n_rect->autorelease();
				drawRects->addObject(n_rect);
			}

			// down right
			t_size.width = t_rect->origin.x + t_rect->size.width - crashPoint.x;
			t_size.height = crashPoint.y - t_rect->origin.y;
			if(t_size.height >= pixelSize) // down right create
			{
				IntRect* n_rect = new IntRect(crashPoint.x, t_rect->origin.y, t_size.width, t_size.height);
				n_rect->autorelease();
				drawRects->addObject(n_rect);
			}

			// right up
			t_size.width = t_rect->origin.x + t_rect->size.width - crashPoint.x - pixelSize;
			t_size.height = t_rect->origin.y + t_rect->size.height - crashPoint.y;
			if(t_size.width >= pixelSize)
			{
				IntRect* n_rect = new IntRect(crashPoint.x+pixelSize, crashPoint.y, t_size.width, t_size.height);
				n_rect->autorelease();
				drawRects->addObject(n_rect);
			}

			// up left
			t_size.width = crashPoint.x - t_rect->origin.x + pixelSize;
			t_size.height = t_rect->origin.y + t_rect->size.height - crashPoint.y - pixelSize;
			if(t_size.height >= pixelSize)
			{
				IntRect* n_rect = new IntRect(t_rect->origin.x, crashPoint.y + pixelSize, t_size.width, t_size.height);
				n_rect->autorelease();
				drawRects->addObject(n_rect);
			}
			removeArray.push_back(t_rect);
		}
	}

	while(!removeArray.empty())
	{
		IntRect* t_rect = removeArray.back();
		removeArray.pop_back();
		drawRects->removeObject(t_rect);
	}
}

void VisibleParent::divideRects(IntRect crashRect)
{
	int decrease_count = 0;
	for(int i=crashRect.origin.x;i<crashRect.origin.x + crashRect.size.width;i++)
	{
		for(int j=crashRect.origin.y;j<crashRect.origin.y + crashRect.size.height;j++)
		{
			if(IntPoint(i,j).isInnerMap() && mySD->silData[i][j] && (myGD->mapState[i][j] == mapOldget || myGD->mapState[i][j] == mapOldline))
				decrease_count++;
		}
	}
	
	myGD->communication("UI_decreasePercentages", decrease_count);
	
	crashRect.origin.x = (crashRect.origin.x-1.f)*pixelSize;
	crashRect.origin.y = (crashRect.origin.y-1.f)*pixelSize;
	crashRect.size.width = crashRect.size.width*pixelSize;
	crashRect.size.height = crashRect.size.height*pixelSize;
	
	CCRect crash_rect = CCRectMake(crashRect.origin.x, crashRect.origin.y, crashRect.size.width, crashRect.size.height);
	
	vector<IntRect*> removeArray;
	int loopCnt = drawRects->count();
	
	CCArray* add_rects = CCArray::create();
	
	for(int i=0;i<loopCnt;i++)
	{
		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
		CCRect f_rect = CCRectMake(t_rect->origin.x, t_rect->origin.y, t_rect->size.width, t_rect->size.height);
		if(crash_rect.intersectsRect(f_rect))
		{
			// divide rect
			IntSize t_size;
			
			// left down
			t_size.width = crashRect.origin.x - t_rect->origin.x;
			t_size.height = (crashRect.origin.y + crashRect.size.height) - t_rect->origin.y;
			if(t_size.width >= pixelSize && t_size.height >= pixelSize) // left down create
			{
				if(t_size.width > t_rect->size.width)
					t_size.width = t_rect->size.width;
				if(t_size.height > t_rect->size.height)
					t_size.height = t_rect->size.height;
				
				IntRect* n_rect = new IntRect(t_rect->origin.x, t_rect->origin.y, t_size.width, t_size.height);
				n_rect->autorelease();
				add_rects->addObject(n_rect);
			}
			
			// down right
			t_size.width = (t_rect->origin.x + t_rect->size.width) - crashRect.origin.x;
			t_size.height = crashRect.origin.y - t_rect->origin.y;
			if(t_size.height >= pixelSize && t_size.width >= pixelSize) // down right create
			{
				if(t_size.width > t_rect->size.width)
					t_size.width = t_rect->size.width;
				if(t_size.height > t_rect->size.height)
					t_size.height = t_rect->size.height;
				
				int after_x;
				if(crashRect.origin.x < t_rect->origin.x)
					after_x = t_rect->origin.x;
				else
					after_x = crashRect.origin.x;
				
				IntRect* n_rect = new IntRect(after_x, t_rect->origin.y, t_size.width, t_size.height);
				n_rect->autorelease();
				add_rects->addObject(n_rect);
			}
			
			// right up
			t_size.width = (t_rect->origin.x + t_rect->size.width) - (crashRect.origin.x + crashRect.size.width);
			t_size.height = (t_rect->origin.y + t_rect->size.height) - crashRect.origin.y;
			if(t_size.width >= pixelSize && t_size.height >= pixelSize)
			{
				if(t_size.width > t_rect->size.width)
					t_size.width = t_rect->size.width;
				if(t_size.height > t_rect->size.height)
					t_size.height = t_rect->size.height;
				
				int after_y;
				if(crashRect.origin.y < t_rect->origin.y)
					after_y = t_rect->origin.y;
				else
					after_y = crashRect.origin.y;
				
				IntRect* n_rect = new IntRect(crashRect.origin.x + crashRect.size.width, after_y, t_size.width, t_size.height);
				n_rect->autorelease();
				add_rects->addObject(n_rect);
			}
			
			// up left
			t_size.width = (crashRect.origin.x + crashRect.size.width) - t_rect->origin.x;
			t_size.height = (t_rect->origin.y + t_rect->size.height) - (crashRect.origin.y + crashRect.size.height);
			if(t_size.height >= pixelSize && t_size.width >= pixelSize)
			{
				if(t_size.width > t_rect->size.width)
					t_size.width = t_rect->size.width;
				if(t_size.height > t_rect->size.height)
					t_size.height = t_rect->size.height;
				
				IntRect* n_rect = new IntRect(t_rect->origin.x, crashRect.origin.y + crashRect.size.height, t_size.width, t_size.height);
				n_rect->autorelease();
				add_rects->addObject(n_rect);
			}
			
			removeArray.push_back(t_rect);
		}
	}
	
	drawRects->addObjectsFromArray(add_rects);
	
	while(!removeArray.empty())
	{
		IntRect* t_rect = removeArray.back();
		removeArray.pop_back();
		drawRects->removeObject(t_rect);
	}
}

void VisibleParent::setMoveGamePosition( CCPoint t_p )
{
	//		if(!myGD->is_setted_jack || myGD->game_step == kGS_unlimited)
	{
//		float game_node_scale = myGD->Fcommunication("Main_getGameNodeScale");
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float y_value = -t_p.y*myGD->game_scale+480.f*frame_size.height/frame_size.width/2.f;// (160-t_p.y)*MY_SCALE-73.f+myDSH->bottom_base-myDSH->ui_jack_center_control;
		if(!myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
		{
			if(y_value > 80)																	y_value = 80;
			else if(y_value < -430*myGD->game_scale+480*frame_size.height/frame_size.width - 60)		y_value = -430*myGD->game_scale+480*frame_size.height/frame_size.width - 60;
		}
		
		float x_value = -t_p.x*myGD->game_scale+480.f/2.f;
		if(!myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
		{
			if(x_value > myGD->boarder_value+80)													x_value = myGD->boarder_value+80;
			else if(x_value < -320*myGD->game_scale-myGD->boarder_value+480.f-80)					x_value = -320*myGD->game_scale-myGD->boarder_value+480.f-80;
		}
		

//		if(myGD->gamescreen_type == kGT_full)				myVS->setMoveGamePosition(ccp(myGD->boarder_value,y_value));
//		else if(myGD->gamescreen_type == kGT_leftUI)		myVS->setMoveGamePosition(ccp(50+myGD->boarder_value,y_value));
//		else if(myGD->gamescreen_type == kGT_rightUI)		myVS->setMoveGamePosition(ccp(myGD->boarder_value,y_value));
		myVS->setMoveGamePosition(ccp(x_value, y_value));
	}
}

void VisibleParent::setLimittedMapPosition()
{
	limitted_map_position = myVS->getMoveGamePosition();
}

void VisibleParent::changingGameStep( int t_step )
{
	IntPoint jack_point = myGD->getJackPoint();
	CCPoint jack_position = jack_point.convertToCCP();

	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float y_value = -jack_position.y*myGD->game_scale+480.f*frame_size.height/frame_size.width/2.f;// (160-t_p.y)*MY_SCALE-73.f+myDSH->bottom_base-myDSH->ui_jack_center_control;
	if(!myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
	{
		if(y_value > 80)																	y_value = 80;
		else if(y_value < -430*myGD->game_scale+480*frame_size.height/frame_size.width - 60)		y_value = -430*myGD->game_scale+480*frame_size.height/frame_size.width - 60;
	}
	
	float x_value = -jack_position.x*myGD->game_scale+480.f/2.f;
	if(!myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
	{
		if(x_value > myGD->boarder_value)														x_value = myGD->boarder_value;
		else if(x_value < -320*myGD->game_scale-myGD->boarder_value+480.f)						x_value = -320*myGD->game_scale-myGD->boarder_value+480.f;
	}

//	if(myGD->gamescreen_type == kGT_full)				jack_position = ccp(myGD->boarder_value,y_value);
//	else if(myGD->gamescreen_type == kGT_leftUI)		jack_position = ccp(50+myGD->boarder_value,y_value);
//	else if(myGD->gamescreen_type == kGT_rightUI)		jack_position = ccp(myGD->boarder_value,y_value);
	jack_position = ccp(x_value, y_value);

	CCPoint after_position = ccpAdd(limitted_map_position, ccpMult(ccpSub(jack_position, limitted_map_position), t_step/15.f));
	myVS->setMoveGamePosition(after_position);
}

void VisibleParent::myInit( const char* filename, bool isPattern, string sil_filename )
{
		drawRects = new CCArray(1);
	setPosition(CCPointZero);

	myGD->V_Ip["VS_divideRect"] = std::bind(&VisibleParent::divideRect, this, _1);
	myGD->V_Ir["VS_divideRects"] = std::bind(&VisibleParent::divideRects, this, _1);
	myGD->V_CCP["VS_setMoveGamePosition"] = std::bind(&VisibleParent::setMoveGamePosition, this, _1);
	myGD->V_V["VS_setLimittedMapPosition"] = std::bind(&VisibleParent::setLimittedMapPosition, this);
	myGD->V_I["VS_changingGameStep"] = std::bind(&VisibleParent::changingGameStep, this, _1);

	myVS = VisibleSprite::create(filename, isPattern, drawRects, sil_filename);
	myVS->setPosition(CCPointZero);
	addChild(myVS);

	myGD->V_CCO["VS_setSceneNode"] = std::bind(&VisibleSprite::setSceneNode, myVS, _1);
	myGD->V_V["VS_setLight"] = std::bind(&VisibleSprite::setLight, myVS);
	myGD->CCP_V["VS_getMoveGamePosition"] = std::bind(&VisibleSprite::getMoveGamePosition, myVS);
	
}
