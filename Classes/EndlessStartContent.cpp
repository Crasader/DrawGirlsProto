//
//  EndlessStartContent.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 21..
//
//

#include "EndlessStartContent.h"
#include "CCMenuLambda.h"
#include "KSUtil.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "CommonButton.h"
#include "FlagSelector.h"
#include "CommonAnimation.h"
EndlessStartContent* EndlessStartContent::create(int t_touch_priority, function<void(CCObject*)> t_selector)
{
	EndlessStartContent* t_ctc = new EndlessStartContent();
	t_ctc->myInit(t_touch_priority, t_selector);
	t_ctc->autorelease();
	return t_ctc;
}

//void ControlTipContent::completedAnimationSequenceNamed (char const * name)
//{
//	string t_name = name;
//	
//	if(t_name == "Default Timeline")
//	{
//		if(!close_button->isVisible())
//		{
//			close_button->setVisible(true);
//			is_menu_enable = true;
//		}
//		
//		addChild(KSTimer::create(0.5f, [=]()
//								 {
//									 ccb_manager->runAnimationsForSequenceNamed("Default Timeline");
//								 }));
//	}
//}

void EndlessStartContent::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	CommonAnimation::closePopup(this, this, nullptr, [=](){
		
	}, [=](){
		end_selector(NULL);
		getParent()->removeFromParent();
	});
}

void EndlessStartContent::myInit(int t_touch_priority, function<void(CCObject*)> t_selector)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_selector = t_selector;
	
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	left_back->setContentSize(CCSizeMake(180, 120));
	left_back->setPosition(ccp(-120, 0));
	addChild(left_back);
	
	CCScale9Sprite* left_flag_back = CCScale9Sprite::create("mainpopup_pupple4.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_flag_back->setContentSize(CCSizeMake(60, 45));
	left_flag_back->setPosition(ccp(left_back->getContentSize().width/2.f, 88));
	left_back->addChild(left_flag_back);
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	CCSprite* left_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(myDSH->getStringForKey(kDSH_Key_flag)).c_str());
	left_flag->setScale(1.2f);
	left_flag->setPosition(ccp(left_flag_back->getContentSize().width/2.f, left_flag_back->getContentSize().height/2.f));
	left_flag_back->addChild(left_flag);
	
	CCScale9Sprite* left_bottom_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	left_bottom_back->setContentSize(CCSizeMake(left_back->getContentSize().width-15, 55));
	left_bottom_back->setPosition(ccp(left_back->getContentSize().width/2.f, 36));
	left_back->addChild(left_bottom_back);
	
	KSLabelTTF* left_nick = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 15);
	left_nick->enableOuterStroke(ccBLACK, 1.f);
	left_nick->setPosition(ccp(left_bottom_back->getContentSize().width/2.f, left_bottom_back->getContentSize().height-18));
	left_bottom_back->addChild(left_nick);
	
//	KSLabelTTF* left_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalLevel), mySGD->endless_my_level.getV())->getCString(), mySGD->getFont().c_str(), 12);
//	left_level->setPosition(ccp(left_bottom_back->getContentSize().width/2.f, left_bottom_back->getContentSize().height-38));
//	left_bottom_back->addChild(left_level);
	
	KSLabelTTF* left_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalIngWin), mySGD->endless_my_victory.getV())->getCString(), mySGD->getFont().c_str(), 15);
	left_ing_win->setColor(ccc3(255,170,20));
	left_ing_win->setPosition(ccp(left_bottom_back->getContentSize().width/2.f, left_bottom_back->getContentSize().height-38));
	left_bottom_back->addChild(left_ing_win);
	
//	int win_count = mySGD->endless_my_win.getV();
//	int lose_count = mySGD->endless_my_lose.getV();
//	int win_rate = 100.f*win_count/(win_count + lose_count);
//	KSLabelTTF* left_info_score = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessInfoScoreValue), win_count, lose_count, win_rate)->getCString(), mySGD->getFont().c_str(), 18);
//	left_info_score->enableOuterStroke(ccBLACK, 1.f);
//	left_info_score->setColor(ccYELLOW);
//	left_info_score->setPosition(ccp(left_bottom_back->getContentSize().width/2.f, 22));
//	left_bottom_back->addChild(left_info_score);
	
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_back->setContentSize(CCSizeMake(180, 120));
	right_back->setPosition(ccp(120, 0));
	addChild(right_back);
	
	CCScale9Sprite* right_flag_back = CCScale9Sprite::create("mainpopup_pupple4.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_flag_back->setContentSize(CCSizeMake(60, 45));
	right_flag_back->setPosition(ccp(right_back->getContentSize().width/2.f, 88));
	right_back->addChild(right_flag_back);
	
	CCSprite* right_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(mySGD->endless_flag.getV()).c_str());
	right_flag->setScale(1.2f);
	right_flag->setPosition(ccp(right_flag_back->getContentSize().width/2.f, right_flag_back->getContentSize().height/2.f));
	right_flag_back->addChild(right_flag);
	
	CCScale9Sprite* right_bottom_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_bottom_back->setContentSize(CCSizeMake(right_back->getContentSize().width-15, 55));
	right_bottom_back->setPosition(ccp(right_back->getContentSize().width/2.f, 36));
	right_back->addChild(right_bottom_back);
	
	KSLabelTTF* right_nick = KSLabelTTF::create(mySGD->endless_nick.getV().c_str(), mySGD->getFont().c_str(), 15);
	right_nick->enableOuterStroke(ccBLACK, 1.f);
	right_nick->setPosition(ccp(right_bottom_back->getContentSize().width/2.f, right_bottom_back->getContentSize().height-18));
	right_bottom_back->addChild(right_nick);

//	KSLabelTTF* right_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalLevel), mySGD->endless_level.getV())->getCString(), mySGD->getFont().c_str(), 12);
//	right_level->setPosition(ccp(right_bottom_back->getContentSize().width/2.f, right_bottom_back->getContentSize().height-38));
//	right_bottom_back->addChild(right_level);
	
	KSLabelTTF* right_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rivalIngWin), mySGD->endless_victory.getV())->getCString(), mySGD->getFont().c_str(), 15);
	right_ing_win->setColor(ccc3(255,170,20));
	right_ing_win->setPosition(ccp(right_bottom_back->getContentSize().width/2.f, right_bottom_back->getContentSize().height-38));
	right_bottom_back->addChild(right_ing_win);
	
//	int right_win_count = mySGD->replay_playing_info.get(mySGD->getReplayKey(kReplayKey_win), Json::Value()).asInt();
//	int right_lose_count = mySGD->replay_playing_info.get(mySGD->getReplayKey(kReplayKey_lose), Json::Value()).asInt();
//	int right_win_rate = 100.f*right_win_count/(right_win_count + right_lose_count);
//	KSLabelTTF* right_info_score = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessInfoScoreValue), right_win_count, right_lose_count, right_win_rate)->getCString(), mySGD->getFont().c_str(), 18);
//	right_info_score->enableOuterStroke(ccBLACK, 1.f);
//	right_info_score->setColor(ccYELLOW);
//	right_info_score->setPosition(ccp(right_bottom_back->getContentSize().width/2.f, 22));
//	right_bottom_back->addChild(right_info_score);
	
	
	
	CCSprite* vs_img = CCSprite::create("endless_vs.png");
	vs_img->setPosition(ccp(0,0));
	addChild(vs_img);
	
	
	CCSprite* n_close = CCSprite::create("whitePaper.png");
	n_close->setOpacity(0);
	CCSprite* s_close = CCSprite::create("whitePaper.png");
	s_close->setOpacity(0);
	
	CCMenuItemSprite* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(EndlessStartContent::menuAction));
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setTouchPriority(touch_priority-1);
	close_menu->setPosition(ccp(0,0));
	addChild(close_menu);
	
	
	CommonAnimation::openPopup(this, this, nullptr, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
	
//	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t)
//										 {
//											 
//										 }, [=](int t)
//										 {
//											 
//										 }));
}