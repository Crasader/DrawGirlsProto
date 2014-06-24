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
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(480,320));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-15, 272));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, 143));
	back_case->addChild(back_in);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardTitle), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0.f,0.5f));
	title_label->setPosition(ccp(15,293));
	back_case->addChild(title_label);
	
	KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardContent), mySGD->getFont().c_str(), 10);
	content_label->setAnchorPoint(ccp(0,0.5f));
	content_label->setPosition(ccp(title_label->getContentSize().width+5, title_label->getContentSize().height/2.f));
	title_label->addChild(content_label);
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccpFromSize(back_case->getContentSize()) + ccp(-25,-25));
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
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	left_back->setContentSize(CCSizeMake(226, 260));
	left_back->setPosition(ccp(120,136.5));
	back_in->addChild(left_back);
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	right_back->setContentSize(CCSizeMake(226, 260));
	right_back->setPosition(ccp(345,136.5));
	back_in->addChild(right_back);
	
	KSLabelTTF* left_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageTitle), mySGD->getFont().c_str(), 15);
	left_title->setColor(ccc3(255, 170, 20));
	left_title->setPosition(ccp(left_back->getContentSize().width/2.f, 240));
	left_back->addChild(left_title);
	
	KSLabelTTF* right_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardEndlessTitle), mySGD->getFont().c_str(), 15);
	right_title->setColor(ccc3(255, 170, 20));
	right_title->setPosition(ccp(right_back->getContentSize().width/2.f, 240));
	right_back->addChild(right_title);
	
	CCScale9Sprite* left_graph_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_graph_back->setContentSize(CCSizeMake(213, 70));
	left_graph_back->setPosition(ccp(left_back->getContentSize().width/2.f, 193.5));
	left_back->addChild(left_graph_back);
	
	CCSprite* left_graph_case = CCSprite::create("ending_graph.png");//"rankreward_graph.png");
	left_graph_case->setPosition(ccp(left_graph_back->getContentSize().width/2.f, 21));
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
	
	float stage_rank_percent = 1.f*mySGD->rank_reward_data["stage"]["myrank"].asInt()/mySGD->rank_reward_data["stage"]["alluser"].asInt();
	
	CCSprite* stage_percent_case = CCSprite::create("gameresult_rank_percent.png");
	stage_percent_case->setAnchorPoint(ccp(0.5,0));
	stage_percent_case->setPosition(ccpFromSize(left_graph_case->getContentSize()) + ccp(0,-5));
	left_graph_case->addChild(stage_percent_case);
	
	KSLabelTTF* stage_percent_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", stage_rank_percent*100.f)->getCString(), mySGD->getFont().c_str(), 13);
	stage_percent_label->setColor(ccc3(255, 170, 20));
	stage_percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	stage_percent_label->setPosition(ccpFromSize(stage_percent_case->getContentSize()/2.f) + ccp(1,2));
	stage_percent_case->addChild(stage_percent_label);
	
	
	CCScale9Sprite* left_first_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_first_back->setContentSize(CCSizeMake(213, 35));
	left_first_back->setPosition(ccp(left_back->getContentSize().width/2.f, 145));
	left_back->addChild(left_first_back);
	
	KSLabelTTF* left_first_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageScore), mySGD->getFont().c_str(), 12);
	left_first_title->setAnchorPoint(ccp(0,0.5));
	left_first_title->setPosition(ccp(10, left_first_back->getContentSize().height/2.f));
	left_first_back->addChild(left_first_title);
	
	KSLabelTTF* left_first_content = KSLabelTTF::create(KS::insert_separator(mySGD->rank_reward_data["stage"]["score"].asInt()).c_str(), mySGD->getFont().c_str(), 12);
	left_first_content->setColor(ccc3(255, 170, 20));
	left_first_content->setAnchorPoint(ccp(1,0.5));
	left_first_content->setPosition(ccp(left_first_back->getContentSize().width-10, left_first_back->getContentSize().height/2.f));
	left_first_back->addChild(left_first_content);
	
	
	CCScale9Sprite* left_second_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_second_back->setContentSize(CCSizeMake(213, 35));
	left_second_back->setPosition(ccp(left_back->getContentSize().width/2.f, 117.5));
	left_back->addChild(left_second_back);
	
	KSLabelTTF* left_second_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageRank), mySGD->getFont().c_str(), 12);
	left_second_title->setAnchorPoint(ccp(0,0.5));
	left_second_title->setPosition(ccp(10, left_second_back->getContentSize().height/2.f));
	left_second_back->addChild(left_second_title);
	
	KSLabelTTF* left_second_content = KSLabelTTF::create((KS::insert_separator(mySGD->rank_reward_data["stage"]["myrank"].asInt()) + CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageRankValue), stage_rank_percent*100.f)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
	left_second_content->setColor(ccc3(255, 170, 20));
	left_second_content->setAnchorPoint(ccp(1,0.5));
	left_second_content->setPosition(ccp(left_second_back->getContentSize().width-10, left_second_back->getContentSize().height/2.f));
	left_second_back->addChild(left_second_content);
	
	
	CCScale9Sprite* left_third_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_third_back->setContentSize(CCSizeMake(213, 35));
	left_third_back->setPosition(ccp(left_back->getContentSize().width/2.f, 90));
	left_back->addChild(left_third_back);
	
	KSLabelTTF* left_third_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardStagePlayCount), mySGD->getFont().c_str(), 12);
	left_third_title->setAnchorPoint(ccp(0,0.5));
	left_third_title->setPosition(ccp(10, left_third_back->getContentSize().height/2.f));
	left_third_back->addChild(left_third_title);
	
	KSLabelTTF* left_third_content = KSLabelTTF::create(KS::insert_separator(mySGD->rank_reward_data["stage"]["count"].asInt()).c_str(), mySGD->getFont().c_str(), 12);
	left_third_content->setColor(ccc3(255, 170, 20));
	left_third_content->setAnchorPoint(ccp(1,0.5));
	left_third_content->setPosition(ccp(left_third_back->getContentSize().width-10, left_third_back->getContentSize().height/2.f));
	left_third_back->addChild(left_third_content);
	
	
	CCScale9Sprite* left_reward_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_reward_back->setContentSize(CCSizeMake(213, 70));
	left_reward_back->setPosition(ccp(left_back->getContentSize().width/2.f, 42));
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
	
	
	
	CCScale9Sprite* right_graph_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_graph_back->setContentSize(CCSizeMake(213, 70));
	right_graph_back->setPosition(ccp(right_back->getContentSize().width/2.f, 193.5));
	right_back->addChild(right_graph_back);
	
	CCSprite* right_graph_case = CCSprite::create("rankreward_graph.png");
	right_graph_case->setPosition(ccp(right_graph_back->getContentSize().width/2.f, 21));
	right_graph_back->addChild(right_graph_case);
	
	Json::Value endless_reward_list = mySGD->rank_reward_data["endless"]["rewardList"];
	
	Json::Value endless_top10_reward = endless_reward_list["t10"]["reward"];
	if(endless_top10_reward.size() >= 2)
	{
		CCNode* reward_img = getRewardImg("many");
		reward_img->setPosition(ccp(13.5, right_graph_case->getContentSize().height/2.f));
		right_graph_case->addChild(reward_img);
	}
	else
	{
		CCNode* reward_img = getRewardImg(endless_top10_reward[0]["type"].asString());
		reward_img->setPosition(ccp(13.5, right_graph_case->getContentSize().height/2.f));
		right_graph_case->addChild(reward_img);
		
		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(endless_top10_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
		right_graph_case->addChild(count_label);
	}
	
	Json::Value endless_p30_reward = endless_reward_list["p30"]["reward"];
	if(endless_p30_reward.size() >= 2)
	{
		CCNode* reward_img = getRewardImg("many");
		reward_img->setPosition(ccp(49.5, right_graph_case->getContentSize().height/2.f));
		right_graph_case->addChild(reward_img);
	}
	else
	{
		CCNode* reward_img = getRewardImg(endless_p30_reward[0]["type"].asString());
		reward_img->setPosition(ccp(49.5, right_graph_case->getContentSize().height/2.f));
		right_graph_case->addChild(reward_img);
		
		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(endless_p30_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
		right_graph_case->addChild(count_label);
	}
	
	Json::Value endless_p50_reward = endless_reward_list["p50"]["reward"];
	if(endless_p50_reward.size() >= 2)
	{
		CCNode* reward_img = getRewardImg("many");
		reward_img->setPosition(ccp(92, right_graph_case->getContentSize().height/2.f));
		right_graph_case->addChild(reward_img);
	}
	else
	{
		CCNode* reward_img = getRewardImg(endless_p50_reward[0]["type"].asString());
		reward_img->setPosition(ccp(92, right_graph_case->getContentSize().height/2.f));
		right_graph_case->addChild(reward_img);
		
		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(endless_p50_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
		right_graph_case->addChild(count_label);
	}
	
	Json::Value endless_p100_reward = endless_reward_list["p100"]["reward"];
	if(endless_p100_reward.size() >= 2)
	{
		CCNode* reward_img = getRewardImg("many");
		reward_img->setPosition(ccp(150, right_graph_case->getContentSize().height/2.f));
		right_graph_case->addChild(reward_img);
	}
	else
	{
		CCNode* reward_img = getRewardImg(endless_p100_reward[0]["type"].asString());
		reward_img->setPosition(ccp(150, right_graph_case->getContentSize().height/2.f));
		right_graph_case->addChild(reward_img);
		
		KSLabelTTF* count_label = KSLabelTTF::create(("+" + KS::insert_separator(endless_p100_reward[0]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 8);
		count_label->setPosition(reward_img->getPosition() + ccp(0,-5));
		right_graph_case->addChild(count_label);
	}
	
	float endless_rank_percent = 1.f*mySGD->rank_reward_data["endless"]["myrank"].asInt()/mySGD->rank_reward_data["endless"]["alluser"].asInt();
	
	CCSprite* endless_percent_case = CCSprite::create("gameresult_rank_percent.png");
	endless_percent_case->setAnchorPoint(ccp(0.5,0));
	endless_percent_case->setPosition(ccpFromSize(right_graph_case->getContentSize()) + ccp(0,-5));
	right_graph_case->addChild(endless_percent_case);
	
	KSLabelTTF* endless_percent_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", endless_rank_percent*100.f)->getCString(), mySGD->getFont().c_str(), 13);
	endless_percent_label->setColor(ccc3(255, 170, 20));
	endless_percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
	endless_percent_label->setPosition(ccpFromSize(endless_percent_case->getContentSize()/2.f) + ccp(1,2));
	endless_percent_case->addChild(endless_percent_label);
	
	
	CCScale9Sprite* right_first_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_first_back->setContentSize(CCSizeMake(213, 35));
	right_first_back->setPosition(ccp(right_back->getContentSize().width/2.f, 145));
	right_back->addChild(right_first_back);
	
	KSLabelTTF* right_first_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardEndlessScore), mySGD->getFont().c_str(), 12);
	right_first_title->setAnchorPoint(ccp(0,0.5));
	right_first_title->setPosition(ccp(10, right_first_back->getContentSize().height/2.f));
	right_first_back->addChild(right_first_title);
	
	KSLabelTTF* right_first_content = KSLabelTTF::create(KS::insert_separator(mySGD->rank_reward_data["endless"]["score"].asInt()).c_str(), mySGD->getFont().c_str(), 12);
	right_first_content->setColor(ccc3(255, 170, 20));
	right_first_content->setAnchorPoint(ccp(1,0.5));
	right_first_content->setPosition(ccp(right_first_back->getContentSize().width-10, right_first_back->getContentSize().height/2.f));
	right_first_back->addChild(right_first_content);
	
	
	CCScale9Sprite* right_second_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_second_back->setContentSize(CCSizeMake(213, 35));
	right_second_back->setPosition(ccp(right_back->getContentSize().width/2.f, 117.5));
	right_back->addChild(right_second_back);
	
	KSLabelTTF* right_second_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardEndlessRank), mySGD->getFont().c_str(), 12);
	right_second_title->setAnchorPoint(ccp(0,0.5));
	right_second_title->setPosition(ccp(10, right_second_back->getContentSize().height/2.f));
	right_second_back->addChild(right_second_title);
	
	KSLabelTTF* right_second_content = KSLabelTTF::create((KS::insert_separator(mySGD->rank_reward_data["endless"]["myrank"].asInt()) + CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardEndlessRankValue), endless_rank_percent*100.f)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
	right_second_content->setColor(ccc3(255, 170, 20));
	right_second_content->setAnchorPoint(ccp(1,0.5));
	right_second_content->setPosition(ccp(right_second_back->getContentSize().width-10, right_second_back->getContentSize().height/2.f));
	right_second_back->addChild(right_second_content);
	
	
	CCScale9Sprite* right_third_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_third_back->setContentSize(CCSizeMake(213, 35));
	right_third_back->setPosition(ccp(right_back->getContentSize().width/2.f, 90));
	right_back->addChild(right_third_back);
	
	KSLabelTTF* right_third_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankRewardEndlessVictory), mySGD->getFont().c_str(), 12);
	right_third_title->setAnchorPoint(ccp(0,0.5));
	right_third_title->setPosition(ccp(10, right_third_back->getContentSize().height/2.f));
	right_third_back->addChild(right_third_title);
	
	KSLabelTTF* right_third_content = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardEndlessVictoryScript), KS::insert_separator(mySGD->rank_reward_data["endless"]["count"].asInt()).c_str())->getCString(), mySGD->getFont().c_str(), 12);
	right_third_content->setColor(ccc3(255, 170, 20));
	right_third_content->setAnchorPoint(ccp(1,0.5));
	right_third_content->setPosition(ccp(right_third_back->getContentSize().width-10, right_third_back->getContentSize().height/2.f));
	right_third_back->addChild(right_third_content);
	
	
	CCScale9Sprite* right_reward_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_reward_back->setContentSize(CCSizeMake(213, 70));
	right_reward_back->setPosition(ccp(right_back->getContentSize().width/2.f, 42));
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
			stage_after_position = ccp(26.5f*(mySGD->rank_reward_data["stage"]["myrank"].asInt()/11.f), left_graph_case->getContentSize().height) + ccp(0,-5);
			stage_percent_label->setFontSize(12);
			stage_percent_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardRankValue), mySGD->rank_reward_data["stage"]["myrank"].asInt())->getCString());
		}
		else
		{
			stage_after_position = ccp(26.5f + (left_graph_case->getContentSize().width-26.5f)*mySGD->rank_reward_data["stage"]["myrank"].asInt()/(mySGD->rank_reward_data["stage"]["alluser"].asInt()-10), left_graph_case->getContentSize().height) + ccp(0,-5);
		}
		
		CCMoveTo* stage_move = CCMoveTo::create(0.5f, stage_after_position);
		stage_percent_case->runAction(stage_move);
		
		
		CCPoint endless_after_position;
		if(mySGD->rank_reward_data["endless"]["myrank"].asInt() <= 10)
		{
			endless_after_position = ccp(26.5f*(mySGD->rank_reward_data["endless"]["myrank"].asInt()/11.f), right_graph_case->getContentSize().height) + ccp(0,-5);
			endless_percent_label->setFontSize(12);
			endless_percent_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardRankValue), mySGD->rank_reward_data["endless"]["myrank"].asInt())->getCString());
		}
		else
		{
			endless_after_position = ccp(26.5f + (right_graph_case->getContentSize().width-26.5f)*mySGD->rank_reward_data["endless"]["myrank"].asInt()/(mySGD->rank_reward_data["endless"]["alluser"].asInt()-10), right_graph_case->getContentSize().height) + ccp(0,-5);
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
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypeGold), KS::insert_separator(reward_list[0]["count"].asInt()).c_str())->getCString())->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_ruby)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypeRuby), KS::insert_separator(reward_list[0]["count"].asInt()).c_str())->getCString())->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_item9)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypeItem9))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_item6)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypeItem6))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_item11)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypeItem11))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass1)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypePass1))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass2)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypePass2))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass3)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypePass3))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass4)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypePass4))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass5)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypePass5))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else if(t_type == kGoodsType_pass6)
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypePass6))->getCString(), mySGD->getFont().c_str(), 12);
	}
	else
	{
		return_node = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_rankRewardStageReward), myLoc->getLocalForKey(kMyLocalKey_rankRewardGoodsTypeMany))->getCString(), mySGD->getFont().c_str(), 12);
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
	else
	{
		return_node = CCSprite::create("price_package_img.png");
	}
	return_node->setScale(0.6f);
	
	return return_node;
}
