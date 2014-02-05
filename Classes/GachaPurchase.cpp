//
//  GachaPurchase.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 5..
//
//

#include "GachaPurchase.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "GachaBase.h"
#include "PuzzleMapScene.h"
#include "HatGacha.h"
#include "HorseGacha.h"
#include "SlotMachine.h"
#include "Roulette.h"
#include "MainFlowScene.h"

enum GachaPurchaseZorder{
	kGachaPurchaseZorder_gray = 1,
	kGachaPurchaseZorder_back,
	kGachaPurchaseZorder_realGacha,
	kGachaPurchaseZorder_content
};

enum GachaPurchaseMenuTag{
	kGachaPurchaseMenuTag_close = 1,
	kGachaPurchaseMenuTag_ruby,
	kGachaPurchaseMenuTag_gold,
	kGachaPurchaseMenuTag_candy,
	kGachaPurchaseMenuTag_gachaBase = 10000
};

void GachaPurchase::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

void GachaPurchase::setOutAllObjectAction(CCObject* t_out, SEL_CallFunc d_out)
{
	target_out = t_out;
	delegate_out = d_out;
}

void GachaPurchase::setInAllObjectAction(CCObject* t_in, SEL_CallFunc d_in)
{
	target_in = t_in;
	delegate_in = d_in;
}

GachaPurchase* GachaPurchase::create(GachaPurchaseStartMode start_mode /*= kGachaPurchaseStartMode_select*/,
																		 std::function<void(void)> t_finish)
{
	GachaPurchase* t_gp = new GachaPurchase();
	if(t_gp && t_gp->init(start_mode, t_finish))
	{
		t_gp->autorelease();
		return t_gp;
	}
	CC_SAFE_DELETE(t_gp);
	return NULL;
}

bool GachaPurchase::init(GachaPurchaseStartMode start_mode, std::function<void(void)> t_finish)
{
	if(!CCLayer::init())
	{
		return false;
	}
	finish_function = t_finish;
	recent_mode = start_mode;
	
	is_menu_enable = false;
	is_touch_on = false;
	setTouchEnabled(true);
	
	if(recent_mode == kGachaPurchaseStartMode_select)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, kGachaPurchaseZorder_gray);
		
		main_case = CCSprite::create("gacha_purchase_back.png");
		main_case->setOpacity(255);
		main_case->setPosition(ccp(240,-160));
		addChild(main_case, kGachaPurchaseZorder_back);
		
		CCSprite* n_close = CCSprite::create("cardchange_cancel.png");
		CCSprite* s_close = CCSprite::create("cardchange_cancel.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(GachaPurchase::menuAction));
		close_item->setTag(kGachaPurchaseMenuTag_close);
		
		CCMenu* close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kGachaPurchaseMenuTag_close));
		main_case->addChild(close_menu);
		close_menu->setTouchPriority(-171);
		
		
		CCSprite* n_ruby = CCSprite::create("gacha_purchase_ruby.png");
		CCLabelTTF* n_ruby_price = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaRubyFee())->getCString(), mySGD->getFont().c_str(), 15);
		n_ruby_price->setPosition(ccp(n_ruby->getContentSize().width/2.f, 22));
		n_ruby->addChild(n_ruby_price);
		CCSprite* s_ruby = CCSprite::create("gacha_purchase_ruby.png");
		s_ruby->setColor(ccGRAY);
		CCLabelTTF* s_ruby_price = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaRubyFee())->getCString(), mySGD->getFont().c_str(), 15);
		s_ruby_price->setPosition(ccp(s_ruby->getContentSize().width/2.f, 22));
		s_ruby->addChild(s_ruby_price);
		
		CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(GachaPurchase::menuAction));
		ruby_item->setTag(kGachaPurchaseMenuTag_ruby);
		
		CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
		ruby_menu->setPosition(getContentPosition(kGachaPurchaseMenuTag_ruby));
		main_case->addChild(ruby_menu);
		ruby_menu->setTouchPriority(-171);
		
		CCSprite* n_gold = CCSprite::create("gacha_purchase_gold.png");
		CCLabelTTF* n_gold_price = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaGoldFee())->getCString(), mySGD->getFont().c_str(), 15);
		n_gold_price->setPosition(ccp(n_gold->getContentSize().width/2.f, 22));
		n_gold->addChild(n_gold_price);
		CCSprite* s_gold = CCSprite::create("gacha_purchase_gold.png");
		s_gold->setColor(ccGRAY);
		CCLabelTTF* s_gold_price = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaGoldFee())->getCString(), mySGD->getFont().c_str(), 15);
		s_gold_price->setPosition(ccp(s_gold->getContentSize().width/2.f, 22));
		s_gold->addChild(s_gold_price);
		
		CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(GachaPurchase::menuAction));
		gold_item->setTag(kGachaPurchaseMenuTag_gold);
		
		CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
		gold_menu->setPosition(getContentPosition(kGachaPurchaseMenuTag_gold));
		main_case->addChild(gold_menu);
		gold_menu->setTouchPriority(-171);
		
		CCSprite* n_candy = CCSprite::create("gacha_purchase_candy.png");
		CCLabelTTF* n_candy_price = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaSocialFee())->getCString(), mySGD->getFont().c_str(), 15);
		n_candy_price->setPosition(ccp(n_candy->getContentSize().width/2.f, 22));
		n_candy->addChild(n_candy_price);
		CCSprite* s_candy = CCSprite::create("gacha_purchase_candy.png");
		s_candy->setColor(ccGRAY);
		CCLabelTTF* s_candy_price = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaSocialFee())->getCString(), mySGD->getFont().c_str(), 15);
		s_candy_price->setPosition(ccp(s_candy->getContentSize().width/2.f, 22));
		s_candy->addChild(s_candy_price);
		
		CCMenuItem* candy_item = CCMenuItemSprite::create(n_candy, s_candy, this, menu_selector(GachaPurchase::menuAction));
		candy_item->setTag(kGachaPurchaseMenuTag_candy);
		
		CCMenu* candy_menu = CCMenu::createWithItem(candy_item);
		candy_menu->setPosition(getContentPosition(kGachaPurchaseMenuTag_candy));
		main_case->addChild(candy_menu);
		candy_menu->setTouchPriority(-171);
	}
	else if(recent_mode == kGachaPurchaseStartMode_reward)
	{
		left_curtain = CCSprite::create("curtain_left.png");
		left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
		left_curtain->setPosition(ccp(0, 160));
		addChild(left_curtain, kGachaPurchaseZorder_content);
		
		right_curtain = CCSprite::create("curtain_left.png");
		right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		right_curtain->setFlipX(true);
		right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
		right_curtain->setPosition(ccp(480,160));
		addChild(right_curtain, kGachaPurchaseZorder_content);
	}
	
	return true;
}

void GachaPurchase::onEnter()
{
	CCLayer::onEnter();
	
	if(recent_mode == kGachaPurchaseStartMode_select)
		showPopup();
	else if(recent_mode == kGachaPurchaseStartMode_reward)
		startGacha();
}

void GachaPurchase::startGacha()
{
	CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(240,160));
	left_curtain->runAction(left_move);
	
	CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(240,160));
	CCCallFunc* move_end = CCCallFunc::create(this, callfunc_selector(GachaPurchase::endGachaListPopup));
	CCSequence* right_seq = CCSequence::createWithTwoActions(right_move, move_end);
	right_curtain->runAction(right_seq);
}

void GachaPurchase::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	endShowPopup();
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(GachaPurchase::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void GachaPurchase::endShowPopup()
{
	is_menu_enable = true;
}

void GachaPurchase::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,-160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(GachaPurchase::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void GachaPurchase::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void GachaPurchase::gachaListPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,-160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(GachaPurchase::endGachaListPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
	
	left_curtain = CCSprite::create("curtain_left.png");
	left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
	left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
	left_curtain->setPosition(ccp(0, 160));
	addChild(left_curtain, kGachaPurchaseZorder_content);
	
	right_curtain = CCSprite::create("curtain_left.png");
	right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
	right_curtain->setFlipX(true);
	right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
	right_curtain->setPosition(ccp(480,160));
	addChild(right_curtain, kGachaPurchaseZorder_content);
	
	CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(240,160));
	left_curtain->runAction(left_move);
	
	CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(240,160));
	right_curtain->runAction(right_move);
}

void GachaPurchase::endGachaListPopup()
{
	if(gray)
		gray->removeFromParent();
	if(main_case)
		main_case->removeFromParent();
	
	gacha_cnt = 4;
	
	recent_gacha = rand()%gacha_cnt;
	
	gacha_touch = CCSprite::create("gacha_touch.png");
	gacha_touch->setPosition(ccp(240, 480));
	addChild(gacha_touch, kGachaPurchaseZorder_content);
	gacha_touch->runAction(CCMoveTo::create(0.4f, ccp(240, 290)));
	
	float base_angle = 90.f + 360.f/gacha_cnt/2.f;
	float base_distance = 100.f;
	for(int i=1;i<=gacha_cnt;i++)
	{
		CCSprite* t_gacha = CCSprite::create(CCString::createWithFormat("gacha_gacha%d_off.png", i)->getCString());
		t_gacha->setPosition(ccpAdd(ccp(240,140),
									ccp(cosf((base_angle+(i-1)*(360.f/gacha_cnt))/180.f*M_PI)*(base_distance+200),
										sinf((base_angle+(i-1)*(360.f/gacha_cnt))/180.f*M_PI)*(base_distance+200))));
		addChild(t_gacha, kGachaPurchaseZorder_content, kGachaPurchaseMenuTag_gachaBase+i-1);
		t_gacha->runAction(CCMoveTo::create(0.4f, ccpAdd(ccp(240,140),
														 ccp(cosf((base_angle+(i-1)*(360.f/gacha_cnt))/180.f*M_PI)*base_distance,
															 sinf((base_angle+(i-1)*(360.f/gacha_cnt))/180.f*M_PI)*base_distance))));
		
		CCSprite* on_gacha = CCSprite::create(CCString::createWithFormat("gacha_gacha%d_on.png", i)->getCString());
		on_gacha->setPosition(ccp(t_gacha->getContentSize().width/2.f, t_gacha->getContentSize().height/2.f));
		on_gacha->setVisible(false);
		t_gacha->addChild(on_gacha, 1, 1);
	}
	
	getChildByTag(kGachaPurchaseMenuTag_gachaBase+recent_gacha)->getChildByTag(1)->setVisible(true);
	
	schedule(schedule_selector(GachaPurchase::changeGacha), 0.2f);
	
	is_touch_on = true;
}

void GachaPurchase::changeGacha()
{
	recent_gacha = (recent_gacha+1)%gacha_cnt;
	
	int before_gacha = recent_gacha == 0 ? gacha_cnt-1 : recent_gacha-1;
	getChildByTag(kGachaPurchaseMenuTag_gachaBase+before_gacha)->getChildByTag(1)->setVisible(false);
	getChildByTag(kGachaPurchaseMenuTag_gachaBase+recent_gacha)->getChildByTag(1)->setVisible(true);
}

void GachaPurchase::popupClose()
{
	is_menu_enable = true;
}

CCPoint GachaPurchase::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kGachaPurchaseMenuTag_close)
		return_value = ccp(165,73);
	else if(t_tag == kGachaPurchaseMenuTag_ruby)
		return_value = ccp(-113,-13);
	else if(t_tag == kGachaPurchaseMenuTag_gold)
		return_value = ccp(0,-13);
	else if(t_tag == kGachaPurchaseMenuTag_candy)
		return_value = ccp(112,-13);
	
	return_value = ccpAdd(return_value, ccp(main_case->getContentSize().width/2.f, main_case->getContentSize().height/2.f));
	
	return return_value;
}

void GachaPurchase::menuAction(CCObject *pSender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kGachaPurchaseMenuTag_close)
	{
		hidePopup();
	}
	else if(tag == kGachaPurchaseMenuTag_ruby)
	{
		bool is_gacha_enable = false;
		
		if(mySGD->getStar() >= mySGD->getGachaRubyFee())
			is_gacha_enable = true;
		
		if(is_gacha_enable)
		{
			keep_price_type = 1;
			mySGD->setStar(mySGD->getStar() - mySGD->getGachaRubyFee());
			myDSH->saveUserData({kSaveUserData_Key_star}, nullptr);
			
			if(target_out)
				(target_out->*delegate_out)();
			gachaListPopup();
		}
		else
			is_menu_enable = true;
	}
	else if(tag == kGachaPurchaseMenuTag_gold)
	{
		bool is_gacha_enable = false;
		
		if(mySGD->getGold() >= mySGD->getGachaGoldFee())
			is_gacha_enable = true;
		
		if(is_gacha_enable)
		{
			keep_price_type = 2;
			mySGD->setGold(mySGD->getGold() - mySGD->getGachaGoldFee());
			myDSH->saveUserData({kSaveUserData_Key_gold}, nullptr);
			
			if(target_out)
				(target_out->*delegate_out)();
			gachaListPopup();
		}
		else
			is_menu_enable = true;
	}
	else if(tag == kGachaPurchaseMenuTag_candy)
	{
		bool is_gacha_enable = false;
		
		if(mySGD->getFriendPoint() >= mySGD->getGachaSocialFee())
			is_gacha_enable = true;
		
		if(is_gacha_enable)
		{
			keep_price_type = 3;
			mySGD->setFriendPoint(mySGD->getFriendPoint() - mySGD->getGachaSocialFee());
			myDSH->saveUserData({kSaveUserData_Key_friendPoint}, nullptr);
			
			if(target_out)
				(target_out->*delegate_out)();
			gachaListPopup();
		}
		else
			is_menu_enable = true;
	}
}

void GachaPurchase::visibling()
{
	visibling_cnt++;
	
	CCNode* recent_gacha_img = getChildByTag(kGachaPurchaseMenuTag_gachaBase+recent_gacha)->getChildByTag(1);
	recent_gacha_img->setVisible(!recent_gacha_img->isVisible());
	
	if(visibling_cnt == 9)
	{
		unschedule(schedule_selector(GachaPurchase::visibling));
		
//		recent_gacha; // 0 ~ 3
//		gacha_cnt; // 총 가차개수. 4
		// 0 1
		// 3 2 순서임.
		
		CCMoveTo* left_move = CCMoveTo::create(0.3f, ccp(0, 160));
		left_curtain->runAction(left_move);
		
		CCMoveTo* right_move = CCMoveTo::create(0.3f, ccp(480, 160));
		right_curtain->runAction(right_move);
		
		gacha_touch->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.3f, ccp(240,550)), CCCallFunc::create(gacha_touch, callfunc_selector(CCSprite::removeFromParent))));
		
		float base_angle = 90.f + 360.f/gacha_cnt/2.f;
		float base_distance = 100.f;
		for(int i=1;i<=gacha_cnt;i++)
		{
			CCMoveTo* t_move = CCMoveTo::create(0.3f, ccpAdd(ccp(240,140),
															 ccp(cosf((base_angle+(i-1)*(360.f/gacha_cnt))/180.f*M_PI)*(base_distance+300),
																 sinf((base_angle+(i-1)*(360.f/gacha_cnt))/180.f*M_PI)*(base_distance+300))));
			getChildByTag(kGachaPurchaseMenuTag_gachaBase+i-1)->runAction(t_move);
		}
		
//		auto target = target_in;
//		auto delegate = delegate_in;
//		HorseGachaSub* p = HorseGachaSub::create([=](){
//			(target->*delegate)();
//		});
		auto target = target_in;
		auto delegate = delegate_in;
		auto finalFunction = [=]()
		{
			CCLog("gacha close");
			
			CCMoveTo* left_in_move = CCMoveTo::create(0.3f, ccp(240,160));
			CCDelayTime* left_delay = CCDelayTime::create(0.3f);
			CCMoveTo* left_out_move = CCMoveTo::create(0.3f, ccp(0, 160));
			CCCallFunc* left_remove = CCCallFunc::create(left_curtain, callfunc_selector(CCSprite::removeFromParent));
			CCSequence* left_seq = CCSequence::create(left_in_move, left_delay, left_out_move, left_remove, NULL);
			left_curtain->runAction(left_seq);
			
			CCMoveTo* right_in_move = CCMoveTo::create(0.3f, ccp(240,160));
			CCDelayTime* right_delay = CCDelayTime::create(0.3f);
			CCCallFunc* remove_main = CCCallFunc::create(main_case, callfunc_selector(CCSprite::removeFromParent));
			CCMoveTo* right_out_move = CCMoveTo::create(0.3f, ccp(480,160));
			CCCallFunc* right_remove = CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParent));
			CCSequence* right_seq = CCSequence::create(right_in_move, right_delay, remove_main, right_out_move, right_remove, NULL);
			right_curtain->runAction(right_seq);
			if(target)
				(target->*delegate)();
			if(finish_function)
				finish_function();
		};
		
		switch(recent_gacha)
		{
			case 0:
			{
				// 가차 종류에 따라 다른 상품 넣음.
			
				if(keep_price_type == 1)
				{
					RouletteSub* p = RouletteSub::create(finalFunction, {
						RewardSprite::create(RewardKind::kRuby, 20, "", 1),
						RewardSprite::create(RewardKind::kGold, 500, "", 2),
						RewardSprite::create(RewardKind::kDash, 1, "", 3),
						RewardSprite::create(RewardKind::kPower, 1, "", 4),
						RewardSprite::create(RewardKind::kSlience, 1, "", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "", 6),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "", 7)
					}, recent_mode, GachaCategory::kRubyGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
				else if(keep_price_type == 2)
				{
					RouletteSub* p = RouletteSub::create(finalFunction, {
						RewardSprite::create(RewardKind::kRuby, 20, "", 1),
						RewardSprite::create(RewardKind::kGold, 500, "", 2),
						RewardSprite::create(RewardKind::kDash, 1, "", 3),
						RewardSprite::create(RewardKind::kPower, 1, "", 4),
						RewardSprite::create(RewardKind::kSlience, 1, "", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "", 6),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "", 7)
					}, recent_mode, GachaCategory::kGoldGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
				else 
				{
					RouletteSub* p = RouletteSub::create(finalFunction, {
						RewardSprite::create(RewardKind::kRuby, 20, "", 1),
						RewardSprite::create(RewardKind::kGold, 500, "", 2),
						RewardSprite::create(RewardKind::kDash, 1, "", 3),
						RewardSprite::create(RewardKind::kPower, 1, "", 4),
						RewardSprite::create(RewardKind::kSlience, 1, "", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "", 6),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "", 7)
					}, recent_mode, GachaCategory::kSocialGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
			}
				break;
			case 1: // 
			{
				if(keep_price_type == 1)
				{
					HatGachaSub* p = HatGachaSub::create(finalFunction,
							{
								RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
						RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
						RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
						RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
						RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5),
						RewardSprite::create(RewardKind::kGold, 1000, "price_gold_img.png", 5)
							},
							//																						 {},
							recent_mode, GachaCategory::kRubyGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
				else if(keep_price_type == 2)
				{
					HatGachaSub* p = HatGachaSub::create(finalFunction,
							{
								RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
						RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
						RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
						RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
						RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5),
						RewardSprite::create(RewardKind::kGold, 1000, "price_gold_img.png", 5)
							},
							//																						 {},
							recent_mode, GachaCategory::kGoldGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
				else
				{
					HatGachaSub* p = HatGachaSub::create(finalFunction,
							{
								RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
						RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
						RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
						RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
						RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5),
						RewardSprite::create(RewardKind::kGold, 1000, "price_gold_img.png", 5)
							},
							//																						 {},
							recent_mode, GachaCategory::kSocialGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
			}
				break;
			case 2:
			{

				if(keep_price_type == 1)
				{
					HorseGachaSub* p = HorseGachaSub::create(finalFunction,
							{
								RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
						RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
						RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
						RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
						RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5)
							}
							, recent_mode, GachaCategory::kRubyGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
				else if(keep_price_type == 2)
				{
					HorseGachaSub* p = HorseGachaSub::create(finalFunction,
							{
								RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
						RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
						RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
						RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
						RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5)
							}
							, recent_mode, GachaCategory::kGoldGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
				else
				{
					HorseGachaSub* p = HorseGachaSub::create(finalFunction,
							{
								RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
						RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
						RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
						RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
						RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5)
							}
							, recent_mode, GachaCategory::kSocialGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
			}
				break;
			case 3: // 빠ㅊ칭ㅋ코
			{
				if(keep_price_type == 1)
				{
					SlotMachineSub* p = SlotMachineSub::create(finalFunction,
							{
								RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
						RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
						RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
						RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
						RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5)
							},
							recent_mode, GachaCategory::kRubyGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
				else if(keep_price_type == 2)
				{
					SlotMachineSub* p = SlotMachineSub::create(finalFunction,
							{
								RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
						RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
						RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
						RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
						RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5)
							},
							recent_mode, GachaCategory::kGoldGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
				else 
				{
					SlotMachineSub* p = SlotMachineSub::create(finalFunction,
							{
								RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
						RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
						RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
						RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
						RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
						RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5),
						RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5)
							},
							recent_mode, GachaCategory::kSocialGacha);
					getParent()->addChild(p, kMainFlowZorder_popup - 1);
				}
			}
		}
		
	}
}

bool GachaPurchase::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	if(is_touch_on)
	{
		is_touch_on = false;
		unschedule(schedule_selector(GachaPurchase::changeGacha));
		
		main_case = CCSprite::create("gacha_back_table.png");
		main_case->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		main_case->setPosition(ccp(240,160));
		addChild(main_case, kGachaPurchaseZorder_back);
		main_case->setOpacity(0);	
		visibling_cnt = 0;
		schedule(schedule_selector(GachaPurchase::visibling), 0.4f);
	}
	
	return true;
}
void GachaPurchase::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void GachaPurchase::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void GachaPurchase::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void GachaPurchase::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
