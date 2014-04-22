// OnePercentGacha.h
//

#ifndef LZZ_OnePercentGacha_h
#define LZZ_OnePercentGacha_h
#include "cocos2d.h"
#include "cocos-ext.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "CountingBMLabel.h"
#include "LogData.h"

USING_NS_CC;
USING_NS_CC_EXT;
#define LZZ_INLINE inline
enum OnePercentGachaZorder
{
	kOnePercentGacha_Z_gray = 1,
	kOnePercentGacha_Z_back,
	kOnePercentGacha_Z_content
};
enum OnePercentGachaMenuTag
{
	kOnePercentGacha_MT_cancel = 1,
	kOnePercentGacha_MT_ok,
	kOnePercentGacha_MT_stop
};
class CommonButton;
class OnePercentGacha : public CCLayer
{
public:
	static OnePercentGacha * create (CCObject * t_cancel, SEL_CallFunc d_cancel, CCObject * t_ok, SEL_CallFuncF d_ok, float t_recent_percent);
private:
	bool is_menu_enable;
	int touched_number;
	CCNode* m_container;
	CCSprite* gray;
	CommonButton * cancel_menu;
//	CommonButton * ok_menu;
	CCControlButton* gacha_button;
	CCControlButton* stop_button;
	CCObject * target_cancel;
	SEL_CallFunc delegate_cancel;
	CCObject * target_ok;
	SEL_CallFuncF delegate_ok;
	CCScale9Sprite* back_img;
	float recent_percent;
	void myInit (CCObject * t_cancel, SEL_CallFunc d_cancel, CCObject * t_ok, SEL_CallFuncF d_ok, float t_recent_percent);
	void startShow();
	void endShow();
	void gachaOn ();
	void repeatAction ();
	void gachaAction(CCObject* sender, CCControlEvent t_event);
	void gachaStopAction(CCObject* sender, CCControlEvent t_event);
	void menuAction (CCObject * sender);
	void endAction ();
	void changePercentLabel ();
	CountingBMLabel * percent_label;
	CCProgressTimer * progress_img;
	CCSprite * arrow_img;
	void setBack ();
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};
#undef LZZ_INLINE
#endif
