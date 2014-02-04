// OnePercentGacha.cpp
//

#include "OnePercentGacha.h"
#define LZZ_INLINE inline
using namespace std;
OnePercentGacha * OnePercentGacha::create (CCObject * t_cancel, SEL_CallFunc d_cancel, CCObject * t_ok, SEL_CallFuncF d_ok, float t_recent_percent)
{
	OnePercentGacha* t_tnp = new OnePercentGacha();
	t_tnp->myInit(t_cancel, d_cancel, t_ok, d_ok, t_recent_percent);
	t_tnp->autorelease();
	return t_tnp;
}
void OnePercentGacha::myInit (CCObject * t_cancel, SEL_CallFunc d_cancel, CCObject * t_ok, SEL_CallFuncF d_ok, float t_recent_percent)
{
	recent_percent = t_recent_percent;
	target_cancel = t_cancel;
	delegate_cancel = d_cancel;
	target_ok = t_ok;
	delegate_ok = d_ok;
	
	back_img = NULL;
	cancel_menu = NULL;
	ok_menu = NULL;
	
	CCSprite* gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,myDSH->ui_center_y));
	gray->setScaleY(myDSH->ui_top/320.f);
	addChild(gray, kOnePercentGacha_Z_gray);
	
	setBack();
	
	is_menu_enable = true;
	
	touched_number = 0;
	
	setTouchEnabled(true);
}
void OnePercentGacha::gachaOn ()
{
	CCMoveTo* t_move1 = CCMoveTo::create(0.05f, ccpAdd(ccp(240, myDSH->ui_center_y+25.f), ccp(111,0)));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(OnePercentGacha::repeatAction));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move1, t_call);
	arrow_img->runAction(t_seq);
}
void OnePercentGacha::repeatAction ()
{
	is_menu_enable = true;
	ok_menu->setEnabled(true);
	CCMoveTo* t_move2 = CCMoveTo::create(0.1f, ccpAdd(ccp(240, myDSH->ui_center_y+25.f), ccp(-111,0)));
	CCMoveTo* t_move3 = CCMoveTo::create(0.1f, ccpAdd(ccp(240, myDSH->ui_center_y+25.f), ccp(111,0)));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_move3);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
	arrow_img->runAction(t_repeat);
}
void OnePercentGacha::menuAction (CCObject * sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kOnePercentGacha_MT_ok)
	{
		myLog->addLog(kLOG_gacha_onePercent, -1);
		mySGD->setStar(mySGD->getStar() - mySGD->getGachaOnePercentFee());
		gachaOn();
		ok_menu->removeFromParent();
		
		CCSprite* n_stop = CCSprite::create("one_percent_gacha_stop.png");
		CCSprite* s_stop = CCSprite::create("one_percent_gacha_stop.png");
		s_stop->setColor(ccGRAY);
		
		CCMenuItem* stop_item = CCMenuItemSprite::create(n_stop, s_stop, this, menu_selector(OnePercentGacha::menuAction));
		stop_item->setTag(kOnePercentGacha_MT_stop);
		
		ok_menu = CCMenu::createWithItem(stop_item);
		ok_menu->setEnabled(false);
		ok_menu->setPosition(ccp(240, myDSH->ui_center_y-65.f));
		addChild(ok_menu, kOnePercentGacha_Z_content);
		
		cancel_menu->setEnabled(false);
		//			(target_ok->*delegate_ok)(recent_percent);
	}
	else if(tag == kOnePercentGacha_MT_cancel)
	{
		(target_cancel->*delegate_cancel)();
		removeFromParent();
	}
	else if(tag == kOnePercentGacha_MT_stop)
	{
		ok_menu->setEnabled(false);
		arrow_img->stopAllActions();
		float position_value = (arrow_img->getPositionX() - 129.f)/222.f;
		recent_percent += position_value/100.f;
		if(recent_percent > 1.f)
			recent_percent = 1.f;
		
		CCProgressFromTo* t_progress_action = CCProgressFromTo::create(1.f, 0.f, position_value*100.f);
		CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(OnePercentGacha::changePercentLabel));
		CCDelayTime* t_delay2 = CCDelayTime::create(3.f);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(OnePercentGacha::endAction));
		CCSequence* t_seq = CCSequence::create(t_delay1, t_call1, t_delay2, t_call2, NULL);
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_progress_action, t_seq);
		progress_img->runAction(t_spawn);
	}
}
void OnePercentGacha::endAction ()
{
	(target_ok->*delegate_ok)(recent_percent);
	removeFromParent();
}
void OnePercentGacha::changePercentLabel ()
{
	percent_label->setString(CCString::createWithFormat("%.1f", recent_percent*100.f)->getCString());
}
void OnePercentGacha::setBack ()
{
	back_img = CCSprite::create("one_percent_gacha_back.png");
	back_img->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(back_img, kOnePercentGacha_Z_back);
	
	percent_label = CountingBMLabel::create(CCString::createWithFormat("%.1f", recent_percent*100.f)->getCString(), "bb_white_font.fnt", 0.5f, "%.1f");
	percent_label->setPosition(ccp(305, myDSH->ui_center_y+87.f));
	addChild(percent_label, kOnePercentGacha_Z_content);
	
	progress_img = CCProgressTimer::create(CCSprite::create("one_percent_gacha_graph.png"));
	progress_img->setType(kCCProgressTimerTypeBar);
	progress_img->setMidpoint(ccp(0,0));
	progress_img->setBarChangeRate(ccp(1,0));
	progress_img->setPercentage(0.f);
	progress_img->setPosition(ccp(240, myDSH->ui_center_y+10.f));
	addChild(progress_img, kOnePercentGacha_Z_content);
	
	arrow_img = CCSprite::create("one_percent_gacha_arrow.png");
	arrow_img->setPosition(ccp(240, myDSH->ui_center_y+25.f));
	addChild(arrow_img, kOnePercentGacha_Z_content);
	
	CCSprite* n_ok = CCSprite::create("one_percent_gacha_button.png");
	CCSprite* n_price_type = CCSprite::create("price_ruby_img.png");
	n_price_type->setPosition(ccp(n_ok->getContentSize().width/2.f+15, n_ok->getContentSize().height/2.f));
	n_ok->addChild(n_price_type);
	CCLabelTTF* n_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaOnePercentFee())->getCString(), mySGD->getFont().c_str(), 15);
	n_price_label->setPosition(ccp(n_ok->getContentSize().width/2.f+40, n_ok->getContentSize().height/2.f));
	n_ok->addChild(n_price_label);
	
	CCSprite* s_ok = CCSprite::create("one_percent_gacha_button.png");
	s_ok->setColor(ccGRAY);
	CCSprite* s_price_type = CCSprite::create("price_ruby_img.png");
	s_price_type->setPosition(ccp(s_ok->getContentSize().width/2.f+15, s_ok->getContentSize().height/2.f));
	s_ok->addChild(s_price_type);
	CCLabelTTF* s_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaOnePercentFee())->getCString(), mySGD->getFont().c_str(), 15);
	s_price_label->setPosition(ccp(s_ok->getContentSize().width/2.f+40, s_ok->getContentSize().height/2.f));
	s_ok->addChild(s_price_label);
	
	CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(OnePercentGacha::menuAction));
	ok_item->setTag(kOnePercentGacha_MT_ok);
	ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setPosition(ccp(240, myDSH->ui_center_y-65.f));
	addChild(ok_menu, kOnePercentGacha_Z_content);
	
	
	CCSprite* n_cancel = CCSprite::create("one_percent_gacha_cancel.png");
	CCSprite* s_cancel = CCSprite::create("one_percent_gacha_cancel.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(OnePercentGacha::menuAction));
	cancel_item->setTag(kOnePercentGacha_MT_cancel);
	cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(385, myDSH->ui_center_y+55.f));
	addChild(cancel_menu, kOnePercentGacha_Z_content);
}
bool OnePercentGacha::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number != 0)		return true;
	if(cancel_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kOnePercentGacha_MT_cancel;
	else if(ok_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kOnePercentGacha_MT_ok;
	return true;
}
void OnePercentGacha::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number == kOnePercentGacha_MT_cancel)							cancel_menu->ccTouchMoved(pTouch, pEvent);
	else if(touched_number == kOnePercentGacha_MT_ok)						ok_menu->ccTouchMoved(pTouch, pEvent);
}
void OnePercentGacha::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number == kOnePercentGacha_MT_cancel){			cancel_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
	else if(touched_number == kOnePercentGacha_MT_ok){			ok_menu->ccTouchEnded(pTouch, pEvent);			touched_number = 0;	}
}
void OnePercentGacha::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number == kOnePercentGacha_MT_cancel){			cancel_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
	else if(touched_number == kOnePercentGacha_MT_ok){			ok_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
}
void OnePercentGacha::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
#undef LZZ_INLINE
