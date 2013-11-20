// NotiPopup.h
//

#ifndef LZZ_NotiPopup_h
#define LZZ_NotiPopup_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum NotiPopupZorder
{
  kNP_Z_gray = 1,
  kNP_Z_back,
  kNP_Z_content
};
enum NotiPopupMenuTag
{
  kNP_MT_close = 1,
  kNP_MT_content_close = 2,
  kNP_MT_itemBase = 100
};
class NotiPopup : public CCLayer
{
public:
  static NotiPopup * create (CCObject * t_close, SEL_CallFunc d_close);
private:
  bool is_menu_enable;
  int touched_number;
  CCMenu * close_menu;
  CCMenu * button_menu;
  CCMenu * content_close_menu;
  CCObject * target_close;
  SEL_CallFunc delegate_close;
  bool is_list;
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
