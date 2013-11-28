//
//  CollectionBook.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 20..
//
//

#ifndef __DGproto__CollectionBook__
#define __DGproto__CollectionBook__

#include "cocos2d.h"
#include "SilhouetteData.h"
#include "StageImgLoader.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

//class CollectionAnimation : public CCSprite
//{
//public:
//	static CollectionAnimation* create(int t_card_number);
//	
//	void setAnimation(int t_card_number);
//	
//	void startAnimation();
//	
//	int stage_number;
//	int level_number;
//	CCSize ani_size;
//	
//private:
//	int loop_length;
//	deque<int> animation_frame;
//	int ing_animation_frame;
//	
//	void startStageAnimation();
//	
//	void frameAnimation();
//};

class CollectionBook : public cocos2d::CCLayer, public CCEditBoxDelegate// public CCTextFieldDelegate
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CollectionBook);
	
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
	
private:
	bool is_menu_enable;
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
	
	int recent_card_number;
	bool is_enable_pageturn;
	
	CCSprite* recent_left_img;
	CCSprite* recent_right_img;
	CCSprite* after_left_img;
	CCSprite* after_right_img;
	
	CCSprite* covered_left_img;
	CCSprite* covered_right_img;
	
	CCSprite* animation_img;
	SEL_CallFunc end_animation_delegate;
	
	CCPoint begin_point;
	
	CCEditBox* input_text;
//	CCTextFieldTTF* input_text;
//    bool was_open_text;
	
//	void endCloseTextInput();
	
	CCPoint getContentPosition(int t_tag);
	int getContentRotate(int t_tag);
	
	
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
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__CollectionBook__) */
