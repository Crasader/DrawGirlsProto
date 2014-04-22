// StartMapGacha.h
//

#ifndef LZZ_StartMapGacha_h
#define LZZ_StartMapGacha_h
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
#define LZZ_INLINE inline
enum StartMapGachaZorder
{
  kSMG_Z_back = 1,
  kSMG_Z_content
};
enum StartMapGachaMenuTag
{
  kSMG_MT_gacha = 1,
  kSMG_MT_cancel
};
class CommonButton;
class StartMapGacha : public CCLayer
{
public:
  static StartMapGacha * create (CCObject * t_gacha, SEL_CallFunc d_gacha);
private:
  bool is_menu_enable;
	CCControlButton* gacha_button;
//  CCMenu * gacha_menu;
  CommonButton * cancel_menu;
  CCObject * target_gacha;
  SEL_CallFunc delegate_gacha;
	CCScale9Sprite* back;
//  CCSprite * back;
  void myInit (CCObject * t_gacha, SEL_CallFunc d_gacha);
  CCPoint getContentPosition (int t_tag);
  void menuAction (CCObject * sender);
	void gachaAction(CCObject* sender, CCControlEvent t_event);
};
#undef LZZ_INLINE
#endif
