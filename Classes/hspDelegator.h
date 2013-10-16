//
//  hspDelegator.h
//  hspConnector
//
//  Created by LitQoo on 13. 10. 10..
//
//

#ifndef __hspConnector__hspDelegator__
#define __hspConnector__hspDelegator__

#include <iostream>
#include "cocos2d.h"
#include "jsoncpp/json.h"

using namespace cocos2d;
using namespace std;
typedef void (CCObject::*hspSelType)(Json::Value);
#define hsp_selector(_SELECTOR) (hspSelType)(&_SELECTOR)

class hspDelegator
{
    
public:
	
	struct DeleSel
	{
		CCObject* target;
		hspSelType selector;
        Json::Value param;
        Json::Value callbackParam;
	};
    
    
public:
	void removeTarget(CCObject* _obj)
	{
        Json::Value value;
		for(auto iter = DeleSelTable.begin(); iter != DeleSelTable.end(); iter++)
		{
			if(iter->second.target == _obj)
			{
				DeleSelTable.erase(iter);
			}
		}
	}
	int add(CCObject* target, hspSelType selector,Json::Value param,Json::Value callbackParam)
	{
		key++;
		DeleSel temp;
		temp.target = target;
		temp.selector = selector;
        temp.param = param;
        temp.callbackParam = callbackParam;
		DeleSelTable[key] = temp;
		return key;
	}
	void remove(int _key)
	{
		if(DeleSelTable.find(_key) != DeleSelTable.end())
			DeleSelTable.erase(_key);
	}
	DeleSel load(int _key)
	{
		if(DeleSelTable.find(_key) != DeleSelTable.end())
		{
			return DeleSelTable[_key];
		}
		else
		{
			DeleSel _t;
			_t.target = 0;
			_t.selector = 0;
			return _t;
		}
	}
    
public:
	static hspDelegator* get()
	{
		static hspDelegator* _ins = 0;
		if(_ins == 0)
			_ins = new hspDelegator();
		return _ins;
	}
    
private:
	hspDelegator()
	{
		key = 0;
        buff="";
	}
	~hspDelegator(){}
private:
	map<int, DeleSel> DeleSelTable;
	int key;
    
public:
    string buff;
};
#endif /* defined(__hspConnector__hspDelegator__) */
