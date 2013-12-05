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

enum GachaPurchaseZorder{
	kGachaPurchaseZorder_gray = 1,
	kGachaPurchaseZorder_back,
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

bool GachaPurchase::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	is_menu_enable = false;
	is_touch_on = false;
	setTouchEnabled(true);
	
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
	main_case->setPosition(ccp(240,-160));
	addChild(main_case, kGachaPurchaseZorder_back);
	
	
	CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
	CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(GachaPurchase::menuAction));
	close_item->setTag(kGachaPurchaseMenuTag_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kGachaPurchaseMenuTag_close));
	main_case->addChild(close_menu);
	close_menu->setTouchPriority(-171);
	
	
	CCSprite* n_ruby = CCSprite::create("gacha_purchase_ruby.png");
	CCLabelTTF* n_ruby_price = CCLabelTTF::create(CCString::createWithFormat("%d", 1)->getCString(), mySGD->getFont().c_str(), 15);
	n_ruby_price->setPosition(ccp(n_ruby->getContentSize().width/2.f+20, 25));
	n_ruby->addChild(n_ruby_price);
	CCSprite* s_ruby = CCSprite::create("gacha_purchase_ruby.png");
	s_ruby->setColor(ccGRAY);
	CCLabelTTF* s_ruby_price = CCLabelTTF::create(CCString::createWithFormat("%d", 1)->getCString(), mySGD->getFont().c_str(), 15);
	s_ruby_price->setPosition(ccp(s_ruby->getContentSize().width/2.f+20, 25));
	s_ruby->addChild(s_ruby_price);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(GachaPurchase::menuAction));
	ruby_item->setTag(kGachaPurchaseMenuTag_ruby);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(getContentPosition(kGachaPurchaseMenuTag_ruby));
	main_case->addChild(ruby_menu);
	ruby_menu->setTouchPriority(-171);
	
	CCSprite* n_gold = CCSprite::create("gacha_purchase_gold.png");
	CCLabelTTF* n_gold_price = CCLabelTTF::create(CCString::createWithFormat("%d", 2000)->getCString(), mySGD->getFont().c_str(), 15);
	n_gold_price->setPosition(ccp(n_gold->getContentSize().width/2.f+20, 25));
	n_gold->addChild(n_gold_price);
	CCSprite* s_gold = CCSprite::create("gacha_purchase_gold.png");
	s_gold->setColor(ccGRAY);
	CCLabelTTF* s_gold_price = CCLabelTTF::create(CCString::createWithFormat("%d", 2000)->getCString(), mySGD->getFont().c_str(), 15);
	s_gold_price->setPosition(ccp(s_gold->getContentSize().width/2.f+20, 25));
	s_gold->addChild(s_gold_price);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(GachaPurchase::menuAction));
	gold_item->setTag(kGachaPurchaseMenuTag_gold);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(getContentPosition(kGachaPurchaseMenuTag_gold));
	main_case->addChild(gold_menu);
	gold_menu->setTouchPriority(-171);
	
	CCSprite* n_candy = CCSprite::create("gacha_purchase_candy.png");
	CCLabelTTF* n_candy_price = CCLabelTTF::create(CCString::createWithFormat("%d", 200)->getCString(), mySGD->getFont().c_str(), 15);
	n_candy_price->setPosition(ccp(n_candy->getContentSize().width/2.f+20, 25));
	n_candy->addChild(n_candy_price);
	CCSprite* s_candy = CCSprite::create("gacha_purchase_candy.png");
	s_candy->setColor(ccGRAY);
	CCLabelTTF* s_candy_price = CCLabelTTF::create(CCString::createWithFormat("%d", 200)->getCString(), mySGD->getFont().c_str(), 15);
	s_candy_price->setPosition(ccp(s_candy->getContentSize().width/2.f+20, 25));
	s_candy->addChild(s_candy_price);
	
	CCMenuItem* candy_item = CCMenuItemSprite::create(n_candy, s_candy, this, menu_selector(GachaPurchase::menuAction));
	candy_item->setTag(kGachaPurchaseMenuTag_candy);
	
	CCMenu* candy_menu = CCMenu::createWithItem(candy_item);
	candy_menu->setPosition(getContentPosition(kGachaPurchaseMenuTag_candy));
	main_case->addChild(candy_menu);
	candy_menu->setTouchPriority(-171);
	
	
	return true;
}

void GachaPurchase::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
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
	gacha_cnt = 4;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,-160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(GachaPurchase::endGachaListPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
	
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
}

void GachaPurchase::endGachaListPopup()
{
	gray->removeFromParent();
	main_case->removeFromParent();
	
	recent_gacha = rand()%gacha_cnt;
	
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
		return_value = ccp(180,85);
	else if(t_tag == kGachaPurchaseMenuTag_ruby)
		return_value = ccp(-127,-18);
	else if(t_tag == kGachaPurchaseMenuTag_gold)
		return_value = ccp(0,-18);
	else if(t_tag == kGachaPurchaseMenuTag_candy)
		return_value = ccp(127,-18);
	
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
		
		if(mySGD->getStar() >= 1)
			is_gacha_enable = true;
		
		if(is_gacha_enable)
		{
			mySGD->setStar(mySGD->getStar() - 1);
			myDSH->saveUserData({kSaveUserData_Key_star}, nullptr);
			
			(target_out->*delegate_out)();
			gachaListPopup();
		}
		else
			is_menu_enable = true;
	}
	else if(tag == kGachaPurchaseMenuTag_gold)
	{
		bool is_gacha_enable = false;
		
		if(mySGD->getGold() >= 2000)
			is_gacha_enable = true;
		
		if(is_gacha_enable)
		{
			mySGD->setGold(mySGD->getGold() - 2000);
			myDSH->saveUserData({kSaveUserData_Key_gold}, nullptr);
			
			(target_out->*delegate_out)();
			gachaListPopup();
		}
		else
			is_menu_enable = true;
	}
	else if(tag == kGachaPurchaseMenuTag_candy)
	{
		bool is_gacha_enable = false;
		
		if(mySGD->getFriendPoint() >= 200)
			is_gacha_enable = true;
		
		if(is_gacha_enable)
		{
			mySGD->setFriendPoint(mySGD->getFriendPoint() - 200);
			myDSH->saveUserData({kSaveUserData_Key_friendPoint}, nullptr);
			
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
		
		GachaBase* t_gacha;
		t_gacha->setFinalAction(target_in, delegate_in);
		getParent()->addChild(t_gacha, kPMS_Z_popup);
		// 경수
		CCLog("Go! real gacha!! : %d", recent_gacha);
	}
}

bool GachaPurchase::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	if(is_touch_on)
	{
		is_touch_on = false;
		unschedule(schedule_selector(GachaPurchase::changeGacha));
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