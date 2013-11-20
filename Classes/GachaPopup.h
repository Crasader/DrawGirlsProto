// GachaPopup.h
//

#ifndef LZZ_GachaPopup_h
#define LZZ_GachaPopup_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum GachaPopupZorder
{
  kGP_Z_gray = 1,
  kGP_Z_back,
  kGP_Z_content
};
enum GachaPopupMenuTag
{
  kGP_MT_close = 1,
  kGP_MT_action,
  kGP_MT_result_close
};
class GachaPopup : public CCLayer
{
public:
  static GachaPopup * create (CCObject * t_close, SEL_CallFunc d_close);
private:
  bool is_menu_enable;
  int touched_number;
  CCMenu * close_menu;
  CCMenu * button_menu;
  CCMenu * result_close_menu;
  CCObject * target_close;
  SEL_CallFunc delegate_close;
  bool is_roulette;
  CCSprite * back;
  void myInit (CCObject * t_close, SEL_CallFunc d_close);
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
