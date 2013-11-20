// LogoutPopup.h
//

#ifndef LZZ_LogoutPopup_h
#define LZZ_LogoutPopup_h
#include "cocos2d.h"
#include "hspConnector.h"
#include "DataStorageHub.h"
#include "TitleScene.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum LogoutPopupZorder
{
  kLP_Z_gray = 1,
  kLP_Z_back,
  kLP_Z_content
};
enum LogoutPopupMenuTag
{
  kLP_MT_close = 1,
  kLP_MT_logout
};
class LogoutPopup : public CCLayer
{
public:
  static LogoutPopup * create (CCObject * t_close, SEL_CallFunc d_close);
private:
  bool is_menu_enable;
  int touched_number;
  CCMenu * close_menu;
  CCMenu * logout_menu;
  CCObject * target_close;
  SEL_CallFunc delegate_close;
  void myInit (CCObject * t_close, SEL_CallFunc d_close);
  CCPoint getContentPosition (int t_tag);
  void menuAction (CCObject * sender);
  void resultLogoutAction (Json::Value result_data);
  virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
  virtual void registerWithTouchDispatcher ();
};
#undef LZZ_INLINE
#endif
