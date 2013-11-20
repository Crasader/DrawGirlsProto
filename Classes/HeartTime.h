// HeartTime.h
//

#ifndef LZZ_HeartTime_h
#define LZZ_HeartTime_h
#include "cocos2d.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include <chrono>

USING_NS_CC;
using namespace std;

#define CHARGE_TIME 300
#define HEART_DISTANCE 20
#define LZZ_INLINE inline
class HeartTime : public CCNode
{
public:
  static HeartTime * create ();
  bool startGame ();
  void backHeart ();
  bool isStartable ();
private:
  vector <CCSprite*> heart_list;
  CCLabelTTF * state_label;
  bool is_checking;
  void checkingTime ();
  void myInit ();
};
#undef LZZ_INLINE
#endif
