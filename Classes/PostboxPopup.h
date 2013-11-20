// PostboxPopup.h
//

#ifndef LZZ_PostboxPopup_h
#define LZZ_PostboxPopup_h
#include "cocos2d.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum PostboxPopupZorder
{
  kPP_Z_gray = 1,
  kPP_Z_back,
  kPP_Z_content
};
enum PostboxPopupMenuTag
{
  kPP_MT_close = 1,
  kPP_MT_challenge_close,
  kPP_MT_help_close,
  kPP_MT_ticketrequest_close,
  kPP_MT_noti_close,
  kPP_MT_receive_base = 100
};
enum PostboxPopupState
{
  kPP_State_postbox = 1,
  kPP_State_challenge,
  kPP_State_help,
  kPP_State_ticketrequest,
  kPP_State_noti
};
enum PostboxReceived
{
  kPR_challenge = 1,
  kPR_heart,
  kPR_help,
  kPR_noti,
  kPR_ticketrequest
};
class PostboxPopup : public CCLayer
{
public:
  static PostboxPopup * create (CCObject * t_close, SEL_CallFunc d_close);
private:
  bool is_menu_enable;
  int touched_number;
  CCMenu * close_menu;
  CCMenu * challenge_close_menu;
  CCMenu * help_close_menu;
  CCMenu * ticketrequest_close_menu;
  CCMenu * noti_close_menu;
  CCObject * target_close;
  SEL_CallFunc delegate_close;
  PostboxPopupState my_state;
  CCSprite * back;
  std::vector <PostboxReceived> received_list;
  void myInit (CCObject * t_close, SEL_CallFunc d_close);
  void createReceivedList ();
  CCPoint getContentPosition (int t_tag);
  void backPostbox ();
  void menuAction (CCObject * sender);
  virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
  virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
  virtual void registerWithTouchDispatcher ();
};
#undef LZZ_INLINE
#endif
