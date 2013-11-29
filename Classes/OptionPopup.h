//
//  OptionPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#ifndef __DGproto__OptionPopup__
#define __DGproto__OptionPopup__

#include "cocos2d.h"
#include "GameData.h"
#include "DataStorageHub.h"

USING_NS_CC;
using namespace std;

class OptionPopup : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(OptionPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* main_case;
	CCSprite* gray;
	
	bool is_menu_enable;
	
	CCSprite* bgm_img;
	CCSprite* effect_img;
	
	CCMenu* bgm_menu;
	CCMenu* effect_menu;
	
	CCMenuItem* joystick_right_item;
	CCMenuItem* joystick_left_item;
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	CCPoint getContentPosition(int t_tag);
	
	void resetBgmMenu();
	void resetEffectMenu();
	
	void cheatAction(CCObject* sender);
	
	void menuAction(CCObject* sender);
	void popupClose();
};

#endif /* defined(__DGproto__OptionPopup__) */
