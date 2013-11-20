// WithdrawPopup.h
//

#ifndef LZZ_WithdrawPopup_h
#define LZZ_WithdrawPopup_h
#include "cocos2d.h"
USING_NS_CC;
#define LZZ_INLINE inline
enum WithdrawPopupZorder
{
  kWP_Z_gray = 1,
  kWP_Z_back,
  kWP_Z_content
};
enum WithdrawPopupMenuTag
{
  kWP_MT_close = 1
};
class WithdrawPopup : public CCLayer
{
public:
  static WithdrawPopup * create (CCObject * t_close, SEL_CallFunc d_close);
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
