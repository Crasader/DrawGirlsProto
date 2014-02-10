// OnePercentGacha.cpp
//

#include "OnePercentGacha.h"
#include "CommonButton.h"
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
	
	is_menu_enable = false;
	
	touched_number = 0;
	
	setTouchEnabled(true);
	
	startShow();
}
void OnePercentGacha::startShow()
{
	CCMoveTo* title_move = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y+75));
	CCMoveTo* back_move = CCMoveTo::create(0.5f, ccp(240,myDSH->ui_center_y-30));
	CCCallFunc* back_call = CCCallFunc::create(this, callfunc_selector(OnePercentGacha::endShow));
	CCSequence* back_seq = CCSequence::createWithTwoActions(back_move, back_call);
	
	title_img->runAction(title_move);
	back_img->runAction(back_seq);
}
void OnePercentGacha::endShow()
{
	is_menu_enable = true;
}
void OnePercentGacha::gachaOn ()
{
	CCMoveTo* t_move1 = CCMoveTo::create(0.05f, ccpAdd(ccp(back_img->getContentSize().width/2.f, back_img->getContentSize().height/2.f+30.f), ccp(111,0)));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(OnePercentGacha::repeatAction));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move1, t_call);
	arrow_img->runAction(t_seq);
}
void OnePercentGacha::repeatAction ()
{
	is_menu_enable = true;
	ok_menu->setEnabled(true);
	CCMoveTo* t_move2 = CCMoveTo::create(0.1f, ccpAdd(ccp(back_img->getContentSize().width/2.f, back_img->getContentSize().height/2.f+30.f), ccp(-111,0)));
	CCMoveTo* t_move3 = CCMoveTo::create(0.1f, ccpAdd(ccp(back_img->getContentSize().width/2.f, back_img->getContentSize().height/2.f+30.f), ccp(111,0)));
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
		
		
		ok_menu->removeFromParent();
		
		ok_menu = CommonButton::create("멈춰!", 12, CCSizeMake(100, 38), CommonButtonYellow, -180);
		ok_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kOnePercentGacha_MT_stop);
								 menuAction(t_node);
							 });
		ok_menu->setPosition(ccp(back_img->getContentSize().width/2.f+80, back_img->getContentSize().height/2.f-49.f));
		back_img->addChild(ok_menu);
		ok_menu->setEnabled(false);
		
		cancel_menu->setEnabled(false);
		//			(target_ok->*delegate_ok)(recent_percent);
		
		CCSprite* stop_ment = CCSprite::create("one_percent_gacha_stop.png");
		stop_ment->setPosition(ccpAdd(ok_menu->getPosition(), ccp(-stop_ment->getContentSize().width/2.f - 80,0)));
		back_img->addChild(stop_ment);
		
		gachaOn();
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
		float position_value = (arrow_img->getPositionX()+back_img->getPositionX()-back_img->getContentSize().width/2.f - 129.f)/222.f;
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
	back_img->setPosition(ccp(240-480,myDSH->ui_center_y-30));
	addChild(back_img, kOnePercentGacha_Z_back);
	
	title_img = CCSprite::create("one_percent_gacha_title.png");
	title_img->setPosition(ccp(240-480,myDSH->ui_center_y+75));
	addChild(title_img, kOnePercentGacha_Z_back);
	
	percent_label = CountingBMLabel::create(CCString::createWithFormat("%.1f", recent_percent*100.f)->getCString(), "bb_white_font.fnt", 0.5f, "%.1f");
	percent_label->setPosition(ccp(title_img->getContentSize().width/2.f + 65, title_img->getContentSize().height/2.f));
	title_img->addChild(percent_label, kOnePercentGacha_Z_content);
	
	progress_img = CCProgressTimer::create(CCSprite::create("one_percent_gacha_graph.png"));
	progress_img->setType(kCCProgressTimerTypeBar);
	progress_img->setMidpoint(ccp(0,0));
	progress_img->setBarChangeRate(ccp(1,0));
	progress_img->setPercentage(0.f);
	progress_img->setPosition(ccp(back_img->getContentSize().width/2.f, back_img->getContentSize().height/2.f+18.f));
	back_img->addChild(progress_img, kOnePercentGacha_Z_content);
	
	CCLabelTTF* left_percent = CCLabelTTF::create("0%", mySGD->getFont().c_str(), 14);
	left_percent->setAnchorPoint(ccp(0,1));
	left_percent->setPosition(ccpAdd(progress_img->getPosition(), ccp(-progress_img->getSprite()->getContentSize().width/2.f-10, -progress_img->getSprite()->getContentSize().height/2.f-2)));
	back_img->addChild(left_percent, kOnePercentGacha_Z_content);
	
	CCLabelTTF* right_percent = CCLabelTTF::create("1%", mySGD->getFont().c_str(), 14);
	right_percent->setAnchorPoint(ccp(1,1));
	right_percent->setPosition(ccpAdd(progress_img->getPosition(), ccp(progress_img->getSprite()->getContentSize().width/2.f+20, -progress_img->getSprite()->getContentSize().height/2.f-2)));
	back_img->addChild(right_percent, kOnePercentGacha_Z_content);
	
	
	arrow_img = CCSprite::create("one_percent_gacha_arrow.png");
	arrow_img->setPosition(ccp(back_img->getContentSize().width/2.f, back_img->getContentSize().height/2.f+28.f));
	back_img->addChild(arrow_img, kOnePercentGacha_Z_content);
	
	
	ok_menu = CommonButton::create("", CommonButtonYellow);
	ok_menu->setSize(CCSizeMake(100, 38));
	ok_menu->setTouchPriority(-180);
	ok_menu->setPrice(PriceTypeRuby, mySGD->getGachaOnePercentFee());
	ok_menu->setFunction([=](CCObject* sender)
						 {
							 CCNode* t_node = CCNode::create();
							 t_node->setTag(kOnePercentGacha_MT_ok);
							 menuAction(t_node);
						 });
	ok_menu->setPosition(ccp(back_img->getContentSize().width/2.f+80, back_img->getContentSize().height/2.f-49.f));
	back_img->addChild(ok_menu);
	
	
	cancel_menu = CommonButton::createCloseButton();
	cancel_menu->setTouchPriority(-180);
	cancel_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kOnePercentGacha_MT_cancel);
								 menuAction(t_node);
							 });
	cancel_menu->setPosition(ccp(back_img->getContentSize().width/2.f + 145, back_img->getContentSize().height/2.f + 70));
	back_img->addChild(cancel_menu);
}
bool OnePercentGacha::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	return true;
}
void OnePercentGacha::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void OnePercentGacha::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void OnePercentGacha::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void OnePercentGacha::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
#undef LZZ_INLINE
