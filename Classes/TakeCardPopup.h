// TakeCardPopup.h
//

#ifndef LZZ_TakeCardPopup_h
#define LZZ_TakeCardPopup_h
#include "cocos2d.h"


USING_NS_CC;
#define LZZ_INLINE inline
enum TakeCardPopupZorder
{
	kTakeCardPopup_Z_gray = 1,
	kTakeCardPopup_Z_img,
	kTakeCardPopup_Z_particle
};
class TakeCardPopup : public CCLayer
{
public:
	static TakeCardPopup * create (int t_stage, int t_grade, CCObject* t_end_call, SEL_CallFunc d_end_call);
private:
	bool is_touch_enable;
	CCParticleSystemQuad * star_particle;
	CCObject* target_end_call;
	SEL_CallFunc delegate_end_call;
	CCSprite * gray;
	CCSprite * take_label;
	CCSprite * take_card;
	CCSprite * card_back;
	CCLabelTTF* reward_gold_label;
	void endCall();
	void myInit (int t_stage, int t_grade, CCObject* t_end_call, SEL_CallFunc d_end_call);
	void openingAction ();
	void changeBack ();
	void changeTakeCard ();
	void touchOn ();
	void fadeGrayTitle ();
	void closingAction ();
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};
#undef LZZ_INLINE
#endif
