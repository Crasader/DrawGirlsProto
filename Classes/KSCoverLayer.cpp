#include "KSCoverLayer.h"
int KSCoverLayer::touchPriority = INT_MIN + 1000;



void KSCoverLayer::registerWithTouchDispatcher()
{
	cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate
		(this, INT_MIN + 1, true);
}

bool KSCoverLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLog("kscoverlayer swallow");
	return true; // swallow;
}

bool KSCoverLayer::init()
{
	if ( !this->CCLayerColor::initWithColor(cocos2d::ccc4(0, 0, 0, 0)))
		return false;

	CCSprite* b = CCSprite::create();
	b->setTextureRect(CCRectMake(0, 0, 600, 320));
	b->setColor(ccc3(0, 0, 0));
	//
	if(animation)
	{
		b->setOpacity(0);

		b->runAction(CCFadeTo::create(0.5f, 150));
	}
	else
	{
		b->setOpacity(150);
	}
	//		b->setOpacity(0);
	//	b->setContentSize(CCSizeMake(600, 320));
	b->setPosition(ccp(240, 160));
	b->setAnchorPoint(ccp(0.5f, 0.5f));

	addChild(b,0);
	//	this->ignoreAnchorPointForPosition(true);
	setTouchEnabled(true);

	return true;
}
