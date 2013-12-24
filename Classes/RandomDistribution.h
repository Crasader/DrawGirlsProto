#ifndef __RandomDistribution__
#define __RandomDistribution__

#include "cocos2d.h"
#include <random>
//#include "DataStorageHub.h"
USING_NS_CC;


class RandomDistribution : public CCLayer
{
public:
	CCDrawNode* m_dn;
	std::mt19937 m_rEngine;
	RandomDistribution();
	virtual ~RandomDistribution();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static CCScene* scene()
	{
		// 'scene' is an autorelease object
		CCScene *scene = CCScene::create();
		
		// 'layer' is an autorelease object
		RandomDistribution *layer = RandomDistribution::create();
//		layer->setAnchorPoint(ccp(0.5,0));
//		layer->setScale(myDSH->screen_convert_rate);
//		layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
		
		scene->addChild(layer);
		return scene;
	}
	CREATE_FUNC(RandomDistribution);
	void update(float dt);
	//virtual void registerWithTouchDispatcher();
};

#endif
