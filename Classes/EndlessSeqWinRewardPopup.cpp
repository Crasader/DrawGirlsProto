//
//  EndlessSeqWinRewardPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 6. 16..
//
//

#include "EndlessSeqWinRewardPopup.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "FormSetter.h"
#include "StarGoldData.h"
#include "StyledLabelTTF.h"
#include "CommonAnimation.h"
#include "StageImgLoader.h"

EndlessSeqWinRewardPopup* EndlessSeqWinRewardPopup::create(int t_touch_priority, function<void()> t_end_func, Json::Value t_reward)
{
	EndlessSeqWinRewardPopup* t_mup = new EndlessSeqWinRewardPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_reward);
	t_mup->autorelease();
	return t_mup;
}

void EndlessSeqWinRewardPopup::myInit(int t_touch_priority, function<void()> t_end_func, Json::Value t_reward)
{
	startFormSetter(this);
	
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	setFormSetter(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251, 113));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-12.f));
	back_case->addChild(back_in);
	setFormSetter(back_in);
	
	
	int victory_value = t_reward["endlessData"]["ing_win"].asInt();
	
	KSLabelTTF* title_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardTitle), victory_value), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(-85, back_case->getContentSize().height/2.f-35));
	back_case->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25,back_case->getContentSize().height-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
								  
								  CommonAnimation::closePopup(this, m_container, gray, [=](){
									  
								  }, [=](){
									  end_func();
									  removeFromParent();
									  
								  });
							  });
	back_case->addChild(close_button);
	
	Json::Value t_reward_list = t_reward["reward"];
	
	string reward_type = "p6";
	
	if(t_reward_list.size() >= 2)
	{
		reward_type = "many";
	}
	else
	{
		reward_type = t_reward_list[0]["type"].asString();
	}

	CCNode* reward_node = getRewardNode(reward_type);
	reward_node->setPosition(ccp(back_in->getContentSize().width/2.f, 75));
	back_in->addChild(reward_node);
	
	KSLabelTTF* content_label = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardContent), victory_value), mySGD->getFont().c_str(), 12);
	content_label->setColor(ccc3(255, 170, 20));
	content_label->setPosition(ccp(back_in->getContentSize().width/2.f, 42));
	back_in->addChild(content_label);
	
	int reward_count_value = t_reward_list[0]["count"].asInt();
	
	string goods_string = getRewardString(reward_type, reward_count_value);
	
	StyledLabelTTF* goods_label = StyledLabelTTF::create(goods_string.c_str(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	goods_label->setAnchorPoint(ccp(0.5f,0.5f));
	goods_label->setPosition(ccp(back_in->getContentSize().width/2.f, 20));
	back_in->addChild(goods_label);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
	}, [=](){
		is_menu_enable = true;
	});
}

string EndlessSeqWinRewardPopup::getRewardString(string t_type, int t_count)
{
	string return_value = "";
	
	GoodsType goods_type = mySGD->getGoodsKeyToType(t_type);
	
	if(goods_type == kGoodsType_gold)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeGold), t_count);
	}
	else if(goods_type == kGoodsType_ruby)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeRuby), t_count);
	}
	else if(goods_type == kGoodsType_item9)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeItem9), t_count);
	}
	else if(goods_type == kGoodsType_item6)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeItem6), t_count);
	}
	else if(goods_type == kGoodsType_item11)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeItem11), t_count);
	}
	else if(goods_type == kGoodsType_pass1)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass1), t_count);
	}
	else if(goods_type == kGoodsType_pass2)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass2), t_count);
	}
	else if(goods_type == kGoodsType_pass3)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass3), t_count);
	}
	else if(goods_type == kGoodsType_pass4)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass4), t_count);
	}
	else if(goods_type == kGoodsType_pass5)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass5), t_count);
	}
	else if(goods_type == kGoodsType_pass6)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass6), t_count);
	}
	else if(goods_type == kGoodsType_pass7)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass7), t_count);
	}
	else if(goods_type == kGoodsType_pass8)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass8), t_count);
	}
	else if(goods_type == kGoodsType_pass9)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass9), t_count);
	}
	else if(goods_type == kGoodsType_pass10)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass10), t_count);
	}
	else if(goods_type == kGoodsType_pass11)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass11), t_count);
	}
	else if(goods_type == kGoodsType_pass12)
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass12), t_count);
	}
	else
	{
		return_value = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeMany), t_count);
	}
	
	return return_value;
}

CCNode* EndlessSeqWinRewardPopup::getRewardNode(string t_type)
{
	CCNode* return_node = CCNode::create();
	
	GoodsType goods_type = mySGD->getGoodsKeyToType(t_type);
	
	if(goods_type == kGoodsType_gold)
	{
		CCSprite* t_img = CCSprite::create("shop_gold2.png");
		t_img->setScale(0.7f*1.5f);
		t_img->setPosition(ccp(0,-10));
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_ruby)
	{
		CCSprite* t_img = CCSprite::create("shop_ruby2.png");
		t_img->setScale(0.7f*1.5f);
		t_img->setPosition(ccp(0,-10));
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_item9)
	{
		CCSprite* t_img = CCSprite::create("item9.png");
		t_img->setScale(0.55f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_item6)
	{
		CCSprite* t_img = CCSprite::create("item6.png");
		t_img->setScale(0.55f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_item11)
	{
		CCSprite* t_img = CCSprite::create("item11.png");
		t_img->setScale(0.55f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass1)
	{
		CCSprite* t_img = CCSprite::create("pass_ticket1.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass2)
	{
		CCSprite* t_img = CCSprite::create("pass_ticket2.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass3)
	{
		CCSprite* t_img = CCSprite::create("pass_ticket3.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass4)
	{
		CCSprite* t_img = CCSprite::create("pass_ticket4.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass5)
	{
		CCSprite* t_img = CCSprite::create("pass_ticket5.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass6)
	{
		CCSprite* t_img = CCSprite::create("pass_ticket6.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass7)
	{
		CCSprite* t_img = CCSprite::create("icon_p7.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass8)
	{
		CCSprite* t_img = CCSprite::create("icon_p8.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass9)
	{
		CCSprite* t_img = CCSprite::create("icon_p9.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass10)
	{
		CCSprite* t_img = CCSprite::create("icon_p10.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass11)
	{
		CCSprite* t_img = CCSprite::create("icon_p11.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else if(goods_type == kGoodsType_pass12)
	{
		CCSprite* t_img = CCSprite::create("icon_p12.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	else
	{
		CCSprite* t_img = CCSprite::create("icon_box.png");
		t_img->setScale(1.f*1.5f);
		return_node->addChild(t_img);
	}
	
	CCSprite* light_img = CCSprite::create("newitem_back.png");
	light_img->setScale(0.6f);
	return_node->addChild(light_img, -1);
	
	light_img->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 90)));

	
	return return_node;
}
