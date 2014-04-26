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
	addChild(KSTimer::create(3.f, [=]()
													 {
														 schedule(schedule_selector(CountingGame::createObject), 1/30.f);
													 }));
}
bool CountingGame::init(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction)
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
	normal_distribution<> dist(20, 4);
	m_goalCount = dist(m_rEngine);
	m_menu = CCMenuLambda::create();
	m_menu->setTouchEnabled(true);
	m_menu->setTouchPriority(m_priority);
	m_menu->setPosition(ccp(0, 0));
	m_menu->setPropaOnBegan(true);
	addChild(m_menu, 1);
	
	m_timeFnt = CCLabelBMFont::create(CCString::createWithFormat("%d", m_remainTime)->getCString(), "allfont.fnt");
	m_timeFnt->setPosition(ccp(420, 270));
	addChild(m_timeFnt, 2);
	
	auto ready_go = KS::loadCCBI<CCSprite*>(this, "ui_ready.ccbi");
	addChild(ready_go.first);
	ready_go.first->setPosition(ccp(240, 160));
//	addChild(KSTimer::create(3.f, [=]()
//													 {
//														 schedule(schedule_selector(CountingGame::createObject)); // 임시.
//														 
//													 }));
	return true;
}

void CountingGame::update(float dt)
{
	
	auto end = chrono::system_clock::now();
	int timer = chrono::system_clock::to_time_t(end) - m_startTime;
	m_timeFnt->setString(CCString::createWithFormat("%d", MAX(0, m_remainTime - timer))->getCString());
	if(m_remainTime < timer) // timeover
	{
		CCSprite* failSprite = CCSprite::create("bonusgame_fail.png");
		failSprite->setPosition(ccp(240, 160));
		addChild(failSprite);
		m_menu->setTouchEnabled(false);
		addChild(KSTimer::create(3.f, [=]()
														 {
															 m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent));
														 }));
		unscheduleUpdate();
	}
}
void CountingGame::createObject(float dt)
{
	uniform_int_distribution<> distX(50, 480-130);
	uniform_int_distribution<> distY(50, 320-50);
	float x, y;

	int loopCount = 0;
	while(1)
	{
		bool coll = false;
		loopCount++;
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
		if(loopCount >= 100)
		{
			m_goalCount = m_objects.size(); // 전부 추가했다고 봄.
			break;
		}
		
	}
	if(loopCount < 100)
	{
//		auto ccbi = KS::loadCCBI<CCSprite*>(this, "mob_cow.ccbi");
//		CCSprite* t = ccbi.first;
		CCSprite* t = CCSprite::create("bonusboss1.png");
		t->setPosition(ccp(x, y));
		m_objects.push_back(t);
		m_thiz->addChild(t);
	}
	if(m_objects.size() >= m_goalCount)
	{
		unschedule(schedule_selector(CountingGame::createObject));
		
		addChild(KSTimer::create(2.f, [=]()
														 {
															 auto end = chrono::system_clock::now();
															 m_startTime = chrono::system_clock::to_time_t(end);
															 scheduleUpdate();
															 for(auto i : m_objects)
															 {
																 i->setVisible(false);
															 }
															 CCSprite* quiz = CCSprite::create("bonusgame_bosscount_back.png");
															 quiz->setPosition(ccp(190, 160));
															 m_thiz->addChild(quiz);
															 
															 int number = m_objects.size();
															 uniform_int_distribution<> dist(0, 3);
															 int correctIndex = dist(m_rEngine);
															 std::vector<int> corrects;
															 corrects.assign(4, number);
															 for(int i=0; i<corrects.size(); i++)
															 {
																 corrects[i] += (i - correctIndex) * 3;
															 }
															 CCPoint examplePositions[] = {ccp(120, 160), {303, 160}, {120, 120}, {303, 120}};
															 for(int i=0; i<corrects.size(); i++)
															 {
																 
																 CCMenuItemFontLambda* item1 = CCMenuItemFontLambda::create
																 (CCString::createWithFormat("%d", corrects[i])->getCString(),
																	[=](CCObject* s)
																	{
																		CCMenuItemLambda* item = dynamic_cast<CCMenuItemLambda*>(s);
																		if(i == correctIndex)
																		{
																			CCSprite* resultMark = CCSprite::create("bonusgame_bosscount_ok.png");
																			resultMark->setPosition(item->getPosition());
																			addChild(resultMark, 10);
																			
																			CCLog("answer");
																			CCLabelBMFont* result = CCLabelBMFont::create("ANSWER", "allfont.fnt");
																			result->setPosition(ccp(240, 160));
																			addChild(result);
																			CCLog("correct!!");
																			m_menu->setTouchEnabled(false);
																			unscheduleUpdate();
//																			mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + 1);
//																			myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v)
//																													{
																														addChild(KSTimer::create(1.f, [=](){
																															m_menu->setVisible(false);
																															quiz->setVisible(false);
																															resultMark->setVisible(false);
																															CCLabelTTF* confirmPlz = CCLabelTTF::create("확인해보세요.", "", 14.f);
																															confirmPlz->setPosition(ccp(190, 280));
																															m_thiz->addChild(confirmPlz);
																															for(auto i : m_objects)
																															{
																																i->setVisible(true);
																															}
																															addChild(KSTimer::create(4.f, [=]()
																																											 {
																																												 CCSprite* successSprite = CCSprite::create("bonusgame_succes.png");
																																												 successSprite->setPosition(ccp(240, 160));
																																												 addChild(successSprite);
																																												 CCSprite* reward = CCSprite::create("price_ruby_img.png");
																																												 reward->setPosition(ccp(205, 133));
																																												 addChild(reward);
																																												 reward->setScale(2.f);
																																												 addChild(KSTimer::create(2.f, [=]()
																																																									{
																																																										m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent));
																																																									}));
																																											 }));
																														}));
//																													});
																		}
																		else
																		{
																			CCSprite* resultMark = CCSprite::create("bonusgame_bosscount_cancel.png");
																			resultMark->setPosition(item->getPosition());
																			addChild(resultMark, 10);
																			CCLabelBMFont* result = CCLabelBMFont::create("WRONG", "allfont.fnt");
																			result->setPosition(ccp(240, 160));
																			addChild(result);
																			m_menu->setTouchEnabled(false);
																			addChild(KSTimer::create(2.f, [=]()
																															 {
																																 m_menu->setVisible(false);
																																 quiz->setVisible(false);
																																 resultMark->setVisible(false);
																																 CCLabelTTF* confirmPlz = CCLabelTTF::create("확인해보세요.", "", 14.f);
																																 confirmPlz->setPosition(ccp(190, 280));
																																 m_thiz->addChild(confirmPlz);
																																 for(auto i : m_objects)
																																 {
																																	 i->setVisible(true);
																																 }
																																 addChild(KSTimer::create(7.f, [=]()
																																													{
																																														CCSprite* failSprite = CCSprite::create("bonusgame_fail.png");
																																														failSprite->setPosition(ccp(240, 160));
																																														addChild(failSprite);
																																														addChild(KSTimer::create(2.f, [=]()
																																																										 {
																																																											 m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent));
																																																										 }));
																																													}));
																															 }));
																			unscheduleUpdate();
																		}
																	});
																 item1->setPosition(examplePositions[i]);
																 m_menu->addChild(item1);
															 }
//															 m_menu->alignItemsHorizontallyWithPadding(70.f);
//															 m_menu->alignItemsHorizontally();
														 }));
	}
}

