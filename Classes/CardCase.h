//
//  CardCase.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 12..
//
//

#ifndef __DGproto__CardCase__
#define __DGproto__CardCase__

#include "cocos2d.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"

USING_NS_CC;

enum CARDCASE_Zorder{
	kCARDCASE_Z_sideCase = 1,
	kCARDCASE_Z_innerCase,
	kCARDCASE_Z_data
};

class CardCase : public CCNode
{
public:
	static CardCase* create(int t_selected_card_number)
	{
		CardCase* t_cc = new CardCase();
		t_cc->myInit(t_selected_card_number);
		t_cc->autorelease();
		return t_cc;
	}
	
	static CardCase* create(int t_card_stage, int t_card_level)
	{
		CardCase* t_cc = new CardCase();
		t_cc->myInit(NSDS_GI(t_card_stage, kSDS_SI_level_int1_card_i, t_card_level));
		t_cc->autorelease();
		return t_cc;
	}
	
private:
	
	void myInit(int t_selected_card_number)
	{
		CCSprite* top_case = CCSprite::create("card_case_top.png");
		top_case->setPosition(ccp(160,430));
		addChild(top_case, kCARDCASE_Z_sideCase);
		
		CCSprite* bottom_case = CCSprite::create("card_case_bottom.png");
		bottom_case->setPosition(ccp(160,0));
		addChild(bottom_case, kCARDCASE_Z_sideCase);
		
		CCSprite* left_case = CCSprite::create("card_case_left.png");
		left_case->setPosition(ccp(0,200));
		addChild(left_case, kCARDCASE_Z_sideCase);
		
		CCSprite* right_case = CCSprite::create("card_case_right.png");
		right_case->setPosition(ccp(320,200));
		addChild(right_case, kCARDCASE_Z_sideCase);
		
		CCSprite* rank_case = CCSprite::create("card_case_rank.png");
		rank_case->setPosition(ccp(30,402));
		addChild(rank_case, kCARDCASE_Z_innerCase);
		
		CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, t_selected_card_number))->getCString(), mySGD->getFont().c_str(), 30);
		rank_label->setAnchorPoint(ccp(0.5,0.5));
		rank_label->setPosition(ccp(rank_case->getContentSize().width/2.f, rank_case->getContentSize().height/2.f-3));
		rank_case->addChild(rank_label, kCARDCASE_Z_data);
		
		CCSprite* durability_case = CCSprite::create("card_case_durability.png");
		durability_case->setPosition(ccp(287,417));
		addChild(durability_case, kCARDCASE_Z_innerCase);
		
		CCLabelTTF* total_durability_label = CCLabelTTF::create(CCString::createWithFormat("/%d", NSDS_GI(kSDS_CI_int1_durability_i, t_selected_card_number))->getCString(), mySGD->getFont().c_str(), 10);
		total_durability_label->setColor(ccBLACK);
		total_durability_label->setAnchorPoint(ccp(1.f,0.5));
		total_durability_label->setPosition(ccp(42,21));
		durability_case->addChild(total_durability_label, kCARDCASE_Z_data);
		
		CCLabelTTF* recent_durability_label = CCLabelTTF::create(CCString::createWithFormat("%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, t_selected_card_number))->getCString(), mySGD->getFont().c_str(), 20);
		recent_durability_label->setColor(ccBLACK);
		recent_durability_label->setAnchorPoint(ccp(0.5,0.5));
		recent_durability_label->setPosition(ccp(20,25));
		durability_case->addChild(recent_durability_label, kCARDCASE_Z_data);
		
		CCSprite* option_case = CCSprite::create("card_case_option.png");
		option_case->setPosition(ccpMult(ccp(65,28), 1.5f));
		option_case->setScale(1.5f);
		addChild(option_case, kCARDCASE_Z_innerCase);
		
		CCProgressTimer* pow_progress = CCProgressTimer::create(CCSprite::create("card_damage_bar.png"));
		pow_progress->setType(kCCProgressTimerTypeBar);
		pow_progress->setMidpoint(ccp(0,0));
		pow_progress->setBarChangeRate(ccp(1,0));
		pow_progress->setPosition(ccp(82,27));
		pow_progress->setPercentage(100.f);
		option_case->addChild(pow_progress);
		
		CCProgressFromTo* pow_action = CCProgressFromTo::create(1.f, 0.f, 100.f);
		pow_progress->runAction(pow_action);
		
		CCProgressTimer* spd_progress = CCProgressTimer::create(CCSprite::create("card_speed_bar.png"));
		spd_progress->setType(kCCProgressTimerTypeBar);
		spd_progress->setMidpoint(ccp(0,0));
		spd_progress->setBarChangeRate(ccp(1,0));
		spd_progress->setPosition(ccp(82,17));
		spd_progress->setPercentage(100.f);
		option_case->addChild(spd_progress);
		
		CCProgressFromTo* spd_action = CCProgressFromTo::create(1.f, 0.f, 100.f);
		spd_progress->runAction(spd_action);
		
		CCProgressTimer* dex_progress = CCProgressTimer::create(CCSprite::create("card_dex_bar.png"));
		dex_progress->setType(kCCProgressTimerTypeBar);
		dex_progress->setMidpoint(ccp(0,0));
		dex_progress->setBarChangeRate(ccp(1,0));
		dex_progress->setPosition(ccp(82,7));
		dex_progress->setPercentage(100.f);
		option_case->addChild(dex_progress);
		
		CCProgressFromTo* dex_action = CCProgressFromTo::create(1.f, 0.f, 100.f);
		dex_progress->runAction(dex_action);
	}
};

#endif /* defined(__DGproto__CardCase__) */
