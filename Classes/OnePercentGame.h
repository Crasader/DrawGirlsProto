#ifndef ONEPERCENTGAME_H
#define ONEPERCENTGAME_H
#include "DataStorageHub.h"
#include "cocos2d.h"
#include "CountingBMLabel.h"
#include "LogData.h"
#include "StarGoldData.h"
USING_NS_CC;

#include "cocos-ext.h"

USING_NS_CC_EXT;
enum OnePercentGameZorder
{
	kOnePercentGame_Z_gray = 1,
	kOnePercentGame_Z_back,
	kOnePercentGame_Z_content,
	kOnePercentGame_Z_shop
};

class CommonButton;
class StyledLabelTTF;
class OnePercentGame : public CCLayer
{
public:
	OnePercentGame();
	virtual ~OnePercentGame();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static OnePercentGame* create()
	{
		OnePercentGame* t = new OnePercentGame();
		t->init();
		t->autorelease();
		return t;
	}
	static OnePercentGame* create(float originalPercent, std::function<void(void)> cancelGacha, std::function<void(float)> tryGacha,
																bool openEffect = true)
	{

		OnePercentGame* t = new OnePercentGame();
		t->init(originalPercent, cancelGacha, tryGacha, openEffect);
		t->autorelease();
		return t;
	}
	bool init(float originalPercent, std::function<void(void)> cancelGacha, std::function<void(float)> tryGacha,
						bool openEffect);
	void gameUISetting(bool hide = true) ;
	void gachaAction(CCObject* sender, CCControlEvent t_event) ;
	void showFail() ;
	void showSuccess() ;
	void update(float dt) ;
	virtual void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
protected:
	CCSprite* m_grayBack;
	CCNode* m_container;

	CommonButton * cancel_menu;
	CCScale9Sprite* back_img;
	float recent_percent;
	CountingBMLabel * percent_label;
	CCProgressTimer * progress_img;
	CCSprite * arrow_img;
	CCControlButton* gacha_button;
	CCSprite* m_cursor;
	int m_cursorDirection;
	static const int leftLimit = -115;
	static const int rightLimit = 115;
	static const int graphHeight = 176;
	int m_99State;
	CCNode* m_currentGainArea, *m_desc;
	int m_validSize;
	CCSprite* m_validArea;
	CCSprite* m_shutter;
	bool m_touchEnable;
	std::function<void(void)> m_cancelGacha;
	std::function<void(float)> m_resultGacha;
	float m_totalPercent;
	CCLabelTTF* m_lblPercent;
	StyledLabelTTF* m_desc2;
	CCClippingNode* m_closer;
	CCNode* m_graphNode;
	CCNode* m_stencil;
	CCSprite* m_gradient;
	std::function<void(void)> m_toDown, m_toUp;
	CCNode* m_tutorialBox, *m_desc3;
	CCNode* m_failBox, *m_failContent;
//	CCNode* m_command1;
};
#endif
