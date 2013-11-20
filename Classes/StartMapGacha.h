// StartMapGacha.h
//

#ifndef LZZ_StartMapGacha_h
#define LZZ_StartMapGacha_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum StartMapGachaZorder
{
  kSMG_Z_back = 1,
  kSMG_Z_content
};
enum StartMapGachaMenuTag
{
  kSMG_MT_gacha = 1,
  kSMG_MT_cancel
};
class StartMapGacha : public CCLayer
{
public:
  static StartMapGacha * create (CCObject * t_gacha, SEL_CallFunc d_gacha);
private:
  bool is_menu_enable;
  int touched_number;
  CCMenu * gacha_menu;
  CCMenu * cancel_menu;
  CCObject * target_gacha;
  SEL_CallFunc delegate_gacha;
  CCSprite * back;
  void myInit (CCObject * t_gacha, SEL_CallFunc d_gacha);
  CCPoint getContentPosition (int t_tag);
  void menuAction (CCObject * sender);
  virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
  virtual void registerWithTouchDispatcher ();
};
#undef LZZ_INLINE
#endif
