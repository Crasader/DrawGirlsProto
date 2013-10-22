//
//  PathManager.h
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 26..
//
//

#ifndef DrawingJack_PathManager_h
#define DrawingJack_PathManager_h

#include "cocos2d.h"
#include "GameData.h"
#include <list>
#include <algorithm>

using namespace cocos2d;
using namespace std;

enum childTagInPathParent{
	childTagInPathParentPathNode = 1,
	childTagInPathParentPathBreaking
};

class PathNode : public CCNode
{
public:
	IntPointVector myPointVector;
	CCSprite* pathImg;
	int pathScale;
	
	static PathNode* create(IntPointVector t_pv)
	{
		PathNode* t_pn = new PathNode();
		t_pn->myInit(t_pv);
		t_pn->autorelease();
		return t_pn;
	}
	
	void changeScaleImg(int changeValue)
	{
		pathScale += changeValue;
		pathImg->setScaleX(pathScale);
	}
	
private:
	CCSprite* pathEdge;
	
	void myInit(IntPointVector t_pv)
	{
		myPointVector = t_pv;
		
		ElementCode elemental_type = ElementCode(DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_lastSelectedElement));
		string elemental_string;
		if(elemental_type == kElementCode_empty)			elemental_string = "path_empty.png";
		else if(elemental_type == kElementCode_life)		elemental_string = "path_life.png";
		else if(elemental_type == kElementCode_fire)		elemental_string = "path_fire.png";
		else if(elemental_type == kElementCode_water)		elemental_string = "path_water.png";
		else if(elemental_type == kElementCode_wind)		elemental_string = "path_wind.png";
		else if(elemental_type == kElementCode_lightning)	elemental_string = "path_lightning.png";
		else if(elemental_type == kElementCode_plasma)		elemental_string = "path_plasma.png";
		
		pathImg = CCSprite::create(elemental_string.c_str());
		pathImg->setAnchorPoint(ccp(0.0, 0.5));
		pathImg->setRotation(myPointVector.distance.getAngle());
		pathScale = 1;
		pathImg->setScaleX(pathScale);
		addChild(pathImg);
		setPosition(ccp((myPointVector.origin.x-1)*pixelSize+1, (myPointVector.origin.y-1)*pixelSize+1));
		
//		if(elemental_type == kElementCode_empty)			elemental_string = "path_edge_empty.png";
//		else if(elemental_type == kElementCode_life)		elemental_string = "path_edge_life.png";
//		else if(elemental_type == kElementCode_fire)		elemental_string = "path_edge_fire.png";
//		else if(elemental_type == kElementCode_water)		elemental_string = "path_edge_water.png";
//		else if(elemental_type == kElementCode_wind)		elemental_string = "path_edge_wind.png";
//		else if(elemental_type == kElementCode_lightning)	elemental_string = "path_edge_lightning.png";
//		else if(elemental_type == kElementCode_plasma)		elemental_string = "path_edge_plasma.png";
//		
//		pathEdge = CCSprite::create(elemental_string.c_str());
//		addChild(pathEdge);
	}
};

class PathBreaking : public CCSprite
{
public:
	static PathBreaking* create(IntPoint* s_p)
	{
		PathBreaking* t_pb = new PathBreaking();
		if(t_pb && t_pb->initWithFile("bomb.png", CCRectMake(0, 0, 26, 26)))
		{
			t_pb->myInit(s_p);
			t_pb->autorelease();
			return t_pb;
		}
		CC_SAFE_DELETE(t_pb);
		return NULL;
	}
	
	void startBomb()
	{
		CCSprite* texture = CCSprite::create("bomb.png");
		CCAnimation* t_anin = CCAnimation::create();
		t_anin->setDelayPerUnit(0.1);
		for(int i=0;i<5;i++)
			t_anin->addSpriteFrameWithTexture(texture->getTexture(), CCRectMake(i*26, 0, 26, 26));
		CCAnimate* t_anit = CCAnimate::create(t_anin);
		
		CCCallFunc* callSelfRemove = CCCallFunc::create(this, callfunc_selector(PathBreaking::selfRemove));
		
		CCSequence* t_s = CCSequence::createWithTwoActions(t_anit, callSelfRemove);
		
		runAction(t_s);
	}
	
private:
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit(IntPoint* s_p)
	{
		setPosition(ccp((s_p->x-1)*pixelSize+1, (s_p->y-1)*pixelSize+1));
	}
};

enum pathBreakingState{
	pathBreakingStateFalse = 1,
	pathBreakingStateTrue
};

class PathBreakingParent : public CCNode
{
public:
	static PathBreakingParent* create()
	{
		PathBreakingParent* t_pbp = new PathBreakingParent();
		t_pbp->myInit();
		t_pbp->autorelease();
		return t_pbp;
	}
	
	void pathChainBomb(IntPoint s_p, list<PathNode*>* t_it)
	{
		IntPoint* start = new IntPoint(s_p.x, s_p.y);
		start->autorelease();
		
		it = t_it;
		
		for(auto i = it->begin();i!=it->end();i++)
		{
			PathNode* t_pn = *i;
			
		}
		
		PathBreaking* s_pb = PathBreaking::create(start);
		addChild(s_pb);
		s_pb->startBomb();
		
		schedule(schedule_selector(PathBreakingParent::chainBombSchedule));
	}
	
private:
	
	list<PathNode*>* it;
	
	void myInit()
	{
		
	}
	
	void chainBombSchedule()
	{
//		if(!chainBombArray.empty())
//		{
//			IntPoint* t_p = chainBombArray.front();
//			
//			IntPoint jackPoint = myGD->getJackPoint();
//			
//			CCPoint sub_ccp = ccpSub(t_p->convertToCCP(), jackPoint.convertToCCP());
//			float sub_value = sqrtf(powf(sub_ccp.x, 2.f) + powf(sub_ccp.y, 2.f));
//			
//			if(sub_value < 4.f) // die
//			{
//				myGD->communication("Jack_startDieEffect");
//				beforeArray->removeAllObjects();
//				getParent()->setTag(pathBreakingStateFalse);
//				unschedule(schedule_selector(PathBreakingParent::chainBombSchedule));
//				removeFromParentAndCleanup(true);
//				return;
//			}
//				
//			chainBombArray.pop();
//			
//			for(int i=directionLeft;i<=directionUp;i+=2)
//			{
//				IntVector t_v = IntVector::directionVector((IntDirection)i);
//				IntPoint* n_p = new IntPoint(t_p->x + t_v.dx, t_p->y + t_v.dy);
//				n_p->autorelease();
//				
//				bool isContains = false;
//				int loopCnt = beforeArray->count();
//				for(int j=0;j<loopCnt;j++)
//				{
//					IntPoint* c_p = (IntPoint*)beforeArray->objectAtIndex(j);
//					if(c_p->x == n_p->x && c_p->y == n_p->y)
//					{
//						isContains = true;
//						break;
//					}
//				}
//				
//				if(n_p->isInnerMap() && myGD->mapState[n_p->x][n_p->y] == mapNewline && !isContains)
//				{
//					chainBombArray.push(n_p);
//					beforeArray->addObject(n_p);
//					
//					PathBreaking* t_pb = PathBreaking::create(n_p);
//					addChild(t_pb);
//					t_pb->startBomb();
//				}
//			}
//		}
//		else
//		{
//			beforeArray->removeAllObjects();
//			getParent()->setTag(pathBreakingStateFalse);
//			unschedule(schedule_selector(PathBreakingParent::chainBombSchedule));
//			removeFromParentAndCleanup(true);
//		}
	}
};

class PathManager : public CCNode
{
public:
	static PathManager* create()
	{
		PathManager* t_PM = new PathManager();
		t_PM->myInit();
		t_PM->autorelease();
		return t_PM;
	}
	
	void addPath(IntPointVector t_pv) // t_ms.origin == beforePoint
	{
		if(myList.empty()) // first path
			newPathAdd(t_pv);
		else if(myList.back()->myPointVector.distance.getAngle() != t_pv.distance.getAngle()) // differ last path
			newPathAdd(t_pv);
		else
			originalExpansion();
	}
	
	void cleanPath()
	{
		myList.clear();
		
		bool isRemoveTargetNull = false;
		
		while (!isRemoveTargetNull)
		{
			CCNode* t_child = getChildByTag(childTagInPathParentPathNode);
			if(t_child == NULL)
				isRemoveTargetNull = true;
			else
				removeChildByTag(childTagInPathParentPathNode, true);
		}
//		removeAllChildrenWithCleanup(true);
	}
	
	IntPoint pathBackTracking()
	{
		if(!myList.empty())
		{
			PathNode* b_node = myList.back();
			
			if(b_node->pathScale > 1) // reduce
			{
				b_node->changeScaleImg(-1);
				IntPointVector b_pv = b_node->myPointVector;
				IntPoint r_p = IntPoint(roundf(b_pv.origin.x + b_pv.distance.dx*b_node->pathScale), roundf(b_pv.origin.y + b_pv.distance.dy*b_node->pathScale));
				if(myGD->mapState[r_p.x][r_p.y] == mapNewline)
				{
					aloneNewlineRemove(r_p);
					myGD->mapState[r_p.x][r_p.y] = mapEmpty;
				}
				
				return r_p;
			}
			else // pop back
			{
				IntPointVector b_pv = b_node->myPointVector;
				IntPoint r_p = IntPoint(b_pv.origin.x, b_pv.origin.y);
				
				if(myGD->mapState[r_p.x][r_p.y] == mapNewline)
				{
					aloneNewlineRemove(r_p);
					myGD->mapState[r_p.x][r_p.y] = mapEmpty;
				}
				
				myList.pop_back();
				b_node->removeFromParentAndCleanup(true);
				return r_p;
			}
		}
		else
		{
			return IntPoint();
		}
	}
	
private:
	
	list<PathNode*> myList;
	
	void addPathBreaking(IntPoint start)
	{
		if(getTag() == pathBreakingStateFalse)
		{
			setTag(pathBreakingStateTrue);
			PathBreakingParent* n_pbp = PathBreakingParent::create();
			n_pbp->setTag(childTagInPathParentPathBreaking);
			addChild(n_pbp);
			n_pbp->pathChainBomb(start, &myList);
		}
	}
	
	void aloneNewlineRemove(IntPoint r_p)
	{
		for(int i=r_p.x-2;i<=r_p.x+2;i++)
		{
			for(int j=r_p.y-2;j<=r_p.y+2;j++)
			{
				if(i == r_p.x && j == r_p.y)
					continue;
				IntPoint t_p = IntPoint(i,j);
				if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
				{
					if(myGD->mapState[t_p.x-1][t_p.y] != mapNewline && myGD->mapState[t_p.x][t_p.y-1] != mapNewline &&
					   myGD->mapState[t_p.x+1][t_p.y] != mapNewline && myGD->mapState[t_p.x][t_p.y+1] != mapNewline)
						myGD->mapState[t_p.x][t_p.y] = mapEmpty;
				}
			}
		}
	}
	
	void originalExpansion()
	{
		myList.back()->changeScaleImg(1);
	}
	
	void newPathAdd(IntPointVector t_pv)
	{
		PathNode* t_pn = PathNode::create(t_pv);
		t_pn->setTag(childTagInPathParentPathNode);
		addChild(t_pn);
		
		myList.push_back(t_pn);
	}
	
	void myInit()
	{
		
//		myGD->regPM(this, callfuncIpv_selector(PathManager::addPath),
//					callfunc_selector(PathManager::cleanPath),
//					callfuncIp_selector(PathManager::addPathBreaking));
		
		myGD->V_Ipv["PM_addPath"] = std::bind(&PathManager::addPath, this, _1);
		myGD->V_V["PM_cleanPath"] = std::bind(&PathManager::cleanPath, this);
		myGD->V_Ip["PM_addPathBreaking"] = std::bind(&PathManager::addPathBreaking, this, _1);
		setTag(pathBreakingStateFalse);
	}
};

#endif
