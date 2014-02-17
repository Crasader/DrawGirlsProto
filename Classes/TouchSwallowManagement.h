//
//  TouchSwallowManagement.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 15..
//
//

#ifndef __DGproto__TouchSwallowManagement__
#define __DGproto__TouchSwallowManagement__

#include "cocos2d.h"
#include <algorithm>

USING_NS_CC;
using namespace std;

class TouchSwallowManagement
{
public:
	static TouchSwallowManagement* sharedInstance()
	{
		static TouchSwallowManagement* t_tsm = NULL;
		if(!t_tsm)
		{
			t_tsm = new TouchSwallowManagement();
			t_tsm->myInit();
		}
		return t_tsm;
	}
	
	void addSwallowLayer(CCLayer* t_layer)
	{
		swallow_list.push_back(t_layer);
	}
	
	void removeSwallowLayer(CCLayer* t_layer)
	{
		vector<CCLayer*>::iterator iter = find(swallow_list.begin(), swallow_list.end(), t_layer);
		if(iter != swallow_list.end())
		{
			swallow_list.erase(iter);
		}
	}
	
	void resetSwallowList()
	{
		swallow_list.clear();
	}
	
	void offAllSwallow()
	{
		for(int i=0;i<swallow_list.size();i++)
		{
			CCLayer* t_swallow_target = swallow_list[i];
			t_swallow_target->setTouchEnabled(false);
		}
	}
	
	void onAllSwallow()
	{
		for(int i=0;i<swallow_list.size();i++)
		{
			CCLayer* t_swallow_target = swallow_list[i];
			t_swallow_target->setTouchEnabled(true);
		}
	}
	
private:
	
	vector<CCLayer*> swallow_list;
	
	void myInit()
	{
		resetSwallowList();
	}
};

#endif /* defined(__DGproto__TouchSwallowManagement__) */
