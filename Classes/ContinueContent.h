//
//  ContinueContent.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 3..
//
//

#ifndef __DGproto__ContinueContent__
#define __DGproto__ContinueContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCMenuLambda.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum ContinueContentMenuTag{
	kContinueContentMenuTag_end = 0,
	kContinueContentMenuTag_continue
};
class ContinueContent : public CCNode
{
public:
	static ContinueContent* create(int t_touch_priority, function<void(void)> t_end, function<void(void)> t_continue)
	{
		ContinueContent* t_ctc = new ContinueContent();
		t_ctc->myInit(t_touch_priority, t_end, t_continue);
		t_ctc->autorelease();
		return t_ctc;
	}
	
	void startShow()
	{
		setScale(0);
		
		CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::endShow));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		runAction(t_seq);
	}
	
private:
	int touch_priority;
	function<void(void)> end_selector;
	function<void(void)> continue_selector;
	
	bool is_menu_enable;
	bool is_continue;
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kContinueContentMenuTag_end)
		{
			end_selector();
		}
		else if(tag == kContinueContentMenuTag_continue)
		{
			if(mySGD->getStar() >= mySGD->getPlayContinueFee())
			{
				is_continue = true;
				mySGD->setStar(mySGD->getStar()-mySGD->getPlayContinueFee());
				CCSprite* price_type = CCSprite::create("price_ruby_img.png");
				price_type->setOpacity(0);
				price_type->setPosition(ccpAdd(ccp(52,-48), ccp(-15, 0)));
				addChild(price_type);
				
				CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("-%d", mySGD->getPlayContinueFee())->getCString(), mySGD->getFont().c_str(), 16);
				price_label->setOpacity(0);
				price_label->setAnchorPoint(ccp(0,0.5f));
				price_label->setPosition(ccp(price_type->getContentSize().width/2.f+15,price_type->getContentSize().height/2.f));
				price_type->addChild(price_label);
				
				CCFadeTo* t_fade1 = CCFadeTo::create(0.2f, 255);
				CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
				CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 0);
				CCSequence* t_seq = CCSequence::create(t_fade1, t_delay1, t_fade2, NULL);
				
				CCMoveBy* t_move1 = CCMoveBy::create(0.9f, ccp(0,50));
				
				CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_seq, t_move1);
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::startHide));
				CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
				
				price_type->runAction(t_seq2);
				
				CCFadeTo* t_fade3 = CCFadeTo::create(0.2f, 255);
				CCDelayTime* t_delay3 = CCDelayTime::create(0.2f);
				CCFadeTo* t_fade4 = CCFadeTo::create(0.5f, 0);
				CCSequence* t_seq3 = CCSequence::create(t_fade3, t_delay3, t_fade4, NULL);
				price_label->runAction(t_seq3);
			}
			else
			{
				CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
				case_back->setPosition(CCPointZero);
				addChild(case_back);
				
				case_back->setContentSize(CCSizeMake(250, 150));
				
				CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6));
				content_back->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(0,20)));
				case_back->addChild(content_back);
				
				content_back->setContentSize(CCSizeMake(230,90));
				
				
				CCSprite* price_type_img1 = CCSprite::create("price_ruby_img.png");
				price_type_img1->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(-55,35)));
				case_back->addChild(price_type_img1);
				
				CCSprite* price_type_img2 = CCSprite::create("price_ruby_img.png");
				price_type_img2->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(-88,17)));
				case_back->addChild(price_type_img2);
				
				CCLabelTTF* price_value_label = CCLabelTTF::create(" 가 부족합니다.\n   10개를 구입하시겠습니까?", mySGD->getFont().c_str(), 16);
				price_value_label->setAnchorPoint(ccp(0,0.5f));
				price_value_label->setPosition(ccp(price_type_img1->getContentSize().width/2.f-35, price_type_img1->getContentSize().height/2.f-8));
				price_type_img1->addChild(price_value_label);
				
				
				CCScale9Sprite* n_cancel = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
				n_cancel->setContentSize(CCSizeMake(95,45));
				CCLabelTTF* n_cancel_label = CCLabelTTF::create("취소", mySGD->getFont().c_str(), 14);
				n_cancel_label->setPosition(ccp(n_cancel->getContentSize().width/2.f, n_cancel->getContentSize().height/2.f));
				n_cancel->addChild(n_cancel_label);
				
				CCScale9Sprite* s_cancel = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
				s_cancel->setContentSize(CCSizeMake(95,45));
				s_cancel->setColor(ccGRAY);
				CCLabelTTF* s_cancel_label = CCLabelTTF::create("취소", mySGD->getFont().c_str(), 14);
				s_cancel_label->setPosition(ccp(s_cancel->getContentSize().width/2.f, s_cancel->getContentSize().height/2.f));
				s_cancel->addChild(s_cancel_label);
				
				CCMenuItemLambda* cancel_item = CCMenuItemSpriteLambda::create(n_cancel, s_cancel, [=](CCObject* sender){case_back->removeFromParent();is_menu_enable = true;});
				
				CCMenuLambda * cancel_menu = CCMenuLambda::createWithItem(cancel_item);
				cancel_menu->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(-52,-48)));
				case_back->addChild(cancel_menu);
				cancel_menu->setTouchPriority(touch_priority-2);
				
				
				CCScale9Sprite* n_buy = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
				n_buy->setContentSize(CCSizeMake(95,45));
				CCLabelTTF* n_buy_label = CCLabelTTF::create("임시 결재", mySGD->getFont().c_str(), 14);
				n_buy_label->setPosition(ccp(n_buy->getContentSize().width/2.f, n_buy->getContentSize().height/2.f));
				n_buy->addChild(n_buy_label);
				
				CCScale9Sprite* s_buy = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
				s_buy->setContentSize(CCSizeMake(95,45));
				s_buy->setColor(ccGRAY);
				CCLabelTTF* s_buy_label = CCLabelTTF::create("임시 결재", mySGD->getFont().c_str(), 14);
				s_buy_label->setPosition(ccp(s_buy->getContentSize().width/2.f, s_buy->getContentSize().height/2.f));
				s_buy->addChild(s_buy_label);
				
				CCMenuItemLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender)
				{
					is_continue = true;
					case_back->removeFromParent();
					mySGD->setStar(mySGD->getStar()+10-mySGD->getPlayContinueFee());
					CCSprite* price_type = CCSprite::create("price_ruby_img.png");
					price_type->setOpacity(0);
					price_type->setPosition(ccpAdd(ccp(52,-48), ccp(-15, 0)));
					addChild(price_type);
					
					CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("-%d", mySGD->getPlayContinueFee())->getCString(), mySGD->getFont().c_str(), 16);
					price_label->setOpacity(0);
					price_label->setAnchorPoint(ccp(0,0.5f));
					price_label->setPosition(ccp(price_type->getContentSize().width/2.f+15,price_type->getContentSize().height/2.f));
					price_type->addChild(price_label);
					
					CCFadeTo* t_fade1 = CCFadeTo::create(0.2f, 255);
					CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
					CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 0);
					CCSequence* t_seq = CCSequence::create(t_fade1, t_delay1, t_fade2, NULL);
					
					CCMoveBy* t_move1 = CCMoveBy::create(0.9f, ccp(0,50));
					
					CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_seq, t_move1);
					CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::startHide));
					CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
					
					price_type->runAction(t_seq2);
					
					CCFadeTo* t_fade3 = CCFadeTo::create(0.2f, 255);
					CCDelayTime* t_delay3 = CCDelayTime::create(0.2f);
					CCFadeTo* t_fade4 = CCFadeTo::create(0.5f, 0);
					CCSequence* t_seq3 = CCSequence::create(t_fade3, t_delay3, t_fade4, NULL);
					price_label->runAction(t_seq3);
				});
				
				CCMenuLambda* buy_menu = CCMenuLambda::createWithItem(buy_item);
				buy_menu->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(52,-48)));
				case_back->addChild(buy_menu);
				buy_menu->setTouchPriority(touch_priority-2);
				
				case_back->setScale(0);
				CCScaleTo* t_scale = CCScaleTo::create(0.2f, 1.f);
				case_back->runAction(t_scale);
			}
		}
	}
	
	void endShow()
	{
		is_menu_enable = true;
	}
	void startHide()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.3f, 0.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::endHide));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		runAction(t_seq);
	}
	void endHide()
	{
		if(is_continue)
			continue_selector();
		else
			end_selector();
	}
	
	void myInit(int t_touch_priority, function<void(void)> t_end, function<void(void)> t_continue)
	{
		is_continue = false;
		is_menu_enable = false;
		
		touch_priority = t_touch_priority;
		end_selector = t_end;
		continue_selector = t_continue;
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setPosition(CCPointZero);
		addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(230, 150));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6));
		content_back->setPosition(ccp(0,20));
		addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(210,90));
		
		
		CCSprite* price_type_img = CCSprite::create("price_ruby_img.png");
		price_type_img->setPosition(ccp(-60,28));
		addChild(price_type_img);
		
		CCLabelTTF* price_value_label = CCLabelTTF::create(CCString::createWithFormat("%d 가 소진됩니다.\n이어하시겠습니까?", mySGD->getPlayContinueFee())->getCString(), mySGD->getFont().c_str(), 16);
		price_value_label->setAnchorPoint(ccp(0,0.5f));
		price_value_label->setPosition(ccp(price_type_img->getContentSize().width/2.f+8, price_type_img->getContentSize().height/2.f-8));
		price_type_img->addChild(price_value_label);
		
		
		CCScale9Sprite* n_end = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
		n_end->setContentSize(CCSizeMake(95,45));
		CCLabelTTF* n_end_label = CCLabelTTF::create("끝내기", mySGD->getFont().c_str(), 14);
		n_end_label->setPosition(ccp(n_end->getContentSize().width/2.f, n_end->getContentSize().height/2.f));
		n_end->addChild(n_end_label);
		
		CCScale9Sprite* s_end = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
		s_end->setContentSize(CCSizeMake(95,45));
		s_end->setColor(ccGRAY);
		CCLabelTTF* s_end_label = CCLabelTTF::create("끝내기", mySGD->getFont().c_str(), 14);
		s_end_label->setPosition(ccp(s_end->getContentSize().width/2.f, s_end->getContentSize().height/2.f));
		s_end->addChild(s_end_label);
		
		CCMenuItem* end_item = CCMenuItemSprite::create(n_end, s_end, this, menu_selector(ContinueContent::menuAction));
		end_item->setTag(kContinueContentMenuTag_end);
		
		CCMenu* end_menu = CCMenu::createWithItem(end_item);
		end_menu->setPosition(-52,-48);
		addChild(end_menu);
		end_menu->setTouchPriority(touch_priority-1);
		
		
		CCScale9Sprite* n_continue = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
		n_continue->setContentSize(CCSizeMake(95,45));
		CCLabelTTF* n_continue_label = CCLabelTTF::create("이어하기", mySGD->getFont().c_str(), 14);
		n_continue_label->setPosition(ccp(n_continue->getContentSize().width/2.f, n_continue->getContentSize().height/2.f));
		n_continue->addChild(n_continue_label);
		
		CCScale9Sprite* s_continue = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
		s_continue->setContentSize(CCSizeMake(95,45));
		s_continue->setColor(ccGRAY);
		CCLabelTTF* s_continue_label = CCLabelTTF::create("이어하기", mySGD->getFont().c_str(), 14);
		s_continue_label->setPosition(ccp(s_continue->getContentSize().width/2.f, s_continue->getContentSize().height/2.f));
		s_continue->addChild(s_continue_label);
		
		CCMenuItem* continue_item = CCMenuItemSprite::create(n_continue, s_continue, this, menu_selector(ContinueContent::menuAction));
		continue_item->setTag(kContinueContentMenuTag_continue);
		
		CCMenu* continue_menu = CCMenu::createWithItem(continue_item);
		continue_menu->setPosition(52,-48);
		addChild(continue_menu);
		continue_menu->setTouchPriority(touch_priority-1);
	}
};

#endif /* defined(__DGproto__ContinueContent__) */
