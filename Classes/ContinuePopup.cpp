//
//  ContinuePopup.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 10..
//
//

#include "ContinuePopup.h"

ContinuePopup* ContinuePopup::create( CCObject* t_end, SEL_CallFunc d_end, CCObject* t_continue, SEL_CallFunc d_continue )
{
	ContinuePopup* t_cpl = new ContinuePopup();
	t_cpl->myInit(t_end, d_end, t_continue, d_continue);
	t_cpl->autorelease();
	return t_cpl;
}

void ContinuePopup::myInit( CCObject* t_end, SEL_CallFunc d_end, CCObject* t_continue, SEL_CallFunc d_continue )
{
	touched_number = 0;
	target_end = t_end;
	delegate_end = d_end;
	target_continue = t_continue;
	delegate_continue = d_continue;

	CCSprite* gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,myDSH->ui_center_y));
	gray->setScaleY(myDSH->ui_top/320.f);
	addChild(gray, kCPL_Z_gray);

	CCSprite* cpl_back = CCSprite::create("continue_back.png");
	cpl_back->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(cpl_back, kCPL_Z_back);
	
	CCSprite* price_type = CCSprite::create("price_ruby_img.png");
	price_type->setPosition(ccp(190,myDSH->ui_center_y+35));
	addChild(price_type, kCPL_Z_menu);
	
	CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getPlayContinueFee())->getCString(), mySGD->getFont().c_str(), 16);
	price_label->setAnchorPoint(ccp(0,0.5f));
	price_label->setPosition(ccp(price_type->getContentSize().width/2.f+15,price_type->getContentSize().height/2.f));
	price_type->addChild(price_label);

	CCSprite* n_end = CCSprite::create("continue_end_game.png");
	CCSprite* s_end = CCSprite::create("continue_end_game.png");
	s_end->setColor(ccGRAY);

	CCMenuItem* end_item = CCMenuItemSprite::create(n_end, s_end, this, menu_selector(ContinuePopup::menuAction));
	end_item->setTag(kCPL_MT_end);

	end_menu = CCMenu::createWithItem(end_item);
	end_menu->setPosition(ccp(175,myDSH->ui_center_y-45));
	addChild(end_menu, kCPL_Z_menu);

	if(mySGD->getStar() >= mySGD->getPlayContinueFee())
	{
		CCSprite* n_continue = CCSprite::create("continue_ok.png");
		CCSprite* s_continue = CCSprite::create("continue_ok.png");
		s_continue->setColor(ccGRAY);

		CCMenuItem* continue_item = CCMenuItemSprite::create(n_continue, s_continue, this, menu_selector(ContinuePopup::menuAction));
		continue_item->setTag(kCPL_MT_continue);

		continue_menu = CCMenu::createWithItem(continue_item);
		continue_menu->setPosition(ccp(305,myDSH->ui_center_y-45));
		addChild(continue_menu, kCPL_Z_menu);
	}
	else
	{
		CCSprite* n_disable = CCSprite::create("continue_ok.png");
		n_disable->setColor(ccc3(100,100,100));
		CCSprite* s_disable = CCSprite::create("continue_ok.png");
		s_disable->setColor(ccc3(100,100,100));

		CCMenuItem* continue_item = CCMenuItemSprite::create(n_disable, s_disable, this, menu_selector(ContinuePopup::menuAction));
		continue_item->setTag(kCPL_MT_continue);

		continue_menu = CCMenu::createWithItem(continue_item);
		continue_menu->setEnabled(false);
		continue_menu->setPosition(ccp(305,myDSH->ui_center_y-45));
		addChild(continue_menu, kCPL_Z_menu);
	}

	is_menu_enable = true;

	setTouchEnabled(true);

	AudioEngine::sharedInstance()->setAppBack();
	mySGD->is_paused = true;
	CCDirector::sharedDirector()->pause();
}

void ContinuePopup::menuAction( CCObject* sender )
{
	if(!is_menu_enable)
		return;

	is_menu_enable = false;
	touched_number = -1;

	int tag = ((CCNode*)sender)->getTag();

	if(tag == kCPL_MT_end)
	{
		(target_end->*delegate_end)();
		removeAction();
		removeFromParent();
	}
	else if(tag == kCPL_MT_continue)
	{
		myLog->addLog(kLOG_action_continue, -1);

		end_menu->setEnabled(false);
		continue_menu->setEnabled(false);

		removeAction();

		mySGD->setStar(mySGD->getStar() - mySGD->getPlayContinueFee());
		myDSH->saveUserData({kSaveUserData_Key_star}, nullptr);

		CCSprite* price_type = CCSprite::create("price_ruby_img.png");
		price_type->setOpacity(0);
		price_type->setPosition(ccpAdd(continue_menu->getPosition(), ccp(-15, 0)));
		addChild(price_type, kCPL_Z_effect);
		
		CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("-%d", mySGD->getPlayContinueFee())->getCString(), mySGD->getFont().c_str(), 16);
		price_label->setOpacity(0);
		price_label->setAnchorPoint(ccp(0,0.5f));
		price_label->setPosition(ccp(price_type->getContentSize().width/2.f+15,price_type->getContentSize().height/2.f));
		price_type->addChild(price_label);

		CCFadeTo* t_fade1 = CCFadeTo::create(0.2f, 255);
		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
		CCFadeTo* t_fade2 = CCFadeTo::create(1.f, 0);
		CCSequence* t_seq = CCSequence::create(t_fade1, t_delay1, t_fade2, NULL);

		CCMoveBy* t_move1 = CCMoveBy::create(1.4f, ccp(0,70));

		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_seq, t_move1);
		CCDelayTime* t_delay2 = CCDelayTime::create(0.5f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinuePopup::endContinueEffect));
		CCSequence* t_seq2 = CCSequence::create(t_spawn, t_delay2, t_call, NULL);

		price_type->runAction(t_seq2);
		
		CCFadeTo* t_fade3 = CCFadeTo::create(0.2f, 255);
		CCDelayTime* t_delay3 = CCDelayTime::create(0.2f);
		CCFadeTo* t_fade4 = CCFadeTo::create(1.f, 0);
		CCSequence* t_seq3 = CCSequence::create(t_fade3, t_delay3, t_fade4, NULL);
		price_label->runAction(t_seq3);
	}
}

void ContinuePopup::endContinueEffect()
{
	(target_continue->*delegate_continue)();
	removeFromParent();
}

void ContinuePopup::removeAction()
{
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
	CCDirector::sharedDirector()->resume();
}

bool ContinuePopup::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number != 0)		return true;
	if(end_menu->ccTouchBegan(pTouch, pEvent))					touched_number = kCPL_MT_end;
	else if(continue_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kCPL_MT_continue;
	return true;
}

void ContinuePopup::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kCPL_MT_end)							end_menu->ccTouchMoved(pTouch, pEvent);
	else if(touched_number == kCPL_MT_continue)					continue_menu->ccTouchMoved(pTouch, pEvent);
}

void ContinuePopup::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kCPL_MT_end){							end_menu->ccTouchEnded(pTouch, pEvent);			touched_number = 0;		}
	else if(touched_number == kCPL_MT_continue){				continue_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;		}
}

void ContinuePopup::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kCPL_MT_end){							end_menu->ccTouchEnded(pTouch, pEvent);			touched_number = 0;		}
	else if(touched_number == kCPL_MT_continue){				continue_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;		}
}

void ContinuePopup::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
