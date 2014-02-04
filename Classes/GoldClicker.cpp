#include "GoldClicker.h"
#include "ProbSelector.h"
#include "cocos2d.h"
GoldClicker::GoldClicker()
{
}
GoldClicker::~GoldClicker()
{
}
void GoldClicker::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, m_priority, true);
}

bool GoldClicker::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	if(m_startMine)
	{
		m_hamerManager->runAnimationsForSequenceNamed("go");
		ProbSelector ps{1, 9, 4};
		
		m_stone->setScale(0.95f);
		
		int result = ps.getResult();
		if(result == 0)
		{
			CCParticleSystemQuad* particle = CCParticleSystemQuad::create("minigame_ruby.plist");
			particle ->setPositionType(kCCPositionTypeGrouped);
			addChild(particle);
			particle->setAutoRemoveOnFinish(true);
			particle->setPosition(ccp(240, 160));
			
			m_obtainRuby++;
			m_obtainRubyFnt->setString(CCString::createWithFormat("%d", m_obtainRuby)->getCString());
		}
		else if(result == 1)
		{
			CCParticleSystemQuad* particle = CCParticleSystemQuad::create("minigame_gold.plist");
		
			particle ->setPositionType(kCCPositionTypeGrouped);
			addChild(particle);
			particle->setAutoRemoveOnFinish(true);
			particle->setPosition(ccp(240, 160));
			m_obtainGold++;
			m_obtainGoldFnt->setString(CCString::createWithFormat("%d", m_obtainGold)->getCString());
		}
		else if(result == 2)
		{
			CCParticleSystemQuad* particle = CCParticleSystemQuad::create("minigame_social.plist");
			particle ->setPositionType(kCCPositionTypeGrouped);
			addChild(particle);
			particle->setAutoRemoveOnFinish(true);
			particle->setPosition(ccp(240, 160));
			m_obtainSocial++;
			m_obtainSocialFnt->setString(CCString::createWithFormat("%d", m_obtainSocial)->getCString());
		}

		CCLog("mine!");
	}
	return true;
}


void GoldClicker::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	m_stone->setScale(1.0f);
}

