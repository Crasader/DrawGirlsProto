//
//  CumberShowWindow.h
//  DGproto
//
//  Created by ksoo k on 2013. 11. 21..
//
//

#ifndef __DGproto__CumberShowWindow__
#define __DGproto__CumberShowWindow__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include "Well512.h"

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

/*
 , public CCBSelectorResolver  public CCBAnimationManagerDelegate
 
 
 virtual void completedAnimationSequenceNamed(const char *name) = 0;
 
 virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName) = 0;
 virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };
 virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) = 0;
 */

enum CumberShowWindowSceneCode{
	kCumberShowWindowSceneCode_puzzle = 0,
	kCumberShowWindowSceneCode_cardChange
};

class KSSnakeBase;
class CumberShowWindow : public CCNode, public CCBSelectorResolver,  public CCBAnimationManagerDelegate
{
public:
	virtual void completedAnimationSequenceNamed(const char *name)
	{
		
	}
	
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
	{
		return NULL;
	}
	virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
	{
		return NULL;
	}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
	{
		return NULL;
	}
	static CumberShowWindow* create(int stageNumber, CumberShowWindowSceneCode t_code = kCumberShowWindowSceneCode_puzzle)
	{
		CumberShowWindow* t_blg = new CumberShowWindow();
		t_blg->init(stageNumber, t_code);
		t_blg->autorelease();
		return t_blg;
	}
	bool init(int, CumberShowWindowSceneCode);
	CCBAnimationManager* m_circleAnimation;
	CCBAnimationManager* m_snakeHeadAnimation;
	std::vector<CCBAnimationManager*> m_snakeBodiesAnimation;
	CCBAnimationManager* m_snakeTailAnimation;
	
	CCSprite* m_circleSprite;
	KSSnakeBase* m_snakeNode;
	CCSprite* m_juniorSprite;
	CCBAnimationManager* m_juniorAnimation;
	
	
	Well512 m_well512;
};
#endif /* defined(__DGproto__CumberShowWindow__) */
