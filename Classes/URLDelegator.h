#pragma once



#include "cocos2d.h"
//#include "des.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include "jni.h"
#endif
using namespace cocos2d;
using namespace std;
typedef void (CCObject::*RecvSelType)(string);
#define recv_selector(_SELECTOR) (RecvSelType)(&_SELECTOR)
class URLDelegator
{
	
public:
	
	struct DeleSel
	{
		CCObject* target;
		RecvSelType selector;
	};
	
	
public:
	void removeTarget(CCObject* _obj)
	{
		// DeleTable<int, DeleSel>
		for(auto iter = DeleSelTable.begin(); iter != DeleSelTable.end();)
		{
			if(iter->second.target == _obj)
			{
				DeleSelTable.erase(iter++);
			}
			else
				++iter;
		}
	}
	int addDeleSel(CCObject* target, RecvSelType selector)
	{
		key++;
		DeleSel temp;
		temp.target = target;
		temp.selector = selector;
		DeleSelTable[key] = temp;
		return key;
	}
	void removeDeleSel(int _key)
	{
		if(DeleSelTable.find(_key) != DeleSelTable.end())
			DeleSelTable.erase(_key);
	}
	DeleSel getDeleSel(int _key)
	{
		// find
		if(DeleSelTable.find(_key) != DeleSelTable.end())
		{
			return DeleSelTable[_key];
		}
		else
		{
			//CCLOG("not found!!!");
			DeleSel _t;
			_t.target = 0;
			_t.selector = 0;
			return _t;
		}
		
	}
	
public:
	static URLDelegator* getInstance()
	{
		static URLDelegator* _ins = 0;
		if(_ins == 0)
			_ins = new URLDelegator();
		return _ins;
	}
	
private:
	URLDelegator()
	{
		key = 0;
	}
	~URLDelegator(){}
private:
	map<int, DeleSel> DeleSelTable;
	int key;
};