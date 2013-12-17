#include "RandomDistribution.h"

RandomDistribution::RandomDistribution()
{
	
}
RandomDistribution::~RandomDistribution()
{
}
//void RandomDistribution::registerWithTouchDispatcher()
//{
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
//}

//bool RandomDistribution::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

// 대충 되는 버전.
bool RandomDistribution::init()
{
	CCLayer::init();
	srand(time(NULL));
	m_rect = new CCSprite();
	m_rect->init();
//	m_rect->setTextureRect(CCRectMake(0,0,1,1));
//	CCTexture2D* t2d = new CCTexture2D();
	
	m_batchNode = CCSpriteBatchNode::createWithTexture(m_rect->getTexture(), 100000);
	addChild(m_batchNode);
	schedule(schedule_selector(RandomDistribution::update), 0.f, 360, 0.f);
	
	std::random_device rd;
	m_rEngine.seed(rd());
	
	std::uniform_int_distribution<> distX(0, 480-1), distY(0, 480-1);
	std::uniform_int_distribution<> distColor(0, 255);
	for(int i=0; i<200000; i++)
	{
		
#if 1
		CCSprite* rect = CCSprite::createWithTexture(m_rect->getTexture());
		rect->setTextureRect(CCRectMake(0,0,1,1));
		rect->setColor(ccc3(rand()%256, rand()%256, rand()%256));
		rect->setPosition(ccp(rand()%480, rand()%480));
#endif
#if 0
		CCSprite* rect = CCSprite::createWithTexture(m_rect->getTexture());
		rect->setTextureRect(CCRectMake(0,0,1,1));
		
		rect->setColor(ccc3(distColor(m_rEngine), distColor(m_rEngine), distColor(m_rEngine)));
		rect->setPosition(ccp(distX(m_rEngine), distY(m_rEngine)));
#endif
		m_batchNode->addChild(rect);
	}

	
	return true;
}


void RandomDistribution::update(float dt)
{
	
}