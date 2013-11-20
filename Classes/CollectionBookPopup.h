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
#include "SilhouetteData.h"
#include "StageImgLoader.h"

USING_NS_CC;
using namespace std;

class CollectionAnimation : public CCSprite
{
public:
	static CollectionAnimation* create(int t_card_number)
	{
		CollectionAnimation* t_ca = new CollectionAnimation();
		t_ca->setAnimation(t_card_number);
		t_ca->initWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_animation.png", t_ca->stage_number, t_ca->level_number)->getCString()), CCRectMake(0, 0, t_ca->ani_size.width, t_ca->ani_size.height));
		t_ca->setPosition(mySD->getAnimationPosition(t_ca->stage_number));
		t_ca->autorelease();
		t_ca->startAnimation();
		return t_ca;
	}
	
	void setAnimation(int t_card_number)
	{
		stage_number = NSDS_GI(kSDS_CI_int1_stage_i, t_card_number);
		level_number = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
		ani_size = mySD->getAnimationCutSize(stage_number);
		
		mySD->setAnimationLoop(stage_number);
		loop_length = mySD->getAnimationLoopLength(stage_number);
		
		for(int i=0;i<loop_length;i++)
			animation_frame.push_back(mySD->getAnimationLoopPoint(i));
	}
	
	void startAnimation()
	{
		startStageAnimation();
	}
	
	int stage_number;
	int level_number;
	CCSize ani_size;
	
private:
	int loop_length;
	deque<int> animation_frame;
	int ing_animation_frame;
	
	void startStageAnimation()
	{
		ing_animation_frame = 0;
		
		int random_value = rand()%16;
		
		if(random_value >= 2 && random_value <= 4)
			random_value = 7;
		else if(random_value >= 5 && random_value <= 10)
			random_value = 10;
		else if(random_value >= 11 && random_value <= 13)
			random_value = 13;
		else if(random_value == 14 || random_value == 15)
			random_value = 16;
		else
			random_value++;
		
		CCDelayTime* t_delay = CCDelayTime::create(random_value*0.1f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CollectionAnimation::frameAnimation));
		CCAction* t_seq = CCSequence::create(t_delay, t_call, NULL);
		runAction(t_seq);
	}
	
	void frameAnimation()
	{
		int loop_point = animation_frame[ing_animation_frame];
		setTextureRect(CCRectMake(loop_point*ani_size.width, 0, ani_size.width, ani_size.height));
		
		ing_animation_frame++;
		if(ing_animation_frame >= loop_length)
			startStageAnimation();
		else
		{
			CCDelayTime* t_delay = CCDelayTime::create(0.1f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CollectionAnimation::frameAnimation));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			runAction(t_seq);
		}
	}
};

class CollectionBookPopup : public CCLayer, public CCTextFieldDelegate
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
	
private:
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	CCNode* main_case;
	
	bool is_menu_enable;
	
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
	
	CCTextFieldTTF* input_text;
    bool was_open_text;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	void endCloseTextInput();
	
	CCPoint getContentPosition(int t_tag);
	int getContentRotate(int t_tag);
	
	virtual bool onTextFieldInsertText(CCTextFieldTTF* sender, const char* text, int nLen);
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
	
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
	
	void startPreFullSelectedPage();
	void startPreSelectedPage();
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__CollectionBookPopup__) */
