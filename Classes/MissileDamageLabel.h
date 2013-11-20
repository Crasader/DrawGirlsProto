// MissileDamageLabel.h
//

#ifndef LZZ_MissileDamageLabel_h
#define LZZ_MissileDamageLabel_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
class MissileDamageLabel : public CCNode
{
public:
  static MissileDamageLabel * create (int t_damage);
  void startMyAction ();
private:
  CCSpriteBatchNode * m_batch_node;
  std::vector <CCSprite*> digit_imgs;
  void lastAnimation ();
  void visibleOn (CCObject * t_target);
  void myInit (int t_damage);
  CCRect getNumberRect (int t_number);
};
#undef LZZ_INLINE
#endif
