#ifndef ONEPERCENTTUTORIAL_H
#define ONEPERCENTTUTORIAL_H

#include "cocos2d.h"
USING_NS_CC;

#include "DataStorageHub.h"
#include "cocos2d.h"
#include "CountingBMLabel.h"
#include "LogData.h"
#include "StarGoldData.h"
USING_NS_CC;

#include "cocos-ext.h"

USING_NS_CC_EXT;
enum OnePercentTutorialZorder
{
	kOnePercentTutorial_Z_gray = 1,
	kOnePercentTutorial_Z_back,
	kOnePercentTutorial_Z_content,
	kOnePercentTutorial_Z_shop
};

class CommonButton;
class StyledLabelTTF;

class OnePercentTutorial : public CCLayer
{
public:
	OnePercentTutorial();
	virtual ~OnePercentTutorial();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(float originalPercent,
										std::function<void(void)> cancelGacha,
										std::function<void(float)> tryGacha);
	static OnePercentTutorial* create(float originalPercent,
																		std::function<void(void)> cancelGacha,
																		std::function<void(float)> tryGacha)
	{
		OnePercentTutorial* t = new OnePercentTutorial();
		t->init(originalPercent, cancelGacha, tryGacha);
		t->autorelease();
		return t;
	}
	//virtual void registerWithTouchDispatcher();
	void gachaAction(CCObject* sender, CCControlEvent t_event) ;
	void update(float dt);
protected:
	int m_tutorialStep;
	static const int leftLimit = -115;
	static const int rightLimit = 115;
	static const int graphHeight = 176;
	int m_cursorDirection;
	CCSprite* m_grayBack;
	CCNode* m_container;

	CommonButton * cancel_menu;
	CCSprite* back_img;
	CCControlButton* gacha_button;
	StyledLabelTTF* m_gachaDesc;
	CCSprite* m_cursor;
	float m_originalPercent;
	std::function<void(void)> m_cancelGacha;
	std::function<void(float)> m_tryGacha;
	
	
};

#endif
