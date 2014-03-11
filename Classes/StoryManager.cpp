//
//  StoryManager.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 11..
//
//

#include "StoryManager.h"


bool StoryManager::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	
	return true;
}

void StoryManager::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void StoryManager::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void StoryManager::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void StoryManager::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, m_touch_priority, true);
}