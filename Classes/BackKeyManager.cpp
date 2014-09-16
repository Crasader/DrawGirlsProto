//
//  BackKeyManager.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 9. 15..
//
//

#include "BackKeyManager.h"
#include "BackKeyBase.h"

void BackKeyManager::addBase(BackKeyBase* t_base)
{
	for(auto iter = base_list.begin();iter!=base_list.end();iter++)
	{
		(*iter)->over_popup_count++;
	}
	
	base_list.push_back(t_base);
}
void BackKeyManager::removeBase(BackKeyBase* t_base)
{
	for(auto iter = base_list.begin();iter!=base_list.end();)
	{
		if(t_base == *iter)
		{
			iter = base_list.erase(iter);
		}
		else
		{
			if((*iter)->over_popup_count > 0)
				(*iter)->over_popup_count--;
			++iter;
		}
	}
}

void BackKeyManager::myInit()
{
	base_list.clear();
}