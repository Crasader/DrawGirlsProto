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
	
	back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));// CCSprite::create("start_map_gacha_back.png");
	back->setContentSize(CCSizeMake(200, 50));
	back->setAnchorPoint(ccp(0.5f,1.f));
	back->setPosition(ccp(240,0));
	addChild(back, kSMG_Z_back);
	
	
	CCLabelTTF* g_label = CCLabelTTF::create();
	
	KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_areaReGacha), mySGD->getFont().c_str(), 11);
	gacha_label->enableOuterStroke(ccBLACK, 1.f);
	gacha_label->setPosition(ccp(-35,0));
	g_label->addChild(gacha_label);
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("common_button_yellowdown.png", CCRectMake(0,0,34,34), CCRectMake(16,16,2,2));
	price_back->setContentSize(CCSizeMake(75, 33));
	price_back->setPosition(ccp(38, 0));
	g_label->addChild(price_back);
	
	if(mySGD->getGoodsValue(kGoodsType_pass2) > 0)
	{
		CCSprite* pass_ticket = CCSprite::create("pass_ticket2.png");
		pass_ticket->setPosition(ccp(20,16.f));
		price_back->addChild(pass_ticket);
		
		KSLabelTTF* free_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_free), mySGD->getFont().c_str(), 11);
		free_label->enableOuterStroke(ccBLACK, 1.f);
		free_label->setPosition(ccp(49,16));
		price_back->addChild(free_label);
	}
	else
	{
		CCSprite* price_type = CCSprite::create("ui_gold_img.png");
		price_type->setPosition(ccp(15,15));
		price_back->addChild(price_type);
		
		KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d", mySGD->getGachaMapFee())->getCString()).c_str(), mySGD->getFont().c_str(), 15);
		price_label->enableOuterStroke(ccBLACK, 1.f);
		price_label->setPosition(ccp(47,15));
		price_back->addChild(price_label);
	}
	
	CCScale9Sprite* gacha_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	gacha_button = CCControlButton::create(g_label, gacha_back);
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(StartMapGacha::gachaAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(160,40));
	gacha_button->setPosition(ccp(85,25));
	back->addChild(gacha_button);
	
	gacha_button->setTouchPriority(-170);
	
	
	
//	CCSprite* n_gacha = CCSprite::create("start_map_gacha_button.png");
//	CCSprite* n_gacha_price_type = CCSprite::create("price_gold_img.png");
//	n_gacha_price_type->setPosition(ccp(n_gacha->getContentSize().width/2.f+10, n_gacha->getContentSize().height/2.f));
//	n_gacha->addChild(n_gacha_price_type);
//	CCLabelTTF* n_gacha_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaMapFee())->getCString(), mySGD->getFont().c_str(), 15);
//	n_gacha_price_label->setPosition(ccp(n_gacha->getContentSize().width/2.f+35, n_gacha->getContentSize().height/2.f));
//	n_gacha->addChild(n_gacha_price_label);
//	
//	CCSprite* s_gacha = CCSprite::create("start_map_gacha_button.png");
//	s_gacha->setColor(ccGRAY);
//	CCSprite* s_gacha_price_type = CCSprite::create("price_gold_img.png");
//	s_gacha_price_type->setPosition(ccp(s_gacha->getContentSize().width/2.f+10, s_gacha->getContentSize().height/2.f));
//	s_gacha->addChild(s_gacha_price_type);
//	CCLabelTTF* s_gacha_price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaMapFee())->getCString(), mySGD->getFont().c_str(), 15);
//	s_gacha_price_label->setPosition(ccp(s_gacha->getContentSize().width/2.f+35, s_gacha->getContentSize().height/2.f));
//	s_gacha->addChild(s_gacha_price_label);
//	
//	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(StartMapGacha::menuAction));
//	gacha_item->setTag(kSMG_MT_gacha);
//	
//	gacha_menu = CCMenu::createWithItem(gacha_item);
//	gacha_menu->setPosition(getContentPosition(kSMG_MT_gacha));
//	back->addChild(gacha_menu, kSMG_Z_content);
//	gacha_menu->setTouchPriority(-170);
	
	
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
	else if(t_tag == kSMG_MT_cancel)	return_value = ccp(177,25);
	
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
