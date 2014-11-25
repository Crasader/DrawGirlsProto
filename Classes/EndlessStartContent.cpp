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
#include "TypingBox.h"
#include "StageImgLoader.h"

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
	
	
	CCSprite* left_back = CCSprite::create("endless_popup.png");
	left_back->setPosition(ccp(-110, 0));
	addChild(left_back);
	
	CCNode* left_character_node = CCNode::create();
	left_character_node->setScale(0.6f);
	left_character_node->setPosition(ccp(left_back->getContentSize().width/2.f, 75));
	left_back->addChild(left_character_node);
	
	auto left_character_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, mySGD->getSelectedCharacterHistory().characterIndex.getV()) + ".ccbi");
	CCSprite* left_character_img = left_character_ccb.first;
	left_character_img->setPosition(ccp(0,0));
	left_character_node->addChild(left_character_img);
	
//	left_character_ccb.second->runAnimationsForSequenceNamed("move_down");
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	CCSprite* left_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(myDSH->getStringForKey(kDSH_Key_flag)).c_str());
	left_flag->setScale(0.7f);
	left_flag->setPosition(ccp(left_back->getContentSize().width/2.f, 48));
	left_back->addChild(left_flag);
	
	KSLabelTTF* left_nick = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 13);
	left_nick->setPosition(ccp(left_back->getContentSize().width/2.f, 47));
	left_back->addChild(left_nick);
	
	left_flag->setPositionX(left_flag->getPositionX() - left_nick->getContentSize().width/2.f - 2);
	left_nick->setPositionX(left_nick->getPositionX() + left_flag->getContentSize().width/2.f*left_flag->getScale() + 2);
	
//	KSLabelTTF* left_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalLevel), mySGD->endless_my_level.getV())->getCString(), mySGD->getFont().c_str(), 12);
//	left_level->setPosition(ccp(left_bottom_back->getContentSize().width/2.f, left_bottom_back->getContentSize().height-38));
//	left_bottom_back->addChild(left_level);
	
	KSLabelTTF* left_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalIngWin), mySGD->endless_my_victory.getV())->getCString(), mySGD->getFont().c_str(), 20);
	left_ing_win->setColor(ccc3(0,215,220));
	left_ing_win->setPosition(ccp(left_back->getContentSize().width/2.f, 25));
	left_back->addChild(left_ing_win);
	
//	int win_count = mySGD->endless_my_win.getV();
//	int lose_count = mySGD->endless_my_lose.getV();
//	int win_rate = 100.f*win_count/(win_count + lose_count);
//	KSLabelTTF* left_info_score = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_endlessInfoScoreValue), win_count, lose_count, win_rate)->getCString(), mySGD->getFont().c_str(), 18);
//	left_info_score->enableOuterStroke(ccBLACK, 1.f);
//	left_info_score->setColor(ccYELLOW);
//	left_info_score->setPosition(ccp(left_bottom_back->getContentSize().width/2.f, 22));
//	left_bottom_back->addChild(left_info_score);
	
	
	CCSprite* right_back = CCSprite::create("endless_popup.png");
	right_back->setPosition(ccp(110, 0));
	addChild(right_back);
	
	int character_number = mySGD->endless_character_no.getV();
	int character_count = NSDS_GI(kSDS_GI_characterCount_i);
	int found_index = -1;
	for(int i=0;found_index == -1 && i<character_count;i++)
	{
		if(NSDS_GI(kSDS_GI_characterInfo_int1_no_i, i+1) == character_number)
		{
			found_index = i+1;
		}
	}
	
	if(found_index != -1)
	{
		CCNode* right_character_node = CCNode::create();
		right_character_node->setScale(0.6f);
		right_character_node->setPosition(ccp(right_back->getContentSize().width/2.f, 75));
		right_back->addChild(right_character_node);
		
		auto right_character_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, found_index) + ".ccbi");
		CCSprite* right_character_img = right_character_ccb.first;
		right_character_img->setPosition(ccp(0,0));
		right_character_node->addChild(right_character_img);
		
//		right_character_ccb.second->runAnimationsForSequenceNamed("move_down");
	}
	
	CCSprite* right_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(mySGD->endless_flag.getV()).c_str());
	right_flag->setScale(0.7f);
	right_flag->setPosition(ccp(right_back->getContentSize().width/2.f, 48));
	right_back->addChild(right_flag);
	
	KSLabelTTF* right_nick = KSLabelTTF::create(mySGD->endless_nick.getV().c_str(), mySGD->getFont().c_str(), 13);
	right_nick->setPosition(ccp(right_back->getContentSize().width/2.f, 47));
	right_back->addChild(right_nick);

	right_flag->setPositionX(right_flag->getPositionX() - right_nick->getContentSize().width/2.f - 2);
	right_nick->setPositionX(right_nick->getPositionX() + right_flag->getContentSize().width/2.f*right_flag->getScale() + 2);
	
//	KSLabelTTF* right_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalLevel), mySGD->endless_level.getV())->getCString(), mySGD->getFont().c_str(), 12);
//	right_level->setPosition(ccp(right_bottom_back->getContentSize().width/2.f, right_bottom_back->getContentSize().height-38));
//	right_bottom_back->addChild(right_level);
	
	KSLabelTTF* right_ing_win = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rivalIngWin), mySGD->endless_victory.getV())->getCString(), mySGD->getFont().c_str(), 20);
	right_ing_win->setColor(ccc3(0,215,220));
	right_ing_win->setPosition(ccp(right_back->getContentSize().width/2.f, 25));
	right_back->addChild(right_ing_win);
	
//	int right_win_count = mySGD->replay_playing_info.get(mySGD->getReplayKey(kReplayKey_win), Json::Value()).asInt();
//	int right_lose_count = mySGD->replay_playing_info.get(mySGD->getReplayKey(kReplayKey_lose), Json::Value()).asInt();
//	int right_win_rate = 100.f*right_win_count/(right_win_count + right_lose_count);
//	KSLabelTTF* right_info_score = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_endlessInfoScoreValue), right_win_count, right_lose_count, right_win_rate)->getCString(), mySGD->getFont().c_str(), 18);
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
	
	
	if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
	{
		CCNode* scenario_node = CCNode::create();
		addChild(scenario_node, 9999);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		
		CCNode* t_stencil_node = CCNode::create();
		
		
		CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
		t_clipping->setAlphaThreshold(0.1f);
		
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
		t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		
		t_clipping->setInverted(true);
		scenario_node->addChild(t_clipping, 0);
		
		
		CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
		ikaruga->setScale(1.f-((360-myDSH->ui_top)*3.f/16.f)/100.f);
		ikaruga->setAnchorPoint(ccp(0,0));
		ikaruga->setPosition(ccp(-240-ikaruga->getContentSize().width*ikaruga->getScale(), -myDSH->ui_center_y));
		scenario_node->addChild(ikaruga, 1);
		
		TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50)-ccp(240,myDSH->ui_center_y), ccp(480.f-56.f,32.5f)-ccp(240,myDSH->ui_center_y));
		typing_box->setHide();
		scenario_node->addChild(typing_box, 2);
		
		typing_box->showAnimation(0.3f);
		
		function<void()> end_func1 = [=]()
		{
			addChild(KSTimer::create(0.1f, [=]()
									 {
										 scenario_node->removeFromParent();
									 }));
		};
		
		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
															  {
																  ikaruga->setPositionX(-240-ikaruga->getContentSize().width*ikaruga->getScale() + ikaruga->getContentSize().width*ikaruga->getScale()*2.f/3.f*t);
															  }, [=](float t)
															  {
																  ikaruga->setPositionX(-240-ikaruga->getContentSize().width*ikaruga->getScale() + ikaruga->getContentSize().width*ikaruga->getScale()*2.f/3.f*t);
																  
																  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent60), end_func1);
															  }));
	}
}




