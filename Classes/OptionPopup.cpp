//
//  OptionPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//
#include "StoryLayer.h"
#include "OptionPopup.h"
#include "MyLocalization.h"
//#include "WorldMapScene.h"
#include "PuzzleMapScene.h"
#include "HelpPopup.h"
#include "LogoutPopup.h"
#include "NotiPopup.h"
#include "WithdrawPopup.h"
#include "PuzzleCache.h"
#include "MiniGamePopup.h"
#include "cocos-ext.h"
#include "StageImgLoader.h"
#include "KHAlertView.h"
#include "TutorialScene.h"
#include "MinsuScene.h"
#include "KSUtil.h"
#include "ASPopupView.h"
#include "CommonButton.h"
#include "hspConnector.h"
#include "TitleRenewal.h"

#include "KSLabelTTF.h"
#include "PopCake.h"
#include "EffectSprite.h"
#include "FlagSelector.h"

#include "CouponPopup.h"
#include "StyledLabelTTF.h"
#include "FormSetter.h"
#include "OnePercentGame.h"
#include "FiveRocksCpp.h"
#include "CommonAnimation.h"
#include "Terms.h"
#include "AccountManagerPopup.h"
#include "StyledLabelTTF.h"
#include "OnePercentTutorial.h"
#include "Diary19Popup.h"
#include "LoadingLayer.h"
#include "JoystickSizeQuestionPopup.h"
#include "IntroducerPopup.h"
#include "GDWebSprite.h"
#include "AttendancePopup.h"

#include "LoadingTipScene.h"

USING_NS_CC_EXT;

enum OptionPopupZorder{
	kOP_Z_gray = 1,
	kOP_Z_back,
	kOP_Z_content,
	kOP_Z_popup
};

enum OptionPopupMenuTag{
	kOP_MT_close = 1,
	kOP_MT_bgm,
	kOP_MT_effect,
	kOP_MT_gameui_left,
	kOP_MT_gameui_full,
	kOP_MT_gameui_right,
	kOP_MT_help,
	kOP_MT_toDiary19,
	kOP_MT_logout,
	kOP_MT_noti,
	kOP_MT_withdraw,
	kOP_MT_joystickPositioning,
	kOP_MT_joystickMoving,
	kOP_MT_joystickVib,
	kOP_MT_tutorial,
	kOP_MT_safety,
	kOP_MT_joystickSize,
	kOP_MT_joystickBig,
	kOP_MT_joystickSmall,
	kOP_MT_useSideDirection,
	kOP_MT_joystickSizeQuestion,
	kOP_MT_push,
	kOP_MT_message,
	kOP_MT_coupon,
	kOP_MT_community,
	kOP_MT_recommender,
	kOP_MT_tip,
	kOP_MT_kakao,
	kOP_MT_attendance
};

void OptionPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

//void OptionPopup::completedAnimationSequenceNamed (char const * name)
//{
//	string t_name = name;
//	
//	if(t_name == "Default Timeline" || t_name == "end_die_animation")
//	{
//		CCSprite* remove_target = effect_que.front();
//		effect_que.pop_front();
//		CCBAnimationManager* remove_animation = effect_animation_manager.front();
//		effect_animation_manager.pop_front();
//		remove_animation->setDelegate(NULL);
////		CC_SAFE_RELEASE(dynamic_cast<CCObject*>(remove_animation->getDelegate()));
//		removeChild(remove_target);
//	}
//}

bool OptionPopup::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
//	KSLabelTTF* timeOver = KSLabelTTF::create("타임오버", mySGD->getFont().c_str(), 45.f);
//	
//	timeOver->enableOuterStroke(ccc3(0, 0, 0), 2.5f, 190, true);
//	timeOver->setGradientColor(ccc4(255, 115, 250, 255), ccc4(215, 60, 130, 255), ccp(0, -1));
//	CommonAnimation::applyBigShadow(timeOver, timeOver->getFontSize());
//	addChild(timeOver, 999999);
//	timeOver->setPosition(ccp(240, 160));
	
//	Terms* terms = Terms::create(-999999, [=](){
//		CCLog("END!!!!!");
//	});
//	addChild(terms, 10000);
	
//	KSLabelTTF* ttt= KSLabelTTF::create("대한민국 독립 만세 \n크크크크크 만세", mySGD->getFont().c_str(),
//																			14.f, CCSizeMake(100, 300), CCTextAlignment::kCCTextAlignmentLeft);
//	addChild(ttt, 10000);
//	ttt->setPosition(ccp(240, 160));
//	Json::Value value1, value2, value3, value4, value5, value6;
//	value1["fillcolor"] = StyledLabelTTF::makeRGB(255, 0, 0);
//	value1["font"] = mySGD->getFont().c_str();
//	value1["size"] = 15.f;
//	value2["fillcolor"] = StyledLabelTTF::makeRGB(0, 255, 0);
//	value2["font"] = mySGD->getFont().c_str();
//	value2["size"] = 15.f;
//
//	value3["linebreak"] = true;
//	value3["linespacing"] = 30.f;
//	
//	value4["fillcolor"] = StyledLabelTTF::makeRGB(0, 0, 255);
//	value4["font"] = mySGD->getFont2();
//	value4["size"] = 20.f;
//	
//	value5["fillcolor"] = StyledLabelTTF::makeRGB(255, 0, 255);
//	value5["font"] = mySGD->getFont2();
//	value5["size"] = 25.f;
//	
//	
//	StyledLabelTTF* slttf = StyledLabelTTF::create({
//		{"가을하늘공활한데", value1.toStyledString()},
//		{"높고구름없이", value2.toStyledString()},
//		{"", value3.toStyledString()},
//		{"밝은달은우리가슴", value4.toStyledString()},
//		{"일편단심일세", value5.toStyledString()},
//		{"", value3.toStyledString()}
//	}, StyledAlignment::kCenterAlignment);
//	slttf->setPosition(ccp(240, 320));
//	slttf->setOldAnchorPoint();
//	
//	startFormSetter(this);
//	setFormSetter(slttf);
//	addChild(slttf);
//	{
//		Json::Value value1, value2, value3, value4, value5, value6;
//		value1["fillcolor"] = StyledLabelTTF::makeRGB(255, 0, 0);
//		value1["font"] = mySGD->getFont().c_str();
//		value1["size"] = 15.f;
//		value2["fillcolor"] = StyledLabelTTF::makeRGB(0, 255, 0);
//		value2["font"] = mySGD->getFont().c_str();
//		value2["size"] = 15.f;
//		
//		value3["linebreak"] = true;
//		value3["linespacing"] = 30.f;
//		
//		value4["fillcolor"] = StyledLabelTTF::makeRGB(0, 0, 255);
//		value4["font"] = mySGD->getFont2();
//		value4["size"] = 20.f;
//		
//		value5["fillcolor"] = StyledLabelTTF::makeRGB(255, 0, 255);
//		value5["font"] = mySGD->getFont2();
//		value5["size"] = 25.f;
//		
//		
//		StyledLabelTTF* slttf = StyledLabelTTF::create({
//			{"동해물과백두산이", value1.toStyledString()},
//			{"마르고닳도록", value2.toStyledString()},
//			{"", value3.toStyledString()},
//			{"하느님이보우하사", value4.toStyledString()},
//			{"우리나라만세", value5.toStyledString()},
//			{"", value3.toStyledString()}
//		}, StyledAlignment::kLeftAlignment);
//		slttf->setPosition(ccp(0, 160));
//		addChild(slttf);
//	}
//	OnePercentGame* opg = OnePercentGame::create(0.998, nullptr, nullptr);
//	addChild(opg, 9999);
//	OnePercentTutorial* opt = OnePercentTutorial::create(0.99, nullptr, nullptr);
//	addChild(opt, 9999);
	

//	KSLabelTTF* gradient = KSLabelTTF::create("총점", mySGD->getFont().c_str(), 25.f);
//	gradient->enableOuterStroke(ccc3(0, 0, 0), 1.f);
////	gradient->disableOuterStroke();
//	gradient->enableGradation(ccc4(255, 207, 0, 255), ccc4(255, 113, 0, 255), ccp(0, -1));
//	gradient->setPosition(ccp(100, 100));
//	addChild(gradient, 9999999);
//	fiverocks::FiveRocksBridge::trackPurchase("RUBY_SINGLE_PACK",  // product identifier
//																 "KRW",               // currency code (ISO 4217)
//																 1000,                // price
//																 nullptr);         // campaignId
//	
//	fiverocks::FiveRocksBridge::trackEvent("Economy",             // Event Category
//											 "EarnRuby",                         // Event Name
//											 "BuySingPack",                      // Event Parameter 1 (Acquisition channel)
//											 "Normal",  // Event Parameter 2 (Type)
//											 1);                                 // Event Value
	
//	return true;
//	Json::Value param;
//	param["productid"] = "g_10289_001";
//	hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
//		if(v["issuccess"].asInt())
//		{
////			reqItemDelivery(); // ;
//			Json::Value param;
//			param["memberID"] = hspConnector::get()->getMemberID();
//			GraphDog::get()->command("requestItemDelivery", param, [=](Json::Value t){
//				// 여기서 진짜 갱신.
//				KS::KSLog("refresh !!!!", t);
//			});
//		}
//		KS::KSLog("in-app test \n%", v);
//	});
	
	setTouchEnabled(true);
	
//	addChild(KSSchedule::create([=](float t)
//								{
//									CCPoint t_p;
//									t_p.x = rand()%101-50 + 240;
//									t_p.y = rand()%71-35 + 160;
//									
//									auto t_ccb = KS::loadCCBI<CCSprite*>(this, "bossbomb2.ccbi");//KS::loadCCBI<CCSprite*>(this, "ingame_item_bonustime.ccbi");//KS::loadCCBI<CCSprite*>(this, "fx_cha_die1.ccbi");//KS::loadCCBI<CCSprite*>(this, "startsetting_question.ccbi");//KS::loadCCBI<CCSprite*>(this, "fx_item2.ccbi");
//									
//									CCSprite* take_effect = t_ccb.first;
//									take_effect->setPosition(t_p);
////									take_effect->setScale(1.f/myGD->game_scale);
//									addChild(take_effect, 99999);
//									
//									addChild(KSTimer::create(0.5f, [=](){take_effect->removeFromParent();}));
//									
////									t_ccb.second->setDelegate(this);
//////									t_ccb.second->runAnimationsForSequenceNamed("Default Timeline");
////									effect_animation_manager.push_back(t_ccb.second);
////									
////									effect_que.push_back(take_effect);
//									
//									return true;
//								}));
	
//	addChild(ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
//		
//		CCLOG("ok@@");
//		
//	},12.f,CCPointZero,true,true,[=](){
//		
//		CCLOG("close");
//		
//	}));
	
	
	//MiniGamePopup* t_popup = MiniGamePopup::create((MiniGameCode)(kMiniGameCode_counting), nullptr);
	//addChild(t_popup, 4);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kOP_Z_gray);
	
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kOP_Z_back);
	
	
//	function<void()> ani_func = [=]()
//	{
//		{
//			CCPoint rand_position = ccp(rand()%7-3, rand()%7-3);
//			CCSprite* t_gold = CCSprite::create("icon_g.png");
//			t_gold->setScale(0.7f + (rand()%31 - 15)/100.f);
//			t_gold->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position);
//			main_case->addChild(t_gold, 9999);
//			
//			addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position, ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32) + rand_position, 0.5f, [=](CCPoint t_p)
//													 {
//														 t_gold->setPosition(t_p);
//													 }, [=](CCPoint t_p)
//													 {
//														 t_gold->setPosition(t_p);
//														 t_gold->removeFromParent();
//													 }));
//		}
//		
//		{
//			addChild(KSTimer::create(0.05f, [=]()
//									 {
//										 CCPoint rand_position = ccp(rand()%7-3, rand()%7-3);
//										 CCSprite* t_gold = CCSprite::create("icon_g.png");
//										 t_gold->setScale(0.7f + (rand()%31 - 15)/100.f);
//										 t_gold->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position);
//										 main_case->addChild(t_gold, 9999);
//										 
//										 addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position, ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32) + rand_position, 0.45f, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																				  }, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																					  t_gold->removeFromParent();
//																				  }));
//									 }));
//		}
//		
//		{
//			addChild(KSTimer::create(0.1f, [=]()
//									 {
//										 CCPoint rand_position = ccp(rand()%7-3, rand()%7-3);
//										 CCSprite* t_gold = CCSprite::create("icon_g.png");
//										 t_gold->setScale(0.7f + (rand()%31 - 15)/100.f);
//										 t_gold->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position);
//										 main_case->addChild(t_gold, 9999);
//										 
//										 addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position, ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32) + rand_position, 0.4f, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																				  }, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																					  t_gold->removeFromParent();
//																				  }));
//									 }));
//		}
//		
//		{
//			CCPoint rand_position = ccp(rand()%7-3, rand()%7-3);
//			CCSprite* t_gold = CCSprite::create("icon_g.png");
//			t_gold->setScale(0.7f + (rand()%31 - 15)/100.f);
//			t_gold->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position);
//			main_case->addChild(t_gold, 9999);
//			
//			addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position, ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32) + rand_position, 0.45f, [=](CCPoint t_p)
//													 {
//														 t_gold->setPosition(t_p);
//													 }, [=](CCPoint t_p)
//													 {
//														 t_gold->setPosition(t_p);
//														 t_gold->removeFromParent();
//													 }));
//		}
//		
//		{
//			addChild(KSTimer::create(0.05f, [=]()
//									 {
//										 CCPoint rand_position = ccp(rand()%7-3, rand()%7-3);
//										 CCSprite* t_gold = CCSprite::create("icon_g.png");
//										 t_gold->setScale(0.7f + (rand()%31 - 15)/100.f);
//										 t_gold->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position);
//										 main_case->addChild(t_gold, 9999);
//										 
//										 addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position, ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32) + rand_position, 0.4f, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																				  }, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																					  t_gold->removeFromParent();
//																				  }));
//									 }));
//		}
//		
//		{
//			addChild(KSTimer::create(0.1f, [=]()
//									 {
//										 CCPoint rand_position = ccp(rand()%7-3, rand()%7-3);
//										 CCSprite* t_gold = CCSprite::create("icon_g.png");
//										 t_gold->setScale(0.7f + (rand()%31 - 15)/100.f);
//										 t_gold->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position);
//										 main_case->addChild(t_gold, 9999);
//										 
//										 addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position, ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32) + rand_position, 0.35f, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																				  }, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																					  t_gold->removeFromParent();
//																				  }));
//									 }));
//		}
//		
//		{
//			CCPoint rand_position = ccp(rand()%7-3, rand()%7-3);
//			CCSprite* t_gold = CCSprite::create("icon_g.png");
//			t_gold->setScale(0.7f + (rand()%31 - 15)/100.f);
//			t_gold->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position);
//			main_case->addChild(t_gold, 9999);
//			
//			addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position, ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32) + rand_position, 0.55f, [=](CCPoint t_p)
//													 {
//														 t_gold->setPosition(t_p);
//													 }, [=](CCPoint t_p)
//													 {
//														 t_gold->setPosition(t_p);
//														 t_gold->removeFromParent();
//													 }));
//		}
//		
//		{
//			addChild(KSTimer::create(0.05f, [=]()
//									 {
//										 CCPoint rand_position = ccp(rand()%7-3, rand()%7-3);
//										 CCSprite* t_gold = CCSprite::create("icon_g.png");
//										 t_gold->setScale(0.7f + (rand()%31 - 15)/100.f);
//										 t_gold->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position);
//										 main_case->addChild(t_gold, 9999);
//										 
//										 addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position, ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32) + rand_position, 0.5f, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																				  }, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																					  t_gold->removeFromParent();
//																				  }));
//									 }));
//		}
//		
//		{
//			addChild(KSTimer::create(0.1f, [=]()
//									 {
//										 CCPoint rand_position = ccp(rand()%7-3, rand()%7-3);
//										 CCSprite* t_gold = CCSprite::create("icon_g.png");
//										 t_gold->setScale(0.7f + (rand()%31 - 15)/100.f);
//										 t_gold->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position);
//										 main_case->addChild(t_gold, 9999);
//										 
//										 addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32) + rand_position, ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32) + rand_position, 0.45f, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																				  }, [=](CCPoint t_p)
//																				  {
//																					  t_gold->setPosition(t_p);
//																					  t_gold->removeFromParent();
//																				  }));
//									 }));
//		}
//		
//		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin_total.png");
//		CCSprite* t_coin = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 30, 30));
//		t_coin->setScale(0.3f);
//		CCAnimation* t_animation = CCAnimation::create();
//		t_animation->setDelayPerUnit(0.1f);
//		int add_count = 0;
//		for(int i=0;add_count < 6;i=(i+1)%6)
//		{
//			add_count++;
//			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*30, 0, 30, 30));
//		}
//		CCAnimate* t_animate = CCAnimate::create(t_animation);
//		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
//		t_coin->runAction(t_repeat);
//		
//		t_coin->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32));
//		main_case->addChild(t_coin, 9999);
//		
//		t_coin->addChild(KSGradualValue<float>::create(0.3f, 1.f, 0.2f, [=](float t_f)
//											   {
//												   t_coin->setScale(t_f);
//											   }, [=](float t_f)
//											   {
//												   t_coin->setScale(t_f);
//												   t_coin->addChild(KSTimer::create(0.1f, [=]()
//																					{
//																						t_coin->addChild(KSGradualValue<float>::create(1.f, 0.3f, 0.2f, [=](float t_f)
//																																	   {
//																																		   t_coin->setScale(t_f);
//																																	   }, [=](float t_f)
//																																	   {
//																																		   t_coin->setScale(t_f);
//																																	   }));
//																					}));
//											   }));
//		
//		addChild(KSGradualValue<CCPoint>::create(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-105,-32), ccpFromSize(main_case->getContentSize()/2.f) + ccp(105,-32), 0.5f, [=](CCPoint t_p)
//												 {
//													 t_coin->setPosition(t_p);
//												 }, [=](CCPoint t_p)
//												 {
//													 t_coin->setPosition(t_p);
//													 t_coin->removeFromParent();
//												 }));
//
//		
//	};
//	
//	ani_func();
//	for(int i=1;i<9999;i++)
//	{
//		addChild(KSTimer::create(i, ani_func));
//	}
	
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_option), mySGD->getFont().c_str(), 15);
	title_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);

	CommonAnimation::applyShadow(title_label);


	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(424, 194));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f + 5);
	main_case->addChild(main_inner);
	
	if(myDSH->isCheatKeyEnable())
	{
		CCSprite* n_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		n_temp->setOpacity(0);
		CCSprite* s_temp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 30, 30));
		s_temp->setOpacity(0);
		
		CCMenuItemSprite* temp_item = CCMenuItemSprite::create(n_temp, s_temp, this, menu_selector(OptionPopup::cheatAction));
		temp_item->setTag(1);
		CCMenu* temp_menu = CCMenu::createWithItem(temp_item);
		temp_menu->setPosition(ccp(15,main_case->getContentSize().height+15));
		main_case->addChild(temp_menu, kOP_Z_content);
		temp_menu->setTouchPriority(-171);
	}
	
	CCMenu* tab_menu = CCMenu::create();
	tab_menu->setPosition(CCPointZero);
	main_case->addChild(tab_menu, kOP_Z_content);
	tab_menu->setTouchPriority(-171);
	
	CCSprite* n_coupon_img = CCSprite::create("tabbutton_up.png");
	KSLabelTTF* n_coupon_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_regCoupon), mySGD->getFont().c_str(), 12.5f);
	n_coupon_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_coupon_label->setPosition(ccpFromSize(n_coupon_img->getContentSize()/2.f) + ccp(0,2));
	n_coupon_img->addChild(n_coupon_label);
	
	CCSprite* s_coupon_img = CCSprite::create("tabbutton_up.png");
	s_coupon_img->setColor(ccGRAY);
	KSLabelTTF* s_coupon_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_regCoupon), mySGD->getFont().c_str(), 12.5f);
	s_coupon_label->disableOuterStroke();
	s_coupon_label->setColor(ccGRAY);
	s_coupon_label->setPosition(ccpFromSize(s_coupon_img->getContentSize()/2.f) + ccp(0,2));
	s_coupon_img->addChild(s_coupon_label);
	
	CCSprite* d_coupon_img = CCSprite::create("tabbutton_down.png");
	d_coupon_img->setColor(ccGRAY);
	KSLabelTTF* d_coupon_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_regCoupon), mySGD->getFont().c_str(), 12.5f);
	d_coupon_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_coupon_label->setPosition(ccpFromSize(d_coupon_img->getContentSize()/2.f) + ccp(0,2));
	d_coupon_img->addChild(d_coupon_label);
	
	
	CCMenuItem* coupon_item = CCMenuItemSprite::create(n_coupon_img, s_coupon_img, d_coupon_img, this, menu_selector(OptionPopup::menuAction));
	coupon_item->setTag(kOP_MT_coupon);
	coupon_item->setPosition(ccp(394,256.5f));
	tab_menu->addChild(coupon_item);
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	coupon_item->setVisible(mySGD->getIosMenuVisible() && (mySGD->getIosHideVer() != graphdog->getAppVersionString()));
#endif
	
	CCSprite* n_community_img = CCSprite::create("tabbutton_up.png");
	KSLabelTTF* n_community_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_community), mySGD->getFont().c_str(), 12.5f);
	n_community_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_community_label->setPosition(ccpFromSize(n_community_img->getContentSize()/2.f) + ccp(0,2));
	n_community_img->addChild(n_community_label);
	
	CCSprite* s_community_img = CCSprite::create("tabbutton_up.png");
	s_community_img->setColor(ccGRAY);
	KSLabelTTF* s_community_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_community), mySGD->getFont().c_str(), 12.5f);
	s_community_label->disableOuterStroke();
	s_community_label->setColor(ccGRAY);
	s_community_label->setPosition(ccpFromSize(s_community_img->getContentSize()/2.f) + ccp(0,2));
	s_community_img->addChild(s_community_label);
	
	CCSprite* d_community_img = CCSprite::create("tabbutton_down.png");
	d_community_img->setColor(ccGRAY);
	KSLabelTTF* d_community_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_community), mySGD->getFont().c_str(), 12.5f);
	d_community_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_community_label->setPosition(ccpFromSize(d_community_img->getContentSize()/2.f) + ccp(0,2));
	d_community_img->addChild(d_community_label);
	
	
	CCMenuItem* community_item = CCMenuItemSprite::create(n_community_img, s_community_img, d_community_img, this, menu_selector(OptionPopup::menuAction));
	community_item->setTag(kOP_MT_community);
	community_item->setPosition(ccp(319,256.5f));
	tab_menu->addChild(community_item);
	
	
	CCSprite* n_tip_img = CCSprite::create("tabbutton_up.png");
	KSLabelTTF* n_tip_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_recommenderReg), mySGD->getFont().c_str(), 12.5f);
	n_tip_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_tip_label->setPosition(ccpFromSize(n_tip_img->getContentSize()/2.f) + ccp(0,2));
	n_tip_img->addChild(n_tip_label);
	
	CCSprite* s_tip_img = CCSprite::create("tabbutton_up.png");
	s_tip_img->setColor(ccGRAY);
	KSLabelTTF* s_tip_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_recommenderReg), mySGD->getFont().c_str(), 12.5f);
	s_tip_label->disableOuterStroke();
	s_tip_label->setColor(ccGRAY);
	s_tip_label->setPosition(ccpFromSize(s_tip_img->getContentSize()/2.f) + ccp(0,2));
	s_tip_img->addChild(s_tip_label);
	
	CCSprite* d_tip_img = CCSprite::create("tabbutton_down.png");
	d_tip_img->setColor(ccGRAY);
	KSLabelTTF* d_tip_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_recommenderReg), mySGD->getFont().c_str(), 12.5f);
	d_tip_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_tip_label->setPosition(ccpFromSize(d_tip_img->getContentSize()/2.f) + ccp(0,2));
	d_tip_img->addChild(d_tip_label);
	
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip_img, s_tip_img, d_tip_img, this, menu_selector(OptionPopup::menuAction));
	tip_item->setTag(kOP_MT_recommender);
	tip_item->setPosition(ccp(244,256.5f));
	tab_menu->addChild(tip_item);
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	tip_item->setVisible(mySGD->getIosMenuVisible() && (mySGD->getIosHideVer() != graphdog->getAppVersionString()));
#endif
	
	CCSprite* n_kakao_img = CCSprite::create("option_kakao.png");
//	KSLabelTTF* n_kakao_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_accountLink), mySGD->getFont().c_str(), 12.5f);
//	n_kakao_label->setPosition(ccpFromSize(n_kakao_img->getContentSize()/2.f) + ccp(0,-1));
//	n_kakao_img->addChild(n_kakao_label);
	
	CCSprite* s_kakao_img = CCSprite::create("option_kakao.png");
	s_kakao_img->setColor(ccGRAY);
//	KSLabelTTF* s_kakao_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_accountLink), mySGD->getFont().c_str(), 12.5f);
//	s_kakao_label->setColor(ccGRAY);
//	s_kakao_label->setPosition(ccpFromSize(s_kakao_img->getContentSize()/2.f) + ccp(0,-1));
//	s_kakao_img->addChild(s_kakao_label);
	
	CCSprite* d_kakao_img = CCSprite::create("option_kakao.png");
	d_kakao_img->setColor(ccGRAY);
//	KSLabelTTF* d_kakao_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_accountLink), mySGD->getFont().c_str(), 12.5f);
//	d_kakao_label->setPosition(ccpFromSize(d_kakao_img->getContentSize()/2.f) + ccp(0,-1));
//	d_kakao_img->addChild(d_kakao_label);
	
	
//	CCMenuItem* kakao_item = CCMenuItemSprite::create(n_kakao_img, s_kakao_img, d_kakao_img, this, menu_selector(OptionPopup::menuAction));
//	kakao_item->setTag(kOP_MT_kakao);
//	kakao_item->setPosition(ccp(169,256.5f));
//	tab_menu->addChild(kakao_item);
	
	CCSprite* n_attendance_img = CCSprite::create("tabbutton_up.png");
	KSLabelTTF* n_attendance_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kAttendanceCheck), mySGD->getFont().c_str(), 12.5f);
	n_attendance_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_attendance_label->setPosition(ccpFromSize(n_tip_img->getContentSize()/2.f) + ccp(0,2));
	n_attendance_img->addChild(n_attendance_label);
	
	CCSprite* s_attendance_img = CCSprite::create("tabbutton_up.png");
	s_attendance_img->setColor(ccGRAY);
	KSLabelTTF* s_attendance_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kAttendanceCheck), mySGD->getFont().c_str(), 12.5f);
	s_attendance_label->disableOuterStroke();
	s_attendance_label->setColor(ccGRAY);
	s_attendance_label->setPosition(ccpFromSize(s_attendance_img->getContentSize()/2.f) + ccp(0,2));
	s_attendance_img->addChild(s_attendance_label);
	
	CCSprite* d_attendance_img = CCSprite::create("tabbutton_down.png");
	d_attendance_img->setColor(ccGRAY);
	KSLabelTTF* d_attendance_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kAttendanceCheck), mySGD->getFont().c_str(), 12.5f);
	d_attendance_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_attendance_label->setPosition(ccpFromSize(d_attendance_img->getContentSize()/2.f) + ccp(0,2));
	d_attendance_img->addChild(d_attendance_label);
	
	
	CCMenuItem* attendance_item = CCMenuItemSprite::create(n_attendance_img, s_attendance_img, d_attendance_img, this, menu_selector(OptionPopup::menuAction));
	attendance_item->setTag(kOP_MT_attendance);
	attendance_item->setPosition(ccp(169,256.5f));
	tab_menu->addChild(attendance_item);
	
	
	
	
	CommonButton* close_menu = CommonButton::createCloseButton(-171);
	close_menu->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
													{
														CCNode* t_node = CCNode::create();
														t_node->setTag(kOP_MT_close);
														menuAction(t_node);
													});
	main_case->addChild(close_menu, kOP_Z_content);
	
	
	CCSprite* system_tab = CCSprite::create("option_tab.png");
	system_tab->setPosition(ccp(50, 210));
	main_case->addChild(system_tab, kOP_Z_back);
	
	KSLabelTTF* system_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_system), mySGD->getFont().c_str(), 12);
	system_label->setColor(ccc3(20, 50, 70));
	system_label->disableOuterStroke();
	system_label->setPosition(ccp(system_tab->getContentSize().width*0.45f, system_tab->getContentSize().height*0.5f));
	system_tab->addChild(system_label);
	
	system_tab->setStringData("testobj");
	
	KSLabelTTF* bgm_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_bgm), mySGD->getFont().c_str(), 11.5f);
//	bgm_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    bgm_label->disableOuterStroke();
	bgm_label->setAnchorPoint(ccp(0,0.5f));
	bgm_label->setPosition(ccpAdd(getContentPosition(kOP_MT_bgm), ccp(-100,0)));
	main_case->addChild(bgm_label, kOP_Z_back);
	
	CCSprite* bgm_scroll = CCSprite::create("option_scroll.png");
	bgm_scroll->setPosition(getContentPosition(kOP_MT_bgm));
	main_case->addChild(bgm_scroll, kOP_Z_back);
	
	CCSprite* n_bgm = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	n_bgm->setOpacity(0);
	CCSprite* s_bgm = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	s_bgm->setOpacity(0);
	
	CCMenuItem* bgm_item = CCMenuItemSprite::create(n_bgm, s_bgm, this, menu_selector(OptionPopup::menuAction));
	bgm_item->setTag(kOP_MT_bgm);
	
	bgm_menu = CCMenu::createWithItem(bgm_item);
	bgm_menu->setPosition(getContentPosition(kOP_MT_bgm));
	main_case->addChild(bgm_menu, kOP_Z_content);
	bgm_menu->setTouchPriority(-171);
	
	bgm_img = NULL;
	resetBgmMenu();
	
	
	
	KSLabelTTF* effect_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_effect), mySGD->getFont().c_str(), 11.5f);
//	effect_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    effect_label->disableOuterStroke();
	effect_label->setAnchorPoint(ccp(0,0.5f));
	effect_label->setPosition(ccpAdd(getContentPosition(kOP_MT_effect), ccp(-100,0)));
	main_case->addChild(effect_label, kOP_Z_back);
	
	CCSprite* effect_scroll = CCSprite::create("option_scroll.png");
	effect_scroll->setPosition(getContentPosition(kOP_MT_effect));
	main_case->addChild(effect_scroll, kOP_Z_back);
	
	CCSprite* n_effect = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	n_effect->setOpacity(0);
	CCSprite* s_effect = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	s_effect->setOpacity(0);
	
	CCMenuItem* effect_item = CCMenuItemSprite::create(n_effect, s_effect, this, menu_selector(OptionPopup::menuAction));
	effect_item->setTag(kOP_MT_effect);
	
	effect_menu = CCMenu::createWithItem(effect_item);
	effect_menu->setPosition(getContentPosition(kOP_MT_effect));
	main_case->addChild(effect_menu, kOP_Z_content);
	effect_menu->setTouchPriority(-171);
	
	effect_img = NULL;
	resetEffectMenu();
	
	
	KSLabelTTF* joystick_position_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_joystickPosition), mySGD->getFont().c_str(), 11.5f);
	joystick_position_label->disableOuterStroke();
//	joystick_position_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	joystick_position_label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	joystick_position_label->setAnchorPoint(ccp(0,0.5f));
	joystick_position_label->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickPositioning), ccp(-100,0)));
	main_case->addChild(joystick_position_label, kOP_Z_back);
	
	CCSprite* joystick_position_scroll = CCSprite::create("option_scroll.png");
	joystick_position_scroll->setPosition(getContentPosition(kOP_MT_joystickPositioning));
	main_case->addChild(joystick_position_scroll, kOP_Z_back);
	
	CCSprite* n_joystick_positioning = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	n_joystick_positioning->setOpacity(0);
	CCSprite* s_joystick_positioning = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	s_joystick_positioning->setOpacity(0);
	
	CCMenuItem* joystick_positioning_item = CCMenuItemSprite::create(n_joystick_positioning, s_joystick_positioning, this, menu_selector(OptionPopup::menuAction));
	joystick_positioning_item->setTag(kOP_MT_joystickPositioning);
	
	joystick_positioning_menu = CCMenu::createWithItem(joystick_positioning_item);
	joystick_positioning_menu->setPosition(getContentPosition(kOP_MT_joystickPositioning));
	main_case->addChild(joystick_positioning_menu, kOP_Z_content);
	joystick_positioning_menu->setTouchPriority(-171);
	
	joystick_positioning_img = NULL;
	resetJoystickPositioningMenu();
	
	
	KSLabelTTF* joystick_moving_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_joystickControl), mySGD->getFont().c_str(), 11.5f);
	joystick_moving_label->disableOuterStroke();
	joystick_moving_label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	joystick_moving_label->setAnchorPoint(ccp(0,0.5f));
	joystick_moving_label->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickMoving), ccp(-100,0)));
	main_case->addChild(joystick_moving_label, kOP_Z_back);
	
	CCSprite* joystick_moving_scroll = CCSprite::create("option_scroll.png");
	joystick_moving_scroll->setPosition(getContentPosition(kOP_MT_joystickMoving));
	main_case->addChild(joystick_moving_scroll, kOP_Z_back);
	
	CCSprite* n_joystick_moving = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	n_joystick_moving->setOpacity(0);
	CCSprite* s_joystick_moving = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	s_joystick_moving->setOpacity(0);
	
	CCMenuItem* joystick_moving_item = CCMenuItemSprite::create(n_joystick_moving, s_joystick_moving, this, menu_selector(OptionPopup::menuAction));
	joystick_moving_item->setTag(kOP_MT_joystickMoving);
	
	joystick_moving_menu = CCMenu::createWithItem(joystick_moving_item);
	joystick_moving_menu->setPosition(getContentPosition(kOP_MT_joystickMoving));
	main_case->addChild(joystick_moving_menu, kOP_Z_content);
	joystick_moving_menu->setTouchPriority(-171);
	
	joystick_moving_img = NULL;
	resetJoystickMovingMenu();
	
	
	KSLabelTTF* joystick_size_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_joystickSize), mySGD->getFont().c_str(), 11.5f);
	joystick_size_label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
//	joystick_size_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    joystick_size_label->disableOuterStroke();
	joystick_size_label->setAnchorPoint(ccp(0,0.5f));
	joystick_size_label->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickSize), ccp(-100,0)));
	main_case->addChild(joystick_size_label, kOP_Z_back);
	
	CCSprite* joystick_size_scroll = CCSprite::create("option_scroll2.png");
	joystick_size_scroll->setPosition(getContentPosition(kOP_MT_joystickSize) + ccp(-13,0));
	main_case->addChild(joystick_size_scroll, kOP_Z_back);
	
	CCSprite* n_joystick_big = CCSprite::create("option_plus.png");
	KSLabelTTF* n_big_label = KSLabelTTF::create("+", mySGD->getFont().c_str(), 12);
	n_big_label->setPosition(ccpFromSize(n_joystick_big->getContentSize()/2.f));
	n_joystick_big->addChild(n_big_label);
	CCSprite* s_joystick_big = CCSprite::create("option_plus.png");
	s_joystick_big->setColor(ccGRAY);
	KSLabelTTF* s_big_label = KSLabelTTF::create("+", mySGD->getFont().c_str(), 12);
	s_big_label->setPosition(ccpFromSize(s_joystick_big->getContentSize()/2.f));
	s_joystick_big->addChild(s_big_label);
	
	CCMenuItem* joystick_size_big_item = CCMenuItemSprite::create(n_joystick_big, s_joystick_big, this, menu_selector(OptionPopup::menuAction));
	joystick_size_big_item->setTag(kOP_MT_joystickBig);
	joystick_size_big_item->setPosition(ccp(18,0));
	
	
	CCSprite* n_joystick_small = CCSprite::create("option_plus.png");
	KSLabelTTF* n_small_label = KSLabelTTF::create("-", mySGD->getFont().c_str(), 12);
	n_small_label->setPosition(ccpFromSize(n_joystick_small->getContentSize()/2.f));
	n_joystick_small->addChild(n_small_label);
	CCSprite* s_joystick_small = CCSprite::create("option_plus.png");
	s_joystick_small->setColor(ccGRAY);
	KSLabelTTF* s_small_label = KSLabelTTF::create("-", mySGD->getFont().c_str(), 12);
	s_small_label->setPosition(ccpFromSize(s_joystick_small->getContentSize()/2.f));
	s_joystick_small->addChild(s_small_label);
	
	CCMenuItem* joystick_size_small_item = CCMenuItemSprite::create(n_joystick_small, s_joystick_small, this, menu_selector(OptionPopup::menuAction));
	joystick_size_small_item->setTag(kOP_MT_joystickSmall);
	joystick_size_small_item->setPosition(ccp(-45,0));
	
	
	CCMenu* joystick_size_menu = CCMenu::create();
	joystick_size_menu->setPosition(getContentPosition(kOP_MT_joystickSize)+ccp(1,0));
	main_case->addChild(joystick_size_menu, kOP_Z_content);
	joystick_size_menu->setTouchPriority(-171);
	
	joystick_size_menu->addChild(joystick_size_big_item);
	joystick_size_menu->addChild(joystick_size_small_item);
	
	joystick_size_value = KSLabelTTF::create(ccsf("%d", myDSH->getIntegerForKey(kDSH_Key_joystickSize)+5), mySGD->getFont().c_str(), 17);
	joystick_size_value->enableOuterStroke(ccBLACK, 1.5f, 255*0.6f, true);
	joystick_size_value->setPosition(ccp(joystick_size_scroll->getContentSize().width/2.f, joystick_size_scroll->getContentSize().height/2.f-1.5f));
	joystick_size_scroll->addChild(joystick_size_value);
	
	
	CCSprite* n_question = CCSprite::create("option_question.png");
	CCSprite* s_question = CCSprite::create("option_question.png");
	s_question->setColor(ccGRAY);
	
	CCMenuItem* question_item = CCMenuItemSprite::create(n_question, s_question, this, menu_selector(OptionPopup::menuAction));
	question_item->setTag(kOP_MT_joystickSizeQuestion);
	
	CCMenu* question_menu = CCMenu::createWithItem(question_item);
	question_menu->setPosition(getContentPosition(kOP_MT_joystickSize) + ccp(47, 0));
	main_case->addChild(question_menu, kOP_Z_content);
	question_menu->setTouchPriority(-171);
	
	
	KSLabelTTF* joystick_vib_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_joystickVib), mySGD->getFont().c_str(), 11.5f);
	joystick_vib_label->disableOuterStroke();
	joystick_vib_label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	joystick_vib_label->setAnchorPoint(ccp(0,0.5f));
	joystick_vib_label->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickVib), ccp(-100,0)));
	main_case->addChild(joystick_vib_label, kOP_Z_back);
	
	CCSprite* joystick_vib_scroll = CCSprite::create("option_scroll.png");
	joystick_vib_scroll->setPosition(getContentPosition(kOP_MT_joystickVib));
	main_case->addChild(joystick_vib_scroll, kOP_Z_back);
	
	CCSprite* n_joystick_vib = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	n_joystick_vib->setOpacity(0);
	CCSprite* s_joystick_vib = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	s_joystick_vib->setOpacity(0);
	
	CCMenuItem* joystick_vib_item = CCMenuItemSprite::create(n_joystick_vib, s_joystick_vib, this, menu_selector(OptionPopup::menuAction));
	joystick_vib_item->setTag(kOP_MT_joystickVib);
	
	joystick_vib_menu = CCMenu::createWithItem(joystick_vib_item);
	joystick_vib_menu->setPosition(getContentPosition(kOP_MT_joystickVib));
	main_case->addChild(joystick_vib_menu, kOP_Z_content);
	joystick_vib_menu->setTouchPriority(-171);
	
	joystick_vib_img = NULL;
	resetJoystickVibMenu();
	
	
//	myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection)
	KSLabelTTF* use_side_direction_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_useSideDirection), mySGD->getFont().c_str(), 11.5f);
	use_side_direction_label->disableOuterStroke();
	use_side_direction_label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	use_side_direction_label->setAnchorPoint(ccp(0,0.5f));
	use_side_direction_label->setPosition(ccpAdd(getContentPosition(kOP_MT_useSideDirection), ccp(-100,0)));
	main_case->addChild(use_side_direction_label, kOP_Z_back);
	
	CCSprite* use_side_direction_scroll = CCSprite::create("option_scroll.png");
	use_side_direction_scroll->setPosition(getContentPosition(kOP_MT_useSideDirection));
	main_case->addChild(use_side_direction_scroll, kOP_Z_back);
	
	CCSprite* n_use_side_direction_moving = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	n_use_side_direction_moving->setOpacity(0);
	CCSprite* s_use_side_direction_moving = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	s_use_side_direction_moving->setOpacity(0);
	
	CCMenuItem* use_side_direction_item = CCMenuItemSprite::create(n_use_side_direction_moving, s_use_side_direction_moving, this, menu_selector(OptionPopup::menuAction));
	use_side_direction_item->setTag(kOP_MT_useSideDirection);
	
	use_side_direction_menu = CCMenu::createWithItem(use_side_direction_item);
	use_side_direction_menu->setPosition(getContentPosition(kOP_MT_useSideDirection));
	main_case->addChild(use_side_direction_menu, kOP_Z_content);
	use_side_direction_menu->setTouchPriority(-171);
	
	use_side_direction_img = NULL;
	resetUseSideDirectionMenu();
	
	
	
	
	KSLabelTTF* safety_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_safetyMode), mySGD->getFont().c_str(), 11.5f);
	safety_label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
//	safety_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    safety_label->disableOuterStroke();
	safety_label->setAnchorPoint(ccp(0,0.5f));
	safety_label->setPosition(ccpAdd(getContentPosition(kOP_MT_safety), ccp(-100,0)));
	main_case->addChild(safety_label, kOP_Z_back);
	
	CCSprite* safety_scroll = CCSprite::create("option_scroll.png");
	safety_scroll->setPosition(getContentPosition(kOP_MT_safety));
	main_case->addChild(safety_scroll, kOP_Z_back);
	
	CCSprite* n_safety = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	n_safety->setOpacity(0);
	CCSprite* s_safety = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	s_safety->setOpacity(0);
	
	CCMenuItem* safety_item = CCMenuItemSprite::create(n_safety, s_safety, this, menu_selector(OptionPopup::menuAction));
	safety_item->setTag(kOP_MT_safety);
	
	safety_menu = CCMenu::createWithItem(safety_item);
	safety_menu->setPosition(getContentPosition(kOP_MT_safety));
	main_case->addChild(safety_menu, kOP_Z_content);
	safety_menu->setTouchPriority(-171);
	
	safety_img = NULL;
	resetSafetyMenu();
	
	
	CCSprite* alert_tab = CCSprite::create("option_tab.png");
	alert_tab->setPosition(ccp(50, 88));
	main_case->addChild(alert_tab, kOP_Z_back);
	
	KSLabelTTF* alert_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noti), mySGD->getFont().c_str(), 12);
	alert_label->setColor(ccc3(20, 50, 70));
	alert_label->disableOuterStroke();
	alert_label->setPosition(ccp(alert_tab->getContentSize().width*0.45f, alert_tab->getContentSize().height*0.5f));
	alert_tab->addChild(alert_label);
	
	
	KSLabelTTF* push_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_pushNoti), mySGD->getFont().c_str(), 11.5f);
//	push_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    push_label->disableOuterStroke();
	push_label->setAnchorPoint(ccp(0,0.5f));
	push_label->setPosition(ccpAdd(getContentPosition(kOP_MT_push), ccp(-100,0)));
	main_case->addChild(push_label, kOP_Z_back);
	
	CCSprite* push_scroll = CCSprite::create("option_scroll.png");
	push_scroll->setPosition(getContentPosition(kOP_MT_push));
	main_case->addChild(push_scroll, kOP_Z_back);
	
	CCSprite* n_push = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	n_push->setOpacity(0);
	CCSprite* s_push = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
	s_push->setOpacity(0);
	
	CCMenuItem* push_item = CCMenuItemSprite::create(n_push, s_push, this, menu_selector(OptionPopup::menuAction));
	push_item->setTag(kOP_MT_push);
	
	push_menu = CCMenu::createWithItem(push_item);
	push_menu->setPosition(getContentPosition(kOP_MT_push));
	main_case->addChild(push_menu, kOP_Z_content);
	push_menu->setTouchPriority(-171);
	
	myDSH->setBoolForKey(kDSH_Key_isPushOff, !myHSP->getIsEnablePushNotification());
	
	push_img = NULL;
	resetPushMenu();
	
	
	
	CCSprite* n_tutorial_img = CCSprite::create("subbutton_purple3.png");
	KSLabelTTF* n_tutorial_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kTutorialReplay), mySGD->getFont().c_str(), 12.5f);
	n_tutorial_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_tutorial_label->setPosition(ccpFromSize(n_tutorial_img->getContentSize()/2.f) + ccp(0,0));
	n_tutorial_img->addChild(n_tutorial_label);
	
	CCSprite* s_tutorial_img = CCSprite::create("subbutton_purple3.png");
	s_tutorial_img->setColor(ccGRAY);
	KSLabelTTF* s_tutorial_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kTutorialReplay), mySGD->getFont().c_str(), 12.5f);
	s_tutorial_label->setColor(ccGRAY);
	s_tutorial_label->disableOuterStroke();
	s_tutorial_label->setPosition(ccpFromSize(s_tutorial_img->getContentSize()/2.f) + ccp(0,0));
	s_tutorial_img->addChild(s_tutorial_label);
	
	CCSprite* d_tutorial_img = CCSprite::create("subbutton_purple3.png");
	d_tutorial_img->setColor(ccGRAY);
	KSLabelTTF* d_tutorial_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kTutorialReplay), mySGD->getFont().c_str(), 12.5f);
	d_tutorial_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_tutorial_label->setPosition(ccpFromSize(d_tutorial_img->getContentSize()/2.f) + ccp(0,0));
	d_tutorial_img->addChild(d_tutorial_label);
	
	
	CCMenuItem* tutorial_item = CCMenuItemSprite::create(n_tutorial_img, s_tutorial_img, d_tutorial_img, this, menu_selector(OptionPopup::menuAction));
	tutorial_item->setTag(kOP_MT_tutorial);
	tutorial_item->setPosition(getContentPosition(kOP_MT_message) + ccp(-50, 0));
	tab_menu->addChild(tutorial_item);

	
//	KSLabelTTF* message_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_messageNoti), mySGD->getFont().c_str(), 11.5f);
//	message_label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
//	message_label->setAnchorPoint(ccp(0,0.5f));
//	message_label->disableOuterStroke();
//	message_label->setPosition(ccpAdd(getContentPosition(kOP_MT_message), ccp(-100,0)));
//	main_case->addChild(message_label, kOP_Z_back);
//	
//	CCSprite* message_scroll = CCSprite::create("option_scroll.png");
//	message_scroll->setPosition(getContentPosition(kOP_MT_message));
//	main_case->addChild(message_scroll, kOP_Z_back);
//	
//	CCSprite* n_message = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
//	n_message->setOpacity(0);
//	CCSprite* s_message = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 115, 30));
//	s_message->setOpacity(0);
//	
//	CCMenuItem* message_item = CCMenuItemSprite::create(n_message, s_message, this, menu_selector(OptionPopup::menuAction));
//	message_item->setTag(kOP_MT_message);
//	
//	message_menu = CCMenu::createWithItem(message_item);
//	message_menu->setPosition(getContentPosition(kOP_MT_message));
//	main_case->addChild(message_menu, kOP_Z_content);
//	message_menu->setTouchPriority(-171);
//	
//	message_img = NULL;
//	resetMessageMenu();
	
	
	CCScale9Sprite* info_back = CCScale9Sprite::create("option_info_back.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	info_back->setContentSize(CCSizeMake(350, 35));
	info_back->setPosition(ccp(272, 54));
	main_case->addChild(info_back, kOP_Z_content);
	
	CCSprite* info_line = CCSprite::create("common_line.png");
	info_line->setRotation(90);
	info_line->setScaleX(18/info_line->getContentSize().width);
	info_line->setPosition(ccp(220, 17.5f));
	info_back->addChild(info_line);
	
	KSLabelTTF* id_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_memberID), mySGD->getFont().c_str(), 11.5f);
	id_label->setColor(ccc3(54, 36, 148));
	id_label->disableOuterStroke();
	id_label->setAnchorPoint(ccp(0,0.5f));
	id_label->setPosition(ccp(10,info_back->getContentSize().height/2.f));
	info_back->addChild(id_label);
	
	
	KSLabelTTF* my_id_label = KSLabelTTF::create(myHSP->getSocialID()/*KS::longLongToStrForDG(mySGD->user_index)*/.c_str(), mySGD->getFont().c_str(), 11.5f);
	my_id_label->setColor(ccc3(54, 36, 148));
	my_id_label->disableOuterStroke();
	my_id_label->setAnchorPoint(ccp(0,0.5));
	my_id_label->setPosition(ccp(id_label->getPositionX() + id_label->getContentSize().width + 3,info_back->getContentSize().height/2.f));
	info_back->addChild(my_id_label, kOP_Z_content);
	
	
	KSLabelTTF* version_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gameversion), mySGD->getFont().c_str(), 11.5f);
	version_label->setColor(ccc3(54, 36, 148));
	version_label->disableOuterStroke();
	version_label->setAnchorPoint(ccp(0,0.5f));
	version_label->setPosition(ccp(225,info_back->getContentSize().height/2.f));
	info_back->addChild(version_label, kOP_Z_back);
	setFormSetter(version_label);
	
	KSLabelTTF* my_version_label = KSLabelTTF::create(GraphDog::get()->getAppVersionString().c_str(),
																										mySGD->getFont().c_str(), 11.5f);
	my_version_label->setColor(ccc3(54, 36, 148));
	my_version_label->disableOuterStroke();
	my_version_label->setAnchorPoint(ccp(0,0.5));
	my_version_label->setPosition(ccp(285.0, info_back->getContentSize().height/2.f));
	info_back->addChild(my_version_label, kOP_Z_content);
	setFormSetter(my_version_label);
	
	
	CCSprite* n_withdraw_img = CCSprite::create("option_withdraw.png");
	KSLabelTTF* n_withdraw_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_withdraw), mySGD->getFont().c_str(), 11.5f);
	n_withdraw_label->setColor(ccc3(54, 36, 148));
	n_withdraw_label->disableOuterStroke();
	n_withdraw_label->setPosition(ccpFromSize(n_withdraw_img->getContentSize()/2.f) + ccp(0,0));
	n_withdraw_img->addChild(n_withdraw_label);
	
	CCSprite* s_withdraw_img = CCSprite::create("option_withdraw.png");
	s_withdraw_img->setColor(ccGRAY);
	KSLabelTTF* s_withdraw_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_withdraw), mySGD->getFont().c_str(), 11.5f);
	s_withdraw_label->setColor(ccc3(54, 36, 148));
	s_withdraw_label->disableOuterStroke();
	s_withdraw_label->setPosition(ccpFromSize(s_withdraw_img->getContentSize()/2.f) + ccp(0,0));
	s_withdraw_img->addChild(s_withdraw_label);
	
	CCSprite* d_withdraw_img = CCSprite::create("option_withdraw.png");
	d_withdraw_img->setColor(ccGRAY);
	KSLabelTTF* d_withdraw_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_withdraw), mySGD->getFont().c_str(), 11.5f);
	d_withdraw_label->setColor(ccc3(54, 36, 148));
	d_withdraw_label->disableOuterStroke();
	d_withdraw_label->setPosition(ccpFromSize(d_withdraw_img->getContentSize()/2.f) + ccp(0,0));
	d_withdraw_img->addChild(d_withdraw_label);
	
	
	CCMenuItem* withdraw_item = CCMenuItemSprite::create(n_withdraw_img, s_withdraw_img, d_withdraw_img, this, menu_selector(OptionPopup::menuAction));
	withdraw_item->setTag(kOP_MT_withdraw);
	withdraw_item->setPosition(getContentPosition(kOP_MT_withdraw));
	tab_menu->addChild(withdraw_item);
	
	
	//	CCSprite* n_logout = CCSprite::create("option_logout.png");
	//	CCSprite* s_logout = CCSprite::create("option_logout.png");
	//	s_logout->setColor(ccGRAY);
	//
	//	CCMenuItem* logout_item = CCMenuItemSprite::create(n_logout, s_logout, this, menu_selector(OptionPopup::menuAction));
	//	logout_item->setTag(kOP_MT_logout);
	//
	//	CCMenu* logout_menu = CCMenu::createWithItem(logout_item);
	//	logout_menu->setPosition(getContentPosition(kOP_MT_logout));
	//	main_case->addChild(logout_menu, kOP_Z_content);
	//	logout_menu->setTouchPriority(-171);
	
	
	CCSprite* n_accountLink_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_accountLink_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_accountLink), mySGD->getFont().c_str(), 12.5f);
	n_accountLink_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_accountLink_label->setPosition(ccpFromSize(n_accountLink_img->getContentSize()/2.f) + ccp(0,-1));
	n_accountLink_img->addChild(n_accountLink_label);
	
	CCSprite* s_accountLink_img = CCSprite::create("subbutton_pink.png");
	s_accountLink_img->setColor(ccGRAY);
	KSLabelTTF* s_accountLink_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_accountLink), mySGD->getFont().c_str(), 12.5f);
	s_accountLink_label->setColor(ccGRAY);
	s_accountLink_label->disableOuterStroke();
	s_accountLink_label->setPosition(ccpFromSize(s_accountLink_img->getContentSize()/2.f) + ccp(0,-1));
	s_accountLink_img->addChild(s_accountLink_label);
	
	CCSprite* d_accountLink_img = CCSprite::create("subbutton_pink.png");
	d_accountLink_img->setColor(ccGRAY);
	KSLabelTTF* d_accountLink_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_accountLink), mySGD->getFont().c_str(), 12.5f);
	d_accountLink_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_accountLink_label->setPosition(ccpFromSize(d_accountLink_img->getContentSize()/2.f) + ccp(0,-1));
	d_accountLink_img->addChild(d_accountLink_label);
	
	
	CCMenuItem* accountLink_item = CCMenuItemSprite::create(n_accountLink_img, s_accountLink_img, d_accountLink_img, this, menu_selector(OptionPopup::menuAction));
	accountLink_item->setTag(kOP_MT_tip);
	accountLink_item->setPosition(ccp(395,16));
	tab_menu->addChild(accountLink_item);
	
	
	CCSprite* n_help_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_help_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_callCenter), mySGD->getFont().c_str(), 12.5f);
	n_help_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_help_label->setPosition(ccpFromSize(n_help_img->getContentSize()/2.f) + ccp(0,-1));
	n_help_img->addChild(n_help_label);
	
	CCSprite* s_help_img = CCSprite::create("subbutton_pink.png");
	s_help_img->setColor(ccGRAY);
	KSLabelTTF* s_help_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_callCenter), mySGD->getFont().c_str(), 12.5f);
	s_help_label->setColor(ccGRAY);
	s_help_label->disableOuterStroke();
	s_help_label->setPosition(ccpFromSize(s_help_img->getContentSize()/2.f) + ccp(0,-1));
	s_help_img->addChild(s_help_label);
	
	CCSprite* d_help_img = CCSprite::create("subbutton_pink.png");
	d_help_img->setColor(ccGRAY);
	KSLabelTTF* d_help_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_callCenter), mySGD->getFont().c_str(), 12.5f);
	d_help_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_help_label->setPosition(ccpFromSize(d_help_img->getContentSize()/2.f) + ccp(0,-1));
	d_help_img->addChild(d_help_label);
	
	
	CCMenuItem* help_item = CCMenuItemSprite::create(n_help_img, s_help_img, d_help_img, this, menu_selector(OptionPopup::menuAction));
	help_item->setTag(kOP_MT_help);
	help_item->setPosition(ccp(293,16));//395
	tab_menu->addChild(help_item);
	
	
	
//	CCSprite* n_diary_img = CCSprite::create("option_19.png");
//	KSLabelTTF* n_diary_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_toDiary19), mySGD->getFont().c_str(), 12.5f);
//	n_diary_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
//	n_diary_label->setPosition(ccpFromSize(n_diary_img->getContentSize()/2.f) + ccp(0,-1));
//	n_diary_img->addChild(n_diary_label);
//	
//	CCSprite* s_diary_img = CCSprite::create("option_19.png");
//	s_diary_img->setColor(ccGRAY);
//	KSLabelTTF* s_diary_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_toDiary19), mySGD->getFont().c_str(), 12.5f);
//	s_diary_label->setColor(ccGRAY);
//	s_diary_label->disableOuterStroke();
//	s_diary_label->setPosition(ccpFromSize(s_diary_img->getContentSize()/2.f) + ccp(0,-1));
//	s_diary_img->addChild(s_diary_label);
//	
//	CCSprite* d_diary_img = CCSprite::create("option_19.png");
//	d_diary_img->setColor(ccGRAY);
//	KSLabelTTF* d_diary_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_toDiary19), mySGD->getFont().c_str(), 12.5f);
//	d_diary_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
//	d_diary_label->setPosition(ccpFromSize(d_diary_img->getContentSize()/2.f) + ccp(0,-1));
//	d_diary_img->addChild(d_diary_label);
//	
//	
//	CCMenuItem* diary_item = CCMenuItemSprite::create(n_diary_img, s_diary_img, d_diary_img, this, menu_selector(OptionPopup::menuAction));
//	diary_item->setTag(kOP_MT_toDiary19);
//	diary_item->setPosition(ccp(191,16));//293
//	tab_menu->addChild(diary_item);

	
	//	CCSprite* n_noti = CCSprite::create("option_noti.png");
	//	CCSprite* s_noti = CCSprite::create("option_noti.png");
	//	s_noti->setColor(ccGRAY);
	//
	//	CCMenuItem* noti_item = CCMenuItemSprite::create(n_noti, s_noti, this, menu_selector(OptionPopup::menuAction));
	//	noti_item->setTag(kOP_MT_noti);
	//
	//	CCMenu* noti_menu = CCMenu::createWithItem(noti_item);
	//	noti_menu->setPosition(getContentPosition(kOP_MT_noti));
	//	main_case->addChild(noti_menu, kOP_Z_content);
	//	noti_menu->setTouchPriority(-171);
	
	is_menu_enable = false;
	
	
	
//	
//	GDWebSprite::printCache();
//	GDWebSprite::removeCache();
//	CCSprite* gdwe = GDWebSprite::create("http://182.162.201.147:10010/images/ui/startpack.png", "icon_r.png", CCSizeMake(910/2.f, 440/2.f));
//	gdwe->setAnchorPoint(ccp(0.5,0.5));
//	gdwe->setPosition(ccp(100,100));
//	addChild(gdwe,99999);
	
//	StoryLayer::startStory("puzzle1",[](){ CCLOG("ENDED STORY");});
	
	
	
	
	
	
	return true;
}

void OptionPopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void OptionPopup::showPopup()
{
//	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
//	gray->runAction(gray_fade);
//	
//	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-14.f));
//	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(OptionPopup::endShowPopup));
//	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
//	main_case->runAction(main_seq);
	
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, bind(&OptionPopup::endShowPopup, this));
}

void OptionPopup::endShowPopup()
{
	is_menu_enable = true;
}

void OptionPopup::hidePopup()
{
	is_menu_enable = false;
	CommonAnimation::closePopup(this, main_case, gray, [=](){
		
	}, bind(&OptionPopup::endHidePopup, this));
	
}

void OptionPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void OptionPopup::cheatAction(CCObject *sender)
{
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == 1) // gold up 10000
	{
		myDSH->setIntegerForKey(kDSH_Key_savedGold, myDSH->getIntegerForKey(kDSH_Key_savedGold)+10000);
	}
}

void OptionPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kOP_MT_close)
	{
		hidePopup();
	}
	else if(tag == kOP_MT_bgm)
	{
		myDSH->setBoolForKey(kDSH_Key_bgmOff, !myDSH->getBoolForKey(kDSH_Key_bgmOff));
		AudioEngine::sharedInstance()->setSoundOnOff(!myDSH->getBoolForKey(kDSH_Key_bgmOff));
		resetBgmMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_effect)
	{
		myDSH->setBoolForKey(kDSH_Key_effectOff, !myDSH->getBoolForKey(kDSH_Key_effectOff));
		AudioEngine::sharedInstance()->setEffectOnOff(!myDSH->getBoolForKey(kDSH_Key_effectOff));
		resetEffectMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_gameui_left)
	{
		myGD->setUItype(kGT_leftUI);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_gameui_full)
	{
		myGD->setUItype(kGT_full);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_gameui_right)
	{
		myGD->setUItype(kGT_rightUI);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_help)
	{
		myHSP->openCSCenter("");
//		addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_communityNotOpenTitle), myLoc->getLocalForKey(LK::kMyLocalKey_communityNotOpenContent)), 999);
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_attendance)
	{
		AttendancePopup* t_popup = AttendancePopup::create(-300, [=]()
														   {
															   is_menu_enable = true;
														   });
		addChild(t_popup, kOP_Z_popup);
	}
	else if(tag == kOP_MT_recommender)
	{
		if(mySGD->getIntroducerID() == 0)
		{
			IntroducerPopup* t_popup = IntroducerPopup::create(-999, [=](){is_menu_enable = true;}, [=]()
			{
				is_menu_enable = false;
				CommonAnimation::closePopup(this, main_case, gray, [=](){
					
				}, [=]()
											{
												open_message_popup_func();
												endHidePopup();
											});
			});
			addChild(t_popup, 999);
		}
		else
		{
			addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_introducerAfter), [=](){is_menu_enable = true;}), 999);
		}
	}
	else if(tag == kOP_MT_toDiary19)
	{
		if(graphdog->isExistApp())
		{
			// 다이어리 앱 설치되어 있음
			
			LoadingLayer* t_loading = LoadingLayer::create(-9999);
			addChild(t_loading, 9999);
			t_loading->startLoading();
			
			Json::Value t_param;
			t_param["memberID"] = myHSP->getMemberID();
			
			myHSP->command("makediarycode", t_param, [=](Json::Value result_data)
						   {
							   if(result_data["result"]["code"].asInt() == GDSUCCESS)
								{
									graphdog->openDiaryApp(t_param["memberID"].asString(), result_data["diaryCode"].asString(), -1); // 다이어리 앱 실행 result_data["diaryCode"].asString() 과 myHSP->getMemberID() 를 보내줌
								}
							   else
								{
									CCLOG("failed makediarycode");
								}
							   
							   t_loading->removeFromParent();
							   is_menu_enable = true;
						   });
		}
		else
		{
			Diary19Popup* t_popup = Diary19Popup::create(-999, [=]()
														 {
															 is_menu_enable = true;
														 });
			addChild(t_popup, 999);
		}
	}
	else if(tag == kOP_MT_logout)
	{
		//		LogoutPopup* t_lp = LogoutPopup::create(this, callfunc_selector(OptionPopup::popupClose));
		//		addChild(t_lp, kOP_Z_popup);
		
		ASPopupView* t_popup = ASPopupView::create(-300);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, 160));
		t_popup->setBasePosition(ccp(240, 160));
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		addChild(t_popup, kOP_Z_popup);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setPosition(ccp(0,0));
		t_container->addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(240, 210));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		content_back->setPosition(ccp(0,25));
		t_container->addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(220, 140));
		
		KSLabelTTF* ment_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_checkLogout),	mySGD->getFont().c_str(), 15);
		ment_label->setPosition(ccp(0,25));
		t_container->addChild(ment_label);
		
		
		
		CommonButton* cancel_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_cancel), 15, CCSizeMake(100, 50), CommonButtonGreen, t_popup->getTouchPriority()-5);
		cancel_button->setPosition(ccp(-55,-75));
		cancel_button->setFunction([=](CCObject* sender)
															 {
																 is_menu_enable = true;
																 t_popup->removeFromParent();
															 });
		t_container->addChild(cancel_button);
		
		
		CommonButton* ok_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), 15, CCSizeMake(100, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
		ok_button->setPosition(ccp(55,-75));
		ok_button->setFunction([=](CCObject* sender)
													 {
														 cancel_button->setEnabled(false);
														 ok_button->setEnabled(false);
														 
														 cancel_button->setVisible(false);
														 ok_button->setVisible(false);
														 
//														 CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//														 CCBReader* reader = new CCBReader(nodeLoader);
//														 CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
														 CCSprite* loading_progress_img = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
														 loading_progress_img->setPosition(ccp(0,-75));
														 t_container->addChild(loading_progress_img);
//														 reader->release();
														 
														 
														 hspConnector::get()->logout([=](Json::Value result_data)
																												 {
																													 CCLOG("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
																													 if(result_data["error"]["isSuccess"].asBool())
																													 {
																														 myDSH->clear();
																														 myDSH->resetDSH();
																														 CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
																													 }
																													 else
																													 {
																														 loading_progress_img->removeFromParent();
																														 cancel_button->setEnabled(true);
																														 ok_button->setEnabled(true);
																														 
																														 cancel_button->setVisible(true);
																														 ok_button->setVisible(true);
																														 
																														 CCLOG("fail logout");
																													 }
																												 });
													 });
		t_container->addChild(ok_button);
	}
	else if(tag == kOP_MT_noti)
	{
		NotiPopup* t_np = NotiPopup::create(this, callfunc_selector(OptionPopup::popupClose));
		addChild(t_np, kOP_Z_popup);
	}
	else if(tag == kOP_MT_withdraw)
	{
		//		WithdrawPopup* t_wp = WithdrawPopup::create(this, callfunc_selector(OptionPopup::popupClose));
		//		addChild(t_wp, kOP_Z_popup);
		
		TRACE();
		ASPopupView* t_popup = ASPopupView::create(-300);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, 160));
		t_popup->setBasePosition(ccp(240, 160));
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		addChild(t_popup, kOP_Z_popup);
		
		CCSprite* case_back = CCSprite::create("popup_small_back.png");
		case_back->setPosition(ccp(0,0));
		t_container->addChild(case_back);
		
		CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		back_in->setContentSize(CCSizeMake(case_back->getContentSize().width-45, case_back->getContentSize().height-123));
		back_in->setPosition(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f+8));
		case_back->addChild(back_in);
		
		
		KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_withdrawTitle), mySGD->getFont().c_str(), 12);
		title_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
		title_label->setAnchorPoint(ccp(0.5f,0.5f));
		title_label->setPosition(ccp(-85, case_back->getContentSize().height/2.f-35));
		t_container->addChild(title_label);
		
		KSLabelTTF* ment1_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_withdrawContent1), mySGD->getFont().c_str(), 12);
		ment1_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
		ment1_label->setAnchorPoint(ccp(0.5f,0.5f));
		ment1_label->setPosition(ccp(0,28));
		t_container->addChild(ment1_label);
		
		KSLabelTTF* ment2_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_withdrawContent2), mySGD->getFont().c_str(), 12);
		ment2_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
		ment2_label->setAnchorPoint(ccp(0.5f,0.5f));
		ment2_label->setPosition(ccp(0,8));
		t_container->addChild(ment2_label);
		
		KSLabelTTF* ment3_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_withdrawContent3), mySGD->getFont().c_str(), 12);
		ment3_label->setColor(ccc3(255, 170, 20));
		ment3_label->disableOuterStroke();
		ment3_label->setAnchorPoint(ccp(0.5f,0.5f));
		ment3_label->setPosition(ccp(0,-12));
		t_container->addChild(ment3_label);
		
		
		
		CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
		close_button->setPosition(ccp(case_back->getContentSize().width/2.f-22, case_back->getContentSize().height/2.f-25));
		close_button->setFunction([=](CCObject* sender)
								  {
									  is_menu_enable = true;
									  t_popup->removeFromParent();
								  });
		t_container->addChild(close_button);
		
		
		CommonButton* ok_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_doWithdraw), 12, CCSizeMake(101, 44), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), t_popup->getTouchPriority()-5);
		ok_button->setPosition(ccp(0,-53));
		ok_button->setFunction([=](CCObject* sender)
													 {
														 close_button->setEnabled(false);
														 ok_button->setEnabled(false);
														 
														 close_button->setVisible(false);
														 ok_button->setVisible(false);
														 
//														 CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//														 CCBReader* reader = new CCBReader(nodeLoader);
//														 CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
														 CCSprite* loading_progress_img = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
														 loading_progress_img->setPosition(ccp(0,-53));
														 t_container->addChild(loading_progress_img);
//														 reader->release();
														 
														 Json::Value param;
														 TRACE();
														 param["memberID"] = hspConnector::get()->getSocialID();// hspConnector::get()->myKakaoInfo["user_id"].asString(); 기능 제거한다고 하면서 그래프도그가 수정됨
														 hspConnector::get()->command("dropoutuser", param, [=](Json::Value result_data)
																													{
																														TRACE();

																														if(result_data["result"]["code"].asInt() == GDSUCCESS)
																														{
																															TRACE();
																															hspConnector::get()->withdrawAccount([](Json::Value p){
																																TRACE();
																																myDSH->clear();
																																myDSH->resetDSH();
																																mySGD->withdraw();
																																CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
																															});
																															
																														}
																														else
																														{
																															KS::KSLog("%", result_data["result"]);
																															TRACE();
																															loading_progress_img->removeFromParent();
																															close_button->setEnabled(true);
																															ok_button->setEnabled(true);
																															
																															close_button->setVisible(true);
																															ok_button->setVisible(true);
																															
																															CCLOG("fail dropoutuser");
																														}
																													});
													 });
		t_container->addChild(ok_button);
	}
	else if(tag == kOP_MT_joystickPositioning)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_left)
			myDSH->setIntegerForKey(kDSH_Key_controlJoystickDirection, kControlJoystickDirection_right);
		else if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_right)
			myDSH->setIntegerForKey(kDSH_Key_controlJoystickDirection, kControlJoystickDirection_left);
		
		resetJoystickPositioningMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_joystickMoving)
	{
		myDSH->setBoolForKey(kDSH_Key_isJoystickCenterNotFixed, !myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed));
		resetJoystickMovingMenu();
		is_menu_enable = true;
		
		//		myDSH->setBoolForKey(kDSH_Key_isControlJoystickNotFixed, !myDSH->getBoolForKey(kDSH_Key_isControlJoystickNotFixed));
		//		resetJoystickMovingMenu();
		//		is_menu_enable = true;
	}
	else if(tag == kOP_MT_joystickVib)
	{
		myDSH->setBoolForKey(kDSH_Key_isOffJoystickVib, !myDSH->getBoolForKey(kDSH_Key_isOffJoystickVib));
		resetJoystickVibMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_joystickBig)
	{
		int recent_size = myDSH->getIntegerForKey(kDSH_Key_joystickSize)+5;
		if(recent_size < 9)
		{
			myDSH->setIntegerForKey(kDSH_Key_joystickSize, recent_size+1-5);
			joystick_size_value->setString(ccsf("%d", recent_size+1));
		}
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_joystickSmall)
	{
		int recent_size = myDSH->getIntegerForKey(kDSH_Key_joystickSize)+5;
		if(recent_size > 1)
		{
			myDSH->setIntegerForKey(kDSH_Key_joystickSize, recent_size-1-5);
			joystick_size_value->setString(ccsf("%d", recent_size-1));
		}
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_useSideDirection)
	{
		myDSH->setBoolForKey(kDSH_Key_isEnableIrregularDirection, !myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection));
		resetUseSideDirectionMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_joystickSizeQuestion)
	{
		JoystickSizeQuestionPopup* t_popup = JoystickSizeQuestionPopup::create(-300, [=](){is_menu_enable = true;});
		addChild(t_popup, kOP_Z_popup);
	}
	else if(tag == kOP_MT_tutorial)
	{
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_init);
		mySGD->is_option_tutorial = true;
		mySGD->resetLabels();
		
		AudioEngine::sharedInstance()->stopSound();
		
		mySGD->setNextSceneName("playtutorial");
		
		AudioEngine::sharedInstance()->preloadEffectScene("playtutorial");
		
		addChild(KSTimer::create(0.3f, [=]()
								 {
									 LoadingTipScene* loading_tip = LoadingTipScene::getLoadingTipSceneLayer();
									 addChild(loading_tip, kOP_Z_popup);
								 }));
		
//		CCDirector::sharedDirector()->replaceScene(TutorialScene::scene());
	}
	else if(tag == kOP_MT_safety)
	{
		myDSH->setBoolForKey(kDSH_Key_isSafetyMode, !myDSH->getBoolForKey(kDSH_Key_isSafetyMode));
		mySGD->is_safety_mode = myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
		
		string ment_str;
		if(mySGD->is_safety_mode)
			ment_str = myLoc->getLocalForKey(LK::kMyLocalKey_optionSecretOn);
		else
			ment_str = myLoc->getLocalForKey(LK::kMyLocalKey_optionSecretOff);
		
		StyledLabelTTF* ment_label = StyledLabelTTF::create(ment_str.c_str(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
		ment_label->setAnchorPoint(ccp(0.5f,0.5f));
		addChild(ASPopupView::getCommonNoti(-300, myLoc->getLocalForKey(LK::kMyLocalKey_optionSecretTitle), ment_label, [=](){is_menu_enable = true;}), kOP_Z_popup);
		
		resetSafetyMenu();
	}
	else if(tag == kOP_MT_push)
	{
		myDSH->setBoolForKey(kDSH_Key_isPushOff, !myDSH->getBoolForKey(kDSH_Key_isPushOff));
		myHSP->setIsEnablePushNotification(!myDSH->getBoolForKey(kDSH_Key_isPushOff));
		resetPushMenu();
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_message)
	{
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_coupon)
	{
		CouponPopup* t_popup = CouponPopup::create(-300, [=](){is_menu_enable = true;}, [=]()
		{
			is_menu_enable = false;
			CommonAnimation::closePopup(this, main_case, gray, [=](){
				
			}, [=]()
										{
											open_message_popup_func();
											endHidePopup();
										});
			
		});
		addChild(t_popup, kOP_Z_popup);
	}
	else if(tag == kOP_MT_community)
	{
		myHSP->openHSPUrl("http://cafe.naver.com/ddmksk");
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_tip)
	{
		AccountManagerPopup* amp = AccountManagerPopup::create(-172);
		addChild(amp, kOP_Z_popup);
		
		is_menu_enable = true;
	}
	else if(tag == kOP_MT_kakao)
	{
		
		Json::Value msgInfo = mySGD->getKakaoMsg();
		
		string msg = msgInfo["msg"].asString();
		GraphDogLib::ReplaceString(msg,"[p1]",myDSH->getStringForKey(kDSH_Key_nick).c_str());
		int ret = hspConnector::get()->sendKakaoMsg(msgInfo["title"].asString(),msg.c_str(),msgInfo["url"].asString());
		
		
		if(ret == 0) {
			auto ment = StyledLabelTTF::create(getLocal(LK::kFriendNeedKakao),
																				 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
			ment->setAnchorPoint(ccp(0.5f, 0.5f));
			ASPopupView* as = ASPopupView::getCommonNoti2(-172, "에러",
																										ment, nullptr, ccp(0, 0), true);
			addChild(as, kOP_Z_content);
		}
		
		is_menu_enable = true;
	}
}

void OptionPopup::popupClose()
{
	is_menu_enable = true;
}

void OptionPopup::resetBgmMenu()
{
	if(bgm_img)
	{
		bgm_img->removeFromParent();
		bgm_img = NULL;
	}
	
	bgm_img = CCSprite::create("subbutton_purple2.png");
	main_case->addChild(bgm_img, kOP_Z_content);
	
	string inner_text;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_bgmOff))
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOff);
		img_position = ccp(28,0);
	}
	else
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOn);
		img_position = ccp(-28,0);
	}
	
	KSLabelTTF* inner_label = KSLabelTTF::create(inner_text.c_str(), mySGD->getFont().c_str(), 12);
//	inner_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    inner_label->disableOuterStroke();
	inner_label->setPosition(ccp(bgm_img->getContentSize().width/2.f, bgm_img->getContentSize().height/2.f));
	bgm_img->addChild(inner_label);
	
	bgm_img->setPosition(ccpAdd(getContentPosition(kOP_MT_bgm), img_position));
}

void OptionPopup::resetEffectMenu()
{
	if(effect_img)
	{
		effect_img->removeFromParent();
		effect_img = NULL;
	}
	
	effect_img = CCSprite::create("subbutton_purple2.png");
	main_case->addChild(effect_img, kOP_Z_content);
	
	string inner_text;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_effectOff))
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOff);
		img_position = ccp(28,0);
	}
	else
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOn);
		img_position = ccp(-28,0);
	}
	
	KSLabelTTF* inner_label = KSLabelTTF::create(inner_text.c_str(), mySGD->getFont().c_str(), 12);
//	inner_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    inner_label->disableOuterStroke();
	inner_label->setPosition(ccp(effect_img->getContentSize().width/2.f, effect_img->getContentSize().height/2.f));
	effect_img->addChild(inner_label);
	
	
	effect_img->setPosition(ccpAdd(getContentPosition(kOP_MT_effect), img_position));
}

void OptionPopup::resetJoystickPositioningMenu()
{
	if(joystick_positioning_img)
	{
		joystick_positioning_img->removeFromParent();
		joystick_positioning_img = NULL;
	}
	
	joystick_positioning_img = CCSprite::create("subbutton_purple2.png");
	main_case->addChild(joystick_positioning_img, kOP_Z_content);
	
	string inner_text;
	CCPoint img_position;
	if(myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection) == kControlJoystickDirection_right)
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_right);
		img_position = ccp(28,0);
	}
	else
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_left);
		img_position = ccp(-28,0);
	}
	
	KSLabelTTF* inner_label = KSLabelTTF::create(inner_text.c_str(), mySGD->getFont().c_str(), 12);
//	inner_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    inner_label->disableOuterStroke();
	inner_label->setPosition(ccp(joystick_positioning_img->getContentSize().width/2.f, joystick_positioning_img->getContentSize().height/2.f));
	joystick_positioning_img->addChild(inner_label);
	
	joystick_positioning_img->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickPositioning), img_position));
}

void OptionPopup::resetJoystickMovingMenu()
{
	if(joystick_moving_img)
	{
		joystick_moving_img->removeFromParent();
		joystick_moving_img = NULL;
	}
	
	joystick_moving_img = CCSprite::create("subbutton_purple2.png");
	main_case->addChild(joystick_moving_img, kOP_Z_content);
	
	string inner_text;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed))//ControlJoystickNotFixed))
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_fix);
		img_position = ccp(-28,0);
	}
	else
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_move);
		img_position = ccp(28,0);
	}
	
	KSLabelTTF* inner_label = KSLabelTTF::create(inner_text.c_str(), mySGD->getFont().c_str(), 12);
//	inner_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    inner_label->disableOuterStroke();
	inner_label->setPosition(ccp(joystick_moving_img->getContentSize().width/2.f, joystick_moving_img->getContentSize().height/2.f));
	joystick_moving_img->addChild(inner_label);
	
	joystick_moving_img->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickMoving), img_position));
}

void OptionPopup::resetJoystickVibMenu()
{
	if(joystick_vib_img)
	{
		joystick_vib_img->removeFromParent();
		joystick_vib_img = NULL;
	}
	
	joystick_vib_img = CCSprite::create("subbutton_purple2.png");
	main_case->addChild(joystick_vib_img, kOP_Z_content);
	
	string inner_text;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_isOffJoystickVib))//ControlJoystickNotFixed))
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOff);
		img_position = ccp(28,0);
	}
	else
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOn);
		img_position = ccp(-28,0);
	}
	
	KSLabelTTF* inner_label = KSLabelTTF::create(inner_text.c_str(), mySGD->getFont().c_str(), 12);
//	inner_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    inner_label->disableOuterStroke();
	inner_label->setPosition(ccp(joystick_vib_img->getContentSize().width/2.f, joystick_vib_img->getContentSize().height/2.f));
	joystick_vib_img->addChild(inner_label);
	
	joystick_vib_img->setPosition(ccpAdd(getContentPosition(kOP_MT_joystickVib), img_position));
}

void OptionPopup::resetUseSideDirectionMenu()
{
	if(use_side_direction_img)
	{
		use_side_direction_img->removeFromParent();
		use_side_direction_img = NULL;
	}
	
	use_side_direction_img = CCSprite::create("subbutton_purple2.png");
	main_case->addChild(use_side_direction_img, kOP_Z_content);
	
	string inner_text;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection))//ControlJoystickNotFixed))
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOn);
		img_position = ccp(-28,0);
	}
	else
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOff);
		img_position = ccp(28,0);
	}
	
	KSLabelTTF* inner_label = KSLabelTTF::create(inner_text.c_str(), mySGD->getFont().c_str(), 12);
//	inner_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    inner_label->disableOuterStroke();
	inner_label->setPosition(ccp(use_side_direction_img->getContentSize().width/2.f, use_side_direction_img->getContentSize().height/2.f));
	use_side_direction_img->addChild(inner_label);
	
	use_side_direction_img->setPosition(ccpAdd(getContentPosition(kOP_MT_useSideDirection), img_position));
}

void OptionPopup::resetSafetyMenu()
{
	if(safety_img)
	{
		safety_img->removeFromParent();
		safety_img = NULL;
	}
	
	safety_img = CCSprite::create("subbutton_purple2.png");
	main_case->addChild(safety_img, kOP_Z_content);
	
	string inner_text;
	CCPoint img_position;
	if(!myDSH->getBoolForKey(kDSH_Key_isSafetyMode))
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOff);
		img_position = ccp(28,0);
	}
	else
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOn);
		img_position = ccp(-28,0);
	}
	
	KSLabelTTF* inner_label = KSLabelTTF::create(inner_text.c_str(), mySGD->getFont().c_str(), 12);
//	inner_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    inner_label->disableOuterStroke();
	inner_label->setPosition(ccp(safety_img->getContentSize().width/2.f, safety_img->getContentSize().height/2.f));
	safety_img->addChild(inner_label);
	
	safety_img->setPosition(ccpAdd(getContentPosition(kOP_MT_safety), img_position));
}

void OptionPopup::resetPushMenu()
{
	if(push_img)
	{
		push_img->removeFromParent();
		push_img = NULL;
	}
	
	push_img = CCSprite::create("subbutton_purple2.png");
	main_case->addChild(push_img, kOP_Z_content);
	
	string inner_text;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_isPushOff))
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOff);
		img_position = ccp(28,0);
	}
	else
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOn);
		img_position = ccp(-28,0);
	}
	
	KSLabelTTF* inner_label = KSLabelTTF::create(inner_text.c_str(), mySGD->getFont().c_str(), 12);
//	inner_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    inner_label->disableOuterStroke();
	inner_label->setPosition(ccp(push_img->getContentSize().width/2.f, push_img->getContentSize().height/2.f));
	push_img->addChild(inner_label);
	
	push_img->setPosition(ccpAdd(getContentPosition(kOP_MT_push), img_position));
}

void OptionPopup::resetMessageMenu()
{
	if(message_img)
	{
		message_img->removeFromParent();
		message_img = NULL;
	}
	
	message_img = CCSprite::create("subbutton_purple2.png");
	main_case->addChild(message_img, kOP_Z_content);
	
	string inner_text;
	CCPoint img_position;
	if(myDSH->getBoolForKey(kDSH_Key_isMessageOff))
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOff);
		img_position = ccp(28,0);
	}
	else
	{
		inner_text = myLoc->getLocalForKey(LK::kMyLocalKey_lightOn);
		img_position = ccp(-28,0);
	}
	
	KSLabelTTF* inner_label = KSLabelTTF::create(inner_text.c_str(), mySGD->getFont().c_str(), 12);
//	inner_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
    inner_label->disableOuterStroke();
	inner_label->setPosition(ccp(message_img->getContentSize().width/2.f, message_img->getContentSize().height/2.f));
	message_img->addChild(inner_label);
	
	message_img->setPosition(ccpAdd(getContentPosition(kOP_MT_message), img_position));
}

CCPoint OptionPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kOP_MT_close)				return_value = ccp(448, 256);
	
	else if(t_tag == kOP_MT_gameui_left)	return_value = ccp(143, 113);
	else if(t_tag == kOP_MT_gameui_full)	return_value = ccp(204, 113);
	else if(t_tag == kOP_MT_gameui_right)	return_value = ccp(265, 113);
	
	else if(t_tag == kOP_MT_bgm)					return_value = ccp(200, 210);
	else if(t_tag == kOP_MT_effect)					return_value = ccp(380, 210);
	else if(t_tag == kOP_MT_joystickPositioning)	return_value = ccp(200, 180);
	else if(t_tag == kOP_MT_joystickMoving)			return_value = ccp(380, 180);
	else if(t_tag == kOP_MT_joystickSize)			return_value = ccp(200, 150);
	else if(t_tag == kOP_MT_joystickVib)			return_value = ccp(380, 150);
	else if(t_tag == kOP_MT_useSideDirection)		return_value = ccp(200, 120);
	else if(t_tag == kOP_MT_safety)					return_value = ccp(380, 120);
	else if(t_tag == kOP_MT_push)					return_value = ccp(200, 88);
	else if(t_tag == kOP_MT_message)				return_value = ccp(380, 88);
	
	else if(t_tag == kOP_MT_withdraw)		return_value = ccp(65, 54);
	else if(t_tag == kOP_MT_logout)			return_value = ccp(273, 43);
	
	else if(t_tag == kOP_MT_coupon)			return_value = ccp(390, 256);
	else if(t_tag == kOP_MT_community)		return_value = ccp(313, 256);
	else if(t_tag == kOP_MT_tip)			return_value = ccp(236, 256);
	else if(t_tag == kOP_MT_kakao)		return_value = ccp(157, 256);
	
	else if(t_tag == kOP_MT_help)			return_value = ccp(410, 36);
	
	else if(t_tag == kOP_MT_noti)			return_value = ccp(117, 54);
	
	return return_value;
}

bool OptionPopup::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void OptionPopup::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void OptionPopup::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void OptionPopup::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void OptionPopup::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
