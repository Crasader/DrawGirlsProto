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
	}
};

enum pathBreakingState{
	pathBreakingStateFalse = 1,
	pathBreakingStateTrue
};

class PathBreakingParent : public CCNode
{
public:
	static PathBreakingParent* create(IntPoint t_start, list<IntPoint>* t_linked_list)
	{
		PathBreakingParent* t_pbp = new PathBreakingParent();
		t_pbp->myInit(t_start, t_linked_list);
		t_pbp->autorelease();
		return t_pbp;
	}
	
	bool isActing(){	return is_acting;	}
	
private:
	list<IntPoint>* plinked_list;
	CCSprite* pre_img;
	CCSprite* next_img;
	
	list<IntPoint>::iterator pre_it;
	list<IntPoint>::iterator next_it;
	
	bool is_acting;
	
	void tracing()
	{
		if(pre_img)
		{
			CCPoint sub_point = ccpSub(myGD->getJackPoint().convertToCCP(), pre_img->getPosition());
			float sub_value = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
			if(sub_value < 4.f)
			{
				myGD->communication("Jack_startDieEffect");
				unschedule(schedule_selector(PathBreakingParent::tracing));
				getParent()->setTag(pathBreakingStateFalse);
				removeFromParent();
				return;
			}
			else
			{
				pre_it--;
				if(pre_it != plinked_list->begin())
					pre_img->setPosition((*pre_it).convertToCCP());
				else
				{
					pre_img->removeFromParent();
					pre_img = NULL;
				}
			}
		}
		
		if(next_img)
		{
			CCPoint sub_point = ccpSub(myGD->getJackPoint().convertToCCP(), next_img->getPosition());
			float sub_value = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
			if(sub_value < 4.f)
			{
				myGD->communication("Jack_startDieEffect");
				unschedule(schedule_selector(PathBreakingParent::tracing));
				getParent()->setTag(pathBreakingStateFalse);
				removeFromParent();
				return;
			}
			else
			{
				next_it++;
				if(next_it != plinked_list->end())
					next_img->setPosition((*next_it).convertToCCP());
				else
				{
					next_img->removeFromParent();
					next_img = NULL;
				}
			}
		}
	}
	
	void myInit(IntPoint t_start, list<IntPoint>* t_linked_list)
	{
		plinked_list = t_linked_list;
		
		pre_img = NULL;
		next_img = NULL;
		
		bool is_found = false;
		for(list<IntPoint>::iterator i = plinked_list->begin();i!=plinked_list->end() && !is_found;i++)
		{
			IntPoint t_p = *i;
			if(t_p.x == t_start.x && t_p.y == t_start.y)
			{
				is_found = true;
				
				pre_it = next_it = i;
				
				CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
				auto reader = new CCBReader(nodeLoader);
				if(i != plinked_list->begin())
				{
					pre_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_pollution3.ccbi",this));
					pre_img->setPosition(t_p.convertToCCP());
					addChild(pre_img);
				}
				
				if(++i != plinked_list->end())
				{
					next_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_pollution3.ccbi", this));
					next_img->setPosition(t_p.convertToCCP());
					addChild(next_img);
				}
				break;
			}
		}
		
		if(is_found && (pre_img || next_img))
		{
			is_acting = true;
			schedule(schedule_selector(PathBreakingParent::tracing));
		}
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
		{
			newPathAdd(t_pv);
			linked_list.clear();
		}
		else if(myList.back()->myPointVector.distance.getAngle() != t_pv.distance.getAngle()) // differ last path
			newPathAdd(t_pv);
		else
			originalExpansion();
		
		IntPoint t_p = IntPoint(t_pv.origin.x, t_pv.origin.y);
		linked_list.push_back(t_p);
	}
	
	void cleanPath()
	{
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
	list<IntPoint> linked_list;
	
	void addPathBreaking(IntPoint start)
	{
		if(getTag() == pathBreakingStateFalse)
		{
			setTag(pathBreakingStateTrue);
			PathBreakingParent* n_pbp = PathBreakingParent::create(start, &linked_list);
			n_pbp->setTag(childTagInPathParentPathBreaking);
			if(n_pbp->isActing())
				addChild(n_pbp);
			else
				setTag(pathBreakingStateFalse);
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
		myGD->V_Ipv["PM_addPath"] = std::bind(&PathManager::addPath, this, _1);
		myGD->V_V["PM_cleanPath"] = std::bind(&PathManager::cleanPath, this);
		myGD->V_Ip["PM_addPathBreaking"] = std::bind(&PathManager::addPathBreaking, this, _1);
		setTag(pathBreakingStateFalse);
	}
};

#endif
