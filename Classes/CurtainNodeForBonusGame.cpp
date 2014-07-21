#include "CurtainNodeForBonusGame.h"

CurtainNodeForBonusGame::CurtainNodeForBonusGame()
{
//	m_titleBonusGame = nullptr;
	m_titleStr = nullptr;
//	m_contentBack = nullptr;
	 m_startMenu = nullptr;
	m_onPressStartButton = nullptr;
//	m_obtainReward = nullptr;
}
CurtainNodeForBonusGame::~CurtainNodeForBonusGame()
{
	CCLOG("~Curtain");
}
void CurtainNodeForBonusGame::registerWithTouchDispatcher()
{
CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
pDispatcher->addTargetedDelegate(this, m_touchPriority, true);
}

bool CurtainNodeForBonusGame::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

	return true;
}

bool CurtainNodeForBonusGame::init()
{
	CCLayer::init();
	
	return true;
}


void CurtainNodeForBonusGame::menuAction(std::function<void(void)> callback)
{
	addChild(KSGradualValue<CCPoint>::create(m_leftCurtain->getPosition(), ccp(-80 - 20, 160), 0.3f, [=](CCPoint t){
		m_leftCurtain->setPosition(t);
	}, [=](CCPoint t){
		m_leftCurtain->setPosition(t);
	}));
	addChild(KSGradualValue<CCPoint>::create(m_rightCurtain->getPosition(), ccp(560 + 20, 160), 0.3f, [=](CCPoint t){
		m_rightCurtain->setPosition(t);
	}, [=](CCPoint t){
		m_rightCurtain->setPosition(t);
		if(callback)
		{
			callback();
		}
		removeFromParent();
	}));
	
	
//	if(m_titleBonusGame)
//	{
//		CCMoveTo* title_move = CCMoveTo::create(0.3f, ccp(240, 500));
//		m_titleBonusGame->runAction(title_move);
//	}
	
	if(m_titleStr)
	{
		CCMoveTo* title_move2 = CCMoveTo::create(0.3f, ccp(240, 500));
		m_titleStr->runAction(title_move2);
		
	}
	
//	if(m_contentBack)
//	{
//		CCMoveTo* content_move = CCMoveTo::create(0.3f, ccp(-190, 150));
//		m_contentBack->runAction(content_move);
//	}
	
	if(m_startMenu)
	{
		
		addChild(KSGradualValue<CCPoint>::create(m_startMenu->getPosition(), ccp(240, -200), 0.4f,
																						 [=](CCPoint t){
																							 m_startMenu->setPosition(t);
																						 },
																						 [=](CCPoint t){
																							 m_startMenu->setPosition(t);
																							 if(callback)
																							 {
																								 callback();
																							 }
																						 }));
	}
	
//	if(m_obtainReward)
//	{
//		
//		addChild(KSGradualValue<CCPoint>::create(m_obtainReward->getPosition(), ccp(240, -200), 0.4f,
//																						 [=](CCPoint t){
//																							 m_obtainReward->setPosition(t);
//																						 },
//																						 [=](CCPoint t){
//																							 m_obtainReward->setPosition(t);
//																							 if(callback)
//																							 {
//																								 callback();
//																							 }
//																						 }));
//	}
	
	//		CCMoveTo* menu_move = CCMoveTo::create(0.3f, ccp(240, -200));
	//		CCCallFunc* menu_call = CCCallFunc::create(this, callfunc_selector(CurtainNodeForBonusGame::startGame));
	//		CCSequence* menu_seq = CCSequence::createWithTwoActions(menu_move, menu_call);
	//		m_startMenu->runAction(menu_seq);
	// 커튼을 치기 전에 미리 생성해놈.
	if(m_onPressStartButton)
		m_onPressStartButton();
}
