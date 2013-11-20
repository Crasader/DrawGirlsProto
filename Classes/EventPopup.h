// EventPopup.h
//

#ifndef LZZ_EventPopup_h
#define LZZ_EventPopup_h
#include "cocos2d.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "StageInfoDown.h"
#include "StageSettingPopup.h"
#include "PuzzleMapScene.h"

USING_NS_CC;
#define LZZ_INLINE inline
enum EventPopupZorder
{
  kEP_Z_gray = 1,
  kEP_Z_back,
  kEP_Z_selected,
  kEP_Z_content,
  kEP_Z_popup
};
enum EventPopupMenuTag
{
  kEP_MT_close = 1,
  kEP_MT_start,
  kEP_MT_eventStageBase = 10000
};
class EventPopup : public CCLayer
{
public:
  static EventPopup * create ();
  void setHideFinalAction (CCObject * t_final, SEL_CallFunc d_final);
private:
  bool is_menu_enable;
  CCObject * target_final;
  SEL_CallFunc delegate_final;
  int touched_number;
  CCMenu * close_menu;
  CCMenu * start_menu;
  CCSprite * gray;
  CCSprite * main_case;
  CCSprite * selected_event_button;
  int selected_event_code;
  int el_length;
  void myInit ();
  virtual void onEnter ();
  void showPopup ();
  void endShowPopup ();
  void hidePopup ();
  void endHidePopup ();
  CCPoint getContentPosition (int t_tag);
  void startCancel ();
  void menuAction (CCObject * sender);
  void successStageInfoDown ();
};
#undef LZZ_INLINE
#endif
