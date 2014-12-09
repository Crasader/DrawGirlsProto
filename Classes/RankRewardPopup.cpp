//
//  RankRewardPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 6. 10..
//
//

#include "RankRewardPopup.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "AudioEngine.h"
#include "StyledLabelTTF.h"
#include "FormSetter.h"
#include "MyLocalization.h"
#include "CommonAnimation.h"
#include "StyledLabelTTF.h"
#include "LabelTTFMarquee.h"

RankRewardPopup* RankRewardPopup::create(int t_touch_priority, function<void()> t_end_func)
{
	RankRewardPopup* t_mup = new RankRewardPopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void RankRewardPopup::myInit(int t_touch_priority, function<void()> t_end_func)
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
	
	back_case = CCSprite::create("mainpopup2_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	
	CCScale9Sprite* title_back = CCScale9Sprite::create("title_tab.png", CCRectMake(0, 0, 90, 35), CCRectMake(44, 17, 2, 1));
	title_back->setContentSize(CCSizeMake(130, 35));
	title_back->setPosition(ccp(80,back_case->getContentSize().height-10));
	back_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardTitle), mySGD->getFont().c_str(), 15);
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);
	
	
	CCScale9Sprite* tip_marquee_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	tip_marquee_back->setContentSize(CCSizeMake(278, 26));
	tip_marquee_back->setPosition(ccp(back_case->getContentSize().width*0.655f, back_case->getContentSize().height+2-23.5f));
	back_case->addChild(tip_marquee_back);
	
	LabelTTFMarquee* tipMaquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 278, 22, "");
	tipMaquee->addText(ccsf("<font size=12>%s</font>", myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardContent)));
	tipMaquee->setPosition(ccpFromSize(tip_marquee_back->getContentSize()/2.f));
	tipMaquee->startMarquee();
	tipMaquee->setAnchorPoint(ccp(0.5f,0.5f));
	tip_marquee_back->addChild(tipMaquee);
	
	CCSprite* tipBack = CCSprite::create("tabbutton_up.png");
	tipBack->setPosition(ccp(back_case->getContentSize().width*0.417f, back_case->getContentSize().height+2-25));
	back_case->addChild(tipBack);
	KSLabelTTF* tipLbl = KSLabelTTF::create("TIP", mySGD->getFont().c_str(), 14.f);
	//	tipLbl->disableOuterStroke();
	tipLbl->setPosition(ccpFromSize(tipBack->getContentSize()) / 2.f + ccp(0,1));
	tipBack->addChild(tipLbl);
	
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccpFromSize(back_case->getContentSize()) + ccp(-20,-12));
	back_case->addChild(close_button);
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  mySGD->resetRankReward();
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
								  
								  CommonAnimation::closePopup(this, m_container, gray, [=](){
									  
								  }, [=](){
									  end_func(); removeFromParent();
								  });
							  });
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	left_back->setContentSize(CCSizeMake(210, 205));
	left_back->setPosition(ccp(back_case->getContentSize().width/2.f-109,123.5));
	back_case->addChild(left_back);
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	right_back->setContentSize(CCSizeMake(210, 205));
	right_back->setPosition(ccp(back_case->getContentSize().width/2.f+109,123.5));
	back_case->addChild(right_back);
	
	KSLabelTTF* left_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageTitle), mySGD->getFont().c_str(), 13);
	left_title->setAnchorPoint(ccp(0,0.5f));
	left_title->setPosition(ccp(15, 191));
	left_back->addChild(left_title);
	
	KSLabelTTF* right_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardEndlessTitle), mySGD->getFont().c_str(), 13);
	right_title->setAnchorPoint(ccp(0,0.5f));
	right_title->setPosition(ccp(15, 191));
	right_back->addChild(right_title);
	
	CCScale9Sprite* left_graph_back = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	left_graph_back->setContentSize(CCSizeMake(202, 45));
	left_graph_back->setPosition(ccp(left_back->getContentSize().width/2.f, 157.5));
	left_back->addChild(left_graph_back);
	
	CCSprite* left_graph_case = CCSprite::create("ending_graph.png");//"rankreward_graph.png");
	left_graph_case->setPosition(ccp(left_graph_back->getContentSize().width/2.f, 30));
	left_graph_back->addChild(left_graph_case);
	
	Json::Value stage_reward_list = mySGD->rank_reward_data["stage"]["rewardList"];
	
//	Json::Value stage_top10_reward = stage_reward_list["t10"]["reward"];
//	if(stage_top10_reward.size() >= 2)
//	{
//		CCNode* reward_img = getRewardImg("many");
//		reward_img->setPosition(ccp(13.5, left_graph_case->getContentSize().height/2.f));
//		left_graph_case->addChild(reward_img);
//	}
//	else
//	{
//		CCNode* reward_img = getRewardImg(stage_top10_reward[0]["type"].asString());
//		reward_img->setPosition(ccp(13.5, left_graph_case->getContentSize().height/2.f));
//		left_graph_case->addChild(reward_img);
//		
//		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(stage_top10_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
//		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
//		left_graph_case->addChild(count_label);
//	}
//	
//	Json::Value stage_p30_reward = stage_reward_list["p30"]["reward"];
//	if(stage_p30_reward.size() >= 2)
//	{
//		CCNode* reward_img = getRewardImg("many");
//		reward_img->setPosition(ccp(49.5, left_graph_case->getContentSize().height/2.f));
//		left_graph_case->addChild(reward_img);
//	}
//	else
//	{
//		CCNode* reward_img = getRewardImg(stage_p30_reward[0]["type"].asString());
//		reward_img->setPosition(ccp(49.5, left_graph_case->getContentSize().height/2.f));
//		left_graph_case->addChild(reward_img);
//		
//		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(stage_p30_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
//		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
//		left_graph_case->addChild(count_label);
//	}
//	
//	Json::Value stage_p50_reward = stage_reward_list["p50"]["reward"];
//	if(stage_p50_reward.size() >= 2)
//	{
//		CCNode* reward_img = getRewardImg("many");
//		reward_img->setPosition(ccp(92, left_graph_case->getContentSize().height/2.f));
//		left_graph_case->addChild(reward_img);
//	}
//	else
//	{
//		CCNode* reward_img = getRewardImg(stage_p50_reward[0]["type"].asString());
//		reward_img->setPosition(ccp(92, left_graph_case->getContentSize().height/2.f));
//		left_graph_case->addChild(reward_img);
//		
//		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(stage_p50_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
//		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
//		left_graph_case->addChild(count_label);
//	}
//	
//	Json::Value stage_p100_reward = stage_reward_list["p100"]["reward"];
//	if(stage_p100_reward.size() >= 2)
//	{
//		CCNode* reward_img = getRewardImg("many");
//		reward_img->setPosition(ccp(150, left_graph_case->getContentSize().height/2.f));
//		left_graph_case->addChild(reward_img);
//	}
//	else
//	{
//		CCNode* reward_img = getRewardImg(stage_p100_reward[0]["type"].asString());
//		reward_img->setPosition(ccp(150, left_graph_case->getContentSize().height/2.f));
//		left_graph_case->addChild(reward_img);
//		
//		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(stage_p100_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
//		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
//		left_graph_case->addChild(count_label);
//	}
	
	if(mySGD->rank_reward_data["stage"]["alluser"].asInt() == 0)
		mySGD->rank_reward_data["stage"]["alluser"] = 1;
	float stage_rank_percent = 1.f*mySGD->rank_reward_data["stage"]["myrank"].asInt()/mySGD->rank_reward_data["stage"]["alluser"].asInt();
	
	CCSprite* stage_percent_case = CCSprite::create("gameresult_rank_percent.png");
	stage_percent_case->setAnchorPoint(ccp(0.5,1));
	stage_percent_case->setPosition(left_back->getPosition() + ccpFromSize(left_back->getContentSize()/(-2.f)) + left_graph_back->getPosition() + ccpFromSize(left_graph_back->getContentSize()/(-2.f)) +
									left_graph_case->getPosition() + ccpFromSize(left_graph_case->getContentSize()/(-2.f)) + ccp(left_graph_case->getContentSize().width, left_graph_case->getContentSize().height/2.f) + ccp(0,-1));
	back_case->addChild(stage_percent_case, 1);
	
	KSLabelTTF* stage_percent_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", ceilf(stage_rank_percent*100.f))->getCString(), mySGD->getFont().c_str(), 13);
	stage_percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	stage_percent_label->setPosition(ccpFromSize(stage_percent_case->getContentSize()/2.f) + ccp(0,-3));
	stage_percent_case->addChild(stage_percent_label);
	
	
	CCScale9Sprite* left_first_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	left_first_back->setContentSize(CCSizeMake(202, 21));
	left_first_back->setPosition(ccp(left_back->getContentSize().width/2.f, 122));
	left_back->addChild(left_first_back);
	
	KSLabelTTF* left_first_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageScore), mySGD->getFont().c_str(), 11);
	left_first_title->setAnchorPoint(ccp(0,0.5));
	left_first_title->setPosition(ccp(10, left_first_back->getContentSize().height/2.f));
	left_first_back->addChild(left_first_title);
	
	KSLabelTTF* left_first_content = KSLabelTTF::create(KS::insert_separator(mySGD->rank_reward_data["stage"]["score"].asInt()).c_str(), mySGD->getFont().c_str(), 13);
	left_first_content->setColor(ccc3(255, 170, 20));
	left_first_content->setAnchorPoint(ccp(1,0.5));
	left_first_content->setPosition(ccp(left_first_back->getContentSize().width-10, left_first_back->getContentSize().height/2.f));
	left_first_back->addChild(left_first_content);
	
	
	CCScale9Sprite* left_second_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	left_second_back->setContentSize(CCSizeMake(202, 21));
	left_second_back->setPosition(ccp(left_back->getContentSize().width/2.f, 99.5));
	left_back->addChild(left_second_back);
	
	KSLabelTTF* left_second_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageRank), mySGD->getFont().c_str(), 11);
	left_second_title->setAnchorPoint(ccp(0,0.5));
	left_second_title->setPosition(ccp(10, left_second_back->getContentSize().height/2.f));
	left_second_back->addChild(left_second_title);
	
	KSLabelTTF* left_second_content = KSLabelTTF::create((KS::insert_separator(mySGD->rank_reward_data["stage"]["myrank"].asInt()) + CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageRankValue), ceilf(stage_rank_percent*100.f))->getCString()).c_str(), mySGD->getFont().c_str(), 13);
	left_second_content->setColor(ccc3(255, 170, 20));
	left_second_content->setAnchorPoint(ccp(1,0.5));
	left_second_content->setPosition(ccp(left_second_back->getContentSize().width-10, left_second_back->getContentSize().height/2.f));
	left_second_back->addChild(left_second_content);
	
	
	CCScale9Sprite* left_third_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	left_third_back->setContentSize(CCSizeMake(202, 21));
	left_third_back->setPosition(ccp(left_back->getContentSize().width/2.f, 77));
	left_back->addChild(left_third_back);
	
	KSLabelTTF* left_third_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStagePlayCount), mySGD->getFont().c_str(), 11);
	left_third_title->setAnchorPoint(ccp(0,0.5));
	left_third_title->setPosition(ccp(10, left_third_back->getContentSize().height/2.f));
	left_third_back->addChild(left_third_title);
	
	KSLabelTTF* left_third_content = KSLabelTTF::create(KS::insert_separator(mySGD->rank_reward_data["stage"]["count"].asInt()).c_str(), mySGD->getFont().c_str(), 13);
	left_third_content->setColor(ccc3(255, 170, 20));
	left_third_content->setAnchorPoint(ccp(1,0.5));
	left_third_content->setPosition(ccp(left_third_back->getContentSize().width-10, left_third_back->getContentSize().height/2.f));
	left_third_back->addChild(left_third_content);
	
	
	CCScale9Sprite* left_reward_back = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	left_reward_back->setContentSize(CCSizeMake(202, 60));
	left_reward_back->setPosition(ccp(left_back->getContentSize().width/2.f, 34));
	left_back->addChild(left_reward_back);
	
	Json::Value stage_take_reward_list = mySGD->rank_reward_data["reward"];
	if(stage_take_reward_list.size() >= 2)
	{
		CCNode* reward_img = getTakeRewardImg("many");
		reward_img->setPosition(ccp(40, left_reward_back->getContentSize().height/2.f));
		left_reward_back->addChild(reward_img);
		
		CCNode* script_node = getTakeRewardScript(stage_take_reward_list);
		script_node->setPosition(ccp(130, left_reward_back->getContentSize().height/2.f));
		left_reward_back->addChild(script_node);
	}
	else
	{
		CCNode* reward_img = getTakeRewardImg(stage_take_reward_list[0]["type"].asString());
		reward_img->setPosition(ccp(40, left_reward_back->getContentSize().height/2.f));
		left_reward_back->addChild(reward_img);
		
		CCNode* script_node = getTakeRewardScript(stage_take_reward_list);
		script_node->setPosition(ccp(130, left_reward_back->getContentSize().height/2.f));
		left_reward_back->addChild(script_node);
	}
	
	CCScale9Sprite* right_graph_back = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	right_graph_back->setContentSize(CCSizeMake(202, 45));
	right_graph_back->setPosition(ccp(right_back->getContentSize().width/2.f, 157.5));
	right_back->addChild(right_graph_back);
	
	CCSprite* right_graph_case = CCSprite::create("ending_graph.png");// rankreward_graph.png");
	right_graph_case->setPosition(ccp(right_graph_back->getContentSize().width/2.f, 30));
	right_graph_back->addChild(right_graph_case);
	
	Json::Value endless_reward_list = mySGD->rank_reward_data["endless"]["rewardList"];
	
//	Json::Value endless_top10_reward = endless_reward_list["t10"]["reward"];
//	if(endless_top10_reward.size() >= 2)
//	{
//		CCNode* reward_img = getRewardImg("many");
//		reward_img->setPosition(ccp(13.5, right_graph_case->getContentSize().height/2.f));
//		right_graph_case->addChild(reward_img);
//	}
//	else
//	{
//		CCNode* reward_img = getRewardImg(endless_top10_reward[0]["type"].asString());
//		reward_img->setPosition(ccp(13.5, right_graph_case->getContentSize().height/2.f));
//		right_graph_case->addChild(reward_img);
//		
//		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(endless_top10_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
//		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
//		right_graph_case->addChild(count_label);
//	}
//	
//	Json::Value endless_p30_reward = endless_reward_list["p30"]["reward"];
//	if(endless_p30_reward.size() >= 2)
//	{
//		CCNode* reward_img = getRewardImg("many");
//		reward_img->setPosition(ccp(49.5, right_graph_case->getContentSize().height/2.f));
//		right_graph_case->addChild(reward_img);
//	}
//	else
//	{
//		CCNode* reward_img = getRewardImg(endless_p30_reward[0]["type"].asString());
//		reward_img->setPosition(ccp(49.5, right_graph_case->getContentSize().height/2.f));
//		right_graph_case->addChild(reward_img);
//		
//		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(endless_p30_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
//		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
//		right_graph_case->addChild(count_label);
//	}
//	
//	Json::Value endless_p50_reward = endless_reward_list["p50"]["reward"];
//	if(endless_p50_reward.size() >= 2)
//	{
//		CCNode* reward_img = getRewardImg("many");
//		reward_img->setPosition(ccp(92, right_graph_case->getContentSize().height/2.f));
//		right_graph_case->addChild(reward_img);
//	}
//	else
//	{
//		CCNode* reward_img = getRewardImg(endless_p50_reward[0]["type"].asString());
//		reward_img->setPosition(ccp(92, right_graph_case->getContentSize().height/2.f));
//		right_graph_case->addChild(reward_img);
//		
//		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(endless_p50_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
//		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
//		right_graph_case->addChild(count_label);
//	}
//	
//	Json::Value endless_p100_reward = endless_reward_list["p100"]["reward"];
//	if(endless_p100_reward.size() >= 2)
//	{
//		CCNode* reward_img = getRewardImg("many");
//		reward_img->setPosition(ccp(150, right_graph_case->getContentSize().height/2.f));
//		right_graph_case->addChild(reward_img);
//	}
//	else
//	{
//		CCNode* reward_img = getRewardImg(endless_p100_reward[0]["type"].asString());
//		reward_img->setPosition(ccp(150, right_graph_case->getContentSize().height/2.f));
//		right_graph_case->addChild(reward_img);
//		
//		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(endless_p100_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
//		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
//		right_graph_case->addChild(count_label);
//	}
	
	if(mySGD->rank_reward_data["endless"]["alluser"].asInt() == 0)
		mySGD->rank_reward_data["endless"]["alluser"] = 1;
	float endless_rank_percent = 1.f*mySGD->rank_reward_data["endless"]["myrank"].asInt()/mySGD->rank_reward_data["endless"]["alluser"].asInt();
	
	CCSprite* endless_percent_case = CCSprite::create("gameresult_rank_percent.png");
	endless_percent_case->setAnchorPoint(ccp(0.5,1));
	endless_percent_case->setPosition(right_back->getPosition() + ccpFromSize(right_back->getContentSize()/(-2.f)) + right_graph_back->getPosition() + ccpFromSize(right_graph_back->getContentSize()/(-2.f)) +
									  right_graph_case->getPosition() + ccpFromSize(right_graph_case->getContentSize()/(-2.f)) + ccp(right_graph_case->getContentSize().width, right_graph_case->getContentSize().height/2.f) + ccp(0,-1));
	back_case->addChild(endless_percent_case);
	
	KSLabelTTF* endless_percent_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", ceilf(endless_rank_percent*100.f))->getCString(), mySGD->getFont().c_str(), 13);
	endless_percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	endless_percent_label->setPosition(ccpFromSize(endless_percent_case->getContentSize()/2.f) + ccp(0,-3));
	endless_percent_case->addChild(endless_percent_label);
	
	
	CCScale9Sprite* right_first_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	right_first_back->setContentSize(CCSizeMake(202, 21));
	right_first_back->setPosition(ccp(right_back->getContentSize().width/2.f, 122));
	right_back->addChild(right_first_back);
	
	KSLabelTTF* right_first_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardEndlessScore), mySGD->getFont().c_str(), 11);
	right_first_title->setAnchorPoint(ccp(0,0.5));
	right_first_title->setPosition(ccp(10, right_first_back->getContentSize().height/2.f));
	right_first_back->addChild(right_first_title);
	
	KSLabelTTF* right_first_content = KSLabelTTF::create(KS::insert_separator(mySGD->rank_reward_data["endless"]["score"].asInt()).c_str(), mySGD->getFont().c_str(), 13);
	right_first_content->setColor(ccc3(255, 170, 20));
	right_first_content->setAnchorPoint(ccp(1,0.5));
	right_first_content->setPosition(ccp(right_first_back->getContentSize().width-10, right_first_back->getContentSize().height/2.f));
	right_first_back->addChild(right_first_content);
	
	
	CCScale9Sprite* right_second_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	right_second_back->setContentSize(CCSizeMake(202, 21));
	right_second_back->setPosition(ccp(right_back->getContentSize().width/2.f, 99.5));
	right_back->addChild(right_second_back);
	
	KSLabelTTF* right_second_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardEndlessRank), mySGD->getFont().c_str(), 11);
	right_second_title->setAnchorPoint(ccp(0,0.5));
	right_second_title->setPosition(ccp(10, right_second_back->getContentSize().height/2.f));
	right_second_back->addChild(right_second_title);
	
	KSLabelTTF* right_second_content = KSLabelTTF::create((KS::insert_separator(mySGD->rank_reward_data["endless"]["myrank"].asInt()) + CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardEndlessRankValue), ceilf(endless_rank_percent*100.f))->getCString()).c_str(), mySGD->getFont().c_str(), 13);
	right_second_content->setColor(ccc3(255, 170, 20));
	right_second_content->setAnchorPoint(ccp(1,0.5));
	right_second_content->setPosition(ccp(right_second_back->getContentSize().width-10, right_second_back->getContentSize().height/2.f));
	right_second_back->addChild(right_second_content);
	
	
	CCScale9Sprite* right_third_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	right_third_back->setContentSize(CCSizeMake(202, 21));
	right_third_back->setPosition(ccp(right_back->getContentSize().width/2.f, 77));
	right_back->addChild(right_third_back);
	
	KSLabelTTF* right_third_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardEndlessVictory), mySGD->getFont().c_str(), 11);
	right_third_title->setAnchorPoint(ccp(0,0.5));
	right_third_title->setPosition(ccp(10, right_third_back->getContentSize().height/2.f));
	right_third_back->addChild(right_third_title);
	
	KSLabelTTF* right_third_content = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardEndlessVictoryScript), KS::insert_separator(mySGD->rank_reward_data["endless"].get("victory", 0).asInt()).c_str())->getCString(), mySGD->getFont().c_str(), 13);
	right_third_content->setColor(ccc3(255, 170, 20));
	right_third_content->setAnchorPoint(ccp(1,0.5));
	right_third_content->setPosition(ccp(right_third_back->getContentSize().width-10, right_third_back->getContentSize().height/2.f));
	right_third_back->addChild(right_third_content);
	
	
	CCScale9Sprite* right_reward_back = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	right_reward_back->setContentSize(CCSizeMake(202, 60));
	right_reward_back->setPosition(ccp(right_back->getContentSize().width/2.f, 34));
	right_back->addChild(right_reward_back);
	
	Json::Value endless_take_reward_list = mySGD->rank_reward_data["reward"];
	if(endless_take_reward_list.size() >= 2)
	{
		CCNode* reward_img = getTakeRewardImg("many");
		reward_img->setPosition(ccp(40, right_reward_back->getContentSize().height/2.f));
		right_reward_back->addChild(reward_img);
		
		CCNode* script_node = getTakeRewardScript(endless_take_reward_list);
		script_node->setPosition(ccp(130, right_reward_back->getContentSize().height/2.f));
		right_reward_back->addChild(script_node);
	}
	else
	{
		CCNode* reward_img = getTakeRewardImg(endless_take_reward_list[0]["type"].asString());
		reward_img->setPosition(ccp(40, right_reward_back->getContentSize().height/2.f));
		right_reward_back->addChild(reward_img);
		
		CCNode* script_node = getTakeRewardScript(endless_take_reward_list);
		script_node->setPosition(ccp(130, right_reward_back->getContentSize().height/2.f));
		right_reward_back->addChild(script_node);
	}
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=]()
	{
		CCPoint stage_after_position;
		if(mySGD->rank_reward_data["stage"]["myrank"].asInt() <= 10)
		{
			if(mySGD->rank_reward_data["stage"]["alluser"].asInt() == 0)
				stage_after_position = stage_percent_case->getPosition() + ccp(-left_graph_case->getContentSize().width,0);
			else
				stage_after_position = stage_percent_case->getPosition() + ccp(-left_graph_case->getContentSize().width*(1.f-1.f*mySGD->rank_reward_data["stage"]["myrank"].asInt()/mySGD->rank_reward_data["stage"]["alluser"].asInt()), 0);
			stage_percent_label->setFontSize(12);
			stage_percent_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardRankValue), mySGD->rank_reward_data["stage"]["myrank"].asInt())->getCString());
		}
		else
		{
			if(mySGD->rank_reward_data["stage"]["alluser"].asInt() == 0)
				stage_after_position = stage_percent_case->getPosition() + ccp(-left_graph_case->getContentSize().width,0);
			else
				stage_after_position = stage_percent_case->getPosition() + ccp(-left_graph_case->getContentSize().width*(1.f-1.f*mySGD->rank_reward_data["stage"]["myrank"].asInt()/mySGD->rank_reward_data["stage"]["alluser"].asInt()), 0);
		}
		
		CCMoveTo* stage_move = CCMoveTo::create(0.5f, stage_after_position);
		stage_percent_case->runAction(stage_move);
		
		
		CCPoint endless_after_position;
		if(mySGD->rank_reward_data["endless"]["myrank"].asInt() <= 10)
		{
			if(mySGD->rank_reward_data["endless"]["alluser"].asInt() == 0)
				endless_after_position = endless_percent_case->getPosition() + ccp(-right_graph_case->getContentSize().width,0);
			else
				endless_after_position = endless_percent_case->getPosition() + ccp(-right_graph_case->getContentSize().width*(1.f-1.f*mySGD->rank_reward_data["endless"]["myrank"].asInt()/mySGD->rank_reward_data["endless"]["alluser"].asInt()), 0);
			endless_percent_label->setFontSize(12);
			endless_percent_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardRankValue), mySGD->rank_reward_data["endless"]["myrank"].asInt())->getCString());
		}
		else
		{
			if(mySGD->rank_reward_data["endless"]["alluser"].asInt() == 0)
				endless_after_position = endless_percent_case->getPosition() + ccp(-right_graph_case->getContentSize().width,0);
			else
				endless_after_position = endless_percent_case->getPosition() + ccp(-right_graph_case->getContentSize().width*(1.f-1.f*mySGD->rank_reward_data["endless"]["myrank"].asInt()/mySGD->rank_reward_data["endless"]["alluser"].asInt()), 0);
		}
		
		CCMoveTo* endless_move = CCMoveTo::create(0.5f, endless_after_position);
		endless_percent_case->runAction(endless_move);
		
		is_menu_enable = true;
	});
}

CCNode* RankRewardPopup::getTakeRewardScript(Json::Value reward_list)
{
	GoodsType t_type;
	if(reward_list.size() >= 2)
		t_type = mySGD->getGoodsKeyToType("many");
	else
		t_type = mySGD->getGoodsKeyToType(reward_list[0]["type"].asString());
	
	CCNode* return_node = NULL;
	if(t_type == kGoodsType_gold)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypeGold), KS::insert_separator(reward_list[0]["count"].asInt()).c_str())->getCString())->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_ruby)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypeRuby), KS::insert_separator(reward_list[0]["count"].asInt()).c_str())->getCString())->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_item9)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypeItem9))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_item6)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypeItem6))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_item11)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypeItem11))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass1)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass1))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass2)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass2))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass3)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass3))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass4)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass4))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass5)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass5))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass6)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass6))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass7)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass7))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass8)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass8))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass9)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass9))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass10)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass10))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass11)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass11))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass12)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypePass12))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(LK::kMyLocalKey_rankRewardGoodsTypeMany))->getCString(), mySGD->getFont().c_str(), 12);
	}
	
	return return_node;
}

CCNode* RankRewardPopup::getTakeRewardImg(string t_goods_str)
{
	GoodsType t_type = mySGD->getGoodsKeyToType(t_goods_str);
	
	CCNode* return_node = NULL;
	if(t_type == kGoodsType_gold)
	{
		return_node = CCSprite::create("rankpopup_top30_gold.png");
	}
	else if(t_type == kGoodsType_ruby)
	{
		return_node = CCSprite::create("rankpopup_top10_ruby.png");
	}
	else if(t_type == kGoodsType_item9)
	{
		return_node = CCSprite::create("item9.png");
	}
	else if(t_type == kGoodsType_item6)
	{
		return_node = CCSprite::create("item6.png");
	}
	else if(t_type == kGoodsType_item11)
	{
		return_node = CCSprite::create("item11.png");
	}
	else if(t_type == kGoodsType_pass1)
	{
		return_node = CCSprite::create("pass_ticket1.png");
	}
	else if(t_type == kGoodsType_pass2)
	{
		return_node = CCSprite::create("pass_ticket2.png");
	}
	else if(t_type == kGoodsType_pass3)
	{
		return_node = CCSprite::create("pass_ticket3.png");
	}
	else if(t_type == kGoodsType_pass4)
	{
		return_node = CCSprite::create("pass_ticket4.png");
	}
	else if(t_type == kGoodsType_pass5)
	{
		return_node = CCSprite::create("pass_ticket5.png");
	}
	else if(t_type == kGoodsType_pass6)
	{
		return_node = CCSprite::create("pass_ticket6.png");
	}
	else if(t_type == kGoodsType_pass7)
	{
		return_node = CCSprite::create("icon_p7.png");
	}
	else if(t_type == kGoodsType_pass8)
	{
		return_node = CCSprite::create("icon_p8.png");
	}
	else if(t_type == kGoodsType_pass9)
	{
		return_node = CCSprite::create("icon_p9.png");
	}
	else if(t_type == kGoodsType_pass10)
	{
		return_node = CCSprite::create("icon_p10.png");
	}
	else if(t_type == kGoodsType_pass11)
	{
		return_node = CCSprite::create("icon_p11.png");
	}
	else if(t_type == kGoodsType_pass12)
	{
		return_node = CCSprite::create("icon_p12.png");
	}
	else
	{
		return_node = CCSprite::create("rankpopup_top50_gift.png");
	}
	
	return return_node;
}

CCNode* RankRewardPopup::getRewardImg(string t_goods_str)
{
	GoodsType t_type = mySGD->getGoodsKeyToType(t_goods_str);
	
	CCNode* return_node = NULL;
	if(t_type == kGoodsType_gold)
	{
		return_node = CCSprite::create("price_gold_img.png");
	}
	else if(t_type == kGoodsType_ruby)
	{
		return_node = CCSprite::create("price_ruby_img.png");
	}
	else if(t_type == kGoodsType_item9)
	{
		return_node = CCSprite::create(CCString::createWithFormat("icon_%s.png", mySGD->getGoodsTypeToKey(t_type).c_str())->getCString());
	}
	else if(t_type == kGoodsType_item6)
	{
		return_node = CCSprite::create(CCString::createWithFormat("icon_%s.png", mySGD->getGoodsTypeToKey(t_type).c_str())->getCString());
	}
	else if(t_type == kGoodsType_item11)
	{
		return_node = CCSprite::create(CCString::createWithFormat("icon_%s.png", mySGD->getGoodsTypeToKey(t_type).c_str())->getCString());
	}
	else if(t_type == kGoodsType_pass1)
	{
		return_node = CCSprite::create("pass_ticket1.png");
	}
	else if(t_type == kGoodsType_pass2)
	{
		return_node = CCSprite::create("pass_ticket2.png");
	}
	else if(t_type == kGoodsType_pass3)
	{
		return_node = CCSprite::create("pass_ticket3.png");
	}
	else if(t_type == kGoodsType_pass4)
	{
		return_node = CCSprite::create("pass_ticket4.png");
	}
	else if(t_type == kGoodsType_pass5)
	{
		return_node = CCSprite::create("pass_ticket5.png");
	}
	else if(t_type == kGoodsType_pass6)
	{
		return_node = CCSprite::create("pass_ticket6.png");
	}
	else if(t_type == kGoodsType_pass7)
	{
		return_node = CCSprite::create("icon_p7.png");
	}
	else if(t_type == kGoodsType_pass8)
	{
		return_node = CCSprite::create("icon_p8.png");
	}
	else if(t_type == kGoodsType_pass9)
	{
		return_node = CCSprite::create("icon_p9.png");
	}
	else if(t_type == kGoodsType_pass10)
	{
		return_node = CCSprite::create("icon_p10.png");
	}
	else if(t_type == kGoodsType_pass11)
	{
		return_node = CCSprite::create("icon_p11.png");
	}
	else if(t_type == kGoodsType_pass12)
	{
		return_node = CCSprite::create("icon_p12.png");
	}
	else
	{
		return_node = CCSprite::create("price_package_img.png");
	}
	return_node->setScale(0.6f);
	
	return return_node;
}
