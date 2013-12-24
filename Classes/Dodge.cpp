//
//  Dodge.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 12. 11..
//
//

#include "Dodge.h"
#include "StarGoldData.h"
#include "KSUtil.h"
#include "ProbSelector.h"

CCPoint dodgeScreenSize = ccp(337, 320);

void BulletContainer::update(float dt)
{
	auto childs = getChildren();
	vector<CCNode*> eraseList;
	//		CCLog("children count %d", getChildrenCount());
	for(int i=0; i<getChildrenCount(); i++)
	{
		DodgeBullet* child = dynamic_cast<DodgeBullet*>(childs->objectAtIndex(i));
		CCAssert(child, "");
		
		if(child->m_bullet->getPosition().x < -20 || child->m_bullet->getPosition().x > dodgeScreenSize.x + 20 ||
			 child->m_bullet->getPosition().y > dodgeScreenSize.y + 20 || child->m_bullet->getPosition().y < -20)
		{
			eraseList.push_back(child);
		}
	}
	
	for(auto i : eraseList)
	{
		i->removeFromParent();
	}
}
bool Dodge::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(m_state == DodgeState::kReady)
	{
		m_state = DodgeState::kPlay;
		m_readyFnt->removeFromParent();
		
		addChild(KSTimer::create(1.5f, [=](){scheduleUpdate();}));
		
	}
	else if(m_state == DodgeState::kResult)
	{
		auto dodge = Dodge::create();
		getParent()->addChild(dodge, getZOrder());
		dodge->setAnchorPoint(ccp(0.5,0));
		dodge->setScale(myDSH->screen_convert_rate);
		dodge->setPosition(ccpAdd(dodge->getPosition(), myDSH->ui_zero_point));
		removeFromParent();
	}
	return true;
}
void Dodge::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(m_state == DodgeState::kPlay)
	{
		CCPoint current = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
		
		CCPoint before = CCDirector::sharedDirector()->convertToGL(pTouch->getPreviousLocationInView());
		auto displacement = (current - before) * 1.75f;
		m_player->setPosition(m_player->getPosition() + displacement);
	}
}
bool Dodge::init()
{
	CCLayer::init();
	// 480 - 143 = 337
	setTouchEnabled(true);
	m_readyFnt = CCLabelTTF::create("준비", mySGD->getFont().c_str(), 25);
	m_readyFnt->setPosition(dodgeScreenSize / 2.f);
	addChild(m_readyFnt);
	
	m_player = DodgePlayer::create();
	addChild(m_player);
	m_player->setPosition(dodgeScreenSize / 2.f);
	
	m_bulletContainer = BulletContainer::create();
	addChild(m_bulletContainer);
	
	m_bulletCountGoal = 10;
	m_bulletCount = 0;
	m_timer = 0;
	schedule(schedule_selector(Dodge::checkCollision));
	
	CCSprite* back = CCSprite::create("dodgeback.png");
	back->setPosition(ccp(480, 160));
	back->setAnchorPoint(ccp(1.f, 0.5f));
	addChild(back, 100);
	
	m_flowTimeFnt = CCLabelBMFont::create("0", "etc_font.fnt");
	m_flowTimeFnt->setPosition(ccp(dodgeScreenSize.x + 100, 200));
	addChild(m_flowTimeFnt, 101);
	
	return true;
}

void Dodge::update(float dt)
{
	m_timer += dt;
	m_flowTimeFnt->setString(CCString::createWithFormat("%.1f", m_timer)->getCString());
	m_bulletCountGoal = 10 + m_timer / 1.f;
	// 충돌 처리.
	ProbSelector ps = {1,40};
	if(ps.getResult() == 0) // 되도록이면 한번에 생성..
	{
		while(m_bulletContainer->getChildrenCount() < m_bulletCountGoal)
		{
			float x, y;
			
			if(m_well512.GetPlusMinus() > 0)
			{
				x = m_well512.GetFloatValue(0, dodgeScreenSize.x);
				if(m_well512.GetPlusMinus() > 0)
				{
					y = dodgeScreenSize.y + 10.f;
				}
				else
				{
					y = -10;
				}
			}
			else
			{
				y = m_well512.GetFloatValue(0, dodgeScreenSize.y);
				if(m_well512.GetPlusMinus() > 0)
				{
					x = dodgeScreenSize.x + 10.f;
				}
				else
				{
					x = -10;
				}
			}
			auto bullet = DodgeBullet::create(ccp(x, y));
			float angle = atan2(m_player->getPosition().y - y, m_player->getPosition().x - x);
			bullet->m_dv = ccp(cos(angle), sin(angle))*m_well512.GetFloatValue(0.7f, 2.5f);// (m_player->getPosition() - ccp(x, y)) / m_well512.GetFloatValue(100.f, 150.f);
			m_bulletContainer->addChild(bullet);
			m_bulletCount++;
		}
	}
	
}


void Dodge::checkCollision(float dt)
{
	auto childs = m_bulletContainer->getChildren();

	//		CCLog("children count %d", getChildrenCount());
	for(int i=0; i<m_bulletContainer->getChildrenCount(); i++)
	{
		DodgeBullet* child = dynamic_cast<DodgeBullet*>(childs->objectAtIndex(i));
		CCAssert(child, "");
		
		m_player->getPosition();
		if(child->m_bullet->boundingBox().containsPoint(m_player->getPosition()))
		{
			m_state = DodgeState::kResult;
			m_player->removeFromParent();
			m_flowTimeFnt->setColor(ccc3(255, 0, 0));
			this->unscheduleAllSelectors();
//			unschedule(schedule_selector(Dodge::update));
			
//			onExit();
			
			
			break;
		}
	}
}

