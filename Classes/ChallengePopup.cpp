//
//  ChallengePopup.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 1..
//
//

#include "ChallengePopup.h"

ChallengePopup* ChallengePopup::create( CCObject* t_return, SEL_CallFunc d_return )
{
	ChallengePopup* t_cp = new ChallengePopup();
	t_cp->myInit(t_return, d_return);
	t_cp->autorelease();
	return t_cp;
}

void ChallengePopup::myInit( CCObject* t_return, SEL_CallFunc d_return )
{
	target_return = t_return;
	delegate_return = d_return;

	CCSprite* gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,160));
	addChild(gray, kCP_Z_gray);

	CCSprite* back = CCSprite::create("challenge_back.png");
	back->setPosition(ccp(240,160));
	addChild(back, kCP_Z_back);


	CCSprite* n_return = CCSprite::create("challenge_return.png");
	CCSprite* s_return = CCSprite::create("challenge_return.png");
	s_return->setColor(ccGRAY);

	CCMenuItem* return_item = CCMenuItemSprite::create(n_return, s_return, this, menu_selector(ChallengePopup::menuAction));
	return_item->setTag(kCP_MT_return);

	return_menu = CCMenu::createWithItem(return_item);
	return_menu->setPosition(getContentPosition(kCP_MT_return));
	addChild(return_menu, kCP_Z_content);

	is_menu_enable = true;

	touched_number = 0;

	setTouchEnabled(true);
}

CCPoint ChallengePopup::getContentPosition( int t_tag )
{
	CCPoint return_value;

	if(t_tag == kCP_MT_return)	return_value = ccp(345,283);

	return return_value;
}

void ChallengePopup::menuAction( CCObject* sender )
{
	if(!is_menu_enable)
		return;

	is_menu_enable = false;

	int tag = ((CCNode*)sender)->getTag();

	if(tag == kCP_MT_return)
	{
		(target_return->*delegate_return)();
	}

	removeFromParent();
}

bool ChallengePopup::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number != 0)		return true;
	if(return_menu->ccTouchBegan(pTouch, pEvent))	touched_number = kCP_MT_return;
	return true;
}

void ChallengePopup::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kCP_MT_return)				return_menu->ccTouchMoved(pTouch, pEvent);
}

void ChallengePopup::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kCP_MT_return){			return_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
}

void ChallengePopup::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kCP_MT_return){			return_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
}

void ChallengePopup::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
