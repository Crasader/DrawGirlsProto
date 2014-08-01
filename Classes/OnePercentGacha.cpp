// OnePercentGacha.cpp
//

#include "OnePercentGacha.h"
#include "CommonButton.h"
#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "KSLabelTTF.h"
#include "FormSetter.h"
#include "LoadingLayer.h"
#include "MyLocalization.h"
#include "ShopPopup.h"
#include "CommonAnimation.h"

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
	
	recent_percent*=1000.f;
	recent_percent = floor(recent_percent);
	recent_percent/=1000.f;
	
	target_cancel = t_cancel;
	delegate_cancel = d_cancel;
	target_ok = t_ok;
	delegate_ok = d_ok;
	
	back_img = NULL;
	cancel_menu = NULL;
	
	gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,myDSH->ui_center_y));
	gray->setScaleY(myDSH->ui_top/320.f);
	addChild(gray, kOnePercentGacha_Z_gray);
	gray->setOpacity(0);
	
	setBack();
	
	is_menu_enable = false;
	
	touched_number = 0;
	
	setTouchEnabled(true);
	
	startShow();
}
void OnePercentGacha::startShow()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.25f, 255);
	gray->runAction(gray_fade);
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		endShow();
	});
}
void OnePercentGacha::endShow()
{
	is_menu_enable = true;
}
void OnePercentGacha::gachaOn ()
{
	CCMoveTo* t_move1 = CCMoveTo::create(0.05f, ccp(0+114,13));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(OnePercentGacha::repeatAction));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move1, t_call);
	arrow_img->runAction(t_seq);
}
void OnePercentGacha::repeatAction ()
{
	is_menu_enable = true;
	stop_button->setEnabled(true);
	CCMoveTo* t_move2 = CCMoveTo::create(0.1f, ccp(0-114,13));
	CCMoveTo* t_move3 = CCMoveTo::create(0.1f, ccp(0+114,13));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_move2, t_move3);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
	arrow_img->runAction(t_repeat);
}

void OnePercentGacha::gachaAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	
	if(mySGD->getGoodsValue(kGoodsType_pass5) > 0)
	{
		myLog->addLog(kLOG_gacha_onePercent, -1);
		
		LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
		addChild(t_loading, 9999);
		
		mySGD->addChangeGoods("g_99_p");
		
		mySGD->changeGoods([=](Json::Value result_data){
			t_loading->removeFromParent();
			if(result_data["result"]["code"] == GDSUCCESS)
			{
				gacha_button->removeFromParent();
				
				CCLabelTTF* t_label = CCLabelTTF::create();
				
				KSLabelTTF* stop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_pleaseClickButton), mySGD->getFont().c_str(), 13);
				stop_label->setColor(ccBLACK);
				stop_label->setPosition(ccp(0,15));
				t_label->addChild(stop_label);
				
				KSLabelTTF* stop_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_stop), mySGD->getFont().c_str(), 28);
				stop_label2->setColor(ccBLACK);
				stop_label2->setPosition(ccp(0,-12));
				t_label->addChild(stop_label2);
				
				
				CCScale9Sprite* stop_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
				
				stop_button = CCControlButton::create(t_label, stop_back);
				stop_button->addTargetWithActionForControlEvents(this, cccontrol_selector(OnePercentGacha::gachaStopAction), CCControlEventTouchUpInside);
				stop_button->setPreferredSize(CCSizeMake(170,65));
				stop_button->setPosition(ccp(0,-70));
				m_container->addChild(stop_button, kOnePercentGacha_Z_content);
				
				stop_button->setTouchPriority(-180);
				
				stop_button->setEnabled(false);
				cancel_menu->setEnabled(false);
				
				gachaOn();
			}
			else
			{
				mySGD->clearChangeGoods();
				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_noti), myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
				is_menu_enable = true;
			}
		});
	}
	else if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getGachaOnePercentFee())
	{
		myLog->addLog(kLOG_gacha_onePercent, -1);
		
		LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
		addChild(t_loading, 9999);
		
		mySGD->addChangeGoods("g_99_r");
		
		mySGD->changeGoods([=](Json::Value result_data){
			t_loading->removeFromParent();
			if(result_data["result"]["code"] == GDSUCCESS)
			{
				gacha_button->removeFromParent();
				
				CCLabelTTF* t_label = CCLabelTTF::create();
				
				KSLabelTTF* stop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_pleaseClickButton), mySGD->getFont().c_str(), 13);
				stop_label->setColor(ccBLACK);
				stop_label->setPosition(ccp(0,15));
				t_label->addChild(stop_label);
				
				KSLabelTTF* stop_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_stop), mySGD->getFont().c_str(), 28);
				stop_label2->setColor(ccBLACK);
				stop_label2->setPosition(ccp(0,-12));
				t_label->addChild(stop_label2);
				
				
				CCScale9Sprite* stop_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
				
				stop_button = CCControlButton::create(t_label, stop_back);
				stop_button->addTargetWithActionForControlEvents(this, cccontrol_selector(OnePercentGacha::gachaStopAction), CCControlEventTouchUpInside);
				stop_button->setPreferredSize(CCSizeMake(170,65));
				stop_button->setPosition(ccp(0,-70));
				m_container->addChild(stop_button, kOnePercentGacha_Z_content);
				
				stop_button->setTouchPriority(-180);
				
				stop_button->setEnabled(false);
				cancel_menu->setEnabled(false);
				
				gachaOn();
			}
			else
			{
				mySGD->clearChangeGoods();
				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_noti), myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
				is_menu_enable = true;
			}
		});
	}
	else
	{
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_noti), myLoc->getLocalForKey(kMyLocalKey_rubyNotEnought), [=](){is_menu_enable = true;}), 9999);
		
//		ShopPopup* t_popup = ShopPopup::create();
//		t_popup->setScale(myDSH->screen_convert_rate);
//		t_popup->setShopCode(kSC_ruby);
//		t_popup->setCloseFunc([=]()
//							  {
//								  is_menu_enable = true;
//							  });
//		addChild(t_popup, kOnePercentGacha_Z_shop);
	}
}

void OnePercentGacha::requestItemDelivery()
{
	vector<CommandParam> command_list;
	
	Json::Value transaction_param;
	transaction_param["memberID"] = hspConnector::get()->getMemberID();
	command_list.push_back(CommandParam("starttransaction", transaction_param, [=](Json::Value result_data)
										{
											if(result_data["result"]["code"].asInt() == GDSUCCESS)
											{
												inapp_loading->removeFromParent();
												
												t_popup->removeFromParent();
												
												gacha_button->removeFromParent();
												
												CCLabelTTF* t_label = CCLabelTTF::create();
												
												KSLabelTTF* stop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_pleaseClickButton), mySGD->getFont().c_str(), 13);
												stop_label->setColor(ccBLACK);
												stop_label->setPosition(ccp(0,15));
												t_label->addChild(stop_label);
												
												KSLabelTTF* stop_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_stop), mySGD->getFont().c_str(), 28);
												stop_label2->setColor(ccBLACK);
												stop_label2->setPosition(ccp(0,-12));
												t_label->addChild(stop_label2);
												
												
												CCScale9Sprite* stop_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
												
												stop_button = CCControlButton::create(t_label, stop_back);
												stop_button->addTargetWithActionForControlEvents(this, cccontrol_selector(OnePercentGacha::gachaStopAction), CCControlEventTouchUpInside);
												stop_button->setPreferredSize(CCSizeMake(170,65));
												stop_button->setPosition(ccp(0,-70));
												m_container->addChild(stop_button, kOnePercentGacha_Z_content);
												
												stop_button->setTouchPriority(-180);
												
												stop_button->setEnabled(false);
												cancel_menu->setEnabled(false);
												
												gachaOn();
											}
											else
											{
												addChild(KSTimer::create(3.f, [=](){requestItemDelivery();}));
											}
										}));
	
	Json::Value request_param;
	request_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("requestItemDelivery", request_param, nullptr));
	if(mySGD->isChangedGoods())
	{
		vector<CommandParam> t_list = mySGD->getChangeGoodsParam(json_selector(mySGD, StarGoldData::saveChangeGoodsTransaction));
		for(int i=0;i<t_list.size();i++)
		{
			command_list.push_back(t_list[i]);
		}
	}
	
	hspConnector::get()->command(command_list);
}

void OnePercentGacha::gachaStopAction(CCObject *sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	stop_button->setEnabled(false);
	arrow_img->stopAllActions();
	float position_value = (arrow_img->getPositionX()+114.f)/228.f;
	
	position_value*=10.f;
	position_value = floor(position_value);
	position_value/=10.f;
	
	KSLabelTTF* get_percent = KSLabelTTF::create(CCString::createWithFormat("%.1f%%", position_value)->getCString(), mySGD->getFont().c_str(), 13);
	get_percent->setColor(ccORANGE);
	get_percent->setPosition(ccp(arrow_img->getContentSize().width/2.f, arrow_img->getContentSize().height/2.f + 3));
	arrow_img->addChild(get_percent);
	
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

void OnePercentGacha::menuAction (CCObject * sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kOnePercentGacha_MT_ok)
	{
		
	}
	else if(tag == kOnePercentGacha_MT_cancel)
	{
		(target_cancel->*delegate_cancel)();
		removeFromParent();
	}
	else if(tag == kOnePercentGacha_MT_stop)
	{
		
	}
}
void OnePercentGacha::endAction ()
{
	(target_ok->*delegate_ok)(recent_percent);
	removeFromParent();
}
void OnePercentGacha::changePercentLabel ()
{
	percent_label->setString(CCString::createWithFormat("%.1f%%", recent_percent*100.f)->getCString());
}
void OnePercentGacha::setBack ()
{
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(m_container, kOnePercentGacha_Z_back);
	
	back_img = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_img->setContentSize(CCSizeMake(315, 250));
	back_img->setPosition(ccp(0,0));
	m_container->addChild(back_img, kOnePercentGacha_Z_back);
	
	CCScale9Sprite* inner_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	inner_back->setContentSize(CCSizeMake(295, 190));
	inner_back->setPosition(ccp(0,-20));
	m_container->addChild(inner_back, kOnePercentGacha_Z_back);
	
	KSLabelTTF* recent_percent_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalPercent4), mySGD->getFont().c_str(), 18);
	recent_percent_label->setPosition(ccp(-70,95));
	m_container->addChild(recent_percent_label, kOnePercentGacha_Z_content);
	
	percent_label = CountingBMLabel::create(CCString::createWithFormat("%.1f%%", recent_percent*100.f)->getCString(), "one_percent_gacha_font.fnt", 0.5f, "%.1f%%", false);
	percent_label->setPosition(ccp(50, 95));
	m_container->addChild(percent_label, kOnePercentGacha_Z_content);
	
	CCSprite* progress_back = CCSprite::create("one_percent_gacha_graph_back.png");
	progress_back->setPosition(ccp(0,10));
	m_container->addChild(progress_back, kOnePercentGacha_Z_content);
	
	progress_img = CCProgressTimer::create(CCSprite::create("one_percent_gacha_graph.png"));
	progress_img->setType(kCCProgressTimerTypeBar);
	progress_img->setMidpoint(ccp(0,0));
	progress_img->setBarChangeRate(ccp(1,0));
	progress_img->setPercentage(0.f);
	progress_img->setPosition(ccp(0, 10));
	m_container->addChild(progress_img, kOnePercentGacha_Z_content);
	
	KSLabelTTF* left_percent = KSLabelTTF::create("0%", mySGD->getFont().c_str(), 14);
	left_percent->enableOuterStroke(ccBLACK, 1.f);
	left_percent->setAnchorPoint(ccp(0,1));
	left_percent->setPosition(ccpAdd(progress_img->getPosition(), ccp(-progress_img->getSprite()->getContentSize().width/2.f-10, -progress_img->getSprite()->getContentSize().height/2.f-2)));
	m_container->addChild(left_percent, kOnePercentGacha_Z_content);
	
	
	KSLabelTTF* right_percent = KSLabelTTF::create("1%", mySGD->getFont().c_str(), 14);
	right_percent->enableOuterStroke(ccBLACK, 1.f);
	right_percent->setAnchorPoint(ccp(1,1));
	right_percent->setPosition(ccpAdd(progress_img->getPosition(), ccp(progress_img->getSprite()->getContentSize().width/2.f+20, -progress_img->getSprite()->getContentSize().height/2.f-2)));
	m_container->addChild(right_percent, kOnePercentGacha_Z_content);
	
	
	arrow_img = CCSprite::create("one_percent_gacha_arrow.png");
	arrow_img->setAnchorPoint(ccp(0.5,0));
	arrow_img->setPosition(ccp(0, 13));
	m_container->addChild(arrow_img, kOnePercentGacha_Z_content);
	
	
	CCLabelTTF* t_label = CCLabelTTF::create();
	
	KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_go100percent), mySGD->getFont().c_str(), 13);
	gacha_label->setColor(ccBLACK);
	gacha_label->setPosition(ccp(0,15));
	t_label->addChild(gacha_label);
	
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("normal_yellow_down.png", CCRectMake(0,0,34,34), CCRectMake(16,16,2,2));
	price_back->setContentSize(CCSizeMake(90, 34));
	price_back->setPosition(ccp(0, -10));
	t_label->addChild(price_back);
	
	if(mySGD->getGoodsValue(kGoodsType_pass5) > 0)
	{
		CCSprite* pass_ticket = CCSprite::create("pass_ticket5.png");
		pass_ticket->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
		price_back->addChild(pass_ticket);
		
		CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
		pass_count_case->setPosition(ccpFromSize(pass_ticket->getContentSize()/2.f) + ccp(9,6));
		pass_ticket->addChild(pass_count_case);
		
		CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass5)), mySGD->getFont().c_str(), 8);
		pass_count_label->setColor(ccc3(255, 255, 255));
		float min_width = 10+pass_count_label->getContentSize().width;
		if(min_width < 20)
			min_width = 20;
		pass_count_case->setContentSize(CCSizeMake(min_width, 20));
		pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
		pass_count_case->addChild(pass_count_label);
		
		KSLabelTTF* free_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_free), mySGD->getFont().c_str(), 20);
		free_label->setColor(ccWHITE);
		free_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
		price_back->addChild(free_label);
	}
	else
	{
		CCSprite* price_type = CCSprite::create("price_ruby_img.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaOnePercentFee())->getCString(), mySGD->getFont().c_str(), 20);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
	}
	
	
	CCScale9Sprite* gacha_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	gacha_button = CCControlButton::create(t_label, gacha_back);
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(OnePercentGacha::gachaAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(170,65));
	gacha_button->setPosition(ccp(0,-70));
	m_container->addChild(gacha_button, kOnePercentGacha_Z_content);
	
	gacha_button->setTouchPriority(-180);
	
	
	
	cancel_menu = CommonButton::createCloseButton();
	cancel_menu->setTouchPriority(-180);
	cancel_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kOnePercentGacha_MT_cancel);
								 menuAction(t_node);
							 });
	cancel_menu->setPosition(ccp(125, 95));
	m_container->addChild(cancel_menu, kOnePercentGacha_Z_content);
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
