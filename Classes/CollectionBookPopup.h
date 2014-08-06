//
//  CollectionBookPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#ifndef __DGproto__CollectionBookPopup__
#define __DGproto__CollectionBookPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <deque>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CollectionAnimation : public CCSprite
{
public:
	static CollectionAnimation* create(int t_card_number);
	
	void setAnimation(int t_card_number);
	
	void startAnimation();
	
	int stage_number;
	int level_number;
	CCSize ani_size;
	
private:
	int loop_length;
	deque<int> animation_frame;
	int ing_animation_frame;
	
	void startStageAnimation();
	
	void frameAnimation();
};

class CollectionBookPopup : public CCLayer, public CCEditBoxDelegate// public CCTextFieldDelegate
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CollectionBookPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
	
private:
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	CCNode* main_case;
	
	bool is_menu_enable;
	
	int recent_card_number;
	bool is_enable_pageturn;
	
	int page_cnt;
	
	CCSprite* recent_left_img;
	CCSprite* recent_right_img;
	CCSprite* after_left_img;
	CCSprite* after_right_img;
	
	CCSprite* covered_left_img;
	CCSprite* covered_right_img;
	
	CCSprite* animation_img;
	SEL_CallFunc end_animation_delegate;
	
	CCPoint begin_point;
	
//	CCEditBox* input_text; // last
//	CCTextFieldTTF* input_text;
//    bool was_open_text;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
//	void endCloseTextInput();
	
	CCPoint getContentPosition(int t_tag);
	
//	virtual bool onTextFieldInsertText(CCTextFieldTTF* sender, const char* text, int nLen);
//	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
//	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
	
	void setRightPage(CCNode* target, int card_number);
	void setLeftPage(CCNode* target, int card_number);
	
	void startNextPageFull();
	void startPrePageFull();
	void ingPageFull();
	
	int touch_end_direction;
	int touch_direction;
	bool is_touch_enable;
	float animation_angle;
	
	void startNextPage();
	void startPrePage();
	void ingPage();
	void endNextPage();
	void endPrePage();
	void endPage();
	
	void startNextFullSelectedPage();
	void startNextSelectedPage();
	void endNextSelectedPage();
	
	void startPreFullSelectedPage();
	void startPreSelectedPage();
	void endPreSelectedPage();
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__CollectionBookPopup__) */
