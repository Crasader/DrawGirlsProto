#ifndef CUMBEREMOTION_H
#define CUMBEREMOTION_H

#include "cocos2d.h"
USING_NS_CC;
#include "KSUtil.h"

enum class EmotionState
{
	kFun = 1,
	kStun = 2,
	kRidicule = 3,
	kSad = 4,
	kJoy = 5,
	kNone = 6
};
class CumberEmotion : public CCNode
{
public:
	CumberEmotion();
	virtual ~CumberEmotion();
	virtual bool init();
	static CumberEmotion* create()
	{
		CumberEmotion* t = new CumberEmotion();
		t->init();
		t->autorelease();
		return t;
	}
	void onTouchingLine();
	void goStun();
	void releaseStun();
	void onKillingJack();
	void presentationEmotion(const std::string& emotion);
protected:
	CCSprite* m_currentEmotion;
	EmotionState m_emotionState;
	//virtual void registerWithTouchDispatcher();
};

#endif
