#include "GaBaBo.h"
#include "FormSetter.h"
#include "CommonButton.h"
#include "ks19937.h"
#include "ProbSelector.h"
#include "KSLabelTTF.h"
#include <boost/format.hpp>

GaBaBo::GaBaBo()
{
	
}
GaBaBo::~GaBaBo()
{
}
//void GaBaBo::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool GaBaBo::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool GaBaBo::init()
{
	CCLayer::init();
	CCSprite* bg = CCSprite::create("gababo_bg.png");

	m_step = 1;
	m_stepSprite = nullptr;
	bg->setPosition(ccp(240, 160));

	addChild(bg);


	CCSprite* leftTop = CCSprite::create("gababo_lefttop.png");

	addChild(leftTop);
	leftTop->setPosition(ccp( 50, 250));


	loadImage(m_step);

	CCSprite* think = CCSprite::create("gababo_think.png");
	addChild(think, 1);
	think->setPosition(ccp(330, 220));

	CCMenuLambda* gababo = CCMenuLambda::create();
	gababo->setPosition(CCPointZero);
	addChild(gababo, 1);
	
	auto allInActiver = [=](){
		m_ba->setSelectedIndex(0);
		m_ga->setSelectedIndex(0);
		m_bo->setSelectedIndex(0);
	};
	m_ba = CCMenuItemToggleLambda::createWithTarget([=](CCObject* obj){
		allInActiver();
		m_ba->setSelectedIndex(1);
	}, CCMenuItemImageLambda::create("ba_inactive.png", "ba_inactive.png", nullptr),
		CCMenuItemImageLambda::create("ba_active.png", "ba_active.png", nullptr));
	gababo->addChild(m_ba, 1);
	m_ba->setPosition(ccp(140, 50));


	m_ga = CCMenuItemToggleLambda::createWithTarget([=](CCObject* obj){
		allInActiver();
		m_ga->setSelectedIndex(1);
	}, CCMenuItemImageLambda::create("ga_inactive.png", "ga_inactive.png", nullptr),
		CCMenuItemImageLambda::create("ga_active.png", "ga_active.png", nullptr));
	gababo->addChild(m_ga, 1);
	m_ga->setPosition(ccp(240, 50));


	m_bo = CCMenuItemToggleLambda::createWithTarget([=](CCObject* obj){
		allInActiver();
		m_bo->setSelectedIndex(1);
	}, CCMenuItemImageLambda::create("bo_inactive.png", "bo_inactive.png", nullptr),
		CCMenuItemImageLambda::create("bo_active.png", "bo_active.png", nullptr));
	gababo->addChild(m_bo, 1);
	m_bo->setPosition(ccp(340, 50));


	m_ba->setSelectedIndex(1);
	
	m_computerThinkSprites[kAttackGa] = CCSprite::create("ga.png");
	m_computerThinkSprites[kAttackBa] = CCSprite::create("ba.png");
	m_computerThinkSprites[kAttackBo] = CCSprite::create("bo.png");
	for(auto i : m_computerThinkSprites)
	{
		addChild(i.second, 1); // 일단 붙임.
		i.second->setPosition(ccp(350, 230));
		i.second->setVisible(false);
	}
	m_computerThink = 1; // 가위로 세팅	
	m_computerThinkSprites[m_computerThink]->setVisible(true);

	initGameTime();
	m_resultShowing = false;
	
	m_remainTimeFnt = CCLabelBMFont::create("", "allfont.fnt");
	addChild(m_remainTimeFnt, 1);
	m_remainTimeFnt->setPosition(ccp(30, 260));

	m_lastChangeTime = getCurrentTimeStamp();

	
	scheduleUpdate();
	return true;
}
int GaBaBo::getCurrentTimeStamp()
{
	auto duration = chrono::system_clock::now().time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();


	
	return millis;
}
void GaBaBo::loadImage(int step)
{
	if(m_stepSprite != nullptr)
		m_stepSprite->removeFromParent();
	m_stepSprite = CCSprite::create(boost::str(boost::format("ga%||.png") % step).c_str());
	m_stepSprite->setPosition(ccp(240, 160));
	addChild(m_stepSprite);
}

void GaBaBo::update(float dt)
{
	ProbSelector ps;
	ps.pushProb(1);
	ps.pushProb(100);
	if(ps.getResult() == 0 && m_lastChangeTime + 1 < getCurrentTimeStamp())
	{
		m_lastChangeTime = getCurrentTimeStamp();
		do
		{
			int v = ks19937::getIntValue(1, 3);
			int oldThink = m_computerThink;
			m_computerThink = v;
			if(oldThink != v)
				break;
		}while(1);
		for(auto i : m_computerThinkSprites)
		{
			i.second->setVisible(false);
		}
		m_computerThinkSprites[m_computerThink]->setVisible(true);
	}

	m_remainTime -= dt;
	m_remainTimeFnt->setString(boost::str(boost::format("%||") % m_remainTime).c_str());
	if(m_remainTime <= 0 && m_resultShowing == false)
	{
		m_resultShowing = true;
		m_ba->setEnabled(false);
		m_ga->setEnabled(false);
		m_bo->setEnabled(false);
		int mySelect = 1;
		if(m_ba->getSelectedIndex() == 1)
		{
			mySelect = kAttackBa;
		}
		if(m_ga->getSelectedIndex() == 1)
		{
			mySelect = kAttackGa;
		}
		if(m_bo->getSelectedIndex() == 1)
		{
			mySelect = kAttackBo;
		}

		int D = mySelect - m_computerThink % 3;
		std::string resultString;
		int gameResult = 1;
		if(mySelect == m_computerThink) // Draw
		{
			resultString = "Draw";
			gameResult = 1;
		}
		else if(D == 1) // Win
		{
			resultString = "You Win";
			gameResult = 2;
		}
		else // Lose
		{
			resultString = "You Lose";
			gameResult = 3;
		}
		KSLabelTTF* result = KSLabelTTF::create(resultString.c_str(), mySGD->getFont().c_str(), 60.f);
		result->setPosition(ccp(240, 180));
		addChild(result, 1);
		result->setRotation(30);
		result->CCLabelTTF::setFontFillColor(ccc3(255, 0, 0));
		result->enableStroke(ccc3(255, 255, 0), 2.f);

		addChild(KSTimer::create(3.f, [=](){
			
			// Draw
			if(gameResult == 1)
			{
				result->removeFromParent();
				initGameTime();
				m_resultShowing = false;
				m_ba->setEnabled(true);
				m_ga->setEnabled(true);
				m_bo->setEnabled(true);
				scheduleUpdate();
			}
			// Win
			else if(gameResult == 2)
			{
				CCSprite* light = KS::loadCCBI<CCSprite*>(this, "e_1.ccbi").first;
				light->setPosition(ccp(240, 160));
				addChild(light, 100);
				addChild(KSTimer::create(1.2f, [=](){
					m_ba->setEnabled(true);
					m_ga->setEnabled(true);
					m_bo->setEnabled(true);
					result->removeFromParent();
					initGameTime();
					m_resultShowing = false;
					m_step++;
					this->loadImage(m_step);
					scheduleUpdate();
				}));
				
			}
			// Lose
			else
			{
				CCScale9Sprite* main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
				main_case->setContentSize(CCSizeMake(480, 280));
			}
		}));
		unscheduleUpdate();
	}
}
