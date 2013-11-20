// TicketNeedPopup.h
//

#ifndef LZZ_TicketNeedPopup_h
#define LZZ_TicketNeedPopup_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum TicketNeedPopupZorder
{
  kTNP_Z_gray = 1,
  kTNP_Z_back,
  kTNP_Z_content
};
enum TicketNeedPopupMenuTag
{
  kTNP_MT_close = 1,
  kTNP_MT_rightnow,
  kTNP_MT_rightNow_back,
  kTNP_MT_rightNow_buy
};
enum TicketNeedPopupState
{
  kTNP_State_default = 0,
  kTNP_State_rightNow
};
class TicketNeedPopup : public CCLayer
{
public:
  static TicketNeedPopup * create (CCObject * t_close, SEL_CallFunc d_close, CCObject * t_pass, SEL_CallFunc d_pass);
private:
  TicketNeedPopupState my_state;
  bool is_menu_enable;
  int touched_number;
  CCMenu * close_menu;
  CCMenu * rightnow_menu;
  CCMenu * rightNow_back_menu;
  CCMenu * rightNow_buy_menu;
  CCObject * target_close;
  SEL_CallFunc delegate_close;
  CCObject * target_pass;
  SEL_CallFunc delegate_pass;
  CCSprite * back_img;
  void myInit (CCObject * t_close, SEL_CallFunc d_close, CCObject * t_pass, SEL_CallFunc d_pass);
  void setBack ();
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
