// hspDelegator.h
//

#ifndef LZZ_hspDelegator_h
#define LZZ_hspDelegator_h
#include <iostream>
#include "cocos2d.h"
#include "jsoncpp/json.h"

using namespace cocos2d;
using namespace std;
typedef void (CCObject::*hspSelType)(Json::Value);
#define hsp_selector(_SELECTOR) (hspSelType)(&_SELECTOR)
#define LZZ_INLINE inline
class hspDelegator
{
public:
  struct DeleSel
  {
    CCObject * target;
    hspSelType selector;
    Json::Value param;
    Json::Value callbackParam;
  };
public:
  void removeTarget (CCObject * _obj);
  int add (CCObject * target, hspSelType selector, Json::Value param, Json::Value callbackParam);
  void remove (int _key);
  DeleSel load (int _key);
public:
  static hspDelegator * get ();
private:
  hspDelegator ();
  ~ hspDelegator ();
private:
  map <int, DeleSel> DeleSelTable;
  int key;
public:
  string buff;
};
#undef LZZ_INLINE
#endif
