//
//  InAppDelegator.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 12. 11..
//
//

#ifndef __BasketWorldCup2__InAppDelegator__
#define __BasketWorldCup2__InAppDelegator__


#include "cocos2d.h"
//#include "des.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include "jni.h"
#endif
using namespace cocos2d;
using namespace std;
typedef void (CCObject::*RecvInAppSelType)(string, int); // itemID, result_code
#define recvInApp_selector(_SELECTOR) (RecvInAppSelType)(&_SELECTOR)
class InAppDelegator
{	
public:
	struct DeleSel
	{
		CCObject* target;
		RecvInAppSelType selector;
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
	int addDeleSel(CCObject* target, RecvInAppSelType selector)
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
	static InAppDelegator* getInstance()
	{
		static InAppDelegator* _ins = 0;
		if(_ins == 0)
			_ins = new InAppDelegator();
		return _ins;
	}
	
private:
	InAppDelegator()
	{
		key = 0;
	}
	~InAppDelegator(){}
private:
	map<int, DeleSel> DeleSelTable;
	int key;
};

template<typename T>
class KSDelegator
{
	
public:
	
	struct DeleSel
	{
		CCObject* target;
		T selector;
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
	int addDeleSel(CCObject* target, T selector)
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
			DeleSel _t;
			_t.target = 0;
			_t.selector = 0;
			return _t;
		}
		
	}
	
public:
	static KSDelegator* getInstance()
	{
		static KSDelegator* _ins = 0;
		if(_ins == 0)
			_ins = new KSDelegator();
		return _ins;
	}
	
private:
	KSDelegator()
	{
		key = 0;
	}
	virtual ~KSDelegator(){}
private:
	map<int, DeleSel> DeleSelTable;
	int key;
};

#endif /* defined(__BasketWorldCup2__InAppDelegator__) */
