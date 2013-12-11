//
//  GachaPurchase.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 5..
//
//

#ifndef __DGproto__GachaPurchase__
#define __DGproto__GachaPurchase__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum GachaPurchaseStartMode{
	kGachaPurchaseStartMode_select = 1,
	kGachaPurchaseStartMode_reward
};

class GachaPurchase : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
	
	static GachaPurchase* create(GachaPurchaseStartMode start_mode = kGachaPurchaseStartMode_select,
															 std::function<void(void)> t_finish = nullptr);
//    CREATE_FUNC(GachaPurchase);
	
	virtual void onEnter();
	
	virtual bool init(GachaPurchaseStartMode start_mode, std::function<void(void)> t_finish);
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	void setOutAllObjectAction(CCObject* t_out, SEL_CallFunc d_out);
	void setInAllObjectAction(CCObject* t_in, SEL_CallFunc d_in);
	
private:
	std::function<void(void)> finish_function;
	GachaPurchaseStartMode recent_mode;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCObject* target_out;
	SEL_CallFunc delegate_out;
	
	CCObject* target_in;
	SEL_CallFunc delegate_in;
	
	CCSprite* gray;
	CCSprite* main_case;
	
	CCSprite* left_curtain;
	CCSprite* right_curtain;
	
	CCSprite* gacha_touch;
	int gacha_cnt;
	
	bool is_touch_on;
	bool is_menu_enable;
	
	void popupClose();
	
	void startGacha();
	
	void gachaListPopup();
	void endGachaListPopup();
	int recent_gacha;
	void changeGacha();
	int visibling_cnt;
	void visibling();
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	CCPoint getContentPosition(int t_tag);
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};

#endif /* defined(__DGproto__GachaPurchase__) */
