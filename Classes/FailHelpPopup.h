// FailHelpPopup.h
//

#ifndef LZZ_FailHelpPopup_h
#define LZZ_FailHelpPopup_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum FailHelpPopupZorder
{
  kFHP_Z_gray = 1,
  kFHP_Z_back,
  kFHP_Z_content
};
enum FailHelpPopupMenuTag
{
  kFHP_MT_close = 1,
  kFHP_MT_help,
  kFHP_MT_help_close
};
class FailHelpPopup : public CCLayer
{
public:
  static FailHelpPopup * create (CCObject * t_close, SEL_CallFunc d_close);
private:
  bool is_menu_enable;
  int touched_number;
  CCMenu * close_menu;
  CCMenu * help_menu;
  CCMenu * help_close_menu;
  CCObject * target_close;
  SEL_CallFunc delegate_close;
  CCSprite * back;
  bool is_list;
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
