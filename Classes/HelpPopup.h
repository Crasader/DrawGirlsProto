// HelpPopup.h
//

#ifndef LZZ_HelpPopup_h
#define LZZ_HelpPopup_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum HelpPopupZorder
{
  kHP_Z_gray = 1,
  kHP_Z_back,
  kHP_Z_content
};
enum HelpPopupMenuTag
{
  kHP_MT_close = 1
};
class HelpPopup : public CCLayer
{
public:
  static HelpPopup * create (CCObject * t_close, SEL_CallFunc d_close);
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
