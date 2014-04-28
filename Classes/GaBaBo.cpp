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
	m_winCount = m_drawCount = m_loseCount = 0;
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
	m_stepSprite->setScale(320.f / m_stepSprite->getContentSize().height);
	m_stepSprite->setPosition(ccp(240, 160));
	addChild(m_stepSprite);
}


void GaBaBo::update(float dt)
{
	m_remainTime -= dt;
	ProbSelector ps;
	ps.pushProb(1);
	if(m_remainTime >= 4)
		ps.pushProb(50);
	else
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
		
		
		CCSprite* result;
		if(mySelect == m_computerThink) // Draw
		{
			resultString = "Draw";
			gameResult = 1;
			result = KS::loadCCBI<CCSprite*>(this, "e_draw.ccbi").first;
			result->setPosition(ccp(240, 180));
			addChild(result, 1);
			m_drawCount++;
		}
		else if(D == 1) // Win
		{
			resultString = "You Win";
			gameResult = 2;
			result = KS::loadCCBI<CCSprite*>(this, "e_win.ccbi").first;
			result->setPosition(ccp(240, 180));
			addChild(result, 1);
			m_winCount++;
		}
		else // Lose
		{
			resultString = "You Lose";
			gameResult = 3;
			result = KS::loadCCBI<CCSprite*>(this, "e_lose.ccbi").first;
			result->setPosition(ccp(240, 180));
			addChild(result, 1);
			m_loseCount++;
		}

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
//				result->removeFromParent();
				CCSprite* light = KS::loadCCBI<CCSprite*>(this, "e_1.ccbi").first;
				light->setPosition(ccp(240, 160));
				addChild(light, 100);
				addChild(KSTimer::create(0.9f, [=](){
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
				
				main_case->setContentSize(CCSizeMake(300, 250));
				main_case->setPosition(ccp(240,160));
				addChild(main_case, 2);
				
				CCScale9Sprite* main_inner = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
				main_inner->setContentSize(CCSizeMake(270, 200));
				main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.44f);
				main_case->addChild(main_inner);
				
				
				CommonButton* closeBtn = CommonButton::createCloseButton(-200);
				closeBtn->setFunction([=](CCObject*){
					//		hspConnector::get()->removeTarget(this);
					//		this->hidePopup();
				});
				closeBtn->setPosition(ccp(275, 225));
				main_case->addChild(closeBtn);
				
				KSLabelTTF* title_label = KSLabelTTF::create("게임결과", mySGD->getFont().c_str(), 17);
				title_label->setPosition(ccp(45,227));
				main_case->addChild(title_label);
				
				KSLabelTTF* winLabel = KSLabelTTF::create(boost::str(boost::format("%|11|") % "WIN : ").c_str(), mySGD->getFont().c_str(), 15.f);
				winLabel->setPosition(ccp(50, 190));
				main_case->addChild(winLabel);
				
				CCLabelBMFont* winCountFnt = CCLabelBMFont::create(boost::str(boost::format("%|#|") % m_winCount).c_str(), "allfont.fnt");
				winCountFnt->setPosition(ccp(200, 190));
				main_case->addChild(winCountFnt);
				
				KSLabelTTF* drawLabel = KSLabelTTF::create(boost::str(boost::format("%|11|") % "DRAW : ").c_str(), mySGD->getFont().c_str(), 15.f);
				drawLabel->setPosition(ccp(50, 160));
				main_case->addChild(drawLabel);
				
				CCLabelBMFont* drawCountFnt = CCLabelBMFont::create(boost::str(boost::format("%|#|") % m_drawCount).c_str(), "allfont.fnt");
				drawCountFnt->setPosition(ccp(200, 160));
				main_case->addChild(drawCountFnt);
				
				KSLabelTTF* loseLabel = KSLabelTTF::create(boost::str(boost::format("%|11|") % "LOSE : ").c_str(), mySGD->getFont().c_str(), 15.f);
				loseLabel->setPosition(ccp(50, 130));
				main_case->addChild(loseLabel);
				
				CCLabelBMFont* loseCountFnt = CCLabelBMFont::create(boost::str(boost::format("%|#|") % m_loseCount).c_str(), "allfont.fnt");
				loseCountFnt->setPosition(ccp(200, 130));
				main_case->addChild(loseCountFnt);

			}
		}));
		unscheduleUpdate();
	}
}
