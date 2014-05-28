#include "AsyncSprite.h"

AsyncSprite::AsyncSprite()
{
	
}
AsyncSprite::~AsyncSprite()
{
}
//void AsyncSprite::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool AsyncSprite::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool AsyncSprite::init()
{
	CCSprite::init();
	
	return true;
}


bool AsyncSprite::init(const char* fileName)
{
	CCSprite::init();
	m_fileName = fileName;
	return true;
}
void AsyncSprite::loadImage(CCObject* par)
{
	m_parent = par;
	CCTextureCache::sharedTextureCache()->addImageAsync(m_fileName.c_str(), this, callfuncO_selector(AsyncSprite::loadedImage));
}
void AsyncSprite::loadedImage(CCObject* obj)
{
	if(AsyncSpriteManager::getInstance()->isAliveObject(m_parent))
	{
		initWithFile(m_fileName.c_str());
	}
}
