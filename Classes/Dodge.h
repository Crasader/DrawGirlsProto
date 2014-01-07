#pragma once

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "Well512.h"
#include "KSUtil.h"
#include "StageImgLoader.h"
USING_NS_CC;
enum class DodgeState
{
	kReady = 1,
	kPlay,
	kResult
	
};


class DodgeBullet : public CCNode
{
public:
	DodgeBullet(){}
	virtual ~DodgeBullet(){}
	static DodgeBullet* create(CCPoint startPosition)
	{
		DodgeBullet* t = new DodgeBullet();
		t->init(startPosition);
		t->autorelease();
		
		return t;
	}
	bool init(CCPoint startPosition)
	{
		CCNode::init();
		m_bullet = CCSprite::create("chapter1_basic_missile_1.png");
		m_bullet->setPosition(startPosition);
		
		addChild(m_bullet);
		
		scheduleUpdate();
		return true;
	}
	CCSprite* m_bullet;
	CCPoint m_dv; // 속도
	void update(float dt)
	{
		m_bullet->setPosition(m_bullet->getPosition() + m_dv);
	}
	
protected:
};

class DodgePlayer : public CCNode
{
public:
	static DodgePlayer* create()
	{
		DodgePlayer* t = new DodgePlayer();
		t->init();
		t->autorelease();
		return t;
	}
	bool init()
	{
		CCNode::init();
		auto t_pair = KS::loadCCBIForFullPath<CCSprite*>(this,
								StageImgLoader::sharedInstance()->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1) + ".ccbi");
		m_image = t_pair.first;
		addChild(m_image);
		
		return true;
	}
	CCSprite* m_image;
};

class BulletContainer : public CCNode
{
public:
	static BulletContainer * create(void)
	{
		BulletContainer * pRet = new BulletContainer();
    if (pRet && pRet->init())
    {
			pRet->autorelease();
    }
    else
    {
			CC_SAFE_DELETE(pRet);
    }
		return pRet;
	}
	bool init()
	{
		CCNode::init();
		
		scheduleUpdate();
		return true;
	}
	void update(float dt);
};
#define __TYPE__ Dodge
class Dodge : public CCLayer
{
public:
	Dodge() : m_state(DodgeState::kReady),
	m_remainTime(15.f)
	{}
	virtual ~Dodge()
	{
		
	}
	static __TYPE__* create(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction)
	{
    __TYPE__ *pRet = new __TYPE__();
    if (pRet && pRet->init(priority, hideFunction))
    {
			pRet->autorelease();
			return pRet;
    }
    else
    {
			delete pRet;
			pRet = NULL;
			return NULL;
    }
	}
	static CCScene* scene()
	{
		// 'scene' is an autorelease object
		CCScene *scene = CCScene::create();
		
		// 'layer' is an autorelease object
		Dodge *layer = Dodge::create(0, nullptr);
		layer->setAnchorPoint(ccp(0.5,0));
		layer->setScale(myDSH->screen_convert_rate);
		layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
		
		scene->addChild(layer);
		return scene;
	}
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_priority, true);
	}
	bool init(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction);
	void update(float dt);
	void checkCollision(float dt);
	void timeChecker(float dt);
protected:
	std::function<void(CCObject*, SEL_CallFunc)> m_hideFunction;
	CCClippingNode* m_thiz;
	int m_priority;
	float m_timer;
	float m_remainTime;
	CCLabelBMFont* m_flowTimeFnt;
	CCLabelTTF* m_readyFnt;
	Well512 m_well512;
	int m_bulletCountGoal;
	int m_bulletCount;
	DodgeState m_state;
	DodgePlayer* m_player;
	BulletContainer* m_bulletContainer;
};
#undef __TYPE__

