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
#include "cocos-ext.h"
#include "LogData.h"
#include "DataStorageHub.h"
#include "KSUtil.h"

using namespace cocos2d;
using namespace std;
using namespace extension;

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
	
	static PathNode* create(IntPointVector t_pv, string line_color)
	{
		PathNode* t_pn = new PathNode();
		t_pn->myInit(t_pv, line_color);
		t_pn->autorelease();
		return t_pn;
	}
	
	void changeScaleImg(int changeValue, IntPoint t_p = IntPoint())
	{
//		if(!t_p.isNull())
//		{
//			int distance = abs(myPointVector.origin.x - t_p.x) + abs(myPointVector.origin.y - t_p.y);
//			
//			if(distance < pathScale + changeValue - 1)
//				return;
//		}
		
		pathScale += changeValue;
		pathImg->setScaleX(float(pathScale));
	}
	void changeScaleDirect(int set_value)
	{
		pathScale = set_value;
		pathImg->setScaleX(float(pathScale));
	}
	
private:
	CCSprite* pathEdge;
	
	void myInit(IntPointVector t_pv, string line_color)
	{
		myPointVector = t_pv;
		
		pathImg = CCSprite::create(("path_" + line_color + ".png").c_str());
		pathImg->setAnchorPoint(ccp(0.0, 0.5));
		pathImg->setRotation(myPointVector.distance.getAngle());
		pathScale = 1;
		pathImg->setScaleX(pathScale);
		pathImg->setScaleY(0.5f);
		addChild(pathImg);
		pathImg->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		
//		CCLOG("addPath x : %d , y : %d, direction : %d, %d", myPointVector.origin.x, myPointVector.origin.y, myPointVector.distance.dx, myPointVector.distance.dy);
		
		setPosition(ccp((myPointVector.origin.x-1)*pixelSize+1, (myPointVector.origin.y-1)*pixelSize+1));
//		setPosition(myGD->getCommunicationNode("Jack_getJack")->getPosition());
		
		CCSprite* path_edge = CCSprite::create(("path_edge_" + line_color + ".png").c_str());
		path_edge->setScale(0.6f);
		addChild(path_edge);
		path_edge->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	}
};

enum pathBreakingState{
	pathBreakingStateFalse = 1,
	pathBreakingStateTrue
};

class PathBreakingParent : public CCNode
{
public:
	static PathBreakingParent* create(IntPoint t_start, vector<IntPoint>* t_linked_list)
	{
		PathBreakingParent* t_pbp = new PathBreakingParent();
		t_pbp->myInit(t_start, t_linked_list);
		t_pbp->autorelease();
		return t_pbp;
	}
	
	bool isActing(){	return is_acting;	}
	
private:
	vector<IntPoint>* plinked_list;
	CCSprite* pre_img;
	CCSprite* next_img;
	
	int pre_it;
	int next_it;
	
	bool is_acting;
	
	void tracing()
	{
		bool is_end = false;
		
		int loop_value = 1;
		if(myGD->getCommunicationBool("UI_isExchanged"))
			loop_value++;
		
		for(int i=0;!is_end && i<loop_value;i++)//2배속
		{
			if(pre_img)
			{
				pre_it--;
				if(pre_it > 0)
					pre_img->setPosition((*plinked_list)[pre_it].convertToCCP());
				else
				{
					pre_img->removeFromParent();
					pre_img = NULL;
				}
			}
			
			if(next_img)
			{
				next_it++;
				
				if(next_it < plinked_list->size()-1)
					next_img->setPosition((*plinked_list)[next_it].convertToCCP());
				else
				{
					getParent()->setTag(pathBreakingStateFalse);
					myGD->communication("Jack_startDieEffect", DieType::kDieType_missileToLine);
					unschedule(schedule_selector(PathBreakingParent::tracing));
					is_end = true;
					removeFromParent();
					return;
				}
			}
		}
	}
	
	void myInit(IntPoint t_start, vector<IntPoint>* t_linked_list)
	{
		plinked_list = t_linked_list;
		
		pre_img = NULL;
		next_img = NULL;
		
		bool is_found = false;
		int list_end = plinked_list->size();
		for(int i = 0;i<list_end && !is_found;i++)
		{
			IntPoint t_p = (*plinked_list)[i];
			if(t_p.x == t_start.x && t_p.y == t_start.y)
			{
				is_found = true;
				
				pre_it = next_it = i;
				
//				CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//				auto reader = new CCBReader(nodeLoader);
				if(i > 0)
				{
//					pre_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_pollution3.ccbi",this));
					pre_img = KS::loadCCBI<CCSprite*>(this, "fx_pollution3.ccbi").first;
					pre_img->setPosition(t_p.convertToCCP());
					addChild(pre_img);
					KS::setBlendFunc(pre_img, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
				}
				
				if(i < plinked_list->size()-1)
				{
//					next_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_pollution3.ccbi", this));
					next_img = KS::loadCCBI<CCSprite*>(this, "fx_pollution3.ccbi").first;
					next_img->setPosition(t_p.convertToCCP());
					addChild(next_img);
					KS::setBlendFunc(next_img, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
				}
//				reader->release();
				break;
			}
		}
		
		is_acting = false;
		if(is_found && (pre_img || next_img))
		{
			is_acting = true;
			schedule(schedule_selector(PathBreakingParent::tracing));
		}
	}
};

const int SHORTLINE_VALUE = 3;

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
		{
			newPathAdd(t_pv);
			linked_list.clear();
		}
		else if(myList.back()->myPointVector.distance.getAngle() != t_pv.distance.getAngle()) // differ last path
			newPathAdd(t_pv);
		else
			originalExpansion(t_pv.origin);
		
		IntPoint t_p = IntPoint(t_pv.origin.x, t_pv.origin.y);
		linked_list.push_back(t_p);
	}
	
	void checkBeforeNewline(IntPoint t_p)
	{
		if(myGD->mapState[t_p.x][t_p.y] == mapNewline)
			overlap_map[t_p.x][t_p.y]++;
	}
	
	bool checkRemoveNewline(IntPoint t_p)
	{
		if(overlap_map[t_p.x][t_p.y] > 0)
		{
			overlap_map[t_p.x][t_p.y]--;
			return false;
		}
		else
		{
			overlap_map[t_p.x][t_p.y] = 0;
			return true;
		}
	}
	
	void cleanPath()
	{
		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
			{
				overlap_map[i][j] = 0;
			}
		}
		
		myList.clear();
		
		PathBreakingParent* t_breaking = (PathBreakingParent*)getChildByTag(childTagInPathParentPathBreaking);
		if(t_breaking)
			t_breaking->removeFromParent();
		linked_list.clear();
		
		setTag(pathBreakingStateFalse);
		
		bool isRemoveTargetNull = false;
		
		while (!isRemoveTargetNull)
		{
			CCNode* t_child = getChildByTag(childTagInPathParentPathNode);
			if(t_child == NULL)
				isRemoveTargetNull = true;
			else
				removeChildByTag(childTagInPathParentPathNode, true);
		}
	}
	
	IntPoint pathBackTracking()
	{
		if(!myList.empty())
		{
			linked_list.pop_back();
			
			PathNode* b_node = myList.back();
			
			if(b_node->pathScale > 1) // reduce
			{
				b_node->changeScaleImg(-1);
				IntPointVector b_pv = b_node->myPointVector;
				IntPoint r_p = IntPoint(roundf(b_pv.origin.x + b_pv.distance.dx*b_node->pathScale), roundf(b_pv.origin.y + b_pv.distance.dy*b_node->pathScale));
				if(myGD->mapState[r_p.x][r_p.y] == mapNewline)
				{
					aloneNewlineRemove(r_p);
					if(checkRemoveNewline(r_p))
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
					if(checkRemoveNewline(r_p))
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
	vector<IntPoint> linked_list;
	int overlap_map[162][217];
	
	void addPathBreaking(IntPoint start)
	{
		if(getTag() == pathBreakingStateFalse)
		{
			setTag(pathBreakingStateTrue);
			PathBreakingParent* n_pbp = PathBreakingParent::create(start, &linked_list);
			n_pbp->setTag(childTagInPathParentPathBreaking);
			if(n_pbp->isActing())
				addChild(n_pbp, 2);
			else
			{
				setTag(pathBreakingStateFalse);
				myGD->communication("Jack_startDieEffect", DieType::kDieType_missileToLine);
			}
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
	
	void originalExpansion(IntPoint t_p)
	{
		myList.back()->changeScaleImg(1, t_p);
	}
	
	void newPathAdd(IntPointVector t_pv)
	{
		if(!myList.empty())
		{
			IntDirection t_direction = myList.back()->myPointVector.distance.getDirection();
			IntPoint before_point = myList.back()->myPointVector.origin;
			int before_scale;
			if(t_direction == directionLeft || t_direction == directionRight)
			{
				before_scale = abs(before_point.x - t_pv.origin.x);
			}
			else if(t_direction == directionUp || t_direction == directionDown)
			{
				before_scale = abs(before_point.y - t_pv.origin.y);
			}
			myList.back()->changeScaleDirect(before_scale);
		}
		
		PathNode* t_pn = PathNode::create(t_pv, path_color);
		t_pn->setTag(childTagInPathParentPathNode);
		addChild(t_pn, 1);
		
		myList.push_back(t_pn);
	}
	
	string path_color;
	
	void checkLastAddPath(IntPointVector t_pointvector)
	{
		if(myList.empty())
			return;
		PathNode* last_path_node = myList.back();
		if(last_path_node->myPointVector.distance.getAngle() == t_pointvector.distance.getAngle() && last_path_node->pathScale == 1 &&
		   (last_path_node->myPointVector.origin.x != t_pointvector.origin.x || last_path_node->myPointVector.origin.y != t_pointvector.origin.y))
		{
			if(myGD->mapState[t_pointvector.origin.x][t_pointvector.origin.y] != mapOldline)
			{
				t_pointvector.origin.x -= t_pointvector.distance.dx;
				t_pointvector.origin.y -= t_pointvector.distance.dy;
			}
			
			last_path_node->myPointVector = t_pointvector;
			last_path_node->setPosition(t_pointvector.origin.convertToCCP());
			last_path_node->changeScaleImg(-1);
		}
	}
	
	void lastPathRemove()
	{
		if(myList.empty())
			return;
		PathNode* last_path_node = myList.back();
		if(last_path_node->pathScale == 1)
		{
			myList.pop_back();
			last_path_node->removeFromParent();
		}
		else
			last_path_node->changeScaleImg(-1);
	}
	
	bool isShortLine()
	{
		int length = 0;
		for(auto i = myList.begin(); i != myList.end() ; i++)
		{
			length += (*i)->pathScale;
		}
		if(length > SHORTLINE_VALUE)
			return false;
		
		return true;
	}
	
	void myInit()
	{
		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
			{
				overlap_map[i][j] = 0;
			}
		}
		
		int path_color_code = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_lineColor_i, mySGD->getSelectedCharacterHistory().characterIndex.getV());
		if(path_color_code == 1)
			path_color = "life";
		else if(path_color_code == 2)
			path_color = "fire";
		else if(path_color_code == 3)
			path_color = "water";
		else if(path_color_code == 4)
			path_color = "wind";
		else if(path_color_code == 5)
			path_color = "lightning";
		else if(path_color_code == 6)
			path_color = "plasma";
		else
			path_color = "empty";
		
		myGD->V_Ipv["PM_addPath"] = std::bind(&PathManager::addPath, this, _1);
		myGD->V_V["PM_cleanPath"] = std::bind(&PathManager::cleanPath, this);
		myGD->V_Ip["PM_addPathBreaking"] = std::bind(&PathManager::addPathBreaking, this, _1);
		myGD->V_Ip["PM_checkBeforeNewline"] = std::bind(&PathManager::checkBeforeNewline, this, _1);
		myGD->B_Ip["PM_checkRemoveNewline"] = std::bind(&PathManager::checkRemoveNewline, this, _1);
		myGD->V_Ipv["PM_checkLastAddPath"] = std::bind(&PathManager::checkLastAddPath, this, _1);
		myGD->V_V["PM_lastPathRemove"] = std::bind(&PathManager::lastPathRemove, this);
		myGD->B_V["PM_isShortLine"] = std::bind(&PathManager::isShortLine, this);
		setTag(pathBreakingStateFalse);
	}
};

#endif
