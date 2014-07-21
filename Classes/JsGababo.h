#ifndef JSGABABO_H
#define JSGABABO_H

#include "cocos2d.h"
USING_NS_CC;


#include "DataStorageHub.h"
#include "CCMenuLambda.h"
#include "EasingAction.h"
#include "CurtainNodeForBonusGame.h"


class JsGababo : public CCLayer
{
public:
	JsGababo();
	virtual ~JsGababo();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static JsGababo* create()
	{
		JsGababo* t = new JsGababo();
		t->init();
		t->autorelease();
		return t;
	}
	virtual bool init(int touchPriority, const std::vector<BonusGameReward>& rewards, std::function<void(int)> endFunction);
	static JsGababo* create(int prior, const std::vector<BonusGameReward>& rewards, std::function<void(int)> endFunction)
	{
		JsGababo* t = new JsGababo();
		t->init(prior, rewards, endFunction);
		t->autorelease();
		return t;
	}
	//virtual void registerWithTouchDispatcher();
	void setupReward();
	void setupTutorial();
	void setupCongMessage();
	void setupHands();
	void loadImage(int step);
	void showReward();
	void contextSwitching(CCNode* from, CCNode* to, std::function<void(void)> mid, std::function<void(void)> end);
	void showHands();
	void showHandsMotion(std::function<void(void)> endLeft, std::function<void(void)> endRight);
	void showHandsMotionWrapper();
	void onPressConfirm(CCObject* t);
	void showResult();
	int winSkill(int i);
	int loseSkill(int i);
	int drawSkill(int i);
protected:
	
	std::function<void(int)> m_endFunction;
	int m_touchPriority;
	CCNode* m_back;
	CCScale9Sprite* m_front1;
	CCScale9Sprite* m_front2;
	CCScale9Sprite* m_front3;
	CCNode* m_realFront;
	std::vector<BonusGameReward> m_rewards;
	CCSprite* m_stepFrame;
	CCSprite* m_stepSprite;
//	int m_gameStep; // ?
	int m_winCount; // ?
	CCNode* m_leftHandContainer;
	CCNode* m_rightHandContainer;
	CCNode* m_leftHand;
	CCNode* m_rightHand;
	int m_mySelection;
	StyledLabelTTF* m_message;
	std::string m_currentJudge;
	CCNode* m_resultStamp;
	CCSprite* m_currentRewardCursor;
	int m_tutorialStep;
	CommonButton* m_ba, *m_ga, *m_bo;
	CommonButton* m_confirmButton;
	CCBAnimationManager* meManager;
	CCBAnimationManager* npcManager;
	CCSprite* m_meCharSprite;
	CCSprite* m_npcCharSprite;
	CCSprite* m_tutoGababo;
};

#endif
