// MobHpGraph.h
//

#ifndef LZZ_MobHpGraph_h
#define LZZ_MobHpGraph_h
#include "cocos2d.h"
#include "KSCumberBase.h"

USING_NS_CC;
#define LZZ_INLINE inline
class MobHpGraph : public CCNode
{
public:
  static MobHpGraph * create (CCObject * t_target);
  CCObject * getTargetNode ();
  void setGameover ();
private:
  KSCumberBase * target_node;
  float max_life;
  float last_life;
  CCProgressTimer * hp_progress;
  CCSprite * t_case;
  void startGraph ();
  void graphing ();
  void realHidingAction ();
  void hidingAction ();
  void myInit (CCObject * t_target);
};
#undef LZZ_INLINE
#endif
