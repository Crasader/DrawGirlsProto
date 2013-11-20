//
//  DecreaseCardDurabilityPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 14..
//
//

#ifndef __DGproto__DecreaseCardDurabilityPopup__
#define __DGproto__DecreaseCardDurabilityPopup__

#include "cocos2d.h"
#include "CardCase.h"
#include "StageImgLoader.h"
#include "StarGoldData.h"

USING_NS_CC;
using namespace std;

enum DecreaseCardDurabilityPopupZorder{
	kDecreaseCardDurabilityPopup_Z_gray = 1,
	kDecreaseCardDurabilityPopup_Z_img,
	kDecreaseCardDurabilityPopup_Z_particle
};

class RemoveCardSprite : public CCSprite
{
public:
	static RemoveCardSprite* create(CCTexture2D* t_texture);
	
	virtual void visit();
	
	void startSlowRemoveCard();
	
	void startFastRemoveCard();
	
	void startRecovery();
	
private:
	CCRect view_rect;
	CCSize frame_size;
	int ing_frame;
	float decrease_value;
	
	void recoveringCard();
	
	void slowRemovingCard();
	
	void fastRemovingCard();
	
	void myInit(CCTexture2D* t_texture);
};

class DecreaseCardDurabilityPopup : public CCLayer
{
public:
	static DecreaseCardDurabilityPopup* create(int t_stage, int t_grade);
	
private:
	bool is_touch_enable;
	CCParticleSystemQuad* arrow_particle;
	CCSprite* gray;
	CCSprite* decrease_durability_label;
	RemoveCardSprite* selected_card;
	CardCase* t_case;
	CCMenu* recovery_menu;
	bool is_menu_on;
	
	int remove_state;
	
	int stage_number;
	int grade_number;
	
	void myInit(int t_stage, int t_grade);
	
	void openingAction();
	
	void startEffect();
	
	CCParticleSystemQuad* remove_particle;
	
	void removeParticle();
	
	void menuAction(CCObject* sender);
	
	void touchOn();
	
	void fadeGrayTitle();
	
	void closingAction();
	
	void fastRemoveAction();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__DecreaseCardDurabilityPopup__) */
