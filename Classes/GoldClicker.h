#ifndef GOLDCLICKER_H
#define GOLDCLICKER_H

#include "cocos2d.h"
USING_NS_CC;
#include "KSUtil.h"

#include "StarGoldData.h"
#include "DataStorageHub.h"

class GoldClicker : public CCLayer
{
public:

	GoldClicker();
	virtual ~GoldClicker();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(int priority, const std::function<void(CCObject*, SEL_CallFunc, int, int)>& hideFunction)
	{
		CCLayer::init();
			
		m_flowTime = 0.f;
		m_odd = false;
		m_priority = priority;
		m_hideFunction = hideFunction;
		m_remainTime = 15.f;
		m_startMine = false;
		m_obtainGold = m_obtainRuby = m_obtainSocial = 0;
		CCSprite* back = CCSprite::create("minigame_bg.png");
		addChild(back);
		back->setPosition(ccp(240, 160));
		m_stone = CCSprite::create("minigame5_time_1.png");
		addChild(m_stone);
		m_stone->setPosition(ccp(240, 160));

		CCSprite* itemList = CCSprite::create("minigame_icon.png");
		itemList->setPosition(ccp(70, 250));
		addChild(itemList, 1);
		
		m_obtainGoldFnt = CCLabelBMFont::create("0", "timefont.fnt");
		m_obtainGoldFnt->setPosition(ccp(130, 262));
		addChild(m_obtainGoldFnt, 1);
		m_obtainRubyFnt = CCLabelBMFont::create("0", "timefont.fnt");
		m_obtainRubyFnt->setPosition(ccp(130, 238));
		addChild(m_obtainRubyFnt, 1);
		m_obtainSocialFnt = CCLabelBMFont::create("0", "timefont.fnt");
		m_obtainSocialFnt->setPosition(ccp(130, 217));
		addChild(m_obtainSocialFnt, 1);
		CCSprite* ready = KS::loadCCBI<CCSprite*>(this, "minigame1.ccbi").first;
		addChild(ready, 1);

		ready->setPosition(ccp(240, 160));

		m_howToUse = KS::loadCCBI<CCSprite*>(this, "minigame3.ccbi").first;
		addChild(m_howToUse);
		m_howToUse->setPosition(ccp(300, 200));
		addChild(KSTimer::create(2.f, [=]()
														 {
															 addChild(KSGradualValue<float>::create(255.f, 0.f, 1.f,
																															[=](float t)
																															{
																																KS::setOpacity(m_howToUse, t);
																															},
																															[=](float t)
																															{
																																m_howToUse->setVisible(false);	
																															}));
														 }));
		
		CCSprite* b2 = KS::loadCCBI<CCSprite*>(this, "minigame4.ccbi").first;
		b2->setPosition(ccp(240, 160));
		addChild(b2, 5);
		CCSprite* b3 = KS::loadCCBI<CCSprite*>(this, "minigame5.ccbi").first;
		b3->setPosition(ccp(240, 160));
		addChild(b3, 5);
		CCSprite* b4 = KS::loadCCBI<CCSprite*>(this, "minigame6.ccbi").first;
		b4->setPosition(ccp(240, 160));
		addChild(b4, 5);
		
		CCPoint progressPosition = ccp(240, 300);
		m_timer1 = CCProgressTimer::create(CCSprite::create("minigame5_time_3.png"));
		m_timer1->setPosition(progressPosition);
		m_timer1->setType(kCCProgressTimerTypeBar);
		m_timer1->setBarChangeRate(ccp(1, 0));
		m_timer1->setMidpoint(ccp(0.0f, 0.5f));
		addChild(m_timer1, 5);

		m_timer2 = CCProgressTimer::create(CCSprite::create("minigame5_time_2.png"));
		m_timer2->setPosition(progressPosition);
		m_timer2->setType(kCCProgressTimerTypeBar);
		m_timer2->setBarChangeRate(ccp(1, 0));
		m_timer2->setMidpoint(ccp(0.0f, 0.5f));
		addChild(m_timer2, 5);
		m_timer2->setVisible(false);
		
		m_timer1->setPercentage(100.f);
		CCSprite* timeProgressBack = CCSprite::create("minigame_.png");
		timeProgressBack->setPosition(ccp(230, 300));
		addChild(timeProgressBack, 6);
		
		auto ccbLoad = KS::loadCCBI<CCSprite*>(this, "minigame2.ccbi");
		m_hamerManager = ccbLoad.second;
		m_hamerSprite = ccbLoad.first;
		addChild(m_hamerSprite, 4);
		m_hamerSprite->setPosition(ccp(292, 194));
	
		setTouchEnabled(true);
		return true;
	}
	static GoldClicker* create(int priority, const std::function<void(CCObject*, SEL_CallFunc, int, int)>& hideFunction)
	{
		GoldClicker* t = new GoldClicker();
		t->init(priority, hideFunction);
		t->autorelease();
		return t;
	}
	void startSchedule()
	{
		scheduleUpdate();
		m_startMine = true;
	}
	void update(float dt)
	{
		m_flowTime += dt;
		m_odd = !m_odd;
		m_timer1->setPercentage((m_remainTime - m_flowTime ) / m_remainTime * 100.f);
		m_timer2->setPercentage((m_remainTime - m_flowTime ) / m_remainTime * 100.f);
		if(m_odd)
		{
			m_timer1->setVisible(true);
			m_timer2->setVisible(false);
		}
		else
		{
			m_timer1->setVisible(false);
			m_timer2->setVisible(true);
		}
		if(m_remainTime <= m_flowTime)
		{
			unscheduleUpdate();

			setTouchEnabled(false);
//			m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent), m_obtainRuby, m_obtainGold, m_obtainSocial);
//			mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + m_obtainRuby);
//			mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) + m_obtainGold);
//			mySGD->setFriendPoint(mySGD->getFriendPoint() + m_obtainSocial);
//			myDSH->saveUserData({kSaveUserData_Key_star, kSaveUserData_Key_gold}, [=](Json::Value v)
//					{
						addChild(KSTimer::create(3.f, [=](){
							m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent), m_obtainRuby, m_obtainGold);
							//m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent));
						}));
//					});
		}
	}
	virtual void registerWithTouchDispatcher();
protected:
	bool m_odd;
	float m_remainTime;
	float m_flowTime;
	bool m_startMine;
	int m_priority;
	int m_obtainGold, m_obtainRuby, m_obtainSocial;
	std::function<void(CCObject*, SEL_CallFunc, int, int)> m_hideFunction;
	CCProgressTimer* m_timer1;
	CCProgressTimer* m_timer2;
	CCSprite* m_howToUse;
	CCBAnimationManager* m_hamerManager;
	CCSprite* m_hamerSprite;
	CCSprite* m_stone;
	CCLabelBMFont* m_obtainRubyFnt;
	CCLabelBMFont* m_obtainGoldFnt;
	CCLabelBMFont* m_obtainSocialFnt;
};

#endif
