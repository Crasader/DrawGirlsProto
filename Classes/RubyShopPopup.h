// RubyShopPopup.h
//

#ifndef LZZ_RubyShopPopup_h
#define LZZ_RubyShopPopup_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum RubyShopPopupZorder
{
  kRSP_Z_gray = 1,
  kRSP_Z_back,
  kRSP_Z_content
};
enum RubyShopPopupMenuTag
{
  kRSP_MT_close = 1
};
class RubyShopPopup : public CCLayer
{
public:
  static RubyShopPopup * create (CCObject * t_close, SEL_CallFunc d_close);
private:
  bool is_menu_enable;
  int touched_number;
  CCMenu * close_menu;
  CCObject * target_close;
  SEL_CallFunc delegate_close;
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
