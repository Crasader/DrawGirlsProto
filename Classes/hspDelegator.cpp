// hspDelegator.cpp
//

#include "hspDelegator.h"
#define LZZ_INLINE inline
void hspDelegator::removeTarget (CCObject * _obj)
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
int hspDelegator::add (CCObject * target, hspSelType selector, Json::Value param, Json::Value callbackParam)
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
void hspDelegator::remove (int _key)
        {
		if(DeleSelTable.find(_key) != DeleSelTable.end())
			DeleSelTable.erase(_key);
	}
hspDelegator::DeleSel hspDelegator::load (int _key)
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
hspDelegator * hspDelegator::get ()
        {
		static hspDelegator* _ins = 0;
		if(_ins == 0)
			_ins = new hspDelegator();
		return _ins;
	}
hspDelegator::hspDelegator ()
        {
		key = 0;
        buff="";
	}
hspDelegator::~ hspDelegator ()
                       {}
#undef LZZ_INLINE
