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
#include <memory>
#include "KSGeometry.h"
template <typename Node>
class CircularQueue
{
private:
	Node* node; // 노드 배열
	int capacity; // 큐의 용량
	int front; // 전단의 인덱스
	int rear; // 후단의 인덱스
public:
	CircularQueue(int capacity)
	{
		this->capacity = capacity + 1; // 노드 배열의 크기는 실제 용량에서 1을 더한 크기 (더미 공간 때문)
		node = new Node[this->capacity]; // Node 구조체 capacity + 1개를 메모리 공간에 할당한다
		front = 0; rear = 0; // 전단과 후단의 초기화
	}
	
	~CircularQueue()
	{
		delete []node; // 노드 배열 소멸
	}
	void clear()
	{
		front = rear = 0;
	}
	void enqueue(const Node& data)
	{
		int pos; // 데이터가 들어갈 인덱스
		pos = rear;
		rear = (rear + 1) % (capacity);
		node[pos] = data; // pos 번째의 노드의 데이터에 data를 대입한다
	}
	Node dequeue() {
		int pos = front; // pos에 전단의 인덱스 대입
		front = (front + 1) % capacity;
		return node[pos]; // 제외되는 데이터를 반환한다
	}
	int getSize() {
		if (front <= rear) // 전단의 인덱스가 후단의 인덱스와 같거나 그보다 작다면
			return rear - front; // 후단의 인덱스에서 전단의 인덱스를 뺀값을 반환한다
		else // 전단의 인덱스가 후단의 인덱스보다 크다면
			return capacity - front + rear; // 용량에서 전단의 인덱스를 뺀 뒤에 후단의 인덱스를 더한 값을 반환한다
	}
	bool isEmpty() {
		return front == rear; // 전단의 인덱스와 후단의 인덱스가 같을 경우 true, 아니면 false
	}
	bool isFull() {
		return front == (rear + 1) % capacity;
	}
	int getRear() { return rear; }
	int getFront() { return front; }
	vector<Node> getTotalData()
	{
		int tempFront = front;
		vector<Node> T;
		while(tempFront != rear)
		{
			T.push_back(node[tempFront]);
			tempFront = (tempFront + 1) % capacity;
		}
		return T;
	}

	//	void show()
	//	{
	//		int tempFront = front;
	//		while(tempFront != rear)
	//		{
	//			cout << node[tempFront].data << "/";
	//
	//			tempFront = (tempFront + 1) % capacity;
	//		}
	//		cout << endl;
	//	}
};

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
	chrono::time_point<chrono::system_clock> start, end;
	chrono::duration<double> elapsed_seconds;
	start = chrono::system_clock::now();

	auto dgPointer = GameData::sharedGameData();
	if(dgPointer->game_step == kGS_limited)
	{
		int mid = (mapWidthInnerEnd - mapWidthInnerBegin) / 2;
		
		for(int i=mid;i>=mapWidthInnerBegin;i--)
		{
			if(dgPointer->mapState[i][dgPointer->limited_step_top] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, dgPointer->limited_step_top));
			if(dgPointer->mapState[i][dgPointer->limited_step_bottom] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, dgPointer->limited_step_bottom));
			
		}
		for(int i=mid + 1;i<mapWidthInnerEnd;i++)
		{
			if(dgPointer->mapState[i][dgPointer->limited_step_top] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, dgPointer->limited_step_top));
			if(dgPointer->mapState[i][dgPointer->limited_step_bottom] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, dgPointer->limited_step_bottom));
			
		}
	}
	else
	{
		int mid = (mapWidthInnerEnd - mapWidthInnerBegin) / 2;
		for(int i=mid + 1;i<mapWidthInnerEnd;i++)
		{
			if(dgPointer->mapState[i][mapHeightInnerBegin] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, mapHeightInnerBegin));
			if(dgPointer->mapState[i][mapHeightInnerEnd-1] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, mapHeightInnerEnd-1));
		}
		for(int i=mid;i>=mapWidthInnerBegin;i--)
		{
			if(dgPointer->mapState[i][mapHeightInnerBegin] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, mapHeightInnerBegin));
			if(dgPointer->mapState[i][mapHeightInnerEnd-1] == mapEmpty)
				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, mapHeightInnerEnd-1));
		}
	}
//	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
//	{
//		if(dgPointer->mapState[i][mapHeightInnerBegin] == mapEmpty)
//			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, mapHeightInnerBegin));
//		if(dgPointer->mapState[i][mapHeightInnerEnd-1] == mapEmpty)
//			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, mapHeightInnerEnd-1));
//		if(dgPointer->game_step == kGS_limited)
//		{
//			if(dgPointer->mapState[i][dgPointer->limited_step_top] == mapEmpty)
//				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, dgPointer->limited_step_top));
//			if(dgPointer->mapState[i][dgPointer->limited_step_bottom] == mapEmpty)
//				bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(i, dgPointer->limited_step_bottom));
//		}
//	}
	
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("process step 1 / time : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
	
	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(dgPointer->mapState[mapWidthInnerBegin][j] == mapEmpty)
			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(mapWidthInnerBegin, j));
		if(dgPointer->mapState[mapWidthInnerEnd-1][j] == mapEmpty)
			bfsCheck(mapEmpty, mapScaningEmptySide, IntPoint(mapWidthInnerEnd-1, j));
	}
	
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("process step 2 / time : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
	
	vector<CCNode*> main_cumber_vector = dgPointer->getMainCumberCCNodeVector();
	int main_cumber_count = main_cumber_vector.size();
	bool is_found = false;
	IntPoint mainCumberPoint = IntPoint();
	for(int i=0;i<main_cumber_count && !is_found;i++)
	{
		CCNode* t_boss = main_cumber_vector[i];
		IntPoint t_boss_point = dgPointer->getMainCumberPoint(t_boss);
		if(!t_boss_point.isNull() && dgPointer->mapState[t_boss_point.x][t_boss_point.y] == mapScaningEmptySide)
		{
			is_found = true;
			mainCumberPoint = t_boss_point;
		}
	}
	
	
//	IntPoint mainCumberPoint = dgPointer->getMainCumberPoint();
	
	// locked main cumber then reverse
	if(!is_found) // != mapScaningEmptySide
	{
		for(int i=0;i<main_cumber_count;i++)
		{
			CCNode* t_boss = main_cumber_vector[i];
			IntPoint t_boss_point = dgPointer->getMainCumberPoint(t_boss);
			if(!t_boss_point.isNull() && dgPointer->mapState[t_boss_point.x][t_boss_point.y] == mapEmpty)
			{
				mainCumberPoint = t_boss_point;
			}
		}
		//CCAssert(!mainCumberPoint.isNull(), "what?");
		if(!mainCumberPoint.isNull())
		{
			if(dgPointer->game_step == kGS_limited)
			{
				for(int j=mapHeightInnerBegin;j < dgPointer->limited_step_bottom;j++)
				{
					for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
					{
						if(dgPointer->mapState[i][j] == mapOutline)
							dgPointer->mapState[i][j] = mapNewget;
					}
				}
				for(int j=dgPointer->limited_step_top+1;j < mapHeightInnerEnd;j++)
				{
					for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
					{
						if(dgPointer->mapState[i][j] == mapOutline)
							dgPointer->mapState[i][j] = mapNewget;
					}
				}
			}
			
			bfsCheck(mapEmpty, mapScaningEmptySide, mainCumberPoint); // main cumber
			
			for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
			{
				if(dgPointer->mapState[i][mapHeightInnerBegin] == mapScaningEmptySide)
					bfsCheck(mapScaningEmptySide, mapNewget, IntPoint(i, mapHeightInnerBegin));
				if(dgPointer->mapState[i][mapHeightInnerEnd-1] == mapScaningEmptySide)
					bfsCheck(mapScaningEmptySide, mapNewget, IntPoint(i, mapHeightInnerEnd-1));
			}
			for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
			{
				if(dgPointer->mapState[mapWidthInnerBegin][j] == mapScaningEmptySide)
					bfsCheck(mapScaningEmptySide, mapNewget, IntPoint(mapWidthInnerBegin, j));
				if(dgPointer->mapState[mapWidthInnerEnd-1][j] == mapScaningEmptySide)
					bfsCheck(mapScaningEmptySide, mapNewget, IntPoint(mapWidthInnerEnd-1, j));
			}
		}
	}
	
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("process step 3 / time : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
	
	// new inside check
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(dgPointer->mapState[i][j] == mapEmpty)
				bfsCheck(mapEmpty, mapNewget, IntPoint(i, j));
		}
	}
	
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	
	/////////////////////////////////// by HS Start
	// 새로 얻은 영역에 대한 정보 처리.
	CCLOG("process step 4 / time : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
	
	mapType mapGainState[162][217];
	memcpy(mapGainState, mapInitState, sizeof(mapType) * 162 * 217);
	// outside recovery and new inside add show
	int newInsideCnt = 0;
	int sil_inside_cnt = 0;
	int empty_inside_cnt = 0;
	auto sd = mySD;
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			auto mapValue = &dgPointer->mapState[i][j];
			switch(*mapValue)
			{
				case mapScaningEmptySide:
					*mapValue = mapEmpty;
					mapGainState[i][j] = mapEmpty;
					break;
				case mapRealNewline:
					*mapValue = mapOldline;
					mapGainState[i][j] = mapOldline;
					newInsideCnt++;
					if(sd->silData[i][j])		sil_inside_cnt++;
					else						empty_inside_cnt++;

					break;
				case mapNewget:
					*mapValue = mapOldget;
					mapGainState[i][j] = mapOldget;
					newInsideCnt++;
					if(sd->silData[i][j])		sil_inside_cnt++;
					else						empty_inside_cnt++;

					break;
			}
		}
	}
	
	std::vector<IntRectSTL> intRects = getGainRects(mapGainState);
	{
		// 여기서 IntRects 를 돌면서 그리면 될 듯 함!
		int rects = intRects.size();
		CCNode* effectSprites = CCNode::create();
		addChild(effectSprites, gainEffectZorder);
		float minx = numeric_limits<float>::max(), miny = numeric_limits<float>::max(),
		maxx = numeric_limits<float>::min(), maxy = numeric_limits<float>::min();
		for(int i=0; i<rects; i++)
		{
			IntRectSTL rect = intRects[i];
//			CCLOG("%d %d %d %d", rect.x, rect.y, rect.x + rect->size.width, rect.y + rect->size.height);
			CCSprite* gainRectSprite = CCSprite::create();
			gainRectSprite->setAnchorPoint(ccp(0.5f, 0.5f));
			
			gainRectSprite->setTextureRect(CCRectMake(rect.x, rect.y, rect.width, rect.height));
			minx = MIN(minx, rect.x);
			miny = MIN(miny, rect.y);
			maxx = MAX(maxx, rect.x + rect.width);
			maxy = MAX(maxy, rect.y + rect.height);
//			auto transRect = rtSetScale(CCRectMake(rect.x, rect.y, rect.width, rect.height), 1.f, ccp(0.5f, 0.5f));
			gainRectSprite->setPosition(ccp(rect.x + rect.width / 2.f, rect.y + rect.height / 2.f));
//			gainRectSprite->setPosition(transRect.origin);
			effectSprites->addChild(gainRectSprite);
		}
		float centerX  = (maxx + minx) / 2.f;
		float centerY = (maxy +	miny) / 2.f;
		for(int i=0; i<effectSprites->getChildrenCount(); i++)
		{
			CCSprite* tt = (CCSprite*)effectSprites->getChildren()->objectAtIndex(i);
			tt->setPosition(tt->getPosition() - ccp(centerX, centerY));
			
		}
		effectSprites->setPosition(ccp(centerX, centerY));
		addChild(KSGradualValue<float>::create(0.f, 1.f , 0.1f, [=](float t){
			float a = 1 + (10.f / (maxx - minx)) * t;
			float b = 1 + (10.f / (maxy - miny)) * t;
			float c = 255 + (0 - 255.f) * t;
			
			effectSprites->setScaleX(a);
			effectSprites->setScaleY(b);
			KS::setOpacity(effectSprites, c);
		}, [=](float t){
			
			float a = 1 + (10.f / (maxx - minx)) * t;
			float b = 1 + (10.f / (maxy - miny)) * t;
			float c = 255 + (0 - 255.f) * t;
			
//			effectSprites->setScaleX(a);
//			effectSprites->setScaleY(b);
			KS::setOpacity(effectSprites, c);
			effectSprites->removeFromParent();
		}));
//		CCLOG("NEW RECT END");
	}
	
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(dgPointer->mapState[i][j] == mapOldline &&
			   dgPointer->mapState[i-1][j] != mapEmpty && dgPointer->mapState[i-1][j+1] != mapEmpty &&
			   dgPointer->mapState[i-1][j-1] != mapEmpty && dgPointer->mapState[i][j+1] != mapEmpty &&
			   dgPointer->mapState[i][j-1] != mapEmpty && dgPointer->mapState[i+1][j+1] != mapEmpty &&
			   dgPointer->mapState[i+1][j] != mapEmpty && dgPointer->mapState[i+1][j-1] != mapEmpty &&
			   
			   dgPointer->mapState[i-1][j] != mapOutline && dgPointer->mapState[i-1][j+1] != mapOutline &&
			   dgPointer->mapState[i-1][j-1] != mapOutline && dgPointer->mapState[i][j+1] != mapOutline &&
			   dgPointer->mapState[i][j-1] != mapOutline && dgPointer->mapState[i+1][j+1] != mapOutline &&
			   dgPointer->mapState[i+1][j] != mapOutline && dgPointer->mapState[i+1][j-1] != mapOutline)
			{
				dgPointer->mapState[i][j] = mapOldget;
//				mapGainState[i][j] = mapOldget;
			}
		}
	}
	
	
	
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("process step 5 / time : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
	
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		if(dgPointer->mapState[i][mapHeightInnerBegin] != mapEmpty && dgPointer->mapState[i][mapHeightInnerBegin] != mapOutline)
			dgPointer->mapState[i][mapHeightInnerBegin] = mapOldline;
		if(dgPointer->mapState[i][mapHeightInnerEnd-1] != mapEmpty && dgPointer->mapState[i][mapHeightInnerEnd-1] != mapOutline)
			dgPointer->mapState[i][mapHeightInnerEnd-1] = mapOldline;
	}
	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(dgPointer->mapState[mapWidthInnerBegin][j] != mapEmpty && dgPointer->mapState[mapWidthInnerBegin][j] != mapOutline)
			dgPointer->mapState[mapWidthInnerBegin][j] = mapOldline;
		if(dgPointer->mapState[mapWidthInnerEnd-1][j] != mapEmpty && dgPointer->mapState[mapWidthInnerEnd-1][j] != mapOutline)
			dgPointer->mapState[mapWidthInnerEnd-1][j] = mapOldline;
	}
	
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("process step 6 / time : %f", elapsed_seconds.count());
	
	if(dgPointer->game_step == kGS_limited)
	{
		int total_cell = 0;
		int getted_cell = 0;
		for(int j=dgPointer->limited_step_bottom;j<=dgPointer->limited_step_top;j++)
		{
			for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
			{
				total_cell++;
				if(dgPointer->mapState[i][j] == mapOldget || dgPointer->mapState[i][j] == mapOldline)
					getted_cell++;
			}
		}
		
		if(1.f*getted_cell/total_cell > 0.3f)
			dgPointer->communication("Main_setUnlimitMap");
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
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	double score_rate = NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_int2_score_d, t_history.characterIndex.getV(), t_history.characterLevel.getV());
	if(score_rate < 1.0)
		score_rate = 1.0;
	score_rate -= 1.0;
	int sub_score = addScore*score_rate;
	
	mySGD->area_score = mySGD->area_score.getV() + addScore + sub_score;
	
	dgPointer->communication("UI_addScore", addScore, sub_score);
	
	resetRects(true);
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("process step 7(resetRect!!!) / time : %f", elapsed_seconds.count());
	start = chrono::system_clock::now(); 
}

std::vector<IntRectSTL> MapScanner::getGainRects(mapType (*gainMap)[217])
{
	vector<pair<IntPointSTL, mapType>> checkingIndex;
	std::vector<IntRectSTL> rects;
	rects.reserve(10);
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(gainMap[i][j] == mapOldget || gainMap[i][j] == mapOldline)
			{
				IntRectSTL t_rect = newRectChecking(gainMap, &checkingIndex, IntMoveStateSTL(i, j, directionRightUp));
				rects.push_back(t_rect);
			}
		}
	}
	
	// newRectChecking 에서 mapScaningCheckLine mapScaningCheckGet 이 대입된 인덱스를 가져 오는게 속도 면에서 나을 듯??
	for(auto& check : checkingIndex)
	{
		auto checkValue = &gainMap[check.first.x][check.first.y];
		switch(check.second)
		{
			case mapScaningCheckLine:
				*checkValue = mapOldline;
				break;
				
			case mapScaningCheckGet:
				*checkValue = mapOldget;
				break;
		}
	}
	return rects;
}
void MapScanner::resetRects(bool is_after_scanmap)
{
	if(!is_after_scanmap)
	{
		myGD->is_need_resetRects = true;
		return;
	}
	myGD->is_changed_map = true;
	
	chrono::time_point<chrono::system_clock> start, end;
	chrono::duration<double> elapsed_seconds;
	start = chrono::system_clock::now();
	
	vector<pair<IntPointSTL, mapType>> checkingIndex;
	auto gdPointer = myGD;
	// view rects reset
//	CCArray* rects = CCArray::createWithCapacity(256);
	std::vector<IntRectSTL> rects;
	rects.reserve(256);
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(gdPointer->mapState[i][j] == mapOldget || gdPointer->mapState[i][j] == mapOldline)
			{
				rects.push_back( newRectChecking(gdPointer->mapState, &checkingIndex, IntMoveStateSTL(i, j, directionRightUp)) );
//				rects->addObject(t_rect);
			}
		}
	}
	
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("reset rects : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
	visibleImg->setDrawRects(rects);
	
	// newRectChecking 에서 mapScaningCheckLine mapScaningCheckGet 이 대입된 인덱스를 가져 오는게 속도 면에서 나을 듯??
	float drawCellCnt = 0;
	for(auto& check : checkingIndex)
	{
		auto checkValue = &gdPointer->mapState[check.first.x][check.first.y];
		switch(check.second)
		{
			case mapScaningCheckLine:
				*checkValue = mapOldline;
				if(mySD->silData[check.first.x][check.first.y])
				{
					drawCellCnt++;
				}
				
				break;
				
			case mapScaningCheckGet:
				*checkValue = mapOldget;
				if(mySD->silData[check.first.x][check.first.y])
				{
					drawCellCnt++;
				}
				
				break;
		}
	}
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("reset rects 2 : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
//	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
//	{
//		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
//		{
//			auto checkValue = &gdPointer->mapState[i][j];
//			switch(*checkValue)
//			{
//				case mapScaningCheckLine:
//					*checkValue = mapOldline;
//					if(mySD->silData[i][j])
//					{
//						drawCellCnt++;
//					}
//					break;
//				case mapScaningCheckGet:
//					*checkValue = mapOldget;
//					if(mySD->silData[i][j])
//					{
//						drawCellCnt++;
//					}
//					break;
//			}
//		}
//	}
	gdPointer->communication("UI_setPercentage", float(drawCellCnt/mySD->must_cnt), is_after_scanmap);
	
	if(mySGD->is_write_replay)
	{
		if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() > 0)
			myGD->communication("UI_checkMapTimeVector");
		else
			myGD->communication("UI_writeMap");
	}
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("reset rects3 : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
}

IntRectSTL MapScanner::newRectChecking(mapType (*mapArray)[217], vector<pair<IntPointSTL, mapType>>* checkingIndex, const IntMoveStateSTL& start)
{
//	auto gdPointer = myGD;
	IntPoint origin = IntPoint(start.origin.x, start.origin.y);
	IntSize size = IntSize(0, 0);
	
	bool isUpper = true;
	bool isRighter = true;
	CircularQueue<IntMoveStateSTL> loopArray(350);
	loopArray.enqueue(start);
	
	CircularQueue<IntMoveStateSTL> nextLoopArray(350);
	
	while(!loopArray.isEmpty())
	{
		if(isUpper)				size.height++;
		if(isRighter)			size.width++;
		
		bool upable = isUpper;
		bool rightable = isRighter;
		
		while(!loopArray.isEmpty())
		{
//			loopCnt++;
			IntMoveStateSTL t_ms = loopArray.dequeue();
//			loopArray.pop();
			
			
			if(t_ms.direction == directionUp && !isUpper)
				continue;
			if(t_ms.direction == directionRight && !isRighter)
				continue;
		
			auto root = &mapArray[t_ms.origin.x][t_ms.origin.y];
			switch(*root)
			{
				case mapOldget:
					*root = mapScaningCheckGet;
					if(checkingIndex)
						checkingIndex->push_back(pair<IntPointSTL, mapType>(t_ms.origin, *root));
					break;
				case mapOldline:
					*root = mapScaningCheckLine;
					if(checkingIndex)
						checkingIndex->push_back(pair<IntPointSTL, mapType>(t_ms.origin, *root));
					break;
			}
			
			switch(t_ms.direction)
			{
				case directionUp:
					if(isUpper)
					{
						IntMoveStateSTL n_msUp = IntMoveStateSTL(t_ms.origin.x, t_ms.origin.y+1, directionUp);
						auto checkValue = mapArray[n_msUp.origin.x][n_msUp.origin.y];
						if((checkValue == mapOldline ||
								checkValue == mapOldget) && n_msUp.origin.isInnerMap())
						{
							nextLoopArray.enqueue(n_msUp);
						}
						else
						{
							upable = false;
						}
					}
					break;
				case directionRight:
					if(isRighter)
					{
						IntMoveStateSTL n_msRight = IntMoveStateSTL(t_ms.origin.x+1, t_ms.origin.y, directionRight);
						auto checkValue = mapArray[n_msRight.origin.x][n_msRight.origin.y];
						if((checkValue == mapOldline || checkValue == mapOldget) && n_msRight.origin.isInnerMap())
						{
							nextLoopArray.enqueue(n_msRight);
						}
						else
						{
							rightable = false;
						}
					}
					break;
				case directionRightUp:
					if(isUpper)
					{
						IntMoveStateSTL n_msUp = IntMoveStateSTL(t_ms.origin.x, t_ms.origin.y+1, directionUp);
						auto checkValue = mapArray[n_msUp.origin.x][n_msUp.origin.y];
						if((checkValue == mapOldline || checkValue == mapOldget) && n_msUp.origin.isInnerMap() )
						{
							nextLoopArray.enqueue(n_msUp);
						}
						else		upable = false;
					}
					
					if(isRighter)
					{
						IntMoveStateSTL n_msRight = IntMoveStateSTL(t_ms.origin.x+1, t_ms.origin.y, directionRight);
						auto checkValue = mapArray[n_msRight.origin.x][n_msRight.origin.y];
						if((checkValue == mapOldline || checkValue == mapOldget) && n_msRight.origin.isInnerMap() )
						{
							nextLoopArray.enqueue(n_msRight);
						}
						else		rightable = false;
					}
					
					if(upable && rightable)
					{
						IntMoveStateSTL n_msRightUp = IntMoveStateSTL(t_ms.origin.x+1, t_ms.origin.y+1, directionRightUp);
						auto checkValue = mapArray[n_msRightUp.origin.x][n_msRightUp.origin.y];
						if((checkValue == mapOldline ||
								checkValue == mapOldget) && n_msRightUp.origin.isInnerMap() )
						{
							nextLoopArray.enqueue(n_msRightUp);
						}
						else		rightable = false;
					}
					break;
					
			}
		}
		
		isUpper = upable;
		isRighter = rightable;
		
		if(isUpper || isRighter)
		{
			for(auto& i : nextLoopArray.getTotalData())
			{
				loopArray.enqueue(i);
			}
			nextLoopArray.clear();
				
		}
	}
//	CCLOG("max1 %d max2 %d", loopArraySize, nextLoopArraySize);
	
//	CCLOG("loop count : %d", loopCnt);
	
	IntRectSTL r_rect = IntRectSTL((origin.x-1)*pixelSize, (origin.y-1)*pixelSize, size.width*pixelSize, size.height*pixelSize);
	return r_rect;
}



template <typename T>
class Code131_Queue
{
private:
	int head, tail;           // Index to top of the queue
	static const int MAXSIZE = 30000;
	T theQueue[MAXSIZE];  // The queue
public:
	Code131_Queue()         // Class constructor
	{
		head = tail = -1;
	}
	~Code131_Queue()        // Class destuctor
	{
		
	}
	void ClearQueue()       // Remove all items from the queue
	{
		head = tail = -1; // Reset indices to start over
	}
	bool Enqueue(const T& ch)   // Enter an item in the queue
	{
//		if(isFull()) return false;
		
		// Increment tail index
		tail++;
		// Add the item to the Queue
		theQueue[tail % MAXSIZE] = ch;
		CCLOG("queue = %d", tail % MAXSIZE);
		return true;
	}
	T Dequeue()          // Remove an item from the queue
	{
		T ch;
		
		head++;
		ch = theQueue[head % MAXSIZE];		// Get character to return
		return ch;				// Return popped character
		
		
		// Check for empty Queue
		if((head == tail))
		{
			assert("EMPTY");
//			return '\0';  // Return null character if queue is empty
		}
		else
		{
			head++;
			ch = theQueue[head % MAXSIZE];		// Get character to return
			return ch;				// Return popped character
		}
	}
	bool isEmpty()          // Return true if queue is empty
	{
		return (head == tail);
	}
	bool isFull()           // Return true if queue is full
	{
		return ((tail - MAXSIZE) == head);
	}
};



void MapScanner::bfsCheck(mapType beforeType, mapType afterType, IntPoint startPoint)
{
	BFS_Point s_p;
	s_p.x = startPoint.x;
	s_p.y = startPoint.y;
	CircularQueue<BFS_Point> bfsArray(300);
	myGD->mapState[s_p.x][s_p.y] = afterType;
	bfsArray.enqueue(s_p);

	vector<BFS_Point> check_new_line_list;
	auto dgPointer = myGD;
	while(!bfsArray.isEmpty())
	{
		BFS_Point t_p = bfsArray.dequeue();
		bool emptyLeft = false;
		bool emptyRight = false;
		bool emptyUp = false;
		bool emptyDown = false;
		{
			BFS_Point a_p = BFS_Point(t_p.x - 1, t_p.y + 0);
			
			if(isInnerMap(a_p))
			{
				if(dgPointer->mapState[a_p.x][a_p.y] == beforeType)
				{
					emptyLeft = true;
					dgPointer->mapState[a_p.x][a_p.y] = afterType;
					bfsArray.enqueue(a_p);
				}
				else if(dgPointer->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
				{
					check_new_line_list.push_back(a_p);
					bfsArray.enqueue(a_p);
				}
			}
		}
		{
			BFS_Point a_p = BFS_Point(t_p.x + 1, t_p.y + 0);
			
			if(isInnerMap(a_p))
			{
				if(dgPointer->mapState[a_p.x][a_p.y] == beforeType)
				{
					emptyRight = true;
					dgPointer->mapState[a_p.x][a_p.y] = afterType;
					bfsArray.enqueue(a_p);
				}
				else if(dgPointer->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
				{
					check_new_line_list.push_back(a_p);
					bfsArray.enqueue(a_p);
				}
			}
		}
		{
			BFS_Point a_p = BFS_Point(t_p.x + 0, t_p.y + 1);
			
			if(isInnerMap(a_p))
			{
				if(dgPointer->mapState[a_p.x][a_p.y] == beforeType)
				{
					emptyUp = true;
					dgPointer->mapState[a_p.x][a_p.y] = afterType;
					bfsArray.enqueue(a_p);
				}
				else if(dgPointer->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
				{
					check_new_line_list.push_back(a_p);
					bfsArray.enqueue(a_p);
				}
			}

			
		}
		{
			BFS_Point a_p = BFS_Point(t_p.x + 0, t_p.y - 1);
			
			if(isInnerMap(a_p))
			{
				if(dgPointer->mapState[a_p.x][a_p.y] == beforeType)
				{
					emptyDown = true;
					dgPointer->mapState[a_p.x][a_p.y] = afterType;
					bfsArray.enqueue(a_p);
				}
				else if(dgPointer->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
				{
					check_new_line_list.push_back(a_p);
					bfsArray.enqueue(a_p);
				}
			}

		}
		
//		if(emptyLeft && emptyUp)
//		{
//			BFS_Point t_v = directionVector(directionLeftUp);
//			BFS_Point a_p = BFS_Point(t_p.x + t_v.x, t_p.y + t_v.y);
//			
//			if(isInnerMap(a_p))
//			{
//				if(dgPointer->mapState[a_p.x][a_p.y] == beforeType)
//				{
//					dgPointer->mapState[a_p.x][a_p.y] = afterType;
//					bfsArray.enqueue(a_p);
//				}
//				else if(dgPointer->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
//				{
//					check_new_line_list.push_back(a_p);
//					bfsArray.enqueue(a_p);
//				}
//			}
//		}
//		if(emptyRight && emptyUp)
//		{
//			BFS_Point t_v = directionVector(directionRightUp);
//			BFS_Point a_p = BFS_Point(t_p.x + t_v.x, t_p.y + t_v.y);
//			
//			if(isInnerMap(a_p))
//			{
//				if(dgPointer->mapState[a_p.x][a_p.y] == beforeType)
//				{
//					dgPointer->mapState[a_p.x][a_p.y] = afterType;
//					bfsArray.enqueue(a_p);
//				}
//				else if(dgPointer->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
//				{
//					check_new_line_list.push_back(a_p);
//					bfsArray.enqueue(a_p);
//				}
//			}
//		}
//		if(emptyRight && emptyDown)
//		{
//			BFS_Point t_v = directionVector(directionRightDown);
//			BFS_Point a_p = BFS_Point(t_p.x + t_v.x, t_p.y + t_v.y);
//			
//			if(isInnerMap(a_p))
//			{
//				if(dgPointer->mapState[a_p.x][a_p.y] == beforeType)
//				{
//					dgPointer->mapState[a_p.x][a_p.y] = afterType;
//					bfsArray.enqueue(a_p);
//				}
//				else if(dgPointer->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
//				{
//					check_new_line_list.push_back(a_p);
//					bfsArray.enqueue(a_p);
//				}
//			}
//		}
//		if(emptyLeft && emptyDown)
//		{
//			BFS_Point t_v = directionVector(directionLeftDown);
//			BFS_Point a_p = BFS_Point(t_p.x + t_v.x, t_p.y + t_v.y);
//			
//			if(isInnerMap(a_p))
//			{
//				if(dgPointer->mapState[a_p.x][a_p.y] == beforeType)
//				{
//					dgPointer->mapState[a_p.x][a_p.y] = afterType;
//					bfsArray.enqueue(a_p);
//				}
//				else if(dgPointer->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
//				{
//					check_new_line_list.push_back(a_p);
//					bfsArray.enqueue(a_p);
//				}
//			}
//		}

//		for(int i=directionLeft;i<=directionUp;i+=2)
//		{
//			BFS_Point t_v = directionVector((IntDirection)i);
//			BFS_Point a_p = BFS_Point(t_p.x + t_v.x, t_p.y + t_v.y);
//			
//			if(isInnerMap(a_p))
//			{
//				if(dgPointer->mapState[a_p.x][a_p.y] == beforeType)
//				{
//					dgPointer->mapState[a_p.x][a_p.y] = afterType;
//					bfsArray.enqueue(a_p);
//				}
//				else if(dgPointer->mapState[a_p.x][a_p.y] == mapNewline && find(check_new_line_list.begin(), check_new_line_list.end(), a_p) == check_new_line_list.end())
//				{
//					check_new_line_list.push_back(a_p);
//					bfsArray.enqueue(a_p);
//				}
//			}
//		}
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

	invisibleImg = InvisibleSprite::create(CCString::createWithFormat("card%d_invisible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3))->getCString(), false);
	invisibleImg->setPosition(CCPointZero);
//	int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
//	t_spr->setColorSilhouette(NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d));
//	invisibleImg->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(0.5f),CCFadeIn::create(0.5f),nullptr)));
	

	addChild(invisibleImg, invisibleZorder);
	
	// ######################## hs code bbu woo~ ##############################
	
	EffectSprite* t_img = (EffectSprite*)invisibleImg->getChildByTag(8707);
	
//	invisibleImg->getChildByTag(8707)->removeFromParent();
//	
//	CCClippingNode* t_sprc = (CCClippingNode*)invisibleImg->getChildByTag(8706);
//	t_sprc->setVisible(true);
//	CCSprite* t_spr = (CCSprite*)t_sprc->getChildByTag(8706);
//	int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	int orir =254;
	int orig = 1; //NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d);
	int orib = 1; //NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d);
	
	t_img->setColorSilhouette(orir, orig, orib);
//	t_spr->setColor(ccc3(254, 0, 0));
	
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
			if(r>=orir || b>=orib || g>=orig)mark=0;
		}else{
			r-=rm;
			g-=gm;
			b-=bm;
			if(r<0 || g<0 || b<0)mark=1;
		}
		
		if(r<0)r=0;
		if(g<0)g=0;
		if(b<0)b=0;
		if(r>255)r=255;
		if(g>255)g=255;
		if(b>255)b=255;
		
//		t_spr->setColor(ccc3(r, g, b));
		t_img->setColorSilhouette(r, g, b);
		return true;
	}));
	// ######################## hs code bbu woo~ ##############################
	
	

	std::vector<IntRectSTL> t_rect_array;
	if(visibleImg)
	{
		t_rect_array = visibleImg->getDrawRects();
		visibleImg->removeFromParentAndCleanup(true);
		visibleImg = NULL;
	}

	visibleImg = VisibleParent::create(CCString::createWithFormat("card%d_visible.png",NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3))->getCString(), false, CCString::createWithFormat("card%d_invisible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3))->getCString(), NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3));
	visibleImg->setPosition(CCPointZero);
	addChild(visibleImg, visibleZorder);

	visibleImg->setDrawRects(t_rect_array);
	scanMap();
}

void MapScanner::randomingRectView( CCPoint t_p )
{
//	if(my_tic_toc)
//		AudioEngine::sharedInstance()->playEffect("sound_casting_attack.mp3", false);

	my_tic_toc = !my_tic_toc;

	int base_value = roundf(-t_p.y/((480.f-myGD->boarder_value*2)/(320.f))/2.f); // 중간 괄호 : myGD->game_scale

	int gacha_cnt = mySGD->getStartMapGachaCnt();
	
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

	IntPoint maxPoint = IntPoint(mapWidthInnerEnd-init_rect.size.width-2-mapWidthInnerBegin-20, init_rect.size.height-4);

	init_rect.origin.x = rand()%maxPoint.x+10;//mapWidthInnerBegin+10;
	init_rect.origin.y = rand()%maxPoint.y+base_value+roundf(screen_height/((480.f-myGD->boarder_value*2)/(320.f))/2.f)-init_rect.size.height+2; // 중간 괄호 : myGD->game_scale

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
	switch(direction)
	{
		case(directionLeftUp):			{
				return BFS_Point(-1, 1);
			}
		case(directionLeft)	:		{
				return 		BFS_Point(-1, 0);
			}
		case (directionLeftDown):		{
				return 		BFS_Point(-1, -1);
			}
		case (directionDown)	:		{
				return 		BFS_Point(0, -1);
			}
		case (directionRightDown):	{
				return 	BFS_Point(1, -1);
			}
		case (directionRight):		{
				return BFS_Point(1, 0);;
			}
		case (directionRightUp):		{
				return BFS_Point(1, 1);
			}
		case (directionUp):			{
				return BFS_Point(0, 1);
			}
		default:
			return BFS_Point(0, 0);

	}
}

bool MapScanner::isInnerMap(const BFS_Point& t_p )
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

	visibleImg = VisibleParent::create(visible_filename.filename.c_str(), visible_filename.isPattern, invisible_filename.filename, NSDS_GI(silType, kSDS_SI_level_int1_card_i, 1));
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
		
		CCClippingNode* t_clipping = CCClippingNode::create(CCSprite::create("temp_block_lock.png", CCRectMake(0, sub_value, top_block_lock->getContentSize().width, top_block_lock->getContentSize().height)));
		
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
		
		KSLabelTTF* lock_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_locked), mySGD->getFont().c_str(), 15);
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
		
		KSLabelTTF* lock_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_locked), mySGD->getFont().c_str(), 15);
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
		
		CCClippingNode* t_clipping = CCClippingNode::create(CCSprite::create("temp_block_lock.png", CCRectMake(0, 0, bottom_block_lock->getContentSize().width, bottom_block_lock->getContentSize().height)));
		
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
		t_clipping->setPosition(ccp(bottom_block_lock->getContentSize().width/2.f,bottom_block_lock->getContentSize().height));
		bottom_block_lock->addChild(t_clipping);
		
		t_clipping->setAlphaThreshold(0.1f);
		
		KSLabelTTF* lock_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_locked), mySGD->getFont().c_str(), 15);
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
		
		KSLabelTTF* lock_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_locked), mySGD->getFont().c_str(), 15);
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
	
	
	for(int i=mapWidthOutlineBegin;i<mapWidthOutlineEnd;i++)
	{
		for(int j=mapHeightOutlineBegin;j<mapHeightOutlineEnd;j++)
		{
			if(i == mapWidthOutlineBegin || j == mapHeightOutlineBegin || i == mapWidthOutlineEnd-1 || j == mapHeightOutlineEnd-1)
				mapInitState[i][j] = mapOutline;
			else
				mapInitState[i][j] = mapEmpty;
		}
	}
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
	int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	EffectSprite* t_spr = EffectSprite::createWithTexture(mySIL->addImage(filename));
	t_spr->setPosition(ccp(160,215));
	t_spr->setColorSilhouette(NSDS_GI(t_puzzle_number, kSDS_PZ_color_r_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_g_d), NSDS_GI(t_puzzle_number, kSDS_PZ_color_b_d));

	// ######################## hs code bbu woo~ ##############################
	t_spr->setTag(8707);
	// ######################## hs code bbu woo~ ##############################
	
	addChild(t_spr);
}

VisibleSprite* VisibleSprite::create( const char* filename, bool isPattern, std::vector<IntRectSTL>* t_drawRects, string sil_filename, int t_card_number )
{
	VisibleSprite* t_v = new VisibleSprite();
	t_v->myInit(filename, isPattern, t_drawRects, sil_filename, t_card_number);
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
//	std::chrono::time_point<std::chrono::system_clock> start, end;
//	start = std::chrono::system_clock::now();

	if(myGD->is_need_resetRects)
	{
		myGD->communication("MS_resetRects", true);
		myGD->is_need_resetRects = false;
	}
	
	light_img->draw();
	
	draw();
	if(mySGD->is_safety_mode)
		safety_img->draw();
	
//	end = std::chrono::system_clock::now();
//	std::chrono::duration<double> elapsed_seconds = end-start;
//	CCLog("visit time : %f", elapsed_seconds.count());
	
//	unsigned int loopCnt = drawRects->count();
//	float game_node_scale = myGD->Fcommunication("Main_getGameNodeScale");
//	
//	glEnable(GL_SCISSOR_TEST);
//
//	for(int i=0;i<loopCnt;i++)
//	{
//		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
//		
//		float wScale = viewport[2] / design_resolution_size.width;
//		float hScale = viewport[3] / design_resolution_size.height;
//		
//		float x, y, w, h;
//		
//		if(is_set_scene_node)
//		{
//			x = (t_rect->origin.x*game_node_scale+jack_position.x+scene_node->getPositionX())*wScale + viewport[0]-1-1;
//			y = (t_rect->origin.y*game_node_scale+jack_position.y+scene_node->getPositionY())*hScale + viewport[1]-1-1;
//			w = (t_rect->size.width*game_node_scale)*wScale+2+2;
//			h = (t_rect->size.height*game_node_scale)*hScale+2+2;
//		}
//		else
//		{
//			x = (t_rect->origin.x*game_node_scale+jack_position.x)*wScale + viewport[0]-1-1;
//			y = (t_rect->origin.y*game_node_scale+jack_position.y)*hScale + viewport[1]-1-1;
//			w = (t_rect->size.width*game_node_scale)*wScale+2+2;
//			h = (t_rect->size.height*game_node_scale)*hScale+2+2;
//		}
//		
//		if(y > screen_size.height || y+h < 0.f)
//			continue;
//		else
//		{
//			glScissor(x,y,w,h);
//			light_img->draw();
//		}
//		
//	}
//	
//	for(int i=0;i<loopCnt;i++)
//	{
//		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
//
//		float wScale = viewport[2] / design_resolution_size.width;
//		float hScale = viewport[3] / design_resolution_size.height;
//
//		float x, y, w, h;
//
//		if(is_set_scene_node)
//		{
//			x = (t_rect->origin.x*game_node_scale+jack_position.x+scene_node->getPositionX())*wScale + viewport[0]-1;
//			y = (t_rect->origin.y*game_node_scale+jack_position.y+scene_node->getPositionY())*hScale + viewport[1]-1;
//			w = (t_rect->size.width*game_node_scale)*wScale+2;
//			h = (t_rect->size.height*game_node_scale)*hScale+2;
//		}
//		else
//		{
//			x = (t_rect->origin.x*game_node_scale+jack_position.x)*wScale + viewport[0]-1;
//			y = (t_rect->origin.y*game_node_scale+jack_position.y)*hScale + viewport[1]-1;
//			w = (t_rect->size.width*game_node_scale)*wScale+2;
//			h = (t_rect->size.height*game_node_scale)*hScale+2;
//		}
//
//		if(y > screen_size.height || y+h < 0.f)
//			continue;
//		else
//		{
//			glScissor(x,y,w,h);
//			draw();
//			if(mySGD->is_safety_mode)
//				safety_img->draw();
//		}
//
//	}
//
//	glDisable(GL_SCISSOR_TEST);
}

void VisibleSprite::draw()
{
//	CCSprite::draw();
//	return;
//	glEnable(GL_DEPTH_TEST);
//	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//	KS::KSLog("% % % %", m_vertices, m_textCoords, m_colors, t_vertice_count);
	CC_NODE_DRAW_SETUP();
	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
	//세팅
//	getTexture()->getShaderProgram()->use();
//	
//	getTexture()->getShaderProgram()->setUniformsForBuiltins();
	
	ccGLBindTexture2D(getTexture()->getName());
//	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
	
	#define kQuadSize sizeof(m_sQuad.bl)
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
	{
		glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_textCoords);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, m_colors);
		glDrawArrays(GL_TRIANGLES, 0, t_vertice_count);
	}
//	glDisable(GL_DEPTH_TEST);
}

void VisibleSprite::visitForThumb()
{
//	unsigned int loopCnt = drawRects->count();
//	
//	glEnable(GL_SCISSOR_TEST);
//	
//	for(int i=0;i<loopCnt;i++)
//	{
//		IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
//		
//		float wScale = viewport[2] / (design_resolution_size.width + (viewport[2]-960.f)/2.f); // 1024, 768 / 480, 360 -> + 32, 24
//		float hScale = viewport[3] / (design_resolution_size.height + (viewport[2]-960.f)/2.f*design_resolution_size.height/design_resolution_size.width); // 1136, 641 / 480, 271 -> + 89, 50
//		
//		float x, y, w, h;
//		
//		if(is_set_scene_node)
//		{
//			x = t_rect->origin.x*wScale + viewport[0]-1;
//			y = t_rect->origin.y*hScale + viewport[1]-1;
//			w = t_rect->size.width*wScale + 2;
//			h = t_rect->size.height*hScale + 2;
//		}
//		else
//		{
//			x = t_rect->origin.x*wScale + viewport[0]-1;
//			y = t_rect->origin.y*hScale + viewport[1]-1;
//			w = t_rect->size.width*wScale + 2;
//			h = t_rect->size.height*hScale + 2;
//		}
//		
//		glScissor(x,y,w,h);
		draw();
		if(mySGD->is_safety_mode)
			safety_img->draw();
//	}
//	
//	glDisable(GL_SCISSOR_TEST);
}

void VisibleSprite::replayVisitForThumb(int temp_time)
{
	int play_index = mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_playIndex)].asInt();
	if(play_index >= mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size())
		return;
	
	if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)][play_index].asInt() > temp_time)
		return;
		
	
	memset(replay_draw_array, 0, sizeof(replay_draw_array));
	
//	replay_draw_array[162][217] = {mapEmpty,};
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
						replay_draw_array[i][j] = mapOldline;
		}
	}
	
	rects.clear();
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
		{
			if(replay_draw_array[i][j] == mapOldline)
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
						
						if(replay_draw_array[t_ms.origin.x][t_ms.origin.y] == mapOldline)			replay_draw_array[t_ms.origin.x][t_ms.origin.y] = mapScaningCheckLine;
						
						if(t_ms.direction == directionUp)
						{
							if(isUpper)
							{
								IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
								if(n_msUp.origin.isInnerMap() && replay_draw_array[n_msUp.origin.x][n_msUp.origin.y] == mapOldline)
									nextLoopArray.push(n_msUp);
								else		upable = false;
							}
						}
						else if(t_ms.direction == directionRight)
						{
							if(isRighter)
							{
								IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
								if(n_msRight.origin.isInnerMap() && replay_draw_array[n_msRight.origin.x][n_msRight.origin.y] == mapOldline)
									nextLoopArray.push(n_msRight);
								else		rightable = false;
							}
						}
						else if(t_ms.direction == directionRightUp)
						{
							if(isUpper)
							{
								IntMoveState n_msUp = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
								if(n_msUp.origin.isInnerMap() && replay_draw_array[n_msUp.origin.x][n_msUp.origin.y] == mapOldline)
									nextLoopArray.push(n_msUp);
								else		upable = false;
							}
							
							if(isRighter)
							{
								IntMoveState n_msRight = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
								if(n_msRight.origin.isInnerMap() && replay_draw_array[n_msRight.origin.x][n_msRight.origin.y] == mapOldline)
									nextLoopArray.push(n_msRight);
								else		rightable = false;
							}
							
							if(upable && rightable)
							{
								IntMoveState n_msRightUp = IntMoveState(t_ms.origin.x+1, t_ms.origin.y+1, directionRightUp);
								if(n_msRightUp.origin.isInnerMap() && replay_draw_array[n_msRightUp.origin.x][n_msRightUp.origin.y] == mapOldline)
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
				
				rects.push_back(IntRectSTL((origin.x-1)*pixelSize, (origin.y-1)*pixelSize, size.width*pixelSize, size.height*pixelSize));
			}
		}
	}
	
	auto keep_array = *drawRects;
	*drawRects = rects;
	setRectToVertex();

		draw();
		if(mySGD->is_safety_mode)
			safety_img->draw();

	
//	visit();
	
	mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_playIndex)] = play_index+1;
	
	*drawRects = keep_array;
	setRectToVertex();
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

void VisibleSprite::setRectToVertex()
{
	chrono::time_point<chrono::system_clock> start, end;
	chrono::duration<double> elapsed_seconds;
	start = chrono::system_clock::now();
	if(m_vertices)
	{
		delete [] m_vertices;
		m_vertices = nullptr;
	}
	if(m_textCoords)
	{
		delete [] m_textCoords;
		m_textCoords = nullptr;
	}
	if(light_vertices)
	{
		delete [] light_vertices;
		light_vertices = nullptr;
	}
	if(safety_vertices)
	{
		delete [] safety_vertices;
		safety_vertices = nullptr;
	}
	if(m_colors)
	{
		delete [] m_colors;
		m_colors = nullptr;
		
	}
	
	t_vertice_count = drawRects->size() * 6;
	
	m_vertices = new Vertex3D[t_vertice_count];
	m_textCoords = new ccTex2F[t_vertice_count];
	
	light_vertices = new Vertex3D[t_vertice_count];
	safety_vertices = new Vertex3D[t_vertice_count];
	
	m_colors = new tColor4B[t_vertice_count];
	for(int i=0;i<drawRects->size() ;i++)
	{
		IntRectSTL t_rect = (*drawRects)[i];
		m_vertices[i*6+0].x = t_rect.x;
		m_vertices[i*6+0].y = t_rect.y;
		m_vertices[i*6+0].z = 0;
		
		m_vertices[i*6+1].x = t_rect.x;
		m_vertices[i*6+1].y = t_rect.y + t_rect.height;
		m_vertices[i*6+1].z = 0;
		
		m_vertices[i*6+2].x = t_rect.x + t_rect.width;
		m_vertices[i*6+2].y = t_rect.y + t_rect.height;
		m_vertices[i*6+2].z = 0;
		
		m_vertices[i*6+3].x = t_rect.x;
		m_vertices[i*6+3].y = t_rect.y;
		m_vertices[i*6+3].z = 0;
		
		m_vertices[i*6+4].x = t_rect.x + t_rect.width;
		m_vertices[i*6+4].y = t_rect.y;
		m_vertices[i*6+4].z = 0;
		
		m_vertices[i*6+5].x = t_rect.x + t_rect.width;
		m_vertices[i*6+5].y = t_rect.y + t_rect.height;
		m_vertices[i*6+5].z = 0;
		
		
		m_textCoords[i*6+0].u = t_rect.x/320.f;
		m_textCoords[i*6+0].v = (430.f - t_rect.y) / 430.f;
		
		m_textCoords[i*6+1].u = t_rect.x/320.f;
		m_textCoords[i*6+1].v = (430.f - (t_rect.y + t_rect.height)) / 430.f;
		
		m_textCoords[i*6+2].u = (t_rect.x + t_rect.width)/320.f;
		m_textCoords[i*6+2].v = (430.f - (t_rect.y + t_rect.height)) / 430.f;
		
		m_textCoords[i*6+3].u = t_rect.x/320.f;
		m_textCoords[i*6+3].v = (430.f - t_rect.y) / 430.f;
		
		m_textCoords[i*6+4].u = (t_rect.x + t_rect.width)/320.f;
		m_textCoords[i*6+4].v = (430.f - t_rect.y) / 430.f;
		
		m_textCoords[i*6+5].u = (t_rect.x + t_rect.width)/320.f;
		m_textCoords[i*6+5].v = (430.f - (t_rect.y + t_rect.height)) / 430.f;
		
		
		
		light_vertices[i*6+0].x = t_rect.x-0.5f;
		light_vertices[i*6+0].y = t_rect.y-0.5f;
		light_vertices[i*6+0].z = -1;
		
		light_vertices[i*6+1].x = t_rect.x-0.5f;
		light_vertices[i*6+1].y = t_rect.y + t_rect.height+0.5f;
		light_vertices[i*6+1].z = -1;
		
		light_vertices[i*6+2].x = t_rect.x + t_rect.width+0.5f;
		light_vertices[i*6+2].y = t_rect.y + t_rect.height+0.5f;
		light_vertices[i*6+2].z = -1;
		
		light_vertices[i*6+3].x = t_rect.x-0.5f;
		light_vertices[i*6+3].y = t_rect.y-0.5f;
		light_vertices[i*6+3].z = -1;
		
		light_vertices[i*6+4].x = t_rect.x + t_rect.width+0.5f;
		light_vertices[i*6+4].y = t_rect.y-0.5f;
		light_vertices[i*6+4].z = -1;
		
		light_vertices[i*6+5].x = t_rect.x + t_rect.width+0.5f;
		light_vertices[i*6+5].y = t_rect.y + t_rect.height+0.5f;
		light_vertices[i*6+5].z = -1;
		
		for(int j=0;j<6;j++)
		{
			safety_vertices[i*6+j].x = m_vertices[i*6+j].x;
			safety_vertices[i*6+j].y = m_vertices[i*6+j].y;
			safety_vertices[i*6+j].z = 1;
			
			m_colors[i*6+j].r = 255;
			m_colors[i*6+j].g = 255;
			m_colors[i*6+j].b = 255;
			m_colors[i*6+j].a = 255;
		}
	}
	safety_img->setVertex(safety_vertices, m_textCoords, m_colors, t_vertice_count);
	light_img->setVertex(light_vertices, m_textCoords, m_colors, t_vertice_count);
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("setRectToVertex step 1 / time : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
}

void VisibleSprite::myInit( const char* filename, bool isPattern, std::vector<IntRectSTL>* t_drawRects, string sil_filename, int t_card_number )
{
	initWithTexture(mySIL->addImage(filename));
	setBrighten(1.f);
//	setColor(ccGRAY);
	setPosition(ccp(160,215));

//	if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, t_card_number))
//	{
//		CCSprite* ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card_number)).first;
//		ccb_img->setPosition(ccp(160,215));
//		addChild(ccb_img);
//	}
	
	
	is_set_scene_node = false;

	screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	design_resolution_size = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
	glGetIntegerv(GL_VIEWPORT, viewport);

	m_vertices = NULL;
	m_textCoords = NULL;
	light_vertices = NULL;
	safety_vertices = NULL;
	m_colors = NULL;
	
	drawRects = t_drawRects;
	
	safety_img = RectsSprite::createWithTexture(mySIL->addImage(sil_filename.c_str()));
	safety_img->setSilhouetteConvert(0);
	safety_img->setPosition(ccp(getContentSize().width/2.f, getContentSize().height/2.f));
	addChild(safety_img);
	
	
	light_img = RectsSprite::create("ingame_whiteback.png");
	light_img->setPosition(ccp(getContentSize().width/2.f, getContentSize().height/2.f));
	addChild(light_img, -1);
	
	setRectToVertex();
	
	light_r = 255;
	light_g = 100;
	light_b = 100;
	
	light_step = 0;
	light_frame = 0;
	
	addChild(KSSchedule::create([=](float dt){
		
		if(light_step == 0)
		{
			light_r = 255-light_frame/60.f*155;
			light_g = 100+light_frame/60.f*155;
			light_b = 100;
		}
		else if(light_step == 1)
		{
			light_r = 100;
			light_g = 255-light_frame/60.f*155;
			light_b = 100+light_frame/60.f*155;
		}
		else if(light_step == 2)
		{
			light_r = 100+light_frame/60.f*155;
			light_g = 100;
			light_b = 255;
		}
		else if(light_step == 3)
		{
			light_r = 255;
			light_g = 100+light_frame/60.f*155;
			light_b = 255-light_frame/60.f*155;
		}
		else if(light_step == 4)
		{
			light_r = 255-light_frame/60.f*155;
			light_g = 255;
			light_b = 100+light_frame/60.f*155;
		}
		else if(light_step == 5)
		{
			light_r = 100+light_frame/60.f*155;
			light_g = 255;
			light_b = 255;
		}
		else if(light_step == 6)
		{
			light_r = 255;
			light_g = 255-light_frame/60.f*155;
			light_b = 255-light_frame/60.f*155;
		}
		
		light_img->setColorSilhouette(light_r, light_g, light_b);
		
		light_frame++;
		if(light_frame >= 60)
		{
			light_step++;
			if(light_step > 6)
				light_step = 0;
			light_frame = 0;
		}
		return true;
	}));
}

CCTexture2D* VisibleSprite::createSafetyImage(string fullpath){
	
	
	CCImage* img = new CCImage();
	img->initWithEncryptedImageFileFullPath(fullpath.c_str());
	
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

VisibleParent* VisibleParent::create( const char* filename, bool isPattern, string sil_filename, int t_card_number )
{
	VisibleParent* t_vp = new VisibleParent();
	t_vp->myInit(filename, isPattern, sil_filename, t_card_number);
	t_vp->autorelease();
	return t_vp;
}

void VisibleParent::setDrawRects( const std::vector<IntRectSTL>& t_rects )
{
	drawRects = t_rects;
	myVS->setRectToVertex();
}

const std::vector<IntRectSTL>& VisibleParent::getDrawRects()
{
	return drawRects;
}

void VisibleParent::divideRect( IntPoint crashPoint )
{
	chrono::time_point<chrono::system_clock> start, end;
	chrono::duration<double> elapsed_seconds;
	start = chrono::system_clock::now();
	if(mySD->silData[crashPoint.x][crashPoint.y] == true)
		myGD->communication("UI_decreasePercentage");

	crashPoint.x = (crashPoint.x-1)*pixelSize;
	crashPoint.y = (crashPoint.y-1)*pixelSize;
	int loopCnt = drawRects.size();
	auto drawRectsCopy = drawRects;
	drawRects.clear();
//	for(int i=0;i<loopCnt;i++)
	for(auto iter = drawRectsCopy.begin(); iter != drawRectsCopy.end(); ++iter)
	{
		IntRectSTL t_rect = *iter;
		if(crashPoint.x >= t_rect.x && crashPoint.x < t_rect.x + t_rect.width && crashPoint.y >= t_rect.y && crashPoint.y < t_rect.y + t_rect.height)
		{
			// divide rect
			IntSize t_size;

			// left down
			t_size.width = crashPoint.x - t_rect.x;
			t_size.height = crashPoint.y - t_rect.y + pixelSize;
			if(t_size.width >= pixelSize) // left down create
			{
				drawRects.push_back(IntRectSTL(t_rect.x, t_rect.y, t_size.width, t_size.height));
			}

			// down right
			t_size.width = t_rect.x + t_rect.width - crashPoint.x;
			t_size.height = crashPoint.y - t_rect.y;
			if(t_size.height >= pixelSize) // down right create
			{
				drawRects.push_back(IntRectSTL(crashPoint.x, t_rect.y, t_size.width, t_size.height));
//				IntRect* n_rect = new IntRect(crashPoint.x, t_rect.y, t_size.width, t_size.height);
//				n_rect->autorelease();
//				drawRects->addObject(n_rect);
			}

			// right up
			t_size.width = t_rect.x + t_rect.width - crashPoint.x - pixelSize;
			t_size.height = t_rect.y + t_rect.height - crashPoint.y;
			if(t_size.width >= pixelSize)
			{
				drawRects.push_back(IntRectSTL(crashPoint.x+pixelSize, crashPoint.y, t_size.width, t_size.height));
//				IntRect* n_rect = new IntRect(crashPoint.x+pixelSize, crashPoint.y, t_size.width, t_size.height);
//				n_rect->autorelease();
//				drawRects->addObject(n_rect);
			}

			// up left
			t_size.width = crashPoint.x - t_rect.x + pixelSize;
			t_size.height = t_rect.y + t_rect.height - crashPoint.y - pixelSize;
			if(t_size.height >= pixelSize)
			{
				drawRects.push_back(IntRectSTL(t_rect.x, crashPoint.y + pixelSize, t_size.width, t_size.height));
//				IntRect* n_rect = new IntRect(t_rect.x, crashPoint.y + pixelSize, t_size.width, t_size.height);
//				n_rect->autorelease();
//				drawRects->addObject(n_rect);
			}
		}
		else
		{
			drawRects.push_back(*iter);
		}
	}
	
	
	myVS->setRectToVertex();
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
	CCLOG("devideRect step 1 / time : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
}

void VisibleParent::divideRects(IntRect crashRect)
{
	chrono::time_point<chrono::system_clock> start, end;
	chrono::duration<double> elapsed_seconds;
	start = chrono::system_clock::now();
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
	
	int loopCnt = drawRects.size();
	auto drawRectsCopy = drawRects;
	drawRects.clear();
	
	for(auto iter = drawRectsCopy.begin(); iter != drawRectsCopy.end(); ++iter)
	{
		IntRectSTL t_rect = *iter;
		CCRect f_rect = CCRectMake(t_rect.x, t_rect.y, t_rect.width, t_rect.height);
		if(crash_rect.intersectsRect(f_rect))
		{
			// divide rect
			IntSize t_size;
			
			// left down
			t_size.width = crashRect.origin.x - t_rect.x;
			t_size.height = (crashRect.origin.y + crashRect.size.height) - t_rect.y;
			if(t_size.width >= pixelSize && t_size.height >= pixelSize) // left down create
			{
				if(t_size.width > t_rect.width)
					t_size.width = t_rect.width;
				if(t_size.height > t_rect.height)
					t_size.height = t_rect.height;
				
				drawRects.push_back(IntRectSTL(t_rect.x, t_rect.y, t_size.width, t_size.height));
			}
			
			// down right
			t_size.width = (t_rect.x + t_rect.width) - crashRect.origin.x;
			t_size.height = crashRect.origin.y - t_rect.y;
			if(t_size.height >= pixelSize && t_size.width >= pixelSize) // down right create
			{
				if(t_size.width > t_rect.width)
					t_size.width = t_rect.width;
				if(t_size.height > t_rect.height)
					t_size.height = t_rect.height;
				
				int after_x;
				if(crashRect.origin.x < t_rect.x)
					after_x = t_rect.x;
				else
					after_x = crashRect.origin.x;
				
				drawRects.push_back(IntRectSTL(after_x, t_rect.y, t_size.width, t_size.height));
			}
			
			// right up
			t_size.width = (t_rect.x + t_rect.width) - (crashRect.origin.x + crashRect.size.width);
			t_size.height = (t_rect.y + t_rect.height) - crashRect.origin.y;
			if(t_size.width >= pixelSize && t_size.height >= pixelSize)
			{
				if(t_size.width > t_rect.width)
					t_size.width = t_rect.width;
				if(t_size.height > t_rect.height)
					t_size.height = t_rect.height;
				
				int after_y;
				if(crashRect.origin.y < t_rect.y)
					after_y = t_rect.y;
				else
					after_y = crashRect.origin.y;
				
				drawRects.push_back(IntRectSTL(crashRect.origin.x + crashRect.size.width, after_y, t_size.width, t_size.height));
			}
			
			// up left
			t_size.width = (crashRect.origin.x + crashRect.size.width) - t_rect.x;
			t_size.height = (t_rect.y + t_rect.height) - (crashRect.origin.y + crashRect.size.height);
			if(t_size.height >= pixelSize && t_size.width >= pixelSize)
			{
				if(t_size.width > t_rect.width)
					t_size.width = t_rect.width;
				if(t_size.height > t_rect.height)
					t_size.height = t_rect.height;
				
				drawRects.push_back(IntRectSTL(t_rect.x, crashRect.origin.y + crashRect.size.height, t_size.width, t_size.height));
			}
		
		}
		else
		{
			drawRects.push_back(*iter);
		}
	}
	
	myVS->setRectToVertex();
	end = chrono::system_clock::now();
	elapsed_seconds = end-start;
//	CCLOG("devide 렉츠. 1 / time : %f", elapsed_seconds.count());
	start = chrono::system_clock::now();
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

void VisibleParent::myInit( const char* filename, bool isPattern, string sil_filename, int t_card_number )
{
//	drawRects = new CCArray(1);
	drawRects.clear();
	setPosition(CCPointZero);

	myGD->V_Ip["VS_divideRect"] = std::bind(&VisibleParent::divideRect, this, _1);
	myGD->V_Ir["VS_divideRects"] = std::bind(&VisibleParent::divideRects, this, _1);
	myGD->V_CCP["VS_setMoveGamePosition"] = std::bind(&VisibleParent::setMoveGamePosition, this, _1);
	myGD->V_V["VS_setLimittedMapPosition"] = std::bind(&VisibleParent::setLimittedMapPosition, this);
	myGD->V_I["VS_changingGameStep"] = std::bind(&VisibleParent::changingGameStep, this, _1);

	myVS = VisibleSprite::create(filename, isPattern, &drawRects, sil_filename, t_card_number);
	myVS->setPosition(CCPointZero);
	addChild(myVS);

	myGD->V_CCO["VS_setSceneNode"] = std::bind(&VisibleSprite::setSceneNode, myVS, _1);
	myGD->V_V["VS_setLight"] = std::bind(&VisibleSprite::setLight, myVS);
	myGD->CCP_V["VS_getMoveGamePosition"] = std::bind(&VisibleSprite::getMoveGamePosition, myVS);
	
}
