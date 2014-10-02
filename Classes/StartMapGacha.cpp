// StartMapGacha.cpp
//

#include "StartMapGacha.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"

#define LZZ_INLINE inline
using namespace std;
StartMapGacha * StartMapGacha::create (CCObject * t_gacha, SEL_CallFunc d_gacha)
{
	StartMapGacha* t_ep = new StartMapGacha();
	t_ep->myInit(t_gacha, d_gacha);
	t_ep->autorelease();
	return t_ep;
}
void StartMapGacha::myInit (CCObject * t_gacha, SEL_CallFunc d_gacha)
{
	target_gacha = t_gacha;
	delegate_gacha = d_gacha;
	
	back = CCSprite::create("start_map_gacha_back.png");// CCSprite::create("start_map_gacha_back.png");
	back->setAnchorPoint(ccp(0.5f,1.f));
	back->setPosition(ccp(240,0));
	addChild(back, kSMG_Z_back);
	
	
	CCSprite* n_gacha = CCSprite::create("start_map_gacha_button.png");
	CCSprite* s_gacha = CCSprite::create("start_map_gacha_button.png");
	s_gacha->setColor(ccGRAY);
	
	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(StartMapGacha::menuAction));
	gacha_item->setTag(kSMG_MT_gacha);
	
	gacha_button = CCMenu::createWithItem(gacha_item);
	gacha_button->setPosition(ccp(87,back->getContentSize().height/2.f));
	back->addChild(gacha_button);
	
	gacha_button->setTouchPriority(-170);
	
	CCNode* gacha_node = CCNode::create();
	gacha_node->setPosition(ccpFromSize(back->getContentSize()/2.f));
	back->addChild(gacha_node, 1);
	
	KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_areaReGacha), mySGD->getFont().c_str(), 11);
	gacha_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	gacha_label->setPosition(ccp(-56,0));
	gacha_node->addChild(gacha_label);
	
	CCNode* price_node = CCNode::create();
	price_node->setPosition(ccp(38, 0));
	gacha_node->addChild(price_node);
	
	if(mySGD->getGoodsValue(kGoodsType_pass2) > 0)
	{
		CCSprite* pass_ticket = CCSprite::create("pass_ticket2.png");
		pass_ticket->setPosition(ccp(-40.f,0));
		pass_ticket->setScale(0.7);
		price_node->addChild(pass_ticket);
		
		CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
		pass_count_case->setPosition(ccpFromSize(pass_ticket->getContentSize()/2.f) + ccp(9,6));
		pass_ticket->addChild(pass_count_case);
		
		CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass2)), mySGD->getFont().c_str(), 8);
		pass_count_label->setColor(ccc3(255, 255, 255));
		float min_width = 10+pass_count_label->getContentSize().width;
		if(min_width < 20)
			min_width = 20;
		pass_count_case->setContentSize(CCSizeMake(min_width, 20));
		pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
		pass_count_case->addChild(pass_count_label);
		
		KSLabelTTF* free_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 11);
		free_label->disableOuterStroke();
		free_label->setPosition(ccp(-13.f,0));
		price_node->addChild(free_label);
	}
	else
	{
		CCSprite* price_type = CCSprite::create("ui_gold_img.png");
		price_type->setPosition(ccp(-40.f,0));
		price_type->setScale(0.8);
		price_node->addChild(price_type);
		
		KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d", mySGD->getGachaMapFee())->getCString()).c_str(), mySGD->getFont().c_str(), 11);
		price_label->disableOuterStroke();
		price_label->setPosition(ccp(-13.f,0));
		price_node->addChild(price_label);
	}
	
	
	cancel_menu = CommonButton::createCloseButton();
	cancel_menu->setTouchPriority(-170);
	cancel_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kSMG_MT_cancel);
								 menuAction(t_node);
							 });
	cancel_menu->setPosition(getContentPosition(kSMG_MT_cancel));
	back->addChild(cancel_menu, kSMG_Z_content);
	
	is_menu_enable = true;
	
	CCMoveTo* t_move1 = CCMoveTo::create(0.6f, ccp(240,60));
	CCDelayTime* t_delay = CCDelayTime::create(1.5f);
	CCMoveTo* t_move2 = CCMoveTo::create(0.6f, ccp(240,0));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartMapGacha::removeFromParent));
	CCAction* t_seq = CCSequence::create(t_move1, t_delay, t_move2, t_call, NULL);
	
	back->runAction(t_seq);
}
CCPoint StartMapGacha::getContentPosition (int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kSMG_MT_gacha)			return_value = ccp(78,29);
	else if(t_tag == kSMG_MT_cancel)	return_value = ccp(187,back->getContentSize().height/2.f);
	
	return return_value;
}
void StartMapGacha::gachaAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	if(mySGD->is_paused)
		return;
	
	is_menu_enable = false;
	
	(target_gacha->*delegate_gacha)();
	removeFromParent();
}

void StartMapGacha::outAction()
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	remove_func();
	
	gacha_button->setEnabled(false);
	cancel_menu->setEnabled(false);
	
	back->stopAllActions();
	CCMoveTo* t_move2 = CCMoveTo::create(0.6f, ccp(240,0));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartMapGacha::removeFromParent));
	CCAction* t_seq = CCSequence::create(t_move2, t_call, NULL);
	back->runAction(t_seq);
}

void StartMapGacha::removeFromParent()
{
	remove_func();
	CCLayer::removeFromParent();
}

void StartMapGacha::menuAction (CCObject * sender)
{
	if(!is_menu_enable)
		return;
	if(mySGD->is_paused)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kSMG_MT_gacha)
	{
		(target_gacha->*delegate_gacha)();
		removeFromParent();
	}
	else if(tag == kSMG_MT_cancel)
	{
//		gacha_menu->setEnabled(false);
		gacha_button->setEnabled(false);
		cancel_menu->setEnabled(false);
		
		back->stopAllActions();
		CCMoveTo* t_move2 = CCMoveTo::create(0.6f, ccp(240,0));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StartMapGacha::removeFromParent));
		CCAction* t_seq = CCSequence::create(t_move2, t_call, NULL);
		back->runAction(t_seq);
	}
}
#undef LZZ_INLINE
