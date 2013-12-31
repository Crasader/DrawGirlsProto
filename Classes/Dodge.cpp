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
		
		if(child->m_bullet->getPosition().x < 20 || child->m_bullet->getPosition().x > dodgeScreenSize.x + 20 ||
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
		CCPoint goalPosition = m_player->getPosition() + displacement;
		m_player->setPosition(ccp(clampf(goalPosition.x, 25, 366), clampf(goalPosition.y, 320 - 297, 320 - 24)));
			
	}
}
bool Dodge::init(int priority, const std::function<void(void)>& hideFunction)
{
	CCLayer::init();
// setup stencil shape
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
	
	// setup content
	m_hideFunction = hideFunction;
	m_priority = priority;
	// 480 - 143 = 337
	setTouchEnabled(true);
	m_readyFnt = CCLabelTTF::create("준비", mySGD->getFont().c_str(), 25);
	m_readyFnt->setPosition(dodgeScreenSize / 2.f);
	addChild(m_readyFnt);
	
	m_player = DodgePlayer::create();
	m_thiz->addChild(m_player);
	m_player->setPosition(dodgeScreenSize / 2.f);
	
	m_bulletContainer = BulletContainer::create();
	m_thiz->addChild(m_bulletContainer);
	
	m_bulletCountGoal = 10;
	m_bulletCount = 0;
	m_timer = 0;
	schedule(schedule_selector(Dodge::checkCollision));
	
//	CCSprite* back = CCSprite::create("dodgeback.png");
//	back->setPosition(ccp(480, 160));
//	back->setAnchorPoint(ccp(1.f, 0.5f));
//	addChild(back, 100);
	
	m_flowTimeFnt = CCLabelBMFont::create("0", "etc_font.fnt");
	m_flowTimeFnt->setPosition(ccp(dodgeScreenSize.x + 100, 270));
	addChild(m_flowTimeFnt, 101);
	
	return true;
}

void Dodge::update(float dt)
{
	m_timer += dt;
	bool clear = false;
	if(m_remainTime - m_timer <= 0.f)
	{
		clear = true;
		unscheduleUpdate();
		unschedule(schedule_selector(Dodge::checkCollision));
		CCSprite* successSprite = CCSprite::create("bonusgame_succes.png");
		successSprite->setPosition(ccp(240, 160));
		addChild(successSprite);
		mySGD->setStar(mySGD->getStar() + 1);
		myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v)
												{
													addChild(KSTimer::create(3.f, [=](){
														m_hideFunction();
													}));
												});
	}
	m_flowTimeFnt->setString(CCString::createWithFormat("%.1f", MAX(0, m_remainTime - m_timer))->getCString());
	m_bulletCountGoal = 10 + m_timer / 1.f;
	// 충돌 처리.
	ProbSelector ps = {1,40};
	if(ps.getResult() == 0) // 되도록이면 한번에 생성..
	{
		while(m_bulletContainer->getChildrenCount() < m_bulletCountGoal && !clear)
		{
			float x, y;
			
			if(m_well512.GetPlusMinus() > 0)
			{
				x = m_well512.GetFloatValue(-10, dodgeScreenSize.x + 10);
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
				y = m_well512.GetFloatValue(-10, dodgeScreenSize.y + 10);
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
			CCSprite* failSprite = CCSprite::create("bonusgame_fail.png");
			failSprite->setPosition(ccp(240, 160));
			addChild(failSprite);
			addChild(KSTimer::create(3.f, [=]()
															 {
//																 CCDirector::sharedDirector()->popScene();
																 
																 m_hideFunction();
															 }));
//			unschedule(schedule_selector(Dodge::update));
			
//			onExit();
			
			
			break;
		}
	}
}

