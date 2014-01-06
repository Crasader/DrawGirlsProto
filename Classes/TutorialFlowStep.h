//
//  TutorialFlowStep.h
//  DGproto
//
//  Created by 사원3 on 2014. 1. 5..
//
//

#ifndef __DGproto__TutorialFlowStep__
#define __DGproto__TutorialFlowStep__

#include "cocos2d.h"

USING_NS_CC;

enum TutorialFlowStep
{
	kTutorialFlowStep_puzzleClick = 0,
	kTutorialFlowStep_pieceClick,
	kTutorialFlowStep_readyClick,
	kTutorialFlowStep_startClick,
	kTutorialFlowStep_ingame,
	kTutorialFlowStep_homeClick,
	kTutorialFlowStep_pieceType,
	kTutorialFlowStep_pieceClick2,
	kTutorialFlowStep_readyClick2,
	kTutorialFlowStep_emptyCardClick,
	kTutorialFlowStep_targetCardClick,
	kTutorialFlowStep_mountClick,
	kTutorialFlowStep_closeClick,
	kTutorialFlowStep_backClick,
	kTutorialFlowStep_cardCollectionClick,
	kTutorialFlowStep_upgradeClick,
	kTutorialFlowStep_upgradeScript,
	kTutorialFlowStep_end
};

class TutorialFlowStepLayer : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(TutorialFlowStepLayer);
	
	void initStep(TutorialFlowStep t_step);
	
private:
	TutorialFlowStep recent_step;
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};

#endif /* defined(__DGproto__TutorialFlowStep__) */
