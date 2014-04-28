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
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t)
										 {
											 KS::setOpacity(m_container, 255);
											 endShow();
										 }));
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
	
	if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getGachaOnePercentFee())
	{
		myLog->addLog(kLOG_gacha_onePercent, -1);
		
		LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
		addChild(t_loading, 9999);
		
		mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getGachaOnePercentFee(), "99프로가챠");
		
		mySGD->changeGoods([=](Json::Value result_data){
			t_loading->removeFromParent();
			if(result_data["result"]["code"] == GDSUCCESS)
			{
				gacha_button->removeFromParent();
				
				CCLabelTTF* t_label = CCLabelTTF::create();
				
				KSLabelTTF* stop_label = KSLabelTTF::create("버튼을 눌러주세요.", mySGD->getFont().c_str(), 13);
				stop_label->setColor(ccBLACK);
				stop_label->setPosition(ccp(0,15));
				t_label->addChild(stop_label);
				
				KSLabelTTF* stop_label2 = KSLabelTTF::create("STOP", mySGD->getFont().c_str(), 28);
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
				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
				is_menu_enable = true;
			}
		});
	}
	else
	{
		t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		
		getParent()->addChild(t_popup, getZOrder()+1);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setPosition(CCPointZero);
		t_container->addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(250, 150));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6));
		content_back->setPosition(ccp(0,20));
		t_container->addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(230,90));
		
		
		CCSprite* price_type_img1 = CCSprite::create("price_ruby_img.png");
		price_type_img1->setPosition(ccp(-55,35));
		t_container->addChild(price_type_img1);
		
		CCSprite* price_type_img2 = CCSprite::create("price_ruby_img.png");
		price_type_img2->setPosition(ccp(-88,17));
		t_container->addChild(price_type_img2);
		
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
		
		CCMenuItemLambda* cancel_item = CCMenuItemSpriteLambda::create(n_cancel, s_cancel, [=](CCObject* sender)
																	   {
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

																		   t_popup->removeFromParent();
																		   is_menu_enable = true;
																	   });
		
		CCMenuLambda * cancel_menu = CCMenuLambda::createWithItem(cancel_item);
		cancel_menu->setPosition(ccp(-52,-48));
		t_container->addChild(cancel_menu);
		cancel_menu->setTouchPriority(t_popup->getTouchPriority()-2);
		
		
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
																		CCLOG("buy!");
																		
																		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
																		
																		inapp_loading = LoadingLayer::create(-9999, true);
																		addChild(inapp_loading);
																		
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
																		mySGD->addChangeGoods(kGoodsType_ruby, NSDS_GI(kSDS_GI_shopRuby_int1_count_i, 0), "99프로가챠(IOS-인앱결재)", "", "", true);
																		mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getGachaOnePercentFee(), "99프로가챠(IOS-소모)");
																		
																		mySGD->changeGoods([=](Json::Value result_data){
																			inapp_loading->removeFromParent();
																			if(result_data["result"]["code"].asInt() == GDSUCCESS)
																			{
																				t_popup->removeFromParent();
																				
																				gacha_button->removeFromParent();
																				
																				CCLabelTTF* t_label = CCLabelTTF::create();
																				
																				KSLabelTTF* stop_label = KSLabelTTF::create("버튼을 눌러주세요.", mySGD->getFont().c_str(), 13);
																				stop_label->setColor(ccBLACK);
																				stop_label->setPosition(ccp(0,15));
																				t_label->addChild(stop_label);
																				
																				KSLabelTTF* stop_label2 = KSLabelTTF::create("STOP", mySGD->getFont().c_str(), 28);
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
																				
																				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
																				
																				is_menu_enable = true;
																			}
																		});
																		
																		
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
																		Json::Value param;
																		param["productid"] = mySGD->getInappProduct(0);
																		hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
																			//																				KS::KSLog("in-app test \n%", v);
																			if(v["issuccess"].asInt())
																			{
																				mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getGachaOnePercentFee(), "99프로가챠");
																				requestItemDelivery();
																			}
																			else
																			{
																				inapp_loading->removeFromParent();
																				
																				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
																				
																				is_menu_enable = true;
																			}
																		});
#endif
																	});
		
		CCMenuLambda* buy_menu = CCMenuLambda::createWithItem(buy_item);
		buy_menu->setPosition(ccp(52,-48));
		t_container->addChild(buy_menu);
		buy_menu->setTouchPriority(t_popup->getTouchPriority()-2);
		
		t_container->setScale(0);
		CCScaleTo* t_scale = CCScaleTo::create(0.2f, 1.f);
		t_container->runAction(t_scale);
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
												
												KSLabelTTF* stop_label = KSLabelTTF::create("버튼을 눌러주세요.", mySGD->getFont().c_str(), 13);
												stop_label->setColor(ccBLACK);
												stop_label->setPosition(ccp(0,15));
												t_label->addChild(stop_label);
												
												KSLabelTTF* stop_label2 = KSLabelTTF::create("STOP", mySGD->getFont().c_str(), 28);
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
	
	command_list.push_back(mySGD->getChangeGoodsParam(json_selector(mySGD, StarGoldData::saveChangeGoodsTransaction)));
	
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
	
	KSLabelTTF* recent_percent_label = KSLabelTTF::create("현재 획득영역", mySGD->getFont().c_str(), 18);
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
	
	KSLabelTTF* gacha_label = KSLabelTTF::create("보너스 영역을 뽑습니다.", mySGD->getFont().c_str(), 13);
	gacha_label->setColor(ccBLACK);
	gacha_label->setPosition(ccp(0,15));
	t_label->addChild(gacha_label);
	
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("normal_yellow_down.png", CCRectMake(0,0,34,34), CCRectMake(16,16,2,2));
	price_back->setContentSize(CCSizeMake(90, 34));
	price_back->setPosition(ccp(0, -10));
	t_label->addChild(price_back);
	
	
	CCSprite* price_type = CCSprite::create("common_button_ruby.png");
	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
	price_back->addChild(price_type);
	CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaOnePercentFee())->getCString(), mySGD->getFont().c_str(), 12);
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);
	
	
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
