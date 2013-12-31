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

void CountingGame::startSchedule()
{
	schedule(schedule_selector(CountingGame::createObject));
}
bool CountingGame::init(int priority, const std::function<void(void)>& hideFunction)
{
	CCLayer::init();
	
	CCDrawNode* shape = CCDrawNode::create();
	
	CCPoint pts[4];
	pts[0] = ccp(25, 320 - 297);
	pts[1] = ccp(366, 320 - 297);
	pts[2] = ccp(366, 320 - 24);
	pts[3] = ccp(25, 320 - 24);
	shape->drawPolygon(pts, 4, ccc4f(1, 1, 1, 1), 0, ccc4f(1, 0, 0, 1));

	CCSprite* back = CCSprite::create("bonusgame_back.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	// add shape in stencil
	m_thiz = CCClippingNode::create();
	m_thiz->setAnchorPoint(ccp(0.5, 0.5));
	m_thiz->setStencil(shape);
	this->addChild(m_thiz);
	m_priority = priority;
	m_hideFunction = hideFunction;
	// 480 - 143 = 337
	setTouchEnabled(true);
	std::random_device rd;
	m_rEngine.seed(rd());
	
	uniform_int_distribution<> dist(15, 23);
	m_goalCount = dist(m_rEngine);
	m_menu = CCMenuLambda::create();
	m_menu->setTouchEnabled(true);
	m_menu->setTouchPriority(m_priority);
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
																			CCLabelBMFont* result = CCLabelBMFont::create("ANSWER", "etc_font.fnt");
																			result->setPosition(ccp(240, 160));
																			addChild(result);
																			CCSprite* successSprite = CCSprite::create("bonusgame_succes.png");
																			successSprite->setPosition(ccp(240, 160));
																			addChild(successSprite);
																			CCLog("correct!!");
																			m_menu->setTouchEnabled(false);
																			unscheduleUpdate();
																			mySGD->setStar(mySGD->getStar() + 1);
																			myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v)
																													{
																														addChild(KSTimer::create(3.f, [=](){
																															m_hideFunction();
																														}));
																													});
																		}
																		else
																		{
																			CCLabelBMFont* result = CCLabelBMFont::create("WRONG", "etc_font.fnt");
																			result->setPosition(ccp(240, 160));
																			addChild(result);
																			CCSprite* failSprite = CCSprite::create("bonusgame_fail.png");
																			failSprite->setPosition(ccp(240, 160));
																			addChild(failSprite);
																			m_menu->setTouchEnabled(false);
																			addChild(KSTimer::create(3.f, [=]()
																															 {
																																 m_hideFunction();
																															 }));
																			unscheduleUpdate();
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

