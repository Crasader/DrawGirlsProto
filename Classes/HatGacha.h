#ifndef __HatGacha__
#define __HatGacha__

#include "cocos2d.h"
#include <functional>
USING_NS_CC;
#include "CCMenuLambda.h"
#include "KSAlertView.h"
#include "KSUtil.h"
#include <vector>
#include "Well512.h"
#include "ProbSelector.h"
#include "FromTo.h"
#include "KSUtil.h"
#include "StarGoldData.h"
#include "GachaBase.h"
#include "GachaPurchase.h"


enum class RewardKind
{
	kGold,
	kRuby
};
class RewardSprite : public CCSprite
{
public:
	virtual ~RewardSprite(){}
	RewardKind m_kind;
	int m_value; // 가치.
};

class CCMenuItemToggleWithTopHatLambda : public CCMenuItemToggleLambda
{
public:
	static CCMenuItemToggleWithTopHatLambda * createWithTarget(std::function<void(CCObject*)> selector, CCMenuItemLambda* item, ...)
	{
		va_list args;
		va_start(args, item);
		CCMenuItemToggleWithTopHatLambda *pRet = new CCMenuItemToggleWithTopHatLambda();
		pRet->initWithTarget( selector, item, args);
		pRet->autorelease();
		va_end(args);
		return pRet;
	}
	CCMenuItemToggleLambda* m_hatTop;
	RewardSprite* m_reward;
};


#define __TYPE__ HatGachaSub

enum class SceneState
{
	kPutItem,
	kBeforeCoveringHat,
	kCoveringHat,
	kCoveredHat,
	kRotationHat,
	kStopingHat,
	kStopHat,
	kSelectedHatMoving,
	kShowReward1,
	kShowReward2
};
class HatGachaSub : public CCLayer
{
protected:
	CCMenuLambda* m_menu, *m_internalMenu;
	CCMenuLambda* m_disableMenu;
	std::vector<std::pair<CCMenuItemToggleWithTopHatLambda*, float> > m_hats;
	std::vector<std::pair<RewardSprite*, float> > m_rewards;
	Well512 m_well512;
	bool m_rewardFollowHat;
	SceneState m_state;
	float m_timer;
	float m_oppositeSecond; // 반대쪽으로 돌기 위한 다음 초.
	FromToWithDuration<float> m_rotationSpeed; // 각속도
	const float m_untouchableRotationSpeed;
	int m_untouchableCount;
	CCSprite* m_selectComment;
	CCMenuItemToggleWithTopHatLambda* m_selectedHat;
	GachaPurchaseStartMode m_gachaMode;
public:
	KSAlertView* m_parent;
	std::function<void(void)> m_callback;
	HatGachaSub() : m_rewardFollowHat(true), m_state(SceneState::kPutItem), m_timer(0),
	m_untouchableRotationSpeed(10.f),
	m_rotationSpeed(0.f,3.f,1.5f),
	m_untouchableCount(0),
	m_oppositeSecond(2)
	{
		
	}
	virtual ~HatGachaSub()
	{
		
	}

	void registerWithTouchDispatcher()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN + 1,true);
	}
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if(m_state == SceneState::kShowReward2)
		{
			if(m_parent)
			{
				m_parent->removeFromParent();
				m_parent->m_customCloseFunction();
			}
			
			if(m_callback != nullptr)
			{
				m_callback();
				removeFromParent();
			}
		}
		
		return true;
	}
	static __TYPE__* create(KSAlertView* av, GachaPurchaseStartMode gsm = kGachaPurchaseStartMode_reward) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(av, gsm))
    { \
			pRet->autorelease(); \
			return pRet; \
    } \
    else \
    { \
			delete pRet; \
			pRet = NULL; \
			return NULL; \
    } \
	}
	static __TYPE__* create(std::function<void(void)> callback, GachaPurchaseStartMode gsm = kGachaPurchaseStartMode_reward) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(callback, gsm))
    { \
			pRet->autorelease(); \
			return pRet; \
    } \
    else \
    { \
			delete pRet; \
			pRet = NULL; \
			return NULL; \
    } \
	}
	bool init(KSAlertView* av, GachaPurchaseStartMode gsm)
	{
		return init(av, nullptr, gsm);
	}
	bool init(std::function<void(void)> callback, GachaPurchaseStartMode gsm)
	{
	 return init(nullptr, callback, gsm);
	}
	
	virtual bool init(KSAlertView* av, std::function<void(void)> callback, GachaPurchaseStartMode gsm);
	void update(float dt)
	{
		m_timer += 1/60.f;
		if(m_state == SceneState::kBeforeCoveringHat)
		{
			repositionHat();
		}
		else if(m_state == SceneState::kCoveringHat)
		{
			if(m_timer >= 2)
			{
				for(auto i : m_hats)
				{
					i.first->setSelectedIndex(0);
					i.first->m_hatTop->setSelectedIndex(0);
					i.first->m_reward->setVisible(false);
				}
				m_state = SceneState::kCoveredHat;
				m_timer = 0;
			}
			repositionHat();
		}
		else if(m_state == SceneState::kCoveredHat)
		{
			if(m_timer >= 1)
			{
				m_state = SceneState::kRotationHat;
				m_timer = 0;
			}
			repositionHat();
		}
		else if(m_state == SceneState::kRotationHat)
		{
			//			ProbSelector ps = {0.003, 1.0 - 0.003};
			//			int t = ps.getResult();
			const int fastFrame = 150;
			if(m_timer >= m_oppositeSecond) // 한쪽 방향으로 n초 이상 돌았으면
			{
				m_timer = 0;
				m_oppositeSecond = m_well512.GetFloatValue(fastFrame / 60.f + 0.5f, 4.f);
				bool minus = m_rotationSpeed.getValue() < 0 ? true : false;
				float adder = m_well512.GetFloatValue(2, 3);
				if(minus)
					adder = -adder;
				
				float goalValue = -(m_rotationSpeed.getValue() + adder);
				goalValue = MAX(MIN(goalValue, m_untouchableRotationSpeed), -m_untouchableRotationSpeed);
				if(goalValue >= 6.f)
				{
					m_rotationSpeed.init(m_rotationSpeed.getValue(), goalValue, 0.5f);
				}
				else if(goalValue >= 4.f)
				{
					m_rotationSpeed.init(m_rotationSpeed.getValue(), goalValue, 0.7f);
				}
				else
				{
					m_rotationSpeed.init(m_rotationSpeed.getValue(), goalValue, 1.3f);
				}
			}
			
			for(auto& i : m_hats)
			{
				i.second += m_rotationSpeed.getValue();
			}
			if(m_rotationSpeed.getValue() >= m_untouchableRotationSpeed)
			{
				m_untouchableCount++;
				if(m_untouchableCount >= fastFrame) // 2.x초 이상 빨랐다면 멈추자
				{
					m_state = SceneState::kStopingHat;
					m_rotationSpeed.init(m_rotationSpeed.getValue(), 0.f, 2.f);
				}
			}
			else
			{
				m_untouchableCount = 0;
			}
			//			CCLog("ros%f", m_rotationSpeed.getValue());
			m_rotationSpeed.step(1/60.f);
			repositionHat();
		}
		else if(m_state == SceneState::kStopingHat)
		{
			for(auto& i : m_hats)
			{
				i.second += m_rotationSpeed.getValue();
			}
			bool notFinish = m_rotationSpeed.step(1/60.f);
			if(!notFinish)
			{
				m_state = SceneState::kStopHat;
				m_menu->setTouchEnabled(true);
				m_selectComment = CCSprite::create("hat_select.png");
				CCPoint centerPosition;
				if(m_parent)
					centerPosition = ccp(m_parent->getViewSize().width / 2.f, m_parent->getViewSize().height / 2.f)
					+ ccp(0, 50);
				else
					centerPosition = ccp(240, 160);
				m_selectComment->setPosition
				(centerPosition );
				addChild(m_selectComment, 20);
			}
			repositionHat();
		}
		else if(m_state == SceneState::kStopHat)
		{
			// 클릭가능한 상태.
		}
		
	}
	void repositionHat()
	{
		for(auto i : m_hats)
		{
			i.first->setPosition(retOnTheta(i.second * M_PI / 180.f));
		}
		
		topFollowBottom(); // 모자 위가 모자 밑둥을 따라감
		
		if(m_rewardFollowHat)
		{
			for(auto i : m_hats)
			{
				i.first->m_reward->setScale(i.first->getScale());
				i.first->m_reward->setPosition(i.first->getPosition());
			}
		}
		
		//		for(auto i :)
	}
	void topFollowBottom()
	{
		for(auto i : m_hats)
		{
			CCMenuItemToggleLambda* hatTop;
			hatTop = i.first->m_hatTop;
			hatTop->setAnchorPoint(ccp(0.5f, 0.0f));
			hatTop->setPosition(ccp(i.first->getPositionX(),
															i.first->getPositionY() + i.first->getContentSize().height / 2.f * i.first->getScale() - 2.f));
		}
		
		std::vector<std::pair<int, float> > ySort;
		for(int i=0; i<m_hats.size(); i++)
		{
			ySort.push_back(std::make_pair(i, m_hats[i].first->getPositionY()));
		}
		std::sort(ySort.begin(), ySort.end(), [](const std::pair<int, float>& a,
																						 const std::pair<int, float>& b)
							{
								return a.second > b.second;
							});
		float maxY = ySort[0].second;
		float minY = ySort.rbegin()->second;
		
		int zorder = 1;
		for(auto i : ySort)
		{
			m_hats[i.first].first->setZOrder(zorder);
			m_hats[i.first].first->setScale(1.0f - (i.second - minY) / (maxY - minY)*0.3f);
			CCMenuItemToggleLambda* hatTop;
			hatTop = m_hats[i.first].first->m_hatTop;
			hatTop->setZOrder(zorder);
			hatTop->setScale(1.0f - (i.second - minY) / (maxY - minY)*0.3f);
			zorder++;
		}
		
	}
	CCPoint retOnTheta(float t)
	{
		float r = 100.f;
		CCPoint centerPosition;
		if(m_parent)
			centerPosition = ccp(m_parent->getViewSize().width / 2.f, m_parent->getViewSize().height / 2.f);
		else
			centerPosition = ccp(240, 160);
		return centerPosition +
		ccp(r * cos(t), r * 3.f/4.f * sin(t)) + ccp(0, 0);
	}
	
};
#undef __TYPE__


#define __TYPE__ HatGacha
class HatGacha : public CCLayer
{
public:
	std::function<void(void)> m_closeCallback;
	HatGacha();
	virtual ~HatGacha();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(std::function<void(void)> closeCallback);
	static __TYPE__* create(std::function<void(void)> closeCallback) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(closeCallback))
    { \
			pRet->autorelease(); \
			return pRet; \
    } \
    else \
    { \
			delete pRet; \
			pRet = NULL; \
			return NULL; \
    } \
	}

	//virtual void registerWithTouchDispatcher();
};

#undef __TYPE__


#endif
