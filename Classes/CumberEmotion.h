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
	kNone = 6,
	kBored = 7,
	kCry = 8,
	kAnger = 9,
	kScratch = 10
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
	void toBored();
	void toCry();
	void toAnger();
	void toScratch();
	void presentationEmotion(const std::string& emotion);
	void update(float dt);
protected:
	CCSprite* m_currentEmotion;
	EmotionState m_emotionState;
	float m_boredTimer; // 감정 없는 상태로 얼마나 지속되었는지.
	//virtual void registerWithTouchDispatcher();
};

#endif
