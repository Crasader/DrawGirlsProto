//
//  CardMatching.h
//  DGproto
//
//  Created by ksoo k on 2013. 12. 26..
//
//

#ifndef __DGproto__CardMatching__
#define __DGproto__CardMatching__
#include "cocos2d.h"
#include "CCMenuLambda.h"
#include "DataStorageHub.h"
#include <random>
USING_NS_CC;

enum class MatchingState
{
	kReady,
	kOneSelect,
	kDiff
};
class CardMatching : public CCLayer
{
public:
	static CCScene* scene()
	{
		// 'scene' is an autorelease object
		CCScene *scene = CCScene::create();
		
		// 'layer' is an autorelease object
		CardMatching *layer = CardMatching::create(0, nullptr);
		layer->setAnchorPoint(ccp(0.5,0));
		layer->setScale(myDSH->screen_convert_rate);
		layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));

		scene->addChild(layer);
		return scene;
	}
	
	CardMatching() :
	PUZZLE_COLS(5),
	PUZZLE_ROWS(4),
	m_remainTime(50)
	{}
	virtual ~CardMatching()
	{
		
	}
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction);
	static CardMatching* create(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction)
	{
    CardMatching* pRet = new CardMatching();
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
	enum CuttingType
	{
		kOriginal = 1,
		kFace = 2
	};
	void splitImage(CuttingType ct, const std::string& fileName, int cols, int rows, int padding, int margin, const std::function<void(CCImage*, int)>&);
	void update(float dt);
protected:
	CCClippingNode* m_thiz;
	int m_priority;
	std::function<void(CCObject*, SEL_CallFunc)> m_hideFunction;
	float m_remainTime;
	const int PUZZLE_COLS, PUZZLE_ROWS;
	CCMenuLambda* m_menu;
	std::mt19937 m_rEngine;
	std::vector<CCSprite*> m_matchCards;
	MatchingState m_state;
	int m_firstSelectNum;
	int m_secondSelectNum;
	CCSprite* m_firstSelectSprite;
	CCSprite* m_secondSelectSprite;
	CCMenuItemLambda* m_firstSelectItem, *m_secondSelectItem;
	int m_correctCount;
	CCLabelBMFont* m_timeFnt;
	float m_timer;
	
};


#endif /* defined(__DGproto__CardMatching__) */
