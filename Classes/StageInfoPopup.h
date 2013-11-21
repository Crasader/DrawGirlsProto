//
//  StageInfoPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 21..
//
//

#ifndef __DGproto__StageInfoPopup__
#define __DGproto__StageInfoPopup__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum StageInfoPopupZorder{
	kSIP_Z_gray = 1,
	kSIP_Z_back,
	kSIP_Z_content
};

enum StageInfoPopupMenuTag{
	kSIP_MT_close = 1,
	kSIP_MT_stageLabel,
	kSIP_MT_step1Rank,
	kSIP_MT_step2Rank,
	kSIP_MT_step3Rank,
	kSIP_MT_step1Reward,
	kSIP_MT_step2Reward,
	kSIP_MT_step3Reward,
	kSIP_MT_boss,
	kSIP_MT_monster,
	kSIP_MT_mission
};

class StageInfoPopup : public CCLayer
{
public:
	static StageInfoPopup* create(CCObject* t_close, SEL_CallFunc d_close, int t_stage_number);
	
private:
	bool is_menu_enable;
	
	int stage_number;
	
	int touched_number;
	CCMenu* close_menu;
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	CCSprite* back_img;
	CCSprite* gray;
	
	virtual void onEnter();
	
	void menuAction(CCObject* sender);
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close, int t_stage_number);
	
	void setBack();
	
	CCPoint getContentPosition(int t_tag);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__StageInfoPopup__) */
