//
//  Dodge.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 12. 11..
//
//

#include "CountingGame.h"
#include "StarGoldData.h"
#include "KSUtil.h"
#include "ProbSelector.h"
#include "KSUtil.h"
#include "KSGeometry.h"


bool CountingGame::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	
	return true;
}
void CountingGame::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	
}
bool CountingGame::init()
{
	CCLayer::init();
	// 480 - 143 = 337
	setTouchEnabled(true);
	std::random_device rd;
	m_rEngine.seed(rd());
	schedule(schedule_selector(CountingGame::createObject));
	
	uniform_int_distribution<> dist(15, 23);
	m_goalCount = dist(m_rEngine);
	m_menu = CCMenuLambda::create();
	m_menu->setTouchEnabled(true);
	m_menu->setTouchPriority(INT_MIN);
//	m_menu->setPosition(ccp(0, 0));
	addChild(m_menu, 1);
	return true;
}

void CountingGame::update(float dt)
{
	
}
void CountingGame::createObject(float dt)
{
	uniform_int_distribution<> distX(30, 480-30);
	uniform_int_distribution<> distY(30, 320-30);
	float x, y;

	while(1)
	{
		bool coll = false;
		x = distX(m_rEngine);
		y = distY(m_rEngine);
		for(auto i : m_objects)
		{
			CCRect r = CCRectMake(i->getPosition().x, i->getPosition().y, 1, 1);
			auto r2 = rtSetScale(r, 80.f);
			if(r2.containsPoint(ccp(x, y)))
			{
				coll = true;
				break;
			}
		}
		if(coll == false)
		{
			break;
		}
	}
	auto ccbi = KS::loadCCBI<CCSprite*>(this, "mob_cow.ccbi");
	CCSprite* t = ccbi.first;
	t->setPosition(ccp(x, y));
	m_objects.push_back(t);
	addChild(t);
	if(m_objects.size() >= m_goalCount)
	{
		unschedule(schedule_selector(CountingGame::createObject));
		addChild(KSTimer::create(2.f, [=]()
														 {
															 for(auto i : m_objects)
															 {
																 i->setVisible(false);
															 }
															 int number = m_objects.size();
															 uniform_int_distribution<> dist(0, 3);
															 int correctIndex = dist(m_rEngine);
															 std::vector<int> corrects;
															 corrects.assign(4, number);
															 for(int i=0; i<corrects.size(); i++)
															 {
																 corrects[i] += (i - correctIndex) * 3;
															 }
															 for(int i=0; i<corrects.size(); i++)
															 {
																 
																 CCMenuItemFontLambda* item1 = CCMenuItemFontLambda::create
																 (CCString::createWithFormat("%d", corrects[i])->getCString(),
																	[=](CCObject* s)
																	{
																		if(i == correctIndex)
																		{
																			CCLog("answer");
																		}
																		else
																		{
																			CCLog("wrong answer");
																		}
																	});
																 item1->setPosition(ccp(240, 50));
																 m_menu->addChild(item1);
															 }
															 m_menu->alignItemsHorizontallyWithPadding(70.f);
															 m_menu->alignItemsHorizontally();
														 }));
	}
}

